#pragma once

#include "plan.h"

namespace testbed
{

class ExecutionContext
{
    uint64_t num;
};

// suanzi
class AbstractExecutor
{
public:
    virtual void Init() = 0;
    virtual void Next() = 0;

    ExecutionContext *exec_cxt_;
};

// different category of snanzi



} // namespace testbed