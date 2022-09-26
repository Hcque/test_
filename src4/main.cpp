
#include <memory>
#include <iostream>

#include "common.h"
// #include "expression.h"
#include "executor.h"

namespace testbed
{


class TransactionAbortException
{

};



class ExecutionEngine
{
public:
    ExecutionEngine()
   {

   }

    // disallow copy and move;
    ExecutionEngine(const ExecutionEngine &other) = delete;
    ExecutionEngine operator=(const ExecutionEngine& other) = delete;

    void Executing(const AbstractPlanNode *plan, ExecutionContext *cxt, 
                     std::vector<Tuple> *result_set, Transaction *txn)
    {
        auto executor = ExecutionFactory::CreateExecutor(cxt, plan);

        executor->Init();

        try{
        Tuple tuple;
        RID rid;
        while (executor->Next(&tuple, &rid))
        {
            if (result_set != nullptr)
            

            }

        }

        } catch (TransactionAbortException &e)
        {

        }

    }
};

}


using namespace testbed;

int main()
{
    std::cout << "[main] start ... \n";
    // ExecutionEngine{}.Executing();

}
