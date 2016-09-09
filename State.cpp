#include <iostream>
#include <assert.h>
#include "State.h"
#include "global.h"

using namespace std;

State::State():m_isTop(false)
{
}


State::State(const State& a_other)
{
	m_isTop = a_other.isTop();

	if (m_isTop)
	{
		return;
	}

	// Probably better to do outside of CTOR
	const NodeSet& otherRootSet = a_other.getRootSet();

	for (NodeSetConstIter iter = otherRootSet.begin(); iter != otherRootSet.end(); iter++)
	{
		TreeNode* node = new TreeNode(*(*iter));
		m_rootSet.insert(node);
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

	if (m_isTop)
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
	for (NameSetConstIter iter = nameSet.begin(); iter != nameSet.end(); iter++)
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


void State::removeTrees(const NodeSet& a_nodeSet)
{
	for (NodeSetIter iter = a_nodeSet.begin(); iter != a_nodeSet.end(); iter++)
	{
		removeTree(*iter);
	}
}

void State::removeTree(TreeNode* a_node)
{
	NodeSetIter iter = m_rootSet.find(a_node);
	if (iter == m_rootSet.end())
	{
		return;
	}

	delete a_node;
	m_rootSet.erase(a_node);
	buildVariableMap();

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
			error("State::divideTrees - We assume a root has a unique name1", true);
		}

		for (NodeSetIter otherIter = otherRootSet.begin(); otherIter != otherRootSet.end(); otherIter++)
		{
			string otherRootName;
			if (!((*otherIter)->getUniqueName(otherRootName)))
			{
				error("State::divideTrees - We assume a root has a unique name2", true);
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
			error("State::getUniqueRoot - We assume a root has a unique name2", true);
		}

		if (a_name == rootName)
		{
			return (*iter);
		}
	}

	return NULL;
}


NodeSet* State::getVariableNodes(const string a_name)
{
	VariableMapIter iter = m_variableMap.find(a_name);
	if (iter == m_variableMap.end())
	{
		error("State::getVariableNodes - name wasn't found", false);
		return NULL;
	}

	return &(iter->second);
}


void State::runFunction(const Function& a_func)
{
	if (isTop())
	{
		return;
	}

	debug("State::runFunction - Before running function");

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
	case e_increment:
		functionIncrement(a_func.getFirstVar(), a_func.getValue());
		break;
	case e_decrement:
		functionIncrement(a_func.getFirstVar(), -1 * a_func.getValue());
		break;
	}
}


void State::functionCreateNode(const string& a_name, int a_value)
{
	TreeNode* root = getUniqueRoot(a_name);
	if (root != NULL)
	{
		int minVal = root->getMinValue();
		int maxVal = root->getMaxValue();
		if (a_value < minVal)
		{
			root->setMinValue(a_value);
		}

		if (a_value > maxVal)
		{
			root->setMaxValue(a_value);
		}
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
		error("State::functionSetLeft - Can't set a node as a child if it already has a parent", false);
		return;
	}

	NodeSet* parentSet = getVariableNodes(a_parent);
	if (parentSet == NULL)
	{
		error("State::functionSetLeft - variable doesn't exist", false);
		return;
	}
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionSetLeft - iterator is null", false);
			continue;
		}

		TreeNode* leftChild = (*iter)->getLeftChild();
		if (leftChild == NULL)
		{
			TreeNode* node = new TreeNode(*childNode);
			node->setParent(*iter);
			(*iter)->setLeftChild(node);
		}
		else {
			leftChild->join(childNode);
		}
	}

	m_rootSet.erase(childNode);
	delete childNode;

	buildVariableMap();
}


