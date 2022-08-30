

#include <iostream>

#include "tpc_c.h"
#include "value.h"
#include "mvto.h"
#include "database.h"



template <typename Protocol>
class Worker
{
  public:
    alignas(64) uint64_t txn_cnt{1};
    uint64_t smallest_txn_cnt{1};
    uint64_t largest_txn_cnt{1};

};




// tpcc
class Initializer
{
  public:
    static void create_and_insert_warehouse(uint16_t w_id)
    {
        // reserve space by memalloc
        Warehouse *w = new Warehouse();
         // geneate data
         w->generate(w_id);
         // create index

    }

    static void load_all_tables()
    {
        // sche
        // schema[ get_id<Warehouse>() ] = sizeof( Warehouse );

        // load data
        for (int w_id = 0; w_id < 10; w_id ++ )
        {
            create_and_insert_warehouse(w_id);
        }
    }


  private:
    

};




int main()
{

    // std::unordered_map<int,int> M;
    // for (int i = 0; i < 10; i ++ )    
    // {
    //     M[i] = i;
    // }

    // // auto iter = M.iterator();
    // for (auto it = M.begin(); it != M.end(); ++it)
    // {
    //     std::cout << it->second << std::endl;
    // }

    // auto it = M.find(10);
    // std::cout << "it" << ( it == M.end() ) << std::endl;

    std::cout << "START\n";

    auto db = DataBase::get_db();
    db.load_all_tables();

    // Initializer::load_all_tables();


    // MVTO mvto;
    // mvto.read(0, 1);

    return 0;

}