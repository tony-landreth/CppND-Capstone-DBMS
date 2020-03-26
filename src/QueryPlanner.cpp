#include "QueryPlanner.h"

QueryPlanner::QueryPlanner(int argc, char** argv) : argc_(argc), argv_(argv){};

// For JOIN queries, column names should be of the form tableName.columnName
// In this method, column names get binned to their respective table in
// preparation To initialize separate Projection nodes for each set of column
// names
std::vector<std::vector<std::string> > QueryPlanner::binProjectionKeys(
    std::vector<std::string> prjKeys) {
  std::map<std::string, std::vector<std::string> > bins;
  std::vector<std::vector<std::string> > result;
  char const del = '.';
  std::string firstKey = prjKeys[0];

  // If the first key contains a dot, you're dealing with a JOIN
  if (firstKey.find(".") != std::string::npos) {
    for (std::string str : prjKeys) {
      std::vector<std::string> s = split(str, del);
      std::string tableKey = s[0];
      std::string colKey = s[1];

      // If table key doesn't have a bin, make one and add column key
      if (bins.find(tableKey) == bins.end()) {
        std::vector<std::string> colKeyV{colKey};
        bins.insert({tableKey, colKeyV});
      } else {
        bins[tableKey].push_back(colKey);
      }
    }

    // Push the contents of each bin into the result vector
    std::map<std::string, std::vector<std::string> >::iterator it =
        bins.begin();

    while (it != bins.end()) {
      result.push_back(it->second);
      it++;
    }
  } else {
    result.push_back(prjKeys);
  }

  return result;
};

std::vector<std::vector<std::string> > QueryPlanner::buildFrontEndColumnNames(
    std::vector<std::string> columnNames, Schema schema) {
  std::vector<std::vector<std::string> > result;

  if (columnNames[0] == "*") {
    std::vector<std::string> allCols;
    std::vector<int> allIdxs;
    std::map<int, std::string> revMap;
    std::map<std::string, int> colKeys = schema.columnKeys;

    for (const auto& x : colKeys) {
      revMap[x.second] = x.first;
      allIdxs.push_back(x.second);
    }

    sort(allIdxs.begin(), allIdxs.end());
    for (int i = 0; i < allIdxs.size(); i++) {
      allCols.push_back(revMap[i]);
    }

    result.push_back(allCols);
    return result;
  } else {
    return binProjectionKeys(columnNames);
  }
}

TokenTree QueryPlanner::tokenize() {
  Tokenizer t;
  TokenTree tt = t.tokenize(argv_[1]);

  return tt;
};

// Convert token tree into map to simplify logic when building query
std::map<std::string, std::vector<std::string> > QueryPlanner::mapQuery(
    TokenTree root) {
  int treeSize = root.children.size();
  std::vector<std::string> selCols;
  std::vector<TokenTree> selChildren = select_->children;

  for (TokenTree t : selChildren) {
    if (t.token != "FROM") {
      selCols.push_back(t.token);
    }
  }

  queryData_.insert({"SELECT", selCols});

  if (from_->exist()) {
    std::string tblName = from_->children[0].token;
    queryData_.insert({"FROM", {tblName}});
  }

  if (where_->exist()) {
    std::vector<TokenTree> whrChildren = where_->children;
    TokenTree v1 = whrChildren[0];
    std::string v1Token = whrChildren[0].token;

    std::vector<TokenTree> v1Children = v1.children;
    TokenTree eq = v1Children[0];
    std::string eqToken = eq.token;

    std::vector<TokenTree> eqChildren = eq.children;
    TokenTree v2 = eqChildren[0];
    std::string v2Token = v2.token;
    std::vector<std::string> whrClause{v1Token, eqToken, v2Token};
    queryData_.insert({"WHERE", whrClause});
  }

  if (join_->exist()) {
    std::vector<TokenTree> jnChildren = join_->children;
    TokenTree tbl = jnChildren[0];
    std::string tblToken = tbl.token;

    TokenTree* on = join_->find("ON");
    std::vector<TokenTree> onChildren = on->children;

    std::string rKey = onChildren[0].token;
    std::string sKey = onChildren[2].token;
    std::vector<std::string> jnClause{tblToken, rKey, sKey};
    queryData_.insert({"JOIN", jnClause});
  }

  return queryData_;
}

void QueryPlanner::detectClauses() {
  select_ = tokenTree_.find("SELECT");
  from_ = tokenTree_.find("FROM");
  where_ = tokenTree_.find("WHERE");
  join_ = tokenTree_.find("JOIN");

  if (select_->exist()) selPresent_ = true;

  if (from_->exist()) frmPresent_ = true;  // found

  if (where_->exist()) whrPresent_ = true;  // found

  if (join_->exist()) jnPresent_ = true;  // found
}

