#include <iostream>
#include "gtest/gtest.h"
#include "../src/FileScan.h"
#include "../src/Selection.h"
#include "../src/Projection.h"
#include "../src/Join.h"
#include "../src/QueryPlanner.h"
#include "../src/schema_loader.h"

// Tests for FileScanTest

class FileScanTest : public ::testing::Test {
  protected:
    FileScan fs{"test_data"};
};

TEST_F(FileScanTest, TestNext) {
  fs.scanFile();
  std::vector<std::string> row = fs.next();
  EXPECT_EQ(row[0], "movieId");
  EXPECT_EQ(row[1], "title");
  EXPECT_EQ(row[2], "genres");
  
  row = fs.next();
  std::vector<std::string> expectProperCommaHandling{ "1","\"A Movie Title, With Commas, In the Title\"","Adventure|Animation|Children|Comedy|Fantasy" };
  EXPECT_EQ(expectProperCommaHandling, row);
};

// Tests for SELECT

class SelectionTest : public ::testing::Test {
  protected:
    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>("test_data");
    std::vector<std::string> where{"title", "EQUALS","The Fall"};
};

TEST_F(SelectionTest, TestNext) {
  fs->scanFile();
  TableSchema sch = fs->schema;
  Selection select{ where, std::move( fs ), sch };
  // Advance to a row where title = "The Fall"
  select.next();
  select.next();
  std::vector<std::string> row = select.next();

  std::vector<std::string> theFallRow{ "2", "The Fall", "Adventure|Fantasy" };
  EXPECT_EQ(row, theFallRow);
}

class StarTest : public ::testing::Test {
  protected:
    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>("test_data");
    std::vector<std::string> where{};
};

TEST_F(StarTest, TestSelectStarNext) {
  fs->scanFile();
  TableSchema sch = fs->schema;
  Selection select{ where, std::move( fs ), sch };

  std::vector<std::vector<std::string> > expectedResult{ 
    { "movieId", "title", "genres" },
    { "1", "\"A Movie Title, With Commas, In the Title\"", "Adventure|Animation|Children|Comedy|Fantasy" },
    { "2", "The Fall", "Adventure|Fantasy" },
    { "3", "Jumanji (1995)", "Adventure|Children|Fantasy" }
  };

  std::vector<std::vector<std::string> > results;
  std::vector<std::string> row = select.next();

  while(row.size() > 0) {
    results.push_back(row);
    row = select.next();
  }

  EXPECT_EQ(results, expectedResult);
}

// Tests for Projection
class ProjectionTest : public ::testing::Test {
  protected:
    TableSchema tblSchema = schema_loader("test_data");
    std::map<std::string,int> schema = tblSchema.columnKeys;
    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>("test_data");
    std::vector<std::string> col_names{ "title", "genres" };
    std::vector<std::string> where;
};

TEST_F(ProjectionTest, TestNext) {
  fs->scanFile();
  std::unique_ptr<Selection> select = std::make_unique<Selection>( where, std::move( fs ), tblSchema );
  Projection projection{col_names, std::move( select ), tblSchema };

  // Advance to a row where title = "The Fall"
  projection.next();
  projection.next();
  std::vector<std::string> row = projection.next();
  std::vector<std::string> expectation{ "The Fall", "Adventure|Fantasy" };
  EXPECT_EQ(row, expectation);
}

TEST_F(ProjectionTest, Rewind) {
  fs->scanFile();
  TableSchema sch = fs->schema;
  std::unique_ptr<Selection> select = std::make_unique<Selection>( where, std::move( fs ), tblSchema );
  Projection projection{col_names, std::move( select ), tblSchema};

  // Advance to a row where title = "The Fall"
  projection.next();
  projection.next();
  std::vector<std::string> row = projection.next();
  std::vector<std::string> expectation{ "The Fall", "Adventure|Fantasy" };
  EXPECT_EQ(row, expectation);

  // Rewind to the first row of the table
  projection.rewind();

  // Advance to a row where title = "The Fall"
  row = projection.next();
  row = projection.next();
  row = projection.next();
  EXPECT_EQ(row, expectation);
}

// Tests for Join
class JoinTest : public ::testing::Test {
  protected:
    std::vector<std::string> where{"title", "EQUALS","The Fall"};
    std::vector<std::string> col_names{  "movieId", "title", };
    std::vector<std::string> keys{ "movieId", "movieId" };
    TableSchema tblSchema = schema_loader("test_data");
};

