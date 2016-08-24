#include "State.h"

State::State()
{
	//TODO?
}

State::State(const State& other)
{
	//TODO
}

State::~State()
{
	//TODO
}

void State::addVariable(string a_name, TreeNode* a_node)
{
	VariableMapIter iter = m_variableMap.find(a_name);
	if (iter == m_variableMap.end())
	{
		//ERROR? TBD
	}

	m_variableMap[a_name] = a_node;
}

void State::deleteVariable(string a_name)
{
	//TODO
}

bool State::checkValidity()
{
	return m_root->checkValidity(INT_MIN, INT_MAX);
}

TreeNode* State::findNode(string a_name)
{
	return m_root->findNode(a_name);
}

void State::runCommand(string a_cmd)
{

}
