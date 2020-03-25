#include "FileScan.h"
#include "Selection.h"
#include "Projection.h"
#include "Join.h"
#include "QueryPlanner.h"
#include <sstream> 
#include <iomanip>

using std::setw;

//TODO: Use a Template Function for something

int main(int argc, char** argv) {
  // First query is: "SELECT * FROM movies;"
  // Like this: ./mildDBMS "SELECT * FROM movies;"
  // Second query is: ./mildDBMS "SELECT movies.title, ratings.rating FROM movies JOIN ratings ON movieId=movieId;"

  QueryPlanner qp = QueryPlanner(argc, argv);
  std::vector<std::vector<std::string> > result = qp.run();

  for(int i = 0; i < result.size(); i++) {
    for(int j = 0; j < result[j].size(); j++) {
      std::string r = result[i][j];
      int rowLength = result[i].size();
    
      if(i == 0){
        // Format header
        std::cout << r << setw(80) << std::right;

        if(j == rowLength - 1){
          std::cout << std::left;
        }
      } else {
        // Format rows
        std::cout << r << setw(80) << std::left;
      }
    }
    std::cout << std::endl;
  }

  return 0;
}

