#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

// Return CPU utilization
float Processor::Utilization() { 
  // read current cpu values
  vector<long> cpuStats = convertStrToLong(LinuxParser::CpuUtilization());
  // total cpu time since boot = user+nice+system+idle+iowait+irq+softirq+steal
  float totalTime =
      cpuStats[LinuxParser::kUser_] + cpuStats[LinuxParser::kNice_] +
      cpuStats[LinuxParser::kSystem_] + cpuStats[LinuxParser::kIdle_] +
      cpuStats[LinuxParser::kIOwait_] + cpuStats[LinuxParser::kIRQ_] +
      cpuStats[LinuxParser::kSoftIRQ_] + cpuStats[LinuxParser::kSteal_];
  // idle time since boot = idle + iowait
  float idleTime =
      cpuStats[LinuxParser::kIdle_] + cpuStats[LinuxParser::kIOwait_];

  // calculation of cpu usage since last update
  float diffIdle = idleTime - prevIdleTime_;
  float diffTotal = totalTime - prevTotalTime_;
  float diffUsage = (diffTotal - diffIdle) / diffTotal;
  // total and idle times for next check
  prevIdleTime_ = idleTime;
  prevTotalTime_ = totalTime;
  return diffUsage;
}

// convert string vector into long vector
vector<long> Processor::convertStrToLong(vector<string> values) {
  vector<long> convertedVal{};

  for (int it = 0; it < (int)values.size(); it++) {
    try {
      convertedVal.push_back(std::stol(values[it]));
    } catch (const std::invalid_argument& arg) {
      convertedVal.push_back((long)0);
    }
  }
  return convertedVal;


}