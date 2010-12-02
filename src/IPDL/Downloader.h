

using System;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

namespace IPDL {

  enum DownloadStatus { Complete, Incomplete, AlreadyExists, Unavailable, Error };

  class Downloader {

  private: // Private members

    CookieContainer cookies;
    static int BufferSize = 0x10000;

  public: // Public methods

    // Constructors
    Downloader();
      
    // Delegates ...
    void AtStartHandler(string filename);
    void ProgressHandler(int bytesFetched, int bytesTotal);
    void AtEndHandler(DownloadStatus status, string message);

    // THE BIG ONE
    void Download(string pid,
		  AtStartHandler atStart,
		  ProgressHandler progress,
		  AtEndHandler atEnd);


  private: // Private methods

    Playlist GetPlaylist(string pid);

    string GetTitle(string pid);

    IphonePage GetIphonePage(string pid);

    string FilenameSafe(string text);

    void ReadFromStream(Stream stream, Action<byte[], int> handler);
  };

}
