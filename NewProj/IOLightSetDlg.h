#if !defined(AFX_IOLIGHTSETDLG_H__9DC782CC_5ECA_4591_809A_C945AFF75508__INCLUDED_)
#define AFX_IOLIGHTSETDLG_H__9DC782CC_5ECA_4591_809A_C945AFF75508__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IOLightSetDlg.h : header file
//
#include "CxStatic.h"
#include "xCheckButton.h"
/////////////////////////////////////////////////////////////////////////////
// CIOLightSetDlg dialog

class CIOLightSetDlg : public CDialog
{
// Construction
public:
	bool m_bShowIOCheck;
	CIOLightSetDlg(CWnd* pParent = NULL);   // standard constructor
	static bool g_bShowIO_ST;
// Dialog Data
	//{{AFX_DATA(CIOLightSetDlg)
	enum { IDD = IDD_IO_LIGHT_SET };
	CComboBox	m_cmbSignal3;
	CComboBox	m_cmbSignal2;
	CComboBox	m_cmbSignal1;
	CButton	m_btn_cancel;
	CButton	m_btn_ok;
	CxCheckButton		m_checkShowIO;
	CxStatic	m_static_io3;
	CxStatic	m_static_io2;
	CxStatic	m_static_io1;
	WCHAR*		m_chBuf;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIOLightSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIOLightSetDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnShowIoLightsCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IOLIGHTSETDLG_H__9DC782CC_5ECA_4591_809A_C945AFF75508__INCLUDED_)
