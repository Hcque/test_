
#include <iostream>

#include "lock_manager.h"


LockManager lkm;
int main()
{
    std::cout << "[main] start ... " << std::endl;
    txn_id_t txnid = 1;
    rid_t rid = 1;

    for (int i = 1; i < 10; i ++ )
    {
        txnid = i;
        lkm.LockShared(txnid, rid);
    }
    for (int i = 10; i < 20; i ++ )
    {
        txnid = i;
        lkm.LockShared(txnid, rid);
    }
    for (int i = 1; i < 10; i ++ )
    {
        txnid = i;
        lkm.LockUpgrade(txnid, rid);
    }
    for (int i = 1; i < 20; i ++ )
    {
        txnid = i;
        lkm.Unlock(txnid, rid);
    }
    std::cout << "[main] end ... " << std::endl;

}