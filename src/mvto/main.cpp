
#include <thread>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "transaction.h"
#include "initializer.h"
#include "tpcc.h"


// #include "./../indexes/masstree.hpp"
#include "./../indexes/hash_index.h"

void runtxn()
{
    std::cout << "me " << std::endl;

}

int main(int argc, char *argv[])
{
    std::cout << "./a w_cnt th_cnt\n";
    int w_cnt = atoi(argv[1]);
    int thre_cnt = atoi(argv[2]);
    // init db
    std::cout << "load tables\n";

    using Index = HashIndexes<Value<Version>>;
    using Protocol = MVTO<Index>;
    Initializer<Index>::load_all_tables(w_cnt);
    // Index::get_index();

    // std::cout << "check table\n";
    // typename RecordToTable<WareHouse>::type& t = Database::get_db().get_table<WareHouse>();
    // for (auto it = t.begin(); it != t.end(); ++ it)
    // {
    //     // std::cout << &it << std::endl;
    //     std::cout << it->second.get() << " " << std::endl;
    // }

    std::cout << "start txns\n";
    // run txn
    std::vector<std::thread> threads;
    for (int i = 0; i < thre_cnt; i ++ )
    {
        threads.emplace_back(runtxn);
    }

    for (int i = 0; i < thre_cnt; i ++ )
    {
        threads[i].join();
    }

    return 0;
}