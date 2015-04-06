#include "string"
#include "map"

std::map<std::string, int> commandLineArguments(int argc, char *argv[])
{
  std::map<std::string, int> cmdMap;
  for (unsigned int i=1; i<argc; i+=2) 
  {
    cmdMap[std::string(argv[i])]=atoi(argv[i+1]);
  }
  return cmdMap;
}
