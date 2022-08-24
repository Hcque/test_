#include <stdint.h>
#include <iostream>
#include <immintrin.h>
#include <unordered_map>
#include <vector>

namespace simd
{

struct Bitwidth
{
    static constexpr const uint32_t 
#if defined(__AVX512F__)
    VALUE = 512;
#elif defined(__AVX2__)
    VALUE = 256;
#else 
    VALUE = 256;
#endif
};

template <typename T>
struct Lane
{
    // number of elements in the SIMD lane
    static constexpr const uint32_t COUNT = Bitwidth::VALUE / (sizeof(T) * 8) ;
};

#define SIMD_TOP_LEVEL

#if defined(__AVX512F__)
// #include ""
#elif defined(__AVX2__)
//
#endif

#undef SIMD_TOP_LEVEL

#define ALWAYS_INLINE __attribute__((always_inline))


template <typename Value>
class ReadWriteElement
{
    uintptr_t read_ptr_, write_ptr_;
    uint8_t type_;
    Value *value_;

};

using table_id_t = uint32_t;

template <typename Key, typename Value>
class ReadWriteSet
{
  public:
    using Table = std::unordered_map<Key, ReadWriteElement<Value> >;
    Table &GetTable(table_id_t table_id) { return read_write_set_[table_id] ; }

    private:
     std::unordered_map<table_id_t, uint32_t> read_write_set_;
};

template <typename Key, typename Value>
class WriteSet
{
  public:
    using ChainElement = std::pair<Key, typename std::unordered_map<Key, ReadWriteElement<Value> >::iterator>;
    std::vector<ChainElement> &GetTable(const table_id_t table_id) { return write_set_[table_id]; }

  private:
    std::unordered_map<table_id_t, std::vector<ChainElement> > write_set_;
};

class Vec256b
{
    public:
        Vec256b() = default;
        explicit Vec256b(const __m256i &reg) : reg_(reg) {}
        ALWAYS_INLINE operator __m256i() const { return reg_; }
        void Store(void *ptr) const { _mm256_storeu_si256(reinterpret_cast<__m256i *>(ptr), Reg()); }

    protected:
        const __m256i &Reg() const { return reg_; }
    public:
        const __m256i reg_;
};

class Vec4 : public Vec256b
{
  public:
    Vec4() = default;
    static constexpr uint32_t Size() { return 4; }
};

// insruction set design

} // simd





int main()
{
    using namespace simd;
    __m256i b;
    auto A = Vec256b(b);
    // std::cout << A.reg_ << std::endl;

    WriteSet ws;
}