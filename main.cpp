#include <iostream>     // std::ios, std::istream, std::
#include <sstream>
#include <fstream>      // std::filebuf
#include "Cfg.h"
#include "global.h"

int main(int argc, char** argv)
{
  char* filePath = argv[1];
  stringstream ss;
  ss << "Opening file: " << filePath;
  log(ss.str());
  filebuf fb;
  if (fb.open (filePath,ios::in))
  {
    istream infile(&fb);
	log("Creating control flow graph...");
    Cfg graph(infile);
	log("Computing program states...");
    graph.runProgram();
	log("Checking program validity...");
    if (graph.checkValidity())
    {
		log("The program is valid!");
    }
    else
    {
		log("The program can be invalid!");
    }
    graph.deleteCfg();
    fb.close();
  }
  else
  {
	  ss.str("");
	  ss.clear();
	  ss << "Can't open the file: " << filePath;
	  error(ss.str(), true);
  }
  return 0;
}
