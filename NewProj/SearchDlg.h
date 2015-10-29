#if !defined(AFX_SEARCHDLG_H__C8272FB9_7B77_487C_B5D9_D0BCBF45D73F__INCLUDED_)
#define AFX_SEARCHDLG_H__C8272FB9_7B77_487C_B5D9_D0BCBF45D73F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchDlg.h : header file
//
#include "CxStatic.h"
#include "ReportCtrl.h"
#include "ImageTextButton.h"
/////////////////////////////////////////////////////////////////////////////
// CSearchDlg dialog

class CSearchDlg : public CDialog
{
// Construction
public:
	CSearchDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSearchDlg)
	enum { IDD = IDD_SEARCH };
	CButton	m_btn_Cancel;
	CButton	m_btn_ok;
	CReportCtrl	m_eventType;
	CxStatic	m_begin;
	CxStatic	m_static_date;
	CxStatic	m_static_end;
	CxStatic	m_static_path;
	CxStatic	m_static_selecteventtype;
//	wchar_t*		m_chBuf;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSearchDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnItemEventTypeClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickEventTypeLstcntlr(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickEventTypeLstcntlr(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHDLG_H__C8272FB9_7B77_487C_B5D9_D0BCBF45D73F__INCLUDED_)
