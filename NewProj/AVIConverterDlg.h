#if !defined(AFX_AVICONVERTERDLG_H__4E8E222A_E635_42F3_B94E_F5E75F494511__INCLUDED_)
#define AFX_AVICONVERTERDLG_H__4E8E222A_E635_42F3_B94E_F5E75F494511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AVIConverterDlg.h : header file
//
#include "CxStatic.h"
#include "EditTrans.h"
#include "ImageTextButton.h"
#include "xCheckButton.h"
#include "ReportCtrl.h"
#include "264Spliter.h"
#include "264Demuxer.h"
#include <sqltypes.h>
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CAVIConverterDlg dialog
//AVI_XVID FILE HEADER STRUCTS

struct AVILISTHEADER
{
	char			chAviListHeader[8];//hdrlavih;
	DWORD			dwHeaderSize;//0x38
	DWORD			dwSecPerFrames;//0x186A0(100000) / 10000
	DWORD			dwConst1[3];
	DWORD			dwVideoFrameCounts;//0x11E(286)
	DWORD			dwConst2[3];
	int				nVideoWidth;
	int				nVideoHeight;
	BYTE			byReserved[0x10];
};
struct VIDEOFRAME
{
	char			chFrame[4];//strf
	DWORD			dwFrameSize1;//0x28
	DWORD			dwFrameSize2;//0x28
	int				nVideoWidth;
	int				nVideoHeight;
	DWORD			dwUnk[7];
};
struct VIDEOLIST	//size = 0x78
{
	char			chList[4];//LIST
	DWORD			dwVideoListSize;//0x74
	char			chStringHeader[8];//strlstrh
	DWORD			dwHeaderSize;//0x38
	char			chVideo[8];//vidsxvid
	DWORD			dwReserved1[3];
	DWORD			dwConst[3];
	DWORD			dwVideoFrameCounts;//0x11E(286)
	DWORD			dwReserved2[5];
	VIDEOFRAME		m_VideoFrame;
};
struct STRINGHEADER
{
	char			chStringHeader[8];//strlstrh
	DWORD			dwHeaderSize;//0x38
	char			chAudios[4];//auds
	DWORD			dwReserved1[4];
	int				nChannelCounts;//1;
	DWORD			dwSecPerbits;//0x3E8
	DWORD			dwReserved2[7];
};
struct AUDIOFRAME
{
	char			chFrame[4];//strf
	DWORD			dwFrameSize;//0x10
	DWORD			dwConst1;
	DWORD			dwHz;//1F40
	DWORD			dwSecPerbits;
	DWORD			dwConst2;
};
struct AUDIOLIST
{
	char			chList[4];//LIST
	DWORD			dwAudioListSize;//0x5C
	STRINGHEADER	m_StringHeader;
	AUDIOFRAME		m_AudioFrame;
};
struct RIFF
{
	char			chRiff[4];//RIFF
	DWORD			dwFileSize;
};
struct 	AVI_LIST
{
	char			chAvi_List[8];//AVI_LIST
	DWORD			dwAviListStructSize;//0x124
	AVILISTHEADER	m_AviListHeader;
	VIDEOLIST		m_VideoList;
	AUDIOLIST		m_AudioList;
 };
