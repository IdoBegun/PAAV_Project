#ifndef _CFG_H
#define _CFG_H

#include "TreeNode.h"
#include "State.h"
#include "Function.h"
#include <set>

using namespace std;

enum CfgNodeType {
  e_noneType,
  e_regular,
  e_if,
  e_while
};

typedef struct CfgNode
{
  CfgNode():m_state(), m_nextFunc(), m_children(), m_parents(), type(e_noneType), m_trueChild(NULL), m_falseChild(NULL) {}
  void addChild(CfgNode*& childToAdd);
  void removeChild(CfgNode*& childToRemove);
  State m_state;
  Function m_nextFunc;
  set<struct CfgNode*> m_children;
  set<struct CfgNode*> m_parents;
  CfgNodeType type;
  CfgNode* m_trueChild; //only relevant for if & while nodes
  CfgNode* m_falseChild; //only relevant for if & while nodes
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
  void runProgram(const int& numberOfIterations = 10);
  bool checkValidity();
private:
  CfgGraph createCfg(istream& infile);
  void createRegularNode(CfgNode*& currentNode, CfgGraph& graph, string& line);
  void createWhileGraph(CfgNode*& currentNode, CfgGraph& graph, string& line, istream& infile);
  void createIfGraph(CfgNode*& currentNode, CfgGraph& graph, string& line, istream& infile, CfgNode*& ifNode);
  void createElseGraph(CfgNode*& currentNode, CfgGraph& graph, string& line, istream& infile, CfgNode*& ifNode);
  void updateLeafNode(CfgGraph& graph, CfgGraph& graphToUpdate, CfgNode*& currentNode);
  CfgGraph m_graph;
};

#endif