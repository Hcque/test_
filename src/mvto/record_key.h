#pragma once

#include "record_layout.h"
//////////// Key

// struct WareHouse;
// struct District;

struct WareHouseKey
{
    uint16_t w_key;
    WareHouseKey() : w_key(0) {}
    WareHouseKey(uint16_t w_key) : w_key(w_key) {}
    bool operator<(const WareHouseKey &other) const { return w_key < other.w_key; }
    bool operator==(const WareHouseKey &other) const { return w_key == other.w_key; }

    uint32_t get_raw_key() { return w_key; }
    static WareHouseKey create_key(uint16_t w_id) 
    {
        WareHouseKey k;
        k.w_key = w_id;
        return k;
    }
    static WareHouseKey create_key(WareHouse &w) 
    {
        WareHouseKey k;
        k.w_key = w.w_id;
        return k;
    }

    size_t hash() const noexcept { return w_key; }
};

struct DistrictKey
{
    union
    {
        struct 
        {
            uint32_t d_id : 8;
            uint32_t w_id : 16;
        };
        uint32_t d_key;
    };
    DistrictKey() : d_key(0) {}
    DistrictKey(uint32_t d_key) : d_key(d_key) {}
    uint32_t get_raw_key() { return d_key; }
    bool operator<(const DistrictKey &other) const { return d_key < other.d_key; }
    bool operator==(const DistrictKey &other) const { return d_key == other.d_key; }

    static DistrictKey create_key(uint8_t d_w_id, uint16_t d_id) 
    {
        DistrictKey k;
        k.w_id = d_w_id;
        k.d_id = d_id;
        return k;
    }
    // static WareHouseKey create_key(const WareHouse &w) 
    // {
    //     WareHouseKey k;
    //     k.w_key = w.w_id
    //     return k;
    // }

    size_t hash() const { return d_key; }

};

