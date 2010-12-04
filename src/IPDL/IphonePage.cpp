using System.Text.RegularExpressions;
using System.IO;

#include "IphonePage.h"

namespace IPDL {

	void IphonePage::Init(string source) {
		this->document = source;
	}

	IphonePage::IphonePage(string source) {
		Init(source);
	}

	IphonePage::IphonePage(Stream source) {
		Init((new StreamReader(source)).ReadToEnd());
	}

	static string IphonePage::Url(string pid) {
		return IphonePage.urlPrefix + pid;
	}

	string IphonePage::get_EmbeddedMediaUrl() {
		var regex = new Regex(@"<embed\s[^>]*href=['""]?([^'""\s]+)",
													RegexOptions.IgnoreCase |
													RegexOptions.Singleline |
													RegexOptions.CultureInvariant);
		var match = regex.Match(document);
		if (match.Success)
			return match.Groups[1].Value;
		else
			return null;
	}

	bool IphonePage::get_IsAvailable() {
		return EmbeddedMediaUrl != null;
	}

	string IphonePage::get_FileExtension() {
		var url = get_EmbeddedMediaUrl();
		if      (Regex.IsMatch(url, @"\.mp3"))
			return ".mp3";
		else if (Regex.IsMatch(url, @"\.mp4"))
			return ".mp4";
		else
			return "";
	}

}
