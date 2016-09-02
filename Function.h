#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <string>

using namespace std;

enum FunctionName {
	e_createNode,
	e_setLeftChild,
	e_setRightChild,
	e_setValue
};

class Function
{
public:
	Function(const string& a_str);
	// Default DTOR

	// Getters
	FunctionName getName() const { return m_name; };
	const string& getFirstVar() const { return m_firstVar; };
	const string& getSecondVar() const { return m_secondVar; };
	int getValue() const { return m_value; };

private:
	FunctionName m_name;
	string m_firstVar;
	string m_secondVar;
	int m_value;
};

#endif _FUNCTION_H
