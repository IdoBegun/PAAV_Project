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
  m_graph = createCfg(infile);
}

void Cfg::deleteCfg()
{
  for (set<struct CfgNode*>::iterator iter = m_graph.m_nodes.begin(); 
    iter != m_graph.m_nodes.end();)
  {
    CfgNode* currentNode = *iter;
    iter++;
    delete(currentNode);
  }
}

void CfgNode::addChild(CfgNode*& childToAdd)
{
  this->m_children.insert(childToAdd);
  childToAdd->m_parents.insert(this);
}

void CfgNode::removeChild(CfgNode*& childToRemove)
{
  this->m_children.erase(childToRemove);
  childToRemove->m_parents.erase(this);
}

void CfgGraph::deleteNode(CfgNode* nodeToDelete)
{
  this->m_nodes.erase(nodeToDelete);
  delete(nodeToDelete);
}

void Cfg::createRegularNode(CfgNode*& currentNode, CfgGraph& graph, string& line)
{
  currentNode->type = e_regular;
  currentNode->m_nextFunc = Function(line);
  CfgNode* nextNode = new CfgNode;
  currentNode->addChild(nextNode);
  currentNode = nextNode;
  graph.m_leaf = currentNode;
  graph.m_nodes.insert(currentNode);
}

void Cfg::updateLeafNode(CfgGraph& graph, CfgGraph& graphToUpdate, CfgNode*& currentNode)
{
  for (set<CfgNode*>::iterator leafParentIter = graphToUpdate.m_leaf->m_parents.begin();
    leafParentIter != graphToUpdate.m_leaf->m_parents.end();)
  {
    CfgNode* nodeToUpdate = *leafParentIter;
    leafParentIter++;
    nodeToUpdate->addChild(currentNode);
    nodeToUpdate->removeChild(graphToUpdate.m_leaf);
    if (nodeToUpdate->m_trueChild == graphToUpdate.m_leaf)
    {
      nodeToUpdate->m_trueChild = currentNode;
    }
    if (nodeToUpdate->m_falseChild == graphToUpdate.m_leaf)
    {
      nodeToUpdate->m_falseChild = currentNode;
    }
  }
  graphToUpdate.deleteNode(graphToUpdate.m_leaf);
  graph.m_nodes.insert(graphToUpdate.m_nodes.begin(), graphToUpdate.m_nodes.end());
}

void Cfg::createWhileGraph(CfgNode*& currentNode, CfgGraph& graph, string& line, istream& infile)
{
  currentNode->type = e_while;
  currentNode->m_nextFunc = Function(line);
  CfgGraph whileGraph = createCfg(infile);
  currentNode->addChild(whileGraph.m_root);
  currentNode->m_trueChild = whileGraph.m_root;
  updateLeafNode(graph, whileGraph, currentNode);
  CfgNode* nextNode = new CfgNode; // the node after the while condition is false
  currentNode->addChild(nextNode);
  currentNode->m_falseChild = nextNode;
  currentNode = nextNode;
  graph.m_leaf = currentNode;
  graph.m_nodes.insert(currentNode);
}

void Cfg::createIfGraph(CfgNode*& currentNode, CfgGraph& graph, string& line, istream& infile, CfgNode*& ifNode)
{
  currentNode->type = e_if;
  currentNode->m_nextFunc = Function(line);
  CfgGraph ifGraph = createCfg(infile);
  currentNode->addChild(ifGraph.m_root);
  currentNode->m_trueChild = ifGraph.m_root;
  currentNode->addChild(ifGraph.m_leaf);
  currentNode->m_falseChild = ifGraph.m_leaf;
  ifNode = currentNode; // update in case of "else"
  currentNode = ifGraph.m_leaf; // ifGraph.leaf = nextNode
  graph.m_leaf = currentNode;
  graph.m_nodes.insert(ifGraph.m_nodes.begin(), ifGraph.m_nodes.end());
}

