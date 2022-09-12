
#pragma once

#include <vector>
#include <deque>
#include <atomic>
#include <cassert>
#include <forward_list>
#include <iostream>
#include <string>


// DataTable : the main API for storage engine,
// structure: vector of head of linked list
template <typename Record>
class DataTable
{
  public:
    DataTable(std::string name) : table_name_(name) {}

    uint64_t Select (uint64_t key, std::vector<Record> &outbuf)
    {

    }

    // Record *AllocateRecord()

    bool Insert(uint64_t key, Record rec)
    {
      table_.push_back(rec);
      return 1;

    }
    // void Update(Transaction *txn, const uint64_t key, char *outbuf);

    uint32_t Size() const { return table_.size(); }
    std::vector<Record> table_;
    // bitmap ?
    // std::deque<Tuple> table_;
    std::string table_name_;

    // TimestampManager *ts_manager_;
};


// Imple DataTable

// uint64_t DataTable::Select(Transaction *txn, const uint64_t key, std::vector<Tuple> &outbuf)
// {
//     outbuf.clear();
//     // read only transaction part
//     // find the curr pos
//     for (int i = 0; i < Size(); i ++ )
//     {
//         if (key == table_[i].key_)
//         {
//             // if (version)
//             outbuf.push_back(table_[i]);
//         }
//     }
//     return outbuf.size();
//     // which version in my vision, select the latest
//     // push to the out buffer

//     // push into read set ? 
// }
// template <typename Record>
// bool DataTable<Record>::Insert(uint64_t key, Record *rec)
// {
//     table_.push_back(rec);
//     return 1;
// }




// void DataTable::Update(Transaction *txn, const uint64_t key, char *outbuf)
// {
//     // write set
    
//     int64_t find = -1;
//     for (int i = 0; i < Size(); i ++ ) if (key == table_[i].key_) find = i;
//     if (find == -1) assert(0);

//     Tuple *new_tuple = new Tuple();
//     // tuple.txn_id_t = txn->id;
//     // ts
//     new_tuple->next_ = &(table_[find]);

//     //add pointer to write set

// }

//     // void Delete(Transaction *txn, const Key key, char *outbuf)
//     // {
//     //     // mark as null
//     //     // delete by gc
//     // }





