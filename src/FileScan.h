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
    ~FileScan();                                 // destructor
    FileScan(const FileScan &source);            // copy constructor
    FileScan &operator=(const FileScan &source); // copy assignment
    FileScan(FileScan &&source);                 // move constructor
    FileScan &operator=(const FileScan &&source);// move assignment operator
    std::vector<std::string> next();
    std::map<std::string,int> schema;
    std::string tableName;

  private:
    std::ifstream fileStream_;

};
#endif
