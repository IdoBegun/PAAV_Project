#ifndef _FUNCTION_H
#define _FUNCTION_H

#define CREATE_NODE "createNode"
#define SET_VALUE "setValue"
#define SET_RIGHT "setRight"
#define SET_LEFT "setLeft"

#include <string>

using namespace std;

enum FunctionName {
	e_createNode,
	e_setLeft,
	e_setRight,
	e_setValue,
	e_less,
	e_lessEqual,
	e_greater,
	e_greaterEqual,
	e_increment,
	e_decrement
};

class Function
{
public:
	Function(FunctionName a_name, const string& a_firstVar, const string& a_secondVar, int a_value);
	Function(const string& a_str);
	// Default DTOR

	// Getters
	FunctionName getName() const { return m_name; };
	const string& getFirstVar() const { return m_firstVar; };
	const string& getSecondVar() const { return m_secondVar; };
	int getValue() const { return m_value; };
	bool isConditional();
	
	// Only relevant for conditional functions
	Function invertFunction();

private:
	FunctionName m_name;
	string m_firstVar;
	string m_secondVar;
	int m_value;
};

#endif _FUNCTION_H
