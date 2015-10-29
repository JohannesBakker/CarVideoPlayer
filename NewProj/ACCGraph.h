#if !defined(AFX_ACCGRAPH_H__4771DEA1_C2F3_449E_99DD_97C5448EB227__INCLUDED_)
#define AFX_ACCGRAPH_H__4771DEA1_C2F3_449E_99DD_97C5448EB227__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ACCGraph.h : header file
//
#include "264Spliter.h"
/////////////////////////////////////////////////////////////////////////////
// CACCGraph window

class CACCGraph : public CStatic
{
// Construction
public:
	CACCGraph();

// Attributes
public:
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACCGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	int		m_accId;
	bool	m_bAcc;
	DWORD m_dura;
	static TempBinData m_pACCBinData[100000];
	int   m_BinCount;
	int		m_nAccSpeedCount;
	void setDatas(DWORD,TempBinData*,int);
	virtual ~CACCGraph();

	// Generated message map functions
protected:
	//{{AFX_MSG(CACCGraph)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCGRAPH_H__4771DEA1_C2F3_449E_99DD_97C5448EB227__INCLUDED_)
