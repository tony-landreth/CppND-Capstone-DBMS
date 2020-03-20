#include "Selection.h"
#include "schema_loader.h"

Selection::Selection(std::vector<std::string> where, std::unique_ptr<FileScan> fs, TableSchema sch) : keys(where), fs_(std::move(fs)), schema(sch) {};

std::vector<std::string> Selection::next()
{
  std::vector<std::string> empty_row;
  std::vector<std::string> row;
  row = fs_->next();

  if(row.size() == 0)
    return empty_row;

  if(rowIdx_ == 0) {
    rowIdx_++;
    return row;
  }

  if(keys.size() == 3) {
    std::string key = keys[0]; // used to handle WHERE clauses, e.g. WHERE key EQUAL val
    std::string op = keys[1];  // is either * or EQUAL for the time being
    std::string val = keys[2]; // used to handle WHERE clauses, e.g. WHERE key EQUAL val

    if(op == "EQUALS") {
      std::map<std::string, int> colKeys = schema.columnKeys;
      int colNum = colKeys[key];

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
  } else {
    return row;
  }

  return empty_row;
}
