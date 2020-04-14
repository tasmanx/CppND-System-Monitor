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

Process::Process(int pid) : pid_(pid) {
  User();                      
  Command();                   
  CpuUtilization();                  
  Ram();                       
  UpTime();   
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
  cpu_utilization_ = cpu_.Utilization(pid_);
  return cpu_utilization_; 
}

string Process::Command() { 
  if (command_.empty())
    command_ = LinuxParser::Command(pid_);
  return command_; 
}

// Return ram in MB
string Process::Ram() { 
  ram_ = std::to_string(LinuxParser::Ram(pid_) / 1024);
  return ram_;
}

string Process::User() { 
  if(user_.empty())
    user_ = LinuxParser::User(pid_);
  return user_;
}

long int Process::UpTime() { 
  up_time_ = LinuxParser::UpTime(pid_);
  return up_time_; 
}

bool Process::operator<(Process const& a) const { 
  return cpu_utilization_ > a.cpu_utilization_;
}