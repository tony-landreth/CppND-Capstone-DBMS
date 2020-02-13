#include "QueryPlanner.h"
#include "FileScan.h"
#include "Selection.h"
#include <algorithm>

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
        // TODO: Implement handling for * in SELECT node
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

  for(int i = 0; i < tkns.size(); i++) {
    std::cout << tkns[i] << std::endl;
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
  std::vector<std::string> triple{ "*", "*", "*" };
  Selection sel(triple, std::move(fs));
  results.push_back(sel.next());
  results.push_back(sel.next());
  results.push_back(sel.next());
  results.push_back(sel.next());
  results.push_back(sel.next());

  int i = 0;
  int j = 0;
  int k = 0;

  while(i < results.size()) {
    std::cout << "i: " << i << std::endl;
    while(j < results[i].size()) {
      while(k < results[i][j].size()) {
        std::cout << "results" << results[i][j][k] << std::endl;
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
