#ifndef _DS_H
#define _DS_H

#include <set>

using namespace std;

typedef struct TreeNode
{
  struct TreeNode* leftChild;
  struct TreeNode* rightChild;
  struct TreeNode* parent;
}TreeNode;

typedef struct
{
  struct TreeNode* root;
}CfgState; // TODO: change to class

typedef struct
{
  string line;
}Func; // TODO: change to classes with parameters

typedef struct CfgNode
{
  CfgState state;
  Func nextFunc;
  set<struct CfgNode*> children;
  set<struct CfgNode*> parents;
}CfgNode;

typedef struct CfgTree
{
  struct CfgNode* root;
  struct CfgNode* leaf;
}CfgTree;

#endif