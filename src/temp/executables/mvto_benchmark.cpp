

#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <deque>

#include "mvto.h"
#include "tpc_c.h"

// std::deque<MVTO::Tuple> random_inserts;
// MVTO::DataTable &db;

void GenerateRandomInserts()
{
    std::srand(5);
    for (int i = 0; i < 10; i ++ )
    {
        double key = (100.0 / RAND_MAX) * rand();
        double value = (100.0 / RAND_MAX) * rand();
        std::cout << key << " " << value << std::endl;
        // random_inserts.emplace_back( key, value );
    }

}

void Init()
{
    std::cout << "Init" << std::endl;
    // MVTO::t
    // GenerateRandomInserts();

    // build db
    // MVTO::TimestampManager ts_manager;

    // dtable;
    // 
    // &db = new DataTable();
}


int RunTxns(int *flag, int *count)
{
    // std::cout << "my num: " << *flag << std::endl;
    while (__atomic_load_n(flag,  __ATOMIC_ACQUIRE) )
    {
    // std::cout << "my num: " << *flag << std::endl;

        *count =  *count + 1;
    }

    // MVTO::Transaction *txn;
    // insert data
    // collect stat
    // for (int i = 0; i < 10; i ++)
    // {
        // std::cout << i << std::endl;
        // MVTO::Transaction *txn = new MVTO::Transaction();
        // txn->BeginTransaction();
        // db.Insert(0, random_inserts[i].key_, random_inserts[i].value_);
        // txn->Validate() ? txn->Commit() : txn->Abort();
    // }
    return 0;
}

int count = 0;

int main(int argc, char **argv)
{
    using namespace MVTO;

    std::cout << "./execu num_threads" << std::endl;
    uint32_t num_threads = atoi(argv[1]);

    // Init
    // Init();

    DataBase *db = new DataBase();
    db->CreateTable();
    db->GenerateInitData(num_threads);

    // multi threads
    // std::vector<std::thread> threads;
    // threads.reserve(num_threads);
    // alignas(64) int flag = 1;

    // for (int i = 0; i < num_threads; i ++ )
    // {
    //     // std::cout << flag << std::endl;
    //     threads.emplace_back(RunTxns, &flag, &count);
    // }

    // __atomic_store_n(&flag, 0, __ATOMIC_RELEASE);

    // for (int i = 0; i < num_threads; i ++ )
    // {
    //     threads[i].join();
    // }
    // std::cout << count ;


    return 0;
}