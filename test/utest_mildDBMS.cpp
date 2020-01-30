#include <iostream>
#include "gtest/gtest.h"
#include "../src/FileScan.h"

class FileScanTest : public ::testing::Test {
  protected:
    //TODO: select a subset of movie.csv as test data
    FileScan fs{"test_data"};
};

TEST_F(FileScanTest, TestNext) {
  std::vector<std::string> row = fs.next();
  EXPECT_EQ(row[0], "movieId");
  EXPECT_EQ(row[1], "title");
  EXPECT_EQ(row[2], "genres");
  
  row = fs.next();
  std::vector<std::string> expectProperCommaHandling{ "1","\"A Movie Title, With Commas, In the Title\"","Adventure|Animation|Children|Comedy|Fantasy" };
  EXPECT_EQ(expectProperCommaHandling, row);

}
