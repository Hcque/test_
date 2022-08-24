#progma once

#include <vector>
#include <unordered_set>

#include "spin_latch.h"

namespace mynoise
{

typedef uint64 timestamp_t;

class TimestampManager
{
public:
    TimestampManager();
    ~TimestampManager();

    timestamp_t CheckTimestamp() { return time_ ++ ; }
    timestamp_t CurrentTime() { return time_.load(); }

    timestamp_t BeginTransaction()
    {
        timestamp_t start_time;
        {
            ScopedSpinLatch running_guard(&cur_running_txns_latch_);

            start_time = time_++;
            cur_running_txns_.emplace(start_time);
        }
        return start_time;
    }

    void RemoveTransaction(timestamp_t timestamp);

    bool RemoveTransactions(const std::vector<timestamp_t> &timestamps);

private:
    std::atomic<uint32_t> time_;
    std::unordered_set<timestamp_t> cur_running_txns;
    mutable tbb::spin_mutex cur_running_txns_latch_;

};


}