void Cfg::createElseGraph(CfgNode*& currentNode, CfgGraph& graph, string& line, istream& infile, CfgNode*& ifNode)
{
  ifNode->removeChild(currentNode);
  CfgGraph elseGraph = createCfg(infile);
  ifNode->addChild(elseGraph.m_root);
  ifNode->m_falseChild = elseGraph.m_root;
  updateLeafNode(graph, elseGraph, currentNode);
  // currentNode is the "empty node" for the next function
}

CfgGraph Cfg::createCfg(istream& infile)
{
  CfgGraph graph;
  CfgNode* currentNode = new CfgNode;
  graph.m_root = graph.m_leaf = currentNode;
  graph.m_nodes.insert(currentNode);
  string line;
  bool scopeStarted = false;
  CfgNode* ifNode = NULL;
  while (getline(infile, line))
  {
    line.erase(remove(line.begin(), line.end(), '\t'), line.end());
    string currentFuncName = line.substr(0, line.find('('));
    if (currentFuncName == "createNode")
    {
      createRegularNode(currentNode, graph, line);
    }
    else if (currentFuncName == "setValue")
    {
      createRegularNode(currentNode, graph, line);
    }
    else if (currentFuncName == "setRight")
    {
      createRegularNode(currentNode, graph, line);
    }
    else if (currentFuncName == "setLeft")
    {
      createRegularNode(currentNode, graph, line);
    }
    else if (currentFuncName == "while")
    {
      createWhileGraph(currentNode, graph, line, infile);
    }
    else if (currentFuncName == "if")
    {
      createIfGraph(currentNode, graph, line, infile, ifNode);
    }
    else if (currentFuncName == "else")
    {
      if (ifNode==NULL)
      {
        cout << "else without if" << endl;
        assert(false);
      }
      createElseGraph(currentNode, graph, line, infile, ifNode);
    }
    else if (currentFuncName == "{")
    {
      scopeStarted = true;
      continue;
    }
    else if (currentFuncName == "}")
    {
      if (!scopeStarted)
      {
        cout << "scope has been closed but it hasn't been opened" << endl;
        assert(false);
      }
      return graph;
    }
    else if (currentFuncName == "")
    {
      continue; //an empty line
    }
    else
    {
      cout << "unexpected line: " << line << endl;
      assert(false);
    }
  }
  return graph;
}

void Cfg::runProgram(const int& numberOfIterations)
{
  for (int iter = 0; iter < numberOfIterations; iter++)
  {
    for (set<struct CfgNode*>::iterator graphNodesIter = m_graph.m_nodes.begin();
      graphNodesIter != m_graph.m_nodes.end();
      graphNodesIter++)
    {
      CfgNode* currentNode = *graphNodesIter;
      if (currentNode->type == e_regular)
      {
        State newState(currentNode->m_state);
        newState.runFunction(currentNode->m_nextFunc);
        for (set<struct CfgNode*>::iterator childIter = currentNode->m_children.begin();
          childIter != currentNode->m_children.end();
          childIter++)
        {
          CfgNode* currentChild = *childIter;
          currentChild->m_state.join(newState);
        }
      }
      else if ((currentNode->type == e_if) || (currentNode->type == e_while))
      {
        State newTrueState(currentNode->m_state);
        newTrueState.runFunction(currentNode->m_nextFunc);
        currentNode->m_trueChild->m_state.join(newTrueState);
        State newFalseState(currentNode->m_state);
        Function invertFunc = currentNode->m_nextFunc.invertFunction();
        newFalseState.runFunction(invertFunc);
        currentNode->m_falseChild->m_state.join(newFalseState);
      }
      // in case of e_noneType - nothing need to be done
    }
  }
}

bool Cfg::checkValidity()
{
   for (set<struct CfgNode*>::iterator iter = m_graph.m_nodes.begin(); 
    iter != m_graph.m_nodes.end(); iter++)
   {
     CfgNode* currentNode = *iter;
     if (!currentNode->m_state.checkValidity())
     {
       cout << "check validity of the state failed: " << currentNode->m_nextFunc.getName() << endl;
       return false;
     }
   }
   return true;
}
