#pragma once

#include <vector>
#include <deque>
#include <atomic>
#include <cassert>
#include <forward_list>
#include <iostream>

#include "tuple.h"

namespace MVTO {

class MVTO
{
    void Insert();

    void Delete();

};

class TimestampManager
{
  public:
    uint64_t IncreaseTimestamp() { return time_++ ; }
    uint64_t CurrentTime() { return time_.load(); }
    std::atomic<uint64_t> time_;

};


class Transaction
{
  public:
    void BeginTransaction()
    {
        start_time_ = timestamp_manager_->IncreaseTimestamp();
    }

    // optmistic
    bool Validate()
    {
        // rw , ww, wr , me in the first pos
        // rw , ww, wr, me in the second pos;
        // 6 conditions
        return 0;
    }

    void Commit()
    {
        // flush timestamp to be visiable ( no local)
        // no running 
        
    }
    void Abort()
    {
        // revert history, temp set = undo set, write set


        //

    }

    std::atomic<uint64_t> start_time_, commit_time_, finish_time_;
    TimestampManager *timestamp_manager_;
    // key , mem addr
    std::forward_list<std::pair<uint64_t, uintptr_t> > read_set_, write_set_;

    // UndoBUffer
};


// DataTable : the main API for storage engine,
// structure: vector of head of linked list
class DataTable
{
  public:
    bool Insert(Transaction *txn, uint64_t key, Tuple tuple);
    uint32_t Size() const { return table_.size(); }
    std::vector<Tuple > table_;
    // bitmap ?
    // std::deque<Tuple> table_;

    TimestampManager *ts_manager_;
};


// Imple DataTable

//   void Select(Transaction *txn, const uint64_t key, std::vector<Tuple> &outbuf)
//     {
//         // ts_manager_->IncreaseTimestamp();
//         // outbuf.clear();
//         // // read only transaction part
//         // // find the curr pos
//         // for (int i = 0; i < Size(); i ++ )
//         // {
//         //     if (key == table_[i].key_)
//         //     {
//         //         // Tuple *curr_tuple = &(table_[i]);
//         //         // while (curr_tuple)
//         //         // {
//         //         //     if (curr_tuple->begin_timestamp_.load() <= txn->start_time_ && 
//         //         //     curr_tuple->end_timestamp_.load() > txn->start_time_)
//         //         //     {
//         //         //         outbuf.push_back(*curr_tuple);
//         //         //     }
//         //         // }
//         //     }
//         // }
//         // // which version in my vision, select the latest
//         // // push to the out buffer

//         // // push into read set ? 
//     }
    
bool DataTable::Insert(Transaction *txn, uint64_t key, Tuple tuple)
{

    bool find = 0;
    for (int i = 0; i < Size(); i ++ ) if (key == table_[i].key_) find = 1;
    if (find == 0)
    {
        table_.push_back(tuple);
    }
    else
    {
        // check the timestamp of version, add new in locol.
        assert(0);
    }

    return 1;
    // // add to the version chain (private)
    // auto curr_slot = table[i];
    // while (curr_slot)
    // {
    //     // new to old
    //     if curr_slot->begin_timestamp_ <= t)
    //     {
    //         //insert key
    //     }
    // }
    // // look at the version storage

}


//     void Update(Transaction *txn, const uint64_t key, char *outbuf)
//     {
//         // write set

//         // select
//         // insert new tuple with new ts, add to the version chain
//         // push into write set

//     }
//     // void Delete(Transaction *txn, const Key key, char *outbuf)
//     // {
//     //     // mark as null
//     //     // delete by gc
//     // }


}


