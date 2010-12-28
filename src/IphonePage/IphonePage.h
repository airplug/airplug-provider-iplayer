#ifndef IPHONEPAGE_H
#define IPHONEPAGE_H

#include "IphonePage.h"

#include <string>

using namespace std;

namespace IPDL {
  class IphonePage {

	private: // Data members

    static string urlPrefix;

    string document;
		// The XML parse tree


	public: // Public methods

    static string Url(string pid);

    IphonePage(string source); // Initialize parse tree from string

    string get_EmbeddedMediaUrl();

    bool get_IsAvailable();

    string get_FileExtension();

	private: // Private methods

    void Init(string source);

  };
}

#endif // IPHONEPAGE_H
