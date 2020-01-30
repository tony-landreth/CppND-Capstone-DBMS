#include "Selection.h"

Selection::Selection(std::vector<std::string> triple, FileScan fs) : triple_(triple), fs_(std::move(fs)) {}

std::vector<std::string> Selection::next()
{
  //Hardcode the Schema for now
  std::map <std::string,int> Schema;
  Schema["movieId"] = 0;
  Schema["title"] = 1;
  Schema["genres"] = 2;

  std::string key = triple_[0];
  std::string op = triple_[1];
  std::string val = triple_[2];

  std::vector<std::string> row;
  row = fs_.next();

  if(op == "EQUALS") {
    int colNum = Schema[key];
    std::string col = row[colNum];

    if(col == val) {
      return row;
    }
  }

  //TODO: v2 call return fn_ on row, implementing basic map
  return row;
}
