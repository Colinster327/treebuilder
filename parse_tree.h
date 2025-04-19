/*
 * parse_tree.h
 * Authors: Colin Murphy, Brock Klamfoth
 * Date: 2024-04-17
 * Description: This file contains the parse tree for a tree builder language.
 */

#include "tree_node.h"
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

using namespace std;

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

// Begin Expression Classes
class expression {
public:
  virtual variant<int, string>
  evaluate_expression(map<string, shared_ptr<TreeNode>> &tree_tab,
                      map<string, variant<int, string>> &var_tab) = 0;
};

class constant : public expression {
public:
  constant(variant<int, string> val) { saved_val = val; }

  virtual variant<int, string>
  evaluate_expression(map<string, shared_ptr<TreeNode>> &sym_tab,
                      map<string, variant<int, string>> &var_tab) {
    return saved_val;
  }

private:
  variant<int, string> saved_val;
};

class variable : public expression {
public:
  variable(string name) { saved_name = name; }

  virtual variant<int, string>
  evaluate_expression(map<string, shared_ptr<TreeNode>> &sym_tab,
                      map<string, variant<int, string>> &var_tab) {
    auto it = var_tab.find(saved_name);
    if (it != var_tab.end()) {
      return it->second;
    } else {
      cout << "Error: Variable not found in symbol table." << endl;
      return {};
    }
  }

private:
  string saved_name;
};

class plus_expr : public expression {
public:
  plus_expr(expression *left, expression *right) {
    l = left;
    r = right;
  }

  virtual variant<int, string>
  evaluate_expression(map<string, shared_ptr<TreeNode>> &sym_tab,
                      map<string, variant<int, string>> &var_tab) {
    auto left_val = l->evaluate_expression(sym_tab, var_tab);
    auto right_val = r->evaluate_expression(sym_tab, var_tab);

    if (holds_alternative<int>(left_val) && holds_alternative<int>(right_val)) {
      return get<int>(left_val) + get<int>(right_val);
    } else if (holds_alternative<string>(left_val) &&
               holds_alternative<string>(right_val)) {
      return get<string>(left_val) + get<string>(right_val);
    } else if (holds_alternative<int>(left_val) &&
               holds_alternative<string>(right_val)) {
      return to_string(get<int>(left_val)) + get<string>(right_val);
    } else if (holds_alternative<string>(left_val) &&
               holds_alternative<int>(right_val)) {
      return get<string>(left_val) + to_string(get<int>(right_val));
    } else {
      cout << "Error: Invalid types for addition." << endl;
      return {};
    }
  }

private:
  expression *l;
  expression *r;
};

class minus_expr : public expression {
public:
  minus_expr(expression *left, expression *right) {
    l = left;
    r = right;
  }

  virtual variant<int, string>
  evaluate_expression(map<string, shared_ptr<TreeNode>> &sym_tab,
                      map<string, variant<int, string>> &var_tab) {
    auto left_val = l->evaluate_expression(sym_tab, var_tab);
    auto right_val = r->evaluate_expression(sym_tab, var_tab);
    if (holds_alternative<int>(left_val) && holds_alternative<int>(right_val)) {
      return get<int>(left_val) - get<int>(right_val);
    } else {
      cout << "Error: Invalid types for subtraction." << endl;
      return {};
    }
  }

private:
  expression *l;
  expression *r;
};

class times_expr : public expression {
public:
  times_expr(expression *left, expression *right) {
    l = left;
    r = right;
  }
  virtual variant<int, string>
  evaluate_expression(map<string, shared_ptr<TreeNode>> &sym_tab,
                      map<string, variant<int, string>> &var_tab) {
    auto left_val = l->evaluate_expression(sym_tab, var_tab);
    auto right_val = r->evaluate_expression(sym_tab, var_tab);
    if (holds_alternative<int>(left_val) && holds_alternative<int>(right_val)) {
      return get<int>(left_val) * get<int>(right_val);
    } else {
      cout << "Error: Invalid types for multiplication." << endl;
      return {};
    }
  }

private:
  expression *l;
  expression *r;
};

