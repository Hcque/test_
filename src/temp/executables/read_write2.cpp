
#include <atomic>
#include <vector>
#include <iostream>
#include <thread>

const int N = 5;
std::atomic<int> cnt(N);
std::vector<int> data;

void reader(int id)
{
    for (;;)
    {
        while (atomic_fetch_sub(&cnt, 1) <= 0)
        {
            std::atomic_fetch_add(&cnt, 1);
        }
        if (data.size() == 8) break;

        if ( !data.empty()) std::cout << "reader: " << std::to_string(*data.rbegin()) << std::endl;
        std::atomic_fetch_add(&cnt, 1);
    }
}

void writer()
{
    for (int i = 0; i < 8; i ++ )
    {
        while ( std::atomic_fetch_sub(&cnt, N+1)  != N )
        {
            std::atomic_fetch_add(&cnt, N+1);
        }
        data.push_back(i);
        std::cout << "writer " << i << std::endl;
        std::atomic_fetch_add(&cnt, N+1);
    }
}

int main()
{
    std::vector<std::thread> v;
    for (int i = 0; i < N; i ++ ) v.emplace_back(reader, i);
    v.emplace_back(writer);
    for (auto& t : v)
        t.join();
}








