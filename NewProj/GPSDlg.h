#if !defined(AFX_GPSDLG_H__07C392D0_A178_4920_B7F2_B0AB3CEC137E__INCLUDED_)
#define AFX_GPSDLG_H__07C392D0_A178_4920_B7F2_B0AB3CEC137E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GPSDlg.h : header file
//"
#include "CxStatic.h"
#include "EditTrans.h"
#include "264Spliter.h"
#include "explorer1.h"
/////////////////////////////////////////////////////////////////////////////
// CGPSDlg dialog
//typedef DWORD		(_cdecl* FuncMAP_FixGps)(double, double, int, int);
struct GPS_INFO
{
	float fLat;
	float fLon;
	float fSpeed;
};

class CGPSDlg : public CDialog
{
// Construction
public:
	void	ShowGPS_Pos(MainBinaryData*);
	CGPSDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGPSDlg)
	enum { IDD = IDD_GPS_DIALOG };
	//FuncMAP_FixGps		m_MAP_FixGps;
	bool		m_bGPSDlg;
	//}}AFX_DATA
	GPS_INFO m_gpsOldInfo;
	GPS_INFO m_gpsCurInfo;
	bool		m_bGPSUpdated;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGPSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGPSDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_WebBrowser;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPSDLG_H__07C392D0_A178_4920_B7F2_B0AB3CEC137E__INCLUDED_)
