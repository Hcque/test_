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


struct CustomerKey
{
    union{
        struct 
        {
            uint32_t c_id : 32;
            uint8_t d_id : 8;
            uint16_t w_id : 16;
        };
        uint16_t c_key;

    };
    CustomerKey() : c_key(0) {}
    CustomerKey(uint16_t c_key) : c_key(c_key) {}
    bool operator<(const CustomerKey &other) const { return c_key < other.c_key; }
    bool operator==(const CustomerKey &other) const { return c_key == other.c_key; }

    uint32_t get_raw_key() { return c_key; }
    static CustomerKey create_key(uint16_t w_id, uint8_t d_id, uint32_t c_id) 
    {
        CustomerKey k;
        k.w_id = w_id;
        k.d_id = d_id;
        k.c_id = c_id;
        return k;
    }
    static CustomerKey create_key(const Customer &c) 
    {
        CustomerKey k;
        k.w_id = c.c_w_id;
        k.d_id = c.c_d_id;
        k.c_id = c.c_id;
        return k;
    }

    size_t hash() const noexcept { return c_key; }
};




struct OrderKey
{
    union{
        struct 
        {
            uint32_t o_id : 32;
            uint8_t d_id : 8;
            uint16_t w_id : 16;
        };
        uint16_t o_key;

    };
    OrderKey() : o_key(0) {}
    OrderKey(uint16_t o_key) : o_key(o_key) {}
    bool operator<(const OrderKey &other) const { return o_key < other.o_key; }
    bool operator==(const OrderKey &other) const { return o_key == other.o_key; }

    uint32_t get_raw_key() { return o_key; }
    static OrderKey create_key(uint16_t w_id, uint8_t d_id, uint32_t o_id) 
    {
        OrderKey k;
        k.w_id = w_id;
        k.d_id = d_id;
        k.o_id = o_id;
        return k;
    }
    static OrderKey create_key(const Order &o) 
    {
        OrderKey k;
        k.w_id = o.o_w_id;
        k.d_id = o.o_d_id;
        k.o_id = o.o_id;
        return k;
    }

    size_t hash() const noexcept { return o_key; }
};


struct NewOrderKey
{
    union{
        struct 
        {
            uint32_t o_id : 32;
            uint8_t d_id : 8;
            uint16_t w_id : 16;
        };
        uint16_t o_key;

    };
    NewOrderKey() : o_key(0) {}
    NewOrderKey(uint16_t o_key) : o_key(o_key) {}
    bool operator<(const NewOrderKey &other) const { return o_key < other.o_key; }
    bool operator==(const NewOrderKey &other) const { return o_key == other.o_key; }

    uint32_t get_raw_key() { return o_key; }
    static NewOrderKey create_key(uint16_t w_id, uint8_t d_id, uint32_t o_id) 
    {
        NewOrderKey k;
        k.w_id = w_id;
        k.d_id = d_id;
        k.o_id = o_id;
        return k;
    }
    static NewOrderKey create_key(const NewOrder &o) 
    {
        NewOrderKey k;
        k.w_id = o.no_w_id;
        k.d_id = o.no_d_id;
        k.o_id = o.no_o_id;
        return k;
    }

    size_t hash() const noexcept { return o_key; }
};


struct OrderLineKey
{
    union{
        struct 
        {
            uint8_t ol_number : 8;
            uint32_t o_id : 32;
            uint8_t d_id : 8;
            uint16_t w_id : 16;
        };
        uint16_t ol_key;
    };

    OrderLineKey() : ol_key(0) {}
    OrderLineKey(uint16_t ol_key) : ol_key(ol_key) {}
    bool operator<(const OrderLineKey &other) const { return ol_key < other.ol_key; }
    bool operator==(const OrderLineKey &other) const { return ol_key == other.ol_key; }

    uint32_t get_raw_key() { return ol_key; }
    static OrderLineKey create_key(uint8_t ol_number, uint16_t w_id, uint8_t d_id, uint32_t o_id) 
    {
        OrderLineKey k;
        k.ol_number = ol_number;
        k.w_id = w_id;
        k.d_id = d_id;
        k.o_id = o_id;
        return k;
    }
    static OrderLineKey create_key(const OrderLine &ol) 
    {
        OrderLineKey k;
        k.ol_number = ol.ol_number;
        k.w_id = ol.ol_w_id;
        k.d_id = ol.ol_d_id;
        k.o_id = ol.ol_o_id;
        return k;
    }

    size_t hash() const noexcept { return ol_key; }
};




