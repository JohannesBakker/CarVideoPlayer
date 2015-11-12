#if !defined(AFX_PROGRESSDLG_H__7B234B30_2246_43C0_B393_1A92233C0B9A__INCLUDED_)
#define AFX_PROGRESSDLG_H__7B234B30_2246_43C0_B393_1A92233C0B9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//
#include "264Spliter.h"
#include "AVIConverterDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

class CProgressDlg : public CDialog
{
// Construction
public:
	bool Init_Progress(CFile*, CFile*, DWORD, DWORD, DWORD, DWORD);
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor
public:
	DWORD m_limiteVal;
	DWORD m_limit2ndVal;	// used 2nd Video recording	
	int m_nChannel;
	CFile m_AVIFile;
	CString m_AVIFilePath;
	
	void Init_ConvertHeader(int, int);
	void Init_AVI_Convert(CFile*, CFile*, float, float, float, float, float);
	
	bool WriteFile();
	FuncAVI_Xvid_EnCore			m_Func_XvidEnc;
	FuncAVI_Xvid_Create			m_Func_XvidCreate;
	FuncAVI_Xvid_Global			m_Func_XvidGlobal;
	//AVI_XVID_FILEHEADER Variables
	RIFF				m_Riff;
	AVI_LIST			m_AviList;
	INFOLIST			m_InfoList;
	JUNK				m_Junk;
	//FrameHeaders
	LIST_FRAME_HEADER	m_ListFrameHeader;
	B6_FRAME_HEADER		m_B6FrameHeader;
	AUDIO_FRAME_HEADER	m_AudioFrameHeader;
	TRACK_FRAME			m_TrackFrame;
static	SEEKPOS_FRAME	m_SeekPosFrame[0x10000];
	CFile*				m_1stFile;
	CFile*				m_2ndFile;
	C264Spliter			m_1stSpliter;
	C264Spliter			m_2ndSpliter;
	DWORD				m_dw1stStartPos;
	DWORD				m_dw1stEndPos;
	DWORD				m_dw2ndStartPos;
	DWORD				m_dw2ndEndPos;
	CString				m_1stDestFilePath;
	CString				m_2ndDestFilePath;
	CFile*				m_1stDestFile;
	CFile*				m_2ndDestFile;
	bool				m_bEnd;
static	XVID_HANDLE			m_lpHandle;
static	XVID_FRAME			m_lpFrame;
static	XVID_CREATE			m_lpCreate;
static	XVID_STATE			m_lpState;
// Dialog Data
	//{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_PROGRESS_DIALOG };
	CButton	m_btn_Stop;
	CProgressCtrl	m_progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressDlg)
	afx_msg void OnStop();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__7B234B30_2246_43C0_B393_1A92233C0B9A__INCLUDED_)
