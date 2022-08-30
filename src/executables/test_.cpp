
#include <atomic>
#include <iostream>


using namespace std;

class RWLock
{

  public:
    RWLock() : cnt(0) {}

    void init() { cnt = 0; }

    void lock_shared() 
    {
        int64_t expected;
        while (1) 
        {
            expected = load_acquire(cnt);
            if (expected >= 0 )
                return;
        }
    }

    void lock_upgrade()
    {
        int64_t expected;
        while (1)
        {
            
        }

    }

  private:
    int cnt = 0;

};


class version
{
  public:
    alignas(64) uint64_t read_ts;
    uint64_t write_ts;
    version *prev_;
    void *rec;
    bool deleted;

};

template <typename Version_>
struct Value 
{
    using Version = Version_;
    RWLock rwl;
    Version *version;

    void lock() { rwl.lock(); }
    void unlock() { rwl.unlock(); }

    bool is_empty() { return version->deleted && version->prev == nullptr; }

    void trace_version_chain()
    {
        Version *tmp = version;
        while (tmp != nullptr)
        {
            tmp = tmp->prev_;
        }
    }


}


int main()
{
    
    int v = 0, count = 0;
    while __atomic_load_n(&v,  __ATOMIC_ACQUIRE)
    {

    }
    std::cout << ans;

    return 0;
}
