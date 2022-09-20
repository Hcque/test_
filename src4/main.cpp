
#include <memory>
#include <iostream>

#include "executor.h"

namespace testbed
{

class ExecutionFactory
{
    static std::unique_ptr<AbstractExecutor> CreateExecutor(ExecutionContext *cxt, const AbstractPlanNode *plan);
};


class ExecutionEngine
{
public:
    ExecutionEngine()
    {

    }
    void Executing()
    {
        // auto 

    }
};

}


using namespace testbed;

int main()
{
    std::cout << "[main] start ... \n";
    ExecutionEngine().Executing();

}
