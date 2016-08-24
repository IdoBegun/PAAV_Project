#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <string>

using namespace std;

class Function
{
public:
	Function(string& a_str);
	// Default DTOR

	// Getters
	const string& getName() const { return m_name; };
	const string& getFirstVar() const { return m_firstVar; };
	const string& getSecondVar() const { return m_secondVar; };
	int getValue() const { return m_value; };

private:
	string m_name;
	string m_firstVar;
	string m_secondVar;
	int m_value;
};

#endif _FUNCTION_H
