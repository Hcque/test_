
#include <condition_variable>
#include <mutex>

// writer priority

class ReadWriteLock
{
  public:
    ReadWriteLock(int R_num): MAX_R(R_num) {}

    ~ReadWriteLock() {}


    int MAX_R;
    std::mutex m_;
    std::condition_variable reader, writer;
    int read_cnt{0};
    bool writer_entered{0};

    void ReadLock()
    {
        std::unique_lock<std::mutex> lk(m_);
        while (writer_entered || read_cnt == MAX_R)
            reader.wait(lk);
        // read
        read_cnt ++ ;
    }

    void ReadUnlock()
    {
        std::lock_guard<std::mutex> guard(m_);
        read_cnt -- ;
        if (writer_entered && read_cnt == 0)
        {
                writer.notify_one();
        }
        if ( ! writer_entered && read_cnt == MAX_R - 2)
        {

          reader.notify_one();
          reader.notify_one();
        }
            

        // if (writer_entered)
        // {
        //     if (read_cnt == 0)
        //         writer.notify_one();
        // }
        // else
        // {
        //     if (read_cnt == MAX_R - 1) reader.notify_one();
            
        // }
    }

    void writeLock()
    {
        std::unique_lock<std::mutex> lk(m_);
        while (writer_entered)
        {
            reader.wait(lk);
        }
        writer_entered = 1;
        while (read_cnt > 0 )
        {
            writer.wait(lk);
        }

    }


    void writeUnlock()
    {
        std::lock_guard<std::mutex> guard(m_);
        writer_entered = 0;
        reader.notify_all();
    }

};