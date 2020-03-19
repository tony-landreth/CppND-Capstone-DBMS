#include "QueryPlanner.h"
#include <algorithm>

QueryPlanner::QueryPlanner(int argc, char** argv) : argc_(argc), argv_(argv) {};

TokenTree QueryPlanner::tokenize()
{
  Tokenizer t;
  TokenTree tt = t.tokenize(argv_[1]);

  return tt;
};

// As you traverse the tree, build a pipeline of PlanNodes
// returning a single node on which you can call next()
// TODO: Rename to collectQueryData
std::map<std::string, std::vector<std::string> > QueryPlanner::buildQuery(TokenTree root) {
  int treeSize = root.leaves.size();

  TokenTree firstNode = root.leaves[0];

  int firstNodeSize = firstNode.leaves.size();
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
    if(secondNode.token == "JOIN") {
      std::string tbl = secondNode.leaves[0].token;
      std::string on = secondNode.leaves[1].token;
      std::string rKey = secondNode.leaves[1].leaves[0].token;
      std::string eq = secondNode.leaves[3].token;
      std::string sKey = secondNode.leaves[1].leaves[2].token;
      std::vector<std::string> jnClause{ tbl, rKey, sKey };
      queryData_.insert({"JOIN", jnClause});
    }
  }

  if(root.leaves.size() > 2){
    TokenTree thirdNode = root.leaves[2];
    if(thirdNode.token == "WHERE") {
      std::string v1 = thirdNode.leaves[0].token;
      std::string eq = thirdNode.leaves[1].token;
      std::string v2 = thirdNode.leaves[2].token;
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

  // Handle commands missing the minimal data to issue a query
  bool badQuery = ( !frmPresent_ || !selPresent_ || (queryData_["FROM"].size() > 1));
  if(badQuery){
    results.push_back(badQueryMsg);
    return results;
  }

  // Build rFileScan Node
  std::string tblName = queryData_["FROM"][0];
  std::unique_ptr<FileScan> frmFs = std::make_unique<FileScan>(tblName);
  frmFs->scanFile();
  frmTableSize = frmFs->tableSize;
  TableSchema frmFsSchema = frmFs->schema;

  // Build SELECTION Node
  where = queryData_["WHERE"];

  // Determine which columns will be displayed to the user
  std::vector<std::string> frontEndSelCols = queryData_["SELECT"];
  if(frontEndSelCols[0] == "*"){
    std::vector<std::string> allCols;
    std::vector<int> allIdxs;
    std::map<int, std::string> revMap;
    std::map<std::string, int> colKeys = frmFsSchema.columnKeys;

    for(const auto &x: colKeys){
      revMap[x.second] = x.first;
      allIdxs.push_back(x.second);
    }

    sort(allIdxs.begin(), allIdxs.end());
    for(int i = 0; i < allIdxs.size(); i++){
      allCols.push_back(revMap[i]);
    }

    frontEndSelCols = allCols;
  }
  // Collect columns required to build intermediate query results
  std::vector<std::string> backEndSelCols = frontEndSelCols;
  std::unique_ptr<Selection> sel = std::make_unique<Selection>(where, std::move(frmFs), frmFsSchema);

  // Build Projection Node
  std::unique_ptr<Projection> prjR;

  if(jnPresent_ ){
    // Add JOIN keys to projection node
    std::vector<std::string> jnParams = queryData_["JOIN"];
    std::string sTblName = jnParams[0];
    std::vector<std::string> jnKeys{ jnParams[1], jnParams[2] };

    // TODO: create a separate selCols, i.e. rSelCols and sSeCols
    // So you can handle foreign keys with different names
    for(std::string k : jnKeys){
      if(std::find(backEndSelCols.begin(), backEndSelCols.end(), k) == backEndSelCols.end()){
        backEndSelCols.push_back(k);
      }
    } 

    prjR = std::make_unique<Projection>(backEndSelCols, std::move( sel ), frmFsSchema);
  } else {
    prjR = std::make_unique<Projection>(frontEndSelCols, std::move( sel ), frmFsSchema);
  }

  // Build Join Node
  if(jnPresent_ ){
    std::vector<std::string> jnParams = queryData_["JOIN"];
    // Build sFileScan Node
    std::string sTblName = jnParams[0];
    std::vector<std::string> jnKeys{ jnParams[1], jnParams[2] };
    std::unique_ptr<FileScan> sFrmFs = std::make_unique<FileScan>(sTblName);
    sFrmFs->scanFile();

    // Build sSelection Node
    std::unique_ptr<Selection> sSel = std::make_unique<Selection>(where, std::move(sFrmFs), frmFsSchema);

    // Build sProjection
    std::unique_ptr<Projection> prjS = std::make_unique<Projection>(backEndSelCols, std::move( sSel ), frmFsSchema);
    std::unique_ptr<Join> jn = std::make_unique<Join>(std::move(prjR), std::move(prjS), jnKeys, frmFsSchema, frmFsSchema);

    std::vector<std::string> fixtureKeys{ "title" };
    TableSchema jnSchema;
    jnSchema.tableName = "virtual";
    jnSchema.tableSize = jn->schema.tableSize;

    std::unique_ptr<Projection> sansForeignKeys = std::make_unique<Projection>(frontEndSelCols, std::move( jn ), jnSchema);

    for(int i = 0; i < frmTableSize; i++){
      row = sansForeignKeys->next();

      if(row.size() > 0) {
        results.push_back(row);
      }
    }
  } else {
    for(int i = 0; i < frmTableSize; i++){
      row = prjR->next();
      if(row.size() > 0)
        results.push_back(row);
    }
  }

  return results;
};
