#ifndef TOKENTREE_H
#define TOKENTREE_H

#include <iostream>
#include <sstream>
#include <vector>

/*
   TokenTree stores user input into meaningful units for building
   a query.
*/

class TokenTree {
 public:
  // Method declarations
  TokenTree(){};                                  // constructor
  TokenTree(std::string token);                   // constructor
  ~TokenTree();                                   // destructor
  TokenTree(const TokenTree &source);             // copy constructor
  TokenTree &operator=(const TokenTree &source);  // copy assignment operator
  TokenTree(TokenTree &&source);                  // move constructor
  TokenTree &operator=(TokenTree &&source);       // move assignment operator

  std::vector<std::string> depthFirstSearch(std::vector<std::string> *v);
  TokenTree *find(std::string token);
  bool exist();

  // Variable declarations
  std::vector<TokenTree> children;
  std::string token;

 private:
  // Method declarations
  std::vector<TokenTree *> fetchNode(std::string tkn,
                                     std::vector<TokenTree *> *tt);
};

#endif
