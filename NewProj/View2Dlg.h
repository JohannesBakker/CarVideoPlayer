#if !defined(AFX_VIEW2DLG_H__DA6C8B97_71FA_4DC3_AF29_E146335C5F48__INCLUDED_)
#define AFX_VIEW2DLG_H__DA6C8B97_71FA_4DC3_AF29_E146335C5F48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View2Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView2Dlg dialog

class CView2Dlg : public CDialog
{
// Construction
public:
	CView2Dlg(CWnd* pParent = NULL);   // standard constructor
public:
	bool m_bOneWindow;
	bool m_bView2DB;
	bool		m_bAllUpdown;
	bool		m_bView2Dlg;
	CMenu*		m_pMenu;
// Dialog Data
	//{{AFX_DATA(CView2Dlg)
	enum { IDD = IDD_VIEW2 };
	CStatic	m_static_view2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView2Dlg)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CView2Dlg)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuUpDown();
	afx_msg void OnMenuAllUpDown();
	afx_msg void OnMenuViewScale16_9();
	afx_msg void OnMenuViewScale4_3();
	afx_msg void OnMenuViewScaleAuto();
	afx_msg void OnMenuOneWindow();
	afx_msg void OnMenuTwoWindow();
	afx_msg void OnStaticView2();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMenuTurning();
	afx_msg void OnMenuAllTurning();
	afx_msg void OnMenuShowChanNum();
	afx_msg void OnMenuWmrmkVerify();
	afx_msg void OnMenuUseRgb16();
	afx_msg void OnMenuRestoreSize();
	afx_msg void OnMenuRetoreAllSize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CView2Dlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW2DLG_H__DA6C8B97_71FA_4DC3_AF29_E146335C5F48__INCLUDED_)
