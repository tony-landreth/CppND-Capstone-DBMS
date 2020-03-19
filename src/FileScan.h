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
    FileScan(const std::string tableName);

    // Method declarations
    std::vector<std::string> next();
    void rewind(){};
    void scanFile();

    // Variable declarations
    std::string tableName;
    int tableSize;

  private:
    // Method declarations
    std::vector<std::string> next_();

    // Variable declarations
    int idx_for_next_;
    std::vector<std::vector<std::string> > relation_;
    std::ifstream fileStream_;
};
#endif