TEST_F(JoinTest, TestNext) {
  std::unique_ptr<FileScan> mfs = std::make_unique<FileScan>("test_data");
  mfs->scanFile();
  TableSchema mfsSchema = mfs->schema;
  std::unique_ptr<FileScan> rfs = std::make_unique<FileScan>("test_data");
  TableSchema rfsSchema = rfs->schema;
  rfs->scanFile();
  std::unique_ptr<Selection> mselect = std::make_unique<Selection>( where, std::move( mfs ), tblSchema);
  std::unique_ptr<Selection> rselect = std::make_unique<Selection>( where, std::move( rfs ), tblSchema);
  std::unique_ptr<Projection> mprojection = std::make_unique<Projection>(col_names, std::move( mselect ), tblSchema);
  std::unique_ptr<Projection> rprojection = std::make_unique<Projection>(col_names, std::move( rselect ), tblSchema);

  Join join( std::move( mprojection ), std::move( rprojection ), keys );
  std::vector<std::vector<std::string> > result;

  int tableSize = 5;
  for(int i = 0; i < tableSize; i++){
    std::vector<std::string> r = join.next();

    if(r.size() > 0) {
      result.push_back(r);
    }
  }
  std::vector<std::vector<std::string> > expectation{ { "movieId", "title", "movieId", "title" }, { "2", "The Fall", "2", "The Fall" } };
  EXPECT_EQ(result, expectation);
}

// Tests for Tokenizer
class TokenizerTest : public ::testing::Test {
  protected:
    Tokenizer t;
    std::string query = "SELECT title FROM test_data;";
};

TEST_F(TokenizerTest, Tokenize) {
  TokenTree tt = t.tokenize(query);
  std::vector<std::string> expectedResult{ "ROOT TOKEN", "SELECT", "title", "FROM", "test_data" };
  std::vector<std::string> result;
  result = tt.depthFirstSearch(&result);

  EXPECT_EQ(result, expectedResult);
}

class TokenizerWithWhereTest : public ::testing::Test {
  protected:
    Tokenizer t;
    std::string query = "SELECT title FROM test_data WHERE title EQUALS 'The Fall';";
};

TEST_F(TokenizerWithWhereTest, Tokenize) {
  TokenTree tt = t.tokenize(query);
  std::vector<std::string> expectedResult{ "ROOT TOKEN", "SELECT", "title", "FROM", "test_data", "WHERE", "title", "EQUALS", "The Fall" };
  std::vector<std::string> result;

  result = tt.depthFirstSearch(&result);
  EXPECT_EQ(result, expectedResult);
}

class TokenizerWithJoinTest : public ::testing::Test {
  protected:
    Tokenizer t;
    std::string query = "SELECT * FROM movies JOIN ratings ON movieId = movieId;";
};

TEST_F(TokenizerWithJoinTest, Tokenize) {
  TokenTree tt = t.tokenize(query);
  std::vector<std::string> expectedResult{ "ROOT TOKEN", "SELECT", "*", "FROM", "movies", "JOIN", "ratings", "ON", "movieId", "=", "movieId" };
  std::vector<std::string> result;

  result = tt.depthFirstSearch(&result);
  EXPECT_EQ(result, expectedResult);
}

class TokenizerWithWhereJoinTest : public ::testing::Test {
  protected:
    Tokenizer t;
    std::string query = "SELECT * FROM movies JOIN ratings ON movieId = movieId WHERE title EQUALS 'The Fall';";
};

TEST_F(TokenizerWithWhereJoinTest, Tokenize) {
  TokenTree tt = t.tokenize(query);
  std::vector<std::string> expectedResult{ "ROOT TOKEN", "SELECT", "*", "FROM", "movies", "JOIN", "ratings", "ON", "movieId", "=", "movieId", "WHERE", "title", "EQUALS", "The Fall" };
  std::vector<std::string> result;

  result = tt.depthFirstSearch(&result);
  EXPECT_EQ(result, expectedResult);
}

// Tests for QueryPlanner

