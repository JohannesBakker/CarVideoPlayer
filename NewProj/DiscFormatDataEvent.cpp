///////////////////////////////////////////////////////////////////////
// DiscFormatDataEvent.cpp
//
// Wrapper for DDiscFormat2DataEvents Interface Events
//
// Written by Eric Haddan
//

#include "stdafx.h"
#include <afxctl.h>
#include <oaidl.h>
#include "DiscFormatDataEvent.h"

#include "DiscFormatData.h"

// CDiscFormatDataEvent
#define IMAPILib2_MajorVersion 1
#define IMAPILib2_MinorVersion 0

#define IMAPI_FORMAT2_DATA_WRITE_ACTION_WRITING_DATA 4
#define IMAPI_FORMAT2_DATA_WRITE_ACTION_FINALIZATION 5
IMPLEMENT_DYNAMIC(CDiscFormatDataEvent, CCmdTarget)

BEGIN_INTERFACE_MAP(CDiscFormatDataEvent, CCmdTarget)
	INTERFACE_PART(CDiscFormatDataEvent, IID_IDispatch, FormatDataEvents)
	INTERFACE_PART(CDiscFormatDataEvent, IID_DDiscFormat2DataEvents, FormatDataEvents)
END_INTERFACE_MAP()

CDiscFormatDataEvent::CDiscFormatDataEvent()
: m_hNotifyWnd(NULL)
, m_ptinfo(NULL)
, m_dwCookie(0)
, m_pUnkSink(0)
, m_pUnkSrc(0)
{
}

CDiscFormatDataEvent::~CDiscFormatDataEvent()
{
	if (m_dwCookie && (m_pUnkSrc != NULL) && (m_pUnkSink != NULL))
	{
		AfxConnectionUnadvise(m_pUnkSrc, IID_DDiscFormat2DataEvents, m_pUnkSink,
			TRUE, m_dwCookie);
	}
}


BEGIN_MESSAGE_MAP(CDiscFormatDataEvent, CCmdTarget)
END_MESSAGE_MAP()



// CDiscFormatDataEvent message handlers

