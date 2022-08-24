#pragma once

#include <queue>

namespace mynoise::storage {

class TupleSlot
{
    TupleSlot()
    {

    }

    ~TupleSlot()
    {

    }


};


class RawBlock
{

};

class BlockAllocator
{

};

// Dynamically allocated UndoBUffer: holds UndoRecords
// RedoBuffer is fix sized

class UndoBuffer
{
public:
    class Iterator
    {
    public:
        UndoRecord &operator*() const 
        {
            return *reinterpret_cast<UndoRecord *>((*curr_segment_)->bytes_ + segment_offset_);
        }

        UndoRecord *operator->() const 
        {
            return reinterpret_cast<UndoRecord *>((*curr_segment_)->bytes_ + segment_offset_);
        }

        Iterator &operator++()
        {
            UndoRecord& me = this->operator*();
            segment_offset_ += me.Size();
            if (segment_offset_ == (*curr_segment_)->size() )
            {
                segment_offset_ = 0;
                ++ curr_segment_;
            }
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator copy = *this;
            operator++();
            return copy;
        }

        bool operator==(const Iterator &other) const
        {
            return curr_segment_ == other.curr_segment_ && segment_offset_ == other.segment_offset_;
        }

        bool operator!=(const Iterator &other) const { return !(*this == other ); }


    private:
        friend class UndoBuffer;
        std::vector<RecordBufferSegment *>::iterator curr_segment_;
        uint32_t segment_offset_;

    };

    UndoBuffer() 
    {

    }
    ~UndoBuffer();

    Iterator begin() { }
    Itertor end();
    bool Empty();

private:



};

class RedoBuffer 
{

};









} // namespace mynoise::storage

