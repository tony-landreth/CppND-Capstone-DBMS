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
    std::vector<std::vector<std::string> > next();

  private:
    std::unique_ptr<Projection> r_;
    std::unique_ptr<Projection> s_;
    std::vector<std::string> keys_;
};

#endif
