
#include <atomic>
#include <cstdlib>
#include <mutex>


namespace MVTO {

class ReadWriteLock
{
  public:
    void Lock();
    void Unlock();
    // std::mutex mutex_;
};

class Version
{
  public:

    bool TryRead() //
    {
      if (begin_timestamp_ > txn_timestamp) return 0;
      if (end_timestamp_ < txn_timestamp) return 0;
      if (txn_id != 0) return 0;
      txnid = 0, tehn 
      read_timestamp = txn_timestamp;
      return 1;
    }


    bool TryWrite()
    {
      // find the place 
      auto tmp;
      while (tmp)
      {
        // atomic compare begin & end ts
        last begin
        
      }
      // creat new Version
      Version * newversion = new Version();
      last_begin->next_ = newversion;
      newversion->txn_id_ = txn_id;  // atomic
      newversion->begin = current_time;
      last->endTime = current time;

      // actual write take place

      newversion->txni   0; // atomic

      if (begin_timestamp_ > txn_timestamp) return 0;
      if (end_timestamp_ < txn_timestamp) return 0;
      // atomic set trans id

    }

    txn_id_t txn_id_;
    // std::atomic<uint64_t> begin_timestamp_, end_timestamp_;
    uint64_t begin_timestamp_, end_timestamp_, read_timestamp;
    Version *next_;
    Tuple *record;
}

class Tuple
{
  using txn_id_t = uint64_t;
  public:
    Tuple() {}

//   private:
    uint64_t key_;
    char value_[0];
};


} // namespace MVTO