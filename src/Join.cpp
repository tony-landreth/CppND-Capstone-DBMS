#include "Projection.h"
#include "Join.h"
#include "schema_loader.h"

Join::Join(std::unique_ptr<Projection> r, std::unique_ptr<Projection> s, std::vector<std::string> k) : r_(std::move( r )), s_(std::move( s )), keys_(k) {
}

std::vector<std::string> Join::next() {
  rTableSize = r_->tableSize;
  sTableSize = s_->tableSize;
  std::vector<std::string> result;
  std::string r_key = keys_[0];
  std::string s_key = keys_[1];

  TableSchema rSchema = r_->schema;
  TableSchema sSchema = s_->schema;

  std::string r_table_name = rSchema.tableName;
  std::string s_table_name = sSchema.tableName;
  
  // Rewind the S relation so that a full table scan is possible
  s_->rewind();

  // Get the next row from each node
  std::vector<std::string> r_row = r_->next();
  std::vector<std::string> s_row = s_->next();


  // Use these variables to determine when the foreign keys match between two tables
  std::string r_col;
  std::string s_col;

  // If this is the first call to next(), return title row
  // and build jSchema
  if(rowIdx == 0) {
    rowIdx++;
    std::vector<std::string> result_row;
    std::copy(r_row.begin(), r_row.end(), std::back_inserter(result_row));
    std::copy(s_row.begin(), s_row.end(), std::back_inserter(result_row));

    // Build the column index for foreign keys
    // Allowing foreign keys to have the same name
    std::vector<int> keyMap(result_row.size(), 0);
    for(int i = 0; i < keys_.size(); i++){
      for(int j = 0; j < result_row.size(); j++){
        if(( keyMap[j] == 0 ) && ( keys_[i] == result_row[j] )){
          foreignKeys.push_back(i);
          keyMap[j] = 1;
        }
      }
    }

    return result_row;
  }

  int r_colID = foreignKeys[0];
  int s_colID = foreignKeys[1];

  if(r_row.size() > 0) {
    r_col = r_row[r_colID];
  }

  if(r_col.size() > 0) {
    for(int i = 0; i < sTableSize; i++) {
      if(s_row.size() > 0){
        s_col = s_row[s_colID];

        if(r_col == s_col) {
          std::vector<std::string> result_row;
          std::copy(s_row.begin(), s_row.end(), std::back_inserter(result_row));
          std::copy(r_row.begin(), r_row.end(), std::back_inserter(result_row));

          return result_row;
        }
      }
      s_row = s_->next();
    } 
  } 

  return result;
}

