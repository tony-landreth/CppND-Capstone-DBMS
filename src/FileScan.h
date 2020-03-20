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
    FileScan(const TableSchema schema);

    // Method declarations
    std::vector<std::string> next();
    void rewind(){};
    void scanFile();

    // Variable declarations
    TableSchema schema;

  private:
    // Method declarations
    std::vector<std::string> next_();

    // Variable declarations
    TableSchema schema_;
    int idx_for_next_;
    std::vector<std::vector<std::string> > relation_;
    std::ifstream fileStream_;
};
#endif
