#ifndef EMPTY_PLAN_H
#define EMPTY_PLAN_H
#include<vector>
#include<string>
#include "PlanNode.h"

class EmptyPlan : public PlanNode{
  public:
    std::vector<std::string> next();
};

#endif
