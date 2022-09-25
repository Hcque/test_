#pragma once

#include <vector>
#include <climits>
#include <cstdint>
#include <cassert>


using RID = uint64_t;

class Transaction;



enum TypeId
{
    INVALID = 0,
    BOOLEAN, 
    TINYINT,
    SMALLINT,
    INTEGER,
    BIGINT,
    DECIMAL,
    VARCHAR,
    TIMESTAMP
};

class Value
{
  public:
  explicit Value(const TypeId type)  : manage_data_(0), type_id_(type)
  {
    size_.len_ = UINT_MAX;
  }
  Value() : Value(TypeId::INVALID) {}

  inline TypeId GetTypeId() const { return type_id_; }

  protected:
  union Val {
    int8_t boolean_;
    int8_t tinyint_;
    int16_t smallint_;
    int32_t integer_;
    int64_t bigint_;
    double decimal_;
    uint64_t timestamp_;
    char *varlen_;
    const char *const_varlen_;
  } value_ ;

  union {
    uint32_t len_;
    TypeId elem_type_id_;
  } size_;

  bool manage_data_;
  TypeId type_id_;

};


class Schema;

class Tuple
{
  public:
  Tuple() = default;

  Value GetValue(const Schema *schema, uint32_t column_idx) const;

  public:
    bool allocated_{0};
    uint32_t size_{0};
    char *data_{nullptr};

};
