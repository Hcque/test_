
#include <mutex>

#include "database.h"
#include "write_set.h"

class Transaction
{
    public:

    Transaction(Database &db, WriteSet ws) : db(db), ws(ws) { mutex_.lock(); }
    ~Transaction() { mutex_.unlock(); }
    void abort() { ws.clearall(); }
    bool commit()
    {
        if (ws.apply_to_db())
        {
            return 1;
        }
        else
        {
            abort();
            return 0;
        }
    }

    enum Result { SUCCESS, FAIL, ABORT };

    template <typename Record>
    Result prepare_record_for_insert(Record* &rec_ptr)
    {
        rec_ptr_ = ws.apply_insert_to_writeset<Record>();
        return (rec_ptr_ == nullptr) ? FAIL : SUCCESS;
    }
    template <typename Record>
    Result prepare_record_for_update(Record*& rec_ptr, Record::Key key)
    {
        rec_ptr_ = ws.apply_update_to_writeset<Record>();
        return (rec_ptr_ == nullptr) ? FAIL : SUCCESS;
    }

    private:
    Database& db;
    WriteSet ws;
    std::mutex mutex_;

}