#include "Projection.h"
#include "Join.h"
#include "schema_loader.h"

Join::Join(std::unique_ptr<Projection> r, std::unique_ptr<Projection> s, std::vector<std::string> k) : r_(std::move( r )), s_(std::move( s )), keys_(k) {
}

std::vector<std::vector<std::string> > Join::next() {
  std::vector<std::vector<std::string> > result;
  std::string r_key = keys_[0];
  std::string s_key = keys_[1];

  std::map<std::string, int> r_schema = schema_loader("test_data");
  std::map<std::string, int> s_schema = schema_loader("test_data");

  //TODO: To avoid confusion, make it so FileScan slices header row off
  std::vector<std::vector<std::string> > s_relation = s_->next();
  std::vector<std::vector<std::string> > r_relation = r_->next();
  std::vector<std::string> r_row = r_relation[0];
  int r_colID = r_schema[r_key];
  int s_colID = s_schema[s_key];

  std::vector<std::string> s_row;
  std::string r_col;
  std::string s_col;

  if(r_relation.size() > 0) {
    r_relation = r_->next();
    r_col = r_row[r_colID];
  }

  //  Once you roll r to the point where you've got a record that fits the where clause
  //  You then need to start rolling s
  if(r_col.size() > 0) {
    while( true ) {
      s_relation = s_->next();

      if(s_relation.size() == 0) {
        break;
      }

      std::vector<std::string> s_row = s_relation[0];
      if(s_row.size() > 0)
        s_col = s_row[s_colID];

      if(r_col == s_col) {
        std::vector<std::string> result_row;
        std::copy(s_row.begin(), s_row.end(), std::back_inserter(result_row));
        std::copy(r_row.begin(), r_row.end(), std::back_inserter(result_row));

        result.push_back(result_row);
      }
    }
  }

  return result;
}