struct 	INFOLIST	//size = 0x54
{
	char			chList[4];//LIST
	DWORD			dwInfoListFrameSize;//0x4C
	char			chInfoList[8];//INFOLIST
	DWORD			dwInfoSize;//0x40
	BYTE			byInfo[0x40];
};
struct JUNK
{
	char			chJunk[4];//JUNK
	DWORD			dwJunkSize;
	BYTE			byReserveds[1632];
};
///////////////////////////////////////////////////
//AVI_XVID_VIDEO & AUDIO FRAME_HEADER
struct LIST_FRAME_HEADER	//B0, B5, B2
{
	DWORD			dwVideoID;
	DWORD			dwFrameSize;
	DWORD			dwB0_HeaderID;
	char			chB0_Data;
	DWORD			dwB5_HeaderID;
	DWORD			dwB5_Datas[5];
	DWORD			dwB2_HeaderID;
	DWORD			dwB2_Datas[2];
	DWORD			dwB6_HeaderID;
};
struct B6_FRAME_HEADER
{
	DWORD			dwB6_HeaderID;
	DWORD			dwFrameSize;
};
struct AUDIO_FRAME_HEADER
{
	DWORD			dwAudioHeaderID;
	DWORD			dwFrameSize;
};
struct TRACK_FRAME
{
	char			chList[4];//LIST;
	DWORD			dwSeekFramesPos;
	char			chMovie[4];//movi
};
struct SEEKPOS_FRAME
{
	DWORD			dwFrameHeaderID;
	DWORD			dwListHeaderID;
	DWORD			dwSeekPos;
	DWORD			dwFrameSize;
};
/////////////////////////////////////////////////////////////////////////////
//xvid_encore Parameters Types
struct XVID_HANDLE
{
	int		m_nWidth;
	int		m_nHeight;
	int		Unk1;
	int		Unk2;
	int		Unk3;
	int		Unk4;
	int		Unk5;
	int		Unk6;
	int		Unk7;
	int		Unk8;
	int		Unk9;
	int		Unk10;
	int		Unk11;
	int		Unk12;
	int		Unk13;
	int		Unk14;
	int		Unk15;
	int		Unk16;
	int		Unk17;
	int		Unk18;
	int		Unk19;
	int		Unk20;
	int		Unk21;
	int		Unk22;
	int		Unk23;
	///////////////
	int		Unk24;
	int		Unk25;
	int		Unk26;
	int		Unk27;
	int		Unk28;
	int		Unk29;
	void*	lp1;
	int		Unk30;
	int		Unk31;
	int		Unk32;
	int		Unk33;
	int		Unk34;
	int		Unk35;
	int		Unk36;
	int		Unk37;
	void*	lp2;
	void*	lp3;
	int		Unk38;
	int		Unk39;
	int		Unk40;
	int		Unk41;
	int		Unk42;
	int		Unk43;
	void*	lp4;
	void*	lp5;
	void*	lp6;
	void*	lp7;
	void*	lp8;
	void*	lp9;
	int		Unk44;
	int		Unk45;
	int		Unk46;
	void*	lp10;
	void*	lp11;
	void*	lp12;
	int		Unk47;
	int		Unk48;
	int		Unk49;
	void*	lp13;
	void*	lp14;
	void*	lp15;
	int		Unk50;
	int		Unk51;
	int		Unk52;
	void*	lp16;
	int		Unk53;
	int		Unk54;
	int		Unk55;
	int		Unk56;
	void*	lp17;
	void*	lp18;
	void*	lp19;
	void*	lp20;
	void*	lp21;
	void*	lp22;
	void*	lp23;
	void*	lp24;
	void*	lp25;
	int		Unk57;
};
struct PAP_Data
{
	BYTE	chData[0x20000];
};
struct XVID_FRAME
{
	int		nConst; //00
	int		Unk1;	//04
	int		Unk2;	//08
	int		Unk3;	//0C
	int		Unk4;	//10
	int		Unk5;	//14
	int		Unk6;	//18
	int		Unk7;	//1C
	int		Unk8;	//20
	int		Unk9;	//24
	int		Unk10;	//28
	BYTE*	PushData;//2C	
	int		Unk11;
	int		Unk12;
	int		Unk13;
	int		nWidth;
	int		Unk15;
	int		Unk16;
	int		Unk17;
	int		Unk18;
	int		Unk19;
	int		Unk20;
	BYTE*	PopData;//[0x10000];
	int		Unk21;
	int		Unk22;
};
struct XVID_CREATE
{
	int nConst;
	int Unk1;
	int Unk2;
	int Unk3;
	int Unk4;
	int Unk5;
	int Unk6;
	int Unk7;
	int Unk8;
	int Unk9;
	int Unk10;
	int PushData;
	int Unk11;
	int Unk12;
	int Unk13;
	int nWidth;
	int Unk15;
	int Unk16;
	int Unk17;
	int Unk18;
	int Unk19;
	int Unk20;
	int PopData;
	int Unk21;
};
struct XVID_STATE
{
	int		nConst;
	int		nUnk1;
	int		nUnk2;
	int		nUnk3;
	int		nUnk4;
	int		nEncSize;
	int		nUnk6;
	int		nUnk7;
	int		nUnk8;
	int		nUnk9;
	int		nUnk10;
	int		nUnk11;
	int		nUnk12;
};
/////////////////////////////////////////////////////////////////////////////
// CAVIConverterDlg dialog
typedef DWORD		(_cdecl* FuncAVI_Xvid_EnCore)(void*, int, XVID_FRAME*, XVID_STATE*);
typedef DWORD		(_cdecl* FuncAVI_Xvid_Create)(int, int, XVID_CREATE*, int);
typedef	DWORD		(_cdecl* FuncAVI_Xvid_Global)(int, int, DWORD*, int);

