#ifndef _CFG_H
#define _CFG_H

#include <set>

using namespace std;

typedef struct TreeNode
{
  struct TreeNode* m_leftChild;
  struct TreeNode* m_rightChild;
  struct TreeNode* m_parent;
  int m_minValue;
  int m_maxValue;
  bool checkValidity(int min, int max);
}TreeNode;

typedef struct
{
  struct TreeNode* m_root;
  bool checkValidity();
}State;

typedef struct
{
  string m_line;
}Func; // TODO: change to classes with parameters

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