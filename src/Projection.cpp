#include "Projection.h"
#include "schema_loader.h"

Projection::Projection(std::vector<std::string> column_names, std::unique_ptr<Selection> sel) : column_names_(column_names), sel_(std::move( sel )) {}

std::vector<std::string> Projection::next(){
  std::vector<std::string> result;
  std::vector<std::string> relation = sel_->next();

  if(relation.size() == 0) {
    return result;
  }
  std::vector<std::string> row = relation;

  std::vector<std::string> col_vals;
  tableName = sel_->tableName;
  std::map<std::string,int> schema = schema_loader(tableName);
  for(int i = 0; i < column_names_.size(); i++) {

    std::string column_name = column_names_[i];
    int rowID = schema[column_name];

    if(row.size() != 0) {
      col_vals.push_back(row[rowID]);
    } else {
      col_vals.push_back("");
    }

    return col_vals;

  }
  return result;
}
