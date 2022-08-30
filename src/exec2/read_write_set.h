#pragma once

#include <unordered_map>
#include <vector>

#include "value.h"

namespace MVTO {

enum class ReadWriteType { READ = 0, UPDATE, INSERT, DELETE };

// using TableID = uint64_t;


// template <typename Key, typename Value>
// class ReadWriteSet
// {
//   public:
//     using Table = std::unordered_map<Key, ReadWriteElement<Value> > ;
//     Table &get_table(TableID table_id) { return read_write_set_[table_id]; }
//   private:
//     std::unordered_map<TableID, Table> read_write_set_;
// };

template <typename Key, typename Value>
class GlobalWriteSet{
  public:
    using chaining = std::pair<Key, Version<Value>*>;
    std::vector<chaining> & get_table(TableID table_id) { return write_set_[table_id]; }
private:
  std::unordered_map<TableID, std::vector<chaining>> write_set_;
};



} // namepsace