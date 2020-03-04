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
    std::shared_ptr<PlanNode> queryPlan;

    TokenTree tokenize();
    FileScan scanFile(std::string tableName);
    std::map<std::string, std::vector<std::string> > buildQuery(TokenTree tt);
};

#endif
