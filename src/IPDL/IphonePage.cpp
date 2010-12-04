#include "IphonePage.h"

#include <string>

#include <boost/xpressive/xpressive.hpp>
// Regular expressions

using namespace std;
using namespace boost::xpressive;

namespace IPDL {

	string IphonePage::urlPrefix = "http://www.bbc.co.uk/mobile/iplayer/episode/";

	void IphonePage::Init(string source) {
		this->document = source;
	}

	IphonePage::IphonePage(string source) {
		Init(source);
	}

	string IphonePage::Url(string pid) {
		return IphonePage::urlPrefix + pid;
	}

	string IphonePage::get_EmbeddedMediaUrl() {
		sregex url_regex = sregex::compile("<embed\\s[^>]*href=['""]?)([^'""\\s]+)");
		smatch url_match;

		if ( regex_match(document, url_match, url_regex) ) {
			return url_match[0];
		} else {
			return "";
		}
	}

	bool IphonePage::get_IsAvailable() {
		return get_EmbeddedMediaUrl() != "";
	}

	string IphonePage::get_FileExtension() {
		string url = get_EmbeddedMediaUrl();

		sregex mp3_regex = sregex::compile("\\.mp3");
		sregex mp4_regex = sregex::compile("\\.mp4");

		smatch mp3_match;
		smatch mp4_match;

		if( regex_match(url, mp3_match, mp3_regex)) {
			return ".mp3";
		} else if(regex_match(url, mp4_match, mp4_regex)) {
			return ".mp4";
		} else {
			return "";
		}

	}

}
