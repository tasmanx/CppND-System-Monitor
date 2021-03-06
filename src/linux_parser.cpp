#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// We can update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Return mem_used division by mem_total
float LinuxParser::MemoryUtilization() { 
  int mem_total = 0;
  int mem_free = 0;
  int buffers = 0;
  int cached = 0;
  int slab = 0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    string line, key;
    int value;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:")
        mem_total = value;
      else if (key == "MemFree:")
        mem_free = value;
      else if (key == "Buffers:")
        buffers = value;
      else if (key == "Cached:")
        cached = value;
      else if (key == "Slab:")
        slab = value;
      else if (mem_total != 0 && mem_free != 0 && buffers != 0 && cached != 0 && slab != 0)
        break;
    }
  }
  int mem_used = mem_total - mem_free - buffers - cached - slab; 
  return mem_used / (float)mem_total;
}

long LinuxParser::UpTime() { 
  long value;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}

vector<int> LinuxParser::CpuUtilization() {
  vector<int> cpu_time;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string tmp;
    linestream >> tmp;  // Skip cpu string
    while (linestream >> value)
      cpu_time.push_back(value);
  }
  return cpu_time;
}

int LinuxParser::TotalProcesses() {
  int value;
  string key; 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line); 
      linestream >> key >> value;
      if (key == "processes")
        return value;
    }
  }
  return value;
}

int LinuxParser::RunningProcesses() { 
  int value;
  string key; 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line); 
      linestream >> key >> value;
      if (key == "procs_running")
        return value;
    }
  }
  return value;
}

string LinuxParser::Command(int pid) { 
  string line; 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return line;
}

// Return memory utilization in KB
int LinuxParser::Ram(int pid) {
  string key;
  int value; 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line); 
      linestream >> key >> value;
      if (key == "VmSize:")
        return value;
    }
  }
  return value;
}

string LinuxParser::Uid(int pid) { 
  string key, value; 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line); 
      linestream >> key >> value;
      if (key == "Uid:")
        return value;
    }
  }
  return value;
}

string LinuxParser::User(int pid) { 
  string line;
  string user, x, u_id;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> u_id;
        if (u_id == Uid(pid))
          return user;
    }
  }
  return user;
}

// Return process UpTime in seconds
long LinuxParser::UpTime(int pid) {
  vector<string> proc_stat_file = ProcStatFile(pid);
  long start_time = std::stol(proc_stat_file[21]);
  return UpTime() - (start_time / sysconf(_SC_CLK_TCK));
}

vector<string> LinuxParser::ProcStatFile(int pid) {
  vector<string> proc_stat_file;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);
    std::istringstream linestream(line); 
    while (linestream >> value)
      proc_stat_file.push_back(value);
  }
  return proc_stat_file;
} 