#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

string Process::Command() { 
  if (command_.empty())
    command_ = LinuxParser::Command(pid_);
  return command_; 
}

// Return ram in MB
string Process::Ram() { return std::to_string(LinuxParser::Ram(pid_) / 1024); }

string Process::User() { 
  if(user_.empty())
    user_ = LinuxParser::User(pid_);
  return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }