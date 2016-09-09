#include <iostream>
#include "TreeNode.h"

TreeNode::TreeNode(string a_name, TreeNode* a_parent, TreeNode* a_leftChild, TreeNode* a_rightChild, int a_minValue, int a_maxValue) :
	m_parent(a_parent), m_leftChild(a_leftChild), m_rightChild(a_rightChild), m_minValue(a_minValue), m_maxValue(a_maxValue)
{
	m_nameSet.insert(a_name);
}


TreeNode::TreeNode(const TreeNode& other)
{
	m_nameSet = other.getNameSet();
	m_minValue = other.getMinValue();
	m_maxValue = other.getMaxValue();

	if (other.getLeftChild() != NULL)
	{
		m_leftChild = new TreeNode(*(other.getLeftChild()));
		m_leftChild->setParent(this);
	} else {
		m_leftChild = NULL;
	}

	if (other.getRightChild() != NULL)
	{
		m_rightChild = new TreeNode(*(other.getRightChild()));
		m_rightChild->setParent(this);
	}
	else {
		m_rightChild = NULL;
	}
}


TreeNode::~TreeNode()
{
	delete m_leftChild;
	delete m_rightChild;
}


bool TreeNode::checkValidity(int min, int max)
{
	if ((min > m_minValue) || (m_minValue > m_maxValue) || (m_maxValue > max))
	{
		cout << "error in node:"
			<< "minValue=" << m_minValue
			<< ", maxValue=" << m_maxValue
			<< ", min=" << min
			<< ", max=" << max << endl;
		return false;
	}
	if ((m_leftChild != NULL) && (!m_leftChild->checkValidity(min, m_minValue)))
	{
		return false;
	}
	if ((m_rightChild != NULL) && (!m_rightChild->checkValidity(m_maxValue, max)))
	{
		return false;
	}
	return true;
}


void TreeNode::join(TreeNode* a_other)
{
	if (a_other == NULL)
	{
		return;
	}

	if (m_minValue > a_other->getMinValue())
	{
		m_minValue = a_other->getMinValue();
	}

	if (m_maxValue < a_other->getMaxValue())
	{
		m_maxValue = a_other->getMaxValue();
	}

	const NameSet& otherNameSet = a_other->getNameSet();
	m_nameSet.insert(otherNameSet.begin(), otherNameSet.end());

	TreeNode* otherLeft = a_other->getLeftChild();
	if (otherLeft != NULL)
	{
		if (m_leftChild != NULL)
		{
			m_leftChild->join(otherLeft);
			m_leftChild->setParent(this);
		}
		else {
			m_leftChild = new TreeNode(*otherLeft);
		}
	}

	TreeNode* otherRight = a_other->getRightChild();
	if (otherRight != NULL)
	{
		if (m_rightChild != NULL)
		{
			m_rightChild->join(otherRight);
			m_rightChild->setParent(this);
		}
		else {
			m_rightChild = new TreeNode(*otherRight);
		}
	}
}


void TreeNode::addName(const string& a_name)
{
	m_nameSet.insert(a_name);
}


bool TreeNode::getUniqueName(string& a_name)
{
	if (m_nameSet.size() == 1)
	{
		a_name = *(m_nameSet.begin());
		return true;
	}

	return false;
}


void TreeNode::printTree()
{
	cout << "[";
	if (m_leftChild != NULL)
	{
		m_leftChild->printTree();
	}
	cout << "]";

	for (NameSetConstIter iter = m_nameSet.begin(); iter != m_nameSet.end(); iter++)
	{
		cout << *iter << ",";
	}

	cout << m_minValue << "," << m_maxValue;

	cout << "[";
	if (m_rightChild != NULL)
	{
		m_rightChild->printTree();
	}
	cout << "]";
}
