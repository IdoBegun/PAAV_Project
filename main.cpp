#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>      // std::filebuf
#include "Cfg.h"

int main(int argc, char** argv)
{
  char* filePath = argv[1];
  cout << filePath << endl;
  filebuf fb;
  if (fb.open (filePath,ios::in))
  {
    istream infile(&fb);
    Cfg graph(infile);
    /*graph.runProgram();
    if (graph.checkValidity())
    {
      cout << "the program is valid" << endl;
    }
    else
    {
      cout << "the program can be not valid" << endl;
    }*/
    graph.deleteCfg();
    fb.close();
  }
  else
  {
    cout << "can't open the file: " << filePath << endl;
  }
  return 0;
}
