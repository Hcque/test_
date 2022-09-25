#pragma once

#include <vector>

#include "expression.h"

namespace testbed
{

using table_oid_t = uint64_t;


class Schema
{

};

enum class PlanType { 
    SeqScan, 
    IndexScan,
    
    Insert,
    Update,
    Delete,
    Aggregate,
    Limit,
    Distinct,

    NestedLoopJoin,
    NestedIndexJoin,
    HashJoin
};

class AbstractPlanNode
{
public:
    AbstractPlanNode(const Schema* out_schema, std::vector<const AbstractPlanNode *> &&children) 
    : out_schema_(out_schema)
    , children_(std::move(children)) {}

    virtual PlanType GetType() const  = 0;

    const Schema *out_schema_;
    std::vector<const AbstractPlanNode *> children_;
};

class SeqscanPlanNode : public AbstractPlanNode
{
public:
    SeqscanPlanNode(const Schema *output, const AbstractExpression *predicate, table_oid_t table_oid)
    : AbstractPlanNode(output, {})
    , predicate_(predicate)
    , table_oid_(table_oid) {}

    virtual PlanType GetType() const override { return PlanType::SeqScan; }

    private:
    const AbstractExpression *predicate_;
    table_oid_t table_oid_;
};


class InsertPlanNode : public AbstractPlanNode
{
public:
    InsertPlanNode(const AbstractPlanNode *child, table_oid_t table_oid)
    : AbstractPlanNode(nullptr, {child})
    , table_oid_(table_oid) {}

    InsertPlanNode(std::vector<std::vector<Tuple>>&& values, table_oid_t table_oid)
    : AbstractPlanNode(nullptr, {})
    , values_(values)
    , table_oid_(table_oid) {}

    virtual PlanType GetType() const override { return PlanType::Insert; }

    private:
    std::vector<std::vector<Tuple>> values_;
    table_oid_t table_oid_;
};



} // namespace testbed
