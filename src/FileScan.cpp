#include "FileScan.h"

FileScan::FileScan(std::string tableName)
{
  std::ifstream fileStream;
  std::stringstream fileName;
  fileName << "../test/data/" << tableName << ".csv";
  fileStream_.open(fileName.str());
}

std::vector<std::string> FileScan::next() {
  std::vector<std::string> cols;
  std::string row;
  std::vector<std::string> data;

  if(fileStream_) {
    getline(fileStream_, row);
    std::stringstream ss(row);

    // Split on commas (consider edge cases, e.g. quote escaped names with commas in them)
    while( ss.good() ) {
      std::string substr;
      getline( ss, substr, ',' );
      cols.push_back( substr );
    }

    // TODO: Merge on quotation marks

    int numCols = cols.size() - 1;
    std::string lastEntry = cols.back();

    // Remove \r from last entry in the row
    lastEntry.erase(lastEntry.size() - 1);
    cols[numCols] = lastEntry;

    return cols;
  }

  //We'll return empty string on EOF
  return cols;
}
