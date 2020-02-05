#include "Projection.h"
#include "schema_loader.h"

Projection::Projection(std::string column_name, std::unique_ptr<Selection> sel) : column_name_(column_name), sel_(std::move( sel )) {}

std::vector<std::vector<std::string> > Projection::next(){
  std::vector<std::vector<std::string> > result;
  std::vector<std::vector<std::string> > relation = sel_->next();

  if(relation.size() == 0) {
    return result;
  }
  std::vector<std::string> row = relation[0];

  std::vector<std::string> col_vals;
  //TODO: Make it possible to pass an argument into the schema_loader
  std::map<std::string,int> schema = schema_loader("test_data");
  int rowID = schema[column_name_];


  if(row.size() != 0) {
    col_vals.push_back(row[rowID]);
  } else {
    col_vals.push_back("");
  }

  result.push_back(col_vals);

  return result;
}
