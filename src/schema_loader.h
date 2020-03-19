#ifndef SCHEMA_LOADER_H
#define SCHEMA_LOADER_H
#include<string>
#include<map>

struct TableSchema
{
  std::string tableName;
  int tableSize;
  std::map<std::string, int> columnKeys;
};

TableSchema schema_loader(std::string tableName);

#endif
