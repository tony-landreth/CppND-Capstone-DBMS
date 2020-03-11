#include "schema_loader.h"

// Hardcode the schema until write operations are implemented.
// Notably, the code below does not mark foreign keys and primary
// keys as such.

TableSchema schema_loader(std::string tableName){
  TableSchema schema;
  std::map<std::string, int> colKeys;

  if(tableName == "movies") {
      colKeys = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };
  } else if(tableName == "ratings") {
      colKeys = {
        { "userId", 0 },
        { "movieId", 1 },
        { "rating", 2 },
        { "timestamp", 3}
      };
  } else if(tableName == "test_data") {
      colKeys = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };
  };

  schema.tableName = tableName;
  schema.columnKeys = colKeys;

  return schema;
};
