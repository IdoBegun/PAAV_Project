#ifndef _CFG_H
#define _CFG_H

#include "TreeNode.h"
#include "State.h"
#include <set>

using namespace std;

typedef struct
{
  string m_line;
} Func; // TODO: change to classes with parameters

typedef struct CfgNode
{
  State m_state;
  Func m_nextFunc;
  set<struct CfgNode*> m_children;
  set<struct CfgNode*> m_parents;
}CfgNode;

typedef struct CfgTree
{
  struct CfgNode* m_root;
  struct CfgNode* m_leaf;
  set<struct CfgNode*> m_nodes;
}CfgTree;

class Cfg
{
public:
  Cfg(istream& infile);
  ~Cfg();
  void runProgram() {};
  bool checkValidity();
private:
  CfgTree* createCfg(istream& infile);
  CfgTree* m_tree;
};

#endif