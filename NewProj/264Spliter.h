// 264Spliter.h: interface for the C264Spliter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_264SPLITER_H__F410ED7C_5F96_4205_BE9E_65E5741FD0CD__INCLUDED_)
#define AFX_264SPLITER_H__F410ED7C_5F96_4205_BE9E_65E5741FD0CD__INCLUDED_
#include "264Demuxer.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PACKET_BODY_SIZE 3195
#define PACKET_SIZE				3200
#define FRAME_MAX_SIZE 0x20000
#define SUB_AUDIO_FRAME_SIZE 104
#define PACKET00_TYPE	0x00
#define PACKET01_TYPE	0x01
#define PACKET08_TYPE	0x08
#define PACKET09_TYPE	0x09
#define PACKET10_TYPE	0x10
#define	PACKET11_TYPE	0x11 

#define	ASF_FILE_HEAD_SIZE	1115


struct PACKET_CONST_HEADER
{
	BYTE			byHead;
	WORD			wdConst;
	BYTE			byType;
	BYTE			byConst;
};

struct PACKET00_HEADER //0x00 size = 21
{
	DWORD			dwPTS;
	WORD			wdSize;
	BYTE			byFrameType;
	BYTE			byFrameNum;
	DWORD			dwReserved;
	BYTE			byHeaderId;
	DWORD			dwFrameSize;
	DWORD			dwDTS;
};

struct PACKET01_HEADER //0x01 size = 24
{
	DWORD			dwPTS;
	WORD			wdSize;
	BYTE			byID;
	BYTE			byFrameType;
	BYTE			byFrameNum;
	DWORD			dwReserved;
	BYTE			byHeaderId;
	DWORD			dwFrameSize;
	DWORD			dwDTS;
	WORD			wdTailFrameSize;
};

struct PACKET08_HEADER //0x08 size = 22
{
	DWORD			dwPTS;
	WORD			wdSize;
	BYTE			byID;
	BYTE			byFrameType;
	BYTE			byFrameNum;
	DWORD			dwReserved;
	BYTE			byHeaderId;
	DWORD			dwFrameSize;
	DWORD			dwDTS;
};

struct PACKET09_HEADER //0x09 size = 25
{
	DWORD			dwPTS;
	BYTE			byReserved;
	WORD			wdSize;
	BYTE			byID;
	BYTE			byFrameType;
	BYTE			byFrameNum;
	DWORD			dwReserved;
	BYTE			byHeaderId;
	DWORD			dwFrameSize;
	DWORD			dwDTS;
	WORD			wdTailFrameSize;
};

struct PACKET10_HEADER //0x10 size = 23
{
	DWORD			dwPTS;
	WORD			wdReserved;
	WORD			wdSize;
	BYTE			byFrameType;
	BYTE			byFrameNum;
	DWORD			dwReserved;
	BYTE			byHeaderId;
	DWORD			dwFrameSize;
	DWORD			dwDTS;
};

struct PACKET11_HEADER//0x11 size = 26
{
	DWORD			dwPTS;
	WORD			wdReserved;
	BYTE			byID;
	WORD			wdReserved1;
	BYTE			byFrameType;
	BYTE			byFrameNum;
	DWORD			dwReserved;
	BYTE			byHeaderId;
	DWORD			dwFrameSize;
	DWORD			dwDTS;
	WORD			wdTailFrameSize;
};

struct FRAME_HEADER //size = 17
{
	BYTE			byFrameType;
	BYTE			byFrameNum;
	DWORD			dwReserved;
	BYTE			byHeaderId;
	DWORD			dwFrameSize;
	DWORD			dwDTS;
	WORD			wdTailFrameSize;
};

