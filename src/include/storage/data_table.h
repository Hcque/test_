#progma once

#include <algorithm>

// main storage interface
class DataTable
{
public:
    class SlotIterator
    {
      public:
        const TupleSlot &operator*() const { return current_slot_; }
        const TupleSlot *operator->() const { return &current_slot_; }
        SlotIterator &operator++()
        {
            RawBlock *b = current_slot_.GetBlock();
            if (slot_num_ < max_slot_num_)
            {
                current_slot_ = {b, slot_num_};
            }
            else
            {
                block_index_++;
                UpdateFromNextBlock();
            }
            return *this;
        }

        SlotIterator operator++(int)
        {
            SlotIterator copy = *this;
            operator++();
            return copy;
        }
        bool operator==(const SlotIterator &other) const { return block_index_ == other.block_index_ && slot_num_ == other.slot_num_; }
        bool operator!=(const SlotIterator &other) const { return !(this->operator==(other) ) ; }

        SlotIterator() = default;
      private:

        void UpdateFromNextBlock()
        {

            

        }

        friend class DataTable;
        const DataTable *table_ = nullptr;
        uint64_t block_index_ = 0, end_index_ = 0;
        uint32_t slot_num_ = 0, max_slot_num_ = 0;
        TupleSlot current_slot_ = {nullptr, 0};
    };

    DataTable(){}
    ~DataTable();

    void Scan();

    void Insert();
    void Delete();

    SlotIterator begin() const { return {this}; }
    SlotIterator end() const { return SlotIterator(); }

};