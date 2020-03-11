#ifndef PLANNODE_H
#define PLANNODE_H
#include "schema_loader.h"
#include<string>

class PlanNode
{
  public:
    virtual std::vector<std::string> next() = 0;
    TableSchema schema;
};

#endif
