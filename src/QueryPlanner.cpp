#include "QueryPlanner.h"

QueryPlanner::QueryPlanner(int argc, char** argv) : argc_(argc), argv_(argv) {};

TokenTree QueryPlanner::tokenize()
{
  Tokenizer t;
  TokenTree tt = t.tokenize(argv_[1]);

  return tt;
};

// As you traverse the tree, build a pipeline of PlanNodes
// returning a single node on which you can call next()
std::map<std::string, std::vector<std::string> > QueryPlanner::buildQuery(TokenTree root) {
  // TODO: create an experiment file to try to figure out polymorphism with smart pointers
  // in /experiments
  int treeSize = root.leaves.size();

  TokenTree firstNode = root.leaves[0];

  int firstNodeSize = firstNode.leaves.size();
  //TODO: allow for handling multiple columns
  std::vector<std::string> selCols{ firstNode.leaves[0].token };
  queryData_.insert({"SELECT", selCols});

  int fromNodeIdx = 1;
  TokenTree fromNode = firstNode.leaves[fromNodeIdx];

  if(fromNode.token == "FROM") {
    std::string tblName = fromNode.leaves[0].token;
    queryData_.insert({"FROM", { tblName }});
  }

  if(root.leaves.size() > 1){
    TokenTree secondNode = root.leaves[1];
    if(secondNode.token == "WHERE") {
      std::string v1 = secondNode.leaves[0].token;
      std::string eq = secondNode.leaves[1].token;
      std::string v2 = secondNode.leaves[2].token;
      std::vector<std::string> whrClause{ v1, eq, v2 };
      queryData_.insert({"WHERE", whrClause});
    }
  }


  return queryData_;
}

void QueryPlanner::detectClauses(){
  if ( queryData_.find("SELECT") != queryData_.end() )
    selPresent_ = true; // found

  if ( queryData_.find("FROM") != queryData_.end() )
    frmPresent_ = true; // found

  if ( queryData_.find("WHERE") != queryData_.end() )
    whrPresent_ = true; // found

  if ( queryData_.find("JOIN") != queryData_.end() )
    jnPresent_ = true; // found
}

std::vector<std::vector<std::string> > QueryPlanner::run()
{
  std::vector<std::string> badQueryMsg{ "You have not submitted a valid query.", "The minimal query has the form SELECT * FROM table_name;" };
  std::vector<std::vector<std::string> > results;
  std::vector<std::string> row;
  TokenTree tt = tokenize();
  std::vector<std::string> where;
  int frmTableSize;

  std::map<std::string, std::vector<std::string> > queryData_ = buildQuery(tt);
  detectClauses(); // Set flags for clauses detected in queryData_

  // TODO: extract this into a method that returns a boolean indicating whether query is wff
  // Handle commands missing the minimal data to issue a query
  bool badQuery = ( !frmPresent_ || !selPresent_ || (queryData_["FROM"].size() > 1));
  if(badQuery){
    results.push_back(badQueryMsg);
    return results;
  }

  // Build FROM Node
  std::string tblName = queryData_["FROM"][0];
  std::unique_ptr<FileScan> frmFs = std::make_unique<FileScan>(tblName);
  frmFs->scanFile();
  frmTableSize = frmFs->tableSize;

  // Build SELECTION Node
  where = queryData_["WHERE"];
  std::vector<std::string> selCols = queryData_["SELECT"];
  std::unique_ptr<Selection> sel = std::make_unique<Selection>(where, std::move(frmFs));

  // Build Projection Node
  std::unique_ptr<Projection> prjR = std::make_unique<Projection>(selCols, std::move( sel ));

  // Build Join Node
  // TODO

  // Return query results
  for(int i = 0; i < frmTableSize; i++){
    row = prjR->next();
    if(row.size() > 0)
      results.push_back(row);
  }

  return results;
};