class divide_expr : public expression {
public:
  divide_expr(expression *left, expression *right) {
    l = left;
    r = right;
  }
  virtual variant<int, string>
  evaluate_expression(map<string, shared_ptr<TreeNode>> &sym_tab,
                      map<string, variant<int, string>> &var_tab) {
    auto left_val = l->evaluate_expression(sym_tab, var_tab);
    auto right_val = r->evaluate_expression(sym_tab, var_tab);
    if (holds_alternative<int>(left_val) && holds_alternative<int>(right_val)) {
      int denominator = get<int>(right_val);
      if (denominator == 0) {
        cout << "Error: Division by 0." << endl;
        return {};
      }
      return get<int>(left_val) / get<int>(right_val);
    } else {
      cout << "Error: Invalid types for division." << endl;
      return {};
    }
  }

private:
  expression *l;
  expression *r;
};

class mod_expr : public expression {
public:
  mod_expr(expression *left, expression *right) {
    l = left;
    r = right;
  }
  virtual variant<int, string>
  evaluate_expression(map<string, shared_ptr<TreeNode>> &sym_tab,
                      map<string, variant<int, string>> &var_tab) {
    auto left_val = l->evaluate_expression(sym_tab, var_tab);
    auto right_val = r->evaluate_expression(sym_tab, var_tab);
    if (holds_alternative<int>(left_val) && holds_alternative<int>(right_val)) {
      return get<int>(left_val) % get<int>(right_val);
    } else {
      cout << "Error: Invalid types for modulo division." << endl;
      return {};
    }
  }

private:
  expression *l;
  expression *r;
};

class negate_expr : public expression {
public:
  negate_expr(expression *expr) { e = expr; }

  virtual variant<int, string>
  evaluate_expression(map<string, shared_ptr<TreeNode>> &sym_tab,
                      map<string, variant<int, string>> &var_tab) {
    auto val = e->evaluate_expression(sym_tab, var_tab);
    if (holds_alternative<int>(val)) {
      return -get<int>(val);
    } else {
      cout << "Error: Invalid type for negation." << endl;
      return {};
    }
  }

private:
  expression *e;
};
// End Expression Classes

// Begin Statement Classes
class statement {
public:
  virtual void
  evaluate_statement(map<string, shared_ptr<TreeNode>> &tree_tab,
                     map<string, variant<int, string>> &var_tab) = 0;
};

class compound_statement : public statement {
public:
  compound_statement(statement *first, compound_statement *rest) {
    f = first;
    r = rest;
  }

  virtual void evaluate_statement(map<string, shared_ptr<TreeNode>> &sym_tab,
                                  map<string, variant<int, string>> &var_tab) {
    if (f != NULL) {
      f->evaluate_statement(sym_tab, var_tab);
    }
    if (r != NULL) {
      r->evaluate_statement(sym_tab, var_tab);
    }
  }

private:
  statement *f;
  compound_statement *r;
};

class print_statement : public statement {
public:
  print_statement(expression *expr) { e = expr; }

  virtual void evaluate_statement(map<string, shared_ptr<TreeNode>> &sym_tab,
                                  map<string, variant<int, string>> &var_tab) {
    auto val = e->evaluate_expression(sym_tab, var_tab);
    if (holds_alternative<int>(val)) {
      cout << "Error: Cannot print integer value." << endl;
      return;
    }
    map<string, shared_ptr<TreeNode>>::iterator it;
    it = sym_tab.find(get<string>(val));

    if (it != sym_tab.end()) {
      this->traverse_tree(it->second);
    } else {
      cout << "Error: Tree node not found." << endl;
    }
    cout << endl;
  }

private:
  expression *e;

  void traverse_tree(shared_ptr<TreeNode> node) {
    if (node == NULL) {
      return;
    }

    cout << node->name;

    if (node->children.size() > 0) {
      cout << "[";
      for (size_t i = 0; i < node->children.size(); ++i) {
        traverse_tree(node->children[i]);
        if (i < node->children.size() - 1) {
          cout << ", ";
        }
      }
      cout << "]";
    }
  }
};

