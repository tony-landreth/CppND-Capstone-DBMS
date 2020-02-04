#ifndef PROJECTION_H
#define PROJECTION_H
#include "Selection.h"
#include<string>
#include<map>

class Projection : PlanNode
{
  public:
    Projection(const std::string column_name, const std::unique_ptr<Selection> sel);
    std::vector<std::string> next();

  private:
    std::string column_name_;
    std::unique_ptr<Selection> sel_;
};

#endif
