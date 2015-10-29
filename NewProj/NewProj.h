// NewProj.h : main header file for the NEWPROJ application
//

#if !defined(AFX_NEWPROJ_H__5810EFFE_0716_4698_9BC1_45FA2256D64D__INCLUDED_)
#define AFX_NEWPROJ_H__5810EFFE_0716_4698_9BC1_45FA2256D64D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNewProjApp:
// See NewProj.cpp for the implementation of this class
//

class CNewProjApp : public CWinApp
{
public:
	CNewProjApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNewProjApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJ_H__5810EFFE_0716_4698_9BC1_45FA2256D64D__INCLUDED_)
