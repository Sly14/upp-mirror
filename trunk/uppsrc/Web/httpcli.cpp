#include "Web.h"

NAMESPACE_UPP

bool HttpClient_Trace__;

#define LLOG(x)       if(HttpClient_Trace__) RLOG(x); else;
#define LLOGBLOCK(x)  // DLOGBLOCK(x)

void HttpClient::Trace(bool b)
{
	HttpClient_Trace__ = b;
}

void HttpClient::Init()
{
	port = DEFAULT_PORT;
	timeout_msecs = DEFAULT_TIMEOUT_MSECS;
	max_header_size = DEFAULT_MAX_HEADER_SIZE;
	max_content_size = DEFAULT_MAX_CONTENT_SIZE;
	keepalive = false;
	std_headers = true;
	hasurlvar = false;
	method = METHOD_GET;
}


HttpClient::HttpClient()
{
	Init();
}

HttpClient::HttpClient(const char *url)
{
	Init();
	URL(url);
}

HttpClient& HttpClient::URL(const char *u)
{
	const char *t = u;
	while(*t && *t != '?')
		if(*t++ == '/' && *t == '/') {
			u = ++t;
			break;
		}
	t = u;
	while(*u && *u != ':' && *u != '/' && *u != '?')
		u++;
	if(*u == '?' && u[1])
		hasurlvar = true;
	host = String(t, u);
	port = DEFAULT_PORT;
	if(*u == ':')
		port = ScanInt(u + 1, &u);
	path = u;
	return *this;
}

HttpClient& HttpClient::Proxy(const char *p)
{
	const char *t = p;
	while(*p && *p != ':')
		p++;
	proxy_host = String(t, p);
	proxy_port = 80;
	if(*p++ == ':' && IsDigit(*p))
		proxy_port = ScanInt(p);
	return *this;
}

String HttpClient::ExecuteRedirect(int max_redirect, int retries, Gate2<int, int> progress)
{
	int nredir = 0;
	for(;;) {
		if(progress(0, 0)) {
			aborted = true;
			return String::GetVoid();
		}
		String data = Execute(progress);
		if(status_code >= 400 && status_code < 500) {
			error = status_line;
			return String::GetVoid();
		}
		int r = 0;
		while(data.IsVoid()) {
			if(progress(0, 0)) {
				aborted = true;
				return String::GetVoid();
			}
			if(++r >= retries)
				return String::GetVoid();
			data = Execute(progress);
		}
		if(!IsRedirect())
			return data;
		if(++nredir > max_redirect) {
			error = NFormat("Maximum number of redirections exceeded: %d", max_redirect);
			return String::GetVoid();
		}
		URL(GetRedirectURL());
	}
}

String HttpClient::ReadUntilProgress(char until, int start_time, int end_time, Gate2<int, int> progress)
{
	String out;
	while(!socket.IsEof() && !socket.IsError()) {
		out.Cat(socket.Read(1000, 1000));
		int f = out.Find('\n');
		if(f >= 0) {
			socket.UnRead(out.Mid(f + 1));
			out.Trim(f);
			return out;
		}
		int t = msecs();
		if(t >= end_time) {
			socket.SetErrorText(NFormat(t_("%s:%d receiving headers timed out"), host, port));
			break;
		}
		if(progress(msecs(start_time), end_time - start_time)) {
			aborted = true;
			break;
		}
	}
	return String::GetVoid();
}

HttpClient& HttpClient::Post(const char *id, const String& data)
{
	if(postdata.GetCount())
		postdata << '&';
	postdata << id << '=' << UrlEncode(data);
	return *this;
}

HttpClient& HttpClient::UrlVar(const char *id, const String& data)
{
	int c = *path.Last();
	if(hasurlvar && c != '&')
		path << '&';
	if(!hasurlvar && c != '?')
		path << '?';
	path << id << '=' << UrlEncode(data);
	hasurlvar = true;
	return *this;
}

