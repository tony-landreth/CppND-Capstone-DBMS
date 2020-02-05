#include "FileScan.h"
#include "Selection.h"
#include "Projection.h"
#include "Join.h"

int main() {
  std::map<std::string,int> movie_schema = schema_loader("test_data");
  std::map<std::string,int> ratings_schema = schema_loader("test_data");
  std::unique_ptr<FileScan> mfs = std::make_unique<FileScan>("test_data", movie_schema);
  std::unique_ptr<FileScan> rfs = std::make_unique<FileScan>("test_data", ratings_schema);

  std::vector<std::string> triple{"title", "EQUALS","The Fall"};
  std::unique_ptr<Selection> mselect = std::make_unique<Selection>(triple, std::move( mfs ));
  std::unique_ptr<Selection> rselect = std::make_unique<Selection>(triple, std::move( rfs ));

  std::unique_ptr<Projection> mprojection = std::make_unique<Projection>("title", std::move( mselect ));
  std::unique_ptr<Projection> rprojection = std::make_unique<Projection>("title", std::move( rselect ));

  std::vector<std::string> keys{ "movieId", "movieId" };
  Join join( std::move( mprojection ), std::move( rprojection ), keys );
  std::vector<std::vector<std::string> > result = join.next();
  result = join.next();

  // You should have two rows of The Fall and no other matches because of your Selection triple
  if(result[0][0] == result[0][1])
    std::cout << "You have two matches for The Fall\n";

  return 0;
}
