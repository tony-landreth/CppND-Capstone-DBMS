#include "QueryPlanner.h"

QueryPlanner::QueryPlanner(int argc, char** argv) : argc_(argc), argv_(argv) {};

std::vector<std::string> QueryPlanner::tokenize()
{
  std::string arg;
  std::istringstream tkn_stream(argv_[1]);
  std::vector<std::string> tkns;
  std::string tmp;
  std::vector<std::string> selectors;
  std::vector<std::string> tableNames;

  // Split argv on whitespace
  while (tkn_stream >> tmp) {
    tkns.push_back(tmp);
  }

  std::string tkn;

  for (int i = 0; i < tkns.size(); ++i) {
    tkn = tkns[i];

    if(tkn == "SELECT") {
      // Roll past SELECT token
      i++;
      tkn = tkns[i];

      while(tkn != "FROM") {
        selectors.push_back(tkn);

        // Roll forward by one token
        i++;
        if(i >= tkns.size())
          break;

        tkn = tkns[i];
      }

      // Roll past FROM token
      i++;
      tkn = tkns[i];

      while(tkn != "WHERE") {
        tableNames.push_back(tkn);

        // Roll forward by one token
        i++;
        if(i >= tkns.size())
          break;
        tkn = tkns[i];
      }
    }
  }

  return tkns;
};

std::vector<std::vector<std::string> > QueryPlanner::run()
{
  std::vector<std::vector<std::string> > results;
  std::vector<std::string> query = tokenize();
  std::string tableName = query.back();
  std::string chars = ";\"";


  for(char c: chars) {
    tableName.erase(std::remove(tableName.begin(), tableName.end(), c), tableName.end());
  }

  std::unique_ptr<FileScan> fs = std::make_unique<FileScan>(tableName);
  FileScan* fs_ptr = fs.get();

  auto ftr = std::async(&FileScan::scanFile, fs_ptr);
  ftr.get();

  if(query[1] == "*") {
    std::vector<std::string> where{ query[0], query[1], query[2] };
    Selection sel(where, std::move(fs));
    std::vector<std::string> row = sel.next();

    while(row.size() > 0) {
      if(row[0].size() > 0)
        results.push_back(row);

      row = sel.next();
    }
  } else {
    // If SELECT includes a col name, draw rows from a projection
    // TODO: Allow for Projection to take a vector of col names
    std::vector<std::string> where{ query[0], "*", query[2] };
    std::unique_ptr<Selection> sel = std::make_unique<Selection>(where, std::move(fs));
    std::vector<std::string> col_names{ query[1] };
    Projection proj(col_names, std::move(sel));

    std::vector<std::string> row = proj.next();

    while(row.size() > 0) {
      if(row[0].size() > 0)
        results.push_back(row);

      row = proj.next();
    }
  }
  

  int i = 0;
  int j = 0;
  int k = 0;

  // Build return relation
  while(i < results.size()) {
    while(j < results[i].size()) {
      while(k < results[i][j].size()) {
        k++;
      }
    j++;
    }
    j = 0;
    k = 0;
    i++;
  }

  return results;
};
