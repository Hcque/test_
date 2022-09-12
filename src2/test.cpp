#pragma once

#include "common.h"

void test1()
{
    //////// construct diff workloads for ts //////////
    for (int i = 1; i < 100; i ++ )
    {
        Key key = rand() % 10;
        Insert(key, key);

    }

    int cnt = 0;

    for (int i = 0; i < 10; i ++ )
    {
        Val val;
        cnt += Read(i, val);
    }
    std::cout << "[test1] cnt: " << cnt  << std::endl;

}

void test2()
{
    // for ()
}