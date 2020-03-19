#ifndef PROJECTION_H
#define PROJECTION_H
#include "Selection.h"
#include<string>
#include<map>

class Projection : public PlanNode
{
  public:
    Projection(const std::vector<std::string> column_names, const std::unique_ptr<PlanNode> sel, TableSchema sch);
    std::vector<std::string> next();
    void rewind();
    std::string tableName;
    int tableSize;
    TableSchema schema;

  private:
    std::vector<std::string> column_names_;
    std::unique_ptr<PlanNode> sel_;
    int rowIdx_ = 0;
    std::vector<int> colKeys;
};

#endif
