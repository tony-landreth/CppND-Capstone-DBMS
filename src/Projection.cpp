#include "Projection.h"
#include "schema_loader.h"

Projection::Projection(std::string column_name, std::unique_ptr<Selection> sel) : column_name_(column_name), sel_(std::move( sel )) {}

std::vector<std::string> Projection::next(){
  std::vector<std::string> row = sel_->next();
  std::vector<std::string> col_vals;
  std::map<std::string,int> schema = schema_loader("movies");
  int rowID = schema[column_name_];
  col_vals.push_back(row[rowID]);

  return col_vals;
}
