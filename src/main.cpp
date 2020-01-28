#include "FileScan.h"

int main() {
  FileScan fs("movies");

  std::string s = fs.next();

  while(!s.empty()) {
    std::cout << s << std::endl;
    s = fs.next();
  }

  return 0;
}