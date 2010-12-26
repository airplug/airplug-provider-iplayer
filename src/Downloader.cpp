using System;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

namespace IPDL {

  Downloader::Downloader() {
    this.cookies = new CookieContainer();
  }

  void Downloader::Download(string pid,
									void (atStart*)(string filename),
									void (progress*)(int bytesFetched, int bytesTotal),
									void (atEnd*)(DownloadStatus status, string message)
									) {
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
