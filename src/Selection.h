#ifndef SELECTION_H
#define SELECTION_H
#include "FileScan.h"
#include<string>
#include<map>

class Selection : public PlanNode
{
  public:
    // Method declarations
    Selection(const std::vector<std::string> keys, const std::unique_ptr<FileScan> fs, const TableSchema sch);
    std::vector<std::string> next();
    void rewind(){};

    // Variable declarations
    TableSchema schema;
    std::vector<std::string> keys;

  private:
    // Method declarations

    // Variable declarations
    std::unique_ptr<FileScan> fs_;
    int rowIdx_ = 0;
};

#endif
