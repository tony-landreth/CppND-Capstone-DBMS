#include <iostream>
#include "gtest/gtest.h"
#include "../src/FileScan.h"
#include "../src/Selection.h"
#include "../src/Projection.h"
#include "../src/Join.h"
#include "../src/QueryPlanner.h"
#include "../src/schema.h"

// Tests for FileScanTest
//
//  FileScan nodes scan data from disk and store it in memory.
//  Each table in the database is its own file on disk.
//  One FileScan node opens one file.
//
class FileScanTest : public ::testing::Test {
  protected:
    Schema tblSchema = schema_loader("test_data");
    FileScan fs{ tblSchema };
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
// Select nodes use WHERE clauses to filter table data by rows
class SelectionTest : public ::testing::Test {
  protected:
    Schema schema = schema_loader("test_data");
    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>(schema);
    std::vector<std::string> where{"title", "EQUALS","The Fall"};
};

TEST_F(SelectionTest, TestNext) {
  fs->scanFile();
  Selection select{ where, std::move( fs ), schema };
  // Advance to a row where title = "The Fall"
  select.next();
  select.next();
  std::vector<std::string> row = select.next();

  std::vector<std::string> theFallRow{ "2", "The Fall", "Adventure|Fantasy" };
  EXPECT_EQ(row, theFallRow);
}

// When a `SELECT * FROM table` command is issued, there should be no WHERE clause.
// Ideally the QueryPlanner would skip instantiating a SELECT node in this case.
// Until my QueryPlanner is more ideal, any empty WHERE clause admits all rows.
class StarTest : public ::testing::Test {
  protected:
    Schema schema = schema_loader("test_data");
    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>(schema);
    std::vector<std::string> where{};
};

TEST_F(StarTest, TestSelectStarNext) {
  fs->scanFile();
  Selection select{ where, std::move( fs ), schema };

  // All rows in test_data will be returned
  std::vector<std::vector<std::string> > expectedResult{ 
    { "movieId", "title", "genres" },
    { "1", "\"A Movie Title, With Commas, In the Title\"", "Adventure|Animation|Children|Comedy|Fantasy" },
    { "2", "The Fall", "Adventure|Fantasy" },
    { "3", "Jumanji (1995)", "Adventure|Children|Fantasy" }
  };

  std::vector<std::vector<std::string> > results;
  std::vector<std::string> row;

  for(int i = 0; i < schema.tableSize; i++) {
    row = select.next();
    results.push_back(row);
  }

  EXPECT_EQ(results, expectedResult);
}

// Tests for Projection
// Projection nodes filter all columns but those specified in a SELECT statement.
// SELECT title FROM test_data entails that only the title column from each row be returned.
class ProjectionTest : public ::testing::Test {
  protected:
    Schema schema = schema_loader("test_data");
    std::unique_ptr<FileScan> fs = std::make_unique<FileScan>(schema);
    std::vector<std::string> col_names{ "title", "genres" };
    std::vector<std::string> where;
};

TEST_F(ProjectionTest, TestNext) {
  fs->scanFile();
  std::unique_ptr<Selection> select = std::make_unique<Selection>( where, std::move( fs ), schema );
  Projection projection{col_names, std::move( select ), schema };

  // Advance to a row where title = "The Fall"
  projection.next();
  projection.next();
  std::vector<std::string> row = projection.next();
  std::vector<std::string> expectation{ "The Fall", "Adventure|Fantasy" };
  EXPECT_EQ(row, expectation);
}

// Rewind makes it possible to run JOINs using a naive, inefficient strategy, which
// I will hopefully swap out at some point.
TEST_F(ProjectionTest, Rewind) {
  fs->scanFile();
  std::unique_ptr<Selection> select = std::make_unique<Selection>( where, std::move( fs ), schema );
  Projection projection{col_names, std::move( select ), schema};

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
// Join nodes make it possible to combine data from a pair of tables, call them R and S, into a single query.
// Join nodes relate the data in one table to the data in another table, through a foreignKey.
// Where foreignKeys match, rows get returned, according to a schema that combines columns from
// R and S tables.
class JoinTest : public ::testing::Test {
  protected:
    Schema schema = schema_loader("test_data");
    std::unique_ptr<FileScan> tableR = std::make_unique<FileScan>(schema);
    std::unique_ptr<FileScan> tableS = std::make_unique<FileScan>(schema);

    std::vector<std::string> where{"title", "EQUALS","The Fall"};
    std::vector<std::string> col_names{  "movieId", "title", };
    std::vector<std::string> keys{ "movieId", "movieId" };
    Schema tblSchema = schema_loader("test_data");
};

TEST_F(JoinTest, TestNext) {
  tableR->scanFile();
  tableS->scanFile();
  std::unique_ptr<Selection> mselect = std::make_unique<Selection>( where, std::move( tableR ), schema);
  std::unique_ptr<Selection> rselect = std::make_unique<Selection>( where, std::move( tableS ), schema);
  std::unique_ptr<Projection> mprojection = std::make_unique<Projection>(col_names, std::move( mselect ), schema);
  std::unique_ptr<Projection> rprojection = std::make_unique<Projection>(col_names, std::move( rselect ), schema);

  Join join( std::move( mprojection ), std::move( rprojection ), keys, tblSchema, schema);
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
// The Tokenizer analyzes user input into semantically meaningful units
// so that the QueryPlanner can understand how to build an appropriate 
// pipeline of query plan nodes.
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
// The QueryPlanner reads in a TokenTree, generates an intermediate representation
// and composes a pipeline of query PlanNodes to return requested data.
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

// A self join occurs when the two contributing nodes to the join are read from the
// same table, i.e. a table is joined on itself.
class SelfJoinQueryTest : public ::testing::Test {
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

TEST_F(SelfJoinQueryTest, Run) {
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
