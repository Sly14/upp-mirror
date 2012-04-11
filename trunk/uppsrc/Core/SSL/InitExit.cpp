#include "SSL.h"

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

static int UPP_SSL_alloc = 0;

static void *SslAlloc(size_t size)
{
	size_t alloc = size + sizeof(int);
	int *aptr = (int *)MemoryAllocSz(alloc);
	*aptr++ = alloc;
	UPP_SSL_alloc += alloc;
	LLOG("UPP_SSL_MALLOC(" << (int)size << ", alloc " << alloc << ") -> " << FormatIntHex(aptr) << ", total = " << UPP_SSL_alloc);
	return aptr;
}

static void SslFree(void *ptr)
{
	if(!ptr)
		return;
	int *aptr = (int *)ptr - 1;
	UPP_SSL_alloc -= *aptr;
	LLOG("UPP_SSL_FREE(" << ptr << ", alloc " << *aptr << "), total = " << UPP_SSL_alloc);
	MemoryFree(aptr);
}

static void *SslRealloc(void *ptr, size_t size)
{
	if(!ptr)
		return NULL;
	int *aptr = (int *)ptr - 1;
	if((int)(size + sizeof(int)) <= *aptr) {
		LLOG("UPP_SSL_REALLOC(" << ptr << ", " << (int)size << ", alloc " << *aptr << ") -> keep same block");
		return ptr;
	}
	size_t newalloc = size + sizeof(int);
	int *newaptr = (int *)MemoryAllocSz(newalloc);
	if(!newaptr) {
		LLOG("UPP_SSL_REALLOC(" << ptr << ", " << (int)size << ", alloc " << newalloc << ") -> fail");
		return NULL;
	}
	*newaptr++ = newalloc;
	memcpy(newaptr, ptr, min<int>(*aptr - sizeof(int), size));
	UPP_SSL_alloc += newalloc - *aptr;
	LLOG("UPP_SSL_REALLOC(" << ptr << ", " << (int)size << ", alloc " << newalloc << ") -> "
		<< FormatIntHex(newaptr) << ", total = " << UPP_SSL_alloc);
	MemoryFree(aptr);
	return newaptr;
}

void TcpSocketInit();

INITBLOCK
{
	TcpSocketInit();
	MemoryIgnoreLeaksBlock __;
	CRYPTO_set_mem_functions(SslAlloc, SslRealloc, SslFree);
	SSL_library_init();
	SSL_load_error_strings();
}

EXITBLOCK
{
	CONF_modules_unload(1);
	EVP_cleanup();
	ENGINE_cleanup();
	CRYPTO_cleanup_all_ex_data();
	ERR_remove_state(0);
	ERR_free_strings();
}

static thread__ bool sThreadInit;
static thread__ void (*sPrevExit)();

static void sslExitThread()
{
	ERR_remove_state(0);
	if(sPrevExit)
		(*sPrevExit)();
}

void SslInitThread()
{
	if(sThreadInit || Thread::IsMain())
		return;
	sThreadInit = true;
	sPrevExit = Thread::AtExit(sslExitThread);
}

END_UPP_NAMESPACE
