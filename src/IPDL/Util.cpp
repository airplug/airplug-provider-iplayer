#include "Util.h" // Header file for this implementation

#include <iostream>
// stringstream

#include <math.h>
// floor, log10, pow

#include <boost/xpressive/xpressive.hpp>
// Regular expressions


using namespace std;
using namespace boost::xpressive;


namespace IPDL {
  namespace Util {

    string SIFormat(int number, string units) {

			stringstream out;
			// Write to this, return .str()

      if (number < 1000) {
				out << number << units;
        
			} else {

				int k = static_cast<int>( floor(log10(number) / 3) );
				// "{0:0.00} {1}{2}"
				out << number / pow(1000, k)
						<< SISuffixes[k]
						<< units;
			}

			return out.str();
    }

    string ExtractPid(string url) {
			sregex pid_regex = sregex::compile("(?:/iplayer/episode/|/programmes/|\b)(b[a-z0-9]{7})\b");
			smatch pid_match;
			if (regex_match(url, pid_match, pid_regex)) {
				return pid_match[0];
			} else {
				return "";
			}
    }
  }
}
