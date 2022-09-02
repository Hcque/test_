#pragma once

#include <unordered_map>
#include <set>
#include <map>
#include <cassert>

#include "readwrite_lock.h"
#include "atomic_wrapper.h"

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
    bool is_empty() { return (version.prev == nullptr) && version.deleted; }
    bool is_detached_from_tree() { return version == nullptr; }

};

enum class ReadWriteType { READ = 0, INSERT, UPDATE, DELETE };

template <typename Value>
class ReadWriteElement
{

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
    using Version = typename Index::Version;
    using LeafNode = typename Index::LeafNode;
    using NodeInfo = typename Index::NodeInfo;

  private:
    TxnID txn_id;
    uint64_t start_ts, smallest_ts;
    std::set<TableID> tables_;
    ReadWriteSet<Key, Value> rws;
    WriteSet<Key, Value> ws;

  public:
    Rec *read(TableID table_id, Key key) 
    {
        Index& idx = Index::get_index();
        tables_.insert(table_id);
        auto &rw_table = rws.get_table(table_id);
        auto rw_it = rw_table.find(key);

        if (rw_it == rw_table.end())
        {
            Value *val;
            typename Index::Result res = idx.find(table_id, key, val);
            if (res == Index::Result::NOT_FOUND) return nullptr;

            val->lock();
            // detacted from te tree
            if (val->is_detacted_from_tree())
            {
                val->unlock();
                return nullptr;
            }

            if (val->is_emplty())
            {
                val->unlock();
                return nullptr;
            }

            Version *version = get_correct_version(val);
            if (version == nullptr)
            {
                val->unlock();
                return nullptr;
            }

            version->update_read_ts(start_ts);

            // into read write set
            rw_table.emplace_hint(rw_it, std::piecewise_construct, 
                std::forward_as_tuple(key), 
                std::forward_as_tuple(version->rec, nullptr, ReadWriteType::READ, false, val)
            );
            return version->rec;
        }

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

        tables_.insert(table_id);
        auto &rw_table = rws.get_table(table_id);
        auto rw_it = rw_table.find(key);

        if (rw_it == rw_table.end())
        {
            Value *val;
            typename Index::Result res = idx.find(table_id, key, val);

            if (res == Index::Result::OK) {

                val->lock();
                    if (val->isemplty())
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

                // ??
                // if (version->deleted) return nullptr;

                // aloocate new rec space
                // into write set
                // w_table.emplace_hint();
                // return version->rec;
            }


            // create new val to the write set

        }

        // if could find
        auto rw_type = rw_it->second.type_;
        if (rw_type ==  ReadWriteType::READ) return rw_it->second.read_rec;
        else if (rw_type == ReadWriteType::UPDATE || rw_type == ReadWriteType::INSERT) return rw_it->second.write_rec;
        else if (rw_type == ReadWriteType::DELETE) return nullptr;
        else assert(0);

    }

    Rec *update(TableID table_id, Key key)
    {
        Index& idx = Index::get_index();

        // also need record size

        tables_.insert(table_id);
        auto &rw_table = rws.get_table(table_id);
        auto rw_it = rw_table.find(key);

        if (rw_it == rw_table.end())
        {
            Value *val;
            typename Index::Result res = idx.find(table_id, key, val);
            // if () i// abort if not find


            val->lock();
                if (val->isemplty())
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

            // ??
            // if (version->deleted) return nullptr;

            // aloocate new rec space
            // into write set
            // w_table.emplace_hint();
            // return version->rec;


            // create new val to the write set

        }

        // if could find
        auto rw_type = rw_it->second.type_;
        if (rw_type == ReadWriteType::READ) return rw_it->second.read_rec;
        else if (rw_type == ReadWriteType::UPDATE || rw_type == ReadWriteType::INSERT) return rw_it->second.write_rec;
        else if (rw_type == ReadWriteType::DELETE) return nullptr;
        else assert(0);
    }
    Rec *upsert(TableID table_id, Key key);
    const Rec *remove(TableID table_id, Key key);

    bool read_scan(TableID table_id, Key lkey, Key rkey, uint64_t count, bool rev, std::map<Key, Rec*> &kr_map)
    {

    }

    bool precommit();
    void abort();

    // need acquire value lock before calling
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


