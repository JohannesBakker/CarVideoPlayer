#if !defined(AFX_BATCHCONVERTDLG_H__AD47071E_FC93_4045_A159_9C1BC15E1365__INCLUDED_)
#define AFX_BATCHCONVERTDLG_H__AD47071E_FC93_4045_A159_9C1BC15E1365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchConvertDlg.h : header file
//
#include "CxStatic.h"
#include "EditTrans.h"
#include "ReportCtrl.h"
#include "AVIConverterDlg.h"
#include "xCheckButton.h"
/////////////////////////////////////////////////////////////////////////////
// CBatchConvertDlg dialog

class CBatchConvertDlg : public CDialog
{
// Construction
public:
	CBatchConvertDlg(CWnd* pParent = NULL);   // standard constructor
public:	
	void GetBatchFiles(CFile*, CFile*);
	void InitHeaderDatas(int, int);
	//void OnClose();
	bool		m_bStart;
	bool		m_bEnd;
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

	CFile*				m_p1st264File;
	CFile*				m_p2nd264File;
	CString				m_1stFilePath;
	CString				m_2ndFilePath;
	SYSTEMTIME			m_systemTime;
	tagTIMESTAMP_STRUCT	m_DateTime;
	CDateTimeCtrl*		m_pTimeRangeCtrl;
	CDateTimeCtrl*		m_pToCtrl;
	DWORD				m_dwDuration;
	CFile				m_AVIFile;
	C264Spliter			m_1stSpliter;
	C264Spliter			m_2ndSpliter;
	C264Demuxer			m_Demuxer;
	CString				m_FolderPath;
	CString				m_AVIFilePath;
	CString				m_AVIFileName;
	int					m_nAudioChannel;
	int					m_nVideoWidth;
	int					m_nVideoHeight;
	int					m_nScaleFlag;
	int					m_nChannel;
static	XVID_HANDLE			m_lpHandle;
static	XVID_FRAME			m_lpFrame;
static	XVID_CREATE			m_lpCreate;
static	XVID_STATE			m_lpState;
// Dialog Data
	//{{AFX_DATA(CBatchConvertDlg)
	enum { IDD = IDD_BATCH_CONVERT };
	CDateTimeCtrl	m_ToCtrl;
	CDateTimeCtrl	m_TimeRangeCtrl;
	CProgressCtrl	m_progress1;
	CComboBox	m_cmb_selectaudio;
	CButton	m_btn_target_open;
	CButton	m_btn_Start;
	CButton	m_btn_output_Open;
	CButton	m_btn_Change;
	CButton	m_btn_App;
	CxStatic	m_staticTo;
	CxCheckButton	m_checkPicFlip;
	CReportCtrl	m_fileList;
	CReportCtrl	m_convertList;
	CEditTrans	m_edit_target;
	CEditTrans	m_edit_output;
	CEditTrans	m_edit_filetime;
	CxStatic	m_static_timerange;
	CxStatic	m_static_target;
	CxStatic	m_static_string;
	CxStatic	m_static_selectaudio;
	CxStatic	m_static_progress;
	CxStatic	m_static_output;
	CxStatic	m_static_filetime;
	CxStatic	m_static_file;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchConvertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	public:

	// Generated message map functions
	//{{AFX_MSG(CBatchConvertDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnAppButton();
	afx_msg void OnOutputChangeButton();
	afx_msg void OnStartButton();
	afx_msg void OnOutputOpenButton();
	afx_msg void OnTargetOpenButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCHCONVERTDLG_H__AD47071E_FC93_4045_A159_9C1BC15E1365__INCLUDED_)
