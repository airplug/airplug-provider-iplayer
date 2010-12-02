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


  Downloader::Downloader() {
    this.cookies = new CookieContainer();
  }

  // Virtual solely so that we can subclass for testing, because mocks don't work with delegates.
  void Downloader::Download(string pid, AtStartHandler atStart, ProgressHandler progress, AtEndHandler atEnd) {
    var page = GetIphonePage(pid);

    if (!page.IsAvailable) {
      atEnd(DownloadStatus.Unavailable, pid);
      return;
    }

    var finalPath = FilenameSafe(GetTitle(pid)) + page.FileExtension;
    var tempPath  = finalPath + ".partial";

    if (File.Exists(finalPath)){
      atEnd(DownloadStatus.AlreadyExists, finalPath);
      return;
    }

    atStart(finalPath);

    var request       = new CoreMediaRequest(page.EmbeddedMediaUrl, cookies);
    var contentLength = request.ContentLength;
    int totalReceived = 0;

    using (var localStream = new FileStream(tempPath, FileMode.Append, FileAccess.Write, FileShare.Read)) {
      totalReceived = (int)localStream.Position;
      request.GetResponseStreamFromOffset(totalReceived, remoteStream => {
          ReadFromStream(remoteStream, (buffer, bytesRead) => {
	      localStream.Write(buffer, 0, bytesRead);
	      totalReceived += bytesRead;
	      progress(totalReceived, contentLength);
	    });
        });
    }

    if (totalReceived >= contentLength) {
      File.Move(tempPath, finalPath);
      atEnd(DownloadStatus.Complete, finalPath);
    } else {
      atEnd(DownloadStatus.Incomplete, tempPath);
    }
  }

  Playlist Downloader::GetPlaylist(string pid) {
    Playlist playlist = null;
    (new GeneralRequest(Playlist.Url(pid))).GetResponseStream(stream => {
        playlist = new Playlist(stream);
      });
    return playlist;
  }


  string Downloader::GetTitle(string pid) {
    try {
      return GetPlaylist(pid).Title;
    } catch {
      return pid;
    }
  }

  IphonePage Downloader::GetIphonePage(string pid) {
    IphonePage page = null;
    new IphoneRequest(IphonePage.Url(pid), cookies).GetResponseStream(stream => {
	page = new IphonePage(stream);
      });
    return page;
  }

  string Downloader::FilenameSafe(string text) {
    var result = text;
    foreach (var c in Path.GetInvalidFileNameChars()) {
      result = result.Replace(c.ToString(), "");
    }
    return result;
  }

  void Downloader::ReadFromStream(Stream stream, Action<byte[], int> handler) {
    byte[] buffer = new byte[Downloader.BufferSize];
    int bytesRead;

    do {
      bytesRead = stream.Read(buffer, 0, buffer.Length);
      handler(buffer, bytesRead);
    } while (bytesRead > 0);
  }

}
