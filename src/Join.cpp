#include "Projection.h"
#include "Join.h"
#include "schema.h"


Join::Join(std::unique_ptr<Projection> r, std::unique_ptr<Projection> s, std::vector<std::string> k, Schema rsch, Schema ssch) : r_(std::move( r )), s_(std::move( s )), keys(k), r_schema_(rsch), s_schema_(ssch) {
  schema_.tableName = "virtual";
}

Schema Join::getSchema(){
  return schema_;
}

std::vector<std::string> Join::next() {
  rTableSize_ = r_schema_.tableSize;
  sTableSize_ = s_schema_.tableSize + 1;

  std::vector<std::string> result;
  std::string r_key = keys[0];
  std::string s_key = keys[1];

  // Get the next row from each node
  std::vector<std::string> r_row = r_->next();
  std::vector<std::string> s_row = s_->next();

  // Rewind the S relation so that a full table scan is possible
  s_->rewind();

  // Use these variables to determine when the foreign keys map to matching values between two tables
  std::string r_col;
  std::string s_col;

  // If this is the first call to next(), return title row
  // and construct a schema based on the incoming nodes
  if(rowIdx == 0) {
    // Concatenate the rows
    std::vector<std::string> joinedRow;
    std::copy(r_row.begin(), r_row.end(), std::back_inserter(joinedRow));
    std::copy(s_row.begin(), s_row.end(), std::back_inserter(joinedRow));

    rowIdx++; // Log that you've encountered the header rows for R and S

    // Build the column index for foreign keys
    // Allowing foreign keys to have the same name
    std::vector<int> keyMap(joinedRow.size(), 0);

    for(std::string k : keys){
      int kmIdx = 0;

      for(std::string c : joinedRow) {

        if((k == c) && (keyMap[kmIdx] == 0)){
          keyMap[kmIdx] = 1;
          foreignKeys_.push_back(kmIdx);
        }
        kmIdx++;
      }
    }

    requiredRowSize_ = joinedRow.size();
    return joinedRow;
  }

  int r_colID = foreignKeys_[0];
  int s_colID = foreignKeys_[1];


  // If r is empty, return early
  if(r_row.size() == 0) {
    return result;
  }

  // When R is non-empty, search for foreignKey match in S
  for(int i = 0; i < sTableSize_; i++) {
    // Concatenate the rows
    std::vector<std::string> joinedRow;
    std::copy(r_row.begin(), r_row.end(), std::back_inserter(joinedRow));
    std::copy(s_row.begin(), s_row.end(), std::back_inserter(joinedRow));

    if(joinedRow.size() == requiredRowSize_){
      r_col = joinedRow[r_colID];
      s_col = joinedRow[s_colID];

      if(r_col == s_col) {
        schema_.tableSize++;
        return joinedRow;
      } else {
        s_row = s_->next();
      }
    }
  }

  return result;
}

