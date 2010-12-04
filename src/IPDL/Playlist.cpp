using System;
using System.Linq;
using System.Xml;
using System.Xml.Linq;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace IPDL {

	static string Playlist::Url(string pid) {
		return urlPrefix + pid;
	}

	Playlist::Playlist(string source) {
		Init(source);
	}

	Playlist::Playlist(Stream source) {
		Init((new StreamReader(source)).ReadToEnd());
	}

	void Playlist::Init(string source) {
		this.document = XElement.Parse(source);
	}

	IEnumerable<PlaylistItem> Playlist::get_Items() {
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

	string Playlist::get_Title() {
		var pids =
			from item in document.Descendants(Playlist.playlistNS + "item")
			let grp = (string)item.Attribute("group")
			where grp != null
			select grp;
		var title =
			Regex.Replace(
										(string)document.Element(Playlist.playlistNS + "title"),
										@"\b(\d{2})/(\d{2})/(\d{4})\b",
										@"${3}-${2}-${1}");
		return String.Format("{0} ({1})", title, pids.First());
	}

}
