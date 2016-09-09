#include <iostream>
#include <assert.h>
#include "Function.h"
#include "StringUtils.h"
#include "global.h"


Function::Function() :
	m_name(e_noneFunc)
{}

Function::Function(FunctionName a_name, const string& a_firstVar, const string& a_secondVar, int a_value):
	m_name(a_name), m_firstVar(a_firstVar), m_secondVar(a_secondVar), m_value(a_value)
{}

Function::Function(const string& a_str)
{
	// Remove all whitespaces, semicolons, right round brackets and left curly brackets
	string func = a_str;
	removeChar(func, ' ');
	removeChar(func, '\t');
	removeChar(func, ';');
	removeChar(func, ')');
	removeChar(func, '{');

	// Replace all left brackets with comas
	replaceChar(func, '(', ',');

	vector<string> funcElements = splitByDelim(func, ',');
	string& name = funcElements[0];

	if (name == CREATE_NODE)
	{
		m_name = e_createNode;
		m_firstVar = funcElements[1];
		m_value = stoi(funcElements[2]);
		m_secondVar = "";
	}
	else if (name == SET_LEFT)
	{
    m_name = e_setLeft;
		m_firstVar = funcElements[1];
		m_secondVar = funcElements[2];
		m_value = 0;
	}
	else if (name == SET_RIGHT)
	{
		m_name = e_setRight;
		m_firstVar = funcElements[1];
		m_secondVar = funcElements[2];
		m_value = 0;
	}
	else if (name == SET_VALUE)
	{
		m_firstVar = funcElements[1];
		m_secondVar = "";

		string& exp = funcElements[1];
		size_t pos = exp.find(PLUS);

		if (pos != string::npos)
		{
			m_name = e_increment;
			string var = m_firstVar = exp.substr(0, pos);
			if (var != m_firstVar)
			{
        cout << "Function ctor - parsing problem1" << endl;
        assert(false);
			}
			m_value = stoi(exp.substr(pos + 1, string::npos));
		}
		else
		{
			pos = exp.find(MINUS);
			if (pos != string::npos)
			{
				m_name = e_decrement;
				string var = m_firstVar = exp.substr(0, pos);
				if (var != m_firstVar)
				{
          cout << "Function ctor - parsing problem2" << endl;
          assert(false);
				}
				m_value = stoi(exp.substr(pos + 1, string::npos));
			}
			else
			{
				m_name = e_setValue;
				m_value = stoi(funcElements[2]);
			}
		}
	}
	else if ((name == IF) || (name == WHILE))
	{
		string& exp = funcElements[1];
		size_t pos = exp.find(GREATER_EQUAL);
		if (pos != string::npos)
		{
			m_name = e_greaterEqual;
			m_firstVar = exp.substr(0, pos);
			m_value = stoi(exp.substr(pos + 2, string::npos));
			m_secondVar = "";
		}
		else
		{
			pos = exp.find(GREATER);
			if (pos != string::npos)
			{
				m_name = e_greater;
				m_firstVar = exp.substr(0, pos);
				m_value = stoi(exp.substr(pos + 1, string::npos));
				m_secondVar = "";
			}
			else
			{
				pos = exp.find(LESS_EQUAL);
				if (pos != string::npos)
				{
					m_name = e_lessEqual;
					m_firstVar = exp.substr(0, pos);
					m_value = stoi(exp.substr(pos + 2, string::npos));
					m_secondVar = "";
				}
				else
				{
					pos = exp.find(LESS);
					if (pos != string::npos)
					{
						m_name = e_less;
						m_firstVar = exp.substr(0, pos);
						m_value = stoi(exp.substr(pos + 1, string::npos));
						m_secondVar = "";
					}
				}
			}
		}
	}

	debug("Function::CTOR - building from" + a_str);
}


bool Function::isConditional()
{
	switch(m_name)
	{
	case(e_less):
	case(e_lessEqual):
	case(e_greater):
	case(e_greaterEqual):
		return true;
	default:
		return false;
	}
}


Function Function::invertFunction()
{
	if (!isConditional())
	{
    cout << "Function::invertFunction - not conditional" << endl;
    assert(false); // maybe just return (*this);
	}

	switch (m_name)
	{
	case(e_less):
		return Function(e_greaterEqual, m_firstVar, m_secondVar, m_value);
	case(e_lessEqual):
		return Function(e_greater, m_firstVar, m_secondVar, m_value);
	case(e_greater):
		return Function(e_lessEqual, m_firstVar, m_secondVar, m_value);
	case(e_greaterEqual):
		return Function(e_less, m_firstVar, m_secondVar, m_value);
	default:
    cout << "Function::invertFunction - got to default case - " << m_name << endl;
    assert(false); // maybe just return (*this);	
	}
  return (*this); // in case we remove the assert - get here
}

void Function::printFunction()
{
	cout << "Function::printFunction - Printing function:";
	cout << m_name << "," << m_firstVar << "," << m_secondVar << "," << m_value << endl;
}

void Function::debug(const string& a_message)
{
#if DEBUG>0
	cout << a_message << endl;
	printFunction();
#endif
}
