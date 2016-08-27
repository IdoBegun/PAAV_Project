#include <iostream>
#include "TreeNode.h"

TreeNode::TreeNode(string a_name, TreeNode* a_parent, TreeNode* a_leftChild, TreeNode* a_rightChild, int a_minValue, int a_maxValue) :
	m_name(a_name), m_parent(a_parent), m_leftChild(a_leftChild), m_rightChild(a_rightChild), m_minValue(a_minValue), m_maxValue(a_maxValue) {}

TreeNode::TreeNode(const TreeNode& other)
{
	m_name = other.getName();
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

TreeNode* TreeNode::findNode(string a_name)
{
	if (m_name == a_name)
	{
		return this;
	}
	
	if (m_leftChild != NULL)
	{
		TreeNode* result = m_leftChild->findNode(a_name);
		if (result != NULL)
		{
			return result;
		}
	}

	if (m_rightChild != NULL)
	{
		TreeNode* result = m_rightChild->findNode(a_name);
		if (result != NULL)
		{
			return result;
		}
	}

	return NULL;
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

	if (m_name != a_other->getName())
	{
		//TODO
	}

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

void TreeNode::meet(TreeNode* a_other)
{
	if (a_other == NULL)
	{
		return;
	}

	if (m_minValue < a_other->getMinValue())
	{
		m_minValue = a_other->getMinValue();
	}

	if (m_maxValue > a_other->getMaxValue())
	{
		m_maxValue = a_other->getMaxValue();
	}

	if (m_name != a_other->getName())
	{
		//TODO
	}

	TreeNode* otherLeft = a_other->getLeftChild();
	if (otherLeft != NULL)
	{
		if (m_leftChild != NULL)
		{
			m_leftChild->meet(otherLeft);
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
			m_rightChild->meet(otherRight);
			m_rightChild->setParent(this);
		}
		else {
			m_rightChild = new TreeNode(*otherRight);
		}
	}
}

