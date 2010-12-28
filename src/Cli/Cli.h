#ifndef CLI_H
#define CLI_H

using System;
using System.IO;
using Mono.Options;
using System.Reflection;


#include "Downloader.h"

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
    /* Given a programme identifier, download it.
       This will pass the following three methods to this->downloader
       for it to give us updates on the download's status.
     */

    void DownloadStart(string filename);
    // Be notified that a download with name `filename` has started

    void DownloadProgress(int bytesDownloaded, int total);
    // Be notified that we've downloaded `bytesDownloaded` bytes of the total `total`

    void DownloadEnd(DownloadStatus status, string message);
    // Be notified that the download name `message` ended with status `status`

  }; // class Cli

} // namespace IPDL

#endif // CLI_H
