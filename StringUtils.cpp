#include "StringUtils.h"
#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>      // std::filebuf
#include <sstream>
#include <string>
#include <algorithm>
#include <new>
#include <assert.h> 
#include <limits.h>
#include <vector>

using namespace std;

void removeChar(string& a_str, char a_char)
{
	a_str.erase(remove(a_str.begin(), a_str.end(), a_char), a_str.end());
}

void replaceChar(string& a_str, char a_charOld, char a_charNew)
{
	replace(a_str.begin(), a_str.end(), a_charOld, a_charNew);
}

vector<string> splitByDelim(string& a_str, char a_delim)
{
	vector<string> result;
	stringstream data(a_str);
	string line;
	while (getline(data, line, a_delim))
	{
		result.push_back(line);
	}

	return result;
}

vector<string> funcToElements(string a_str)
{
	removeChar(a_str, ';');
	removeChar(a_str, ')');
	replaceChar(a_str, '(', ',');
	return splitByDelim(a_str, ',');
}