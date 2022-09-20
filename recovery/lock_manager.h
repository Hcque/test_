#pragma once

#include <mutex>
#include <vector>
#include <deque>
#include <cassert>
#include <unordered_map>
#include <condition_variable>

#include "transaction.h"

// concepts of isolation level

// prevention machism

// lock request queue

// notify txns (who is waitting): using cv;

// 2 phase locking, strict two phase locking

// transaction state

// lock_manager.h

using txn_id_t = uint64_t;
using rid_t = uint64_t; // record id

enum class LockType { SHARED = 0, EXCLUSIVE };

struct LockRequest
{
    LockRequest(txn_id_t txnid, rid_t rid, LockType type): txnid(txnid), rid(rid),type_(type) {}
    txn_id_t txnid;
    rid_t rid; 
    LockType type_ = LockType::SHARED;
};

// struct LockRequestQueue
// {
//     LockRequestQueue() {}
//     std::deque<LockRequest> que;

// };


struct LockRequestQueue
{
    bool writer_entered_{0};
    txn_id_t writer_txnid;

    std::unordered_map<txn_id_t, rid_t> que;
};

class LockManager
{
  public:
    std::unordered_map<txn_id_t, Transaction> active_txns_;

    void LockShared(txn_id_t txnid, rid_t rid)
    {
        std::unique_lock<std::mutex> lk(latch_);

        auto it = lock_table_.find(rid);
        // not locked by others
        if (it == lock_table_.end())
        {
           lock_table_.emplace( rid, LockRequestQueue{}  );
           lock_table_[rid].que.emplace(txnid, rid);
           assert( active_txns_[txnid].shared_lock_set_.count(rid) == 0);

           active_txns_[txnid].shared_lock_set_.emplace(rid);
        }
        else // locked by other txns
        {
            auto& que = lock_table_[rid].que;
            // wait
            // 1 conflict & younger :abort
            if (lock_table_[rid].writer_entered_)
            {
                if (lock_table_[rid].writer_txnid < txnid)
                {
                    // exclusive adn younger
                    active_txns_[txnid].state_ = TransactionState::ABORT;
                    return;
                }
                else
                {
                    // 2 conflict & older : wait signal
                    //1 create cv
                    cv_table_.emplace(rid, std::make_shared<std::condition_variable>() );
                    // 2 cv wait
                    // cv_table_[rid]->wait(lk);
                    cv_table_[rid]->wait(lk, [&] { return lock_table_.find(rid) == lock_table_.end() ||
                                                        !lock_table_[rid].writer_entered_; } );
                    if (lock_table_.find(rid) == lock_table_.end())
                    {
                        active_txns_[txnid].state_ = TransactionState::ABORT;
                        return;
                    }
                }
            }

            // locked by shared lock
            active_txns_[txnid].exclusive_lock_set_.emplace(rid);
            que.emplace(txnid, rid);
        }

    }
    void LockExclusive(txn_id_t txnid, rid_t rid)
    {

    }
    void LockUpgrade(txn_id_t txnid, rid_t rid)
    {
        // change the state
        // 1 assert has shared lock

        std::unique_lock<std::mutex> lk(latch_);

        assert(lock_table_.find(rid) != lock_table_.end());
        assert(lock_table_[rid].writer_entered_ == 0);

        assert( lock_table_[rid].que.count(txnid) != 0);


        // 2 test if conflict:
        // not conflict : 
        //       change the state
        if ( !lock_table_[rid].writer_entered_)
        {
            active_txns_[txnid].shared_lock_set_.erase(rid);
            active_txns_[txnid].exclusive_lock_set_.emplace(rid);
            lock_table_[rid].writer_entered_ = 1;
        }
        else
        {
            // if has conflict: youger: abort
            //    older : wait

            // 2 conflict & older : wait signal
                //1 create cv
                cv_table_.emplace(rid, std::make_shared<std::condition_variable>() );
                // 2 cv wait
                // cv_table_[rid]->wait(lk);
                cv_table_[rid]->wait(lk, [&] { return lock_table_.find(rid) == lock_table_.end() ||
                                                    !lock_table_[rid].writer_entered_; } );
                if (lock_table_.find(rid) == lock_table_.end())
                {
                    active_txns_[txnid].state_ = TransactionState::ABORT;
                    return;
                }

                 active_txns_[txnid].shared_lock_set_.erase(rid);
                active_txns_[txnid].exclusive_lock_set_.emplace(rid);
                lock_table_[rid].writer_entered_ = 1;

        }

    }
    void Unlock(txn_id_t txnid, rid_t rid)
    {
        // pop
        // notify all 

        std::unique_lock<std::mutex> lk(latch_);

        active_txns_[txnid].shared_lock_set_.erase(rid);
        active_txns_[txnid].exclusive_lock_set_.erase(rid);
        lock_table_[rid].que.erase(txnid); // erase txnid 

        cv_table_[rid]->notify_all();

    }

  private:
    // std::atomic<bool> enable_cycle_detection_;
    // std::thread *cycle_detection_thread_;
    // aits-for graph
    // std::unordered_map<txn_id_t, std::vector<txn_id_t>> wait_for_;


    std::unordered_map<rid_t, LockRequestQueue > lock_table_;
    std::unordered_map<rid_t, std::shared_ptr<std::condition_variable> > cv_table_;
    std::mutex latch_;

};







