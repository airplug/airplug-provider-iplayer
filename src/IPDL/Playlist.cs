using System;
using System.Linq;
using System.Xml;
using System.Xml.Linq;
using System.Collections.Generic;
using System.IO;

namespace IPDL {
  public class PlaylistItem {
    public string Kind;
    public string Identifier;
    public string Group;
    public string Alternate;
  }

  public class Playlist {
    private XElement document;
    private static XNamespace playlistNS = "http://bbc.co.uk/2008/emp/playlist";
    private static string urlPrefix = "http://www.bbc.co.uk/iplayer/playlist/";

    public static string Url(string pid) {
      return Playlist.urlPrefix + pid;
    }

    public Playlist(string source) {
      Init(source);
    }

    public Playlist(Stream source) {
      Init((new StreamReader(source)).ReadToEnd());
    }

    private void Init(string source) {
      this.document = XElement.Parse(source);
    }

    public IEnumerable<PlaylistItem> Items {
      get {
        return from item in document.Descendants(Playlist.playlistNS + "item")
               let kind = (string)item.Attribute("kind")
               where kind == "radioProgramme" || kind == "programme"
               select new PlaylistItem {
                 Kind       = (kind == "radioProgramme") ? "radio" : "tv",
                 Identifier = (string)item.Attribute("identifier"),
                 Group      = (string)item.Attribute("group"),
                 Alternate  = (string)item.Element(Playlist.playlistNS + "alternate").Attribute("id")
               };
      }
    }

    public string Title {
      get {
        return String.Format("{0} ({1})",
                             (string)document.Element(Playlist.playlistNS + "title"),
                             Items.First().Group);
      }
    }
  }
}