// As you traverse the tree, build a pipeline of PlanNodes
// returning a single node on which you can call next()
std::unique_ptr<std::vector<std::vector<std::string> > > QueryPlanner::run() {
  std::vector<std::string> badQueryMsg{
      "You have not submitted a valid query.",
      "The minimal query has the form SELECT * FROM table_name;"};

  std::unique_ptr<std::vector<std::vector<std::string> > > results =
      std::make_unique<std::vector<std::vector<std::string> > >();
  std::vector<std::string> row;
  tokenTree_ = tokenize();
  std::vector<std::string> where;

  detectClauses();  // Set flags for clauses detected in queryData_
  std::map<std::string, std::vector<std::string> > queryData_ =
      mapQuery(tokenTree_);

  // Handle commands missing the minimal data to issue a query
  bool badQuery =
      (!frmPresent_ || !selPresent_ || (queryData_["FROM"].size() > 1));
  if (badQuery) {
    results->push_back(badQueryMsg);
    return std::move(results);
  }

  // Build rFileScan Node
  std::string tblName = from_->children[0].token;
  Schema schema = get_schema(tblName);
  int frmTableSize = schema.tableSize;
  std::unique_ptr<FileScan> rFs = std::make_unique<FileScan>(schema);
  rFs->scanFile();

  // Build SELECTION Node
  where = queryData_["WHERE"];

  // Determine which columns will be displayed to the user
  std::vector<std::vector<std::string> > projectionBins =
      buildFrontEndColumnNames(queryData_["SELECT"], schema);
  // Collect columns required to build user-facing results
  std::vector<std::string> frontEndSelCols;
  // Collect columns required to build intermediate query results
  std::vector<std::string> rBackEndSelCols;
  std::vector<std::string> sBackEndSelCols;

  if (projectionBins.size() == 2) {
    rBackEndSelCols = projectionBins[0];
    sBackEndSelCols = projectionBins[1];

    frontEndSelCols.reserve(projectionBins[0].size() +
                            projectionBins[1].size());
    frontEndSelCols.insert(frontEndSelCols.end(), rBackEndSelCols.begin(),
                           rBackEndSelCols.end());
    frontEndSelCols.insert(frontEndSelCols.end(), sBackEndSelCols.begin(),
                           sBackEndSelCols.end());

  } else {
    rBackEndSelCols = projectionBins[0];
    sBackEndSelCols = projectionBins[0];
    frontEndSelCols = projectionBins[0];
  }

  std::unique_ptr<Selection> sel =
      std::make_unique<Selection>(where, std::move(rFs), schema);

  // Build Projection Node
  std::unique_ptr<Projection> prjR;
  Schema sTblSchema;

  if (jnPresent_) {
    // Add JOIN keys to projection node
    std::vector<std::string> jnParams = queryData_["JOIN"];
    std::string sTblName = jnParams[0];
    sTblSchema = get_schema(sTblName);
    std::string rJnKey = jnParams[1];
    std::string sJnKey = jnParams[2];

    // If the JOIN keys aren't already in the Projection, add them
    if (std::find(rBackEndSelCols.begin(), rBackEndSelCols.end(), rJnKey) ==
        rBackEndSelCols.end()) {
      rBackEndSelCols.push_back(rJnKey);
    }

    // If the JOIN keys aren't already in the Projection, add them
    if (std::find(sBackEndSelCols.begin(), sBackEndSelCols.end(), rJnKey) ==
        sBackEndSelCols.end()) {
      sBackEndSelCols.push_back(sJnKey);
    }

    prjR =
        std::make_unique<Projection>(rBackEndSelCols, std::move(sel), schema);
  } else {
    prjR =
        std::make_unique<Projection>(frontEndSelCols, std::move(sel), schema);
  }

  // Build Join Node
  if (jnPresent_) {
    std::vector<std::string> jnParams = queryData_["JOIN"];

    // Build sFileScan Node
    std::string sTblName = jnParams[0];
    sTblSchema = get_schema(sTblName);
    std::vector<std::string> jnKeys{jnParams[1], jnParams[2]};
    std::unique_ptr<FileScan> sFs = std::make_unique<FileScan>(sTblSchema);
    sFs->scanFile();

    // Build sSelection Node
    std::unique_ptr<Selection> sSel =
        std::make_unique<Selection>(where, std::move(sFs), sTblSchema);

    // Build sProjection
    std::unique_ptr<Projection> prjS = std::make_unique<Projection>(
        sBackEndSelCols, std::move(sSel), sTblSchema);
    std::unique_ptr<Join> jn = std::make_unique<Join>(
        std::move(prjR), std::move(prjS), jnKeys, schema, sTblSchema);

    // Get the Join node's representation of the data it will return
    Schema jnSchema = jn->getSchema();
    jnSchema.tableName = "virtual";

    std::unique_ptr<Projection> sansForeignKeys =
        std::make_unique<Projection>(frontEndSelCols, std::move(jn), jnSchema);

    for (int i = 0; i < frmTableSize; i++) {
      row = sansForeignKeys->next();

      if (row.size() > 0) {
        results->push_back(row);
      }
    }
  } else {
    for (int i = 0; i < frmTableSize; i++) {
      row = prjR->next();
      if (row.size() > 0) results->push_back(row);
    }
  }

  return std::move(results);
};
