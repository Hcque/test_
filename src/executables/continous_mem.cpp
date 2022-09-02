
#include <cstdint>
#include <cassert>
#include <mutex>
#include <queue>
#include <vector>


#define SZ 32

class RecordBufferSegment
{
  public:
    char *Reserve(const uint32_t size)
    {
        auto result = content_ + size_;
        size_ += size;
        return result;
    }

    RecordBufferSegment *Reset()
    {
        size_ = 0;
        return this;
    }

//   private:
    char content_[SZ];
    uint32_t size_;
};

template <typename T, typename Allocator>
class ObjectPool
{
  public:
    T* Get()
    {
        T *t;
        if (reuse_que_.size() == 0) 
        {
            t = alloc_.New();
            curr_size_ ++ ;
            if (curr_size_ > set_limit_) assert(0);
        }
        else
        {
            t = reuse_que_.front(); reuse_que_.pop();
            alloc_.Reuse(t);
        }
        return t;

    }

    void Release()
    {

    }

  private:
    std::mutex mutex_;
    std::queue<T *> reuse_que_;
    int set_limit_, reuse_limit_;
    int curr_size_;

    Allocator alloc_;

};

class Version
{

};

class RecordBufferSegmentAllocator
{
    void *New() { return new RecordBufferSegment(); }
    void Delete(RecordBufferSegment *ptr) { delete ptr; }
    void Reuse(RecordBufferSegment *ptr) { ptr->Reset(); }
};

class UndoBuffer
{
  public:
    class Iterator
    {
      public:
      Iterator(std::vector<RecordBufferSegment *>::iterator curr_segment,
            int offset) 
            : curr_segment_(curr_segment)
            , offset_(offset) {}

        friend class UndoBuffer;

        Version &operator*() const
        {
            return *(reinterpret_cast<Version *> ( (*curr_segment_)->content_ + offset_ ) );
        }
        Version *operator->() const
        {
            return reinterpret_cast<Version *> ( (*curr_segment_)->content_ + offset_ );
        }

      private:
        std::vector<RecordBufferSegment*>::iterator curr_segment_;
        uint32_t offset_;

    };

    Iterator begin() { return {buffers_.begin(), 0}; }
    Iterator end() { return { buffers_.end(), 0}; }
  private:
    std::vector<RecordBufferSegment *> buffers_;

};


int main()
{
    UndoBuffer *b = new UndoBuffer();

    return 0;


}