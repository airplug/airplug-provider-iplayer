#ifndef CLI_H
#define CLI_H

using System;
using System.IO;
using Mono.Options;
using System.Reflection;


namespace IPDL {
  class Cli {

  private: // Data members

    Downloader &downloader;
    bool succeeded, quiet;
    string lastOutput = "";


  public: // Public methods

    // Constructors
    Cli(Downloader &downloader);
    Cli();

    void Run(string[] args);
    /* Given the arguments to the main program,
       extract any/all programme identifiers,
       and Download() each of them.
     */

    bool get_succeeded();
    // Just get the value of private member `succeeded`


  private: // Private methods

    void ShowVersion();

    void ShowHelp();

    void Download(string identifier);
    // Given a programme identifier, download it

    void DownloadStart(string filename);

    void DownloadProgress(int bytesDownloaded, int total);

    void DownloadEnd(DownloadStatus status, string message);

  }; // class Cli

} // namespace IPDL

#endif // CLI_H
