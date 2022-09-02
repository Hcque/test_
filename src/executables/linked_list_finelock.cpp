
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
    std::mutex lk;
};

struct List
{
    // std::atomic<Node *> head;
    Node* head;
    std::mutex mutex_;
};

void insert(List* list, int value)
{
    Node* n = new Node;
    n->value = value;

    // list->mutex_.lock();
    // std::cout << n << std::endl;
    Node *old_prev;
    Node* prev = list->head;
    prev->lk.lock();
    auto cur = prev->next;
    cur->lk.lock();
    // list->mutex_.unlock();

    while (cur && cur->value < value)
    {
        old_prev = prev;

        prev = cur;
        cur = cur->next;
        prev->lk.unlock();
        cur->lk.lock();
    }
    n->next = cur;
    prev->next = n;

    prev->lk.unlock();
    cur->lk.unlock();
    // list->mutex_.unlock();
}

std::vector<std::thread> ths;

int main(int argc, char *argv[])
{
    List *l = new List();
    l->head = new Node(-2);
    // l->head->value = -
    l->head->next = new Node(1<<30);
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
    int cnt = 0;
    while (cur)
    {
        std::cout << cur << std::endl;
        cnt ++ ;
        cur = cur->next;
    }
    std::cout << "cnt : " << cnt << std::endl;

}