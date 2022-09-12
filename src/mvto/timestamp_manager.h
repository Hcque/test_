
#include <atomic>


class TimestampManager
{
using timestamp_t = uint64_t;
  public:

  timestamp_t CurrentTime() const { return ts.load(); }
  void IncrementTime() 
  {
      ts ++ ;
  }


  std::atomic<timestamp_t> ts{0};

};


