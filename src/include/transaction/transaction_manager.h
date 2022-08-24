#progma once

#include <vector>


namespace mynoise::transaction {

class TransactionManager
{
public:
    TransactionManager()
    {

    }

    TransactionContext Begin(TransactionContext *txn);
    void Abort(TransactionContext *txn);
    void Commit(TransactionContext *txn);
private:
    TimestampManager *timestampManager;

};

}