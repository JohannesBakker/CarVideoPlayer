///////////////////////////////////////////////////////////////////////
// DiscFormatData.cpp
//
// Wrapper for IDiscFormat2Data Interface
//
// Written by Eric Haddan
//

#include "StdAfx.h"
#include "DiscFormatData.h"
#include "DiscRecorder.h"
#include "DiscFormatDataEvent.h"

CDiscFormatData::CDiscFormatData(void)
: m_discFormatData(NULL)
, m_mediaTypesArray(NULL)
, m_hResult(0)
, m_hNotificationWnd(NULL)
, m_closeMedia(true)
{
}

CDiscFormatData::~CDiscFormatData(void)
{
	if (m_discFormatData != NULL)
	{
//		m_discFormatData->Release();
	}
}

///////////////////////////////////////////////////////////////////////
//
// CDiscFormatData::Initialize()
//
// Description:
//		Creates and initializes the IDiscFormat2Data interface
//
bool CDiscFormatData::Initialize(CDiscRecorder* pDiscRecorder, const CString& clientName)
{
	ASSERT(m_discFormatData == NULL);
	ASSERT(pDiscRecorder != NULL);
	if (pDiscRecorder == NULL)
	{
		m_errorMessage = _T("Error - CDiscFormatData::Initialize - pDiscRecorder is NULL");
		return false;
	}

	//
	// Initialize the IDiscFormat2Data Interface
	//
	CLSID MsftDiscFormat2Data;
	MsftDiscFormat2Data.Data1 = 0x2735412A;
	MsftDiscFormat2Data.Data2 = 0x7F64;
	MsftDiscFormat2Data.Data3 = 0x5B0F;
	MsftDiscFormat2Data.Data4[0] = 0x8F;
	MsftDiscFormat2Data.Data4[1] = 0x00;
	MsftDiscFormat2Data.Data4[2] = 0x5D;
	MsftDiscFormat2Data.Data4[3] = 0x77;
	MsftDiscFormat2Data.Data4[4] = 0xAF;
	MsftDiscFormat2Data.Data4[5] = 0xBE;
	MsftDiscFormat2Data.Data4[6] = 0x26;
	MsftDiscFormat2Data.Data4[7] = 0x1E;
	IID	riid;
	riid.Data1 = 0x27354153;
	riid.Data2 = 0x9F64;
	riid.Data3 = 0x5B0F;
	riid.Data4[0] = 0x8F;
	riid.Data4[1] = 0x00;
	riid.Data4[2] = 0x5D;
	riid.Data4[3] = 0x77;
	riid.Data4[4] = 0xAF;
	riid.Data4[5] = 0xBE;
	riid.Data4[6] = 0x26;
	riid.Data4[7] = 0x1E;
	CoInitialize(NULL);
	m_hResult = CoCreateInstance(MsftDiscFormat2Data, NULL, CLSCTX_INPROC_SERVER,
			riid, (void**)&m_discFormatData);
	if (!SUCCEEDED(m_hResult))
	{
		m_errorMessage.Format(_T("Unable to Initialize IDiscFormat2Data - Error:0x%08x"), m_hResult);
		return false;
	}

	//
	// Setup the Disc Format Information
	//
	VARIANT_BOOL isSupported = VARIANT_FALSE;
	void* discMaster = m_discFormatData;
	void* lpRecorder = pDiscRecorder->GetInterface();
	HRESULT hRes;
	__asm{
		mov		esi, discMaster;
		lea		ebx, [lpRecorder];
		mov     eax, esi;
		mov     ecx, [ebx];
		lea		edi, [isSupported];
		push	edi;
		mov		dword ptr[isSupported], 0;
		mov		edx, [eax];
		push	ecx;
		push	eax;
		mov     eax,dword ptr [edx+1Ch]
		call	eax;
		mov		hRes, eax;
	}
	isSupported = VARIANT_TRUE;
	m_hResult = hRes;
//	m_hResult = m_discFormatData->IsRecorderSupported(pDiscRecorder->GetInterface(), &isSupported);
	if (isSupported == VARIANT_FALSE)
	{
		m_errorMessage = _T("Recorder not supported");
		return false;
	}
	lpRecorder = pDiscRecorder->GetInterface();
	__asm{
		mov		esi, discMaster;
		mov     eax, esi;
		mov     ecx, [eax];
		push	lpRecorder;
		push	eax;
		mov     eax,dword ptr [ecx+30h]
		call	eax;
		mov		hRes, eax;
	}
	m_hResult = hRes;
//	m_hResult = m_discFormatData->put_Recorder(pDiscRecorder->GetInterface());
	if (!SUCCEEDED(m_hResult))
	{
		m_errorMessage.Format(_T("IDiscFormat2Data->put_Recorder Failed - Error:0x%08x"), m_hResult);
		return false;
	}
	BSTR	bStr = clientName.AllocSysString();
	__asm{
		mov		esi, discMaster;
		mov     eax, esi;
		mov     ecx, [eax];
		push	bStr;
		push	eax;
		mov     eax,dword ptr [ecx+78h]
		call	eax;
		mov		hRes, eax;
	}	
//	m_hResult = m_discFormatData->put_ClientName(clientName.AllocSysString());
	if (!SUCCEEDED(m_hResult))
	{
		m_errorMessage.Format(_T("IDiscFormat2Data->put_ClientName Failed - Error:0x%08x"), m_hResult);
		return false;
	}
	SAFEARRAY* arr = m_mediaTypesArray;
	__asm{
		mov		esi, discMaster;
		mov     eax, esi;
		mov     ecx, [eax];
		lea		edx, [arr];
		push	edx;
		push	eax;
		mov     eax,dword ptr [ecx+2Ch]
		call	eax;
		mov		hRes, eax;
	}
	m_mediaTypesArray = arr;
//	m_hResult = m_discFormatData->get_SupportedMediaTypes(&m_mediaTypesArray);
	if (!SUCCEEDED(m_hResult))
	{
		m_errorMessage.Format(_T("IDiscFormat2Data->get_SupportedMediaTypes Failed - Error:0x%08x"), m_hResult);
		return false;
	}

	return true;
}

