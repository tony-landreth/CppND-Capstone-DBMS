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
#include "EmptyPlan.h"

class QueryPlanner{
  public:
    QueryPlanner(int argc, char** argv);
    std::vector<std::vector<std::string> > run();

  private:
    char** argv_;
    int argc_;
    //TODO: add _ to end of queryPlan
    //TODO: this is misnamed
    std::shared_ptr<PlanNode> queryPlan;
    std::map<std::string, std::vector<std::string> > queryData;

    TokenTree tokenize();
    FileScan scanFile(std::string tableName);
    std::map<std::string, std::vector<std::string> > buildQuery(TokenTree tt);
    void detectClauses();
    bool whrPresent = false;
    bool selPresent = false;
    bool jnPresent = false;
    bool frmPresent = false;
};

#endif
