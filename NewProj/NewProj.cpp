// NewProj.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewProjApp

BOOL g_bEnableQuitApplication = TRUE;

BEGIN_MESSAGE_MAP(CNewProjApp, CWinApp)
	//{{AFX_MSG_MAP(CNewProjApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewProjApp construction

CNewProjApp::CNewProjApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNewProjApp object

CNewProjApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNewProjApp initialization

BOOL CNewProjApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	CNewProjDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


BOOL CNewProjApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	// When press any key, close applications
	if (pMsg->message == WM_KEYDOWN)
	{
		if (g_bEnableQuitApplication == TRUE) 
		{
			CFile file;
			file.Open(_T(CONFIG_FILE_PATH), CFile::modeWrite);
			file.Write(&CNewProjDlg::g_config_Value_ST, sizeof(Config_Datas));
			file.Close();


			AfxGetMainWnd()->SendMessage(WM_CLOSE);
		}
		
	}

	return CWinApp::PreTranslateMessage(pMsg);
}
