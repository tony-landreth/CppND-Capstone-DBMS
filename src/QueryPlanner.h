#ifndef QUERY_PLANNER_H
#define QUERY_PLANNER_H

#include<sstream> 
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<future>
#include "FileScan.h"
#include "Selection.h"
#include "Projection.h"
#include "Tokenizer.h"

class QueryPlanner{
  public:
    QueryPlanner(int argc, char** argv);
    std::vector<std::vector<std::string> > run();

  private:
    char** argv_;
    int argc_;

    std::vector<std::string> tokenize();
    FileScan scanFile(std::string tableName);
};

#endif
