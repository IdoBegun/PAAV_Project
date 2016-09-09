#ifndef _FUNCTION_H
#define _FUNCTION_H

#define CREATE_NODE "createNode"
#define SET_VALUE "setValue"
#define SET_RIGHT "setRight"
#define SET_LEFT "setLeft"
#define IF "if"
#define WHILE "while"
#define GREATER_EQUAL ">="
#define GREATER ">"
#define LESS_EQUAL "<="
#define LESS "<"
#define PLUS "+"
#define MINUS "-"

#include <string>
#include <sstream>

using namespace std;

enum FunctionName {
	e_noneFunc,
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
  Function();
  Function(const string& a_str);
	Function(FunctionName a_name, const string& a_firstVar, const string& a_secondVar, int a_value);

	// Getters
	FunctionName getName() const { return m_name; };
	const string& getFirstVar() const { return m_firstVar; };
	const string& getSecondVar() const { return m_secondVar; };
	int getValue() const { return m_value; };
	bool isConditional();
	
	// Only relevant for conditional functions
	Function invertFunction();

	void printFunction();

private:
  int stoi(string str) // implementation of the c++11 function
  {
    stringstream ss(str);
    int N;
    ss<<str;
    ss>>N;
    return N;
  }

	void debug(const string& a_message);

	FunctionName m_name;
	string m_firstVar;
	string m_secondVar;
	int m_value;
};

#endif
