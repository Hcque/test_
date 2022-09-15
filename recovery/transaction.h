#pragma once

#include <unordered_set>


enum class TransactionState { INVALID = 0, BEGIN, COMMIT, ABORT };

using txn_id_t = uint64_t;
using rid_t = uint64_t; // record id


class Transaction
{
public:
    txn_id_t txnid;
    TransactionState state_;

    std::unordered_set<rid_t> shared_lock_set_, exclusive_lock_set_;


};
