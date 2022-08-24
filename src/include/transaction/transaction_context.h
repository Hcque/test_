#pragma once

#include <vector>


namespace mynoise::transaction
{

class TransactionContext
{
public:
    TransactionContext()
    {

    }
    ~TransactionContext()
    {

    }

    timestamp_t StartTime() const { return start_time_; }
    timestamp_t FinishTime() const { return finish_time_.load(); }

    storage::UndoRecord *UndoRecordForUpdate();
    storage::UndoRecord *UndoRecordForInsert();
    storage::UndoRecord *UndoRecordForDelete();

    void RegisterAbortAction() {}
    void RegisterCommitAction() {}
    

private:
    std::forward_list<int> undo, redo;
    const timestamp_t start_time_;
    std::atomic<timestamp_t> finish_time_;
    //Redo, Undo buffer
    bool aborted;

};

}