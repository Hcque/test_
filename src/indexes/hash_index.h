#pragma once

#include <unordered_map>

template <typename Value_>
class HashIndexes
{
  public:
    using Key = uint64_t;
    using Value = Value_;
    using HI = std::unordered_map<Key, Value*>;

    enum Result {
        OK = 0,
        NOT_FOUND,
        BAD_INSERT
    };

    Result find(TableID table_id, Key key, Value*& val)
    {
        auto &index = indexes_[table_id];
        if (index.count(key) == 0) return Result::NOT_FOUND;
        val = index[key];
        return Result::OK;
    }

    Result insert(TableID table_id, Key key, Value *val)
    {
        // auto &_index = indexes_[table_id];
        assert(indexes_[table_id].count(key) == 0);
        indexes_[table_id][key] =  val;
        return Result::OK;
    }

    Result remove(TableID table_id, Key key)
    {
        auto &_idx = indexes_[table_id];
        auto it = _idx.find(key);

        assert(it != _idx.end());
        _idx.erase(it);
        return Result::OK;
    }


    static HashIndexes<Value>& get_index() { 
        static HashIndexes<Value> indexes;
        return indexes;
     }

  private:
    std::unordered_map<TableID, HI> indexes_;

};
