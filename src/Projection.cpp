#include "Projection.h"
#include "schema_loader.h"

std::vector<std::string> Projection::next(){

  std::vector<std::string> result;
  std::vector<std::string> row = sel_->next();

  tableSize = sel_->tableSize;
  tableName = schema.tableName;

  std::map<std::string,int> colMap;
  std::vector<std::string> colHeaders;


  if(tableName == "virtual"){
    if(rowIdx_ == 0){
      rowIdx_++;

      // To handle columns with the same name
      // Track whether an index has been taken
      std::vector<int> selColMap(row.size(), 0);

      // For each selected column
      for(int i = 0; i < column_names_.size(); i++){
        for(int j = 0; j < row.size(); j++){
          if(( selColMap[j] == 0 ) && ( column_names_[i] == row[j] )){
            colKeys.push_back(i);
            selColMap[j] = 1;
          }
        }
      }
    }
  } else {
    if(rowIdx_ == 0){
      rowIdx_++;
      colMap = schema.columnKeys;

      for(int i = 0; i < column_names_.size(); i++){
        std::string col_name = column_names_[i];

        int idx = colMap[col_name];
        colKeys.push_back(idx);
      }
    }
  }

  if(row.size() == 0)
    return result;

  if(row[0].size() == 0)
    return result;

  // When no column names are specified, return the whole row
  if(column_names_.size() == 0)
    return row;

  if(column_names_[0] == "*")
    return row;

  for(int i = 0; i < colKeys.size(); i++) {
    int rowID = colKeys[i];
    result.push_back(row[rowID]);
  }
  std::cout << std::endl;

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
