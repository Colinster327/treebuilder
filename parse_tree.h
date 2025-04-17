#include "tree_node.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

// Begin Integer Expression Classes
class integer_expression {
public:
  virtual int evaluate_expression(map<string, TreeNode *> &sym_tab) = 0;
};

class int_constant : public integer_expression {
public:
  int_constant(int val) {
    cout << "int constant" << endl;
    saved_val = val;
  }

  virtual int evaluate_expression(map<string, TreeNode *> &sym_tab) {
    return saved_val;
  }

private:
  int saved_val;
};

class int_plus_expr : public integer_expression {
public:
  int_plus_expr(integer_expression *left, integer_expression *right) {
    cout << "int plus expr" << endl;
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, TreeNode *> &sym_tab) {
    return l->evaluate_expression(sym_tab) + r->evaluate_expression(sym_tab);
  }

private:
  integer_expression *l;
  integer_expression *r;
};
// End Integer Expression Classes

// Begin String Expression Classes
class string_expression {
public:
  virtual string evaluate_expression(map<string, TreeNode *> &sym_tab) = 0;
};

class string_constant : public string_expression {
public:
  string_constant(string val) {
    cout << "string constant" << endl;
    saved_val = val;
  }

  virtual string evaluate_expression(map<string, TreeNode *> &sym_tab) {
    return saved_val;
  }

private:
  string saved_val;
};

class string_plus_expr : public string_expression {
public:
  string_plus_expr(string_expression *left, string_expression *right) {
    cout << "string plus expr" << endl;
    l = left;
    r = right;
  }

  virtual string evaluate_expression(map<string, TreeNode *> &sym_tab) {
    return l->evaluate_expression(sym_tab) + r->evaluate_expression(sym_tab);
  }

private:
  string_expression *l;
  string_expression *r;
};

class string_int_plus_expr : public string_expression {
public:
  string_int_plus_expr(string_expression *left, integer_expression *right) {
    cout << "string int plus expr" << endl;
    ls = left;
    rs = NULL;
    li = NULL;
    ri = right;
  }

  string_int_plus_expr(integer_expression *left, string_expression *right) {
    cout << "int string plus expr" << endl;
    ls = NULL;
    li = left;
    rs = right;
    ri = NULL;
  }

  virtual string evaluate_expression(map<string, TreeNode *> &sym_tab) {
    if (ls) {
      return ls->evaluate_expression(sym_tab) +
             to_string(ri->evaluate_expression(sym_tab));
    } else {
      return to_string(li->evaluate_expression(sym_tab)) +
             rs->evaluate_expression(sym_tab);
    }
  }

private:
  string_expression *ls;
  string_expression *rs;
  integer_expression *li;
  integer_expression *ri;
};
// End String Expression Classes

// Begin Statement Classes
class statement {
public:
  virtual ~statement() {}
  virtual void evaluate_statement(map<string, TreeNode *> &sym_tab) = 0;
};

class compound_statement : public statement {
public:
  compound_statement(statement *first, compound_statement *rest) {
    cout << "compound statement" << endl;
    f = first;
    r = rest;
  }

  virtual void evaluate_statement(map<string, TreeNode *> &sym_tab) {
    if (f != NULL) {
      f->evaluate_statement(sym_tab);
    }
    if (r != NULL) {
      r->evaluate_statement(sym_tab);
    }
  }

private:
  statement *f;
  compound_statement *r;
};

class print_statement : public statement {
public:
  print_statement(string_expression *expr) {
    cout << "print statement" << endl;
    e = expr;
  }

  virtual void evaluate_statement(map<string, TreeNode *> &sym_tab) {
    map<string, TreeNode *>::iterator it;
    it = sym_tab.find(e->evaluate_expression(sym_tab));

    if (it != sym_tab.end()) {
      TreeNode *node = it->second;
      cout << node->name;

      if (node->children.size() > 0) {
        cout << "[";
        print_statement *children_print;
        string_constant *child_name;
        for (size_t i = 0; i < node->children.size(); ++i) {
          child_name = new string_constant(node->children[i]->name);
          children_print = new print_statement(child_name);
          children_print->evaluate_statement(sym_tab);
          cout << (i < node->children.size() - 1 ? ", " : "");
          delete children_print;
          delete child_name;
        }
        cout << "]" << endl;
      }
    } else {
      cout << "Error: Variable not found in symbol table." << endl;
    }
  }

private:
  string_expression *e;
};

class node_statement : public statement {
public:
  node_statement(string_expression *name, integer_expression *weight,
                 string_expression *parent) {
    cout << "node statement" << endl;
    n = name;
    w = weight;
    p = parent;
  }

  node_statement(string_expression *name, integer_expression *weight) {
    cout << "node statement" << endl;
    n = name;
    w = weight;
    p = NULL;
  }

  virtual void evaluate_statement(map<string, TreeNode *> &sym_tab) {
    string node_name = n->evaluate_expression(sym_tab);
    int node_weight = w->evaluate_expression(sym_tab);
    string parent_name;

    if (p) {
      parent_name = p->evaluate_expression(sym_tab);
    }

    if (sym_tab.find(node_name) != sym_tab.end()) {
      cout << "Error: Node already exists in symbol table." << endl;
      return;
    }
    TreeNode *new_node = new TreeNode(node_name, node_weight);
    sym_tab[node_name] = new_node;

    if (!parent_name.empty()) {
      map<string, TreeNode *>::iterator it;
      it = sym_tab.find(parent_name);

      if (it != sym_tab.end()) {
        TreeNode *parent_node = it->second;
        parent_node->children.push_back(new_node);
      } else {
        cout << "Error: Parent node not found in symbol table." << endl;
      }
    }
  }

private:
  string_expression *n;
  integer_expression *w;
  string_expression *p;
};
// End Statement Classes

#endif // PARSE_TREE_H
