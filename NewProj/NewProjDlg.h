// NewProjDlg.h : header file
//

#if !defined(AFX_NEWPROJDLG_H__70FEBDBF_5290_4EB7_8A74_A2CDD019B490__INCLUDED_)
#define AFX_NEWPROJDLG_H__70FEBDBF_5290_4EB7_8A74_A2CDD019B490__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "264Demuxer.h"
#include "264Spliter.h"
#include "View1Dlg.h"
#include "View2Dlg.h"
#include "ToolBarDlg.h"
#include "SystemSetupDlg.h"
#include "RecordPlayerAboutDlg.h"
#include "IOLightSetDlg.h"
#include "SearchDlg.h"
#include "BurnToDiscDlg.h"
#include "GPSDlg.h"
#include "TimeLineDlg.h"
#include "InfoListDlg.h"
#include "type.h"
#include <sqltypes.h>
/////////////////////////////////////////////////////////////////////////////
// CNewProjDlg dialog

enum{
	SCALEMODE_16_9,
	SCALEMODE_4_3,
	SCALMODE_AUTO,
};
enum{
	STATE_PLAY,
	STATE_PAUSE,
	STATE_STOP,
};
class CNewProjDlg : public CDialog
{
// Construction
public:
	CNewProjDlg(CWnd* pParent = NULL);	// standard constructor
public:
	DWORD GetDuration(CFile*);
	bool GetLanguageResource(int);
	bool	m_bDlg;
	CString m_FolderPath;
	CString m_FirstFilePath;
	CString m_SecondFilePath;
static	CString			m_FolderName;
static	SYSTEMTIME		m_FolderTime;
static	bool			g_bOpenFlag;
static 	SYSTEMTIME		m_DateTime[20];
static	DWORD			m_dwDurations[20];
static  float			m_fSeekPos;
	C264Demuxer* m_pDemuxer;
	C264Spliter* m_pSpliter;
	static CString	m_FirstPathArrays[20];
	static CString	m_SecondPathArrays[20];
	static int		m_nPathCounts;
	static int		m_nPathArraysPtr;
static	CView1Dlg m_View1Dlg;
static	CView2Dlg m_View2Dlg;
CToolBarDlg m_ToolBarDlg;
	CSystemSetupDlg m_SystemSetupDlg;
	CRecordPlayerAboutDlg m_RecordPlayerAboutDlg;
	CIOLightSetDlg m_IOLightSetDlg;
	CSearchDlg m_SearchDlg;
	CBurnToDiscDlg m_BurnToDiscDlg;;
	CGPSDlg m_GPSDlg;
	CTimeLineDlg m_TimeLineDlg;
	CInfoListDlg	m_InfoListDlg;
//static Variables
	static	WCHAR	m_wLanguageRes[716][100];
	static CRect	m_AboutRt;
	static BYTE		m_byScaleMode;
	static BYTE		m_bPlayerSatus;
	static BYTE		m_byWindowsCounts;
	static BYTE		m_byWtrMkVerify;
	static BYTE		m_bDateWindow;
static  bool			g_bResetFlag;
static	bool			g_bViewChange;
static	CFile			m_ConfigFile;
static	Config_Datas	g_config_Value_ST;
static ADJUST_CONFIG	g_adjust_Value_ST;
static IO_LIGHT_CONFIG  g_io_Light_Value_ST;
static	int					g_nPlayState;
// Dialog Data
	//{{AFX_DATA(CNewProjDlg)
	enum { IDD = IDD_RECORDPLAYER };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNewProjDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileOpen();
	afx_msg void OnExit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSettings();
	afx_msg void OnAbout();
	afx_msg void OnIoLightSet();
	afx_msg void OnSearch();
	afx_msg void OnBurn();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};
void WndsBK_Paint(CView1Dlg*, CView2Dlg*, int);
int Unichar2Ascii(WCHAR *unichar, char* asciichar);
int Ascii2Unichar(char* asciichar, WCHAR *unichar);
int wtoi(CString str);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJDLG_H__70FEBDBF_5290_4EB7_8A74_A2CDD019B490__INCLUDED_)
