#pragma once

#include "record_layout.h"


void new_order_txn()
{

    Result res;

    ///// 

    WareHouse *w = nullptr;
    WareHouse::Key w_key = District::Key::create_key(wid);
    res = tx.get_record(w, w_key);

    uint32_t o_id = d->d_next_o_id;

    District *d = nullptr;
    District::Key key = District::Key::create_key(wid, did);
    res = tx.prepare_record_for_update(d, key);

    uint32_t o_id = d->d_next_o_id;
    d->d_next_o_id ++ ;

 //////


}