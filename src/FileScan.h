#ifndef FILESCAN_H
#define FILESCAN_H
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include "PlanNode.h"

class FileScan : public PlanNode
{
  public:
    FileScan(std::string tableName);
    std::vector<std::string> next();

  private:
    std::ifstream fileStream_;

};
#endif
