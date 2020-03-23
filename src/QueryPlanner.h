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
#include "utilities.h"

/*
   QueryPlanner determines which PlanNodes need to be instantiated
   to execute a query request, issued by a user.
*/
class QueryPlanner{
  public:
    // Method declarations
    QueryPlanner(){};
    QueryPlanner(int argc, char** argv);
    std::vector<std::vector<std::string> > run();
    std::vector<std::vector<std::string> > binProjectionKeys(std::vector<std::string> prjKeys);

    // Variable declarations

  private:
    // Method declarations
    TokenTree tokenize(); // tokenizes user input
    std::map<std::string, std::vector<std::string> > mapQuery(TokenTree tt); // generates an intermediate representation

    // Variable declarations
    char** argv_;
    int argc_;
    std::map<std::string, std::vector<std::string> > queryData_;
    void detectClauses();
    bool whrPresent_ = false;
    bool selPresent_ = false;
    bool jnPresent_ = false;
    bool frmPresent_ = false;
};

#endif
