#ifndef _TREENODE_H
#define _TREENODE_H

#include <sstream>
#include <string>
#include <set>

using namespace std;

typedef set<string> NameSet;
typedef NameSet::iterator NameSetIter;
typedef NameSet::const_iterator NameSetConstIter;

class TreeNode
{
public:
	TreeNode(string a_name, TreeNode* a_parent, TreeNode* a_leftChild, TreeNode* a_rightChild, int a_minValue, int a_maxValue);
	TreeNode(const TreeNode& other); // Deep CTOR
	~TreeNode(); // Deep DTOR

	// Getters
	const NameSet& getNameSet() const { return m_nameSet; };
	TreeNode* getParent() const { return m_parent; };
	TreeNode* getLeftChild() const { return m_leftChild; };
	TreeNode* getRightChild() const { return m_rightChild; };
	int getMinValue() const { return m_minValue; };
	int getMaxValue() const { return m_maxValue; };

	// Setters
	void setParent(TreeNode* a_parent) { m_parent = a_parent; };
	void setLeftChild(TreeNode* a_leftChild) { m_leftChild = a_leftChild; };
	void setRightChild(TreeNode* a_rightChild) { m_rightChild = a_rightChild; };
	void setMinValue(int a_minValue) { m_minValue = a_minValue; };
	void setMaxValue(int a_maxValue) { m_maxValue = a_maxValue; };

	void addName(const string& a_name);
	bool getUniqueName(string& a_name);
	bool checkValidity(int a_min, int a_max);
	void join(TreeNode* a_other);

	TreeNode* getRoot();

	void printTree();
	operator string() const;

private:
  static string to_string(int num)
  {
    stringstream s;
    s<<num;
    return s.str();
  }

	NameSet m_nameSet;
	TreeNode* m_parent;
	TreeNode* m_leftChild;
	TreeNode* m_rightChild;
	int m_minValue;
	int m_maxValue;
};


#endif
