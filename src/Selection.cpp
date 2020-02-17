#include "Selection.h"
#include "schema_loader.h"

Selection::Selection(std::vector<std::string> where, std::unique_ptr<FileScan> fs) : where_(where), fs(std::move(fs)) {}

std::vector<std::string> Selection::next()
{
  tableName = fs->tableName;
  std::string key = where_[0]; // used to handle WHERE clauses, e.g. WHERE key EQUAL val
  std::string op = where_[1];  // is either * or EQUAL for the time being
  std::string val = where_[2]; // used to handle WHERE clauses, e.g. WHERE key EQUAL val

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
