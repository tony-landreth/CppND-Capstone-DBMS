#ifndef TOKENIZER_H
#define TOKENIZER_H

#include<vector>
#include<sstream> 

class TokenTree
{
  public:
    std::vector<TokenTree> leaves;
    std::string token;
    std::vector<std::string> depthFirstSearch(std::vector<std::string> *v);
};

class Tokenizer
{
  public:
    TokenTree tokenize(std::string str);
  private:
    std::string joinQuotedTokens(std::vector<std::string> tkns);
    int pos = 0;
};

#endif
