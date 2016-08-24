#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>      // std::filebuf
#include <sstream>
#include <string>
#include <algorithm>
#include <new>
#include <assert.h> 
#include <limits.h>
#include "Cfg.h"

Cfg::Cfg(istream& infile)
{
  m_tree = createCfg(infile);
}

Cfg::~Cfg()
{
  for (set<struct CfgNode*>::iterator iter = m_tree->m_nodes.begin(); 
    iter != m_tree->m_nodes.end();)
  {
    CfgNode* currentNode = *iter;
    iter++;
    delete(currentNode);
  }
}

CfgTree* Cfg::createCfg(istream& infile)
{
  CfgTree* tree = new CfgTree;
  CfgNode* currentNode = new CfgNode;
  tree->m_root = tree->m_leaf = currentNode;
  string line;
  bool scopeStarted = false;
  CfgNode* ifNode = NULL;
  while (getline(infile, line))
  {
    line.erase(remove(line.begin(), line.end(), '\t'), line.end());
    string currentFuncName = line.substr(0, line.find('('));
    if (currentFuncName == "createNode")
    {
      currentNode->m_nextFunc.m_line = line;
      CfgNode* nextNode = new CfgNode;
      currentNode->m_children.insert(nextNode);
      nextNode->m_parents.insert(currentNode);
      currentNode = nextNode;
      tree->m_leaf = currentNode;
    }
    else if (currentFuncName == "setNode")
    {
      currentNode->m_nextFunc.m_line = line;
      CfgNode* nextNode = new CfgNode;
      currentNode->m_children.insert(nextNode);
      nextNode->m_parents.insert(currentNode);
      currentNode = nextNode;
      tree->m_leaf = currentNode;
    }
    else if (currentFuncName == "skip")
    {
      currentNode->m_nextFunc.m_line = line;
      CfgNode* nextNode = new CfgNode;
      currentNode->m_children.insert(nextNode);
      nextNode->m_parents.insert(currentNode);
      currentNode = nextNode;
      tree->m_leaf = currentNode;
    }
    else if (currentFuncName == "while")
    {
      currentNode->m_nextFunc.m_line = line;
      CfgTree* whileTree = createCfg(infile);
      currentNode->m_children.insert(whileTree->m_root);
      whileTree->m_root->m_parents.insert(currentNode);
      for (set<CfgNode*>::iterator leafParentIter = whileTree->m_leaf->m_parents.begin();
        leafParentIter != whileTree->m_leaf->m_parents.end();
        leafParentIter++)
      {
        currentNode->m_parents.insert(*leafParentIter);
        (*leafParentIter)->m_children.insert(currentNode);
      }
      delete(whileTree->m_leaf);
      whileTree->m_leaf->m_children.insert(currentNode);
      currentNode->m_parents.insert(whileTree->m_leaf);
      CfgNode* nextNode = new CfgNode; // the node after the while condition is false
      currentNode->m_children.insert(nextNode);
      nextNode->m_parents.insert(currentNode);
      currentNode = nextNode;
      // TODO: delete the CfgTree?
    }
    else if (currentFuncName == "if")
    {
      currentNode->m_nextFunc.m_line = line;
      CfgTree* ifTree = createCfg(infile);
      currentNode->m_children.insert(ifTree->m_root);
      ifTree->m_root->m_parents.insert(currentNode);
      ifTree->m_leaf->m_parents.insert(currentNode);
      currentNode->m_children.insert(ifTree->m_root);
      currentNode->m_children.insert(ifTree->m_leaf);
      ifNode = currentNode; // update in case of "else"
      currentNode = ifTree->m_leaf; // ifTree->leaf = nextNode
      // TODO: delete the CfgTree?
    }
    else if (currentFuncName == "else")
    {
      currentNode->m_nextFunc.m_line = line;
      currentNode->m_parents.erase(ifNode);
      ifNode->m_children.erase(currentNode);
      CfgTree* elseTree = createCfg(infile);
      ifNode->m_children.insert(elseTree->m_root);
      for (set<CfgNode*>::iterator leafParentIter = elseTree->m_leaf->m_parents.begin();
        leafParentIter != elseTree->m_leaf->m_parents.end();
        leafParentIter++)
      {
        currentNode->m_parents.insert(*leafParentIter);
        (*leafParentIter)->m_children.insert(currentNode);
      }
      delete(elseTree->m_leaf);
      // TODO: delete the CfgTree?
    }
    else if (currentFuncName == "{")
    {
      scopeStarted = true;
      continue;
    }
    else if (currentFuncName == "}")
    {
      assert(scopeStarted);
      return tree;
    }
    else
    {
      cout << "unexpected line: " << line;
      assert(false);
    }
  }
  cout << "incorrect scoping - reached EOF";
  return tree;
}

bool Cfg::checkValidity()
{
   for (set<struct CfgNode*>::iterator iter = m_tree->m_nodes.begin(); 
    iter != m_tree->m_nodes.end(); iter++)
   {
     CfgNode* currentNode = *iter;
     if (!currentNode->m_state.checkValidity())
     {
       cout << "check validity of the state failed: " << currentNode->m_nextFunc.m_line;
       return false;
     }
   }
   return true;
}
