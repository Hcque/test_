#pragma once

#include <mutex>


using timestamp_t = uint64_t; 

struct Version
{
    Version *prev;
    bool is_deleted;
    timestamp_t read_ts, write_ts;
    void *rec;

};

template <typename Version>
struct Value
{

    std::mutex latch_;
    void lock() { latch_.lock(); }
    void unlock() { latch_.unlock(); }

};


enum class ReadWriteType { READ = 0, INSERT, UPDATE, DELETE };

template <typename Value>
class ReadWriteElement
{
    void *read_rec, *write_rec;
    ReadWriteType type_ = ReadWriteType::READ;
    bool is_new;
    Value *val;

};








