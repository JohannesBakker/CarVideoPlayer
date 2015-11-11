#if !defined(AFX_TOOLBARDLG_H__F6105C7E_3B10_456A_B7AA_BECB38B9C4B6__INCLUDED_)
#define AFX_TOOLBARDLG_H__F6105C7E_3B10_456A_B7AA_BECB38B9C4B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarDlg.h : header file
//
#include "AVIConverterDlg.h"
#include "RecordPlayerAboutDlg.h"
#include "AdjustVideoParamDlg.h"
#include "264Spliter.h"
#include "TimeRangeDlg.h"
#include "ProgressDlg.h"
#include "GPSDlg.h"
#include "View1Dlg.h"
#include "View2Dlg.h"
#include "type.h"
#include "InfoListDlg.h"
#include "TimeLineDlg.h"
#include "BitmapSlider.h"
#include "BitSlider.h"
#include "CxStatic.h"
#include "type.h"
#include "BitItem.h"
#include "Pipe.h"
#include <mmsystem.h>

#include <sqltypes.h>

#define IO_SIGNALS_NUM	3

typedef struct _264_FILE_HEADER{
	char	chUnk1[0x46];
	DWORD	dwFileSize;
	char	chUnk2[12];
	DWORD	dwReserved1[12];
	char	chUnk3[0x23F];
	DWORD	dwRepairFlag;
	char	chUnk4[0x170];
	DWORD	dwReserved2;
	char	chUnk5[0x1E];
};
/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg dialog

class CPipe;
class CToolBarDlg : public CDialog
{
// Construction
public:
	CToolBarDlg(CWnd* pParent = NULL);   // standard constructor
	void Init_PlayOpertion(CString, CString);
	void WriteBmpHeader(BITMAPFILEHEADER* pOutBmpFileHeader ,BITMAPINFOHEADER* pBmpInfoHeader,int nChoice);
	void WriteAviHeader( int nWidth, int nHeight);
public:
	void SetViewDlgs(CView1Dlg*, CView2Dlg* , CInfoListDlg* , CGPSDlg*,CTimeLineDlg* ,CToolBarDlg*);
	int	 Repairing(CFile*, CFile*);
	void WriteFile(CFile*, BYTE*, DWORD, BYTE);
	void ReSet();
	void Video2Out(char*, int);
	void Video1Out(char*, int);
	void Init_VideoOut(CView1Dlg*, CView2Dlg*);
	RGBQUAD RGBtoHSL(RGBQUAD lRGBColor);
	RGBQUAD HSLtoRGB(RGBQUAD lHSLColor);
	WORD HueToRGB(WORD n1,WORD n2,WORD hue);
	void TimeSeek(float, int);
	
	
//Graphics Variables
	//LPWAVEHDR				m_lpHdr;
	CToolTipCtrl			m_ToolTip;
	CString					m_ToolTipText;
	CToolInfo				m_ToolInfo;
	DWORD					m_dwFirstDTS;
static	CPipe*				m_pipe;
static	bool				g_ErrRead;
	//CSoundOut				m_soundOut;
	CBitItem* m_bitItem;
	static CBatchConvertDlg	m_BatchDlg;
	static bool				g_bView1Updown_ST;
	static bool				g_bView2Updown_ST;
	static bool				g_bView1Turn_ST;
	static bool				g_bView2Turn_ST;
	static bool				m_bFullScreen;
	static CFile*			m_pAviFile;
	int						m_bSnapshotChoice;
	BITMAPFILEHEADER*		m_pbmpFileHeader;
	BITMAPINFOHEADER*		m_pbmpInfoHeader;
	static char				ss_buf[704 * 480 * 3];
	static char				ss_buf2[1408 * 480 * 3];
	int						m_i;
	int						binDataNum;
	int						m_prevPos;
	bool					m_bDrawSeek;
	bool					m_bSignal;
	char					m_statusStr[20];
	CWnd*					pDisPos1;
	CWnd*					pDisPos2;
	CRect					rect1, rect2;
	BITMAPINFO				bmi1;
	BITMAPINFO				bmi2;
	HDC						hDC1, hMemDC1;
	HDC						hDC2, hMemDC2;
	HBITMAP					hBmp1, hOldBmp1;
	HBITMAP					hBmp2, hOldBmp2;
	int						m_nScaleMode;
///////////////
static	tagTIMESTAMP_STRUCT	m_DateTime;
	DWORD					m_dw1stStartPos;
	DWORD					m_dw2ndStartPos;
///////////////
	bool					m_bPlayer1;
	bool					m_bPlayer2;
	int						m_nSpeed;
static	int						m_nStartPos;
static	int						m_nEndPos;
	int						m_nThreadCounts;
	int						m_n1stTimeCount;
	int						m_n2ndTimeCount;
	bool					m_bCheckChannel;
static	C264Spliter			m_1stSpliter;
static	CString				m_1stFilePath;
static	CFile				m_1stFile;
static	C264Spliter			m_2ndSpliter;
static	CString				m_2ndFilePath;
static	CFile				m_2ndFile;
	bool					m_bToolbar;
	bool					m_bVolume;
static CAVIConverterDlg		m_AVIConverterDlg;
	CRecordPlayerAboutDlg	m_RecordPlayerAboutDlg;
	CAdjustVideoParamDlg	m_AdjustDlg;
	CView1Dlg*				m_pView1Dlg;
	CView2Dlg*				m_pView2Dlg;	
	CTimeRangeDlg			m_TimeRangeDlg;
	
