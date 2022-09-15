#pragma once

#include "record_key.h"
#include "record_layout.h"


class NewOrderTxn
{
  public:
    NewOrderTxn()
    {
        input.generate();
    }

    struct 
    {
        void generate()
        {

        }

    } input;
    
    void run(Transaction& txn)
    {

        WareHouse *w = new WareHouse();
        Val val;
        txn.Read(1, val);


    }
};
