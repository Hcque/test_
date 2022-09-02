
// lock contention , basic mode
// summary : stomic data structre key is 
// 1)define lod
// 2) do the operations in while(1) loop
// 3) CaS in the end to jump the loop ( success) 
// 


#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <cassert>


using TxnID = uint64_t;

struct VersionContent
{
    std::atomic<uint64_t> txn_id, read_ts, begin, end;
};


struct Version
{
    int val;
    std::atomic<uint64_t> txn_id, read_ts, begin, end;
    std::atomic<Version *> next;
    std::mutex mutex_;

    static Version *InitRecord()
    {
        return new Version();
    }

    void AtomicRead(TxnID cur_txn_id)
    {
        // uint64_t read_latch = 0; // old
        // uint64_t old_read_ts = read_ts.load();
        // while (1) 
        // {
        //     read_ts.store(old_read_ts);
        //     if (read_ts.load() < cur_txn_id) 
        //     {
        //         read_ts.store( cur_txn_id );
        //     }
        //     if (txn_id.compare_exchange_strong(read_latch, 0))
        //     {
        //         return;
        //     }
        // }
    }

    void AtomicWrite(uint64_t cur_txn_id)
    {
        // allocate
        // Version *version = new Version();
        // version->txn_id.store( cur_txn_id ); 
        // cur_txn_id is old

        assert(cur_txn_id > read_ts.load());
        while (1)
        {
            uint64_t old_txn_id = txn_id.load();
            if (old_txn_id == 0)
            {
                // set latch of both
                // do work
            }
            if (txn_id.compare_exchange_strong(cur_txn_id, old_txn_id) )
            {
                return;
            }

        }
    }


};

struct List
{
    std::atomic<Version *> chain;
    int inplace_for_update{0};
    std::atomic<uint64_t> txn_id, read_ts, begin, end;

    // problem : branch
    void AtomicRead(uint64_t txn_id, int &out)
    {
        if (txn_id < begin.load())
        {
            // chase the version chain
            Version *cur = chain.load();
            while (cur) // atomic?
            {
                if (cur->end.load() > txn_id ) break;
                cur = cur->next.load();
            }

            // read the cur
            out = cur->val;
        }
        else
        {
            // read the current
            out = inplace_for_update;
        }
    }

    // not check read-ts
    void AtomicWrite(uint64_t cur_txn_id)
    {
        // check valid for write
        txn_id->store(cur_txn_id);
        while (1)
        {
            if (read_ts.load() < cur_txn_id)
            {
                if (txn_id.compare_exchange_strong(cur_txn_id, 0))
                {
                    return;
                }
            }
        }

        // allocate
        Version *version = new Version();
        version->txn_id.store( cur_txn_id ); 
        // cur_txn_id is old
        while (1)
        {
            int latch_txn_id = 0;
            // set latch of both
            // txn_id.store(cur_txn_id);

            if (txn_id.compare_exchange_strong(latch_txn_id, cur_txn_id) )
            {
                return;
            }
        }

    }

};


void insert(List* list, int value)
{
    // std::cout << value << std::endl;
    //  list->mutex_.lock();

    Version* version = Version::InitRecord();
    // std::cout << version << std::endl;
    while (1)
    {
        // pre-store for atomic check
        Version *old = list->chain.load();

        // copy head content
        version->next.store(list->chain.load());
        version->val = list->inplace_for_update;

        // inplace-update
        list->inplace_for_update = value;

        // list->chain.store(version);
        if  ( list->chain.compare_exchange_strong(old, version ))
        {
            return;
        }
    }
}

std::vector<std::thread> ths;

int main(int argc, char *argv[])
{
    List *l = new List();
    l->chain.store(0);

    // std::this_thread::sleep_for( std::chrono::seconds(2) );

    int NUM = atoi(argv[1]);

    for (int i = 0; i < NUM; i ++ )
    {
        ths.emplace_back(insert, l, i);
    }

    for (int i = 0; i < NUM; i ++ ) ths[i].join();
    std::cout << "DONE === \n";
    // std::cout << C.load() << std::endl;

    Version *cur = l->chain.load();
    int cnt = 0;
    while (cur)
    {
        // std::cout << cur->value << std::endl;
        cnt ++ ;
    // std::cout << "cnt : " << cur << std::endl;

        cur = cur->next.load();
    }
    std::cout << "cnt : " << cnt << std::endl;


    // test for atomic read
    Version *C = new Version();
    C->txn_id.store(0);
    C->read_ts.store(6);

    // read before

    int a;
    C->AtomicRead(4, a);
    std::cout << "read before " << a << std::endl;

    // read after
    C->AtomicRead(4, a);
    std::cout << "read after " << a << std::endl;




}