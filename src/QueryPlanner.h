#ifndef QUERY_PLANNER_H
#define QUERY_PLANNER_H

#include<map>
#include<sstream> 
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<future>
#include "FileScan.h"
#include "Selection.h"
#include "Projection.h"
#include "Join.h"
#include "Tokenizer.h"

class QueryPlanner{
  public:
    QueryPlanner(int argc, char** argv);
    std::vector<std::vector<std::string> > run();

  private:
    char** argv_;
    int argc_;
    std::map<std::string, std::vector<std::string> > queryData_;
    void detectClauses();
    bool whrPresent_ = false;
    bool selPresent_ = false;
    bool jnPresent_ = false;
    bool frmPresent_ = false;

    TokenTree tokenize();
    FileScan scanFile(std::string tableName);
    std::map<std::string, std::vector<std::string> > buildQuery(TokenTree tt);
};

#endif
