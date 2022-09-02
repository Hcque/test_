#pragma once

#include "atomic_wrapper.h"

class ReadWriteLock
{
  public:

    void initialize() { count_ = 0; }

    void lock()
    {
        int64_t expected;
        while (1)
        {
            expected = load_acquire(count_);
            if (expected == 0 && compare_exchange(count_, expected, -1)) return;
        }
    }

    void unlock()
    {
        fetch_add(count_, 1); // why not loop ??
    }

  private:
  int64_t count_ = 0;

};
