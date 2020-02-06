#ifndef PLANNODE_H
#define PLANNODE_H
#include<string>

class PlanNode
{
  public:
    virtual std::vector<std::vector<std::string> > next() = 0;
};

#endif
