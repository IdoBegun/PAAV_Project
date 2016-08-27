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
  void addChild(CfgNode*& childToAdd);
  void removeChild(CfgNode*& childToRemove);
  State m_state;
  Func m_nextFunc;
  set<struct CfgNode*> m_children;
  set<struct CfgNode*> m_parents;
}CfgNode;

typedef struct CfgGraph
{
  void deleteNode(CfgNode* nodeToDelete);
  struct CfgNode* m_root;
  struct CfgNode* m_leaf;
  set<struct CfgNode*> m_nodes;
}CfgGraph;

class Cfg
{
public:
  Cfg(istream& infile);
  ~Cfg() {};
  void deleteCfg();
  void runProgram() {};
  bool checkValidity();
private:
  CfgGraph createCfg(istream& infile);
  void createRegularNode(CfgNode*& currentNode, CfgGraph& graph, string& line);
  void createWhileGraph(CfgNode*& currentNode, CfgGraph& graph, string& line, istream& infile);
  void createIfGraph(CfgNode*& currentNode, CfgGraph& graph, string& line, istream& infile, CfgNode*& ifNode);
  void createElseGraph(CfgNode*& currentNode, CfgGraph& graph, string& line, istream& infile, CfgNode*& ifNode);
  CfgGraph m_graph;
};

#endif