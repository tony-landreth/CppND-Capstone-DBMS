#include "Projection.h"
#include "schema_loader.h"

Projection::Projection(std::vector<std::string> column_names, std::unique_ptr<Selection> sel) : column_names_(column_names), sel_(std::move( sel )) {}

std::vector<std::string> Projection::next(){
  std::vector<std::string> result;
  std::vector<std::string> row = sel_->next();

  if(row.size() == 0)
    return result;

  if(row[0].size() == 0)
    return result;

  // When no column names are specified, return the whole row
  if(column_names_.size() == 0)
    return row;

  if(column_names_[0] == "*")
    return row;

  tableName = sel_->tableName;
  std::map<std::string,int> schema = schema_loader(tableName);

  for(int i = 0; i < column_names_.size(); i++) {
    std::string column_name = column_names_[i];
    int rowID = schema[column_name];
    result.push_back(row[rowID]);
  }

  return result;
}
