#include "Projection.h"

Projection::Projection(std::string column_name, std::unique_ptr<Selection> sel) : column_name_(column_name), sel_(std::move( sel )) {}

std::vector<std::string> Projection::next(){
  std::vector<std::string> row = sel_->next();
  std::vector<std::string> col_vals;

    //TODO: Figure out how to pull this from the FileScan node, use a std::shared_ptr?
    std::map<std::string,int> schema = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };

  int rowID = schema[column_name_];
  col_vals.push_back(row[rowID]);

  return col_vals;
}
