#pragma once

#include <vector>
#include <climits>
#include <cstdint>
#include <cassert>

#include "common.h"



class AbstractExpression
{
public:
    AbstractExpression(std::vector<const AbstractExpression *> &&children, TypeId ret_type)
    : children_(std::move(children))
    , ret_type_(ret_type) {}
   virtual Value Evaluate( const Tuple *tuple , const Schema *schema) const = 0;
   virtual Value EvaluateJoin( const Tuple *left_tuple , const Schema *left_schema,
                            const Tuple *right_tuple, const Schema *right_schema) const = 0;
   virtual Value EvaluateAggregate( const std::vector<Value> &group_bys, const std::vector<Value> &aggregates) const = 0;
   const AbstractExpression *GetChildAt(uint32_t child_idx) const
   {
     return children_[child_idx];
   }

    std::vector<const AbstractExpression *> children_;
    TypeId ret_type_;
};


// constant value, 

// class ConstantValueExpression : public AbstractExpression
// {
//   public:
//    explicit ConstantValueExpression(const Value value) 
//    : AbstractExpression({}, value.GetTypeId())
//    , value_(value) {}

//    Value Evaluate( const Tuple *tuple , const Schema *schema) const override
//    {
//     return value_;
//    }

//    Value EvaluateJoin( const Tuple *left_tuple , const Schema *left_schema,
//                             const Tuple *right_tuple, const Schema *right_schema) const override
//     {
//         return value_;

//     }
//    Value EvaluateAggregate( const std::vector<Value> &group_bys, const std::vector<Value> &aggregates) const override
//    {
//     return value_;
//    }

//   private:
//     Value value_;
// };

// // column value,
// class ColumnValueExpression : public AbstractExpression
// {
//   public:
//    explicit ColumnValueExpression(TypeId ret_type, uint32_t tuple_idx, uint32_t col_idx) 
//    : AbstractExpression({}, ret_type)
//    , tuple_idx_(tuple_idx)
//    , col_idx_(col_idx)
//    {

//    }

//    Value Evaluate( const Tuple *tuple , const Schema *schema) const override
//    {
//     return tuple->GetValue(schema, col_idx_);
//    }

//    Value EvaluateJoin( const Tuple *left_tuple , const Schema *left_schema,
//                             const Tuple *right_tuple, const Schema *right_schema) const override
//     {
//         return ( tuple_idx_ == 0 ) ? left_tuple->GetValue(left_schema, col_idx_)
//         : right_tuple->GetValue(right_schema, col_idx_);
//     }
//    Value EvaluateAggregate( const std::vector<Value> &group_bys, const std::vector<Value> &aggregates) const override
//    {
//     assert(0);
//    }

//   private:
//   uint32_t col_idx_, tuple_idx_;
// };


// // class ValueFactory
// // {
// //   public:
// //   static inline GetBooleanvalue() { return ; }


// // };


// // comprasion value
// enum class ComparisonType { Equal, NotEqual, LessThan, LessThanOrEqual, GreaterThan, GreaterThanOrEqual};

// class ComparisonExpression : public AbstractExpression
// {
//   private:
//     CmpBool PerformComparison(const value &lhs, const Value &rhs) const 
//     {
//       switch (comp_type_)
//       {
//         case ComparisonType::Equal:
//           return lhs.ComparisonEquals(rhs);
//           case ComparisonType::NotEqual:
//           return lhs.ComparisonNotEquals(rhs);
//       }
//     }
//   public:
//    Value Evaluate( const Tuple *tuple , const Schema *schema) const override
//    {


//    }

//    Value EvaluateJoin( const Tuple *left_tuple , const Schema *left_schema,
//                             const Tuple *right_tuple, const Schema *right_schema) const override
//     {

//     }
//    Value EvaluateAggregate( const std::vector<Value> &group_bys, const std::vector<Value> &aggregates) const override
//    {
//     assert(0);
//    }

//    std::vector<AbstractExpression *> children_;
//    ComparisonType comp_type_;
// };

// // // aggregate value
// class AggregateValueExpression : public AbstractExpression
// {
//   public:
//    Value EvaluateAggregate( const std::vector<Value> &group_bys, const std::vector<Value> &aggregates) const override
//    {
//     return is_group_by_term_ ? group_bys[term_idx_] : aggregates[term_idx_];
//    }

//   private:
//     bool is_group_by_term_;
//     uint32_t term_idx_;
// };


// // anther approach is just in time compliation


