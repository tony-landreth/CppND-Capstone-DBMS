#ifndef JOIN_H
#define JOIN_H
#include "PlanNode.h"
#include "Projection.h"
#include<string>
#include<vector>
#include<map>
#include "schema.h"

class Join : public PlanNode
{
  public:
    // Method declarations
    /* Following existing conventions, when two tables are joined,
       we refer to the joined tables as R and S respectively
    */
    Join(const std::unique_ptr<Projection> r, const std::unique_ptr<Projection> s, const std::vector<std::string> foreignKeyNames, const Schema rsch, const Schema ssch);
    std::vector<std::string> next();
    void rewind(){};

    // Getters
    Schema getSchema();

    // Variable declarations
    std::vector<std::string> keys;

  private:
    // Method declarations

    // Variable declarations
    Schema schema_;
    std::vector<int> foreignKeys_;
    int rTableSize_;
    int sTableSize_;
    std::unique_ptr<Projection> r_;
    std::unique_ptr<Projection> s_;
    int rowIdx = 0;
    Schema r_schema_;
    Schema s_schema_;
    int requiredRowSize_;
};

#endif
