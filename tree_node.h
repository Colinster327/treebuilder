#include <memory>
#include <string>
#include <vector>

using namespace std;

struct TreeNode {
  string name;
  int weight;
  vector<TreeNode *> children;

  TreeNode(const string name, int weight) : name(name), weight(weight) {}
  ~TreeNode() {
    for (auto child : children) {
      delete child;
    }
  }
};
