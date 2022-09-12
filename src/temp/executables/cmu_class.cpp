
#include <atomic>
#include <vector>
#include <thread>
#include <iostream>

template <typename T>
T load(T& ptr) { return __atomic_load_n(&ptr, __ATOMIC_RELAXED); }

template <typename T>
T load_acquire(T& ptr) { return __atomic_load_n(&ptr, __ATOMIC_ACQUIRE); }

template <typename T, typename T2>
void store(T& ptr, T2 val) { return __atomic_store_n(&ptr, __ATOMIC_RELAXED); }

template <typename T, typename T2>
void store_release(T& ptr, T2 val) { return __atomic_store_n(&ptr, __ATOMIC_RELEASE); }

template <typename T, typename T2>
void compare_exchange(T& m, T& before, T2 after) { 
    return __atomic_compare_exchange_n(&m, &before, (T)after, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); 
}

template <typename Int1, typename Int2>
Int1 fetch_add(Int1& m, Int2 v, int memory_order = __ATOMIC_ACQ_REL)
{
    return __atomic_fetch_add(&m, v, memory_order);
}

template <typename T, typename T2>
T exchange(T& m, T2 after)
{
    return __atomic_exchange_n(&m, after, __ATOMIC_ACQ_REL);
}

class RWLock
{
  public:
    RWLock() : cnt(0) {}


  private:
    uint64_t cnt = 0;

};


std::vector<std::thread> v;

// std::atomic<int> cnt(0);
int cnt = 0;

void incr() 
{
    int c = __atomic_load_n(&cnt, __ATOMIC_ACQUIRE);
    c ++ ;
    __atomic_store_n(&cnt, c, __ATOMIC_RELEASE);
    // cnt.store(c);
}

// std::thread incr_th(incr);

int main(int argc, char *argv[])
{
    int N = atoi(argv[1]);

    for (int i = 0; i < N; i ++ )
    {
        v.emplace_back(incr);
    }
    for (int i = 0; i < N; i ++ )
    {
        v[i].join();
    }
    // std::cout << cnt.load() << std::endl;
    std::cout << cnt << std::endl;


}




















