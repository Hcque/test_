#pragma once

#include "record_layout.h"


class NewOrderTxn
{

  public:
    NewOrderTxn(uint16_t wid)
    {
        input.generate(wid);
    }

    struct Input
    {
        uint16_t w_id;
        uint8_t d_id;
        uint32_t c_id;
        uint8_t ol_cnt;

        struct 
        {
            uint16_t ol_supply_w_id;
            uint32_t ol_i_id;
            uint8_t ol_quantity;
        } items [20];


        void generate(uint16_t w_id0)
        {
            w_id = w_id0;
            d_id = rand() % 10;
            c_id = rand() % 1024;
            ol_cnt = rand() % 1024;
            for (int i = 1; i < 20 ; i ++ )
            {
                items[i-1].ol_i_id = rand() % 1024;
                // items[i-1].w_id = w_id;
                items[i-1].ol_quantity = rand() % 10;
            }
        }

    } input;


template <typename Transaction>
void run(Transaction& tx)
{
    // std::cout << "[run newordertxn]" << " start" << std::endl;

    typename Transaction::Result res;
    ///// 
    std::cout << "[run newordertxn]" << " warehouse" << std::endl;
    const WareHouse *w = nullptr;
    WareHouse::Key w_key = WareHouse::Key::create_key(input.w_id);
    res = tx.get_record(w, w_key);

    std::cout << "[run newordertxn]" << " district" << std::endl;
    District *d = nullptr;
    District::Key d_key = District::Key::create_key(input.w_id, input.d_id);
    res = tx.prepare_record_for_update(d, d_key);

    uint32_t o_id = d->d_next_o_id;
    d->d_next_o_id ++ ;

 ////// 3 prim key

    std::cout << "[run newordertxn]" << " customer" << std::endl;

    // cust get reocrd
    const Customer *c = nullptr;
    Customer::Key c_key = Customer::Key::create_key(input.w_id, input.d_id, input.c_id);
    res = tx.get_record(c, c_key);

    // insert for order // 3 prim key
    NewOrder *no = nullptr;
    NewOrder::Key no_key = NewOrder::Key::create_key(input.w_id, input.d_id, o_id);
    res = tx.prepare_record_for_insert(no, no_key);

    std::cout << "[run newordertxn]" << " order" << std::endl;

    Order *o = nullptr;
    Order::Key o_key = Order::Key::create_key(input.w_id, input.d_id, o_id);
    res = tx.prepare_record_for_insert(o, o_key);

    // get item,
    // update sock
    // insert orderline
    std::cout << "[run newtxn] end" << std::endl;

}

};
