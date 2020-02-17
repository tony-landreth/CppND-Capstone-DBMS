#include "Projection.h"
#include "Join.h"
#include "schema_loader.h"

Join::Join(std::unique_ptr<Projection> r, std::unique_ptr<Projection> s, std::vector<std::string> k) : r_(std::move( r )), s_(std::move( s )), keys_(k) {
}

std::vector<std::string> Join::next() {
  std::vector<std::string> result;
  std::string r_key = keys_[0];
  std::string s_key = keys_[1];

  std::string r_table_name = r_->tableName;
  std::string s_table_name = s_->tableName;
  std::map<std::string, int> r_schema = schema_loader(r_table_name);
  std::map<std::string, int> s_schema = schema_loader(s_table_name);

  std::vector<std::string> s_relation = s_->next();
  std::vector<std::string> r_relation = r_->next();
  std::vector<std::string> r_row = r_relation;
  int r_colID = r_schema[r_key];
  int s_colID = s_schema[s_key];

  std::vector<std::string> s_row;
  std::string r_col;
  std::string s_col;

  if(r_relation.size() > 0) {
    r_col = r_row[r_colID];
  }

  //  Once you roll r to the point where you've got a record that fits the where clause
  //  You then need to start rolling s
  if(r_col.size() > 0) {
    while( true ) {

      if(s_relation.size() == 0) {
        break;
      }

      std::vector<std::string> s_row = s_relation;
      if(s_row.size() > 0)
        s_col = s_row[s_colID];

      if(r_col == s_col) {
        std::vector<std::string> result_row;
        std::copy(s_row.begin(), s_row.end(), std::back_inserter(result_row));
        std::copy(r_row.begin(), r_row.end(), std::back_inserter(result_row));

        return result_row;
      }

      s_relation = s_->next();
    }
  }

  return result;
}