String HttpClient::Execute(Gate2<int, int> progress)
{
	LLOGBLOCK("HttpClient::Execute");
	int start_time = msecs();
	int end_time = start_time + timeout_msecs;
	aborted = false;
	server_headers = Null;
	status_line = Null;
	status_code = 0;
	is_redirect = false;
	redirect_url = Null;
	if(socket.IsOpen() && IsError())
		Close();
	error = Null;
	bool use_proxy = !IsNull(proxy_host);
	String sock_host = (use_proxy ? proxy_host : host);
	int sock_port = (use_proxy ? proxy_port : port);

	LLOG("socket host = " << sock_host << ":" << sock_port);
	if(!socket.IsOpen()) {
		if(!ClientSocket(socket, sock_host, sock_port, true, NULL, 0, false)) {
			error = Socket::GetErrorText();
			return String::GetVoid();
		}
		socket.Linger(0);
	}
	while(!socket.PeekWrite(1000)) {
		int time = msecs();
		if(time >= end_time) {
			error = NFormat(t_("%s:%d: connecting to host timed out"), sock_host, sock_port);
			return String::GetVoid();
		}
		if(progress(time - start_time, end_time - start_time)) {
			aborted = true;
			return String::GetVoid();
		}
	}
	String request;
	String ctype = contenttype;
	switch(method) {
		case METHOD_GET:  request << "GET "; break;
		case METHOD_POST:
			request << "POST ";
			if(IsNull(ctype))
				ctype = "application/x-www-form-urlencoded";
			break;
        case METHOD_HEAD: request << "HEAD "; break;
		default: NEVER(); // invalid method
	}
	String host_port = host;
	if(port != DEFAULT_PORT)
		host_port << ':' << port;
	String url;
	url << "http://" << host_port << Nvl(path, "/");
	if(use_proxy)
		request << url;
	else
		request << Nvl(path, "/");
	request << " HTTP/1.1\r\n";
	if(std_headers) {
		request
			<< "URL: " << url << "\r\n"
			<< "Host: " << host_port << "\r\n"
			<< "Connection: " << (keepalive ? "keep-alive" : "close") << "\r\n";
		if(keepalive)
			request << "Keep-alive: 300\r\n"; // 5 minutes (?)
		request << "Accept: " << Nvl(accept, "*/*") << "\r\n";
		request << "Accept-Encoding: gzip\r\n";
		request << "Agent: " << Nvl(agent, "Ultimate++ HTTP client") << "\r\n";
		if(method == METHOD_POST)
			request << "Content-Length: " << postdata.GetLength() << "\r\n";
		if(ctype.GetCount())
			request << "Content-Type: " << ctype << "\r\n";
	}
	if(use_proxy && !IsNull(proxy_username))
		 request << "Proxy-Authorization: basic " << Base64Encode(proxy_username + ':' + proxy_password) << "\r\n";
	if(!IsNull(username) || !IsNull(password))
		request << "Authorization: basic " << Base64Encode(username + ":" + password) << "\r\n";
	request << client_headers << "\r\n" << postdata;
	LLOG("host = " << host << ", port = " << port);
	LLOG("request: " << request);
	int written = 0;
	while(msecs() < end_time) {
		int nwrite = socket.WriteWait(request.GetIter(written), min(request.GetLength() - written, 1000), 1000);
		if(socket.IsError()) {
			error = Socket::GetErrorText();
			return String::GetVoid();
		}
		if((written += nwrite) >= request.GetLength())
			break;
		if(progress(written, request.GetLength())) {
			aborted = true;
			return String::GetVoid();
		}
	}
	if(written < request.GetLength()) {
		error = NFormat(t_("%s:%d: timed out sending request to server"), host, port);
		return String::GetVoid();
	}
	status_line = ReadUntilProgress('\n', start_time, end_time, progress);
	if(socket.IsError()) {
		error = Socket::GetErrorText();
		return String::GetVoid();
	}
	if(status_line.GetLength() < 5 || MemICmp(status_line, "HTTP/", 5)) {
		error = NFormat(t_("%s:%d: invalid server response: %s"), host, port, status_line);
		return String::GetVoid();
	}

	status_code = 0;
	const char *p = status_line.Begin() + 5;
	while(*p && *p != ' ')
		p++;
	if(*p == ' ' && IsDigit(*++p))
		status_code = stou(p);
	is_redirect = (status_code >= 300 && status_code < 400);

	int content_length = -1;
	bool tc_chunked = false;
	bool ce_gzip = false;
	for(;;) {
		String line = ReadUntilProgress('\n', start_time, end_time, progress);
		LLOG("< " << line);
		if(socket.IsError()) {
			error = Socket::GetErrorText();
			return String::GetVoid();
		}

		for(p = line; *p && (byte)*p <= ' '; p++)
			;
		const char *b = p, *e = line.End();
		while(e > b && (byte)e[-1] < ' ')
			e--;
		if(b >= e)
			break;
		static const char cl[] = "content-length:";
		static const char ce[] = "content-encoding:";
		static const char te[] = "transfer-encoding:";
		static const char lo[] = "location:";
		static const int CL_LENGTH = sizeof(cl) - 1;
		static const int CE_LENGTH = sizeof(ce) - 1;
		static const int TE_LENGTH = sizeof(te) - 1;
		static const int LO_LENGTH = sizeof(lo) - 1;
		if(!MemICmp(p, cl, CL_LENGTH)) {
			for(p += CL_LENGTH; *p == ' '; p++)
				;
			if(IsDigit(*p)) {
				content_length = minmax<int>(stou(p), 0, max_content_size);
				if(content_length > max_content_size) {
					error = NFormat(t_("%s:%d: maximum data length exceeded (%d B)"), host, port, max_content_size);
					return String::GetVoid();
				}
			}
		}
		else if(!MemICmp(p, ce, CE_LENGTH)) {
			for(p += CE_LENGTH; *p == ' '; p++)
				;
			static const char gzip[] = "gzip";
			if(e - p == sizeof(gzip) - 1 && !memcmp(p, gzip, sizeof(gzip) - 1))
				ce_gzip = true;
		}
		else if(!MemICmp(p, te, TE_LENGTH)) {
			for(p += TE_LENGTH; *p == ' '; p++)
				;
			static const char ch[] = "chunked";
			if(e - p == sizeof(ch) - 1 && !memcmp(p, ch, sizeof(ch) - 1))
				tc_chunked = true;
		}
		else if(!MemICmp(p, lo, LO_LENGTH)) {
			for(p += LO_LENGTH; *p == ' '; p++)
				;
			redirect_url = String(p, e);
			int q = redirect_url.Find('?');
			int p = path.Find('?');
			if(p >= 0 && q < 0)
				redirect_url.Cat(path.GetIter(p));
		}
		if(server_headers.GetLength() + (e - b) + 2 > max_header_size) {
			error = NFormat(t_("%s:%d: maximum header length exceeded (%d B)"), host, port, max_header_size);
			return String::GetVoid();
		}
		server_headers.Cat(b, int(e - b));
		server_headers.Cat("\r\n");
	}
	if(method == METHOD_HEAD)
		return String::GetVoid();
	String chunked;
	String body;
	while(body.GetLength() < content_length || content_length < 0 || tc_chunked) {
		if(msecs(end_time) >= 0) {
			error = NFormat(t_("%s:%d: timed out when receiving server response"), host, port);
			return String::GetVoid();
		}
		String part = socket.Read(1000);
		if(!part.IsEmpty()) {
			if(body.GetLength() + part.GetLength() > DEFAULT_MAX_CONTENT_SIZE) {
				error = NFormat(t_("Maximum content size exceeded: %d"), body.GetLength() + part.GetLength());
				goto EXIT;
			}
			body.Cat(part);
			if(tc_chunked)
				for(;;) {
					const char *p = body.Begin(), *e = body.End();
					while(p < e && *p != '\n')
						p++;
					if(p >= e)
						break;
					int nextline = int(p + 1 - body.Begin());
					p = body.Begin();
					int part_length = ctoi(*p);
					if((unsigned)part_length >= 16) {
						body.Remove(0, nextline);
						continue;
					}
					for(int i; (unsigned)(i = ctoi(*++p)) < 16; part_length = part_length * 16 + i)
						;
					body.Remove(0, nextline);
					if(part_length <= 0) {
						for(;;) {
							const char *b = body.Begin();
							p = b;
							while(*p && *p != '\n')
								p++;
							if(!*p && socket.IsOpen() && !socket.IsError() && !socket.IsEof()) {
								if(msecs(end_time) >= 0) {
									error = NFormat("Timeout reading footer block (%d B).", body.GetLength());
									break;
								}
								if(body.GetLength() > 3)
									body.Remove(0, body.GetLength() - 3);
								String part = socket.Read(1000);
								body.Cat(part);
								continue;
							}
							const char *l = p;
							if(*p == '\n')
								p++;
							if(l > b && l[-1] == '\r')
								l--;
							if(l == b)
								break;
							server_headers.Cat(b, p - b);
						}
						goto EXIT;
					}
					if(body.GetLength() >= part_length) {
						chunked.Cat(body, part_length);
						body.Remove(0, part_length);
						continue;
					}
					for(;;) {
						String part = socket.Read(-msecs(end_time));
						if(part.IsEmpty()) {
							error = NFormat("Timeout reading Transfer-encoding: chunked block (%d B).", part_length);
							goto EXIT;
						}
						body.Cat(part);
						if(body.GetLength() >= part_length) {
							chunked.Cat(body, part_length);
							body.Remove(0, part_length);
							break;
						}
						if(progress(chunked.GetLength() + body.GetLength(), 0)) {
							aborted = true;
							goto EXIT;
						}
					}
				}
		}
		if(!socket.IsOpen() || socket.IsError() || socket.IsEof()) {
			if(socket.IsError())
				error = Socket::GetErrorText();
			else if(!tc_chunked && content_length > 0 && body.GetLength() < content_length)
				error = NFormat(t_("Partial input: %d out of %d"), body.GetLength(), content_length);
			break;
		}
		if(progress(chunked.GetLength() + body.GetLength(), max(content_length, 0))) {
			aborted = true;
			break;
		}
	}

EXIT:
	if(!keepalive || socket.IsError() || socket.IsEof())
		Close();

	if(tc_chunked)
		body = chunked;

	if(ce_gzip)
		body = GZDecompress(body);
	return body;
}

