#progma once

#include <tbb/spin_mutex>

class ScopedSpinLatch
{
public:
    ScopedSpinLatch(tbb::spin_mutex *const latch): latch_(latch)
    {
        latch_.lock();
    }
    ~ScopedSpinLatch() { latch_.unlock(); }

private:
    tbb::spin_mutex *latch_;
};