#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

using TableID = uint64_t;

struct TableInfo
{
    size_t rec_size = 0;
    TableID secondary = 0;
};


class Schema
{
  public:
  size_t get_record_size(TableID table_id) const { 
      if ( schema.count(table_id) == 0)
      {
          std::cout << table_id << std::endl;
          assert(0);
      }

      return schema.at(table_id).rec_size; 
  }
  bool has_secondary_table(TableID table_id) const { return schema.at(table_id).secondary != 0; }
  TableID get_secondary_table(TableID table_id) const { return schema.at(table_id).secondary; }

  void set_record_size(TableID table_id, size_t rec_size) { schema[table_id].rec_size = rec_size; }

  
  std::vector<TableID> get_tables()
  {
      std::vector<TableID> tables;
      for (auto &[table_id, size]: schema )
      {
          tables.emplace_back(table_id);
      }
      return tables;
  }

  static Schema& get_mutable_schema()
  {
      static Schema sch;
      return sch;
  }
  static const Schema& get_schema() { return get_mutable_schema(); }

  private:
    std::unordered_map<TableID, TableInfo> schema;
};




