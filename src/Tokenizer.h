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
    std::vector<std::string> tkns;          // a vector to hold query tokens
    std::string tkn;                        // a placeholder for the most recently processed token
    int pos = 0;                            // an index for iterating through tokens

    std::string nextToken();                // advances tkn to the next token in tkns and returns tkn
    std::string parseSelect();              // parses SELECT clauses, including FROM statements
    std::string parseWhere();               // parses WHERE clauses, handles only statements of the form: WHERE col1 = col2
    std::string parseJoin();                // parses JOIN clauses, can only JOIN on one other table

    bool endOfQuery = false;
    void checkForEOQ();                     // checks whether the most recently processed token contained a semi-colon

    TokenTree root;                         // a root node for the token tree
    TokenTree sel;                          // a node for a SELECT clause (only this clause is required for a valid query)
    TokenTree whr;                          // a node for a WHERE clause
    TokenTree jn;                           // a node for a JOIN clause
};

#endif