	CInfoListDlg*			m_pInfoListDlg;
	CTimeLineDlg*			m_pTimeLineDlg;
	CGPSDlg*                m_pGpsDlg;
	CToolBarDlg*			m_pToolBarDlg;
static	DWORD				m_duration;
static	OUT_DATAS			m_1stOutDatas[10];
static	OUT_DATAS			m_2ndOutDatas[10];
static	int					m_nOutDatasPtr;
static	int					m_nPlayPtr;
static	bool				g_bSoundPlay;
static	DWORD				g_dwCurDTS;
	_264_FILE_HEADER		m_fileHeader;
	BYTE					m_byLastPacketType;
	DWORD					m_dwTailFrameSize;
	DWORD					m_dwTailPacketSize;
	DWORD					m_dwFrameBeginPos;
// Dialog Data
	//{{AFX_DATA(CToolBarDlg)
	enum { IDD = IDD_TOOLBAR };
	CxStatic	m_number2;
	CxStatic	m_number3;
	CxStatic	m_number1;
	CStatic	m_picture_volume;
	CBitmapSlider	m_slider_volume;
	CBitSlider		m_Slider_Seek;
	CButton	m_Btn_Adjust;
	CButton	m_Btn_Fast;
	CButton	m_Btn_StartCut;
	CButton	m_Btn_SnapShot;
	CButton	m_Btn_Slow;
	CButton	m_Btn_RepairFile;
	CButton	m_Btn_RecordEnd;
	CButton	m_Btn_RecordBegin;
	CButton	m_Btn_PrevSeg;
	CButton	m_Btn_Normal;
	CButton	m_Btn_NextSeg;
	CButton	m_Btn_FullScreen;
	CButton	m_Btn_Forward;
	CButton	m_Btn_EndCut;
	CButton	m_Btn_Back;
	CButton	m_Btn_AVI;
	CButton	m_Btn_2Frame;
	CButton	m_Btn_1Frame;
	CButton	m_btn_Stop;
	CButton	m_btn_Play;
	CxStatic	m_Status_Static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarDlg)
	afx_msg void OnPlayBtnClick();
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAvi();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVolumePicture();
	afx_msg void OnAdjust();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCustomdrawSliderSeek(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderSeek(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOutofmemorySliderSeek(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRecordBegin();
	afx_msg void OnRecordEnd();
	afx_msg void OnFast();
	afx_msg void OnSlow();
	afx_msg void OnNormal();
	afx_msg void OnStop();
	afx_msg void OnStartCut();
	afx_msg void OnEndCut();
	afx_msg void OnPaint();
	afx_msg void OnSnapshot();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTwoWindow();
	
	afx_msg void OnFullscreen();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSnapshotOneCamera();
	afx_msg void OnSnapshotWholeCamera();\
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
public:	
	bool m_bSeekSignal;
	bool m_bAlarmOn;

	int m_SignalId;
	static CString m_AviPathStr;
	bool m_bClickedStop;
	bool m_bIoAlarmSignals[IO_SIGNALS_NUM];

	void OnAlarmSwitch(bool bON);

	afx_msg void OnOneWindow();
	//afx_msg void OnMM_WOM_DONE(UINT parm1, LONG parm2);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedPrevSeg();
	afx_msg void OnBnClickedNextSeg();
	afx_msg void OnBnClickedRepairFile();




};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDLG_H__F6105C7E_3B10_456A_B7AA_BECB38B9C4B6__INCLUDED_)
