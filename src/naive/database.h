
#pragma once

#include <memory>
#include <map>
#include <cassert>

#include "record_layout.h"
#include "record_key.h"

template <typename Record>
struct RecordToTable
{
  using type = std::map<typename Record::Key, std::unique_ptr<Record>>;
};

class Database
{
public:
    Database() {}
    ~Database() {}
  // underling multimap

    using TT = TypeTuple<
    RecordToTable<District>::type,
    RecordToTable<WareHouse>::type
    >;

    TT table_tuple;

    Database(const Database&) = delete;
    Database& operator=(Database const&) =  delete;


    static Database& get_db()
    {
        static Database db;
        return db;
    }

    template <typename Record>
    typename RecordToTable<Record>::type& get_table() 
    {
      return get<typename RecordToTable<Record>::type>(table_tuple);
    }
    template <typename Record>
    Record* alloc_record(typename Record::Key key)
    {
      typename RecordToTable<Record>::type& t = get_table<Record>();
      auto it = t.find(key);
      if (it == t.end())
      {
        auto second =  reinterpret_cast<Record*>( malloc(sizeof(Record)) ) ;
        t.emplace(key, second);
        return second;
      }
      return it->second.get();
      return 0;
    }

    template <typename Record>
    bool insert_record(typename Record::Key key, std::unique_ptr<Record> rec_ptr)
    {
      typename RecordToTable<Record>::type& t = get_table<Record>();
    //   t.try_emplace(key, std::move(rec_ptr));
      return 1;
    }

    template <typename Record>
    bool delete_record(typename Record::Key key, std::unique_ptr<Record> rec_ptr)
    {
      return 0;
    }

    template <typename Record>
    bool update_record(typename Record::Key key, std::unique_ptr<Record> rec_ptr)
    {
      return 0;
    }

};

void c_and_s_district_rec(uint16_t w_id, uint8_t d_id)
{
  District::Key key = District::Key::create_key(w_id, d_id);
  // District *d = reinterpret_cast<District *> ( malloc(sizeof(District)));
  District *d = Database::get_db().alloc_record<District>(key);
  d->generate(w_id, d_id);
}

void c_and_s_warehouse_rec(uint16_t w_id)
{
  WareHouse::Key key = WareHouse::Key::create_key(w_id);
  // WareHouse *w = reinterpret_cast<WareHouse *> ( malloc(sizeof(WareHouse)));
  WareHouse *w = Database::get_db().alloc_record<WareHouse>(key);
  w->generate(w_id);
}


void load_district_table(uint16_t w_id)
{
    for (int d_id = 1; d_id < 10; d_id ++ )
    {
      c_and_s_district_rec(w_id, d_id);
    }
}

void load_warehouse_table(int w_cnt)
{
  std::cout << "load warehouse table" << w_cnt <<  std::endl;
    for (int w_id = 1; w_id < w_cnt; w_id ++ )
    {
      std::cout << "warehouse table: " << w_id << std::endl;
      c_and_s_warehouse_rec(w_id);
      load_district_table(w_id);
    }
}

void load_all_tables(int w_cnt)
{
  load_warehouse_table(w_cnt);
}



