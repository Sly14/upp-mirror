#include "WebSSL.h"
#pragma hdrstop

#ifndef flagNOSSL

NAMESPACE_UPP

#define LOG_UPP_SSL_MALLOC 0

#if LOG_UPP_SSL_MALLOC
static int UPP_SSL_alloc = 0;
#endif

void *SSLAlloc(size_t size)
{
	int alloc = HeapRoundUp(size + sizeof(int));
	int *aptr = (int *)MemoryAlloc(alloc);
	if(!aptr)
	{
#if LOG_UPP_SSL_MALLOC
		RLOG("UPP_SSL_MALLOC(" << (int)size << ", alloc " << alloc << ") -> failed, total = " << UPP_SSL_alloc);
#endif
		return NULL;
	}
	*aptr++ = alloc;
#if LOG_UPP_SSL_MALLOC
	UPP_SSL_alloc += alloc;
	RLOG("UPP_SSL_MALLOC(" << (int)size << ", alloc " << alloc << ") -> " << aptr << ", total = " << UPP_SSL_alloc);
#endif
	return aptr;
}

void SSLFree(void *ptr)
{
	if(!ptr)
		return;
	int *aptr = (int *)ptr - 1;
#if LOG_UPP_SSL_MALLOC
	UPP_SSL_alloc -= *aptr;
	RLOG("UPP_SSL_FREE(" << ptr << ", alloc " << *aptr << "), total = " << UPP_SSL_alloc);
#endif
	MemoryFree(aptr);
}

void *SSLRealloc(void *ptr, size_t size)
{
	if(!ptr)
		return NULL;
	int *aptr = (int *)ptr - 1;
	if((int)(size + sizeof(int)) <= *aptr)
	{
#if LOG_UPP_SSL_MALLOC
		RLOG("UPP_SSL_REALLOC(" << ptr << ", " << (int)size << ", alloc " << *aptr << ") -> keep same block");
#endif
		return ptr;
	}
	int newalloc = HeapRoundUp(size + sizeof(int));
	int *newaptr = (int *)MemoryAlloc(newalloc);
	if(!newaptr)
	{
#if LOG_UPP_SSL_MALLOC
		RLOG("UPP_SSL_REALLOC(" << ptr << ", " << (int)size << ", alloc " << newalloc << ") -> fail");
#endif
		return NULL;
	}
	*newaptr++ = newalloc;
	memcpy(newaptr, ptr, min<int>(*aptr - sizeof(int), size));
#if LOG_UPP_SSL_MALLOC
	UPP_SSL_alloc += newalloc - *aptr;
	RLOG("UPP_SSL_REALLOC(" << ptr << ", " << (int)size << ", alloc " << newalloc << ") -> " << newaptr
		<< ", total = " << UPP_SSL_alloc);
#endif
	MemoryFree(aptr);
	return newaptr;
}

void SSLInit()
{
	static bool inited = false;
	if(!inited)
	{
		inited = true;
		Socket::Init();
		CRYPTO_set_mem_functions(SSLAlloc, SSLRealloc, SSLFree);
		SSL_load_error_strings();
		SSL_library_init();
	}
}

String SSLGetLastError()
{
	char errbuf[150];
	ERR_error_string(ERR_get_error(), errbuf);
	return errbuf;
}

String SSLToString(X509_NAME *name)
{
	char buffer[500];
	return X509_NAME_oneline(name, buffer, sizeof(buffer));
}

Date ASN1ToDate(ASN1_STRING *time)
{
	if(!time) return Null;
	int digit = 0;
	while(digit < time->length && IsDigit(time->data[digit]))
		digit++;
	if(digit < 6)
		return Null;
	int year2 = time->data[0] * 10 + time->data[1] - 11 * '0';
	int month = time->data[2] * 10 + time->data[3] - 11 * '0';
	int day   = time->data[4] * 10 + time->data[5] - 11 * '0';
	return Date(year2 + (year2 < 90 ? 2000 : 1900), month, day);
}

String ASN1ToString(ASN1_STRING *s)
{
	return String(s->data, s->length);
}

String SSLBuffer::Get() const
{
	if(IsEmpty())
		return String::GetVoid();
	return String(buf_mem->data, buf_mem->length);
}

