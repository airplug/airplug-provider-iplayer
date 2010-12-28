// Copyright 2010 the airplug team

#include <iostream>  // stringstream
#include <math.h>    // floor, log10, pow
#include <pcre.h>    // Regular expressions

// Header file for this implementation
#include "./Util.h"


namespace IPDL {
namespace Util {

std::string SIFormat(int number, std::string units) {
  std::string SISuffixes[] = { "", "K", "M", "G", "T", "P", "E", "Z", "Y" };

  std::stringstream out;
  // Write to this, return .str()

  if (number < 1000) {
    out << number;
  } else {
    int k = static_cast<int>(floor(log10(number) / 3));

    out << number / pow(1000, k)
        << SISuffixes[k];
  }

  out << " " << units;
  return out.str();
}

std::string ExtractPid(std::string url) {
  char const * const url_c_string = url.c_str();

  char const * const error;  // Error message goes here if the regex is invalid
  int errcode;               // Error code goes here if invalid
  int erroffset;             // Position of error if invalid

  // Compile the regular expression
  pcre* automaton = pcre_compile2(
      "(?:/iplayer/episode/|/programmes/|\b)(b[a-z0-9]{7})\b",
      0, &errcode, &error, &erroffset, NULL);

  // "Study" the regex: gives optimization info
  pcre_extra *studied = pcre_study(automaton, 0, &error);

  int ovector[2];   // Match info goes in here

  // Execute the regex on the line of input
  int result = pcre_exec(automaton, studied,
                         url_c_string, strlen(url_c_string),
                         0,            // Start offset
                         0,            // Options
                         ovector, 2);  // Put result, if any, here
  
  if (result == 0) return NULL;

  // If we're here, we have a result

  int const pid_length = ovector[1] - ovector[0];
  char * const pid_c_string = new char[pid_length + 1];
  strcpy(pid_c_string, url_c_string[ovector[0]]);
  std::string pid(pid_c_string);  // We'll return this
  delete pid_c_string;

  return pid;
}
}  // Util
}  // IPDL