String HttpClientGet(String url, String proxy, String username, String password,
	String *server_headers, String *error, Gate2<int, int> progress,
	int timeout, int num_redirect, int retries)
{
	HttpClient client;
	String out = client
		.URL(url)
		.User(username, password)
		.TimeoutMsecs(timeout)
		.Proxy(proxy)
		.ExecuteRedirect(num_redirect, progress, progress);
	if(server_headers)
		*server_headers = client.GetHeaders();
	if(error)
		*error = client.GetError();
	return out;
}

String HttpClientGet(String url, String proxy, String *server_headers, String *error,
	Gate2<int, int> progress, int timeout, int max_redirect, int retries)
{
	return HttpClientGet(url, proxy, Null, Null, server_headers, error, progress, timeout, max_redirect, retries);
}

String HttpClientGet(String url, String username, String password,
	String *server_headers, String *error, Gate2<int, int> progress,
	int timeout, int num_redirect, int retries)
{
	return HttpClientGet(url, Null, username, password, server_headers, error, progress, timeout, num_redirect, retries);
}

String HttpClientGet(String url, String *server_headers, String *error,
	Gate2<int, int> progress, int timeout, int max_redirect, int retries)
{
	return HttpClientGet(url, Null, Null, Null, server_headers, error, progress, timeout, max_redirect, retries);
}

END_UPP_NAMESPACE
