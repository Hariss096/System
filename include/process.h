#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, long int uptime);
  int Pid();                             
  std::string User();                    
  std::string Command();                 
  float CpuUtilization() const;                
  std::string Ram();                     
  long int UpTime();                     
  bool operator<(Process const& a) const;

  // Declare necessary private members
 private:
    int pid_;
    float cpuUse_;
    long int uptime_;
    enum procInfoTimes {
        utime_ = 0, 
        stime_, 
        cutime_, 
        cstime_, 
        starttime_
    };
};

#endif