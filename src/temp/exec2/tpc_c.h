#pragma once

#include <unordered_map>
#include <cstdlib>
#include <iostream>

#include "data_table.h"

enum RecordID
{
    UNUSED = 0,
    ITEM = 1,
    WAREHOUSE = 2,
    STOCK = 3,
    DISTRICT = 4,
    ORDER = 8,
    NEWORDER = 10,
    ORDERLINE = 11
};


struct Warehouse
{
    uint16_t w_id;
    double w_tax;
    double w_ytd;

    void generate(uint16_t w_id) 
    {
        w_id = w_id;
        w_tax = rand() % 1000;
        w_ytd = rand() % 1000;
    }
};

struct Stock
{
    uint16_t s_w_id;
    uint32_t s_i_id;
    int16_t s_quantity;
    uint32_t ytd;
};

struct District
{
    uint8_t d_id; // 1
    uint32_t next_o_id; // 4
    double d_tax; //sizeof(double)
    double d_ytd;
};

struct NewOrder
{
    uint16_t no_w_id; //2
    uint8_t no_d_id; //1
    uint32_t no_o_id; // 4
};

struct OrderLine
{
    uint16_t ol_w_id;
    uint8_t ol_d_id;
    uint32_t ol_o_id;
    uint8_t ol_number;
    uint32_t ol_i_id;
    uint16_t ol_supply_w_id;
    uint8_t ol_quantity;
    double ol_amount;
};


using TableID = uint64_t;

template <typename Record>
inline TableID get_id()
{
    if (std::is_same<Record, Warehouse>::value)
    {
        return WAREHOUSE;
    }
    else if (std::is_same<Record, NewOrder>::value)
    {
        return ORDER;
    }
}



