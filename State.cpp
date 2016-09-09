#include <iostream>
#include <sstream>
#include "State.h"
#include "global.h"

using namespace std;

State::State():m_isTop(false)
{
}


State::State(const State& a_other)
{
	debug("State::CTOR - Copying state: " + string(a_other));
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
	debug("State::CTOR - Copied state: " + string(*this));
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

	debug("State::join - joining state: " + string(*this));
	debug("State::join - with state: " + string(a_otherState));

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

	debug("State::join - joined state: " + string(*this));
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
	for (NodeSetConstIter iter = a_nodeSet.begin(); iter != a_nodeSet.end(); iter++)
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
			error("State::divideTrees - Multiple roots (in this state) with the same name " + rootName + " exist! " + string(*this), true);
		}

		for (NodeSetIter otherIter = otherRootSet.begin(); otherIter != otherRootSet.end(); otherIter++)
		{
			string otherRootName;
			if (!((*otherIter)->getUniqueName(otherRootName)))
			{
				error("State::divideTrees - Multiple roots (in joined state) with the same name " + otherRootName + " exist! " + string(a_other), true);
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
			error("State::getUniqueRoot - Multiple roots with the same name " + a_name + " exist! " + string(*this), true);
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

	verbose("State::runFunction - Running function: " + string(a_func));
	debug("State::runFunction - State before running function: " + string(*this));

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
	debug("State::runFunction - State after running function: " + string(*this));
}


void State::functionCreateNode(const string& a_name, int a_value)
{
	TreeNode* root = getUniqueRoot(a_name);
	if (root != NULL)
	{
		verbose("State::functionCreateNode - A root called " + a_name + " already exists");
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

	verbose("State::functionCreateNode - Creating a new node: " + a_name + " with value: " + to_string(a_value));
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
		error("State::functionSetLeft - variable " + a_parent + " doesn't exist", false);
		return;
	}
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionSetLeft - iterator for variable " + a_parent + " is NULL", false);
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
		error("State::functionSetRight - variable " + a_parent + " doesn't exist", false);
		return;
	}
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionSetRight - iterator for variable " + a_parent + " is NULL", false);
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
		stringstream ss;
		error("State::functionSetValue - variable " + a_name + " doesn't exist", false);
		return;
	}
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionSetValue - iterator for variable " + a_name + " is NULL", false);
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
		error("State::functionLessEqual - variable " + a_name + " doesn't exist", false);
		return;
	}
	NodeSet removedRoots;
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			stringstream ss;
			ss << "State::functionLessEqual - iterator for variable " << a_name << " is NULL";
			error(ss.str(), false);
			continue;
		}

		if ((*iter)->getMinValue() > a_value)
		{
			debug("State::functionLessEqual - Adding " + a_name + "'s root to removedRoots set");
			removedRoots.insert((*iter)->getRoot());
			continue;
		}

		if ((*iter)->getMaxValue() > a_value)
		{
			(*iter)->setMaxValue(a_value);
		}
	}

	debug("State::functionLessEqual - removing trees");
	removeTrees(removedRoots);
}


void State::functionGreaterEqual(const string& a_name, int a_value)
{
	NodeSet* parentSet = getVariableNodes(a_name);
	if (parentSet == NULL)
	{
		error("State::functionGreaterEqual - variable " + a_name + " doesn't exist" , false);
		return;
	}
	NodeSet removedRoots;
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionGreaterEqual - iterator for variable " + a_name + " is NULL", false);
			continue;
		}

		if ((*iter)->getMaxValue() < a_value)
		{
			debug("State::functionGreaterEqual - Adding " + a_name + "'s root to removedRoots set");
			removedRoots.insert((*iter)->getRoot());
			continue;
		}

		if ((*iter)->getMinValue() < a_value)
		{
			(*iter)->setMinValue(a_value);
		}
	}

	debug("State::functionGreaterEqual - removing trees");
	removeTrees(removedRoots);
}


void State::functionIncrement(const string& a_name, int a_value)
{
	NodeSet* parentSet = getVariableNodes(a_name);
	if (parentSet == NULL)
	{
		error("State::functionIncrement - variable " + a_name + " doesn't exist", false);
		return;
	}
	for (NodeSetConstIter iter = parentSet->begin(); iter != parentSet->end(); iter++)
	{
		if ((*iter) == NULL)
		{
			error("State::functionIncrement - iterator for variable " + a_name + " is NULL", false);
			continue;
		}

		int maxValue = (*iter)->getMaxValue();
		int minValue = (*iter)->getMinValue();
		(*iter)->setMaxValue(maxValue + a_value);
		(*iter)->setMinValue(minValue + a_value);
	}
}


void State::printState() const
{
	cout << string(*this);
}


State::operator string() const
{
	stringstream ss;

	ss << "State: ";
	if (m_isTop)
	{
		ss << "Top" << endl;
	}
	else
	{
		ss << endl << "\tVariables: ";
		for (VariableMapConstIter iter = m_variableMap.begin(); iter != m_variableMap.end(); iter++)
		{
			ss << iter->first << ",";
		}
		ss << endl << "\tTrees:" << endl;

		for (NodeSetConstIter iter = m_rootSet.begin(); iter != m_rootSet.end(); iter++)
		{
			ss << "\t";
			if ((*iter) == NULL)
			{
				ss << "NULL";
			}
			else
			{
				ss << string(*(*iter));
			}
			ss << endl;
		}
	}
	return ss.str();
}
