#include <string>

#include "format.h"

using std::string;

// Convert seconds to HH:MM:SS format
string Format::ElapsedTime(long seconds) { 
  int hours = 0;
  int minutes = 0;

  hours = seconds/3600;
  seconds = seconds%3600;
  minutes = seconds/60;
  seconds = seconds%60;
  
  // If hours, minutes & seconds contain only single digit add '0' in front
  string hours_string = hours > 9 ? std::to_string(hours) : "0" + std::to_string(hours);
  string minutes_string = minutes > 9 ? std::to_string(minutes) : "0" + std::to_string(minutes);
  string seconds_string = seconds > 9 ? std::to_string(seconds) : "0" + std::to_string(seconds);

  return hours_string + ":" + minutes_string + ":" + seconds_string;
}