#ifndef _STATE_H
#define _STATE_H

#include <string>
#include <map>
#include <set>
#include "TreeNode.h"
#include "Function.h"

using namespace std;

typedef set<TreeNode*> NodeSet;
typedef NodeSet::iterator NodeSetIter;
typedef NodeSet::const_iterator NodeSetConstIter;
typedef map<string, NodeSet> VariableMap;
typedef VariableMap::iterator VariableMapIter;
typedef VariableMap::const_iterator VariableMapConstIter;
typedef set<pair<TreeNode*, TreeNode*>> NodePairSet;
typedef NodePairSet::iterator NodePairSetIter;

class State
{
public:
	State();
	State(const State& a_other); // Deep CTOR
	~State(); // Deep DTOR

	// Getters
	const VariableMap& getVariableMap() const { return m_variableMap; };
	const NodeSet& getRootSet() const { return m_rootSet; };
	bool isTop() const { return m_isTop; };

	bool checkValidity();
	void runFunction(const Function& a_func);
	void join(const State& a_otherState);

	void printState() const;
	operator string() const;

private:
	void buildVariableMap();
	void addNodeNames(TreeNode* a_node);
	void addNodeName(const string& a_name, TreeNode* a_node);
	void removeTrees(const NodeSet& a_nodeSet);
	void removeTree(TreeNode* a_node);
	void divideTrees(const State& a_other, NodePairSet& a_commonRoots, NodeSet& a_myUniqueRoots, NodeSet& a_otherUniqueRoots);
	TreeNode* getUniqueRoot(const string& a_name);
	NodeSet* getVariableNodes(const string a_name);


	void functionCreateNode(const string& a_name, int a_value);
	void functionSetLeft(const string& a_parent, const string& a_child);
	void functionSetRight(const string& a_parent, const string& a_child);
	void functionSetValue(const string& a_name, int a_value);
	void functionLessEqual(const string& a_name, int a_value);
	void functionGreaterEqual(const string& a_name, int a_value);
	void functionIncrement(const string& a_name, int a_value);

	VariableMap m_variableMap;
	NodeSet m_rootSet;
	bool m_isTop;
};

#endif
