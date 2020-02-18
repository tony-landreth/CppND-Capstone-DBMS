#include "FileScan.h"

FileScan::FileScan(std::string tableName) : tableName(tableName)
{
  std::stringstream fileName;
  fileName << "../test/" << tableName << ".csv";
  fileStream_.open(fileName.str());
  std::cout << "File Opened" << std::endl;
}

FileScan::~FileScan()
{
  std::cout << "File Closed" << std::endl;
  fileStream_.close();
}

FileScan::FileScan(const FileScan &source)
{
  std::stringstream fileName;
  fileName << "../test/" << source.tableName << ".csv";
  fileStream_.open(fileName.str());
  std::cout << "COPYING content of FileScan instance " << &source << " to instance " << this << std::endl;
}


FileScan &FileScan::operator=(const FileScan &source)
{
  std::cout << "FileScan Copy Assignment Operator" << std::endl;

  if (this == &source) {
    return *this;
  }

  std::stringstream fileName;
  fileName << "../test/" << source.tableName << ".csv";
  fileStream_.open(fileName.str());

  return *this;
}

FileScan::FileScan(FileScan &&source)
{
  std::cout << "FileScan Move Assignment Operator" << std::endl;
}

FileScan &FileScan::operator=(const FileScan &&source)
{
  if (this == &source) {
    return *this;
  }

  std::stringstream fileName;
  fileName << "../test/" << source.tableName << ".csv";
  fileStream_.open(fileName.str());

  return *this;
}

std::vector<std::string> FileScan::next() {
  std::vector<std::string> cols;
  std::string row;
  std::vector<std::string> result_row;
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

    // If an element has ", concat it with the next element
    // If the next element does not have a ", concat the previous concat with the next element
    std::string quoted_str;
    int size = cols.size();


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
        result_row.push_back(quoted_str);
      } else {
        result_row.push_back(cols[i]);
      }
    }

    // Remove \r from last entry in the row
    std::string lastEntry = result_row.back();
    if(lastEntry.find("\r") != std::string::npos) {
      int numCols = result_row.size() - 1;
      lastEntry.erase(lastEntry.size() - 1);
      result_row[numCols] = lastEntry;
    }

    return result_row;
  }

  //Return empty vector if there's no data
  return result;
}
