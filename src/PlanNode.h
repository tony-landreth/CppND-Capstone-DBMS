#ifndef PLANNODE_H
#define PLANNODE_H
#include "schema_loader.h"
#include<string>

class PlanNode
{
  public:
    // Method declarations
    virtual std::vector<std::string> next() = 0;
    virtual ~PlanNode(){};
    virtual void rewind() = 0;

    // Variable declarations
    TableSchema schema;
    std::vector<std::string> keys;

  private:
    TableSchema schema_;
};

#endif
