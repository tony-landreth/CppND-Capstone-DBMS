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
    void scanFile();
    int tableSize;

  private:
    std::vector<std::string> next_();
    std::ifstream fileStream_;
    std::vector<std::vector<std::string> > relation_;
    int idx_for_next_;
};
#endif
