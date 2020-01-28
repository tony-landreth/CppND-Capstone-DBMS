#ifndef FILESCAN_H
#define FILESCAN_H
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>

class FileScan
{
  public:
    FileScan(std::string tableName);
    std::string next();

  private:
    std::ifstream fileStream_;

};
#endif
