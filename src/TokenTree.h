#ifndef TOKENTREE_H
#define TOKENTREE_H

#include<vector>
#include<sstream> 

/*
   TokenTree stores user input into meaningful units for building
   a query.
*/

class TokenTree
{
  public:
    // Method declarations
    TokenTree(){};
    TokenTree(std::string token);
    std::vector<std::string> depthFirstSearch(std::vector<std::string> *v);
    TokenTree* find(std::string token);
    bool exist();

    //Variable declarations
    std::vector<TokenTree> children;
    std::string token;

  private:
    // Method declarations
    std::vector<TokenTree*> fetchNode(std::string tkn, std::vector<TokenTree*> *tt);
    
};

#endif
