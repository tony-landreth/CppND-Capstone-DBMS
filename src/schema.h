#ifndef SCHEMA_LOADER_H
#define SCHEMA_LOADER_H
#include<string>
#include<map>

// Schema maintains a representation of the shape of
// data as it passes from files reads through a pipeline of
// PlanNodes.
struct Schema
{
  std::string tableName;
  int tableSize = 0;
  std::map<std::string, int> columnKeys;
};

Schema schema_loader(std::string tableName);

#endif
