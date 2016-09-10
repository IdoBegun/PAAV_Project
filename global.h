#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "Function.h"
#include "TreeNode.h"
#include <string>
#include <vector>

#define DEBUG_LEVEL 0 // 0 - only errors and general information, 1 - verbose, 2 - debug

using namespace std;

void removeChar(string& a_str, char a_char);
void replaceChar(string& a_str, char a_charOld, char a_charNew);
vector<string> splitByDelim(string& a_str, char a_delim);

void log(const string& a_str);
void error(const string& a_str, bool a_exit);
void verbose(const string& a_str);
void debug(const string& a_str);

#ifndef CPP11
static string to_string(int num) // implementation of the c++11 function
{
  stringstream s;
  s<<num;
  return s.str();
}

static int stoi(string str) // implementation of the c++11 function
{
  stringstream ss(str);
  int N;
  ss<<str;
  ss>>N;
  return N;
}
#endif
#endif
