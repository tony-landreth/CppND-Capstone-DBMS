#include "FileScan.h"
#include "Selection.h"
#include "Projection.h"
#include "Join.h"
#include <sstream> 

//TODO: Enable Select to take * as input
//TODO: Create an abstraction for the code below

int main(int argc, char** argv) {
  // First query is: "SELECT * FROM movies"
  std::string arg;
  std::istringstream tkn_stream(argv[1]);
  std::vector<std::string> tkns;
  std::string tmp;

  std::vector<std::string> selectors;
  std::vector<std::string> tableNames;


  // Split argv on whitespace
  while (tkn_stream >> tmp) {
    tkns.push_back(tmp);
  }

  std::string tkn;

  for (int i = 0; i < tkns.size(); ++i) {
    tkn = tkns[i];

    if(tkn == "SELECT") {
      // Roll past SELECT token
      i++;
      tkn = tkns[i];

      while(tkn != "FROM") {
        std::cout << "select " << tkn << "\n"; 
        selectors.push_back(tkn);

        // Roll forward by one token
        i++;
        if(i >= tkns.size())
          break;

        tkn = tkns[i];
        // TODO: Implement handling for * in SELECT node
      }

      // Roll past FROM token
      i++;
      tkn = tkns[i];

      while(tkn != "WHERE") {
        std::cout << "from " << tkn << "\n"; 
        tableNames.push_back(tkn);

        // Roll forward by one token
        i++;
        if(i >= tkns.size())
          break;
        tkn = tkns[i];
      }
    }
    //std::unique_ptr<FileScan> fs = std::make_unique<FileScan>("test_data");
  }

  return 0;
}

