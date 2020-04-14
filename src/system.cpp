#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  if (!processes_.empty())
    processes_.clear();
  for (int pid : pids)
    processes_.emplace_back(pid);
  std ::sort(processes_.begin(), processes_.end());
  return processes_; 
}

std::string System::Kernel() { 
  if (kernel_.empty())
    kernel_ = LinuxParser::Kernel();
  return kernel_; 
}


float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { 
  if (operating_system_.empty())
    operating_system_ = LinuxParser::OperatingSystem();
  return operating_system_; 
}

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long System::UpTime() { return LinuxParser::UpTime(); }