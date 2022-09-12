#pragma once
#pragma once

// #include "record_key.h"

//////////// record
struct DistrictKey;
struct WareHouseKey;
struct CustomerKey;
struct OrderKey;
struct NewOrderKey;
struct OrderLineKey;

struct District
{
    using Key = DistrictKey;
    // data
    uint16_t d_w_id;
    uint8_t d_id;
    uint32_t d_next_o_id;
    double d_tax;
    double d_ytd;
    void generate(uint16_t d_w_id_, uint8_t d_id_)
    {
        d_w_id = d_w_id_;
        d_id = d_id_;
        d_next_o_id = 3001;
        d_tax = rand() % 100;
        d_ytd = rand() % 100;
    }
};

struct WareHouse
{
    using Key = WareHouseKey;

    // data

    uint16_t w_id;
    double w_tax;
    double w_ytd;
    void generate(uint16_t w_id_)
    {
        w_id = w_id_;
        w_tax = rand() % 100;
        w_ytd = rand() % 100;
    }

};


struct Customer
{
    using Key = CustomerKey;
    uint16_t c_w_id;
    uint8_t c_d_id;
    uint32_t c_id;
    uint16_t c_payment_cnt;
    uint16_t c_delivery_cnt;

    double c_credit_limit;
    double c_discount;
    double c_balance;
    double c_ytd_payment;

    void generate();

};

struct Order
{
    using Key = OrderKey;
    uint16_t o_w_id;
    uint8_t o_d_id;
    uint32_t o_id;

    uint32_t o_c_id;
    void generate();
    
};

struct NewOrder
{
    using Key = NewOrderKey;
    uint16_t no_w_id;
    uint8_t no_d_id;
    uint32_t no_o_id;

};

struct OrderLine
{
    using Key = OrderLineKey;
    uint8_t ol_number;
    uint16_t ol_w_id;
    uint8_t ol_d_id;
    uint32_t ol_o_id;
    //TODO

};










