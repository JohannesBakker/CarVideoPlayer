#if !defined(AFX_ALARMGRAPH_H__38264522_FB5C_4124_98DD_BBF13D2A6E4C__INCLUDED_)
#define AFX_ALARMGRAPH_H__38264522_FB5C_4124_98DD_BBF13D2A6E4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmGraph.h : header file
//
#include "264Spliter.h"
/////////////////////////////////////////////////////////////////////////////
// CAlarmGraph window

typedef struct {
	DWORD	alarmOffsetTime;
	unsigned char	alarmFlags;
	MainBinaryData	alamBinData;
} AlarmBinData_t;

enum {
	IO_ALARM_SIG_1 = 0,
	IO_ALARM_SIG_2,
	IO_ALARM_SIG_3,
	IO_ALARM_SIG_NUM,
};


class CAlarmGraph : public CWnd
{
// Construction
public:
	CAlarmGraph(CWnd*, CRect, UINT uID);

// Attributes
public:
	DWORD m_dura;
	static TempBinData m_pAlarmBinData[100000];
	int		m_BinCount;
	int		m_nAlarmCount;
	int		m_curPosX;
	int		m_curPosY;

	DWORD	m_dwVideoStartFilePos;
	DWORD	m_dwCurretFilePos;
	DWORD	m_dwSizePerData;
	int		m_nTotalPoints;
	int		m_nPointsPerPage;
	int		m_nStartNo;
	int		m_nOffsetX;

	DWORD	m_dwFirstDTS;
	CArray<AlarmBinData_t, AlarmBinData_t> m_arrBinData;
	bool	m_bIoAlarmSigsON[IO_ALARM_SIG_NUM];

// Operations
public:
	void setDatas(DWORD dura,TempBinData* pBinData, int count );
	void UpdateScrollSizes();	
	void ClearAlarmGraph();

	void UpdateScrollSizesExt();

	void setDatasWithTime(DWORD dwFristDTS, DWORD currDTS, MainBinaryData* pBinData, unsigned char alarmFlag);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarmGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAlarmGraph();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAlarmGraph)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMGRAPH_H__38264522_FB5C_4124_98DD_BBF13D2A6E4C__INCLUDED_)
