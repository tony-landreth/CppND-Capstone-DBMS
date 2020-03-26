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
  QueryPlanner qp = QueryPlanner(argc, argv);
  std::unique_ptr<std::vector<std::vector<std::string> > > result = qp.run();

  for(int i = 0; i < result->size(); i++) {
    std::vector<std::vector<std::string> > row = *result.get();

    for(int j = 0; j < row[i].size(); j++) {
      std::string r = row[i][j];
      int rowLength = row[i].size();
    
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