bool SSLBuffer::Grow(int length)
{
	return !IsEmpty() && BUF_MEM_grow(buf_mem, length);
}

bool SSLBuffer::Set(String d)
{
	if(!buf_mem && !Create())
		return false;
	int len = d.GetLength();
	if(buf_mem->max < len && !Grow(len))
		return false;
	ASSERT(buf_mem->max >= len);
	buf_mem->length = len;
	memcpy(buf_mem, d, len);
	return true;
}

bool SSLStream::OpenBuffer(const char *data, int length)
{
	return Set(BIO_new_mem_buf(const_cast<char *>(data), length));
}

bool SSLStream::CreateBuffer()
{
	Clear();
	SSLBuffer buf;
	if(!buf.Create() || !Create(BIO_s_mem()))
		return false;
	BIO_set_mem_buf(bio, buf.Detach(), BIO_CLOSE);
	return true;
}

String SSLStream::GetResult() const
{
	if(IsEmpty())
		return String::GetVoid();
	BUF_MEM *bm = NULL;
	BIO_get_mem_ptr(bio, &bm);
	if(!bm)
		return String::GetVoid();
	return String(bm->data, bm->length);
}

bool SSLKey::Load(String data)
{
	Clear();
	SSLStream strm;
	if(!strm.OpenBuffer(data.Begin(), data.GetLength()))
		return false;
	return Set(PEM_read_bio_PrivateKey(strm, NULL, NULL, NULL));
}

bool SSLCertificate::Load(String data, bool asn1)
{
	Clear();
	SSLStream in, pem, *sio = &in;
	if(!in.OpenBuffer(data, data.GetLength()))
		return false;
	if(!asn1)
	{
		if(!pem.Create(BIO_f_base64()))
			return false;
		BIO_push(pem, in);
		sio = &pem;
	}
	return Set(d2i_X509_bio(*sio, NULL));
}

String SSLCertificate::Save(bool asn1) const
{
	if(IsEmpty())
		return String::GetVoid();
	SSLStream out, pem, *sio = &out;
	if(!out.CreateBuffer())
		return String::GetVoid();
	if(!asn1)
	{
		if(!pem.Create(BIO_f_base64()))
			return String::GetVoid();
		BIO_push(pem, out);
		sio = &pem;
	}
	i2d_X509_bio(*sio, cert);
	return out.GetResult();
}

String SSLCertificate::GetSubjectName() const
{
	ASSERT(!IsEmpty());
	return SSLToString(X509_get_subject_name(cert));
}

String SSLCertificate::GetIssuerName() const
{
	ASSERT(!IsEmpty());
	return SSLToString(X509_get_issuer_name(cert));
}

Date SSLCertificate::GetNotBefore() const
{
	ASSERT(!IsEmpty());
	return ASN1ToDate(X509_get_notBefore(cert));
}

Date SSLCertificate::GetNotAfter() const
{
	ASSERT(!IsEmpty());
	return ASN1ToDate(X509_get_notAfter(cert));
}

int SSLCertificate::GetVersion() const
{
	ASSERT(!IsEmpty());
	return X509_get_version(cert);
}

String SSLCertificate::GetSerialNumber() const
{
	ASSERT(!IsEmpty());
	return ASN1ToString(X509_get_serialNumber(cert));
}

/*
String SSLCertificate::GetSignatureType() const
{
	ASSERT(!IsEmpty());
	return ASN1ToString(X509_get
}

String SSLCertificate::GetPublicKey() const
{
	ASSERT(!IsEmpty());
}

String SSLCertificate::GetAuthority() const
{
	ASSERT(!IsEmpty());
}

String SSLCertificate::GetHashType() const
{
	ASSERT(!IsEmpty());
}

String SSLCertificate::GetHash() const
{
	ASSERT(!IsEmpty());
}
*/

bool SSLContext::CipherList(const char *list)
{
	ASSERT(ssl_ctx);
	return SSL_CTX_set_cipher_list(ssl_ctx, list);
}

