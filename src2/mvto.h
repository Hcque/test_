
#include <unordered_map>
#include <set>
#include <vector>

#include "value.h"


using Rec = void;
using TableID = uint64_t;
using TxnID = uint64_t;


template <typename Key, typename Value>
class ReadWriteSet
{
    using Table = std::unordered_map<Key, ReadWriteElement<Value> >;
    Table& get_table(TableID table_id) { return rws[table_id]; }

    std::unordered_map<TableID, Table> rws;

};

template <typename Key, typename Value>
class WriteSet
{
   using chain = std::pair<Key, typename std::unordered_map<Key, ReadWriteElement<Value>>::iterator >;
   std::unordered_map<TableID, std::vector<chain> > ws;
};


template <typename Index>
class MVTO
{
public:

    using Key = typename Index::Key;
    using Value = typename Index::Value;

    Rec* read(TableID table_id, Key key, Value *val);
    Rec* insert(TableID table_id, Key key, Value *val);
    Rec* update(TableID table_id, Key key, Value *val);
    Rec* remove(TableID table_id, Key key, Value *val);


    TxnID txnid;
    timestamp_t start_ts, smallest_ts, largest_ts;
    std::set<TableID> tables_;

    ReadWriteSet<Key, Value> rws;
    WriteSet<Key, Value> ws;

};

Rec* MVTO::read(TableID table_id, Key key, Value *val)
{
    auto& rw_table = rws.get_table(table_id);
    auto rw_it = rw_table.find(key);
    if (it == rw_table.end())
    {
        if ()
        


    }

    if (it->type_ == ReadWriteType::READ) return it->read_rec;
    else if (it->type_ == ReadWriteType::INSERT &&
        it->type_ == ReadWriteType::UPDATE 
    ) return it->write_rec;
    else return nullptr;

}








