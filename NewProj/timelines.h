#if !defined(AFX_TIMELINES_H__BA61AD03_DE6F_4F08_8218_1AEFFEACC0BB__INCLUDED_)
#define AFX_TIMELINES_H__BA61AD03_DE6F_4F08_8218_1AEFFEACC0BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CTimeLines wrapper class

class CTimeLines : public CWnd
{
protected:
	DECLARE_DYNCREATE(CTimeLines)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x61eaa0f0, 0x4988, 0x11d1, { 0x8c, 0x43, 0x0, 0xa0, 0xc9, 0x59, 0xbc, 0xa } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMELINES_H__BA61AD03_DE6F_4F08_8218_1AEFFEACC0BB__INCLUDED_)
