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
    Join(const std::unique_ptr<Projection> r, const std::unique_ptr<Projection> s, const std::vector<std::string> keys);
    std::vector<std::string> next();
    int rTableSize;
    int sTableSize;
    // Joining table columns affects the number of columns in the in-memory table representation
    // which undermines the schema used as table read time. jSchema is used to produce a sort of virtual
    // schema to represent the ordering of the joined data in memory.
    std::map<std::string,int> jSchema;

  private:
    std::unique_ptr<Projection> r_;
    std::unique_ptr<Projection> s_;
    std::vector<std::string> keys_;
    int rowIdx = 0;
};

#endif
