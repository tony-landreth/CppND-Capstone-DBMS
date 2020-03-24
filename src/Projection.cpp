#include "Projection.h"
#include "schema.h"

Projection::Projection(std::vector<std::string> column_names, std::unique_ptr<PlanNode> sel, Schema sch) : keys(column_names), sel_(std::move( sel )), schema_(sch) {};

std::vector<std::string> Projection::next(){

  std::vector<std::string> result;
  std::vector<std::string> row = sel_->next();

  std::map<std::string,int> colMap;
  std::vector<std::string> colHeaders;


  if(schema_.tableName == "virtual"){
    if(rowIdx_ == 0){
      rowIdx_++;

      // To handle columns with the same name
      // Track whether an index has been taken
      std::vector<int> selColMap(row.size(), -1);

      // Match each column name to its position in the header row
      for(int i = 0; i < keys.size(); i++){
        for(int j = 0; j < row.size(); j++){
          if(( selColMap[j] == -1 ) && ( keys[i] == row[j] )){
            selColMap[j] = j;
          }
        }
      }
      // Only keep an index in selColMap if it belongs to a selected column
      for(int i : selColMap) {
        if(i != -1){
          colKeys.push_back(i);
        }
      }
    }
  } else {
    if(rowIdx_ == 0){
      rowIdx_++;
      colMap = schema_.columnKeys;

      for(int i = 0; i < keys.size(); i++){
        std::string col_name = keys[i];

        int idx = colMap[col_name];
        colKeys.push_back(idx);
      }
    }
  }

  if(row.size() == 0)
    return result;

  if(row[0].size() == 0)
    return result;

  for(int i = 0; i < colKeys.size(); i++) {
    int rowID = colKeys[i];
    result.push_back(row[rowID]);
  }

  // Now that you've projected into a lower-dimensional space, the file schema is no longer useful
  return result;
}

void Projection::rewind(){
  std::unique_ptr<FileScan> fs = std::make_unique<FileScan>(schema_);
  fs->scanFile();

  if(sel_->keys.size() > 0) {
    std::unique_ptr<Selection> s = std::make_unique<Selection>(sel_->keys, std::move(fs), schema_);
    sel_ = std::move(s);
  } else {
  std::vector<std::string> where;
    std::unique_ptr<Selection> s = std::make_unique<Selection>(where, std::move(fs), schema_);
    sel_ = std::move(s);
  }
}
