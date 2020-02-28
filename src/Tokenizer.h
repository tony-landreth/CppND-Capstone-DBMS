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
    std::vector<std::string> tkns;
    std::string tkn;
    int pos = 0;
    void parseSelect();
    void parseWhere();
    void parseJoin();
    bool endOfQuery = false;
    void checkForEOQ();

    TokenTree sel;
    TokenTree frm;
    TokenTree whr;
    TokenTree jn;
};

#endif
