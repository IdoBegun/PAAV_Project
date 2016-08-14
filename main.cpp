#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>      // std::filebuf
#include <sstream>
#include <string>
#include <new>
#include <assert.h> 
#include "DS.h"

using namespace std;


CfgTree *createCfg(istream& infile)
{
  CfgTree* tree = new CfgTree;
  CfgNode* currentNode = new CfgNode;
  tree->root = tree->leaf = currentNode;
  string line;
  bool scopeStarted = false;
  CfgNode* ifNode = NULL;
  while (getline(infile, line))
  {
    istringstream iss(line);
    string currentFuncName;
    iss >> currentFuncName;
    if (currentFuncName == "createNode")
    {
      currentNode->nextFunc.line = line;
      CfgNode* nextNode = new CfgNode;
      currentNode->children.insert(nextNode);
      nextNode->parents.insert(currentNode);
      currentNode = nextNode;
    }
    else if (currentFuncName == "setNode")
    {
      currentNode->nextFunc.line = line;
      CfgNode* nextNode = new CfgNode;
      currentNode->children.insert(nextNode);
      nextNode->parents.insert(currentNode);
      currentNode = nextNode;
    }
    else if (currentFuncName == "skip")
    {
      currentNode->nextFunc.line = line;
      CfgNode* nextNode = new CfgNode;
      currentNode->children.insert(nextNode);
      nextNode->parents.insert(currentNode);
      currentNode = nextNode;
    }
    else if (currentFuncName == "while")
    {
      currentNode->nextFunc.line = line;
      CfgTree* whileTree = createCfg(infile);
      currentNode->children.insert(whileTree->root);
      whileTree->root->parents.insert(currentNode);
      for (set<CfgNode*>::iterator leafParentIter = whileTree->leaf->parents.begin();
        leafParentIter != whileTree->leaf->parents.end();
        leafParentIter++)
      {
        currentNode->parents.insert(*leafParentIter);
        (*leafParentIter)->children.insert(currentNode);
      }
      delete(whileTree->leaf);
      whileTree->leaf->children.insert(currentNode);
      currentNode->parents.insert(whileTree->leaf);
      CfgNode* nextNode = new CfgNode; // the node after the while condition is false
      currentNode->children.insert(nextNode);
      nextNode->parents.insert(currentNode);
      currentNode = nextNode;
      // TODO: delete the CfgTree?
    }
    else if (currentFuncName == "if")
    {
      currentNode->nextFunc.line = line;
      CfgTree* ifTree = createCfg(infile);
      currentNode->children.insert(ifTree->root);
      ifTree->root->parents.insert(currentNode);
      ifTree->leaf->parents.insert(currentNode);
      currentNode->children.insert(ifTree->root);
      currentNode->children.insert(ifTree->leaf);
      ifNode = currentNode; // update in case of "else"
      currentNode = ifTree->leaf; // ifTree->leaf = nextNode
      // TODO: delete the CfgTree?
    }
    else if (currentFuncName == "else")
    {
      currentNode->nextFunc.line = line;
      currentNode->parents.erase(ifNode);
      ifNode->children.erase(currentNode);
      CfgTree* elseTree = createCfg(infile);
      ifNode->children.insert(elseTree->root);
      for (set<CfgNode*>::iterator leafParentIter = elseTree->leaf->parents.begin();
        leafParentIter != elseTree->leaf->parents.end();
        leafParentIter++)
      {
        currentNode->parents.insert(*leafParentIter);
        (*leafParentIter)->children.insert(currentNode);
      }
      delete(elseTree->leaf);
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
  return NULL;
}

int main(int argc, char** argv)
{
  char* filePath = "example.txt";
  filebuf fb;
  if (fb.open (filePath,ios::in))
  {
    istream infile(&fb);
    CfgTree* tree = createCfg(infile);
    fb.close();
  }
  else
  {
    cout << "can't open the file: " << filePath;
  }
  return 0;
}