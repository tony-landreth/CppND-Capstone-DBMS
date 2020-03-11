#include "Selection.h"
#include "schema_loader.h"

Selection::Selection(std::vector<std::string> where, std::unique_ptr<FileScan> fs) : where(where), fs(std::move(fs)) {}

std::vector<std::string> Selection::next()
{
  tableName = fs->tableName;
  tableSize = fs->tableSize;

  std::vector<std::string> empty_row;
  std::vector<std::string> row;
  row = fs->next();

  if(row.size() == 0)
    return empty_row;

  if(rowIdx == 0) {
    rowIdx++;
    return row;
  }

  if(where.size() == 3) {
    std::string key = where[0]; // used to handle WHERE clauses, e.g. WHERE key EQUAL val
    std::string op = where[1];  // is either * or EQUAL for the time being
    std::string val = where[2]; // used to handle WHERE clauses, e.g. WHERE key EQUAL val

    if(op == "EQUALS") {
      TableSchema tblSchema = schema_loader(tableName);
      std::map<std::string, int> schema = tblSchema.columnKeys;
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
  } else {
    return row;
  }

  return empty_row;
}
