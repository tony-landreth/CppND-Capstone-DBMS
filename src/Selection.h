#ifndef SELECTION_H
#define SELECTION_H
#include "FileScan.h"
#include<string>
#include<map>

class Selection : public PlanNode
{
  public:
    Selection(std::vector<std::string> where, std::unique_ptr<FileScan> fs, TableSchema sch) : where(where), fs(std::move(fs)), schema(sch) {};
    TableSchema schema;
    std::vector<std::string> where;
    std::vector<std::string> next();
    void rewind(){};
    std::unique_ptr<FileScan> fs;
    std::string tableName;
    int tableSize;
    std::vector<std::string> keys;

  private:
    int rowIdx = 0;
};

#endif
