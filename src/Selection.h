#ifndef SELECTION_H
#define SELECTION_H
#include "FileScan.h"
#include<string>
#include<map>

class Selection : PlanNode
{
  public:
    Selection(const std::vector<std::string> triple, const std::unique_ptr<FileScan> fs);
    std::vector<std::string> next();

  private:
    bool fn_;
    std::vector<std::string> triple_;
    std::unique_ptr<FileScan> fs_;
};

#endif
