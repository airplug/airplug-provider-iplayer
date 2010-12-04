using System;
using System.Linq;
using System.Xml;
using System.Xml.Linq;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

#include "pugixml.hpp"

using namespace std;
using namespace pugi;

namespace IPDL {

	static string Playlist::Url(string pid) {
		return urlPrefix + pid;
	}

	Playlist::Playlist(string source) {
		this->document.load(source);
	}


	class PlaylistWalker: pugi::xml_tree_walker {
	public:
		vector<PlaylistItem> &list;

		PlaylistWalker(vector<PlaylistItem> &_list) {
			this->list = _list;
		};

		virtual bool for_each(pugi::xml_node& node) {
			// put in list
			let kind = (string)item.Attribute("kind")
				where kind == "radioProgramme" || kind == "programme"
				select new PlaylistItem {
				Kind       = (kind == "radioProgramme") ? "radio" : "tv",
				Identifier = (string)item.Attribute("identifier"),
				Group      = (string)item.Attribute("group"),
				Alternate  = (string)item.Element(Playlist.playlistNS + "alternate").Attribute("id")
			}
			return true; // continue
		}
	};

	vector<PlaylistItem> Playlist::get_Items() {
		vector<PlaylistItem> list;
		PlaylistWalker walker(list);
		this->document.traverse(walker);
		return list;

		return from item in document.Descendants(Playlist::playlistNS + "item")
			;
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
