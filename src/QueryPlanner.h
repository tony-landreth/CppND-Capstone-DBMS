#ifndef QUERY_PLANNER_H
#define QUERY_PLANNER_H

#include<sstream> 
#include<string>
#include<vector>
#include<iostream>

class QueryPlanner{
  public:
    QueryPlanner(int argc, char** argv);
    void run();

  private:
    char** argv_;
    int argc_;
};

#endif
