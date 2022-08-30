
#include <mutex>
#include <conditional_variable>

class ReadWriteLatch
{

  public:
    ReadWriteLatch() = default;
    void WLock();
    void WUnlock();
    void RLock();
    void RUnlock();
  private:
    std::mutex mutex_;
    std::conditional_variable writer_, reader_;
    uint32_t reader_cnt_{0};
    bool writer_entered;
};

