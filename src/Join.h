#ifndef JOIN_H
#define JOIN_H
#include "PlanNode.h"
#include "Projection.h"
#include<string>
#include<vector>
#include<map>
#include "schema_loader.h"

class Join : public PlanNode
{
  public:
    // Method declarations
    /* Following existing conventions, when two tables are joined,
       we refer to the joined tables as R and S respectively
    */
    Join(const std::unique_ptr<Projection> r, const std::unique_ptr<Projection> s, const std::vector<std::string> foreignKeyNames, const TableSchema rsch, const TableSchema ssch);
    std::vector<std::string> next();
    void rewind(){};

    // Getters
    TableSchema getSchema();

    // Variable declarations
    TableSchema schema;
    std::vector<int> foreignKeys;
    std::vector<std::string> keys;

  private:
    // Method declarations

    // Variable declarations
    int rTableSize_;
    int sTableSize_;
    std::unique_ptr<Projection> r_;
    std::unique_ptr<Projection> s_;
    int rowIdx = 0;
    TableSchema r_schema_;
    TableSchema s_schema_;
};

#endif
