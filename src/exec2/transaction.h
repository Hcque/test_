#pragma once

#include <algorithm>
#include <cassert>
#include <memory>

template <typename Protocol>
class Transaction
{
  public:

    Transaction() {}
    ~Transaction() {}

    void abort()
    {
        // protocol->abort();
        // protocol->set_new_ts();
    }

  private:
    std::unique_ptr<Protocol> protocol = nullptr;
    // read set
    std::vector<uintptr_t> read_set_, write_set_;
    // write set

};
