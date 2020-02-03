#include <iostream>
#include "gtest/gtest.h"
#include "../src/FileScan.h"
#include "../src/Selection.h"
#include "../src/Projection.h"

class FileScanTest : public ::testing::Test {
  protected:
      std::map<std::string,int> schema = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };

    //TODO: select a subset of movie.csv as test data
    FileScan fs{"test_data", schema};
};

TEST_F(FileScanTest, TestNext) {
  std::vector<std::string> row = fs.next();
  EXPECT_EQ(row[0], "movieId");
  EXPECT_EQ(row[1], "title");
  EXPECT_EQ(row[2], "genres");
  
  row = fs.next();
  std::vector<std::string> expectProperCommaHandling{ "1","\"A Movie Title, With Commas, In the Title\"","Adventure|Animation|Children|Comedy|Fantasy" };
  EXPECT_EQ(expectProperCommaHandling, row);
};

class SelectionTest : public ::testing::Test {
  protected:
      std::map<std::string,int> schema = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };

    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>("test_data", schema);
    std::vector<std::string> triple{"title", "EQUALS","The Fall"};
    Selection select{triple, std::move( fs )};
};

TEST_F(SelectionTest, TestNext) {
  // Advance to a row where title = "The Fall"
  select.next();
  select.next();
  std::vector<std::string> row = select.next();

  std::vector<std::string> theFallRow{ "2", "The Fall", "Adventure|Fantasy" };
  EXPECT_EQ(row, theFallRow);
}

class ProjectionTest : public ::testing::Test {
  protected:
      std::map<std::string,int> schema = {
        { "movieId", 0 },
        { "title",   1 },
        { "genres",  2 }
      };

    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>("test_data", schema);
    std::vector<std::string> triple{"title", "EQUALS","The Fall"};
    std::unique_ptr<Selection> select = std::make_unique<Selection>(triple, std::move( fs ));
    Projection projection{"title", std::move( select )};
};

TEST_F(ProjectionTest, TestNext) {
  // Advance to a row where title = "The Fall"
  projection.next();
  projection.next();
  std::vector<std::string> row = projection.next();
  std::vector<std::string> expectation{ "The Fall" };
  EXPECT_EQ(row, expectation);
}
