#include "State.h"

using namespace std;

State::State():m_isEmpty(false)
{
}

State::State(const State& a_other)
{
	m_isEmpty = a_other.isEmpty();

	if (m_isEmpty)
	{
		return;
	}

	// Probably better to do outside of CTOR
	const NodeSet& otherRootSet = a_other.getRootSet();

	for (NodeSetIter iter = otherRootSet.begin(); iter != otherRootSet.end(); iter++)
	{
		TreeNode* node = new TreeNode(*(*iter));
	}

	buildVariableMap();

}

State::~State()
{
	for (NodeSetIter iter = m_rootSet.begin(); iter != m_rootSet.end(); iter++)
	{
		delete (*iter);
	}
}


bool State::checkValidity()
{

	if (m_isEmpty)
	{
		return false;
	}

	for (NodeSetIter iter = m_rootSet.begin(); iter != m_rootSet.end(); iter++)
	{
		TreeNode* node = *iter;
		if (node == NULL)
		{
			return false;
		}
		if (!(node->checkValidity(INT_MIN, INT_MAX)))
		{
			return false;
		}
	}
	return true;
}


void State::runFunction(const Function& a_func)
{
	//TODO
}

void State::join(const State& a_otherState)
{
	NodePairSet commonRoots;
	NodeSet myUniqueRoots;
	NodeSet otherUniqueRoots;

	divideTrees(a_otherState, commonRoots, myUniqueRoots, otherUniqueRoots);
	m_rootSet.clear();

	for (NodePairSetIter iter = commonRoots.begin(); iter != commonRoots.end(); iter++)
	{
		TreeNode* myTree = iter->first;
		TreeNode* otherTree = iter->second;

		myTree->join(otherTree);
		m_rootSet.insert(myTree);
	}

	for (NodeSetIter iter = myUniqueRoots.begin(); iter != myUniqueRoots.end(); iter++)
	{
		m_rootSet.insert(*iter);
	}

	for (NodeSetIter iter = otherUniqueRoots.begin(); iter != otherUniqueRoots.end(); iter++)
	{
		TreeNode* node = new TreeNode(*(*iter));
		m_rootSet.insert(node);
	}

	buildVariableMap();
}


void State::buildVariableMap()
{
	m_variableMap.clear();
	for (NodeSetIter iter = m_rootSet.begin(); iter != m_rootSet.end(); iter++)
	{
		addNodeNames(*iter);
	}

}

void State::addNodeNames(TreeNode* a_node)
{
	if (a_node == NULL)
	{
		return;
	}

	const NameSet& nameSet = a_node->getNameSet();
	for (NameSetIter iter = nameSet.begin(); iter != nameSet.end(); iter++)
	{
		addNodeName(*iter, a_node);
	}

	addNodeNames(a_node->getLeftChild());
	addNodeNames(a_node->getRightChild());

}

void State::addNodeName(const string& a_name, TreeNode* a_node)
{
	VariableMapIter iter = m_variableMap.find(a_name);
	if (iter == m_variableMap.end())
	{
		m_variableMap.insert(make_pair(a_name, NodeSet()));
	}

	m_variableMap[a_name].insert(a_node);
}

void State::divideTrees(const State& a_other, NodePairSet& a_commonRoots, NodeSet& a_myUniqueRoots, NodeSet& a_otherUniqueRoots)
{
	NodeSet otherRootSet = a_other.getRootSet(); // Copy the entire set
	a_commonRoots.clear();
	a_myUniqueRoots.clear();
	a_otherUniqueRoots.clear();

	for (NodeSetIter iter = m_rootSet.begin(); iter != m_rootSet.end(); iter++)
	{
		bool isUnique = true;
		string rootName;
		if (!((*iter)->getUniqueName(rootName)))
		{
			// ERROR? We assume a root has a unique name
		}

		for (NodeSetIter otherIter = otherRootSet.begin(); otherIter != m_rootSet.end(); otherIter++)
		{
			string otherRootName;
			if (!((*otherIter)->getUniqueName(otherRootName)))
			{
				// ERROR? We assume a root has a unique name
			}

			if (rootName == otherRootName)
			{
				a_commonRoots.insert(make_pair((*iter), (*otherIter)));
				otherRootSet.erase(otherIter);
				isUnique = false;
				break;
			}
		}

		if (isUnique)
		{
			a_myUniqueRoots.insert(*iter);
		}
	}
	a_otherUniqueRoots = otherRootSet;

}