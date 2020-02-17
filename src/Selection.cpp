#include "Selection.h"
#include "schema_loader.h"

Selection::Selection(std::vector<std::string> triple, std::unique_ptr<FileScan> fs) : triple_(triple), fs(std::move(fs)) {}

std::vector<std::string> Selection::next()
{
  tableName = fs->tableName;
  std::string key = triple_[0];
  std::string op = triple_[1];
  std::string val = triple_[2];

  std::vector<std::string> empty_row;
  std::vector<std::string> row;
  row = fs->next();

  if(row.size() == 0)
    return empty_row;

  if(op == "EQUALS") {
    std::map<std::string, int> schema = schema_loader(tableName);
    int colNum = schema[key];

    // Return when the row contains only the empty string
    if(( row.size() <= 1 ) && (row[0].empty()))
      return empty_row;

    std::string col = row[colNum];

    if(col == val) {
      return row;
    } else {
      return empty_row;
    }
  }

  if(op == "*") {
    return row;
  }

  return empty_row;
}
