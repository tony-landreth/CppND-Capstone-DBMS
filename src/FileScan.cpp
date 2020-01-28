#include "FileScan.h"
FileScan::FileScan(std::string tableName)
{
  std::ifstream fileStream;
  std::stringstream fileName;
  fileName << "../test/data/" << tableName << ".csv";
  fileStream_.open(fileName.str());
}

std::string FileScan::next()
{
  std::string row;

  if(fileStream_) {
    getline(fileStream_, row);
  }

  //We'll return empty string on EOF
  return row;
}
