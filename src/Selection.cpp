#include "Selection.h"

Selection::Selection(std::vector<std::string> triple, std::unique_ptr<FileScan> fs) : triple_(triple), fs(std::move(fs)) {}

std::vector<std::string> Selection::next()
{
  std::string key = triple_[0];
  std::string op = triple_[1];
  std::string val = triple_[2];

  std::vector<std::string> row;
  row = fs->next();

  if(op == "EQUALS") {
    int colNum = fs->schema[key];
    std::string col = row[colNum];

    if(col == val) {
      return row;
    }
  }

  //TODO: v2 call return fn_ on row, implementing basic map
  return row;
}
