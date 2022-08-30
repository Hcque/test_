
// lock contention , basic mode

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
    // std::atomic<Node *> next;
    Node* next;
    // std::mutex lk;
};

struct List
{
    std::atomic<Node *> head;
    // Node* head;
    // std::mutex mutex_;
};

// void insert(int value)
void insert(List* list, int value)
{
    // std::cout << value << std::endl;
    //  list->mutex_.lock();
    Node* n = new Node;
    n->value = value;

    while (1)
    {

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

    n->next = prev;
    // prev->next = n;
    if  ( list->head.compare_exchange_strong(prev, n ) ) return ;
    }


    // cur->lk.unlock();
    // prev->lk.unlock();
    // list->mutex_.unlock();
}



// std::thread each(insert, &l, )

void delete_(List *list, int value)
{

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
    Node *cur = l->head;
    int cnt = -1;
    while (cur)
    {
        // std::cout << cur->value << std::endl;
        // cval = cur->value;
        // assert(cval -1 == prev);
        // prev = cval;
        cnt ++ ;

        cur = cur->next;
    }
    std::cout << "cnt : " << cnt << std::endl;

}