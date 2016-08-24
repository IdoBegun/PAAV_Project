#include "Function.h"
#include "StringUtils.h"

Function::Function(string& a_str)
{
	vector<string> elements = funcToElements(a_str);
	m_name = elements[0];
	if (m_name == "createNode") {
		m_firstVar = elements[1];
		m_value = stoi(elements[2]);
		m_secondVar = elements[3];
	}
	else if (m_name == "setNode") {
		m_firstVar = elements[1];
		m_value = stoi(elements[2]);
		m_secondVar = string();
	}
	// else if ( ??? )
}
