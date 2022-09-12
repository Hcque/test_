#pragma once

#include <atomic>

class TimestampManager
{
using timestamp_t = uint64_t;
  public:

  // disallow copy and move;
  // TimestampManager(const TimestampManager &other) = delete;
  // TimestampManager& operator=(const &TimestampManager other) = delete;


  timestamp_t CurrentTime() const { return ts.load(); }
  void IncrementTime() 
  {
      ts ++ ;
  }


  std::atomic<timestamp_t> ts{1};

};


