#include <iostream>
#include "gtest/gtest.h"
#include "../src/FileScan.h"

class FileScanTest : public ::testing::Test {
  protected:
    //TODO: select a subset of movie.csv as test data
    FileScan fs{"movies"};
};

TEST_F(FileScanTest, TestNext) {
  std::vector<std::string> row = fs.next();
  EXPECT_EQ(row[0], "movieId");
  EXPECT_EQ(row[1], "title");
  EXPECT_EQ(row[2], "genres");
}
