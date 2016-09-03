#include "Function.h"
#include "StringUtils.h"

Function::Function(FunctionName a_name, const string& a_firstVar, const string& a_secondVar, int a_value):
	m_name(a_name), m_firstVar(a_firstVar), m_secondVar(a_secondVar), m_value(a_value)
{}


Function::Function(const string& a_str)
{
	// TODO
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