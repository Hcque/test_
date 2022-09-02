#pragma once

#include <memory>
#include <map>
#include <cassert>

#include "record_layout.h"
#include "database.h"




enum LogType {INSERT, UPDATE, DELETE };


template <typename Record>
class LogRecord
{
  public:
    LogRecord() = default;
    LogRecord(LogType type, std::unique_ptr<Record> rec_ptr)
        : type_(type)
        , rec_ptr_(std::move(rec_ptr)) {}

  // private:
    LogType type_;
    std::unique_ptr<Record> rec_ptr_;
};


template <typename Record>
struct RecordToWS
{
    using type = std::map<typename Record::Key , LogRecord<Record> > ;
};


class WriteSet
{
  public:
    explicit WriteSet(Database &db) : db(db), ws_tuple() {}
    ~WriteSet() { clearall(); }

    template <typename Record>
    typename RecordToWS<Record>::type& get_ws()
    {
      return get<typename RecordToWS<Record>::type>(ws_tuple);
    }

    template <typename Record>
    Record *apply_update_to_writeset(typename Record::Key rec_key)
    {
      LogRecord<Record> * cur_logrec_ptr = lookup_logrecord<Record>(rec_key);
      if (cur_logrec_ptr)
      {
        switch (cur_logrec_ptr.type_)
        {
          case INSERT: return cur_logrec_ptr.get();
          case UPDATE: return cur_logrec_ptr.get();
          case DELETE: assert(0);
          default: assert(0);
        }
      }

    }

    template <typename Record>
    Record *apply_insert_to_writeset(typename Record::Key rec_key)
    {
      LogRecord<Record> * cur_logrec_ptr = lookup_logrecord<Record>(rec_key);
      if (cur_logrec_ptr)
      {
        switch (cur_logrec_ptr.type_)
        {
          case INSERT: assert(0);
          case UPDATE: return cur_logrec_ptr.get();
          case DELETE: assert(0);
          default: assert(0);
        }
      }
    }


    template <typename Record>
    Record *apply_delete_to_writeset(typename Record::Key rec_key)
    {
      LogRecord<Record> * cur_logrec_ptr = lookup_logrecord<Record>(rec_key);
      if (cur_logrec_ptr)
      {
        switch (cur_logrec_ptr.type_)
        {
          case INSERT: return cur_logrec_ptr.get();
          case UPDATE: return cur_logrec_ptr.get();
          case DELETE: assert(0);
          default: assert(0);
        }
      }
    }

    bool apply_to_db()
    {
      apply_writeset_to_database<District>();
      apply_writeset_to_database<WareHouse>();
      return 1;
    }


    void clearall()
    {
      clear_write_set<District>();
      clear_write_set<WareHouse>();
    }

    template <typename Record>
    void clear_write_set()
    {
      typename RecordToWS<Record>::type &ws = get_ws<Record>();
      //clear cache
      ws.clear();
    }

    void apply_update_to_db();
    void apply_insert_to_db();
    void apply_delete_to_db();

    template <typename Record>
    void apply_writeset_to_database()
    {
      typename RecordToWS<Record>::type &ws = get_ws<Record>();
      for (auto it = ws.begin(); it != ws.end(); ++ it)
      {
        auto key = it->first;
        LogRecord<Record>& lr = (it->second);
        // LogRecord<Record>& lr = std::move(it->second);
        switch(lr.type_)
        {
          case INSERT: db.insert_record<Record>(key, std::move(lr.rec_ptr_)); break;
          case UPDATE: db.update_record<Record>(key, std::move(lr.rec_ptr_)); break;
          case DELETE: 
          //  db.delete_record<Record>(lr); 
          // cache dealloc
            break;
          default: assert(0);
        }
      }

      ws.clear();
    }

  private:
    Database &db;

    using TT = TypeTuple<
        RecordToWS<WareHouse>::type, 
        RecordToWS<District>::type
                >;
    TT ws_tuple;


    template <typename Record>
    LogRecord<Record> *lookup_logrecord(typename Record::Key rec_key)
    {
      typename RecordToWS<Record>::type &ws = get_ws<Record>();
      auto it = ws.find(rec_key);
      if (it == ws.end()) return nullptr;
      return it->second;
    }

    template <typename Record>
    LogRecord<Record> *create_logrecord(typename Record::Key rec_key)
    {
      // typename RecordToWS<Record>::type &ws = get_ws<Record>();
      // auto it = ws.find(rec_key);
      // if (it == ws.end()) return nullptr;
      // return it->second;
    }


};

