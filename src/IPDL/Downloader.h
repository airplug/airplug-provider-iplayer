#ifndef DOWNLOADER_H
#define DOWNLOADER_H

using System;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

using namespace std;

namespace IPDL {

  enum DownloadStatus { Complete, Incomplete, AlreadyExists, Unavailable, Error };

  class Downloader {

  private: // Private members

    CookieContainer cookies;
    static int BufferSize = 0x10000;

  public: // Public methods

    // Constructors
    Downloader();

    // THE BIG ONE
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
