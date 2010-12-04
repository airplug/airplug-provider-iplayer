#ifndef REQUEST_H
#define REQUEST_H

using System;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

namespace IPDL {

  class AbstractRequest {

	private: // Data members

    string userAgent;
    string url;
    CookieContainer cookieContainer;

	public: // Public methods

		AbstractRequest(string url, CookieContainer cookieContainer, string userAgent);

	protected:

    HttpWebRequest Request();
    void WithResponse(HttpWebRequest request, void (ResponseHandler*)(WebResponse response) handler);
    void WithResponseStream(HttpWebRequest request, void (ResponseStreamHandler*)(Stream stream) handler);
  };



  class GeneralRequest : public AbstractRequest {
	public:
		GeneralRequest(string url);
	protected:
    void GetResponseStream(ResponseStreamHandler streamHandler);
  };



  class IphoneRequest : public AbstractRequest {
	private:
    static string UserAgent = "Mozilla/5.0 (iPhone; U; CPU iPhone OS 3_1_2 like Mac OS X; en-us) AppleWebKit/528.18 (KHTML, like Gecko) Version/4.0 Mobile/7D11 Safari/528.16";

	public:
    IphoneRequest(string url, CookieContainer cookieContainer);
  };



  class CoreMediaRequest : public AbstractRequest {

	private: // Data members

    static string UserAgent = "Apple iPhone v1.1.4 CoreMedia v1.0.0.4A102";
    static int MaxSegmentSize = 0x400000;

    int contentLength = -1;

	public: // Public methods

    CoreMediaRequest(string url, CookieContainer cookieContainer);

    int get_ContentLength();

    void GetResponseStreamFromOffset(int offset, ResponseStreamHandler streamHandler);

	private: // Private methods

    void MakeInitialRangeRequestIfNecessary();

  };

}

#endif // REQUEST_H