struct  Hi264Time{
	char			year;
	char			month;
	char			day;
	char			hour;
	char			minute;
	char			second;
};
struct AccelSpeedData
{
	char			temp[12];
	SHORT			acc_R;
	SHORT			acc_G;
	SHORT			acc_B;
	SHORT			accOSet_G;
	SHORT			accTotalOffset;
	SHORT			buf_unknown;
	
};
struct MainBinaryData
{
	AccelSpeedData	accSpeedData;
	char			temp_M[6];
	BYTE			sig_1;
	char			alarmInfo;
	BYTE			sig_2;
	BYTE			sig_3;
	SHORT			speed1;
	SHORT			speed_buf;
	SHORT			speed2;
	BYTE			byGPSN_Hi_Up;
	BYTE			byGPSN_Lw_Up;
	BYTE			byGPSW_Hi_Up;
	BYTE			byGPSW_Lw_Up;
	DWORD			dwGPSN_Down;
	DWORD			dwGPSW_Down;
	WORD			wdUnk1;
	BYTE			byEW_Flag;
	BYTE			byNS_Flag;
	DWORD			dwUnk2;
	
};
struct OUT_DATAS
{
	bool			bBuf;
	DWORD			dwDTS_V;
	char			buf_V[704 * 480 * 3];
	DWORD			dwDTS_A;
	char			buf_A[1600];
	DWORD			dwDTS_B;
	int				nB_Size;
	MainBinaryData	mB_Data;
};
struct TempBinData
{
	MainBinaryData	mBinData;
	int				nBinSize;
	DWORD			dwCurPos;
};
class C264Spliter  
{
public:
	bool	OpenFolder(CString StrFolderPath);
	C264Spliter();
	virtual ~C264Spliter();

	int		OpenFile(CString pStrFileName, float);
	DWORD	GetFileLength();
	void	CloseFile();
	DWORD	GetFilePosition();
	void	Initialization(CFile*);
	void	Init_Voice();
	BOOL	ReadFrameData();
	BYTE	ReadOneFrame(BYTE*& pByPtrFrameOut, DWORD *  pDwFrameSize);
	void	ReadFrameHeader();
	void	ReadPacketHeader();
	bool	Seek(float);
	bool Seek(float fSeekPos, int nPlayerTime);
	DWORD GetFirstDTS();
	DWORD GetLastDTS();
	int GetDatas1();
	int GetDatas2();
	bool ReSeek(void);
public:
	
	static OUT_DATAS			m_1stOutDatas;
	static OUT_DATAS			m_2ndOutDatas;
	int					m_1stFrameNum;
	int					m_2ndFrameNum;
	CString				m_FirstFilePath;
	CFile*				m_file;
	C264Demuxer			m_Demuxer;
	HiHandle*			m_hDec;
	HiHandle*			m_hDec2;
	int					m_byCurFrameNum;
	DWORD				m_dwCurDTS;
static	DWORD			m_dwFirstDTS;
	DWORD				m_dwSeekDTS;
	DWORD				m_dwLastDTS;
static	DWORD			m_dw1stSeekTime;
static	DWORD			m_dw2ndSeekTime;
	bool				m_1stEndFlag;
	bool				m_2ndEndFlag;
	static OUT_FRAME	m_OutFrame;
static	BYTE				m_pY[0x65400];
static 	BYTE				m_pU[0x19500];
static	BYTE				m_pV[0x19500];
static	BYTE				m_pY2[0x65400];
static 	BYTE				m_pU2[0x19500];
static	BYTE				m_pV2[0x19500];
	PACKET_CONST_HEADER m_PacketConstHeader;
	PACKET00_HEADER		m_Packet00Header;
	PACKET01_HEADER		m_Packet01Header;
	PACKET08_HEADER		m_Packet08Header;
	PACKET09_HEADER		m_Packet09Header;
	PACKET10_HEADER		m_Packet10Header;
	PACKET11_HEADER		m_Packet11Header;
	FRAME_HEADER		m_FrameHeader;
	DWORD				m_dwPacketReadSize;
	DWORD				m_dwReadSize;
	DWORD				m_dwSeekPos;
	BYTE				m_byCurPacketType;
	CString				m_strFileName;
	Hi264Time			m_time;
	BYTE				m_buf[FRAME_MAX_SIZE];
	BYTE				m_byFrameData[FRAME_MAX_SIZE];
	DWORD				m_dwCurFrameSize;
	DWORD				m_dwFileLength;
	DWORD				m_dwFilePos;
	DWORD				m_dwCurReadSizeOfFrame;
	DWORD				m_dwCurPacketHeaderSize;
	BYTE				m_byCurFrameType;
};

#endif // !defined(AFX_264SPLITER_H__F410ED7C_5F96_4205_BE9E_65E5741FD0CD__INCLUDED_)
