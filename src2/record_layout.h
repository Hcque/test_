#pragma once
#pragma once

// #include "record_key.h"

//////////// record
struct DistrictKey;
struct WareHouseKey;

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

