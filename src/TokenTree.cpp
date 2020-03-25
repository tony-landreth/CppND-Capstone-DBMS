#include "TokenTree.h"

// TODO: Define a delete function that
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

std::vector<TokenTree*> TokenTree::fetchNode(std::string tkn, std::vector<TokenTree*> *v){
  if( (this->token) == tkn) {
    v->push_back(this);
  }

  for(int i = 0; i < this->children.size(); i++) {
    this->children[i].fetchNode(tkn, v);
  }

  return *v;
}

TokenTree* TokenTree::find(std::string token) {
  std::vector<TokenTree*> tt;
  TokenTree* notFound = new TokenTree("NOT FOUND");
  fetchNode(token, &tt);

  if(tt.size() > 0){
    return tt[0];
  } else {
    return notFound;
  }
};