class QueryPlannerTest : public ::testing::Test {
  protected:
    std::vector<std::string> arguments = {"./mildDBMS", "SELECT * FROM test_data;"};
    std::vector<char*> argv;
};
TEST_F(QueryPlannerTest, Run) {
  std::vector<std::vector<std::string> > expectedResult{ 
    { "movieId", "title", "genres" },
    { "1", "\"A Movie Title, With Commas, In the Title\"", "Adventure|Animation|Children|Comedy|Fantasy" },
    { "2", "The Fall", "Adventure|Fantasy" },
    { "3", "Jumanji (1995)", "Adventure|Children|Fantasy" } 
  };

  for (const auto& arg : arguments)
      argv.push_back((char*)arg.data());

  argv.push_back(nullptr);

  QueryPlanner qp(argv.size() - 1, argv.data());
  std::vector<std::vector<std::string> > result = qp.run();
  EXPECT_EQ(result, expectedResult);
}

class QueryWithProjectionTest : public ::testing::Test {
  protected:
    std::vector<std::string> arguments = {"./mildDBMS", "SELECT title FROM test_data;"};
    std::vector<char*> argv;
    std::vector<std::string> row;
};

TEST_F(QueryWithProjectionTest, Run) {
  std::vector<std::vector<std::string> > expectedResult{ 
    { "title" },
    { "\"A Movie Title, With Commas, In the Title\"" },
    { "The Fall"},
    { "Jumanji (1995)" }
  };

  for (const auto& arg : arguments)
    argv.push_back((char*)arg.data());

  argv.push_back(nullptr);

  QueryPlanner qp(argv.size() - 1, argv.data());
  std::vector<std::vector<std::string> > result = qp.run();

  EXPECT_EQ(result, expectedResult);
}

class ComplexQueryTest : public ::testing::Test {
  protected:
    std::vector<std::string> arguments = {"./mildDBMS", "SELECT title FROM test_data WHERE title EQUALS 'The Fall';"};
    std::vector<char*> argv;
    std::vector<std::string> row;
};

TEST_F(ComplexQueryTest, Run) {
  for (const auto& arg : arguments)
      argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  QueryPlanner qp(argv.size() - 1, argv.data());
  std::vector<std::vector<std::string> > expectedResult{ { "title" }, { "The Fall" } };
  std::vector<std::vector<std::string> > result = qp.run();

  EXPECT_EQ(result, expectedResult);
}

class JoinQueryTest : public ::testing::Test {
  protected:
    std::vector<std::string> arguments = {"./mildDBMS", "SELECT * FROM test_data JOIN test_data ON movieId = movieId;"};
    std::vector<char*> argv;
    std::vector<std::string> row;
    std::vector<std::vector<std::string> >expectedResult{
      { "movieId", "title", "genres", "movieId", "title", "genres" },
      { "1", "\"A Movie Title, With Commas, In the Title\"", "Adventure|Animation|Children|Comedy|Fantasy", "1", "\"A Movie Title, With Commas, In the Title\"", "Adventure|Animation|Children|Comedy|Fantasy" },
      { "2", "The Fall", "Adventure|Fantasy", "2", "The Fall", "Adventure|Fantasy" },
      { "3", "Jumanji (1995)", "Adventure|Children|Fantasy", "3", "Jumanji (1995)", "Adventure|Children|Fantasy" }
    };
};

TEST_F(JoinQueryTest, Run) {
  for (const auto& arg : arguments)
      argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  QueryPlanner qp(argv.size() - 1, argv.data());
  std::vector<std::vector<std::string> > result = qp.run();

  EXPECT_EQ(result, expectedResult);
}

class SelfJoinProjectionWhereQueryTest : public ::testing::Test {
  protected:
    std::vector<std::string> arguments = {"./mildDBMS", "SELECT title FROM movies JOIN movies ON movieId = movieId WHERE title EQUALS 'Sudden Death (1995)';"};
    std::vector<char*> argv;
    std::vector<std::string> row;
    std::vector<std::vector<std::string> >expectedResult{
      { "title", "title" },
      { "Sudden Death (1995)", "Sudden Death (1995)" }
    };
};

TEST_F(SelfJoinProjectionWhereQueryTest, Run) {
  for (const auto& arg : arguments)
      argv.push_back((char*)arg.data());
  argv.push_back(nullptr);

  QueryPlanner qp(argv.size() - 1, argv.data());
  std::vector<std::vector<std::string> > result = qp.run();

  EXPECT_EQ(result, expectedResult);
}
