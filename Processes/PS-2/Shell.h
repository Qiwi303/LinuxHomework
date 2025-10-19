#pragma once

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <sys/types.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <climits>
#include <stdexcept>

class SysCallError: public std::runtime_error{
public:
	explicit SysCallError(const std::string& msg):
        	std::runtime_error(msg) {}
 
};
  
class Shell{
public:
  
          void redirectOut(const char path[], bool Append);
          void checkSysError(const int num, const std::string& error);
          void readAndSplit();
          void execCommand(std::vector<char*>& args);
          void splitAndCallCommands();
          void runShell();
  
  
private:
          std::vector<char*> argv;
          std::vector<std::string> tokens;
          bool finished = false;
          int lastExitCode = 0;
};


