#if !defined(AFX_TIMERANGEDLG_H__F1EFCE39_8F48_4A9C_930C_7BBDD8C54B83__INCLUDED_)
#define AFX_TIMERANGEDLG_H__F1EFCE39_8F48_4A9C_930C_7BBDD8C54B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeRangeDlg.h : header file
//
#include <sqltypes.h>
#include "CxStatic.h"
#include "EditTrans.h"
#include "ProgressDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CTimeRangeDlg dialog

class CTimeRangeDlg : public CDialog
{
// Construction
public:
	CTimeRangeDlg(CWnd* pParent = NULL);   // standard constructor
public:
	CProgressDlg			m_ProgressDlg;
	SYSTEMTIME				m_StartDateTime;
	SYSTEMTIME				m_EndDateTime;
	CFile*					m_1stFile;
	CFile*					m_2ndFile;
	CString					m_1stFilePath;
	CString					m_2ndFilePath;
	bool					m_bRecordCurFlag;
	float					m_flt_1stStartPos, m_flt_1stEndPos, m_flt_2ndStartPos, m_flt_2ndEndPos, m_flt_2ndStopPos;
	DWORD					m_dw1stStartPos, m_dw1stEndPos, m_dw2ndStartPos, m_dw2ndEndPos;
	DWORD					m_dw2ndStopPos;
	
// Dialog Data
	//{{AFX_DATA(CTimeRangeDlg)
	enum { IDD = IDD_TIME_RANGE };
	CButton	m_btn_Cancel;
	CButton	m_btn_Ok;
	CEditTrans	m_editEndYear;
	CEditTrans	m_editEndSec;
	CEditTrans	m_editEndMonth;
	CEditTrans	m_editEndMinu;
	CEditTrans	m_editEndHour;
	CEditTrans	m_editEndDay;
	CEditTrans	m_editBeginYear;
	CEditTrans	m_editBeginSec;
	CEditTrans	m_editBeginMonth;
	CEditTrans	m_editBeginMinu;
	CEditTrans	m_editBeginHour;
	CEditTrans	m_editBeginDay;
	CxStatic	m_static_rangeend;
	CxStatic	m_static_rangebegin;
	CxStatic	m_static_endyear;
	CxStatic	m_static_endsecond;
	CxStatic	m_static_endmonth;
	CxStatic	m_static_endminute;
	CxStatic	m_static_endhour;
	CxStatic	m_static_endday;
	CxStatic	m_static_beginyear;
	CxStatic	m_static_beginsecond;
	CxStatic	m_static_beginmonth;
	CxStatic	m_static_beginminute;
	CxStatic	m_static_beginhour;
	CxStatic	m_static_beginday;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTimeRangeDlg)
	afx_msg void OnCancel();
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void UpdateWnd();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMERANGEDLG_H__F1EFCE39_8F48_4A9C_930C_7BBDD8C54B83__INCLUDED_)