bool SSLContext::UseCertificate(String certdata, String pkeydata, bool cert_asn1)
{
	ASSERT(ssl_ctx);
	if(IsNull(certdata) || IsNull(pkeydata))
		return false;
	SSLCertificate cert;
	SSLKey pkey;
	if(!cert.Load(certdata, cert_asn1) || !pkey.Load(pkeydata))
		return false;
	if(!SSL_CTX_use_certificate(ssl_ctx, cert) || !SSL_CTX_use_PrivateKey(ssl_ctx, pkey))
		return false;
	if(!SSL_CTX_check_private_key(ssl_ctx))
		return false;
	return true;
}

void SSLContext::VerifyPeer(bool verify, int depth)
{
	ASSERT(ssl_ctx);
	SSL_CTX_set_verify(ssl_ctx, verify ? SSL_VERIFY_PEER : SSL_VERIFY_NONE, NULL);
	SSL_CTX_set_verify_depth(ssl_ctx, depth);
}

class SSLSocketData : public Socket::Data
{
public:
	SSLSocketData(SSLContext& context);
	virtual ~SSLSocketData();

	bool                            OpenClient(const char *host, int port, bool nodelay, dword *my_addr, int timeout, bool is_blocking);
	bool                            OpenAccept(SOCKET connection, bool nodelay, bool blocking);

	virtual int                     GetKind() const { return SOCKKIND_SSL; }
	virtual bool                    Peek(int timeout_msec, bool write);
	virtual int                     Read(void *buf, int amount);
	virtual int                     Write(const void *buf, int amount);
	virtual bool                    Accept(Socket& socket, dword *ipaddr, bool nodelay, int timeout_msec);
	virtual bool                    Close(int timeout_msec);
	virtual Value                   GetInfo(String info) const;

	void                            SetSSLError(const char *context);
	void                            SetSSLResError(const char *context, int res);

public:
	SSLContext&                     ssl_context;
	SSL                            *ssl;
	SSLCertificate                  cert;
};

SSLSocketData::SSLSocketData(SSLContext& ssl_context)
: ssl_context(ssl_context)
{
	ssl = NULL;
}

SSLSocketData::~SSLSocketData()
{
	Close(0);
}

void SSLSocketData::SetSSLError(const char *context)
{
	SetError();
	Socket::SetSockError(socket, context, SSLGetLastError());
}

void SSLSocketData::SetSSLResError(const char *context, int res)
{
	SetError();
	int code = SSL_get_error(ssl, res);
	String out;
	switch(code)
	{
#define SSLERR(c) case c: out = #c; break;
	SSLERR(SSL_ERROR_NONE)
	SSLERR(SSL_ERROR_SSL)
	SSLERR(SSL_ERROR_WANT_READ)
	SSLERR(SSL_ERROR_WANT_WRITE)
	SSLERR(SSL_ERROR_WANT_X509_LOOKUP)
	SSLERR(SSL_ERROR_SYSCALL)
	SSLERR(SSL_ERROR_ZERO_RETURN)
	SSLERR(SSL_ERROR_WANT_CONNECT)
#ifdef PLATFORM_WIN32
	SSLERR(SSL_ERROR_WANT_ACCEPT)
#endif
	default: out = "unknown code"; break;
	}
	out << " (" << code << ")";
	Socket::SetSockError(socket, context, out);
}

bool SSLSocketData::Peek(int timeout_msec, bool write)
{
	if(ssl && !write && SSL_pending(ssl) > 0)
		return true;
	return Data::Peek(timeout_msec, write);
}

int SSLSocketData::Read(void *buf, int amount)
{
	ASSERT(ssl);
	int res = SSL_read(ssl, (char *)buf, amount);
	if(res <= 0)
		SetSSLResError("SSL_read", res);
#ifndef NOFAKEERROR
	if(fake_error && res > 0)
	{
		if((fake_error -= res) <= 0)
		{
			fake_error = 0;
			SetError();
			Socket::SetSockError(socket, "SSL_read", "fake error");
			return -1;
		}
		else
			RLOG("SSLSocketData::Read: fake error after " << fake_error);
	}
#endif
	return res;
}

int SSLSocketData::Write(const void *buf, int amount)
{
	ASSERT(ssl);
	int res = SSL_write(ssl, (const char *)buf, amount);
	if(res <= 0)
		SetSSLResError("SSL_write", res);
	return res;
}

