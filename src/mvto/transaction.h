#pragma once

#include <memory>
#include <cassert>

#include "mvto.h"


template <typename Record>
inline TableID get_id() { return sizeof(Record); }


template <typename Protocol>
class Transaction
{
    public:
    Transaction()
    {
        protocol = std::make_unique<Protocol>(txn_cnt_++, txn_cnt_);
    }
    // ~Transaction() { mutex_.unlock(); }

    std::unique_ptr<Protocol> begin_tx()
    {
        // return std::make_unique<Protocol>(nullptr);
    }
    void abort() {
        protocol->abort();
        // protocol->set_new_ts();
    }
    bool commit()
    {
        if (protocol->precommit())
        {
            return 1;
        }
        else
        {
            abort();
            return 0;
        }
    }
    enum class Result { SUCCESS, FAIL, ABORT };

    template <typename Record>
    Result get_record(const Record* &rec_ptr, typename Record::Key key)
    {
        // assume write set does not hold current record
        rec_ptr = reinterpret_cast<const Record *> ( 
            protocol->read(get_id<Record>(), key.get_raw_key()) );
        // assert(rec_ptr != nullptr);
        return rec_ptr == nullptr ? Result::ABORT : Result::SUCCESS;
    }

    template <typename Record>
    Result prepare_record_for_insert(Record* &rec_ptr, typename Record::Key key)
    {
        rec_ptr = reinterpret_cast<Record *> ( 
            protocol->insert(get_id<Record>(), key.get_raw_key()) );
        return rec_ptr == nullptr ? Result::ABORT : Result::SUCCESS;
    }
    template <typename Record>
    Result prepare_record_for_update(Record*& rec_ptr,typename Record::Key key)
    {
        rec_ptr = reinterpret_cast<Record *> ( 
            protocol->update(get_id<Record>(), key.get_raw_key()) );
        return rec_ptr == nullptr ? Result::ABORT : Result::SUCCESS;
    }
    template <typename Record>
    Result prepare_record_for_remove(Record*& rec_ptr,typename Record::Key key)
    {
        rec_ptr = reinterpret_cast<Record *> ( 
            protocol->remove(get_id<Record>(), key.get_raw_key()) );
        return rec_ptr == nullptr ? Result::ABORT : Result::SUCCESS;
    }

    // use read_scan
    template <typename Record, typename Func>
    Result range_query(typename Record::Key low, typename Record::Key up, Func &&func)
    {
        std::map<uint64_t, void *> kr_map;
        // bool scanned = protocol->read_scan();
    }

    private:
        std::unique_ptr<Protocol> protocol = nullptr;
        uint64_t txn_cnt_{0};

};


////////// transaction utils
class Output
{


};



