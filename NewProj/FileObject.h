#pragma once

#include "BaseObject.h"

// CFileObject command target
typedef	HRESULT	(_cdecl* FuncCreateStreamOnFile)(CString pszFile, DWORD grfMode, DWORD dwAttributes, BOOL fCreate, IStream *pstmTemplate, IStream **ppstm);
class CFileObject : public CBaseObject
{
	DECLARE_DYNAMIC(CFileObject)

protected:
	IStream* m_pStream;

public:
	CFileObject(const CString& filePath);
	virtual ~CFileObject();

	virtual ULONGLONG	GetSizeOnDisc();
	IStream*	GetStream();
};


