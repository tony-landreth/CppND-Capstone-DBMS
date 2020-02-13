#include "QueryPlanner.h"


QueryPlanner::QueryPlanner(int argc, char** argv) : argc_(argc), argv_(argv) {};

void QueryPlanner::run()
{
  std::string arg;
  std::istringstream tkn_stream(argv_[1]);
  std::vector<std::string> tkns;
  std::string tmp;
  std::vector<std::string> selectors;
  std::vector<std::string> tableNames;
  tkn_stream >> tmp;

  std::cout << "THE VALUE OF tkn_stream is " << tmp << std::endl;
}
