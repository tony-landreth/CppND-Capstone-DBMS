#include "schema_loader.h"

// Hardcodes the schema until write operations are implemented.
// Notably, the code below does not mark foreign keys and primary
// keys as such.

Schema schema_loader(std::string tableName){
  Schema schema;
  std::map<std::string, int> colKeys;
  int tableSize;

  if(tableName == "movies") {
      colKeys = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };
      tableSize = 100;
  } else if(tableName == "ratings") {
      colKeys = {
        { "userId", 0 },
        { "movieId", 1 },
        { "rating", 2 },
        { "timestamp", 3}
      };
      tableSize = 41702;
  } else if(tableName == "test_data") {
      colKeys = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };
      tableSize = 4;
  };

  schema.tableName = tableName;
  schema.tableSize = tableSize;
  schema.columnKeys = colKeys;

  return schema;
};
