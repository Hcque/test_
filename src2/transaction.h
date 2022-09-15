#pragma once

#include <memory>

#include "common.h"

template <typename Protocol>
class Transaction
{
public:
    uint64_t txnid;
    TimestampManager<Protocol> &tsm;

    std::unique_ptr<Protocol> protocol;
    Transaction(TimestampManager<Protocol>& tsm): tsm(tsm) {
        protocol = std::move(tsm.BeginTransaction());
        txnid = protocol->txnid;
        std::cout << "[Txn]" << txnid << " start " << txnid <<  std::endl;
    }

    // Transaction* BeginTransaction(timestamp_t curr_ts)
    // {
    //     Transaction *txn = new Transaction();
    //     txn->txnid = curr_ts;
    //     return txn;
    // }

    void Insert(Key key, Val val)
    {
        // std::cout << "[Txn] " << txnid << " Insert " << key <<  std::endl;
        protocol->Insert(key, val);
    }

    int Read(Key key, Val& val)
    {
        // std::cout << "[Txn]" << txnid << " Read " << key <<  std::endl;
        return protocol->Read(key, val);
    }

    void Commit()
    {
        // std::cout << "[Txn]" << txnid << " Commit " <<  std::endl;
        // insert into index
    }
    
    void Abort();
    void RollBack();

};
