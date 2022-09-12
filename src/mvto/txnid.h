
struct TxnID
{
    uint64_t txnid_;
    union
    {
        uint32_t txn_counter_;
        uint32_t thread_id_;
    };

    TxnID() : txnid_(0) {}
    TxnID(uint64_t txnid) : txnid_(txnid) {}
    TxnID(uint32_t txn_counter, uint32_t thread_id) : txn_counter_(txn_counter), thread_id_(thread_id) {}


};
