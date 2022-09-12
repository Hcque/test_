
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
#include <climits>


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

    void AtomicRead(TxnID cur_txn_id, int *out)
    {
        // std::cout << "R" << std::endl;
        mutex_.lock();
        // std::cout << "R" << std::endl;
        // uint64_t read_latch = 0; // old
        uint64_t old_read_ts = read_ts.load();
        bool less = (old_read_ts < cur_txn_id);
    
        *out = val;
        if (less) 
        {
            read_ts.store( cur_txn_id );
        }
        

        mutex_.unlock();
    }

    void AtomicWrite(uint64_t cur_txn_id, const int in)
    {
        mutex_.lock();
        // std::cout << "W" << std::endl;
        if(cur_txn_id <= read_ts.load())
        {
            mutex_.unlock();
            return;
        }

        assert(cur_txn_id > read_ts.load());
        if (txn_id != 0)
        {
            mutex_.unlock();
            return;
        }
        assert(txn_id == 0);
        txn_id.store(cur_txn_id);
        mutex_.unlock();

        // allocate
        Version *version = new Version();
        version->txn_id.store( cur_txn_id ); 
        version->begin.store(cur_txn_id);
        version->end.store(UINT_MAX);
        version->read_ts.store(cur_txn_id);

        version->val = in;

        mutex_.lock();
        end.store(cur_txn_id);
        next.store(version);
        txn_id.store(0);
        version->txn_id.store(0);
        mutex_.unlock();
    }
};

struct List
{
    std::atomic<Version *> chain;
    int inplace_for_update{0};
    std::atomic<uint64_t> txn_id, read_ts, begin, end;
    std::mutex value_lock_;

    void Read(uint64_t txn_id, int *out)
    {
        value_lock_.lock();
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
            *out = cur->val;
        }
        else
        {
            // read the current
            *out = inplace_for_update;
        }
        value_lock_.unlock();
    }

    // not check read-ts
    void AtomicWrite(uit64_t cur_txn_id)
    {
        value_lock_.lock();
        // check valid for write
        if (read_ts.load() > cur_txn_id)
        {
            value_lock_.unlock();
            return;
        }

        assert(cur_txn_id > read_ts.load());
        if (txn_id != 0)
        {
            value_lock_.unlock();
            return;
        }
        assert(txn_id == 0);
        txn_id.store(cur_txn_id);

        // allocate
        Version *version = new Version();
        // txn_id.store( cur_txn_id ); 
        begin.store(cur_txn_id);
        end.store(UINT_MAX);
        read_ts.store(cur_txn_id);

        version->val = in;

        end.store(cur_txn_id);
        next.store(version);
        txn_id.store(0);
        version->txn_id.store(0);
        value_lock_.unlock();     

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



void Read(Version *C, uint64_t id, int *out)
{
    C->AtomicRead(id, out);
}

void Write(Version *C, uint64_t id, int in)
{
    C->AtomicWrite(id, in);
}

int OUT[40000];
int main(int argc, char *argv[])
{

    Version *C = new Version();
    C->txn_id.store(0);
    C->read_ts.store(6);

    int NUM = atoi(argv[1]);

    for (int i = 0; i < NUM; i ++ )
    {
        if (i % 2 == 0)
            ths.emplace_back(Read,C, i+1, &(OUT[i]));
        else
            ths.emplace_back(Write,C, i+1, i+1);
    }

    for (int i = 0; i < NUM; i ++ ) ths[i].join();
    std::cout << "DONE === \n";

}