#include "Tokenizer.h"
#include<iostream>

// TODO: Move TokenTree into its own files and define a delete function that
// TODO: deletes all of a TokenTree's children

TokenTree::TokenTree(std::string tkn) : token(tkn){};

bool TokenTree::exist(){
  return (token != "NOT FOUND");
}

// Method for Testing
std::vector<std::string> TokenTree::depthFirstSearch(std::vector<std::string> *v) {
  v->push_back(this->token);

  for(int i = 0; i < this->children.size(); i++) {
    this->children[i].depthFirstSearch(v);
  }

  return *v;
}

std::vector<TokenTree> TokenTree::fetchNode(std::string tkn, std::vector<TokenTree> *v){
  if( (this->token) == tkn) {
    v->push_back(this->token);
  }

  for(int i = 0; i < this->children.size(); i++) {
  ; this->children[i].fetchNode(tkn, v);
  }

  return *v;
}

TokenTree TokenTree::find(std::string token) {
  std::vector<TokenTree> tt;
  TokenTree notFound("NOT FOUND");
  fetchNode(token, &tt);

  if(tt.size() > 0){
    return tt[0];
  } else {
    return notFound;
  }
};

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
    sel.children.push_back(node);

    // Advance by one token
     curr = nextToken();
  }

  // Capture FROM token
  TokenTree frm;
  frm.token = curr;

  TokenTree tbl;
  tbl.token = nextToken();
  frm.children.push_back(tbl);


  curr = nextToken();

  // push additional tables if there are any
  while((curr != "WHERE") && (curr != "JOIN") && !endOfQuery) {
    TokenTree node;
    node.token = curr;
    frm.children.push_back(node);

    // Advance by one token
    nextToken();
  }
  sel.children.push_back(frm);

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

    eql.children.push_back(val);
    prd.children.push_back(eql);
    whr.children.push_back(prd);

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

  on.children.push_back(rId);
  on.children.push_back(eq);
  on.children.push_back(lId);
  tbl.children.push_back(on);
  jn.children.push_back(tbl);

  return curr;
}

TokenTree Tokenizer::tokenize(std::string str) {
  //Initialize the Token Tree
  root.token = "ROOT TOKEN";
  sel.token = "SELECT";
  whr.token = "WHERE";
  jn.token = "JOIN";

  //TODO: Figure out why semi-colon gets stripped from argv
  //str = str + ";";
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

  if(sel.children.size() > 0)
    root.children.push_back(sel);
  if(jn.children.size() > 0)
    root.children.push_back(jn);
  if(whr.children.size() > 0)
    root.children.push_back(whr);

  return root;
};
