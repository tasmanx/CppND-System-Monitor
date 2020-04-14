#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "processor.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;

 private:
   int pid_ = {}; 
   std::string user_ = {};
   std::string command_ = {};
   float cpu_utilization_ = {};
   std::string ram_ = {};
   long int up_time_ = {};
   Processor cpu_ = {};
};

#endif