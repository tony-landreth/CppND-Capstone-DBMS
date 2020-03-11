#include "schema_loader.h"

// Hardcode the schema until write operations are implemented.
// Notably, the code below does not mark foreign keys and primary
// keys as such.

std::map<std::string, int> schema_loader(std::string schema_name) {
  std::map<std::string,int> schema;

  if(schema_name == "movies") {
        schema = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };
  } else if(schema_name == "ratings") {
      schema = {
        { "userId", 0 },
        { "movieId", 1 },
        { "rating", 2 },
        { "timestamp", 3}
      };
  } else if(schema_name == "test_data") {
      schema = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };
  };

  return schema;
}
