
#include <cstdint>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <map>
#include <iostream>
#include <chrono>

#include "lock_manager.h"
#include "readwrite_lock.h"


// the trace is txn access rid with 2 modes:
// 1)read
// 2)write

// EX.
// txnid 1 -> read rid 1
// txnid 2 ->write rid 1 // at this time we have read write lock.

struct A
{

};

std::condition_variable cv;
std::mutex m;
bool ready = 0;


void worker()
{
    std::unique_lock<std::mutex> lk(m);
    while (!ready)
        cv.wait(lk);

    std::cout << " worker processing " << std::endl;
    cv.notify_one();
}


int AAA = 0;
ReadWriteLock rwl(5);

void R()
{
    std::cout << "[READ]" << AAA << std::endl;
}

void W()
{
    std::cout << "[WRITE]" << AAA;

    AAA += 1;

    std::cout << " " <<  AAA << std::endl;
}


// diff RW cases

// R W R

// R W W R

// R MAX R W W R 
// R MAX R W R

// W R R W
// W R MAX R W 
// W R MAX R R W

////////////////////////////// test cases /////////////////////////////
// R W R
void test11()
{
    std::thread t1(R);
    std::this_thread::sleep_for( std::chrono::seconds(1) );

    // std::thread t2(W, 1);
    std::this_thread::sleep_for( std::chrono::seconds(1) );

    std::thread t3(R);
    std::this_thread::sleep_for( std::chrono::seconds(1) );

    t1.join();
    // t2.join();
    t3.join();

}

// R W W R
std::vector<std::thread> ths;
void test22(int SEED)
{
    srand(SEED);
    int cnt = 0;
    for (int i = 0; i < 1000; i ++ )
    {
        // if (rand() % 2 == 0) ths.emplace_back(R);
        // else ths.emplace_back(W), cnt ++ ;

    }
    for (int i = 0; i < 400; i ++ ) 
    {
        for (int j = 0; j < 3; j ++ )
            ths.emplace_back(R);
        for (int j = 0; j < 3; j ++ )
            ths.emplace_back(W), cnt ++;
        for (int j = 0; j < 2; j ++ )
            ths.emplace_back(R);
        for (int j = 0; j < 2; j ++ )
            ths.emplace_back(W), cnt ++;
        for (int j = 0; j < 4; j ++ )
            ths.emplace_back(R);
        for (int j = 0; j < 4; j ++ )
            ths.emplace_back(W), cnt ++;
        for (int j = 0; j < 5; j ++ )
            ths.emplace_back(R);
        for (int j = 0; j < 5; j ++ )
            ths.emplace_back(W), cnt ++ ;
        for (int j = 0; j < 6; j ++ )
            ths.emplace_back(R);
        for (int j = 0; j < 9; j ++ )
            ths.emplace_back(W), cnt ++ ;

    }
    // std::thread t1(R);
    // std::this_thread::sleep_for( std::chrono::seconds(1) );

    // for (int i = 0; i < 10000; i ++ ) ths.emplace_back(W);
    // std::thread t2(W, 1);
    // std::this_thread::sleep_for( std::chrono::seconds(1) );
    // for (int i = 0; i < 1000; i ++ ) ths.emplace_back(R);

    // std::thread t33(R);
    // std::this_thread::sleep_for( std::chrono::seconds(1) );

    for (auto& e: ths) e.join();
    std::cout << "[main] cnt" << cnt << std::endl;

}



int main(int argc, char *argv[])
{

    test22(atoi(argv[1]));



    /*
    std::thread th(worker);

    std::cout << " main" << std::endl;
    {
        std::lock_guard<std::mutex> lk(m);
        ready = 1;
    }
    cv.notify_one();

    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk);
    }

    th.join();
    std::cout << "back to main" << std::endl;
    */




}

