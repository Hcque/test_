
#include <thread>
#include <iostream>
#include <vector>

#include "type_tuple.h"
#include "write_set.h"
#include "database.h"


void runtxn()
{

}

int main(int argc, char *argv[])
{
    std::cout << "./a w_cnt th_cnt\n";
    int w_cnt = atoi(argv[1]);
    int thre_cnt = atoi(argv[2]);
    // init db
    std::cout << "load tables\n";
    load_all_tables(w_cnt);

    std::cout << "check table\n";
    typename RecordToTable<WareHouse>::type& t = Database::get_db().get_table<WareHouse>();
    for (auto it = t.begin(); it != t.end(); ++ it)
    {
        // std::cout << &it << std::endl;
        std::cout << it->second.get() << " " << std::endl;
    }

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