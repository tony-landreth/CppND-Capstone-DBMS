#ifndef PROJECTION_H
#define PROJECTION_H
#include "Selection.h"
#include<string>
#include<map>

class Projection : public PlanNode
{
  public:
    Projection(std::vector<std::string> column_names, std::unique_ptr<Selection> sel, TableSchema sch) : column_names_(column_names), sel_(std::move( sel )), schema(sch) {}
    Projection(const std::vector<std::string> column_names, const std::unique_ptr<PlanNode> sel);
    std::vector<std::string> next();
    void rewind();
    std::string tableName;
    int tableSize;
    TableSchema schema;

  private:
    std::vector<std::string> column_names_;
    std::unique_ptr<PlanNode> sel_;
};

#endif
