// #include "transaction/transaction_manager.h"

// #include <vector>

// namespace mynoise::transaction
// {

//     void TimestampManager::RemoveTransaction(timestamp_t timestamp)
//     {
//         ScopedSpinLatch guard(&cur_running_txns_latch_);
//         cur_running_txns_.erase(timestamp);
//     }

//     bool TimestampManager::RemoveTransactions(const std::vector<timestamp_t> &timestamps)
//     {
//         ScopedSpinLatch guard(&cur_running_txns_latch_);
//         for (const auto &timestamp: timestamps)
//         {
//             cur_running_txns_.erase(timestamp)
//         }
//         return cur_running_txns_.empty();
//     }



// }