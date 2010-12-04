#ifndef PLAYLIST_H
#define PLAYLIST_H

using System;
using System.Linq;
using System.Xml;
using System.Xml.Linq;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;


namespace IPDL {

  class PlaylistItem {
	public:
    string Kind;
    string Identifier;
    string Group;
    string Alternate;
  };

  class Playlist {
		/* A playlist is essentially a list of PlaylistItems
			 that can be initialized from an XML source
			 that completely specifies that list.
		 */

	private:
    XElement document;
		// The parsed XML tree

    static XNamespace playlistNS = "http://bbc.co.uk/2008/emp/playlist";

    static string urlPrefix = "http://www.bbc.co.uk/iplayer/playlist/";
		// The common prefix to all URLs to content

    void Init(string source);

	public:

    Playlist(string source); // You can use a string
    Playlist(Stream source); // or a stream as the XML source

    static string Url(string pid);
		// Just urlPrefix + pid

    IEnumerable<PlaylistItem> get_Items();

    string get_Title();

  };

}

#endif // PLAYLIST_H
