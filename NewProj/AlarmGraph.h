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

// Operations
public:
	void setDatas(DWORD dura,TempBinData* pBinData, int count );
	void UpdateScrollSizes();

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
