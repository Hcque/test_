#pragma once

#include <atomic>

template <typename Protocol>
class TimestampManager
{
using timestamp_t = uint64_t;
  public:

  // disallow copy and move;
  // TimestampManager(const TimestampManager &other) = delete;
  // TimestampManager& operator=(const &TimestampManager other) = delete;


  std::unique_ptr<Protocol> BeginTransaction()
  {
    IncrementTime();
    return std::make_unique<Protocol> (CurrentTime());
  }

  

  timestamp_t CurrentTime() const { return ts.load(); } //.load(); }
  void IncrementTime() 
  {
      ts ++ ;
  }

  // timestamp_t ts;
  std::atomic<timestamp_t> ts{1};

};


