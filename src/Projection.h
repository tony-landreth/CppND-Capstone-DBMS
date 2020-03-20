#ifndef PROJECTION_H
#define PROJECTION_H
#include "Selection.h"
#include<string>
#include<map>

class Projection : public PlanNode
{
  public:
    // Method declarations
    Projection(const std::vector<std::string> keys, const std::unique_ptr<PlanNode> sel, const TableSchema sch);
    std::vector<std::string> next();
    void rewind();

    // Variable declarations
    std::vector<std::string> keys;

  private:
    // Method declarations

    // Variable declarations
    TableSchema schema_;
    std::unique_ptr<PlanNode> sel_;
    int rowIdx_ = 0;
    std::vector<int> colKeys;
};

#endif
