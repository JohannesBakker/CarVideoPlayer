//{{AFX_INCLUDES()
#include "calendar.h"
//}}AFX_INCLUDES
#if !defined(AFX_CALENDARDLG_H__A8CF2E4A_6EAA_4ED7_8FCE_AA9AE4A963E8__INCLUDED_)
#define AFX_CALENDARDLG_H__A8CF2E4A_6EAA_4ED7_8FCE_AA9AE4A963E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalendarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalendarDlg dialog
class CCalendarDlg : public CDialog
{
// Construction
public:
	CCalendarDlg(CWnd* pParent = NULL);   // standard constructor
public:
	BOOL m_bCalendarDlg;
// Dialog Data
	//{{AFX_DATA(CCalendarDlg)
	enum { IDD = IDD_CALENDAR_DIALOG };
	CCalendar	m_calendar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalendarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalendarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALENDARDLG_H__A8CF2E4A_6EAA_4ED7_8FCE_AA9AE4A963E8__INCLUDED_)
