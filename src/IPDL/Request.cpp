using System;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

namespace IPDL {


	AbstractRequest::AbstractRequest(string url, CookieContainer cookieContainer, string userAgent) {
		this.url             = url;
		this.cookieContainer = cookieContainer;
		this.userAgent       = userAgent;
	}

	HttpWebRequest AbstractRequest::Request() {
		HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
		request.Accept          = "*/*";
		request.UserAgent       = userAgent;
		request.CookieContainer = cookieContainer;
#if DEBUG
		Console.WriteLine("Request\n{0}\n\n{1}\n----", url, request.Headers);
#endif
		return request;
	}


	void AbstractRequest::WithResponse(HttpWebRequest request, void (ResponseHandler*)(WebResponse response) handler) {
		using (HttpWebResponse response = (HttpWebResponse)request.GetResponse()) {
			if (cookieContainer != null)
				cookieContainer.Add(response.Cookies);
#if DEBUG
			Console.WriteLine("Response\n{0}\n----", response.Headers);
#endif
			handler(response);
		}
	}

	void AbstractRequest::WithResponseStream(HttpWebRequest request, void (ResponseStreamHandler*)(Stream stream) handler) {
		WithResponse(request, response => {
        using (var stream = response.GetResponseStream()) {
          handler(stream);
        }
      });
	}



	GeneralRequest::GeneralRequest(string url)
		: base(url, null, null) {}

	void GeneralRequest::GetResponseStream(ResponseStreamHandler streamHandler) {
		WithResponseStream(Request(), streamHandler);
	}


	IphoneRequest::IphoneRequest(string url, CookieContainer cookieContainer)
		: base(url, cookieContainer, IphoneRequest.UserAgent) {}

	void IphoneRequest::GetResponseStream(ResponseStreamHandler streamHandler) {
		WithResponseStream(Request(), streamHandler);
	}




	CoreMediaRequest::CoreMediaRequest(string url, CookieContainer cookieContainer)
		: base(url, cookieContainer, CoreMediaRequest.UserAgent) {}

	int CoreMediaRequest::get_ContentLength() {
		MakeInitialRangeRequestIfNecessary();
		return contentLength;
	}


	void CoreMediaRequest::GetResponseStreamFromOffset(int offset, ResponseStreamHandler streamHandler) {
		var contentLength = ContentLength;
		for (int firstByte = offset; firstByte < contentLength; firstByte += CoreMediaRequest.MaxSegmentSize) {
			int lastByte = Math.Min(firstByte + CoreMediaRequest.MaxSegmentSize, contentLength) - 1;
			var request = Request();
			request.AddRange(firstByte, lastByte);
			WithResponseStream(request, streamHandler);
		}
	}

	void CoreMediaRequest::MakeInitialRangeRequestIfNecessary() {
		if (contentLength >= 0)
			return;
		var request = Request();
		request.AddRange(0, 1);
		WithResponse(request, response => {
        this.contentLength = int.Parse(Regex.Match(response.Headers["Content-Range"], @"\d+$").Value);
      });
	}


}
