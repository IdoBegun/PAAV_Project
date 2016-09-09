#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>      // std::filebuf
#include <sstream>
#include <string>
#include <algorithm>
#include <new>
#include <limits.h>
#include "Cfg.h"
#include "global.h"

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
  if (graphToUpdate.m_leaf->m_ifParent != NULL)
  {
    CfgNode* ifNode = graphToUpdate.m_leaf->m_ifParent;
    ifNode->m_afterIfChild = currentNode;
    currentNode->m_ifParent = ifNode;
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
  currentNode->m_afterIfChild = ifGraph.m_leaf;
  ifGraph.m_leaf->m_ifParent = currentNode;
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
  ifNode->m_afterIfChild = currentNode;
  currentNode->m_ifParent = ifNode;
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
		  error("Cfg::createCfg - else without if", true);
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
		  error("Cfg::createCfg - scope has been closed but hasn't been opened", true);
      }
      return graph;
    }
    else if (currentFuncName == "")
    {
      continue; //an empty line
    }
    else
    {
		stringstream ss;
		ss << "Cfg::createCfg - unexpected line: " << line;
		error(ss.str(), true);
    }
  }
  return graph;
}

void Cfg::runUntilNode(CfgNode*& currentNode, CfgNode*& untilNode, const int& numberOfIterations)
{
  while (currentNode != untilNode)
  {
    if (currentNode->type == e_regular)
    {
      State newState(currentNode->m_state);
      newState.runFunction(currentNode->m_nextFunc);
      for (set<CfgNode*>::iterator childIter = currentNode->m_children.begin();
        childIter != currentNode->m_children.end();
        childIter++)
      {
        CfgNode* currentChild = *childIter;
        currentChild->m_state.join(newState);
      }
      currentNode = *(currentNode->m_children.begin());
    }
    else if (currentNode->type == e_if)
    {
      // go to the true direction
      State newTrueState(currentNode->m_state);
      newTrueState.runFunction(currentNode->m_nextFunc);
      currentNode->m_trueChild->m_state.join(newTrueState);
      runUntilNode(currentNode->m_trueChild, currentNode->m_afterIfChild, numberOfIterations);
      // go to the false direction
      State newFalseState(currentNode->m_state);
      Function invertFunc = currentNode->m_nextFunc.invertFunction();
      newFalseState.runFunction(invertFunc);
      currentNode->m_falseChild->m_state.join(newFalseState);
      runUntilNode(currentNode->m_falseChild, currentNode->m_afterIfChild, numberOfIterations);
      // go to the node after the if/else condition
      currentNode = currentNode->m_afterIfChild;
    }
    else if (currentNode->type == e_while)
    {
      // run the while loop numberOfIterations times
      for (int iter = 0; iter < numberOfIterations; iter++)
      {
        State newTrueState(currentNode->m_state);
        newTrueState.runFunction(currentNode->m_nextFunc);
        currentNode->m_trueChild->m_state.join(newTrueState);
        runUntilNode(currentNode->m_trueChild, currentNode, numberOfIterations);
      }
      // go to the false direction
      State newFalseState(currentNode->m_state);
      Function invertFunc = currentNode->m_nextFunc.invertFunction();
      newFalseState.runFunction(invertFunc);
      currentNode->m_falseChild->m_state.join(newFalseState);
      currentNode = currentNode->m_falseChild;
    }
  }
}

void Cfg::runProgram(const int& numberOfIterations)
{
  runUntilNode(m_graph.m_root, m_graph.m_leaf, numberOfIterations);
}

bool Cfg::checkValidity()
{
  return (m_graph.m_leaf->m_state.checkValidity());
}
