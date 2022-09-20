
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>


bool ready = 0;
std::condition_variable cv;

std::mutex latch_;

void worker()
{
    std::unique_lock<std::mutex> lock(latch_);

    while (!ready)
    {
        cv.wait(lock);
    }
    std::cout << "working threads start\n";
    // cv.notify_one();

    std::cout << "working threads done\n";

}


int main()
{

    std::cout << "main start\n";
    std::thread w(worker);
    {
    std::unique_lock<std::mutex> lock(latch_);
    ready = 1;
    }
    cv.notify_one();

    w.join();
    std::cout << "main done\n";

}









