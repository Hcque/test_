#pragma once

#include <atomic>

template <typename T>
T load_acquire(T &val)
{
    return __atomic_load_n(&val, __ATOMIC_ACQUIRE);
}


template <typename T, typename T2>
void store_release(T &val, T2 val2)
{
    __atomic_store_n(&val, (T)val2, __ATOMIC_RELEASE);
}


template <typename T, typename T2>
bool compare_exchange(T &m, T &before, T2 after)
{
    return __atomic_compare_exchange_n(&m, &before, (T)after, false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE);
}

template <typename Int1, typename Int2>
Int1 fetch_add(Int1 &m, Int2 v)
{
    return __atomic_fetch_add(&m, v, __ATOMIC_ACQ_REL);
}
