#ifndef get_schema_H
#define get_schema_H
#include <map>
#include <string>

// Schema maintains a representation of the shape of
// data as it passes from files reads through a pipeline of
// PlanNodes.
struct Schema {
  std::string tableName;
  int tableSize = 0;
  std::map<std::string, int> columnKeys;
};

Schema get_schema(std::string tableName);

#endif
