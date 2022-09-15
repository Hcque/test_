


using lsn_t = uint64_t;
using txn_id_t = uint64_t;

using RID = uint64_t;


// define log record header

class Tuple;


class LogRecord
{

  inline std::string ToString() const
  {

  }

  private:
    uint32_t size{0};
    lsn_t lsn_{INVALID_LSN};
    txn_id_t txn_id_{INVALID_TXN_ID};
    LogRecordType log_record_type_{ INVALID_TYPE};

    // for case insert
    RID insert_rid_;
    Tuple insert_tuple_;

    static const int HEADER_SIZE = 20;

};


class LogManager
{

  public:
    void RunFlushThread();

  private:
    std::atomic<lsn_t> next_lsn_;
    std::atomic<lsn_t> presistent_lsn_;

    char* log_buffer_;
    char* flush_buffer_;

    std::mutex latch_;

    std::thread *flush_thread_;
    std::conditional_variable cv_;

};

class LogRecovery
{
  public:
    void Redo();
    void Undo();

    char* log_buffer_;
    int offset_;

    std::unordered_map<txn_id_t, lsn_t> active_txn_;



};

