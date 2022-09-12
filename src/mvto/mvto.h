#pragma once

#include <unordered_map>
#include <set>
#include <map>
#include <cassert>
#include <algorithm>
#include <cstring>

#include "readwrite_lock.h"
#include "atomic_wrapper.h"
#include "./../protocols/common/schema.hpp"

using Rec = void;
using TxnID = uint64_t;
using TableID = uint64_t;


struct Version
{
    alignas(64) uint64_t read_ts;
    uint64_t write_ts;

    Version *prev;
    void *rec;
    bool deleted;

    // not atomic
    void update_read_ts(uint64_t ts)
    {
        uint64_t curret_ts = load_acquire(read_ts);
        if (ts < curret_ts) return;
        store_release(read_ts, ts);
    }
};

template <typename Version_>
struct Value
{
    using Version = Version_;
    ReadWriteLock rwl;
    Version *version;
    void initialize() 
    {
        rwl.initialize();
        version = nullptr;
    }

    void lock() { rwl.lock(); }
    void unlock() { rwl.unlock(); }
    bool is_empty() { return (version->prev == nullptr) && version->deleted; }
    bool is_detached_from_tree() { return version == nullptr; }

};

enum class ReadWriteType { READ = 0, INSERT, UPDATE, DELETE };

template <typename Value>
struct ReadWriteElement
{
    using Version = typename Value::Version;
    ReadWriteElement(void *rd_rec, void *wt_rec, ReadWriteType type, 
    bool is_new, Value *val)
    : read_rec(rd_rec)
    , write_rec(wt_rec)
    , type_(type)
    , is_new(is_new)
    , val(val) {}

    void *read_rec;
    void *write_rec;

    ReadWriteType type_ = ReadWriteType::READ;
    bool is_new;
    Value *val;

};


template <typename Key, typename Value>
class ReadWriteSet
{
  public:
    using Table = std::unordered_map<Key, ReadWriteElement<Value> >;
    Table &get_table(TableID table_id) { return rws[table_id]; }
  private:
    std::unordered_map<TableID, Table> rws;
};

template <typename Key, typename Value>
class WriteSet
{
  public:
    using Chain = std::pair<Key,typename std::unordered_map<Key, ReadWriteElement<Value> >::iterator >;
    std::vector<Chain> &get_table(TableID table_id) { return ws[table_id]; }
  private:
    std::unordered_map<TableID, std::vector<Chain>> ws;
};


template <typename Index>
class MVTO
{
    using Key = typename Index::Key;
    using Value = typename Index::Value;
    using Version = typename Value::Version;
    // using LeafNode = typename Index::LeafNode;
    // using NodeInfo = typename Index::NodeInfo;

  private:
    TxnID txn_id;
    uint64_t start_ts, smallest_ts, largest_ts;
    std::set<TableID> tables_;
    ReadWriteSet<Key, Value> rws;
    WriteSet<Key, Value> ws;

  public:


    MVTO(TxnID txn_id, uint64_t start_ts) 
        : txn_id(txn_id)
        , start_ts(start_ts) {}

    MVTO(TxnID txn_id, uint64_t start_ts, uint64_t smallest_ts, uint64_t largest_ts) 
        : txn_id(txn_id)
        , start_ts(start_ts)
        , smallest_ts(smallest_ts)
        , largest_ts(largest_ts) {}

    Rec *read(TableID table_id, Key key) 
    {
        Index& idx = Index::get_index();
        tables_.insert(table_id);
        auto &rw_table = rws.get_table(table_id);
        auto rw_it = rw_table.find(key);

        std::cout << "[mvto read] " << "after find " << std::endl;

        if (rw_it == rw_table.end())
        {
            std::cout << "[mvto read] " << "go to end " << std::endl;
            Value *val;
            typename Index::Result res = idx.find(table_id, key, val);
            if (res == Index::Result::NOT_FOUND) return nullptr;

            val->lock();
            // detacted from the tree
            if (val->is_detached_from_tree())
            {
                std::cout << "[mvto read] " << "detached from the tree" << std::endl;

                val->unlock();
                return nullptr;
            }
            // std::cout << "[mvto read] " << "go to end " << std::endl;

            if (val->is_empty())
            {
                std::cout << "[mvto read] " << " is empty " << std::endl;
                val->unlock();
                return nullptr;
            }
            // std::cout << "[mvto read] " << "go to end " << std::endl;

            Version *version = get_correct_version(val);
            if (version == nullptr)
            {
                std::cout << "[mvto read] " << "find nullptr" << std::endl;
                val->unlock();
                return nullptr;
            }

            // std::cout << "[mvto read] " << "go to end " << std::endl;

            version->update_read_ts(start_ts);
            val->unlock();

            if (version->deleted) return nullptr; // ?

            // std::cout << "[mvto read] " << "go to end " << std::endl;

            // into read write set
            rw_table.emplace_hint(rw_it, std::piecewise_construct, 
                std::forward_as_tuple(key), 
                std::forward_as_tuple(version->rec, nullptr, ReadWriteType::READ, false, val)
            );

            // std::cout << "[mvto read] " << "go to end " << std::endl;
            return version->rec;
        }

        std::cout << "[mvto read] " << "type" << std::endl;
        // if could find
        auto rw_type = rw_it->second.type_;
        if (rw_type == ReadWriteType::READ) return rw_it->second.read_rec;
        else if (rw_type == ReadWriteType::UPDATE || rw_type == ReadWriteType::INSERT) 
            return rw_it->second.write_rec;
        else if (rw_type ==  ReadWriteType::DELETE) return nullptr;
        else assert(0);
    }

