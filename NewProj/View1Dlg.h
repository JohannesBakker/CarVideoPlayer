#if !defined(AFX_VIEW1DLG_H__48B2E9D4_DC0A_478C_BCBF_1D1071792514__INCLUDED_)
#define AFX_VIEW1DLG_H__48B2E9D4_DC0A_478C_BCBF_1D1071792514__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View1Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView1Dlg dialog

class CView1Dlg : public CDialog
{
// Construction
public:
	CView1Dlg(CWnd* pParent = NULL);   // standard constructor
public:
	bool m_bTwoWindow;
	bool m_bOneWindow;
	bool m_bView1DB;
	bool		m_bAllUpdown;
	bool		m_bView1Dlg;
static 	CMenu*		m_pMenu;
// Dialog Data
	//{{AFX_DATA(CView1Dlg)
	enum { IDD = IDD_VIEW1 };
	CStatic	m_static_view1;
	CStatic	m_Static_View;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CView1Dlg)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuUpDown();
	afx_msg void OnMenuAllUpDown();
	afx_msg void OnMenuShowChanNum();
	afx_msg void OnMenuUseRgb16();
	afx_msg void OnMenuWmrmkVerify();
	afx_msg void OnMenuViewScale16_9();
	afx_msg void OnMenuViewScale4_3();
	afx_msg void OnMenuViewScaleAuto();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMenuOneWindow();
	afx_msg void OnMenuTwoWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuTurning();
	afx_msg void OnMenuAllTurning();
	afx_msg void OnMenuRestoreSize();
	afx_msg void OnMenuRetoreAllSize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW1DLG_H__48B2E9D4_DC0A_478C_BCBF_1D1071792514__INCLUDED_)
