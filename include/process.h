#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) {}
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
   int pid_ = {}; 
   std::string user_ = {};
   std::string command_ = {};
};

#endif