//{{AFX_INCLUDES()
#include "timelines.h"
//}}AFX_INCLUDES
#if !defined(AFX_TIMELINEDLG_H__9228D71F_55DE_4C12_ACD0_9755425259AE__INCLUDED_)
#define AFX_TIMELINEDLG_H__9228D71F_55DE_4C12_ACD0_9755425259AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeLineDlg.h : header file
//
#include "ImageTextButton.h"
#include "CxStatic.h"
#include "BtnST.h"
#include <sqltypes.h>
/////////////////////////////////////////////////////////////////////////////
// CTimeLineDlg dialog

class CTimeLineDlg : public CDialog
{
// Construction
public:
	CTimeLineDlg(CWnd* pParent = NULL);   // standard constructor
	bool SetTimeRects(tagTIMESTAMP_STRUCT, DWORD, int);
	void InitTimeLine();
	void GetMinuteRect(SYSTEMTIME, DWORD);
	void GetHourRect(SYSTEMTIME, DWORD);
public:
	void PaintHourMinuteLine();
    static CxStatic	m_staticTimes[24];
	CxStatic	m_staticMinutes[13];
	bool		m_bTimeLineDlg;
	bool		m_bPaint;
	bool		m_bPaintMinute;
	wchar_t*		m_chBuf;
	int			m_nLastSelStatic;
	tagTIMESTAMP_STRUCT		m_StartTime;
	tagTIMESTAMP_STRUCT		m_EndTime;
	int			m_nCurHour;
	DWORD		m_dwDuration;
	int			m_nChannels;
	CRect		m_HourRect;
	CRect		m_MinuteRect;
static	bool		m_bTimeFlag;
static  float		m_fTimeSeekPos;
static	bool		m_bTimeSeekFlag;
// Dialog Data
	//{{AFX_DATA(CTimeLineDlg)
	enum { IDD = IDD_TIMELINE_DIALOG };
	CxStatic	m_static_minute;
	CxStatic	m_static_hour;
	CxStatic	m_static_camera2;
	CxStatic	m_static_camera1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTimeLineDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMELINEDLG_H__9228D71F_55DE_4C12_ACD0_9755425259AE__INCLUDED_)
