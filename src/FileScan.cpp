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
  std::vector<std::string> result;

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
    // If an element has ", concat it with the next element
    // If the next element does not have a ", concat the previous concat with the next element
    std::string quoted_str;
    int size = cols.size();
    std::vector<std::string> result;

    for(int i = 0; i < size; i++) {
      if(cols[i].find("\"") != std::string::npos) {

        quoted_str = cols[i];
        i++;

        while(true) {
          quoted_str = quoted_str + "," + cols[i];
          if(cols[i].find("\"") != std::string::npos) {
            break;
          } else {
            i++;
          }
        }
        result.push_back(quoted_str);
      } else {
        result.push_back(cols[i]);
      }
    }

    // Remove \r from last entry in the row
    std::string lastEntry = result.back();
    if(lastEntry.find("\r") != std::string::npos) {
      int numCols = result.size() - 1;
      lastEntry.erase(lastEntry.size() - 1);
      result[numCols] = lastEntry;
    }

    return result;
  }

  //Return empty vector if there's no data
  return cols;
}
