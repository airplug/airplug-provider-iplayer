using System;
using System.IO;
using Mono.Options;
using System.Reflection;

#include "Cli.h"

namespace IPDL {
  class Cli {

	private: // Data members
		       // ------------

    Downloader &downloader;
    bool succeeded, quiet;
    string lastOutput = "";


	public: // Public methods
		      // --------------

		// Constructors
    Cli(Downloader &downloader);
		Cli();

    void Run(string[] args);

    bool get_succeeded();
		// Just get the value of private member `succeeded`


	private: // Private methods
           // ---------------

    void ShowVersion();

    void ShowHelp();

    Assembly get_Assembly();

    void Download(string identifier);

    void DownloadStart(string filename);

    void DownloadProgress(int bytesDownloaded, int total);

    void DownloadEnd(DownloadStatus status, string message);

  }; // class Cli

} // namespace IPDL
