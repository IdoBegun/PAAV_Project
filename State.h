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
typedef map<string, NodeSet> VariableMap;
typedef VariableMap::iterator VariableMapIter;
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
	bool isEmpty() const { return m_isEmpty; };

	bool checkValidity();
	void runFunction(const Function& a_func);
	void join(const State& a_otherState);

private:
	void buildVariableMap();
	void addNodeNames(TreeNode* a_node);
	void addNodeName(const string& a_name, TreeNode* a_node);
	void divideTrees(const State& a_other, NodePairSet& a_commonRoots, NodeSet& a_myUniqueRoots, NodeSet& a_otherUniqueRoots);

	VariableMap m_variableMap;
	NodeSet m_rootSet;
	bool m_isEmpty;
};

#endif _STATE_H