class node_statement : public statement {
public:
  node_statement(expression *name, expression *weight, expression *parent) {
    n = name;
    w = weight;
    p = parent;
  }

  node_statement(expression *name, expression *weight) {
    n = name;
    w = weight;
    p = NULL;
  }

  virtual void evaluate_statement(map<string, shared_ptr<TreeNode>> &sym_tab,
                                  map<string, variant<int, string>> &var_tab) {
    auto name_v = n->evaluate_expression(sym_tab, var_tab);
    auto weight_v = w->evaluate_expression(sym_tab, var_tab);
    variant<int, string> parent_v;

    if (holds_alternative<int>(name_v)) {
      cout << "Error: Node name cannot be an integer." << endl;
      return;
    }

    if (holds_alternative<string>(weight_v)) {
      cout << "Error: Node weight cannot be a string." << endl;
      return;
    }

    if (p) {
      parent_v = p->evaluate_expression(sym_tab, var_tab);

      if (holds_alternative<int>(parent_v)) {
        cout << "Error: Parent name cannot be an integer." << endl;
        return;
      }
    }

    string node_name = get<string>(name_v);
    int node_weight = get<int>(weight_v);

    if (sym_tab.find(node_name) != sym_tab.end()) {
      cout << "Error: Node already exists in symbol table." << endl;
      return;
    }

    auto new_node = make_shared<TreeNode>(node_name, node_weight);
    sym_tab[node_name] = new_node;

    if (p) {
      string parent_name = get<string>(parent_v);
      map<string, shared_ptr<TreeNode>>::iterator it;
      it = sym_tab.find(parent_name);

      if (it != sym_tab.end()) {
        auto parent_node = it->second;
        parent_node->children.push_back(new_node);
      } else {
        cout << "Error: Parent node not found in symbol table." << endl;
      }
    }
  }

private:
  expression *n;
  expression *w;
  expression *p;
};

class for_statement : public statement {
public:
  for_statement(string var, expression *start, expression *end,
                compound_statement *stmt) {
    v = var;
    s = start;
    e = end;
    body = stmt;
    l = NULL;
  }

  for_statement(string var, vector<expression *> *list,
                compound_statement *stmt) {
    v = var;
    l = list;
    body = stmt;
    s = NULL;
    e = NULL;
  }

  virtual void evaluate_statement(map<string, shared_ptr<TreeNode>> &tree_tab,
                                  map<string, variant<int, string>> &var_tab) {
    if (s && e) {
      auto start_val = s->evaluate_expression(tree_tab, var_tab);
      auto end_val = e->evaluate_expression(tree_tab, var_tab);

      if (holds_alternative<int>(start_val) &&
          holds_alternative<int>(end_val)) {
        // Create a new variable table for the loop to ensure scope
        map<string, variant<int, string>> new_var_tab = var_tab;

        for (int i = get<int>(start_val); i <= get<int>(end_val); ++i) {
          new_var_tab[this->v] = i;
          body->evaluate_statement(tree_tab, new_var_tab);
        }
      } else {
        cout << "Error: Start and end values must be integers." << endl;
      }
    } else if (l) {
      // Create a new variable table for the loop to ensure scope
      map<string, variant<int, string>> new_var_tab = var_tab;

      for (auto &item : *l) {
        auto item_val = item->evaluate_expression(tree_tab, var_tab);
        if (holds_alternative<string>(item_val)) {
          new_var_tab[this->v] = get<string>(item_val);
          body->evaluate_statement(tree_tab, new_var_tab);
        } else {
          cout << "Error: List items must be strings." << endl;
        }
      }
    } else {
      cout << "Error: Invalid for statement." << endl;
    }
  }

private:
  string v;
  expression *s;
  expression *e;
  compound_statement *body;
  vector<expression *> *l;
};
// End Statement Classes

#endif // PARSE_TREE_H
