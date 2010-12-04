#ifndef IPHONEPAGE_H
#define IPHONEPAGE_H

using System.Text.RegularExpressions;
using System.IO;

namespace IPDL {
  class IphonePage {

	private: // Data members

    static string urlPrefix = "http://www.bbc.co.uk/mobile/iplayer/episode/";

    string document;
		// The XML parse tree


	public: // Public methods

    static string Url(string pid);

    IphonePage(string source);
    IphonePage(Stream source);

    string get_EmbeddedMediaUrl();

    bool get_IsAvailable();

    string get_FileExtension();

	private: // Private methods

    void Init(string source) {
      this.document = source;
    }

  };
}

#endif // IPHONEPAGE_H
