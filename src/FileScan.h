#ifndef FILESCAN_H
#define FILESCAN_H
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include "PlanNode.h"

class FileScan : public PlanNode
{
  public:
    FileScan(std::string tableName);
    std::vector<std::string> next();
    std::map<std::string,int> schema;
    std::string tableName;

  private:
    std::ifstream fileStream_;

};
#endif
