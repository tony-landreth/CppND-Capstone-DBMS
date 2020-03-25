#include "utilities.h"

// This is a cribbed function from Fluent C++
// See: https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);

   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


std::string removeChars(std::string str, std::string chars){

  for (char c: chars) {
    str.erase(std::remove(str.begin(), str.end(), c), str.end());
  }
  return str;
}
