#pragma once

#include <vector>

namespace testbed
{

class AbstractPlanNode
{
    std::vector<const AbstractPlanNode *> children_;
};


} // namespace testbed
