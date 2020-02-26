#include "Tokenizer.h"
#include<iostream>

std::vector<std::string> TokenTree::depthFirstSearch(std::vector<std::string> *v) {
  v->push_back(this->token);

  for(int i = 0; i < this->leaves.size(); i++) {
    this->leaves[i].depthFirstSearch(v);
  }

  return *v;
}

std::string Tokenizer::joinQuotedTokens(std::vector<std::string> tkns) {
  std::string str = tkns[pos];
  std::string chars = "\"\\;";

    if(str.find("\"") != std::string::npos) {
      // remove unneeded chars
      for (char c: chars) {
        str.erase(std::remove(str.begin(), str.end(), c), str.end());
      }
      pos++;

      // join until you reach the next quotation mark
      while(true) {
        str = str + " " + tkns[pos];
        // remove unneeded chars
        for (char c: chars) {
          str.erase(std::remove(str.begin(), str.end(), c), str.end());
        }

        if(tkns[pos].find("\"") != std::string::npos) {
          // remove slash and quote and semi-colon
          break;
        } else {
          pos++;
        }
      }
    }

  return str;
}

TokenTree Tokenizer::tokenize(std::string str) {
  bool endOfQuery = false;

  //Initialize the Token Tree
  TokenTree root;
  root.token = "ROOT TOKEN";

  // Add a SELECT node
  TokenTree sel;
  sel.token = "SELECT";

  std::istringstream tkn_stream(str);
  std::vector<std::string> tkns;
  std::string tkn;
  std::string tmp;
  std::vector<std::string> selectors;
  std::vector<std::string> tableNames;


  // Split argv on whitespace
  while (tkn_stream >> tmp) {
    tkns.push_back(tmp);
  }

  // Add a FROM node
  TokenTree frm;
  frm.token = "FROM";

  // Add a WHERE node
  TokenTree whr;
  whr.token = "WHERE";

  for (; pos < tkns.size(); ++pos) {
    tkn = tkns[pos];

    if(tkn == "SELECT") {
      // Advance to next token
      pos++;
      tkn = tkns[pos];

      // Add all tokens until FROM is reached
      while(tkn != "FROM") {
        TokenTree node;
        node.token = tkn;
        sel.leaves.push_back(node);

        // Advance by one token
        pos++;
        tkn = tkns[pos];
      }

      // Advance past FROM token
      pos++;
      tkn = tkns[pos];

      // Remove semi-colon, indicating end of query statement
      if(tkn.find(";") != std::string::npos){
        endOfQuery = true;
        tkn.erase(tkn.size() - 1);
      }

      // Advance until JOIN or WHERE
      while( tkn != "WHERE" ) {
        // push table name
        TokenTree frmTable;
        frmTable.token = tkn;
        frm.leaves.push_back(frmTable);

        // Advance by one token
        pos++;
        if(pos>= tkns.size())
          break;
        tkn = tkns[pos];
      }
      // Advance until ;
      if(endOfQuery) { break; }

      if(tkn == "WHERE") {
        pos++;
        if(pos>= tkns.size()) { break; }
        tkn = tkns[pos];
        TokenTree prd;
        prd.token = tkn;

        pos++;
        if(pos>= tkns.size()) { break; }
        tkn = tkns[pos];
        TokenTree eql;
        eql.token = "EQUALS";

        pos++;
        if(pos>= tkns.size()) { break; }
        tkn = joinQuotedTokens(tkns);
        TokenTree val;
        val.token = tkn;

        eql.leaves.push_back(val);
        prd.leaves.push_back(eql);
        whr.leaves.push_back(prd);

        pos++;
        if(pos>= tkns.size()) { break; }
        tkn = tkns[pos];
      }
    }
  }

  if(sel.leaves.size() > 0)
    root.leaves.push_back(sel);
  if(frm.leaves.size() > 0)
    root.leaves.push_back(frm);
  if(whr.leaves.size() > 0)
    root.leaves.push_back(whr);

  return root;
};
