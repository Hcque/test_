#pragma once

#include <vector>
#include <thread>

#include "common.h"
#include "transaction.h"
#include "tpcc.h"

void test1()
{
    //////// construct diff workloads for ts //////////
    // MVTO mvto;
    // TimestampManager<MVTO> tsm;
    // Transaction<MVTO> txn = Transaction<MVTO>(std::move(tsm));
    // for (int i = 1; i < 100; i ++ )
    // {
    //     Key key = rand() % 10;
    //     txn.Insert(key, key);
    // }

    // int cnt = 0;

    // for (int i = 0; i < 10; i ++ )
    // {
    //     Val val;
    //     cnt += txn.Read(i, val);
    // }

    // // std::cout << "[test1] cnt: " << cnt << std::endl;
    // txn.Commit();
}


// for each thread
void EachThread(TimestampManager<MVTO> &tsm)
{
    // std::cout << "[Each thread] " << std::this_thread::get_id() << std::endl;

    Transaction<MVTO> txn = Transaction<MVTO>(tsm);

    // do insert
    NewOrderTxn t1 = NewOrderTxn();
    t1.run(txn);

    // do read

    txn.Commit();
}


void test2(uint64_t num_thread)
{
    TimestampManager<MVTO> tsm;
    // std::cout << "[test2] ts:" << tsm.ts.load() << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < num_thread; i ++ )
    {
        threads.emplace_back(EachThread, std::ref(tsm));
    }
    for (int i = 0; i < num_thread; i ++ )
    {
        threads[i].join();
    }

}