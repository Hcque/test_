
// lock contention , basic mode

// swap old with old is not correct

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <cassert>

struct Node
{
    Node() {}
    Node(int v) : value(v), next(nullptr) {}
    int value;
    std::atomic<Node *> next;
    // Node* next;
    // std::mutex lk;
};

struct List
{
    std::atomic<Node *> head;
    // Node* head;
    // std::mutex mutex_;
};

void insert(List* list, int value)
{
    Node* n = new Node;
    n->value = value;

    while (1)
    {

    Node* old = list->head.load()->next.load();
    Node* prev = list->head.load();
    // prev->lk.lock();
    // auto cur = prev->next;
    // cur->lk.lock();
    // list->mutex_.unlock();

    // while (cur)
    // {
        // if (cur->value > value) 
        // {
            // break;
        // }
        // prev = cur;
        // cur = cur->next;
    // }

    n->next.store(prev->next.load());
    // prev->next.store(n);
    if  ( list->head.load()->next.compare_exchange_strong(old, n ) ) return ;
    // prev->next
    }


    // cur->lk.unlock();
    // prev->lk.unlock();
    // list->mutex_.unlock();
}



std::vector<std::thread> ths;

int main(int argc, char *argv[])
{
    List *l = new List();
    l->head.store(new Node(-1));
    // l->head->value = -1;

    // std::this_thread::sleep_for( std::chrono::seconds(2) );

    int NUM = atoi(argv[1]);

    for (int i = 0; i < NUM; i ++ )
    {
        ths.emplace_back(insert, l, i);
    }

    for (int i = 0; i < NUM; i ++ ) ths[i].join();
    std::cout << "DONE === \n";

        // insert(&l, i);
    Node *cur = l->head.load();
    int cnt = 0;
    while (cur)
    {
        // std::cout << cur << std::endl;
        cnt ++ ;

        cur = cur->next.load();
    }
    std::cout << "cnt : " << cnt << std::endl;

}