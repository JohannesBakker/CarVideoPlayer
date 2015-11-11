#if !defined(AFX_INFOLISTDLG_H__D10136C6_C8F4_4781_9ED8_8D9F1ABFE031__INCLUDED_)
#define AFX_INFOLISTDLG_H__D10136C6_C8F4_4781_9ED8_8D9F1ABFE031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoListDlg.h : header file
//
#include "ReportCtrl.h"
#include "264Spliter.h"
#include "BatchConvertDlg.h"
#include "ACCGraph.h"
#include "SpeedGraph.h"
#include "AlarmGraph.h"
#include <sqltypes.h>
/////////////////////////////////////////////////////////////////////////////
// CInfoListDlg dialog

class CInfoListDlg : public CDialog
{
// Construction
public:
	CInfoListDlg(CWnd* pParent = NULL);   // standard constructor
public:
	void load_file_list(DWORD,SYSTEMTIME ,int,DWORD,DWORD, int);
	void load_event_list(CString,SYSTEMTIME);
	void DrawGraph(MainBinaryData* pData,DWORD curPos,DWORD dura,DWORD nBinSize,DWORD totalSize,bool bSeek, DWORD dwFristDTS, DWORD currDTS, unsigned char alarmFlag);
	void DrawAccelSpeedGraph(MainBinaryData* , DWORD  , DWORD, DWORD );
	void DrawSpeedGraph(MainBinaryData* , DWORD  , DWORD , DWORD, bool);
	void DrawAlarmGraph(DWORD dwFristDTS, DWORD currDTS, MainBinaryData* pData, unsigned char alarmFlag);
	void ClearAlarmGraph();

public:
	bool			m_bInfoList;
	bool			m_bState;
	static TempBinData m_pBinData[100000];
	int			m_BinDataCount;
	int			m_nSpeedCount;
	int			m_nAlarmCount;
	int			m_nAccSpeedCount;
	CFont       m_font;
static	int		g_TabCurSel;
	DWORD		m_dura;
//	DrawPos*    m_pos;
	C264Spliter m_pSplitter;
	int		m_xAcc;
	int		m_yAccR;
	int		m_yAccG;
	int		m_yAccB;
	int		m_accId;
	bool	m_bAcc;
	
	int		m_ySpeed;
	int		m_xSpeed;
	int		m_xAlarm;
	int		m_yLights;
	int		m_yMan;
	int		m_y24;
	int		m_yIn4;
	int		m_yIn5;
	int		m_yIn6;
	int		m_yIO7;
	int		m_yIO8;

	CAlarmGraph	*m_alarmGraphWnd;
// Dialog Data
	//{{AFX_DATA(CInfoListDlg)
	enum { IDD = IDD_INFO_DIALOG };
	
	CSpeedGraph	m_speedGraphWnd;
	CACCGraph	m_accGraphWnd;
	CStatic	m_static_Speed;
	CReportCtrl m_fileList;
	CReportCtrl m_eventList;
	CReportCtrl m_logList;
	CReportCtrl m_diagList;
	CTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoListDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOLISTDLG_H__D10136C6_C8F4_4781_9ED8_8D9F1ABFE031__INCLUDED_)
