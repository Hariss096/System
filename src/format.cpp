#include <string>
#include <cmath>
#include <sstream>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hours, minutes, secs;
    hours = std::trunc(seconds/3600);
    minutes = std::trunc((seconds - hours*3600)/60);
    secs = seconds - hours*3600 - minutes*60;
    std::stringstream stream;
    stream << hours << ":" << minutes << ":" << secs;
    return stream.str(); 
    }