ULONG FAR EXPORT CDiscFormatDataEvent::XFormatDataEvents::AddRef()
{
	METHOD_PROLOGUE(CDiscFormatDataEvent, FormatDataEvents)
	return pThis->ExternalAddRef();
}
ULONG FAR EXPORT CDiscFormatDataEvent::XFormatDataEvents::Release()
{
	METHOD_PROLOGUE(CDiscFormatDataEvent, FormatDataEvents)
	return pThis->ExternalRelease();
}
STDMETHODIMP CDiscFormatDataEvent::XFormatDataEvents::QueryInterface(REFIID riid,
												   LPVOID FAR* ppvObj)
{
	METHOD_PROLOGUE(CDiscFormatDataEvent, FormatDataEvents)
	return (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
}
STDMETHODIMP
CDiscFormatDataEvent::XFormatDataEvents::GetTypeInfoCount(UINT FAR* pctinfo)
{
	METHOD_PROLOGUE(CDiscFormatDataEvent, FormatDataEvents)
	*pctinfo = 1;
	return NOERROR;
}
STDMETHODIMP CDiscFormatDataEvent::XFormatDataEvents::GetTypeInfo(
	UINT itinfo,
	LCID lcid,
	ITypeInfo FAR* FAR* pptinfo)
{
	METHOD_PROLOGUE(CDiscFormatDataEvent, FormatDataEvents)
	*pptinfo = NULL;

	if(itinfo != 0)
		return ResultFromScode(DISP_E_BADINDEX);
	pThis->m_ptinfo->AddRef();
	*pptinfo = pThis->m_ptinfo;
	return NOERROR;
}
STDMETHODIMP CDiscFormatDataEvent::XFormatDataEvents::GetIDsOfNames(
	REFIID riid,
	OLECHAR FAR* FAR* rgszNames,
	UINT cNames,
	LCID lcid,
	DISPID FAR* rgdispid)
{
	METHOD_PROLOGUE(CDiscFormatDataEvent, FormatDataEvents)
	return DispGetIDsOfNames(pThis->m_ptinfo, rgszNames, cNames, rgdispid);
}
STDMETHODIMP CDiscFormatDataEvent::XFormatDataEvents::Invoke(
	DISPID dispidMember,
	REFIID riid,
	LCID lcid,
	WORD wFlags,
	DISPPARAMS FAR* pdispparams,
	VARIANT FAR* pvarResult,
	EXCEPINFO FAR* pexcepinfo,
	UINT FAR* puArgErr)
{
	METHOD_PROLOGUE(CDiscFormatDataEvent, FormatDataEvents)

	return DispInvoke(&pThis->m_xFormatDataEvents, pThis->m_ptinfo,
		dispidMember, wFlags, pdispparams, pvarResult, pexcepinfo, puArgErr);
}


///////////////////////////////////////////////////////////////////////
//
// CDiscFormatDataEvent::CreateEventSink
//
// Description:
//			Establishes a link between the CDiscFormatData(IDiscFormat2Data)
//			and the CDiscFormatDataEvent(DDiscFormat2DataEvents) so 
//			CDiscFormatDataEvent can receive Update messages
//
CDiscFormatDataEvent* CDiscFormatDataEvent::CreateEventSink()
{
	// Create the event sink
	CDiscFormatDataEvent* pDiscFormatDataEvent = new CDiscFormatDataEvent();

	pDiscFormatDataEvent->EnableAutomation();
	pDiscFormatDataEvent->ExternalAddRef();

	return pDiscFormatDataEvent;
}

///////////////////////////////////////////////////////////////////////
//
// CDiscFormatDataEvent::ConnectDiscFormatData
//
// Description:
//			Establishes a link between the CDiscFormatData(IDiscFormat2Data)
//			and the CDiscFormatDataEvent(DDiscFormat2DataEvents) so 
//			CDiscFormatDataEvent can receive Update messages
//
bool CDiscFormatDataEvent::ConnectDiscFormatData(CDiscFormatData* pDiscFormatData)
{
	m_pUnkSink = GetIDispatch(FALSE);
	m_pUnkSrc = (IUnknown*)pDiscFormatData->GetInterface();
	GUID	rguid;
	rguid.Data1 = 0x2735412F;
	rguid.Data2 = 0x7F64;
	rguid.Data3 = 0x5B0F;
	rguid.Data4[0] = 0x8F;
	rguid.Data4[1] = 0x00;
	rguid.Data4[2] = 0x5D;
	rguid.Data4[3] = 0x77;
	rguid.Data4[4] = 0xAF;
	rguid.Data4[5] = 0xBE;
	rguid.Data4[6] = 0x26;
	rguid.Data4[7] = 0x1E;

	LPTYPELIB ptlib = NULL;
	HRESULT hr = LoadRegTypeLib(rguid, 
		IMAPILib2_MajorVersion, IMAPILib2_MinorVersion, 
		LOCALE_SYSTEM_DEFAULT, &ptlib);
	if (FAILED(hr))
	{
		return false;
	}
	hr = ptlib->GetTypeInfoOfGuid(IID_DDiscFormat2DataEvents, &m_ptinfo);
    ptlib->Release();
	if (FAILED(hr))
	{
	    return false;
	}

	BOOL bRet = AfxConnectionAdvise(m_pUnkSrc, IID_DDiscFormat2DataEvents, m_pUnkSink,
		TRUE, &m_dwCookie);
	if (bRet)
	{
		return true;
	}

	return false;
}


///////////////////////////////////////////////////////////////////////
//
// CDiscFormatDataEvent::Update
//
// Description:
//			Receives update notifications from IDiscFormat2Data
//
STDMETHODIMP_(HRESULT) CDiscFormatDataEvent::XFormatDataEvents::Update(IDispatch* objectDispatch, IDispatch* progressDispatch)
{
	METHOD_PROLOGUE(CDiscFormatDataEvent, FormatDataEvents)

    void* progress = NULL;//IDiscFormat2DataEventArgs
	IID riid;
	riid.Data1 = 0x2735413D;
	riid.Data2 = 0x7F64;
	riid.Data3 = 0x5B0F;
	riid.Data4[0] = 0x8F;
	riid.Data4[1] = 0x00;
	riid.Data4[2] = 0x5D;
	riid.Data4[3] = 0x77;
	riid.Data4[4] = 0xAF;
	riid.Data4[5] = 0xBE;
	riid.Data4[6] = 0x26;
	riid.Data4[7] = 0x1E;
    HRESULT hr = progressDispatch->QueryInterface(riid, &progress);

	void* discFormatData = NULL;//IDiscFormat2Data
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
    hr = objectDispatch->QueryInterface(riid, &discFormatData);

    //IMAPI_FORMAT2_DATA_WRITE_ACTION currentAction = IMAPI_FORMAT2_DATA_WRITE_ACTION_VALIDATING_MEDIA;
	DWORD currentAction;
	__asm{
		mov		esi, progress;
		mov     eax, esi;
		mov     ecx, [eax];
		lea		edx, [currentAction];
		push	edx;
		push	eax;
		mov     eax,dword ptr [ecx+44h]
		call	eax;
		mov		hr, eax;
	}
//	hr = progress->get_CurrentAction(&currentAction);
	ASSERT(SUCCEEDED(hr));
	if (FAILED(hr))
	{
		return S_OK;
	}

	IMAPI_STATUS imapiStatus = {0};

	if ((currentAction == IMAPI_FORMAT2_DATA_WRITE_ACTION_WRITING_DATA) ||
		(currentAction == IMAPI_FORMAT2_DATA_WRITE_ACTION_FINALIZATION))
	{
		__asm{
			mov		esi, progress;
			mov     eax, esi;
			mov     ecx, [eax];
			lea		edx, [imapiStatus.elapsedTime];
			push	edx;
			push	eax;
			mov     eax,dword ptr [ecx+38h]
			call	eax;
		}
//		progress->get_ElapsedTime(&imapiStatus.elapsedTime);
		__asm{
			mov		esi, progress;
			mov     eax, esi;
			mov     ecx, [eax];
			lea		edx, [imapiStatus.remainingTime];
			push	edx;
			push	eax;
			mov     eax,dword ptr [ecx+3Ch]
			call	eax;
		}
//		progress->get_RemainingTime(&imapiStatus.remainingTime);
		__asm{
			mov		esi, progress;
			mov     eax, esi;
			mov     ecx, [eax];
			lea		edx, [imapiStatus.totalTime];
			push	edx;
			push	eax;
			mov     eax,dword ptr [ecx+40h]
			call	eax;
		}
//		progress->get_TotalTime(&imapiStatus.totalTime);

		if (currentAction == IMAPI_FORMAT2_DATA_WRITE_ACTION_WRITING_DATA)
		{
			__asm{
				mov		esi, progress;
				mov     eax, esi;
				mov     ecx, [eax];
				lea		edx, [imapiStatus.startLba];
				push	edx;
				push	eax;
				mov     eax,dword ptr [ecx+1Ch]
				call	eax;
			}
//			progress->get_StartLba(&imapiStatus.startLba);
			__asm{
				mov		esi, progress;
				mov     eax, esi;
				mov     ecx, [eax];
				lea		edx, [imapiStatus.sectorCount];
				push	edx;
				push	eax;
				mov     eax,dword ptr [ecx+20h]
				call	eax;
			}
//			progress->get_SectorCount(&imapiStatus.sectorCount);
			__asm{
				mov		esi, progress;
				mov     eax, esi;
				mov     ecx, [eax];
				lea		edx, [imapiStatus.lastReadLba];
				push	edx;
				push	eax;
				mov     eax,dword ptr [ecx+24h]
				call	eax;
			}
//			progress->get_LastReadLba(&imapiStatus.lastReadLba);
			__asm{
				mov		esi, progress;
				mov     eax, esi;
				mov     ecx, [eax];
				lea		edx, [imapiStatus.lastWrittenLba];
				push	edx;
				push	eax;
				mov     eax,dword ptr [ecx+28h]
				call	eax;
			}
//			progress->get_LastWrittenLba(&imapiStatus.lastWrittenLba);
			__asm{
				mov		esi, progress;
				mov     eax, esi;
				mov     ecx, [eax];
				lea		edx, [imapiStatus.totalSystemBuffer];
				push	edx;
				push	eax;
				mov     eax,dword ptr [ecx+2Ch]
				call	eax;
			}
//			progress->get_TotalSystemBuffer(&imapiStatus.totalSystemBuffer);
			__asm{
				mov		esi, progress;
				mov     eax, esi;
				mov     ecx, [eax];
				lea		edx, [imapiStatus.usedSystemBuffer];
				push	edx;
				push	eax;
				mov     eax,dword ptr [ecx+30h]
				call	eax;
			}
//			progress->get_UsedSystemBuffer(&imapiStatus.usedSystemBuffer);
			__asm{
				mov		esi, progress;
				mov     eax, esi;
				mov     ecx, [eax];
				lea		edx, [imapiStatus.freeSystemBuffer];
				push	edx;
				push	eax;
				mov     eax,dword ptr [ecx+34h]
				call	eax;
			}
//			progress->get_FreeSystemBuffer(&imapiStatus.freeSystemBuffer);
		}
	}

	LRESULT ret = ::SendMessage(pThis->m_hNotifyWnd, WM_IMAPI_UPDATE, currentAction, (LPARAM)(LPVOID)&imapiStatus);
	if (ret == RETURN_CANCEL_WRITE)
	{
		__asm{
			mov		esi, progress;
			mov     eax, esi;
			mov     ecx, [eax];
			push	eax;
			mov     eax,dword ptr [ecx+3Ch]
			call	eax;
		}
//		discFormatData->CancelWrite();
	}

	return S_OK;
}
