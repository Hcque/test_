
#include <thread>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "transaction.h"
#include "initializer.h"
#include "tpcc.h"


// #include "./../indexes/masstree.hpp"
#include "./../indexes/hash_index.h"



template <typename Protocol, typename TxProfile>
void runtxn()
{
    std::cout << "[runtxn]" << std::endl;
    TxProfile process(1);
    std::cout << "[runtxn] start tx" << std::endl;
    Transaction<Protocol> tx;
    std::cout << "[runtxn] run" << std::endl;
    process.run(tx);
}

int main(int argc, char *argv[])
{
    std::cout << "./a w_cnt th_cnt\n";
    int w_cnt = atoi(argv[1]);
    int thre_cnt = atoi(argv[2]);
    // init db
    std::cout << "[main] load tables\n";

    using Index = HashIndexes<Value<Version>>;
    using Protocol = MVTO<Index>;
    Initializer<Index>::load_all_tables(w_cnt);


    // run txn
    std::vector<std::thread> threads;
    for (int i = 0; i < thre_cnt; i ++ )
    {
        // std::cout << "[main] thd cnt:" << i << std::endl;
        threads.emplace_back(runtxn<Protocol, NewOrderTxn>);
    }

    for (int i = 0; i < thre_cnt; i ++ )
    {
        threads[i].join();
    }

    return 0;
}