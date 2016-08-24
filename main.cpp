#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>      // std::filebuf
#include "Cfg.h"

int main(int argc, char** argv)
{
  char* filePath = argv[1];
  cout << filePath;
  filebuf fb;
  if (fb.open (filePath,ios::in))
  {
    istream infile(&fb);
    Cfg tree(infile);
    tree.runProgram();
    if (tree.checkValidity())
    {
      cout << "the program is valid";
    }
    else
    {
      cout << "the program can be not valid";
    }
    fb.close();
  }
  else
  {
    cout << "can't open the file: " << filePath;
  }
  return 0;
}
