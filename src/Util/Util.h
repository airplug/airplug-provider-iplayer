// Copyright 2010 the airplug team

#ifndef SRC_UTIL_UTIL_H_
#define SRC_UTIL_UTIL_H_

#include <string>

namespace IPDL {
  namespace Util {

    std::string SISuffixes[] = { "", "K", "M", "G", "T", "P", "E", "Z", "Y" };

		/**
     * Format an integer to an SI-format string representation,
     * appending a string that specifies the units.
     * Example: SIFormat(10, "bytes") = "10 bytes"
     *
     * @param number  the integer to format
     * @param units   a string, specifying the unit, to append
     * @return        the SI representation
		 */
    std::string SIFormat(int number, std::string units);


		/**
     * Given a URL, extract a PID from it.
     * If no PID can be found, return NULL.
     *
     * @param url  The URL to extract a PID from
     * @return     A pointer to the PID as a string, or NULL if none found
		 */
    std::string *ExtractPid(std::string url);
  }
}

#endif  // SRC_UTIL_UTIL_H_
