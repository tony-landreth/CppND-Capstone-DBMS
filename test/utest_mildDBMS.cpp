#include <iostream>
#include "gtest/gtest.h"
#include "../src/FileScan.h"

class FileScanTest : public ::testing::Test {
  protected:
    //TODO: select a subset of movie.csv as test data
    FileScan fs{"movies"};
};

TEST_F(FileScanTest, TestNext) {
  EXPECT_EQ(fs.next(),"movieId,title,genres\r");
}
