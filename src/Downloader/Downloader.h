#ifndef DOWNLOADER_H
#define DOWNLOADER_H

using System;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

using namespace std;

namespace IPDL {

  enum DownloadStatus { Complete, Incomplete, AlreadyExists, Unavailable, Error };

  /**
   * A downloader of a single file,
   * which sends notifications of progress via callbacks.
   */
  class Downloader {

  private: // Private members

    /**
     * A container for cookies.
     * Contains many cookies.
     */
    CookieContainer cookies;

    static int BufferSize = 0x10000;

  public: // Public methods

    /**
     * A constructor.
     * Create a downloader with no arguments:
     * call Download() to use it on a single file.
     */
    Downloader();

    /**
     * Download a specified BBC program to a file.
     *
     * @param pid      the unique identifier of the program to download
     * @param atStart  callback to be notified that the download has started
     * @param progress callback to be notified of a change in how much has downloaded
     * @param atEnd    callback to be notified that the download has finished
     */
    void Download(string pid,
									void (atStart*)(string filename),
									void (progress*)(int bytesFetched, int bytesTotal),
									void (atEnd*)(DownloadStatus status, string message)
									);


  private: // Private methods

    Playlist GetPlaylist(string pid);

    string GetTitle(string pid);

    IphonePage GetIphonePage(string pid);

    string FilenameSafe(string text);

    void ReadFromStream(Stream stream, Action<byte[], int> handler);
  };

}

#endif // DOWNLOADER_H
