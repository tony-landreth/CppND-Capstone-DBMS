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

// destructor
TokenTree::~TokenTree(){
  children.clear();
};

// copy constructor
TokenTree::TokenTree(const TokenTree &source){
  token = source.token;
  children.clear();
  children = source.children;

};

/*
// copy assignment operator
TokenTree &TokenTree::operator=(const TokenTree &source){
  std::cout << "ASSIGNING content of instance " << &source << " to instance " << this << std::endl;
  if (this == &source){
    return *this;
  }

  token = source.token;
  children.clear();
  children = source.children;

  return *this;
};

TokenTree::TokenTree(TokenTree &&source){};                 // move constructor

// move assignment operator
TokenTree &TokenTree::operator=(TokenTree &&source){
  std::cout << "MOVING (assign) instance " << &source << " to instance " << this << std::endl;

  if (this == &source){
    return *this;
  }

  token = source.token;
  children.clear();
  children = source.children;

  return *this;
};      
*/
