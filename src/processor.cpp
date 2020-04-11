#include <vector>
#include <string>
#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<int> cpu_time = LinuxParser::CpuUtilization();
  int total_time = 0;
  for (int time : cpu_time)
    total_time += time;
  float time_being_in_idle = cpu_time[3] / (float)total_time;
  float time_not_in_idle = 1.0 - time_being_in_idle;
  return time_not_in_idle;
}

float Processor::Utilization(int pid) {
  vector<string> proc_stat_file = LinuxParser::ProcStatFile(pid);
  long utime = std::stol(proc_stat_file[13]);
  long stime = std::stol(proc_stat_file[14]);
  long cutime = std::stol(proc_stat_file[15]);
  long cstime = std::stol(proc_stat_file[16]);
  long starttime = std::stol(proc_stat_file[21]);
  long total_time = utime + stime;
  total_time = total_time + cutime + cstime;
  float seconds = LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK));
  return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
}