    Rec *insert(TableID table_id, Key key) 
    {
        Index& idx = Index::get_index();
        // also need record size
        const Schema& sch = Schema::get_schema();
        size_t record_size = sch.get_record_size(table_id);

        tables_.insert(table_id);
        auto &rw_table = rws.get_table(table_id);
        auto rw_it = rw_table.find(key);

        if (rw_it == rw_table.end())
        {
            Value *val;
            typename Index::Result res = idx.find(table_id, key, val);

            if (res == Index::Result::OK) {

                val->lock();
                if (val->is_detached_from_tree())
                {
                    val->unlock();
                    return nullptr;
                }
                if (val->is_empty())
                {
                    val->unlock();
                    return nullptr;
                }

                Version *head_version = val->version;
                Version *version = get_correct_version(val);
                // gc
                gc_version_chain(val);
                if (version == nullptr )
                {
                    val->unlock();
                    return nullptr;
                }
                version->update_read_ts(start_ts);
                val->unlock();
                if ( ! (head_version == version && version->deleted ))
                    return nullptr;
                Rec *rec = malloc(record_size);

                auto new_iter = rw_table.emplace_hint(
                    rw_it, std::piecewise_construct,
                    std::forward_as_tuple(key),
                    std::forward_as_tuple(nullptr, rec, ReadWriteType::INSERT, false, val)
                );
                auto &w_table = ws.get_table(table_id);
                w_table.emplace_back(key, new_iter);
                return rec;
            }

            // create new val to the write set
            Value *new_val = reinterpret_cast<Value *>(malloc(sizeof(Value)));
            new_val->initialize();
            Version *version = reinterpret_cast<Version *>(malloc(sizeof(Version)));
            Rec *rec = malloc(sizeof(record_size));
            new_val->version = version;
            version->read_ts = start_ts;
            version->write_ts = start_ts;
            version->prev = nullptr;
            version->rec = rec;
            version->deleted = false;

            auto new_iter = rw_table.emplace_hint(
                rw_it, std::piecewise_construct,
                std::forward_as_tuple(key),
                std::forward_as_tuple(nullptr, rec, ReadWriteType::INSERT, true, new_val)
            );
            auto &w_table = ws.get_table(table_id);
            w_table.emplace_back(key, new_iter);
            return rec;
        }

        // if could find
        auto rw_type = rw_it->second.type_;
        if (rw_type ==  ReadWriteType::READ ||
        rw_type == ReadWriteType::UPDATE || rw_type == ReadWriteType::INSERT) 
        {
            return nullptr;
        }
        else if (rw_type == ReadWriteType::DELETE) 
        {
            rw_it->second.type_ = ReadWriteType::UPDATE;
            return rw_it->second.write_rec;
        }
        else assert(0);

    }

