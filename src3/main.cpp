
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <set>
#include <list>

std::chrono::milliseconds interval = std::chrono::milliseconds(1000);

using txn_id_t = uint64_t;
using rid_t = uint64_t;
using RID = rid_t;

txn_id_t smallest = 1;

class Transaction;

// 1 from txnid -> Trans : must has a global list: from the txn manager

// 

enum class LockType { SHARED = 0, EXCLUSIVE, UNLOCK };

struct LockManager
{
    std::mutex m_;
    bool is_enable = 1;

    std::unordered_map<txn_id_t, std::vector<txn_id_t>> wait_for_;
    bool *visit;
    bool *rec_stack;

    struct LockRequest
    {
        LockType request_type_;
        txn_id_t txn_id_;

    };

    struct GrantedTxns
    {
        LockType type_;
        std::set<LockRequest> granted_txns_;
    };

    std::unordered_map<RID, GrantedTxns *> lock_table_;

    /////////// lock interface //////////////

    void LockShared(Transaction *txn, const RID &rid);
    void LockExclusive(Transaction *txn, const RID &rid);
    void LockUpgrade(Transaction *txn, const RID &rid);
    void Unlock(Transaction *txn, const RID &rid);
    

    //////////   deadlock detection /////////////

    // std::unordered_map<rid_t, std::vector<txn_id_t>> lock_table_;

    LockManager()
    {
        detection_thread = new std::thread(&LockManager::RunCycleDetetcion, this);
        std::cout << "[logmanager constr] \n";
    }

    ~LockManager()
    {
        detection_thread->join();
        delete detection_thread;
    }

    void AddEdge(txn_id_t t1, txn_id_t t2);
    void RemoveEdge(txn_id_t t1, txn_id_t t2);
    std::vector<std::pair<txn_id_t, txn_id_t>> GetEdgeList()
    {
        std::vector<std::pair<txn_id_t,txn_id_t>> ans;
        for (auto it = wait_for_.begin(); it != wait_for_.end(); ++ it)
        {
            auto startnode = it->first;
            for (auto endnode: it->second)
            {
                ans.push_back({startnode, endnode});
            }
            
        }
        return ans;
    }
    bool HasCycle(txn_id_t *id)
    {
        for (int i = 0; i < 100; i ++ ) visit[i] = 0;

        if (HasCycleUtil(smallest, visit, rec_stack))
        {
            *id = 1;
            return 1;
        }
        return 0;
    }

    void RunCycleDetetcion()
    {
        std::cout << "[RunCycle] \n";
        while (is_enable)
        {
            std::this_thread::sleep_for(interval);

            // TODO
            std::unique_lock<std::mutex> lock(m_);
            std::cout << "[RUN] 1.build G, 2.dfs 3.free" << std::endl;
            run();

        }
    }
    void BuildGraph()
    {


    }

    

    void run()
    {
        //1 build the graph from lock_table_; ? how
        for (auto it = lock_table_.begin(); it != lock_table_.end(); ++ it)
        {
            auto &granted_txns = it->second;
            // txnid
        }

        // new visit
        visit = new bool[1000];
        rec_stack = new bool[1000];

        //2 run 
        txn_id_t newest_txn_id;
        if( HasCycle(&newest_txn_id) )
        {
            // set that transaction ABORT,
            // delete from lockset, locktable
        }

        // delete
        delete visit;
        delete rec_stack;

        return;
    }

    bool HasCycleUtil(uint64_t v, bool visit[], bool *rec_stack)
    {
        if (visit[v] == 0)
        {
            visit[v] = 1; 
            rec_stack[v] = 1;

            for (int j: wait_for_[v])
            {
                if (visit[j] == 0 && HasCycleUtil(j, visit, rec_stack) == 1) return 1;
                else if (rec_stack[j] == 1) return 1;
            }
        }
        rec_stack[v] = 0;
        return 0;
    }

    std::thread *detection_thread;
};


void LockManager::AddEdge(txn_id_t t1, txn_id_t t2)
{
    for (auto& e : wait_for_)
    {
        if (e.first == t1) 
        {
            e.second.push_back(t2);
            return;
        }
    }
}
void LockManager::RemoveEdge(txn_id_t t1, txn_id_t t2)
{
    auto& v = wait_for_[t1];
    for (auto it = v.begin(); it != v.end(); ++ it)
    {
        if (*it == t2) 
        {
            v.erase(it);
            return;
        }
    }

}


struct AAA{
    AAA(int a, int b): aaa(a), bbb(b) {}
    int aaa = 0;
    int bbb = 1;

};

int main()
{
    // LockManager lkm;
    // lkm.RunCycleDetetcion();

    std::list<int> L;
    L.push_back(1);
    L.push_back(2);

    L.erase(L.begin());
    // L.emplace_back(1,2);
    std::cout <<  *L.begin();



}