bool SSLSocketData::OpenClient(const char *host, int port, bool nodelay, dword *my_addr, int timeout, bool blocking)
{
	if(!Data::OpenClient(host, port, nodelay, my_addr, timeout, blocking))
		return false;
	if(!(ssl = SSL_new(ssl_context)))
	{
		SetSSLError("OpenClient / SSL_new");
		return false;
	}
	if(!SSL_set_fd(ssl, socket))
	{
		SetSSLError("OpenClient / SSL_set_fd");
		return false;
	}
	SSL_set_connect_state(ssl);
	int res = SSL_connect(ssl);
	if(res <= 0)
	{
		SetSSLResError(NFormat("OpenClient(host=%s, port=%d) / SSL_connect", host, port), res);
		return false;
	}
	cert.Set(SSL_get_peer_certificate(ssl));
	return true;
}

bool SSLSocketData::OpenAccept(SOCKET conn, bool nodelay, bool blocking)
{
	Attach(conn, nodelay, blocking);
	if(!(ssl = SSL_new(ssl_context)))
	{
		SetSSLError("Accept / SSL_new");
		return false;
	}
	if(!SSL_set_fd(ssl, socket))
	{
		SetSSLError("Accept / SSL_set_fd");
		return false;
	}
	SSL_set_accept_state(ssl);
	int res = SSL_accept(ssl);
	if(res <= 0)
	{
		SetSSLResError("Accept / SSL_accept", res);
		return false;
	}
	cert.Set(SSL_get_peer_certificate(ssl));
	return true;
}

bool SSLSocketData::Accept(Socket& socket, dword *ipaddr, bool nodelay, int timeout_msec)
{
	SOCKET connection = AcceptRaw(ipaddr, timeout_msec);
	if(connection == INVALID_SOCKET)
		return false;
	One<SSLSocketData> data = new SSLSocketData(ssl_context);
	if(!data->OpenAccept(connection, nodelay, is_blocking))
		return false;
	socket.Attach(-data);
	return true;
}

bool SSLSocketData::Close(int timeout_msec)
{
	if(ssl)
		SSL_shutdown(ssl);
	bool res = Data::Close(timeout_msec);
	if(ssl)
	{
		SSL_free(ssl);
		ssl = NULL;
	}
	return res;
}

Value SSLSocketData::GetInfo(String info) const
{
	if(info == SSLInfoCipher())           return SSL_get_cipher(ssl);
	if(info == SSLInfoCertAvail())        return cert.IsEmpty() ? 0 : 1;
	if(info == SSLInfoCertVerified())     return SSL_get_verify_result(ssl) == X509_V_OK ? 1 : 0;
	if(info == SSLInfoCertSubjectName())  return cert.IsEmpty() ? String::GetVoid() : cert.GetSubjectName();
	if(info == SSLInfoCertIssuerName())   return cert.IsEmpty() ? String::GetVoid() : cert.GetIssuerName();
	if(info == SSLInfoCertNotBefore())    return cert.IsEmpty() ? Date(Null)        : cert.GetNotBefore();
	if(info == SSLInfoCertNotAfter())     return cert.IsEmpty() ? Date(Null)        : cert.GetNotAfter();
	if(info == SSLInfoCertVersion())      return cert.IsEmpty() ? int(Null)         : cert.GetVersion();
	if(info == SSLInfoCertSerialNumber()) return cert.IsEmpty() ? String::GetVoid() : cert.GetSerialNumber();

	return Data::GetInfo(info);
}

bool SSLServerSocket(Socket& socket, SSLContext& ssl_context, int port, bool nodelay, int listen_count, bool blocking)
{
	One<SSLSocketData> data = new SSLSocketData(ssl_context);
	if(!data->OpenServer(port, nodelay, listen_count, blocking))
		return false;
	socket.Attach(-data);
	return true;
}

bool SSLClientSocket(Socket& socket, SSLContext& ssl_context, const char *host, int port, bool nodelay, dword *my_addr, int timeout, bool blocking)
{
	One<SSLSocketData> data = new SSLSocketData(ssl_context);
	if(!data->OpenClient(host, port, nodelay, my_addr, timeout, blocking))
		return false;
	socket.Attach(-data);
	return true;
}

END_UPP_NAMESPACE

#endif//flagNOSSL
