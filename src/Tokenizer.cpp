#include "Tokenizer.h"
#include<iostream>

// Method for Testing
std::vector<std::string> TokenTree::depthFirstSearch(std::vector<std::string> *v) {
  v->push_back(this->token);

  for(int i = 0; i < this->leaves.size(); i++) {
    this->leaves[i].depthFirstSearch(v);
  }

  return *v;
}

void Tokenizer::checkForEOQ(){
  std::string stop_char = ";";

  if(tkn.find(";") != std::string::npos) {
    endOfQuery = true;

    // Remove semi-colon from last token in query
    for (char c: stop_char) {
      tkn.erase(std::remove(tkn.begin(), tkn.end(), c), tkn.end());
    }
  }
}

std::string Tokenizer::nextToken() {
  checkForEOQ();

  // Advance only after the first token has been evaluated
  if(!tkn.empty())
    pos++;

  if(pos >= tkns.size())
    return tkn;

  std::string str;
  str = tkns[pos];


  // Join elements of quoted expressions
  std::string quote_chars = "'";
  if(str.find(quote_chars) != std::string::npos) {
    // remove unneeded chars
    for (char c: quote_chars) {
      str.erase(std::remove(str.begin(), str.end(), c), str.end());
    }

    // Advance to next token
    pos++;

    // Join until you reach the next quotation mark
    while(true) {
      str = str + " " + tkns[pos];
      // remove unneeded chars

      if(str.find(quote_chars) != std::string::npos) {
        break;
      } else {
        pos++;
      }
    }
  }

  for (char c: quote_chars) {
    str.erase(std::remove(str.begin(), str.end(), c), str.end());
  }
  tkn = str;
  checkForEOQ();
  return tkn;
}

std::string Tokenizer::parseSelect() {
  std::string curr = nextToken();

  // Add all tokens until FROM is reached
  while(curr != "FROM") {
    TokenTree node;
    node.token = curr;
    sel.leaves.push_back(node);

    // Advance by one token
     curr = nextToken();
  }

  // Capture FROM token
  TokenTree frm;
  frm.token = curr;

  TokenTree tbl;
  tbl.token = nextToken();
  frm.leaves.push_back(tbl);

  curr = nextToken();

  // push additional tables if there are any
  while((curr != "WHERE") && (curr != "JOIN") && !endOfQuery) {
    TokenTree node;
    node.token = curr;
    frm.leaves.push_back(node);

    // Advance by one token
    nextToken();
  }
  sel.leaves.push_back(frm);

  return curr;
}

std::string Tokenizer::parseWhere() {
  std::string curr = nextToken();

  while(( curr != "JOIN" ) && (!endOfQuery)){
    checkForEOQ();

    nextToken();
    TokenTree prd;

    prd.token = curr;

    nextToken();
    TokenTree eql;

    checkForEOQ();
    eql.token = "EQUALS";

    curr = nextToken();
    TokenTree val;

    checkForEOQ();
    val.token = curr;

    eql.leaves.push_back(val);
    prd.leaves.push_back(eql);
    whr.leaves.push_back(prd);

     curr = nextToken();
  }
  return curr;
}

std::string Tokenizer::parseJoin() {

  TokenTree tbl;
  tbl.token = nextToken();

  TokenTree on;
  on.token = nextToken();

  TokenTree rId;
  rId.token = nextToken();

  TokenTree eq;
  eq.token = nextToken();

  TokenTree lId;
  lId.token = nextToken();
  std::string curr = nextToken();

  on.leaves.push_back(rId);
  on.leaves.push_back(eq);
  on.leaves.push_back(lId);
  tbl.leaves.push_back(on);
  jn.leaves.push_back(tbl);

  return curr;
}

TokenTree Tokenizer::tokenize(std::string str) {
  //Initialize the Token Tree
  root.token = "ROOT TOKEN";
  sel.token = "SELECT";
  whr.token = "WHERE";
  jn.token = "JOIN";

  std::istringstream tkn_stream(str);
  std::string tmp;
  std::vector<std::string> selectors;
  std::vector<std::string> tableNames;


  // Split argv on whitespace
  while (tkn_stream >> tmp) {
    tkns.push_back(tmp);
  }

  std::string curr = nextToken();

  while(!endOfQuery) {
    if(curr == "SELECT")
      curr = parseSelect();

    if(curr == "JOIN")
      curr = parseJoin();

    if(curr == "WHERE")
      curr = parseWhere();
  }

  if(sel.leaves.size() > 0)
    root.leaves.push_back(sel);
  if(jn.leaves.size() > 0)
    root.leaves.push_back(jn);
  if(whr.leaves.size() > 0)
    root.leaves.push_back(whr);

  return root;
};
