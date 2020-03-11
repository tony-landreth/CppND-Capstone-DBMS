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

  std::map<std::string, int> rColKeys = rSchema.columnKeys;
  std::map<std::string, int> sColKeys = sSchema.columnKeys;

  // Rewind the S relation so that a full table scan is possible
  s_->rewind();

  std::vector<std::string> r_row = r_->next();
  std::vector<std::string> s_row = s_->next();

  //TODO: rename these to foreign key
  int r_colID = rColKeys[r_key];
  int s_colID = sColKeys[s_key];

  // Use these variables to determine when the foreign keys match between two tables
  std::string r_col;
  std::string s_col;

  if(r_row.size() > 0) {
    r_col = r_row[r_colID];
  }

  // If this is the first call to next(), return title row
  // and build jSchema
  if(rowIdx == 0) {
    rowIdx++;
    std::vector<std::string> result_row;
    std::copy(r_row.begin(), r_row.end(), std::back_inserter(result_row));
    std::copy(s_row.begin(), s_row.end(), std::back_inserter(result_row));

    /*
    // Note that you'll end up dropping keys for identically named columns in the block below.
    // Self joins will not be supported, until column aliasing is implemented
    for(int i = 0; i < result_row.size(); i++){
      jSchema[result_row[i]] = i;
    }
    */

    return result_row;
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

