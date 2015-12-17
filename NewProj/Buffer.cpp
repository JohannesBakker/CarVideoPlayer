// Buffer.cpp: implementation of the CBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Buffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuffer::CBuffer() 
{
	ptr.b = NULL;
	ByteLen = 0;
	m_bAutoDelete = false;
}

CBuffer::CBuffer(DWORD size, bool AutoDelete)
{
	allocBuffer(size, AutoDelete);
}

CBuffer::CBuffer(void* buffer, DWORD length)
{
	setBuffer(buffer, length);
}

CBuffer::~CBuffer()
{
	// remember to delete the memory
	if (m_bAutoDelete && ptr.b != NULL)  delete ptr.b;
}

void CBuffer::Erase()
{
	if (ptr.b) ZeroMemory(ptr.b,ByteLen);
}

void CBuffer::allocBuffer(DWORD size, bool AutoDelete)
{
	
	m_bAutoDelete = AutoDelete;
	
	ByteLen = 0;
	
	try
	{
		ptr.b = new BYTE[size];
		if (ptr.b) ByteLen = size;
	}
	catch(...)
	{
		ErrorMsg(_T("Out of memory!"));
	}
}

void CBuffer::setBuffer(void* buffer, DWORD length)
{
	m_bAutoDelete = false;
	if (buffer)
	{
		ptr.v = buffer;
		ByteLen = length;
	}
	else
	{
		ptr.b = NULL;
		ByteLen = 0;
	}
}

