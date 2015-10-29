#if !defined(AFX_RECORDPLAYERABOUTDLG_H__FE7FB58A_D026_44DA_A90C_DBA9D26D028C__INCLUDED_)
#define AFX_RECORDPLAYERABOUTDLG_H__FE7FB58A_D026_44DA_A90C_DBA9D26D028C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordPlayerAboutDlg.h : header file
//
#include "CxStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CRecordPlayerAboutDlg dialog

class CRecordPlayerAboutDlg : public CDialog
{
// Construction
public:
	CRecordPlayerAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordPlayerAboutDlg)
	enum { IDD = IDD_ABOUT };
	CButton	m_btn_Ok;
	CxStatic		m_static_icon;
	CxStatic	m_static_copyright;
	CxStatic	m_static_version;
	CxStatic	m_static_about;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordPlayerAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordPlayerAboutDlg)
	afx_msg void OnOkButton();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDPLAYERABOUTDLG_H__FE7FB58A_D026_44DA_A90C_DBA9D26D028C__INCLUDED_)
