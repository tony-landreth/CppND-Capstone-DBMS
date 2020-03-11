#ifndef PLANNODE_H
#define PLANNODE_H
#include "schema_loader.h"
#include<string>

class PlanNode
{
  public:
    virtual std::vector<std::string> next() = 0;
    virtual ~PlanNode(){};
    virtual void rewind() = 0;
    TableSchema schema;
    int tableSize; //TODO: move tableSize into TableSchema
};

#endif
