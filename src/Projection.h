#ifndef PROJECTION_H
#define PROJECTION_H
#include "Selection.h"
#include<string>
#include<map>


/*  Projection nodes filter all columns but those specified in a SELECT statement, e.g.
    `SELECT title FROM test_data;` entails that only the title column from each row be returned.
    Projection nodes include a rewind() method for use by Join nodes.
*/

class Projection : public PlanNode
{
  public:
    // Method declarations
    Projection(const std::vector<std::string> keys, const std::unique_ptr<PlanNode> sel, const Schema sch);
    std::vector<std::string> next();
    void rewind();

    // Variable declarations
    std::vector<std::string> keys;

  private:
    // Method declarations

    // Variable declarations
    Schema schema_;
    std::unique_ptr<PlanNode> sel_;
    int rowIdx_ = 0;
    std::vector<int> colKeys;
};

#endif
