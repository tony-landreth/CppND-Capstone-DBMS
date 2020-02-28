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

    for (char c: stop_char) {
      tkn.erase(std::remove(tkn.begin(), tkn.end(), c), tkn.end());
    }
  }
}

//TODO: see if you can make this a nextToken() method, no need for an argument
std::string Tokenizer::joinQuotedTokens(std::vector<std::string> tkns) {
  checkForEOQ();
  std::string str;
  str = tkns[pos];

  std::string quote_chars = "'";

    if(str.find(quote_chars) != std::string::npos) {
      // remove unneeded chars
      for (char c: quote_chars) {
        str.erase(std::remove(str.begin(), str.end(), c), str.end());
      }
      pos++;

      // join until you reach the next quotation mark
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

void Tokenizer::parseSelect() {
  // Advance to next token
  pos++;
  joinQuotedTokens(tkns);

  // Add all tokens until FROM is reached
  while(tkn != "FROM") {
    TokenTree node;
    node.token = tkn;
    sel.leaves.push_back(node);

    // Advance by one token
    pos++;
    joinQuotedTokens(tkns);
  }

  // Advance past FROM token
  pos++;
  joinQuotedTokens(tkns);

  TokenTree tbl;
  tbl.token = tkn;
  frm.leaves.push_back(tbl);

  pos++;
  joinQuotedTokens(tkns);

  // push additional tables if there are any
  while((tkn != "WHERE") && (tkn != "JOIN") && !endOfQuery) {
    TokenTree node;
    node.token = tkn;
    frm.leaves.push_back(node);

    // Advance by one token
    pos++;
    joinQuotedTokens(tkns);
  }
}

void Tokenizer::parseWhere() {
  while(( tkn != "JOIN" ) && (!endOfQuery)){
    checkForEOQ();

    pos++;
    if(pos>= tkns.size()) { break; }
    joinQuotedTokens(tkns);
    TokenTree prd;

    prd.token = tkn;

    pos++;
    if(pos>= tkns.size()) { break; }
    joinQuotedTokens(tkns);
    TokenTree eql;

    checkForEOQ();
    eql.token = "EQUALS";

    pos++;
    if(pos>= tkns.size()) { break; }
    joinQuotedTokens(tkns);
    TokenTree val;

    checkForEOQ();
    val.token = tkn;

    eql.leaves.push_back(val);
    prd.leaves.push_back(eql);
    whr.leaves.push_back(prd);

    pos++;
    if(pos>= tkns.size()) { break; }
    tkn = joinQuotedTokens(tkns);
  }
}

void Tokenizer::parseJoin() {
  //Advance past JOIN token
  //TODO: see if you can move pos++ into joinQuotedTokens
  pos++;
  joinQuotedTokens(tkns);

  TokenTree tbl;
  tbl.token = tkn;
  pos++;

  TokenTree on;
  on.token = "ON";
  pos++;

  TokenTree rId;
  rId.token = joinQuotedTokens(tkns);
  pos++;

  TokenTree eq;
  eq.token = "=";
  pos++;

  TokenTree lId;
  lId.token = joinQuotedTokens(tkns);
  pos ++;
  joinQuotedTokens(tkns);

  on.leaves.push_back(rId);
  on.leaves.push_back(eq);
  on.leaves.push_back(lId);
  tbl.leaves.push_back(on);
  jn.leaves.push_back(tbl);
}

TokenTree Tokenizer::tokenize(std::string str) {
  //Initialize the Token Tree
  TokenTree root;
  root.token = "ROOT TOKEN";
  sel.token = "SELECT";
  frm.token = "FROM";
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

  joinQuotedTokens(tkns);

  while(!endOfQuery) {
    if(tkn == "SELECT")
      parseSelect();

    if(tkn == "JOIN")
      parseJoin();

    if(tkn == "WHERE")
      parseWhere();

    if(pos > tkns.size())
      endOfQuery = true;
      break;
  }

  if(sel.leaves.size() > 0)
    root.leaves.push_back(sel);
  if(frm.leaves.size() > 0)
    root.leaves.push_back(frm);
  if(jn.leaves.size() > 0)
    root.leaves.push_back(jn);
  if(whr.leaves.size() > 0)
    root.leaves.push_back(whr);

  return root;
};
