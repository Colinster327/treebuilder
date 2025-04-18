#include <memory>
#include <string>
#include <vector>

using namespace std;

#ifndef TREE_NODE_H
#define TREE_NODE_H

struct TreeNode {
  string name;
  int weight;
  vector<shared_ptr<TreeNode>> children;

  TreeNode(const string name, int weight) : name(name), weight(weight) {
    children.clear();
  }
};

#endif // TREE_NODE_H
