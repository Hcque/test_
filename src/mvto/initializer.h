#pragma once

#include <cstdlib>

#include "mvto.h"
#include "record_layout.h"
#include "record_key.h"


template <typename Index>
class Initializer
{

    using Key = typename Index::Key;
    using Value = typename Index::Value;
    using Version = typename Value::Version;

    public:
    static void load_all_tables(int w_cnt)
    {
        std::cout << " ////////// " << std::endl;
        std::cout << "load all tables " << w_cnt << std::endl;
        // write schema
        Schema sche = Schema::get_schema();
        sche.set_record_size(get_id<WareHouse>(), sizeof(WareHouse));
        sche.set_record_size(get_id<District>(), sizeof(District));

        // // load data
        load_warehouse_table(w_cnt);
    }

    private:
    static void insert_into_index(TableID table_id, Key key, void *rec)
    {
        Value *val = reinterpret_cast<Value *>(malloc(sizeof(Value)));
        Version *version = reinterpret_cast<Version *>(malloc(sizeof(Version)));
        val->initialize();
        val->version = version;
        version->read_ts = 0;
        version->write_ts = 0;
        version->prev = nullptr;
        version->rec = rec;
        version->deleted = false;
        // Index::get_index().insert(table_id, key, val);
        Index::get_index().insert(table_id, key, val);
    }


    // copy and paste
    static void c_and_s_district_rec(uint16_t w_id, uint8_t d_id)
    {
    District::Key key = District::Key::create_key(w_id, d_id);
    District *d = reinterpret_cast<District *> ( malloc(sizeof(District)));
    //   District *d = Database::get_db().alloc_record<District>(key);
    d->generate(w_id, d_id);
    insert_into_index(get_id<District>(), key.get_raw_key(), reinterpret_cast<void *>(d));
    }

    static void c_and_s_warehouse_rec(uint16_t w_id)
    {
    WareHouse::Key key = WareHouse::Key::create_key(w_id);
    // WareHouse *w = reinterpret_cast<WareHouse *> ( malloc(sizeof(WareHouse)));
    WareHouse *w = reinterpret_cast<WareHouse *>( malloc(sizeof(WareHouse)));
    w->generate(w_id);
    insert_into_index(get_id<WareHouse>(), key.get_raw_key(), reinterpret_cast<void *>(w));
    }

    static void load_district_table(uint16_t w_id)
    {
        for (int d_id = 1; d_id < 10; d_id ++ )
        {
        c_and_s_district_rec(w_id, d_id);
        }
    }

    static void load_warehouse_table(int w_cnt)
    {
    // std::cout << "load warehouse table" << w_cnt <<  std::endl;
        for (int w_id = 1; w_id < w_cnt; w_id ++ )
        {
        std::cout << "warehouse table: " << w_id << std::endl;
        c_and_s_warehouse_rec(w_id);
        load_district_table(w_id);
        }
    }

};