void State::functionSetRight(const string& a_parent, const string& a_child)
{
	TreeNode* childNode = getUniqueRoot(a_child);
	if (childNode == NULL)
	{
		error("State::functionSetRight - Can't set a node as a child if it already has a parent", false);
		return;
	}

	NodeSet* parentSet = getVariableNodes(a_parent);
	if (parentSet == NULL)
	{
		error("State::functionSetRight - variable doesn't exist", false);
		return;
	}
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionSetRight - iterator is null", false);
			continue;
		}

		TreeNode* rightChild = (*iter)->getRightChild();
		if (rightChild == NULL)
		{
			TreeNode* node = new TreeNode(*childNode);
			node->setParent(*iter);
			(*iter)->setRightChild(node);
		}
		else
    {
			rightChild->join(childNode);
		}
	}

	m_rootSet.erase(childNode);
	delete childNode;

	buildVariableMap();
}


void State::functionSetValue(const string& a_name, int a_value)
{
	NodeSet* parentSet = getVariableNodes(a_name);
	if (parentSet == NULL)
	{
		error("State::functionSetValue - variable doesn't exist", false);
		return;
	}
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionSetValue - iterator is null", false);
			continue;
		}

		(*iter)->setMaxValue(a_value);
		(*iter)->setMinValue(a_value);
	}
}


void State::functionLessEqual(const string& a_name, int a_value)
{
	NodeSet* parentSet = getVariableNodes(a_name);
	if (parentSet == NULL)
	{
		error("State::functionLessEqual - variable doesn't exist", false);
		return;
	}
	NodeSet removedRoots;
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionLessEqual - iterator is null", false);
			continue;
		}

		if ((*iter)->getMinValue() > a_value)
		{
			removedRoots.insert((*iter)->getRoot());
			continue;
		}

		if ((*iter)->getMaxValue() > a_value)
		{
			(*iter)->setMaxValue(a_value);
		}
	}

	removeTrees(removedRoots);
}


void State::functionGreaterEqual(const string& a_name, int a_value)
{
	NodeSet* parentSet = getVariableNodes(a_name);
	if (parentSet == NULL)
	{
		error("State::functionGreaterEqual - variable doesn't exist", false);
		return;
	}
	NodeSet removedRoots;
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionGreaterEqual - iterator is null", false);
			continue;
		}

		if ((*iter)->getMaxValue() < a_value)
		{
			removedRoots.insert((*iter)->getRoot());
			continue;
		}

		if ((*iter)->getMinValue() < a_value)
		{
			(*iter)->setMinValue(a_value);
		}
	}

	removeTrees(removedRoots);
}


void State::functionIncrement(const string& a_name, int a_value)
{
	NodeSet* parentSet = getVariableNodes(a_name);
	if (parentSet == NULL)
	{
		error("State::functionIncrement - variable doesn't exist", false);
		return;
	}
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionIncrement - iterator is null", false);
			continue;
		}

		int maxValue = (*iter)->getMaxValue();
		int minValue = (*iter)->getMinValue();
		(*iter)->setMaxValue(maxValue + a_value);
		(*iter)->setMinValue(minValue + a_value);
	}
}


void State::clearState()
{
	for (NodeSetIter iter = m_rootSet.begin(); iter != m_rootSet.end(); iter++)
	{
		delete (*iter);
	}

	m_variableMap.clear();
}


void State::printState() const
{
	cout << "State::printState - Printing State:" << endl;
	if (m_isTop)
	{
		cout << "Top!" << endl;
		return;
	}

	cout << "Variables: ";
	for (VariableMapConstIter iter = m_variableMap.begin(); iter != m_variableMap.end(); iter++)
	{
		cout << iter->first << ",";
	}
	cout << endl << "Trees:" << endl;

	for (NodeSetConstIter iter = m_rootSet.begin(); iter != m_rootSet.end(); iter++)
	{
		(*iter)->printTree();
		cout << endl;
	}
}


void State::error(const string& a_message, bool a_exit)
{
	cout << a_message << endl;
#if DEBUG>0
	printState();
#endif
	if (a_exit)
	{
		assert(false);
	}
}


void State::debug(const string& a_message)
{
#if DEBUG>0
	cout << a_message << endl;
	printState();
#endif
}
