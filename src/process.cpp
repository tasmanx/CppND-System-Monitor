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

float Process::CpuUtilization() { return cpu_.Utilization(pid_); }

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

long int Process::UpTime() { return LinuxParser::UpTime(); }

bool Process::operator<(Process const& a) const { 
  return pid_ > a.pid_;
}