    Rec *update(TableID table_id, Key key)
    {
        std::cout << "[mvto update] start" << std::endl;
        Index& idx = Index::get_index();
        // also need record size
        const Schema& sch = Schema::get_schema();
        std::cout << "[mvto update] after get schema" << table_id << std::endl;
        size_t record_size = sch.get_record_size(table_id);

        std::cout << "[mvto update] after find" << std::endl;
        tables_.insert(table_id);
        auto &rw_table = rws.get_table(table_id);
        auto rw_it = rw_table.find(key);
        std::cout << "[mvto update] after find" << std::endl;

        if (rw_it == rw_table.end())
        {
            Value *val;
            typename Index::Result res = idx.find(table_id, key, val);
            if (res == Index::Result::NOT_FOUND) return nullptr;

            val->lock();
            if (val->is_detached_from_tree())
            {
                val->unlock();
                return nullptr;
            }
            if (val->is_empty())
            {
                delete_from_tree(table_id, key, val);
                val->unlock();
                return nullptr;
            }

            Version *version = get_correct_version(val);
            // gc
            gc_version_chain(val);
            if (version == nullptr )
            {
                val->unlock();
                return nullptr;
            }
            version->update_read_ts(start_ts);
            val->unlock();

            Rec *rec = malloc(record_size);

            auto new_iter = rw_table.emplace_hint(
                rw_it, std::piecewise_construct,
                std::forward_as_tuple(key),
                std::forward_as_tuple(nullptr, rec, ReadWriteType::INSERT, false, val)
            );
            auto &w_table = ws.get_table(table_id);
            w_table.emplace_back(key, new_iter);
            return rec;
        }

        // if could find
        auto rw_type = rw_it->second.type_;
        if (rw_type == ReadWriteType::READ)
        {
            Rec *rec = malloc(record_size);
            memcpy(rec, rw_it->second.read_rec, record_size);
            rw_it->second.write_rec = rec;
            rw_it->second.type_ = ReadWriteType::UPDATE;

            auto &w_table = ws.get_table(table_id);
            w_table.emplace_back(key, rw_it);
            return rec;
        } 
        else if (rw_type == ReadWriteType::UPDATE || rw_type == ReadWriteType::INSERT) return rw_it->second.write_rec;
        else if (rw_type == ReadWriteType::DELETE) return nullptr;
        else assert(0);
    }


    Rec *upsert(TableID table_id, Key key)
    {
        Index& idx = Index::get_index();
        // also need record size
        const Schema& sch = Schema::get_schema();
        size_t record_size = sch.get_record_size(table_id);

        tables_.insert(table_id);
        auto &rw_table = rws.get_table(table_id);
        auto rw_it = rw_table.find(key);

        if (rw_it == rw_table.end())
        {
            Value *val;
            typename Index::Result res = idx.find(table_id, key, val);
            if (res == Index::Result::NOT_FOUND)
            {
                // create new val to the write set
                Value *new_val = reinterpret_cast<Value *>(malloc(sizeof(Value)));
                new_val->initialize();
                Version *version = reinterpret_cast<Version *>(malloc(sizeof(Version)));
                Rec *rec = malloc(sizeof(record_size));
                new_val->version = version;
                version->read_ts = start_ts;
                version->write_ts = start_ts;
                version->prev = nullptr;
                version->rec = rec;
                version->deleted = false;

                auto new_iter = rw_table.emplace_hint(
                    rw_it, std::piecewise_construct,
                    std::forward_as_tuple(key),
                    std::forward_as_tuple(nullptr, rec, ReadWriteType::INSERT, true, new_val)
                );
                auto &w_table = ws.get_table(table_id);
                w_table.emplace_back(key, new_iter);
                return rec;
            } 
            else if (res == Index::Result::OK)
            {
                val->lock();
                if (val->is_detached_from_tree())
                {
                    val->unlock();
                    return nullptr;
                }
                if (val->is_empty())
                {
                    val->unlock();
                    return nullptr;
                }

                Version *head_version = val->version;
                Version *version = get_correct_version(val);
                // gc
                gc_version_chain(val);
                if (version == nullptr )
                {
                    val->unlock();
                    return nullptr;
                }
                version->update_read_ts(start_ts);
                val->unlock();
                if ( (head_version == version) && version->deleted )
                {
                    // insert

                }
                else if ( !version->deleted )
                {
                    // update

                }
                else return nullptr;

                Rec *rec = malloc(record_size);

                auto new_iter = rw_table.emplace_hint(
                    rw_it, std::piecewise_construct,
                    std::forward_as_tuple(key),
                    std::forward_as_tuple(nullptr, rec, ReadWriteType::INSERT, false, val)
                );
                auto &w_table = ws.get_table(table_id);
                w_table.emplace_back(key, new_iter);
                return rec;

            }
            else assert(0);
        }

        // if could find
        auto rw_type = rw_it->second.type_;
        if (rw_type == ReadWriteType::READ)
        {
            Rec *rec = malloc(record_size);
            memcpy(rec, rw_it->second.read_rec, record_size);
            rw_it.second.write_rec = rec;
            rw_it.second.type_ = ReadWriteType::UPDATE;

            auto &w_table = ws.get_table(table_id);
            w_table.emplace_back(key, rw_it);
            return rec;
        } 
        else if (rw_type == ReadWriteType::UPDATE || rw_type == ReadWriteType::INSERT)
        {
            return rw_it->second.write_rec;
        } 
        else if (rw_type == ReadWriteType::DELETE) 
        {
            rw_it.second.type_ = ReadWriteType::UPDATE;
            return rw_it->second.write_rec;
        }
        else assert(0);

    }


    const Rec *remove(TableID table_id, Key key)
    {
        Index& idx = Index::get_index();
        // also need record size
        const Schema& sch = Schema::get_schema();
        size_t record_size = sch.get_record_size(table_id);

        tables_.insert(table_id);
        auto &rw_table = rws.get_table(table_id);
        auto rw_it = rw_table.find(key);

        if (rw_it == rw_table.end())
        {
            Value *val;
            typename Index::Result res = idx.find(table_id, key, val);

            if (res == Index::Result::OK) {

                val->lock();
                if (val->is_detached_from_tree())
                {
                    val->unlock();
                    return nullptr;
                }
                if (val->is_empty())
                {
                    val->unlock();
                    return nullptr;
                }

                Version *head_version = val->version;
                Version *version = get_correct_version(val);
                // gc
                gc_version_chain(val);
                if (version == nullptr )
                {
                    val->unlock();
                    return nullptr;
                }
                version->update_read_ts(start_ts);
                val->unlock();
                if ( ! (head_version == version && version->deleted ))
                    return nullptr;
                Rec *rec = malloc(record_size);

                auto new_iter = rw_table.emplace_hint(
                    rw_it, std::piecewise_construct,
                    std::forward_as_tuple(key),
                    std::forward_as_tuple(nullptr, rec, ReadWriteType::INSERT, false, val)
                );
                auto &w_table = ws.get_table(table_id);
                w_table.emplace_back(key, new_iter);
                return rec;
            }

        }

        // if could find ( in the remove func)
        auto rw_type = rw_it->second.type_;
        if (rw_type ==  ReadWriteType::READ)
        {
            rw_it->second.type = ReadWriteType::DELETE;
            auto &w_table = ws.get_table(table_id);
            // w_table.emplace_back(key, new_iter);
            return rw_it->second.read_rec;
        }
        else if (rw_type == ReadWriteType::UPDATE || rw_type == ReadWriteType::INSERT) 
        {
            return nullptr;
        }
        else if (rw_type == ReadWriteType::DELETE) 
        {
            rw_it->second.type = ReadWriteType::UPDATE;
            return rw_it->second.write_rec;
        }
        else assert(0);


    }

    bool read_scan(TableID table_id, Key lkey, Key rkey, uint64_t count, bool rev, std::map<Key, Rec*> &kr_map)
    {

    }

    bool precommit()
    {
        Index &idx = Index::get_index();
        for (TableID table_id : tables_)
        {
            auto &w_table = ws.get_table(table_id);
            std::sort(w_table.begin(), w_table.end(), [](const auto& lhs, const auto& rhs)
            {
               return lhs.first <= rhs.first;
            });
            for (auto w_iter = w_table.begin(); w_iter != w_table.end(); ++ w_iter)
            {
                auto rw_iter = w_iter->second;
                Value *val = rw_iter->second.val;
                val->lock();
                if (val->is_detached_from_tree() )
                {

                }
                auto rw_type = rw_iter->second.type_;
                bool is_new = rw_iter->second.is_new;

                // insert


            }

        }

    }

    void abort()
    {
        for (TableID table_id : tables_)
        {
            auto &rw_table = rws.get_table(table_id);
            auto &w_table = ws.get_table(table_id);
            for (auto w_iter = w_table.begin(); w_iter != w_table.end(); ++ w_iter)
            {
                auto rw_iter = w_iter->second;
                auto rw_type = rw_iter->second.type_;
                bool is_new = rw_iter->second.type_;
                if (rw_type == ReadWriteType::INSERT && is_new)
                {
                    // deallocate val;
                }
                else
                {
                    delete rw_iter->second.write_rec;
                }

            }
            rw_table.clear();
            w_table().clear();
        }
        tables_.clear();
    }

    // need acquire value lock before calling
    // 
    void delete_from_tree(TableID table_id, Key key, Value *val)
    {
        Index &idx = Index::get_index();
        idx.remove(table_id, key);
        Version *version = val->version;
        val->version = nullptr;

        // gc collect

        delete version->rec;
        delete version;
        return;
    }

    // need acquire value lock before calling
    Version *get_correct_version(Value *val)
    {
        Version *cur = val->version;
        // 9 -> 4 -> 1, start_ts = 8, find 4.
        while (cur != nullptr && start_ts < cur->write_ts)
        {
            cur = cur->prev;
        }
        return cur;
    }


    // need acquire value lock before calling
    void gc_version_chain(Value *val)
    {
        Version *gc_version_plus_one = nullptr;
        Version *cur = val->version;
        // 9 -> 4 -> 1, start_ts = 8, find 4.
        while (cur != nullptr && smallest_ts < cur->write_ts)
        {
            cur = cur->prev;
        }
        if (cur == nullptr ) return;
        
        // keep one
        gc_version_plus_one = cur;
        cur = cur->prev;
        gc_version_plus_one->prev = nullptr;

        Version *tmp;
        while (cur != nullptr)
        {
            tmp = cur->prev;
            delete cur->rec;
            delete cur;
            cur = tmp;
        }
    }

};


