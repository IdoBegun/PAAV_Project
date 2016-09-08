#ifndef _STRINGUTILS_H
#define _STRINGUTILS_H

#include <string>
#include <vector>

using namespace std;

void removeChar(string& a_str, char a_char);
void replaceChar(string& a_str, char a_charOld, char a_charNew);
vector<string> splitByDelim(string& a_str, char a_delim);
vector<string> funcToElements(string a_str);

#endif
