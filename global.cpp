#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>      // std::filebuf
#include <sstream>
#include <string>
#include <algorithm>
#include <new>
#include <limits.h>
#include <vector>
#include "global.h"

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


void log(const string& a_str)
{
	cout << " -I- " << a_str << endl;
}


void error(const string& a_str, bool a_exit)
{
	if (a_exit)
	{
		cout << " -E - Fatal error encountered!" << endl;
	}
	cout << " -E- " << a_str << endl;
	if (a_exit)
	{
		exit(1);
	}
}


void verbose(const string& a_str)
{
#if DEBUG_LEVEL>0
	cout << " -V- " << a_str << endl;
#endif
}


void debug(const string& a_str)
{
#if DEBUG_LEVEL>1
	cout << " -D- " << a_str << endl;
#endif
}