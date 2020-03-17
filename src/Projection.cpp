#include "Projection.h"
#include "schema_loader.h"

std::vector<std::string> Projection::next(){
  std::vector<std::string> result;
  std::vector<std::string> row = sel_->next();

  tableSize = sel_->tableSize;
  tableName = schema.tableName;
  std::map<std::string,int> colKeys = schema.columnKeys;
  std::map<std::string,int> newColKeys;

  if(row.size() == 0)
    return result;

  if(row[0].size() == 0)
    return result;

  // When no column names are specified, return the whole row
  if(column_names_.size() == 0)
    return row;

  if(column_names_[0] == "*")
    return row;

  for(int i = 0; i < column_names_.size(); i++) {
    std::string column_name = column_names_[i];
    int rowID = colKeys[column_name];
    result.push_back(row[rowID]);
    newColKeys[column_name] = i;
  }

  // Now that you've projected into a lower-dimensional space, the file schema is no longer useful

  return result;
}

void Projection::rewind(){
  std::unique_ptr<FileScan> fs = std::make_unique<FileScan>(schema.tableName);
  fs->scanFile();

  if(sel_->keys.size() > 0) {
    std::unique_ptr<Selection> s = std::make_unique<Selection>(sel_->keys, std::move(fs), schema);
    sel_ = std::move(s);
  } else {
  std::vector<std::string> where;
    std::unique_ptr<Selection> s = std::make_unique<Selection>(where, std::move(fs), schema);
    sel_ = std::move(s);
  }
}
