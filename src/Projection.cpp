#include "Projection.h"
#include "schema_loader.h"

Projection::Projection(std::vector<std::string> column_names, std::unique_ptr<Selection> sel) : column_names_(column_names), sel_(std::move( sel )) {}

std::vector<std::vector<std::string> > Projection::next(){
  std::vector<std::vector<std::string> > result;
  std::vector<std::vector<std::string> > relation = sel_->next();

  if(relation.size() == 0) {
    return result;
  }
  std::vector<std::string> row = relation[0];

  std::vector<std::string> col_vals;
  //TODO: Pull schema name off of FileScan
  std::map<std::string,int> schema = schema_loader("test_data");
  for(int i = 0; i < column_names_.size(); i++) {

    std::string column_name = column_names_[i];
    int rowID = schema[column_name];

    if(row.size() != 0) {
      col_vals.push_back(row[rowID]);
    } else {
      col_vals.push_back("");
    }

    result.push_back(col_vals);

  }
  return result;
}
