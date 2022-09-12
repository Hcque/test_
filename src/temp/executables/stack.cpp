
#include <atomic>
#include <thread>
#include <iostream>
#include <cstring>
#include <cassert>

template <typename T>
struct node
{
    T data;
    node *next;
    node(const T& data) : data(data), next(nullptr) {}
};

template <typename T>
class stack
{

  public:
    std::atomic<node<T> *> head;
    void push(const T& data)
    {
        node<T> *new_node = new node<T>(data);

        new_node->next = head.load(std::memory_order_acquire);

        // std::cout << head.load() << std::endl;


        while ( ! std::atomic_compare_exchange_weak(
                                &head, 
                                &new_node->next,
                                new_node
        ))
        {
            // ;
            std::cout << head.load() << std::endl;
        }

    }


};

std::thread ths[10000];
int vis[100001];

stack<int> s;
void run(int a, int b)
{
    for (int i = a; i < b; i ++) s.push(i);
}

int main(int argc, char *argv[])
{
    
    int NUM = atoi(argv[1]);

    for (int i = 0; i < NUM ; i ++ )
    {
        int a= 10*i, b = 10*(i+1);
        ths[i] = std::thread(run, a, b);
    }
    for (int i = 0; i < NUM ; i ++ )
        ths[i].join();
        // s.push(i);
    // s.push(1);
    // std::cout << s.head.load() << std::endl;

    // check the ans
    node<int> *tmp = s.head.load();
    std::memset(vis, 0, sizeof(vis));

    while (tmp)
    {
        std::cout << tmp->data << std::endl;
        if (vis[tmp->data]) assert(0);
        vis[tmp->data] = 1;
        tmp = tmp->next;
    }
    return 0;

}




