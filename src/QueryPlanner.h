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
#include "TokenTree.h"
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
    std::unique_ptr<std::vector<std::vector<std::string> > > run();
    std::vector<std::vector<std::string> > binProjectionKeys(std::vector<std::string> prjKeys);
    std::vector<std::vector<std::string> > buildFrontEndColumnNames(std::vector<std::string> columnNames, Schema schema);

    // Variable declarations

  private:
    // Method declarations
    TokenTree tokenize(); // tokenizes user input
    std::map<std::string, std::vector<std::string> > mapQuery(TokenTree tt); // generates an intermediate representation

    // Variable declarations
    char** argv_;
    int argc_;
    TokenTree tokenTree_;
    std::map<std::string, std::vector<std::string> > queryData_;
    void detectClauses();
    TokenTree* select_;
    TokenTree* from_;
    TokenTree* where_;
    TokenTree* join_;
    bool selPresent_ = false;
    bool frmPresent_ = false;
    bool whrPresent_ = false;
    bool jnPresent_ = false;
};

#endif
