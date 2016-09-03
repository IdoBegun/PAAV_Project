#include "Function.h"
#include "StringUtils.h"

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
		m_secondVar = funcElements[2];
		m_value = 0;
	}
	else if (name == SET_LEFT)
	{
		m_name = e_createNode;
		m_firstVar = funcElements[1];
		m_secondVar = funcElements[2];
		m_value = 0;
	}
	else if (name == SET_RIGHT)
	{
		m_name = e_createNode;
		m_firstVar = funcElements[1];
		m_secondVar = funcElements[2];
		m_value = 0;
	}
	else if (name == SET_VALUE)
	{
		m_name = e_setValue;
		m_firstVar = funcElements[1];
		m_value = stoi(funcElements[2]);
		m_secondVar = "";
	}
	else if (name == IF)
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
		// ERROR?
		return (*this);
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
		// ERROR?
		return (*this);
	}
}