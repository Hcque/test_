#pragma once

#include "plan.h"
#include "common.h"

namespace testbed
{

class AbstractExecutor;

class ExecutionContext
{
    uint64_t num;
};





// suanzi
class AbstractExecutor
{
public:
AbstractExecutor(ExecutionContext *cxt) : exec_cxt_(cxt) {}
    virtual void Init() = 0;
    virtual bool Next(Tuple *tuple, RID *rid) = 0;
    ExecutionContext *exec_cxt_;
};

// different category of snanzi

class TableIterator;


// SeqScanExecutor, 
class SeqScanExecutor : public AbstractExecutor
{
public:
    SeqScanExecutor(ExecutionContext *cxt, const SeqscanPlanNode *plan)
    : AbstractExecutor(cxt)
    , plan_(plan) {}
    void Init() override;
    bool Next(Tuple *tuple, RID *rid) override;

    const SeqscanPlanNode* plan_;
    TableIterator *iterator_;
};

void SeqScanExecutor::Init() 
{
 // get table inerator
 // table_id from plan,
 // table from catalog
}


bool SeqScanExecutor::Next(Tuple *tuple, RID *rid) {
    while (1)
    {
        // iterator_ ++ ;
    }

    // lock

    // column Eval

    // unlock

    // write into output,
}

// InsertExecutor, 
class InsertExecutor : public AbstractExecutor
{
public:
    InsertExecutor(ExecutionContext *cxt, const InsertPlanNode *plan)
    : AbstractExecutor(cxt)
    , plan_(plan) {}
    void Init() override;
    bool Next(Tuple *tuple, RID *rid) override;

    const InsertPlanNode* plan_;
    TableIterator *iterator_;
};

void InsertExecutor::Init()  
{
 // get table inerator
 // table_id from plan,
 // table from catalog
}

bool InsertExecutor::Next(Tuple *tuple, RID *rid) 
{
 return 0;
}



class ExecutionFactory
{
    public:
    static std::unique_ptr<AbstractExecutor> CreateExecutor(ExecutionContext *cxt, const AbstractPlanNode *plan)
    {
        switch (plan->GetType())
        {
            case PlanType::SeqScan :
            {
                return std::make_unique<SeqScanExecutor>(cxt, 
                dynamic_cast<const SeqscanPlanNode *>(plan) );
            }
            case PlanType::Insert :
            {
                return std::make_unique<SeqScanExecutor>(cxt, 
                dynamic_cast<const SeqscanPlanNode *>(plan) );
            }


        }

    }
};


} // namespace testbed



