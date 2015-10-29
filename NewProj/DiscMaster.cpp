///////////////////////////////////////////////////////////////////////
// DiscMaster.cpp
//
// Wrapper for IDiscMaster2 Interface
//
// Written by Eric Haddan
//
#include "StdAfx.h"
#include "DiscMaster.h"

CDiscMaster::CDiscMaster(void)
: m_discMaster(NULL)
, m_hResult(0)
{

}

CDiscMaster::~CDiscMaster(void)
{
	if (m_discMaster)
	{
		void* discMaster = m_discMaster;
		HRESULT hRes;
		__asm{
			mov		esi, discMaster;
			mov     eax, esi;
			mov     ecx, [eax];
			push	esi;
			mov     eax,dword ptr [ecx+4]
			call	eax;
			mov		hRes, eax;
		}
	//		m_discMaster->Release();
	}
}

///////////////////////////////////////////////////////////////////////
//
// CDiscMaster::Initialize()
//
// Description:
//		Creates and initializes the IDiscMaster2 interface
//
bool CDiscMaster::Initialize()
{
	ASSERT(m_discMaster == NULL);
	//
	// Initialize the IDiscMaster2 Interface
	//
	if (m_discMaster == NULL)
	{
		IID		riid;
		CLSID	MsftDiscMaster2;
		
		MsftDiscMaster2.Data1 = 0x2735412E;
		MsftDiscMaster2.Data2 = 0x7F64;
		MsftDiscMaster2.Data3 = 0x5B0F;
		MsftDiscMaster2.Data4[0] = 0x8F;
		MsftDiscMaster2.Data4[1] = 0x0;
		MsftDiscMaster2.Data4[2] = 0x5D;
		MsftDiscMaster2.Data4[3] = 0x77;
		MsftDiscMaster2.Data4[4] = 0xAF;
		MsftDiscMaster2.Data4[5] = 0xBE;
		MsftDiscMaster2.Data4[6] = 0x26;
		MsftDiscMaster2.Data4[7] = 0x1E;
		riid.Data1 = 0x27354130;
		riid.Data2 = 0x7F64;
		riid.Data3 = 0x5B0F;
		riid.Data4[0] = 0x8F;
		riid.Data4[1] = 0x0;
		riid.Data4[2] = 0x5D;
		riid.Data4[3] = 0x77;
		riid.Data4[4] = 0xAF;
		riid.Data4[5] = 0xBE;
		riid.Data4[6] = 0x26;
		riid.Data4[7] = 0x1E;
		long lResult = CoInitialize(NULL);
		HRESULT	m_hResult = CoCreateInstance(MsftDiscMaster2, NULL, CLSCTX_INPROC_SERVER,
				/*__uuidof(IDiscMaster2)*/riid, (void**)&m_discMaster);
//		m_hResult = CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER,
//			__uuidof(IDiscMaster2), (void**)&m_discMaster);

		if (!SUCCEEDED(m_hResult))
		{
			m_errorMessage.Format(_T("Unable to Initialize IDiscMaster2 - Error:0x%08x"), m_hResult);
			return false;
		}
	}

	//
	// Verify that we have some device that uses this interface
	//
	VARIANT_BOOL isSupported = VARIANT_FALSE;
	void* discMaster = m_discMaster;
	HRESULT hRes;
	__asm{
		mov		esi, discMaster;
		mov     eax, esi;
		mov     ecx, [eax];
		lea		edx, [isSupported];
		push	edx;
		push	eax;
		mov     eax,dword ptr [ecx+28h]
		call	eax;
		mov		hRes, eax;
	}
	m_hResult = hRes;
//	m_hResult = m_discMaster->get_IsSupportedEnvironment(&isSupported);
	if (!SUCCEEDED(m_hResult))
	{
		m_errorMessage.Format(_T("IDiscMaster2->get_IsSupportedEnvironment failed! - Error:0x%08x"), m_hResult);
		return false;
	}
	if (isSupported == VARIANT_FALSE)
	{
		m_errorMessage = _T("There were no writable devices detected!");
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////
//
// CDiscMaster::GetTotalDevices()
//
// Description:
//		Returns the total number of installed CD/DVD devices
//
long CDiscMaster::GetTotalDevices()
{
	ASSERT(m_discMaster != NULL);
	if (m_discMaster == NULL)
		return 0;

	long totalDevices = 0;
	void* discMaster = m_discMaster;
	HRESULT hRes;
	__asm{
		mov		esi, discMaster;
		mov     eax, esi;
		mov     ecx, [eax];
		lea		edx, [totalDevices];
		push	edx;
		push	eax;
		mov     eax,dword ptr [ecx+24h]
		call	eax;
		mov		hRes, eax;
	}
	m_hResult = hRes;
//	m_hResult = m_discMaster->get_Count(&totalDevices);
	if (FAILED(m_hResult))
	{
		m_errorMessage.Format(_T("IDiscMaster2->get_Count failed! - Error:0x%08x"), m_hResult);
		return 0;
	}

	return totalDevices;
}

///////////////////////////////////////////////////////////////////////
//
// CDiscMaster::GetDeviceUniqueID()
//
// Description:
//		Returns the unique id of the device
//
CString CDiscMaster::GetDeviceUniqueID(long index)
{
	ASSERT(m_discMaster != NULL);
	ASSERT(index < GetTotalDevices());

	BSTR	uniqueID = NULL;
	void* discMaster = m_discMaster;
	HRESULT hRes;
	__asm{
		mov		esi, discMaster;
		mov     eax, esi;
		mov     ecx, [eax];
		lea		edx, [uniqueID];
		push	edx;
		push	index;
		push	eax;
		mov     eax,dword ptr [ecx+20h]
		call	eax;
		mov		hRes, eax;
	}
	m_hResult = hRes;
//	m_hResult = m_discMaster->get_Item(index, &uniqueID);
	if (FAILED(m_hResult))
	{
		m_errorMessage.Format(_T("IDiscMaster2->get_Item(%d) failed! - Error:0x%08x"),
			index, m_hResult);
		return _T("");
	}

	return uniqueID;
}
