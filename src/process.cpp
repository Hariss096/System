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


Process::Process(const int pid, long int uptime) : pid_(pid), uptime_(uptime) {
// long uptime = LinuxParser::UpTime();

    vector<float> procData = LinuxParser::Cpu(pid_);
    if (procData.size() == 5) {
        float total_time = procData[procInfoTimes::utime_] + procData[procInfoTimes::stime_] +
                                procData[procInfoTimes::cutime_] + procData[procInfoTimes::cstime_];
        float seconds = uptime_ - procData[procInfoTimes::starttime_];
        cpuUse_ = (total_time / sysconf(_SC_CLK_TCK)) / seconds);
    } else {
        cpuUse_ = 0;
    }
}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuUse_; }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return (cpuUse_ < a.cpuUse_); 
}

