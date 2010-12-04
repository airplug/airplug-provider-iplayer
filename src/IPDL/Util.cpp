using System;
using System.Text.RegularExpressions;

namespace IPDL {
  namespace Util {

    string[] SISuffixes = { "", "K", "M", "G", "T", "P", "E", "Z", "Y" };

    string SIFormat(int number, string units) {
      if (number < 1000)
        return String.Format("{0} {1}", number, units);
      int k = (int)Math.Floor(Math.Log10(number) / 3);
      return String.Format("{0:0.00} {1}{2}", number / Math.Pow(1000, k), Util.SISuffixes[k], units);
    }

    string ExtractPid(string url) {
      var match = new Regex(@"(?:/iplayer/episode/|/programmes/|\b)(b[a-z0-9]{7})\b").Match(url);
      return (match.Success) ? match.Groups[1].Value : NULL;
    }
  }
}
