
#include <atomic>
#include <cstdlib>


namespace MVTO {

class Tuple
{
  public:
    Tuple() {}
    // Tuple(const Tuple &other) 
    // {
    //     key_ = other.key_;
    //     value_ = other.value_;
    //     next_ = other.next_;
    // }
    Tuple(uint64_t key, uint64_t value) : key_(key), value_(value) {}

//   private:
    uint64_t key_, value_;
    // std::atomic<uint64_t> begin_timestamp_, end_timestamp_;
    std::atomic<Tuple *> next_ ; // = nullptr;
};


} // namespace MVTO