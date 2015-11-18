#if !defined(AFX_SPEEDGRAPH_H__26233C99_BF7E_483B_915A_3C2390192C69__INCLUDED_)
#define AFX_SPEEDGRAPH_H__26233C99_BF7E_483B_915A_3C2390192C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpeedGraph.h : header file
//
#include "264Spliter.h"
#include "type.h"
/////////////////////////////////////////////////////////////////////////////
// CSpeedGraph window

class CSpeedGraph : public CStatic
{
// Construction
public:
	CSpeedGraph();

// Attributes
public:
	DWORD m_dura;
	static TempBinData m_pSpeedBinData[100000];
	int   m_BinCount;
	int			m_nSpeedCount;
	SpeedUnit_t	m_nSpeedUnit;
// Operations
public:
	void setDatas(DWORD dura,TempBinData* pBinData, int count );
	void SetSpeedUnit(int nSpeedUnit);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpeedGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpeedGraph();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpeedGraph)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPEEDGRAPH_H__26233C99_BF7E_483B_915A_3C2390192C69__INCLUDED_)
