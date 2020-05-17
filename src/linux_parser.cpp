#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
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

// BONUS: Update this to use std::filesystem
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float totalMemory, freeMemory;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          totalMemory = stof(value);
        }
        if (key == "MemFree") {
          freeMemory = stof(value);
        }
      }
    }
  }

  float usedMemory = (totalMemory - freeMemory)/totalMemory;
  return usedMemory; 
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long seconds; 
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> seconds;
  }
  return seconds; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  
  return 0; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    std::vector<std::string> cpu_util{std::istream_iterator<string>{lineStream}, 
    std::istream_iterator<string>{}};
    cpu_util.erase(cpu_util.begin());
    return cpu_util; 
  }
  return {}; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string key, value;
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return std::stoi(value);
      }
    }
  }
  return 0; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string key, value;
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return std::stoi(value);
      }
    }
  }
  return 0; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  string tag, value;
  if (stream.is_open()) {
    std::getline(stream, line); 
    return line;
  }
  return string(); 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  std::string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string key, value;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if (key == "VmSize:") {
        const long kb_ram = std::stol(value);
        const auto mb_ram = kb_ram / 1000;
        return std::to_string(mb_ram);
      }
    }
  }
  return string(); 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if (key == "Uid:")
        return value;
    }
  }
  return string(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  std::string line;
  std::string username, x, user_id;
  std::ifstream stream(LinuxParser::kPasswordPath);
  if (stream.is_open()){
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      lineStream >> username >> x >> user_id; 
      if (user_id == LinuxParser::Uid(pid)) {
        return username;
      }
    }
  }
  return string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  std::string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    std::vector<std::string> data{std::istream_iterator<string>{lineStream}, std::istream_iterator<string>{}};
    return LinuxParser::UpTime() - std::stol(data[21])/sysconf(_SC_CLK_TCK);
  }
  return 0; 
}

std::vector<float> LinuxParser::Cpu(const int pid) {
  std::vector<float> processInfo = {};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::string line;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    const std::vector<std::string> data {std::istream_iterator<string>{lineStream}, std::istream_iterator<string>{}};
    processInfo.push_back(std::stol(data[13]));
    processInfo.push_back(std::stol(data[14]));
    processInfo.push_back(std::stol(data[15]));
    processInfo.push_back(std::stol(data[16]));
    processInfo.push_back(std::stol(data[21]));
    return processInfo;
  }
  return {};
}