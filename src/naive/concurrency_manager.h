#poagma once

#include <mutex>
#include <conditional_variable>


class LockTable // ?
{
    public:


    private:
        int lock_var_;
        std::mutex latch;
        std::conditional_variable cond_var_;
};


class ConcurrencyManager
{
  public:
    void lock()
    {

    }
    void release()
    {

    }

  private:
    bool is_single_thread() { return 1; }
    bool has_lock = 0;
    static LockTable lock_table_;
};

