
#pragma once

#include <unordered_map>
#include <cstdlib>
#include <iostream>

#include "data_table.h"



class TimestampManager
{
  public:
  void Increment() { count_ ++ ; }
  uint64_t CurretTime() { return count_.load(); }

  std::atomic<uint64_t> count_{0};
};


class DataBase
{
    std::unordered_map<std::string, uintptr_t> tables_;
    std::unordered_map<TableID, uint32_t> schema;
  public:

    static DataBase &get_db()
    {
        static DataBase db;
        return db;
    }
    void CreateTable()
    {
        DataTable<Warehouse> *table = new DataTable<Warehouse>("Warehouse");
        tables_["Warehouse"] = reinterpret_cast<uintptr_t>(table);
    }

    void create_and_insert_warehouse(uint16_t w_id)
    {
        std::cerr << "c&insert warehouse" << w_id << "\n";
        // reserve space by memalloc
        Warehouse *w = new Warehouse();
         // geneate data
        std::cerr << "c&insert warehouse" << w_id << "\n";
         w->generate(w_id);
         // create index
        std::cerr << "c&insert warehouse" << w_id << "\n";
         reinterpret_cast<DataTable<Warehouse> *>(tables_["Warehouse"])->Insert(0, *w);

    }

    void load_all_tables()
    {
        CreateTable();
        std::cerr << "load_all_tables\n";
        // sche
        schema[ get_id<Warehouse>() ] = sizeof( Warehouse );

        // load data
        for (int w_id = 0; w_id < 10; w_id ++ )
        {
            create_and_insert_warehouse(w_id);
        }
    }


    template <typename Record>
    void Select(std::string table_name, uint64_t key, std::vector<Record> &output)
    {
         reinterpret_cast<DataTable<Record> *>(tables_[table_name])->Select(0,output);
    }





    void GenerateInitData(int NUM)
    {
        // size_t double_sz = sizeof(double);
        // size_t value_sz = 1+4+double_sz + double_sz;
        // for (int i = 0; i < NUM; i ++ )
        // {
        //     // District *d = new District();
        //     if (i % 100000 == 0 )
        //         std::cout << i << "\n";
            
        //     Tuple *tuple = new Tuple(i, value_sz);
        //     District *d = reinterpret_cast<District *>( tuple->value_ );
        //     d->d_id = i;
        //     d->next_o_id += 1;
        //     d->d_tax = 0.1;
        //     d->d_ytd = 0.2;
        //     tables_["District"]->Insert(nullptr, i, *tuple);
        // }

        // std::vector<Tuple> out;
        // std::cout << tables_["District"]->Select(nullptr, 86, out) << std::endl;

        // std::cout << reinterpret_cast<District *>( out[0].value_)->d_tax << std::endl;


    }

    template <typename Record>
    Record *allocate_record()
    {
        // get table 
        // 
    }

    void RunNewOrder()
    {
        // update district next_oid
        // tables_["District"].update()

        //oid = next_o_id
        // insert into Orders

        // insert into NewOrder

        //  


        // update Stock s_quantity

        // ol_amount, total

        // insert into orderlines

    }

};
