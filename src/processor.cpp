#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::vector;

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