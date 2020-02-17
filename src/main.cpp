#include "FileScan.h"
#include "Selection.h"
#include "Projection.h"
#include "Join.h"
#include "QueryPlanner.h"
#include <sstream> 

//TODO: Implement Rule of Five for something
//TODO: Use a Template Function for something
//TODO: Use concurrency to read entire tables into memory before running JOIN


//TODO: Enable Select to take * as input
//TODO: Create an abstraction for the code below

int main(int argc, char** argv) {
  // First query is: "SELECT * FROM movies"
  // Like this: ./mildDBMS "SELECT * FROM test_data;"
  QueryPlanner qp = QueryPlanner(argc, argv);
  std::vector<std::vector<std::string> > result = qp.run();

  for(int i = 0; i < result.size(); i++) {
    for(int j = 0; j < result[j].size(); j++) {
      std::cout << result[i][j] << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}

