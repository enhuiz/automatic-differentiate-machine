#ifndef GRAPH_H
#define GRAPH_H

#include <memory>
#include <vector>

#include "node.h"

using SymbolTable = std::map<std::string, std::weak_ptr<VarNode>>;

class Graph
{
public:
  Graph(std::shared_ptr<Node> root,
        SymbolTable &&table)
      : root_(root), table_(table) {}

  void Set(std::string var, double value);
  double Evaluate();
  double Gradient(std::string var);

  std::vector<std::string> GetVariables() const;
  bool VariableExist(std::string var) const;

private:
  std::shared_ptr<Node> root_;
  SymbolTable table_;

  bool forward_dirty = true;
  bool backward_dirty = true;
};

#endif
