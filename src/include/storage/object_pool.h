#pragma once

#include <queue>

// #include ""

template <typename T>
class ByteAlignedAllocator
{

}


template <typename T, typename Allocator>
class ObjectPool
{
public:
    ObjectPool()
    {

    }
    ~ObjectPool()
    {

    }

    T *Get()
    {

    }

    void Release()
    {

    }

private:
    std::queue<T *> reuse_queue_;




};
