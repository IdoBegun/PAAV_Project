#ifndef _STATE_H
#define _STATE_H

#include <string>
#include <map>
#include "TreeNode.h"

using namespace std;

typedef map<string, TreeNode*> VariableMap;
typedef VariableMap::iterator VariableMapIter;

class State
{
public:
	State();
	State(const State& other); // Deep CTOR
	~State(); // Deep DTOR

	// Getters
	TreeNode* getRoot() const { return m_root; };

	// Setters
	void setRoot(TreeNode* a_root) { m_root = a_root; };

	// Variable handling
	void addVariable(string a_name, TreeNode* a_node);
	void deleteVariable(string a_name);
	TreeNode* findNode(string a_name);

	bool checkValidity();
	void runCommand(string a_cmd);
	void join(const State& a_otherState);
	void meet(const State& a_otherState);
private:
	
	TreeNode* m_root;
	VariableMap m_variableMap;
};

#endif _STATE_H