class CAVIConverterDlg : public CDialog
{
// Construction
public:
	CAVIConverterDlg(CWnd* pParent = NULL);   // standard constructor
	void GetFiles(CFile*, CFile*);
	void InitHeaderDatas(int, int);
	bool		m_bPaint;
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
static	SEEKPOS_FRAME	m_SeekPosFrame[0x20000];

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
	CStringW				m_AVIFilePath;
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
public:
	//void OnClose();
	// Dialog Data
	//{{AFX_DATA(CAVIConverterDlg)
	enum { IDD = IDD_AVI_CONVERTER };
	CProgressCtrl	m_progress;
	CComboBox	m_cmb_selectaudio;
	CButton	m_btn_target_open;
	CButton	m_btn_Start;
	CButton	m_btn_out_open;
	CButton	m_btn_Change;
	CReportCtrl		m_aviFileList;
/*	CxCheckButton	m_radio4_3;
	CxCheckButton	m_radio16_9;
	CxCheckButton	m_RadioOriginalSize;*/
	CxCheckButton	m_checkMp4Fmt;
	CxCheckButton	m_checkPicFlip;
	CEditTrans	m_edit_target;
	CEditTrans	m_edit_output;
	CEditTrans	m_edit_filetime;
	CEditTrans	m_edit_destfilename;
	CxStatic	m_static_to;
	CxStatic	m_static_target;
	CxStatic	m_static_string2;
	CxStatic	m_static_string1;
	CxStatic	m_static_selectaudio;
	CxStatic	m_static_progress;
	CxStatic	m_static_output;
	CxStatic	m_static_filetime;
	CxStatic	m_static_file;
	CxStatic	m_static_timerange;
	CxStatic	m_static_destfilename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAVIConverterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	public:
		bool m_bRadioOriginalSize;
		bool m_bradio4_3;
		bool m_bradio16_9;

	// Generated message map functions
	//{{AFX_MSG(CAVIConverterDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnStartBtn();
	afx_msg void OnOutputOpenButton();
	afx_msg void OnTargetOpenButton();
	afx_msg void OnChangeButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
// private:
// 	CImageTextButton m_btn_change;
// 	CImageTextButton m_btn_output_open;
// 	CImageTextButton m_btn_target_open;
// 	CImageTextButton m_btn_start;
	afx_msg void OnClose();
	afx_msg void OnBnClickedRadio169();
	afx_msg void OnBnClickedRadioOriginalSize();
	afx_msg void OnBnClickedRadio43();
	CxCheckButton m_RadioOriginalSize;
	CxCheckButton m_radio4_3;
	CxCheckButton m_radio16_9;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVICONVERTERDLG_H__4E8E222A_E635_42F3_B94E_F5E75F494511__INCLUDED_)
