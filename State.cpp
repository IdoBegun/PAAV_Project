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


TreeNode* State::getUniqueRoot(const string& a_name)
{
	for (NodeSetIter iter = m_rootSet.begin(); iter != m_rootSet.end(); iter++)
	{
		string rootName;
		if (!((*iter)->getUniqueName(rootName)))
		{
			// ERROR? We assume a root has a unique name
		}

		if (a_name == rootName)
		{
			return (*iter);
		}
	}

	return NULL;
}


const NodeSet& State::getVariableNodes(const string a_name)
{
	VariableMapIter iter = m_variableMap.find(a_name);
	if (iter == m_variableMap.end())
	{
		// ERROR?
		return NodeSet();
	}

	return (iter->second);
}

void State::runFunction(const Function& a_func)
{
	if (isEmpty())
	{
		return;
	}

	FunctionName name = a_func.getName();
	switch (name)
	{
	case e_createNode:
		functionCreateNode(a_func.getFirstVar(), a_func.getValue());
		break;
	case e_setLeft:
		functionSetLeft(a_func.getFirstVar(), a_func.getSecondVar());
		break;
	case e_setRight:
		functionSetRight(a_func.getFirstVar(), a_func.getSecondVar());
		break;
	case e_setValue:
		functionSetValue(a_func.getFirstVar(), a_func.getValue());
		break;
	case e_less:
		functionLessEqual(a_func.getFirstVar(), a_func.getValue()-1);
		break;
	case e_lessEqual:
		functionLessEqual(a_func.getFirstVar(), a_func.getValue());
		break;
	case e_greater:
		functionGreaterEqual(a_func.getFirstVar(), a_func.getValue()+1);
		break;
	case e_greaterEqual:
		functionGreaterEqual(a_func.getFirstVar(), a_func.getValue());
		break;
	}
}


void State::functionCreateNode(const string& a_name, int a_value)
{
	TreeNode* root = getUniqueRoot(a_name);
	if (root != NULL)
	{
		//ERROR? Can't have 2 roots with the same name
		return;
	}

	TreeNode* node = new TreeNode(a_name, NULL, NULL, NULL, a_value, a_value);
	m_rootSet.insert(node);
	addNodeName(a_name, node);
}

void State::functionSetLeft(const string& a_parent, const string& a_child)
{
	TreeNode* childNode = getUniqueRoot(a_child);
	if (childNode == NULL)
	{
		//ERROR? Can't set a node as a child if it already has a parent
		return;
	}

	const NodeSet& parentSet = getVariableNodes(a_parent);
	for (NodeSetIter iter = parentSet.begin(); iter != parentSet.end(); iter++)
	{
		if ((*iter) == NULL)
		{
			// ERROR?
			continue;
		}

		if (((*iter)->getLeftChild() != NULL))
		{
			(*iter)->setLeftChild(childNode);
		}
	}
}

void State::functionSetRight(const string& a_parent, const string& a_child)
{
	TreeNode* childNode = getUniqueRoot(a_child);
	if (childNode == NULL)
	{
		//ERROR? Can't set a node as a child if it already has a parent
		return;
	}

	const NodeSet& parentSet = getVariableNodes(a_parent);
	for (NodeSetIter iter = parentSet.begin(); iter != parentSet.end(); iter++)
	{
		if ((*iter) == NULL)
		{
			// ERROR?
			continue;
		}

		if (((*iter)->getRightChild() != NULL))
		{
			(*iter)->setRightChild(childNode);
		}
	}
}

void State::functionSetValue(const string& a_name, int a_value)
{
	const NodeSet& parentSet = getVariableNodes(a_name);
	for (NodeSetIter iter = parentSet.begin(); iter != parentSet.end(); iter++)
	{
		if ((*iter) == NULL)
		{
			// ERROR?
			continue;
		}

		(*iter)->setMaxValue(a_value);
		(*iter)->setMinValue(a_value);
	}
}


void State::functionLessEqual(const string& a_name, int a_value)
{
	const NodeSet& parentSet = getVariableNodes(a_name);
	for (NodeSetIter iter = parentSet.begin(); iter != parentSet.end(); iter++)
	{
		if ((*iter) == NULL)
		{
			// ERROR?
			continue;
		}

		if ((*iter)->getMinValue() > a_value)
		{
			m_isEmpty = true;
			return;
		}

		if ((*iter)->getMaxValue() > a_value)
		{
			(*iter)->setMaxValue(a_value);
		}
	}
}


void State::functionGreaterEqual(const string& a_name, int a_value)
{
	const NodeSet& parentSet = getVariableNodes(a_name);
	for (NodeSetIter iter = parentSet.begin(); iter != parentSet.end(); iter++)
	{
		if ((*iter) == NULL)
		{
			// ERROR?
			continue;
		}

		if ((*iter)->getMaxValue() < a_value)
		{
			m_isEmpty = true;
			return;
		}

		if ((*iter)->getMinValue() < a_value)
		{
			(*iter)->setMinValue(a_value);
		}
	}
}
