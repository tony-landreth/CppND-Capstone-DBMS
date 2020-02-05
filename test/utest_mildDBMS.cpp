#include <iostream>
#include "gtest/gtest.h"
#include "../src/FileScan.h"
#include "../src/Selection.h"
#include "../src/Projection.h"
#include "../src/Join.h"
#include "../src/schema_loader.h"

class FileScanTest : public ::testing::Test {
  protected:
    std::map<std::string,int> schema = schema_loader("movies");
    FileScan fs{"test_data", schema};
};

TEST_F(FileScanTest, TestNext) {
  std::vector<std::vector<std::string> > relation = fs.next();
  std::vector<std::string> row = relation[0];
  EXPECT_EQ(row[0], "movieId");
  EXPECT_EQ(row[1], "title");
  EXPECT_EQ(row[2], "genres");
  
  relation = fs.next();
  row = relation[0];
  std::vector<std::string> expectProperCommaHandling{ "1","\"A Movie Title, With Commas, In the Title\"","Adventure|Animation|Children|Comedy|Fantasy" };
  EXPECT_EQ(expectProperCommaHandling, row);
};

class SelectionTest : public ::testing::Test {
  protected:
    std::map<std::string,int> schema = schema_loader("test_data");
    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>("test_data", schema);
    std::vector<std::string> triple{"title", "EQUALS","The Fall"};
    Selection select{triple, std::move( fs )};
};

TEST_F(SelectionTest, TestNext) {
  // Advance to a row where title = "The Fall"
  std::cout << "BEFORE utest\n";
  select.next();
  std::cout << "AFTER utest\n";
  select.next();
  std::vector<std::vector<std::string> > relation = select.next();
  std::vector<std::string> row = relation[0];

  std::vector<std::string> theFallRow{ "2", "The Fall", "Adventure|Fantasy" };
  EXPECT_EQ(row, theFallRow);
}

class ProjectionTest : public ::testing::Test {
  protected:
    std::map<std::string,int> schema = schema_loader("movies");
    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>("test_data", schema);
    std::vector<std::string> triple{"title", "EQUALS","The Fall"};
    std::unique_ptr<Selection> select = std::make_unique<Selection>(triple, std::move( fs ));
    Projection projection{"title", std::move( select )};
};

TEST_F(ProjectionTest, TestNext) {
  // Advance to a row where title = "The Fall"
  projection.next();
  projection.next();
  std::vector<std::vector<std::string> > relation = projection.next();
  std::vector<std::string> row = relation[0];
  std::vector<std::string> expectation{ "The Fall" };
  EXPECT_EQ(row, expectation);
}

class JoinTest : public ::testing::Test {
  protected:
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

};

TEST_F(JoinTest, TestNext) {
  Join join( std::move( mprojection ), std::move( rprojection ), keys );
  std::vector<std::vector<std::string> > result = join.next();
  result = join.next();
  std::vector<std::string> expectation{ "The Fall", "The Fall" };
  EXPECT_EQ(result[0], expectation);
}

