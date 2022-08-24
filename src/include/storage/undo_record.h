#pragma once

#include <vector>

namespace mynoise::storage {

class UndoRecord
{
public:
    UndoRecord()
    {

    }

    ~UndoRecord()
    {

    }

    std::atmoic<UndoRecord *> &Next() { return next_; }
    const std::atmoic<UndoRecord *> &Next() const { return next_; }

    std::atmoic<transaction::timestamp_t> &Timestamp() { return begin_time_; }
    const std::atmoic<transaction::timestamp_t> &Timestamp() const { return begin_time_; }


private:
    std::atmoic<UndoRecord *> next_;
    std::atmoic<timestamp_t> begin_time_;
    DataTable *table_;
    TupleSlot slot_;

};

} // namespace 