ULONG CDiscFormatData::GetTotalSupportedMediaTypes()
{
	if (m_mediaTypesArray == NULL)
		return 0;

	return m_mediaTypesArray->rgsabound[0].cElements;
}

int CDiscFormatData::GetSupportedMediaType(ULONG index)
{
	ASSERT(index < GetTotalSupportedMediaTypes());
	if (index < GetTotalSupportedMediaTypes())
	{
		if (m_mediaTypesArray)
		{
			return ((VARIANT*)(m_mediaTypesArray->pvData))[index].intVal;
		}
	}

	return 0;
}

bool CDiscFormatData::Burn(HWND hNotificationWnd, IStream* streamData)
{
	if (m_discFormatData == NULL)
		return false;

	if (hNotificationWnd == NULL)
		return false;

	if (streamData == NULL)
		return false;

	m_streamData = streamData;
	m_hNotificationWnd = hNotificationWnd;

	// Create the event sink
	CDiscFormatDataEvent* eventSink = CDiscFormatDataEvent::CreateEventSink();
	if (eventSink == NULL)
	{
		m_errorMessage = _T("Unable to create event sink");
		return false;
	}

	if (!eventSink->ConnectDiscFormatData(this))
	{
		m_errorMessage = _T("Unable to connect event sink with interface");
		return false;
	}

	eventSink->SetHwnd(m_hNotificationWnd);
	void* discMaster = m_discFormatData;
	HRESULT hRes;
	m_closeMedia ? VARIANT_TRUE : VARIANT_FALSE;
	bool bCloseMedia = m_closeMedia;
	__asm{
		mov		esi, discMaster;
		mov     eax, esi;
		mov     ecx, [eax];
		lea		edx, [bCloseMedia];
		push	edx;
		push	eax;
		mov     eax,dword ptr [ecx+0xAC]
		call	eax;
		mov		hRes, eax;
	}
	m_hResult = hRes;
//	m_discFormatData->put_ForceMediaToBeClosed(m_closeMedia ? VARIANT_TRUE : VARIANT_FALSE);
	IStream* stream = m_streamData;
	__asm{
		mov		esi, discMaster;
		mov     eax, esi;
		mov     ecx, [eax];
		push	stream;
		push	eax;
		mov     eax,dword ptr [ecx+0xA4]
		call	eax;
		mov		hRes, eax;
	}
	m_hResult = hRes;
//	m_hResult = m_discFormatData->Write(m_streamData);
	delete eventSink;

	if (SUCCEEDED(m_hResult))
	{
		return true;
	}

	m_errorMessage.Format(_T("IDiscFormat2Data->Write Failed! Error:0x%08x"),
		m_hResult);

	return true;
}

