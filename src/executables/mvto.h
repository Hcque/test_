#pragma once

#include <vector>
#include <deque>
#include <atomic>
#include <cassert>
#include <forward_list>
#include <iostream>
#include <string>

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
    DataTable(std::string name) : table_name_(name) {}

    uint64_t Select(Transaction *txn, const uint64_t key, std::vector<Tuple> &outbuf);
    bool Insert(Transaction *txn, uint64_t key, Tuple tuple);
    void Update(Transaction *txn, const uint64_t key, char *outbuf);

    uint32_t Size() const { return table_.size(); }
    std::vector<Tuple > table_;
    // bitmap ?
    // std::deque<Tuple> table_;
    std::string table_name_;

    TimestampManager *ts_manager_;
};


// Imple DataTable

uint64_t DataTable::Select(Transaction *txn, const uint64_t key, std::vector<Tuple> &outbuf)
{
    outbuf.clear();
    // read only transaction part
    // find the curr pos
    for (int i = 0; i < Size(); i ++ )
    {
        if (key == table_[i].key_)
        {
            // if (version)
            outbuf.push_back(table_[i]);
        }
    }
    return outbuf.size();
    // which version in my vision, select the latest
    // push to the out buffer

    // push into read set ? 
}

bool DataTable::Insert(Transaction *txn, uint64_t key, Tuple tuple)
{
    int64_t find = -1;
    // for (int i = 0; i < Size(); i ++ ) if (key == table_[i].key_) find = i;
    if (find == -1)
    {
        table_.push_back(tuple);
    }
    else
    {
        // check the timestamp of version, add new in locol.
        // version chain add
        assert(0);
        int64_t i = find;
        // find the place

        //insert to local
        
        Tuple *tmp = table_[i].next_;
        Tuple *second = new Tuple();
        *second = table_[i];
        table_[i] = tuple;
        table_[i].next_ = second;
        second->next_ = tmp;
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


void DataTable::Update(Transaction *txn, const uint64_t key, char *outbuf)
{
    // write set
    
    int64_t find = -1;
    for (int i = 0; i < Size(); i ++ ) if (key == table_[i].key_) find = i;
    if (find == -1) assert(0);

    Tuple *new_tuple = new Tuple();
    // tuple.txn_id_t = txn->id;
    // ts
    new_tuple->next_ = &(table_[find]);

    //add pointer to write set

}

//     // void Delete(Transaction *txn, const Key key, char *outbuf)
//     // {
//     //     // mark as null
//     //     // delete by gc
//     // }


}


