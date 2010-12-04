#ifndef UTIL_H
#define UTIL_H

#include <string>

using namespace std;

namespace IPDL {
  namespace Util {

    string SISuffixes[] = { "", "K", "M", "G", "T", "P", "E", "Z", "Y" };

		string SIFormat(int number, string units);
		/* Format `number` in SI format, appending `units`.
			 e.g. SIFormat(10, "bytes") = "10 bytes"
		 */

    string ExtractPid(string url);
		/* Given a URL, extract a URL from it if possible, or NULL if not
		 */

  }
}

#endif // UTIL_H
