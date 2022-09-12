#pragma once

#include <cstring>
#include <set>
#include <cassert>
#include <queue>

#include "read_write_set.h"
#include "transaction.h"

namespace MVTO {

using TxID = uint64_t;

// struct TxID
// {
//   uint64_t txid;
//   TxID(uint64_t txid) : txid(txid) {}

// };

// template <typename Index>
class MVTO
{
  // using Key = typename Index::Key;
  // using Value = typename Index::Value;

  using Key = int;
  using Value = int;

  public:
    MVTO() {}
    MVTO(TxID txid, uint64_t ts, uint64_t smallest_ts, uint64_t largest_ts)
    : txid(txid)
    , start_ts(ts)
    , smallest_ts(smallest_ts)
    , largest_ts(largest_ts) {}

    ~MVTO() { 
      //gc
     }

    void set_new_ts(uint64_t start_ts_, uint64_t smallest_ts_, uint64_t largest_ts_)
    {
      start_ts = start_ts_;
      smallest_ts = smallest_ts_;
      largest_ts = largest_ts_;
    }

    void read(TableID table_id, Key key)
    {
        // tables.insert(table_id);
        // auto &rw_table = read_write_set_.get_table(table_id);
        // auto rw_iter = rw_table.find(key);

        // std::cout << rw_iter->first << std::endl;
        // assert(rw_iter == rw_table.end());

        // if (rw_iter == rw_table.end())
        // {
            // Value *val;

            // Version *version = get_correct_version(val);
            // gc_version_chain(val);
            // if (version == nullptr )
            // {
            //     val->unlock();
            //     return 0;
            // }
            // read_write_set_.emplace_back();
        // }
    }

    // void *insert(TableID table_id, Key, key)
    // {

    // }

    // void 


  private:
    TxID txid;
    uint64_t start_ts;
    uint64_t smallest_ts, largest_ts;
    std::set<TableID> tables;
    // // ReadWriteSet<Key, Value> read_write_set_;
    GlobalWriteSet<Key, Value> write_set_;
    std::queue<TxID> active_txns_, recent_txns_;

};


} //namespace MVTO