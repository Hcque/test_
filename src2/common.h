#pragma once

// #include "mvto.h"
// #include "value.h"

#include <unordered_map>
#include <cassert>
#include <vector>
#include <mutex>
#include <iostream>
#include <cstring>
#include <cstdint>

#include "timestamp_manager.h"

using timestamp_t = uint64_t;


struct Version{

    void *rec;
    Version *prev{nullptr};
    uint64_t rdts;
    uint64_t wtts = 0;
    bool is_deleted_;
    Version(){}

    void update_wtts(timestamp_t curr_time)
    {
        wtts = std::max(wtts, curr_time);
    }
};


template <typename Value>
struct NewestVersion{

    NewestVersion(void * rec): rec(rec) { val = new Value(); }
    void *rec;
    bool is_deleted_;
    Value *val;
};
template <typename Version>
struct Value
{
    Version *version;
    std::mutex latch_;
    Value() {
        version = new Version();
    }

    void lock() { latch_.lock();}
    void unlock() { latch_.unlock(); }

};

TimestampManager tsm;
#define start_ts (tsm.CurrentTime())

Version* get_correct_version(Value<Version>* val)
{
    Version* version = val->version;
    while ( version != nullptr && version->wtts > start_ts) 
    {
        std::cout << version->wtts << std::endl;
        version = version->prev;
    }
    return version;
    // may be nullptr
}


using Key = uint64_t;

std::unordered_map<Key, NewestVersion<Value<Version>>> newest_version_list;

using Val = uint64_t;


void Insert(Key key, Val val_to_inserted)
{
    std::cout << "[Insert] " << key << std::endl;
    tsm.IncrementTime();

    auto it = newest_version_list.find(key);
    if (it == newest_version_list.end())
    {
        Val *rec = new Val{};
        *rec = val_to_inserted;
        newest_version_list.emplace(key, reinterpret_cast<void *>(rec));
    }
    else{
        Value<Version> *val = it->second.val;
        assert(val != nullptr);
        val->lock();
        Version *version = get_correct_version(val);
        assert(version != nullptr);

        Version *new_version = new Version();
        new_version->prev = version;
        val->version = new_version;
        Val *to_inserted = new Val();
        *to_inserted = val_to_inserted;
        new_version->rec = to_inserted;
        version->update_wtts(start_ts);
        val->unlock();
    }
}

int Read(Key key, Val& val)
{
    tsm.IncrementTime();
    std::cout << "[Read] " << key  << " ";
    auto it = newest_version_list.find(key);
    int cnt = 0;
    if (it == newest_version_list.end())
    {
        std::cout << 0 << std::endl;
    }
    else{
        // it->second : NewestVersion
        Version* version = it->second.val->version;
        while (version != nullptr)
        {
            cnt ++ ;
            std::cout << "[Read] version ts:" << version->wtts << std::endl;
            version = version->prev;
        }
        std::cout << "[Read] cnt: " << cnt << std::endl;
    }

    return cnt;
}




