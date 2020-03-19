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
    Join(const std::unique_ptr<Projection> r, const std::unique_ptr<Projection> s, const std::vector<std::string> k, const TableSchema rsch, const TableSchema ssch);
    std::vector<std::string> next();
    void rewind(){};

    // Variable declarations
    int rTableSize;
    int sTableSize;
    TableSchema schema;
    std::vector<int> foreignKeys;

  private:
    // Method declarations

    // Variable declarations
    std::unique_ptr<Projection> r_;
    std::unique_ptr<Projection> s_;
    std::vector<std::string> keys_;
    int rowIdx = 0;
    TableSchema r_schema_;
    TableSchema s_schema_;
};

#endif
