#include "Core.h"

NAMESPACE_UPP

InFilterStream::InFilterStream()
{
	Init();
}

void InFilterStream::Init()
{
	static byte h[1];
	ptr = rdlim = h;
	in = NULL;
	eof = false;
}

dword InFilterStream::Avail()
{
	return dword(rdlim - ptr);
}

bool InFilterStream::IsOpen() const
{
	return in->IsOpen();
}

int InFilterStream::_Term()
{
	if(ptr == rdlim)
		Fetch(1);
	return ptr == rdlim ? -1 : *ptr;
}

int InFilterStream::_Get()
{
	if(ptr == rdlim)
		Fetch(1);
	return ptr == rdlim ? -1 : *ptr++;
}

dword InFilterStream::_Get(void *data, dword size)
{
	byte *p = (byte *)data;
	int nn = 0;
	for(int pass = 0; size && pass < 2; pass++) {
		int n = min(size, Avail());
		memcpy(p, ptr, n);
		size -= n;
		p += n;
		ptr += n;
		nn += n;
		if(size)
			Fetch(size);
	}
	return nn;
}

void InFilterStream::Out(const void *ptr, int size)
{
	int l = buffer.GetCount();
	buffer.SetCount(l + size);
	memcpy(buffer.Begin() + l, ptr, size);
}

void InFilterStream::Fetch(int size)
{
	buffer.Clear();
	if(eof) {
		static byte h[1];
		ptr = rdlim = h;
		return;
	}
	if(More)
		while(buffer.GetCount() < size && More());
	else {
		Buffer<byte> b(4096);
		while(buffer.GetCount() < size) {
			int n = in->Get(~b, 4096);
			if(n == 0) {
				End();
				eof = true;
				break;
			}
			Filter(~b, n);
		}
	}
	ptr = buffer.Begin();
	rdlim = buffer.End();
}

OutFilterStream::OutFilterStream()
{
	Init();
}

void OutFilterStream::Init()
{
	buffer.Alloc(4096);
	wrlim = ~buffer + 4096;
	ptr = ~buffer;
	out = NULL;
}

OutFilterStream::~OutFilterStream()
{
	Close();
}

void OutFilterStream::Close()
{
	if(buffer) {
		FlushOut();
		End();
		buffer.Clear();
		out->Close();
	}
}

void OutFilterStream::FlushOut()
{
	if(ptr != ~buffer) {
		Filter(~buffer, (int)(ptr - ~buffer));
		ptr = ~buffer;
	}
}

void OutFilterStream::_Put(int w)
{
	FlushOut();
	*ptr++ = w;
}

void OutFilterStream::_Put(const void *data, dword size)
{
	const byte *p = (const byte *)data;
	for(;;) {
		int n = min(Avail(), size);
		memcpy(ptr, p, n);
		size -= n;
		p += n;
		ptr += n;
		if(size == 0)
			return;
		FlushOut();
	}
}

bool OutFilterStream::IsOpen() const
{
	return buffer;
}

void OutFilterStream::Out(const void *ptr, int size)
{
	out->Put(ptr, size);
}

END_UPP_NAMESPACE
