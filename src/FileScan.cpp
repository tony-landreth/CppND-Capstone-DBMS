#include "FileScan.h"

FileScan::FileScan(std::string tableName) : tableName(tableName){};


//TODO: scanFile should run on init
void FileScan::scanFile() {
  std::stringstream fileName;
  fileName << "../test/" << tableName << ".csv";
  fileStream_.open(fileName.str());

  if(!fileStream_) {
    std::cerr << "No table named " << tableName << std::endl;
    exit(1);
  }

  std::vector<std::string> row;
  row = next_();

  while(row.size() > 0) {
    relation_.push_back(row);
    row = next_();
  }

  fileStream_.close();

  idx_for_next_ = 0;
}

std::vector<std::string> FileScan::next() {
  std::vector<std::string> empty_vector;
  std::vector<std::string> row = relation_[idx_for_next_];
  idx_for_next_++;

  if(idx_for_next_ >= relation_.size()) {
    return empty_vector;
  }

  return row;
}

std::vector<std::string> FileScan::next_() {
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
