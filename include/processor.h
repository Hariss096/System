#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();

  // Declare necessary private members
 private:
  // to track sum of user, nice, system, idle, iowait, irq, softirq, steal
  float prevTotalTime_;
  // previous idle time
  float prevIdleTime_;

  // helper function to convert string vector into long vector
  std::vector<long> convertStrToLong(std::vector<std::string> values);
};

#endif