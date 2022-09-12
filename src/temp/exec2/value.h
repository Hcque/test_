#pragma once

#include <cstdint>
#include <mutex>

#include "atomic_wrapper.hpp"
#include "readwritelock.hpp"

namespace MVTO{

template <typename Record>
class Version
{
  public:
    uint64_t txid;
    uint64_t read_ts;
    uint64_t begin, end;
    Version *prev;
    Record *rec;
    bool deleted;

};


// template <typename Version_>
// struct Value 
// {
//     using Version = Version_;
//     RWLock rwl;
//     Version *version;

//     void initialize() { rwl.initialize(); version = nullptr; }
//     void lock() { rwl.lock(); }
//     void unlock() { rwl.unlock(); }
//     bool is_detached_from_the_tree() { return (version == nullptr); }
//     bool is_empty() { return (version->deleted || version->prev == nullptr); }

//     void trace_version_chain() 
//     {

//     }



// };


} // namespace MVTO









