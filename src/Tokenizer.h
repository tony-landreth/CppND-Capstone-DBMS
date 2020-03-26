#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <sstream>
#include <vector>

#include "TokenTree.h"
#include "utilities.h"

/*
   Tokenizer parses user input into meaningful tokens.
*/

class Tokenizer {
 public:
  // Method declarations
  TokenTree tokenize(const std::string str);

 private:
  // Method declarations

  // advances tkn to the next token in tkns and returns tkn
  std::string nextToken();
  // parses SELECT clauses, including FROM statements
  std::string parseSelect();

  // parses WHERE clauses, handles only statements of
  // the form: WHERE col1 = col2
  std::string parseWhere();
  // parses JOIN clauses, can only JOIN on one other table
  std::string parseJoin();
  // checks for the end of the user input query string
  void checkForEOQ();


  // Variable declarations
  std::vector<std::string> tkns;  // a vector to hold query tokens
  std::string tkn;                // a placeholder for the most recently processed token
  int pos = 0;                    // an index for iterating through tokens
  bool endOfQuery = false;

  TokenTree root;                 // a root node for the token tree
  TokenTree sel;                  // a node for a SELECT clause (only this clause is required
                                  // for a valid query)
  TokenTree whr;                  // a node for a WHERE clause
  TokenTree jn;                   // a node for a JOIN clause
};

#endif
