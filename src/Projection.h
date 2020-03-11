#ifndef PROJECTION_H
#define PROJECTION_H
#include "Selection.h"
#include<string>
#include<map>

class Projection : PlanNode
{
  public:
    Projection(const std::vector<std::string> column_names, const std::unique_ptr<Selection> sel);
    std::vector<std::string> next();
    void rewind();
    std::string tableName;
    int tableSize;

  private:
    std::vector<std::string> column_names_;
    std::unique_ptr<Selection> sel_;
};

#endif
