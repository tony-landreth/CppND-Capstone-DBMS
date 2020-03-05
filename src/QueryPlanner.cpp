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
  std::map<std::string, std::vector<std::string> > queryData;

  TokenTree firstNode = root.leaves[0];

  int firstNodeSize = firstNode.leaves.size();
  //TODO: allow for handling multiple columns
  std::vector<std::string> selCols{ firstNode.leaves[0].token };
  queryData.insert({"SELECT", selCols});

  int fromNodeIdx = 1;
  TokenTree fromNode = firstNode.leaves[fromNodeIdx];

  if(fromNode.token == "FROM") {
    std::string tblName = fromNode.leaves[0].token;
    queryData.insert({"FROM", { tblName }});
  }

  if(root.leaves.size() > 1){
    TokenTree secondNode = root.leaves[1];
    if(secondNode.token == "WHERE") {
      std::string v1 = secondNode.leaves[0].token;
      std::string eq = secondNode.leaves[1].token;
      std::string v2 = secondNode.leaves[2].token;
      std::vector<std::string> whrClause{ v1, eq, v2 };
      queryData.insert({"WHERE", whrClause});
    }
  }


  return queryData;
}

std::vector<std::vector<std::string> > QueryPlanner::run()
{
  std::vector<std::string> badQueryMsg{ "You have not submitted a valid query.", "The minimal query has the form SELECT * FROM table_name;" };
  std::vector<std::vector<std::string> > results;
  std::vector<std::string> row;
  TokenTree tt = tokenize();
  std::vector<std::string> where;
  bool whrPresent = false;
  bool selPresent = false;
  bool jnPresent = false;
  bool frmPresent = false;
  int frmTableSize;

  std::map<std::string, std::vector<std::string> > queryData = buildQuery(tt);

  if ( queryData.find("SELECT") != queryData.end() )
    selPresent = true; // found

  if ( queryData.find("FROM") != queryData.end() )
    frmPresent = true; // found

  if ( queryData.find("WHERE") != queryData.end() )
    whrPresent = true; // found

  if ( queryData.find("JOIN") != queryData.end() )
    jnPresent = true; // found

  // TODO: extract this into a method that returns a boolean indicating whether query is wff
  // Handle commands missing the minimal data to issue a query
  bool badQuery = ( !frmPresent || !selPresent || (queryData["FROM"].size() > 1));
  if(badQuery){
    results.push_back(badQueryMsg);
    return results;
  }

  // Build FROM Node
  std::string tblName = queryData["FROM"][0];
  std::unique_ptr<FileScan> frmFs = std::make_unique<FileScan>(tblName);
  frmFs->scanFile();
  frmTableSize = frmFs->tableSize;

  // Build SELECTION Node
  where = queryData["WHERE"];
  std::vector<std::string> selCols = queryData["SELECT"];
  std::unique_ptr<Selection> sel = std::make_unique<Selection>(where, std::move(frmFs));

  // Build Projection Node
  std::unique_ptr<Projection> prjR = std::make_unique<Projection>(selCols, std::move( sel ));

  // Return query results
  for(int i = 0; i < frmTableSize; i++){
    row = prjR->next();
    if(row.size() > 0)
      results.push_back(row);
  }

  return results;

/*
    if(!jnPresent){
      // Pull data through Projection node
      row = prjR->next();

      while(row.size() > 0){
        results.push_back(row);
        row = prjR->next();
      }
    }

    if(jnPresent){
      //TODO: To make this legit, you'll have to build up an Fs -> Sel -> Prj from ON and SELECT cols
      std::unique_ptr<Projection> prjS = std::make_unique<Projection>(whrV, std::move( sel ));
      std::unique_ptr<Join> jn = std::make_unique<Join>(std::move( prjR ), std::move( prjS ), whrV);
    }
  }
  else{
    results.push_back(badQueryMsg);
    return results;
  }
*/
/*
  std::string tableName = query.back();
  std::string chars = ";\"";

  for(char c: chars) {
    tableName.erase(std::remove(tableName.begin(), tableName.end(), c), tableName.end());
  }

  std::unique_ptr<FileScan> fs = std::make_unique<FileScan>(tableName);
  FileScan* fs_ptr = fs.get();

  auto ftr = std::async(&FileScan::scanFile, fs_ptr);
  ftr.get();

  if(query[1] == "*") {
    std::vector<std::string> where{ query[0], query[1], query[2] };
    Selection sel(where, std::move(fs));
    std::vector<std::string> row = sel.next();

    while(row.size() > 0) {
      if(row[0].size() > 0)
        results.push_back(row);

      row = sel.next();
    }
  } else {
    // If SELECT includes a col name, draw rows from a projection
    // TODO: Allow for Projection to take a vector of col names
    std::vector<std::string> where{ query[0], "*", query[2] };
    std::unique_ptr<Selection> sel = std::make_unique<Selection>(where, std::move(fs));
    std::vector<std::string> col_names{ query[1] };
    Projection proj(col_names, std::move(sel));

    std::vector<std::string> row = proj.next();

    while(row.size() > 0) {
      if(row[0].size() > 0)
        results.push_back(row);

      row = proj.next();
    }
  }
  

  int i = 0;
  int j = 0;
  int k = 0;

  // Build return relation
  while(i < results.size()) {
    while(j < results[i].size()) {
      while(k < results[i][j].size()) {
        k++;
      }
    j++;
    }
    j = 0;
    k = 0;
    i++;
  }
*/
  return results;
};
