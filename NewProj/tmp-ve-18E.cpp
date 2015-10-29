// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "ProgressDlg.h"
#include "ToolBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

SEEKPOS_FRAME		CProgressDlg::m_SeekPosFrame[0x10000];
XVID_HANDLE			CProgressDlg::m_lpHandle;
XVID_FRAME			CProgressDlg::m_lpFrame;
XVID_CREATE			CProgressDlg::m_lpCreate;
XVID_STATE			CProgressDlg::m_lpState;
// CString             CToolBarDlg::m_1stFilePath;
// CString				CToolBarDlg::m_2ndFilePath;

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_1stDestFilePath = "";
	m_2ndDestFilePath = "";
	
}


void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDC_STOP, m_btn_Stop);
	DDX_Control(pDX, IDC_SPLITER_PROGRESS, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

void CProgressDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}
unsigned char	ReadBuf[0x1000000];
#if 0
UINT Cut_264ThreadProc(LPVOID pParam)
{
	CProgressDlg* pDlg = (CProgressDlg*)pParam;
	
	DWORD			dwPackPos;
	ZeroMemory(ReadBuf, 0x1000000);
	pDlg->m_1stFile->SeekToBegin();
	pDlg->m_1stFile->Read(ReadBuf, ASF_FILE_HEAD_SIZE);
	pDlg->m_1stDestFile->Write(ReadBuf, ASF_FILE_HEAD_SIZE);
	if(!pDlg->m_1stSpliter.Seek((float)pDlg->m_dw1stStartPos / pDlg->m_1stFile->GetLength())) return false;
	if((pDlg->m_1stSpliter.m_file->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE == 0x1A)	
	{
		pDlg->m_1stSpliter.m_file->Seek(-0x1A, CFile::current);
		pDlg->m_1stSpliter.m_file->Read(&pDlg->m_1stSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		pDlg->m_1stDestFile->Write(&pDlg->m_1stSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		pDlg->m_1stSpliter.m_file->Read(&pDlg->m_1stSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		pDlg->m_1stDestFile->Write(&pDlg->m_1stSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		pDlg->m_1stSpliter.m_file->Read(ReadBuf, pDlg->m_dw1stEndPos - pDlg->m_dw1stStartPos);
		pDlg->m_1stDestFile->Write(ReadBuf, pDlg->m_dw1stEndPos - pDlg->m_dw1stStartPos);
		dwPackPos = (pDlg->m_1stDestFile->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		pDlg->m_1stDestFile->Write(ReadBuf, PACKET_SIZE - dwPackPos);
	}else{ 
		pDlg->m_1stSpliter.m_file->Seek(-0x11, CFile::current);
		pDlg->m_1stSpliter.m_file->Read(&pDlg->m_1stSpliter.m_FrameHeader, sizeof(FRAME_HEADER));
		dwPackPos = ((pDlg->m_1stSpliter.m_file->GetPosition() - sizeof(FRAME_HEADER)) - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		pDlg->m_1stSpliter.m_PacketConstHeader.byType = 0x00;
		pDlg->m_1stDestFile->Write(&pDlg->m_1stSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		////Make 00Packet
		pDlg->m_1stSpliter.m_Packet00Header.dwPTS = pDlg->m_1stSpliter.m_FrameHeader.dwDTS - 2019;
		pDlg->m_1stSpliter.m_Packet00Header.wdSize = 0x17;
		pDlg->m_1stSpliter.m_Packet00Header.byFrameType = 03;
		pDlg->m_1stSpliter.m_Packet00Header.byFrameNum = 0xff;
		pDlg->m_1stSpliter.m_Packet00Header.dwReserved = 0;
		pDlg->m_1stSpliter.m_Packet00Header.byHeaderId = 0x08;
		pDlg->m_1stSpliter.m_Packet00Header.dwFrameSize = dwPackPos - 29 + 0xC66;
		pDlg->m_1stSpliter.m_Packet00Header.dwDTS = pDlg->m_1stSpliter.m_FrameHeader.dwDTS - 19;
		pDlg->m_1stDestFile->Write(&pDlg->m_1stSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		pDlg->m_1stDestFile->Write(ReadBuf, PACKET_SIZE - sizeof(PACKET_CONST_HEADER) - sizeof(PACKET00_HEADER));
		////Make 01Pakcet
		pDlg->m_1stSpliter.m_PacketConstHeader.byType = 0x01;
		pDlg->m_1stDestFile->Write(&pDlg->m_1stSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		pDlg->m_1stSpliter.m_Packet01Header.dwPTS = pDlg->m_1stSpliter.m_FrameHeader.dwDTS - 2019;
		pDlg->m_1stSpliter.m_Packet01Header.wdSize = 0x17;
		pDlg->m_1stSpliter.m_Packet01Header.byID = 0x82;
		pDlg->m_1stSpliter.m_Packet01Header.byFrameType = 03;
		pDlg->m_1stSpliter.m_Packet01Header.byFrameNum = 0xff;
		pDlg->m_1stSpliter.m_Packet01Header.dwReserved = 0;
		pDlg->m_1stSpliter.m_Packet01Header.byHeaderId = 0x08;
		pDlg->m_1stSpliter.m_Packet01Header.dwFrameSize = dwPackPos - 29 + 0xC66;
		pDlg->m_1stSpliter.m_Packet01Header.dwDTS = pDlg->m_1stSpliter.m_FrameHeader.dwDTS - 19;
		pDlg->m_1stSpliter.m_Packet01Header.wdTailFrameSize = (WORD)dwPackPos - 29;
		////
		pDlg->m_1stDestFile->Write(&pDlg->m_1stSpliter.m_Packet01Header, sizeof(PACKET01_HEADER));
		pDlg->m_1stDestFile->Write(ReadBuf, dwPackPos - 29);
		pDlg->m_1stDestFile->Write(&pDlg->m_1stSpliter.m_FrameHeader, sizeof(FRAME_HEADER));
		pDlg->m_1stSpliter.m_file->Read(ReadBuf, pDlg->m_dw1stEndPos - pDlg->m_dw1stStartPos);
		DWORD dwWriteLen , i = 1;
		dwWriteLen = pDlg->m_dw1stEndPos - pDlg->m_dw1stStartPos;
		while( (i * 1000) < (pDlg->m_dw1stEndPos - pDlg->m_dw1stStartPos)){
			pDlg->m_progress.SetPos(i*1000 / (pDlg->m_dw1stEndPos - pDlg->m_dw1stStartPos) * 100);
			pDlg->m_1stDestFile->Write((ReadBuf + (i - 1) * 1000), 1000);
			dwWriteLen -= 1000;
			i++;
			
		};
		pDlg->m_1stDestFile->Write((ReadBuf + (i - 1) * 1000),dwWriteLen);
		ZeroMemory(ReadBuf, 0x1000000);
		dwPackPos = (pDlg->m_1stDestFile->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		pDlg->m_1stDestFile->Write(ReadBuf, PACKET_SIZE - dwPackPos);
		pDlg->m_1stSpliter.m_file->Seek(-0x80, CFile::end);
		pDlg->m_1stSpliter.m_file->Read(ReadBuf, 0x80);
		pDlg->m_1stDestFile->Write(ReadBuf, 0x80);
		pDlg->m_1stDestFile->Close();
	}
	if(pDlg->m_2ndDestFilePath == "") return false;
	ZeroMemory(ReadBuf, 0x1000000);
	pDlg->m_2ndFile->SeekToBegin();
	pDlg->m_2ndFile->Read(ReadBuf, ASF_FILE_HEAD_SIZE);
	pDlg->m_2ndDestFile->Write(ReadBuf, ASF_FILE_HEAD_SIZE);
	if(!pDlg->m_2ndSpliter.Seek((float)pDlg->m_dw2ndStartPos / pDlg->m_2ndFile->GetLength())) return false;
	if((pDlg->m_2ndSpliter.m_file->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE == 0x1A)	
	{
		pDlg->m_2ndSpliter.m_file->Seek(-0x1A, CFile::current);
		pDlg->m_2ndSpliter.m_file->Read(&pDlg->m_2ndSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		pDlg->m_2ndDestFile->Write(&pDlg->m_2ndSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		pDlg->m_2ndSpliter.m_file->Read(&pDlg->m_2ndSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		pDlg->m_2ndDestFile->Write(&pDlg->m_2ndSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		pDlg->m_2ndSpliter.m_file->Read(ReadBuf, pDlg->m_dw2ndEndPos - pDlg->m_dw2ndStartPos);
		pDlg->m_2ndDestFile->Write(ReadBuf, pDlg->m_dw2ndEndPos - pDlg->m_dw2ndStartPos);
		dwPackPos = (pDlg->m_2ndDestFile->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		pDlg->m_2ndDestFile->Write(ReadBuf, PACKET_SIZE - dwPackPos);
	}else{ 
		pDlg->m_2ndSpliter.m_file->Seek(-0x11, CFile::current);
		pDlg->m_2ndSpliter.m_file->Read(&pDlg->m_2ndSpliter.m_FrameHeader, sizeof(FRAME_HEADER));
		dwPackPos = ((pDlg->m_2ndSpliter.m_file->GetPosition() - sizeof(FRAME_HEADER)) - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		pDlg->m_2ndSpliter.m_PacketConstHeader.byType = 0x00;
		pDlg->m_2ndDestFile->Write(&pDlg->m_2ndSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		////Make 00Packet
		pDlg->m_2ndSpliter.m_Packet00Header.dwPTS = pDlg->m_2ndSpliter.m_FrameHeader.dwDTS - 2019;
		pDlg->m_2ndSpliter.m_Packet00Header.wdSize = 0x17;
		pDlg->m_2ndSpliter.m_Packet00Header.byFrameType = 03;
		pDlg->m_2ndSpliter.m_Packet00Header.byFrameNum = 0xff;
		pDlg->m_2ndSpliter.m_Packet00Header.dwReserved = 0;
		pDlg->m_2ndSpliter.m_Packet00Header.byHeaderId = 0x08;
		pDlg->m_2ndSpliter.m_Packet00Header.dwFrameSize = dwPackPos - 29 + 0xC66;
		pDlg->m_2ndSpliter.m_Packet00Header.dwDTS = pDlg->m_2ndSpliter.m_FrameHeader.dwDTS - 19;
		pDlg->m_2ndDestFile->Write(&pDlg->m_2ndSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		pDlg->m_2ndDestFile->Write(ReadBuf, PACKET_SIZE - sizeof(PACKET_CONST_HEADER) - sizeof(PACKET00_HEADER));
		////Make 01Pakcet
		pDlg->m_2ndSpliter.m_PacketConstHeader.byType = 0x01;
		pDlg->m_2ndDestFile->Write(&pDlg->m_2ndSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		pDlg->m_2ndSpliter.m_Packet01Header.dwPTS = pDlg->m_2ndSpliter.m_FrameHeader.dwDTS - 2019;
		pDlg->m_2ndSpliter.m_Packet01Header.wdSize = 0x17;
		pDlg->m_2ndSpliter.m_Packet01Header.byID = 0x82;
		pDlg->m_2ndSpliter.m_Packet01Header.byFrameType = 03;
		pDlg->m_2ndSpliter.m_Packet01Header.byFrameNum = 0xff;
		pDlg->m_2ndSpliter.m_Packet01Header.dwReserved = 0;
		pDlg->m_2ndSpliter.m_Packet01Header.byHeaderId = 0x08;
		pDlg->m_2ndSpliter.m_Packet01Header.dwFrameSize = dwPackPos - 29 + 0xC66;
		pDlg->m_2ndSpliter.m_Packet01Header.dwDTS = pDlg->m_2ndSpliter.m_FrameHeader.dwDTS - 19;
		pDlg->m_2ndSpliter.m_Packet01Header.wdTailFrameSize = (WORD)dwPackPos - 29;
		////
		pDlg->m_2ndDestFile->Write(&pDlg->m_2ndSpliter.m_Packet01Header, sizeof(PACKET01_HEADER));
		pDlg->m_2ndDestFile->Write(ReadBuf, dwPackPos - 29);
		pDlg->m_2ndDestFile->Write(&pDlg->m_2ndSpliter.m_FrameHeader, sizeof(FRAME_HEADER));
		pDlg->m_2ndSpliter.m_file->Read(ReadBuf, pDlg->m_dw2ndEndPos - pDlg->m_dw2ndStartPos);
		DWORD dwWriteLen , i = 1;
		dwWriteLen = pDlg->m_dw2ndEndPos - pDlg->m_dw2ndStartPos;
		while( (i * 1000) < (pDlg->m_dw2ndEndPos - pDlg->m_dw2ndStartPos)){
			pDlg->m_progress.SetPos(i*1000 / (pDlg->m_dw2ndEndPos - pDlg->m_dw2ndStartPos) * 100);
			pDlg->m_2ndDestFile->Write((ReadBuf + (i - 1) * 1000), 1000);
			dwWriteLen -= 1000;
			i++;
			
		};
		pDlg->m_2ndDestFile->Write((ReadBuf + (i - 1) * 1000),dwWriteLen);
		ZeroMemory(ReadBuf, 0x1000000);
		dwPackPos = (pDlg->m_2ndDestFile->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		pDlg->m_2ndDestFile->Write(ReadBuf, PACKET_SIZE - dwPackPos);
		pDlg->m_2ndSpliter.m_file->Seek(-0x80, CFile::end);
		pDlg->m_2ndSpliter.m_file->Read(ReadBuf, 0x80);
		pDlg->m_2ndDestFile->Write(ReadBuf, 0x80);
		pDlg->m_2ndDestFile->Close();
	}	
	return 0;
}
#endif
bool CProgressDlg::Init_Progress(CFile* _1stFile, CFile* _2ndFile, DWORD dw1stStartPos, DWORD dw1stEndPos, DWORD dw2ndStartPos, DWORD dw2ndEndPos)
{
	m_1stFile = _1stFile;
	m_2ndFile = _2ndFile;
	m_dw1stStartPos = dw1stStartPos;
	m_dw1stEndPos = dw1stEndPos;
	m_dw2ndStartPos = dw2ndStartPos;
	m_dw2ndEndPos = dw2ndEndPos;
	m_1stDestFile = new CFile;
	m_1stSpliter.Initialization(m_1stFile);
	m_1stDestFile->Open("C:\\DVR264\\Output\\" + m_1stDestFilePath, CFile::modeCreate | CFile::modeWrite);
	if(m_dw2ndEndPos != 0)
	{
		m_2ndDestFile = new CFile;
		m_2ndSpliter.Initialization(m_2ndFile);
		m_2ndDestFile->Open("C:\\DVR264\\Output\\" + m_2ndDestFilePath, CFile::modeCreate | CFile::modeWrite);
	}else
		m_2ndDestFilePath = "";
	WriteFile();
	
	//AfxBeginThread(Cut_264ThreadProc,this);
	 
	return false;
}

#if 1
bool CProgressDlg::WriteFile()
{
	DWORD			dwPackPos;
	ZeroMemory(ReadBuf, 0x1000000);
	m_1stFile->SeekToBegin();
	m_1stFile->Read(ReadBuf, ASF_FILE_HEAD_SIZE);
	m_1stDestFile->Write(ReadBuf, ASF_FILE_HEAD_SIZE);
	if(!m_1stSpliter.Seek((float)m_dw1stStartPos / m_1stFile->GetLength())) return false;
	if((m_1stSpliter.m_file->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE == 0x1A)	
	{
		m_1stSpliter.m_file->Seek(-0x1A, CFile::current);
		m_1stSpliter.m_file->Read(&m_1stSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		m_1stDestFile->Write(&m_1stSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		m_1stSpliter.m_file->Read(&m_1stSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		m_1stDestFile->Write(&m_1stSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		m_1stSpliter.m_file->Read(ReadBuf, m_dw1stEndPos - m_dw1stStartPos);
		m_1stDestFile->Write(ReadBuf, m_dw1stEndPos - m_dw1stStartPos);
		dwPackPos = (m_1stDestFile->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		m_1stDestFile->Write(ReadBuf, PACKET_SIZE - dwPackPos);
	}else{ 
		m_1stSpliter.m_file->Seek(-0x11, CFile::current);
		m_1stSpliter.m_file->Read(&m_1stSpliter.m_FrameHeader, sizeof(FRAME_HEADER));
		dwPackPos = ((m_1stSpliter.m_file->GetPosition() - sizeof(FRAME_HEADER)) - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		m_1stSpliter.m_PacketConstHeader.byType = 0x00;
		m_1stDestFile->Write(&m_1stSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		////Make 00Packet
		m_1stSpliter.m_Packet00Header.dwPTS = m_1stSpliter.m_FrameHeader.dwDTS - 2019;
		m_1stSpliter.m_Packet00Header.wdSize = 0x17;
		m_1stSpliter.m_Packet00Header.byFrameType = 03;
		m_1stSpliter.m_Packet00Header.byFrameNum = 0xff;
		m_1stSpliter.m_Packet00Header.dwReserved = 0;
		m_1stSpliter.m_Packet00Header.byHeaderId = 0x08;
		m_1stSpliter.m_Packet00Header.dwFrameSize = dwPackPos - 29 + 0xC66;
		m_1stSpliter.m_Packet00Header.dwDTS = m_1stSpliter.m_FrameHeader.dwDTS - 19;
		m_1stDestFile->Write(&m_1stSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		m_1stDestFile->Write(ReadBuf, PACKET_SIZE - sizeof(PACKET_CONST_HEADER) - sizeof(PACKET00_HEADER));
		////Make 01Pakcet
		m_1stSpliter.m_PacketConstHeader.byType = 0x01;
		m_1stDestFile->Write(&m_1stSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		m_1stSpliter.m_Packet01Header.dwPTS = m_1stSpliter.m_FrameHeader.dwDTS - 2019;
		m_1stSpliter.m_Packet01Header.wdSize = 0x17;
		m_1stSpliter.m_Packet01Header.byID = 0x82;
		m_1stSpliter.m_Packet01Header.byFrameType = 03;
		m_1stSpliter.m_Packet01Header.byFrameNum = 0xff;
		m_1stSpliter.m_Packet01Header.dwReserved = 0;
		m_1stSpliter.m_Packet01Header.byHeaderId = 0x08;
		m_1stSpliter.m_Packet01Header.dwFrameSize = dwPackPos - 29 + 0xC66;
		m_1stSpliter.m_Packet01Header.dwDTS = m_1stSpliter.m_FrameHeader.dwDTS - 19;
		m_1stSpliter.m_Packet01Header.wdTailFrameSize = (WORD)dwPackPos - 29;
		////
		m_1stDestFile->Write(&m_1stSpliter.m_Packet01Header, sizeof(PACKET01_HEADER));
		m_1stDestFile->Write(ReadBuf, dwPackPos - 29);
		m_1stDestFile->Write(&m_1stSpliter.m_FrameHeader, sizeof(FRAME_HEADER));
		m_1stSpliter.m_file->Read(ReadBuf, m_dw1stEndPos - m_dw1stStartPos);
		m_1stDestFile->Write(ReadBuf, m_dw1stEndPos - m_dw1stStartPos);
		ZeroMemory(ReadBuf, 0x1000000);
		dwPackPos = (m_1stDestFile->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		m_1stDestFile->Write(ReadBuf, PACKET_SIZE - dwPackPos);
		m_1stSpliter.m_file->Seek(-0x80, CFile::end);
		m_1stSpliter.m_file->Read(ReadBuf, 0x80);
		m_1stDestFile->Write(ReadBuf, 0x80);
		m_1stDestFile->Close();
	}
	if(m_2ndDestFilePath == "") return false;
	ZeroMemory(ReadBuf, 0x1000000);
	m_2ndFile->SeekToBegin();
	m_2ndFile->Read(ReadBuf, ASF_FILE_HEAD_SIZE);
	m_2ndDestFile->Write(ReadBuf, ASF_FILE_HEAD_SIZE);
	if(!m_2ndSpliter.Seek((float)m_dw2ndStartPos / m_2ndFile->GetLength())) return false;
	if((m_2ndSpliter.m_file->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE == 0x1A)	
	{
		m_2ndSpliter.m_file->Seek(-0x1A, CFile::current);
		m_2ndSpliter.m_file->Read(&m_2ndSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		m_2ndDestFile->Write(&m_2ndSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		m_2ndSpliter.m_file->Read(&m_2ndSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		m_2ndDestFile->Write(&m_2ndSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		m_2ndSpliter.m_file->Read(ReadBuf, m_dw2ndEndPos - m_dw2ndStartPos);
		m_2ndDestFile->Write(ReadBuf, m_dw2ndEndPos - m_dw2ndStartPos);
		dwPackPos = (m_2ndDestFile->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		m_2ndDestFile->Write(ReadBuf, PACKET_SIZE - dwPackPos);
	}else{ 
		m_2ndSpliter.m_file->Seek(-0x11, CFile::current);
		m_2ndSpliter.m_file->Read(&m_2ndSpliter.m_FrameHeader, sizeof(FRAME_HEADER));
		dwPackPos = ((m_2ndSpliter.m_file->GetPosition() - sizeof(FRAME_HEADER)) - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		m_2ndSpliter.m_PacketConstHeader.byType = 0x00;
		m_2ndDestFile->Write(&m_2ndSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		////Make 00Packet
		m_2ndSpliter.m_Packet00Header.dwPTS = m_2ndSpliter.m_FrameHeader.dwDTS - 2019;
		m_2ndSpliter.m_Packet00Header.wdSize = 0x17;
		m_2ndSpliter.m_Packet00Header.byFrameType = 03;
		m_2ndSpliter.m_Packet00Header.byFrameNum = 0xff;
		m_2ndSpliter.m_Packet00Header.dwReserved = 0;
		m_2ndSpliter.m_Packet00Header.byHeaderId = 0x08;
		m_2ndSpliter.m_Packet00Header.dwFrameSize = dwPackPos - 29 + 0xC66;
		m_2ndSpliter.m_Packet00Header.dwDTS = m_2ndSpliter.m_FrameHeader.dwDTS - 19;
		m_2ndDestFile->Write(&m_2ndSpliter.m_Packet00Header, sizeof(PACKET00_HEADER));
		m_2ndDestFile->Write(ReadBuf, PACKET_SIZE - sizeof(PACKET_CONST_HEADER) - sizeof(PACKET00_HEADER));
		////Make 01Pakcet
		m_2ndSpliter.m_PacketConstHeader.byType = 0x01;
		m_2ndDestFile->Write(&m_2ndSpliter.m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		m_2ndSpliter.m_Packet01Header.dwPTS = m_2ndSpliter.m_FrameHeader.dwDTS - 2019;
		m_2ndSpliter.m_Packet01Header.wdSize = 0x17;
		m_2ndSpliter.m_Packet01Header.byID = 0x82;
		m_2ndSpliter.m_Packet01Header.byFrameType = 03;
		m_2ndSpliter.m_Packet01Header.byFrameNum = 0xff;
		m_2ndSpliter.m_Packet01Header.dwReserved = 0;
		m_2ndSpliter.m_Packet01Header.byHeaderId = 0x08;
		m_2ndSpliter.m_Packet01Header.dwFrameSize = dwPackPos - 29 + 0xC66;
		m_2ndSpliter.m_Packet01Header.dwDTS = m_2ndSpliter.m_FrameHeader.dwDTS - 19;
		m_2ndSpliter.m_Packet01Header.wdTailFrameSize = (WORD)dwPackPos - 29;
		////
		m_2ndDestFile->Write(&m_2ndSpliter.m_Packet01Header, sizeof(PACKET01_HEADER));
		m_2ndDestFile->Write(ReadBuf, dwPackPos - 29);
		m_2ndDestFile->Write(&m_2ndSpliter.m_FrameHeader, sizeof(FRAME_HEADER));
		m_2ndSpliter.m_file->Read(ReadBuf, m_dw2ndEndPos - m_dw2ndStartPos);
		m_2ndDestFile->Write(ReadBuf, m_dw2ndEndPos - m_dw2ndStartPos);
		ZeroMemory(ReadBuf, 0x1000000);
		dwPackPos = (m_2ndDestFile->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		m_2ndDestFile->Write(ReadBuf, PACKET_SIZE - dwPackPos);
		m_2ndSpliter.m_file->Seek(-0x80, CFile::end);
		m_2ndSpliter.m_file->Read(ReadBuf, 0x80);
		m_2ndDestFile->Write(ReadBuf, 0x80);
		m_2ndDestFile->Close();
	}
	return false;
}
#endif
UINT AVI_SpliterProc(LPVOID lParam)
{
	CProgressDlg* pDlg = (CProgressDlg*)lParam;
	FILE* fp = fopen(pDlg->m_AVIFilePath, "wb");
	if(pDlg->m_2ndFile != NULL && pDlg->m_1stFile != NULL)
	{
		pDlg->Init_ConvertHeader(1408,480);
		pDlg->m_nChannel = 3;
	}else if(pDlg->m_2ndFile == NULL && pDlg->m_1stFile != NULL){
		pDlg->Init_ConvertHeader(704, 480);
		pDlg->m_nChannel = 1;
	}else if(pDlg->m_2ndFile != NULL && pDlg->m_1stFile == NULL)
	{
		pDlg->Init_ConvertHeader(704, 480);
		pDlg->m_nChannel = 2;

	}

	fprintf(fp, "");
	fclose(fp);
	DWORD dwVideoID = 0x62643030;
	DWORD dwAudioID = 0x62773130;
	if(pDlg->m_1stFile != NULL)pDlg->m_1stSpliter.OpenFile("", 0);
	if(pDlg->m_2ndFile != NULL)pDlg->m_2ndSpliter.OpenFile("", 0);
	if(!pDlg->m_AVIFile.Open(pDlg->m_AVIFilePath, 0x1000 || 0X0002))
	{
		MessageBox(NULL, _T("Error Create AVI FIle!"), NULL, 0);
		return 0;
	}

	pDlg->m_AVIFile.SetLength(0);
	pDlg->m_AVIFile.Write(&pDlg->m_Riff, sizeof(RIFF));
	pDlg->m_AVIFile.Write(&pDlg->m_AviList, sizeof(AVI_LIST));
	pDlg->m_AVIFile.Write(&pDlg->m_InfoList, sizeof(INFOLIST));
	pDlg->m_AVIFile.Write(&pDlg->m_Junk, sizeof(JUNK));
	pDlg->m_AVIFile.Write(&pDlg->m_TrackFrame, sizeof(TRACK_FRAME));
	DWORD dwFilePos = pDlg->m_AVIFile.GetPosition();
	if(pDlg->m_nChannel == 3) pDlg->m_lpFrame.PushData = new BYTE[0x12FC00];
	else pDlg->m_lpFrame.PushData = new BYTE[0x97E00];
	pDlg->m_lpFrame.PopData = new BYTE[0x20000]; 
	DWORD dwGlobal[4];
	memset(dwGlobal, 0, 16);
	dwGlobal[0] = 0x10202;
	int nCounts = 0;
	DWORD dwRetVal = (*pDlg->m_Func_XvidGlobal)(0, 0, dwGlobal, 0);
	dwRetVal = (*pDlg->m_Func_XvidCreate)(0, 0, &pDlg->m_lpCreate, 0);
	dwRetVal = 1;
	int		nFrmaeID = 0;
	pDlg->m_1stSpliter.GetDatas1();
	pDlg->m_1stSpliter.GetDatas1();
	pDlg->m_2ndSpliter.GetDatas2();
	pDlg->m_2ndSpliter.GetDatas2();

		do{
			if( pDlg->m_1stFile != NULL )
				pDlg->m_progress.SetPos((int)((float)pDlg->m_1stSpliter.m_file->GetPosition() / pDlg->m_limiteVal * 100));
			else
				pDlg->m_progress.SetPos((int)((float)pDlg->m_2ndSpliter.m_file->GetPosition() / pDlg->m_limiteVal * 100));
			pDlg->m_1stSpliter.GetDatas1();
			pDlg->m_2ndSpliter.GetDatas2();
			if(pDlg->m_1stSpliter.m_pY == 0) continue;
			if(pDlg->m_nChannel == 1)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x97e00);
				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_1stSpliter.m_pY, 0x52800);
				//memcpy((char*)(m_lpFrame.PushData + 0x65400), m_1stSpliter.m_pU, 0x19500);
				//memcpy((char*)(m_lpFrame.PushData + 0x7E900), m_1stSpliter.m_pV, 0x19500);	
			}else if(pDlg->m_nChannel == 2)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x97e00);
				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_2ndSpliter.m_pY2, 0x52800);
//				memcpy((char*)(m_lpFrame.PushData + 0x65400), m_2ndSpliter.m_pU2, 0x19500);
//				memcpy((char*)(m_lpFrame.PushData + 0x7E900), m_2ndSpliter.m_pV2, 0x19500);	
			}else if(pDlg->m_nChannel == 3)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x12FC00);
				
				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_1stSpliter.m_pY, 0x52800);
				for(int j = 0; j < 480; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 1408 * j), (char*)(pDlg->m_1stSpliter.m_pY + 704 * j), 704);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 1408 * j + 704), (char*)(pDlg->m_1stSpliter.m_pY2 + 704 * j), 704);
				}
				//memcpy((char*)(m_lpFrame.PushData + 0xCA800), m_1stSpliter.m_pU, 0x19500);
				//memcpy((char*)(m_lpFrame.PushData + 0xFD200), m_1stSpliter.m_pV, 0x19500);	
				//memcpy((char*)(m_lpFrame.PushData + 0x52800), m_2ndSpliter.m_pY, 0x52800);
				//memcpy((char*)(m_lpFrame.PushData + 0xE3D00), m_2ndSpliter.m_pU2, 0x19500);
				//memcpy((char*)(m_lpFrame.PushData + 0x116700), m_2ndSpliter.m_pV2, 0x19500);	
			}
			//memset((char*)(m_lpFrame.PushData + 0x7BC00), 0, 0x97E00 - 0x7BC00);
			memset(&pDlg->m_lpState, 0, sizeof(XVID_STATE));
			pDlg->m_lpState.nConst = 0x10202;
			dwRetVal = (*pDlg->m_Func_XvidEnc)((DWORD*)(pDlg->m_lpCreate.Unk21), 2, &pDlg->m_lpFrame, &pDlg->m_lpState);
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwVideoID;
			if(nCounts % 250 == 0)pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0x10;
			else pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			nFrmaeID ++;
			pDlg->m_AVIFile.Write(&dwVideoID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write(pDlg->m_lpFrame.PopData, dwRetVal);
			nCounts++;

			dwRetVal = 0x140;
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			if(1)
			{
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write(pDlg->m_1stSpliter.m_1stOutDatas.buf_A, 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write((char*)(pDlg->m_1stSpliter.m_1stOutDatas.buf_A + 320), 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write((char*)(pDlg->m_1stSpliter.m_1stOutDatas.buf_A + 320 * 2), 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write((char*)(pDlg->m_1stSpliter.m_1stOutDatas.buf_A + 320 * 3), 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write((char*)(pDlg->m_1stSpliter.m_1stOutDatas.buf_A + 320 * 4), 320);
				nFrmaeID ++;
			}else{
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A, 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write((char*)(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A + 320), 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write((char*)(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A + 320 * 2), 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write((char*)(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A + 320 * 3), 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				pDlg->m_AVIFile.Write(&dwAudioID, 4);
				pDlg->m_AVIFile.Write(&dwRetVal, 4);
				pDlg->m_AVIFile.Write((char*)(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A + 320 * 4), 320);
				nFrmaeID ++;
			}
			if(pDlg->m_1stFile != NULL)
				{if(pDlg->m_1stSpliter.m_file->GetPosition() >= pDlg->m_limiteVal - 0xC80) break;}
			else if(pDlg->m_1stFile == NULL)
					if(pDlg->m_2ndSpliter.m_file->GetPosition() >= pDlg->m_limiteVal - 0xC80) break;
			//if(pDlg->m_bEnd == true) break;
		}while(dwRetVal > 0);
		dwRetVal = pDlg->m_AVIFile.GetPosition() - 0x7FC;
		pDlg->m_AVIFile.Seek(0x7F8, CFile::begin);
		pDlg->m_AVIFile.Write(&dwRetVal, 4);
		pDlg->m_AVIFile.SeekToEnd();
		dwRetVal = 0x31786469;
		pDlg->m_AVIFile.Write(&dwRetVal, 4);
		dwRetVal = nFrmaeID * sizeof(SEEKPOS_FRAME);
		pDlg->m_AVIFile.Write(&dwRetVal, 4);
		//if(pDlg->m_bEnd == false)	
		for(int i = 0; i < nFrmaeID; i++)
		{
			pDlg->m_AVIFile.Write(&pDlg->m_SeekPosFrame[i], sizeof(SEEKPOS_FRAME));
		}
		dwRetVal = pDlg->m_AVIFile.GetPosition() - 8;
		pDlg->m_AVIFile.Seek(4, CFile::begin);
		pDlg->m_AVIFile.Write(&dwRetVal, 4);
		pDlg->m_AVIFile.Seek(0x30, CFile::begin);
		pDlg->m_AVIFile.Write(&nCounts, 4);
		pDlg->m_AVIFile.Seek(0x8C, CFile::begin);
		pDlg->m_AVIFile.Write(&nCounts, 4);
		dwRetVal = dwRetVal / 10;
		pDlg->m_AVIFile.Write(&dwRetVal, 4);
		dwRetVal = 0;
		pDlg->m_AVIFile.Seek(0x90,CFile::begin);
		pDlg->m_AVIFile.Write(&dwRetVal,4);
		dwRetVal = 0x100001;
		pDlg->m_AVIFile.Seek(0x134,CFile::begin);
		pDlg->m_AVIFile.Write(&dwRetVal,4);
		pDlg->m_AVIFile.Close();
		pDlg->OnClose();
		MessageBox(NULL, _T("AVI Conversion Success!!!"), NULL, 0);
		
		//pDlg->m_bEnd = false;
		//if(pDlg->m_bEnd == false) pDlg->OnClose();
	return 0;
}
void CProgressDlg::Init_AVI_Convert(CFile * _1stFile, CFile *_2ndFile, float flt_1stStartPos, float flt_1stEndPos, float flt_2ndStartPos, float flt_2ndEndPos)
{
	m_1stFile = _1stFile;
	m_2ndFile = _2ndFile;
	if(flt_1stEndPos != 0)
	{
		m_1stSpliter.Initialization(m_1stFile);
		//m_1stFile->Open(CToolBarDlg::m_1stFilePath,CFile::modeRead);
		int		nPacketCounts;
		DWORD dwPos = (DWORD)(m_1stFile->GetLength() * flt_1stEndPos) + ASF_FILE_HEAD_SIZE;
		nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE - nPacketCounts) / PACKET_SIZE;
		dwPos = ASF_FILE_HEAD_SIZE + nPacketCounts * PACKET_SIZE;
		m_limiteVal = dwPos;
		m_1stSpliter.Seek(flt_1stStartPos);
	}
	if(flt_2ndEndPos != 0)
	{
		m_2ndSpliter.Initialization(m_2ndFile);
		int		nPacketCounts;
		DWORD dwPos = (DWORD)(m_2ndFile->GetLength() * flt_2ndEndPos) + ASF_FILE_HEAD_SIZE;
		nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
		nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE - nPacketCounts) / PACKET_SIZE;
		dwPos = ASF_FILE_HEAD_SIZE + nPacketCounts * PACKET_SIZE;
		m_limiteVal = dwPos;
		//m_2ndFile->Open(CToolBarDlg::m_2ndFilePath,CFile::modeRead);
		m_2ndSpliter.Seek(flt_2ndStartPos);
	}
	AfxBeginThread(AVI_SpliterProc, this);
}

void CProgressDlg::Init_ConvertHeader(int nWidth, int nHeight)
{
	HINSTANCE hXvidEnc = LoadLibrary((LPCTSTR)("xvidcore.dll"));
	m_Func_XvidEnc = (FuncAVI_Xvid_EnCore)GetProcAddress(hXvidEnc,(LPCSTR)("xvid_encore"));
	HINSTANCE hXvidCreate = LoadLibrary((LPCTSTR)("xvidcore.dll"));
	m_Func_XvidCreate = (FuncAVI_Xvid_Create)GetProcAddress(hXvidCreate, (LPCSTR)("xvid_encore"));
	HINSTANCE hXvidGlobal = LoadLibrary((LPCTSTR)("xvidcore.dll"));
	m_Func_XvidGlobal = (FuncAVI_Xvid_Global)GetProcAddress(hXvidGlobal, (LPCSTR)("xvid_global"));
////////////////////////////
	ZeroMemory(&m_lpCreate, sizeof(XVID_CREATE));
	m_lpCreate.nConst = 0x10202;
	m_lpCreate.Unk1 = 3;
	m_lpCreate.Unk2 = nWidth;
	m_lpCreate.Unk3 = nHeight;
	m_lpCreate.Unk4 = 0;
	m_lpCreate.Unk5 = 0;
	m_lpCreate.Unk6 = 0;
	m_lpCreate.Unk7 = 0;
	m_lpCreate.Unk8 = 0;
	m_lpCreate.Unk9 = 0;
	m_lpCreate.Unk10 = 0;
	m_lpCreate.PushData = 1;
	m_lpCreate.Unk11 = 0x19;
	m_lpCreate.Unk12 = 0xFA;
	m_lpCreate.Unk13 = 0;
	m_lpCreate.nWidth = 0x96;
	m_lpCreate.Unk15 = 0x64;
	m_lpCreate.Unk16 = 0;
	m_lpCreate.Unk17 = 0;
	m_lpCreate.Unk18 = 0;
	m_lpCreate.Unk19 = 0;
	m_lpCreate.Unk20 = 0;
	m_lpCreate.PopData = 0;

//////////////////////////////////
	ZeroMemory(&m_lpHandle, sizeof(XVID_HANDLE));
	m_lpHandle.m_nWidth = nWidth;
	m_lpHandle.m_nHeight = nHeight;
	if(nWidth == 704) m_lpHandle.Unk1 = 0x340;
	else m_lpHandle.Unk1 = 0x600;
	m_lpHandle.Unk2 = 0x260;
	if(nWidth == 704) m_lpHandle.Unk3 = 0x2C;
	else m_lpHandle.Unk3 = 0x58;
	m_lpHandle.Unk4 = 0x1E;
	m_lpHandle.Unk5 = 0x0;
	m_lpHandle.Unk6 = 0x1;
	m_lpHandle.Unk7 = 0x19;
	m_lpHandle.Unk8 = 0x3;
	m_lpHandle.Unk9 = 0x0;
	m_lpHandle.Unk10 = 0x96;
	m_lpHandle.Unk11 = 0x64;
	m_lpHandle.Unk12 = 0x0;
	m_lpHandle.Unk13 = 0x2;
	m_lpHandle.Unk14 = 0x2;
	m_lpHandle.Unk15 = 0x2;
	m_lpHandle.Unk16 = 0x1F;
	m_lpHandle.Unk17 = 0x1F;
	m_lpHandle.Unk18 = 0x1F;
	m_lpHandle.Unk19 = 0x0;
	m_lpHandle.Unk20 = 0x0;
	m_lpHandle.Unk21 = 0x0;
	m_lpHandle.Unk22 = 0xFA;
	m_lpHandle.Unk23 = 0x0;
////////////////////////////////////////
	ZeroMemory(&m_lpState, sizeof(XVID_STATE));
	m_lpState.nConst = 0x10202;
	m_lpState.nUnk1 = 0;
	m_lpState.nUnk2 = 0;
	m_lpState.nUnk3 = 0;
	m_lpState.nUnk4 = 0;
	m_lpState.nEncSize = 0;
	m_lpState.nUnk6 = 0;
	m_lpState.nUnk7 = 0;
	m_lpState.nUnk8 = 0;
	m_lpState.nUnk9 = 0;
///////////////////////////////////////
//	ZeroMemory(&m_lpFrame, sizeof(XVID_FRAME));
	m_lpFrame.nConst = 0x10202;
	m_lpFrame.Unk1 = 0;
	m_lpFrame.Unk2 = 0;
	m_lpFrame.Unk3 = 0;
	m_lpFrame.Unk4 = 0;
	m_lpFrame.Unk5 = 0;
	m_lpFrame.Unk6 = 0;
	m_lpFrame.Unk7 = 0;
	m_lpFrame.Unk8 = 6;
	m_lpFrame.Unk9 = 0x3053;
	m_lpFrame.Unk10 = 2;
	
	m_lpFrame.Unk11 = 0;
	m_lpFrame.Unk12 = 0;
	m_lpFrame.Unk13 = 0;
	m_lpFrame.nWidth = nWidth;
	m_lpFrame.Unk15 = 0;
	m_lpFrame.Unk16 = 0;
	m_lpFrame.Unk17 = 0;
	m_lpFrame.Unk18 = 0;
	m_lpFrame.Unk19 = 3;
	m_lpFrame.Unk20 = 0;
	m_lpFrame.Unk21 = 0xFFFFFFFF;
	m_lpFrame.Unk22 = 0;

	ZeroMemory(&m_Riff, sizeof(RIFF));
	m_Riff.chRiff[0] = 'R';
	m_Riff.chRiff[1] = 'I';
	m_Riff.chRiff[2] = 'F';
	m_Riff.chRiff[3] = 'F';

	ZeroMemory(&m_Junk, sizeof(JUNK));
	m_Junk.chJunk[0] = 'J';
	m_Junk.chJunk[1] = 'U';
	m_Junk.chJunk[2] = 'N';
	m_Junk.chJunk[3] = 'K';
	m_Junk.dwJunkSize = 0x660;
	
	ZeroMemory(&m_InfoList, sizeof(INFOLIST));
	m_InfoList.chList[0] = 'L';
	m_InfoList.chList[1] = 'I';
	m_InfoList.chList[2] = 'S';
	m_InfoList.chList[3] = 'T';
	m_InfoList.dwInfoListFrameSize = 0x4C;
	m_InfoList.chInfoList[0] = 'I';
	m_InfoList.chInfoList[1] = 'N';
	m_InfoList.chInfoList[2] = 'F';
	m_InfoList.chInfoList[3] = 'O';
	m_InfoList.chInfoList[4] = 'I';
	m_InfoList.chInfoList[5] = 'S';
	m_InfoList.chInfoList[6] = 'F';
	m_InfoList.chInfoList[7] = 'T';
	m_InfoList.dwInfoSize = 0x40;
	
	ZeroMemory(&m_AviList, sizeof(AVI_LIST));
	m_AviList.chAvi_List[0] = 'A';
	m_AviList.chAvi_List[1] = 'V';
	m_AviList.chAvi_List[2] = 'I';
	m_AviList.chAvi_List[3] = ' ';
	m_AviList.chAvi_List[4] = 'L';
	m_AviList.chAvi_List[5] = 'I';
	m_AviList.chAvi_List[6] = 'S';
	m_AviList.chAvi_List[7] = 'T';
	m_AviList.dwAviListStructSize = 0x124;
	m_AviList.m_AviListHeader.chAviListHeader[0] = 'h';
	m_AviList.m_AviListHeader.chAviListHeader[1] = 'd';
	m_AviList.m_AviListHeader.chAviListHeader[2] = 'r';
	m_AviList.m_AviListHeader.chAviListHeader[3] = 'l';
	m_AviList.m_AviListHeader.chAviListHeader[4] = 'a';
	m_AviList.m_AviListHeader.chAviListHeader[5] = 'v';
	m_AviList.m_AviListHeader.chAviListHeader[6] = 'i';
	m_AviList.m_AviListHeader.chAviListHeader[7] = 'h';
	m_AviList.m_AviListHeader.dwHeaderSize = 0x38;
	m_AviList.m_AviListHeader.dwSecPerFrames = 0x186A0;
	m_AviList.m_AviListHeader.dwConst1[2] = 0x110;
	m_AviList.m_AviListHeader.dwConst2[1] = 2;
	m_AviList.m_AviListHeader.nVideoWidth = nWidth;
	m_AviList.m_AviListHeader.nVideoHeight = nHeight;
	memcpy(m_AviList.m_VideoList.chList, m_InfoList.chList, 4);
	m_AviList.m_VideoList.dwVideoListSize = 0x74;
	m_AviList.m_VideoList.chStringHeader[0] = 's';
	m_AviList.m_VideoList.chStringHeader[1] = 't';
	m_AviList.m_VideoList.chStringHeader[2] = 'r';
	m_AviList.m_VideoList.chStringHeader[3] = 'l';
	m_AviList.m_VideoList.chStringHeader[4] = 's';
	m_AviList.m_VideoList.chStringHeader[5] = 't';
	m_AviList.m_VideoList.chStringHeader[6] = 'r';
	m_AviList.m_VideoList.chStringHeader[7] = 'h';
	m_AviList.m_VideoList.dwHeaderSize = 0x38;
	m_AviList.m_VideoList.chVideo[0] = 'v';
	m_AviList.m_VideoList.chVideo[1] = 'i';
	m_AviList.m_VideoList.chVideo[2] = 'd';
	m_AviList.m_VideoList.chVideo[3] = 's';
	m_AviList.m_VideoList.chVideo[4] = 'x';
	m_AviList.m_VideoList.chVideo[5] = 'v';
	m_AviList.m_VideoList.chVideo[6] = 'i';
	m_AviList.m_VideoList.chVideo[7] = 'd';
	m_AviList.m_VideoList.dwConst[0] = 0xF4240;
	m_AviList.m_VideoList.dwConst[1] = 0x989680;
	m_AviList.m_VideoList.dwReserved2[1] = 0xFFFFFFFF;
	m_AviList.m_VideoList.m_VideoFrame.chFrame[0] = 's';
	m_AviList.m_VideoList.m_VideoFrame.chFrame[1] = 't';
	m_AviList.m_VideoList.m_VideoFrame.chFrame[2] = 'r';
	m_AviList.m_VideoList.m_VideoFrame.chFrame[3] = 'f';
	m_AviList.m_VideoList.m_VideoFrame.dwFrameSize1 = 0x28;
	m_AviList.m_VideoList.m_VideoFrame.dwFrameSize2 = 0x28;
	m_AviList.m_VideoList.m_VideoFrame.nVideoWidth = nWidth;
	m_AviList.m_VideoList.m_VideoFrame.nVideoHeight = nHeight;
	m_AviList.m_VideoList.m_VideoFrame.dwUnk[0] = 0x180001;
	m_AviList.m_VideoList.m_VideoFrame.dwUnk[1] = 0x64697678;
	m_AviList.m_VideoList.m_VideoFrame.dwUnk[2] = nWidth * nHeight * 3;
	memcpy(m_AviList.m_AudioList.chList, m_InfoList.chList, 4);
	m_AviList.m_AudioList.dwAudioListSize = 0x5C;
	memcpy(m_AviList.m_AudioList.m_StringHeader.chStringHeader, m_AviList.m_VideoList.chStringHeader, 8);
	m_AviList.m_AudioList.m_StringHeader.dwHeaderSize = 0x38;
	m_AviList.m_AudioList.m_StringHeader.chAudios[0] = 'a';
	m_AviList.m_AudioList.m_StringHeader.chAudios[1] = 'u';
	m_AviList.m_AudioList.m_StringHeader.chAudios[2] = 'd';
	m_AviList.m_AudioList.m_StringHeader.chAudios[3] = 's';
	m_AviList.m_AudioList.m_StringHeader.nChannelCounts = 1;
	m_AviList.m_AudioList.m_StringHeader.dwSecPerbits = 0x3E80;
	m_AviList.m_AudioList.m_StringHeader.dwReserved2[1] = 0x6FE00;
	m_AviList.m_AudioList.m_StringHeader.dwReserved2[3] = 0xFFFFFFFF;
	m_AviList.m_AudioList.m_StringHeader.dwReserved2[4] = 1;
	memcpy(m_AviList.m_AudioList.m_AudioFrame.chFrame, m_AviList.m_VideoList.m_VideoFrame.chFrame, 4);
	m_AviList.m_AudioList.m_AudioFrame.dwFrameSize = 0x10;
	m_AviList.m_AudioList.m_AudioFrame.dwConst1 = 0x10001;
	m_AviList.m_AudioList.m_AudioFrame.dwHz = 0x1F40;
	m_AviList.m_AudioList.m_AudioFrame.dwSecPerbits = 0x3E80;
	
	ZeroMemory(&m_ListFrameHeader, sizeof(LIST_FRAME_HEADER));
	m_ListFrameHeader.dwVideoID = 0x62643030;
	m_ListFrameHeader.dwB0_HeaderID = 0xB0010000;
	m_ListFrameHeader.chB0_Data = 3;
	m_ListFrameHeader.dwB5_HeaderID = 0xB5010000;
	m_ListFrameHeader.dwB5_Datas[0] = 0x01000009;
	m_ListFrameHeader.dwB5_Datas[1] = 0x01000000;
	m_ListFrameHeader.dwB5_Datas[2] = 0xC4860020;
	m_ListFrameHeader.dwB5_Datas[3] = 0x580C6700;
	m_ListFrameHeader.dwB5_Datas[4] = 0x8F51F010;
	m_ListFrameHeader.dwB2_HeaderID = 0xB2010000;
	m_ListFrameHeader.dwB2_Datas[0] = 0x44697658;
	m_ListFrameHeader.dwB2_Datas[1] = 0x30353030;
	m_ListFrameHeader.dwB6_HeaderID = 0xB6010000;

	ZeroMemory(&m_B6FrameHeader, sizeof(B6_FRAME_HEADER));
	m_B6FrameHeader.dwB6_HeaderID = 0xB6010000;
	
	ZeroMemory(&m_AudioFrameHeader, sizeof(AUDIO_FRAME_HEADER));
	m_AudioFrameHeader.dwAudioHeaderID = 0x62773130;

	ZeroMemory(&m_TrackFrame, sizeof(TRACK_FRAME));
	memcpy(m_TrackFrame.chList, m_InfoList.chList, 4);
	m_TrackFrame.chMovie[0] = 'm';
	m_TrackFrame.chMovie[1] = 'o';
	m_TrackFrame.chMovie[2] = 'v';
	m_TrackFrame.chMovie[3] = 'i';

	ZeroMemory(&m_SeekPosFrame, sizeof(SEEKPOS_FRAME));
}

BOOL CProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_progress.SetRange(0,100);
	// TODO: Add extra initialization here
	HBITMAP hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BITMAP_AVI_STOP));
	m_btn_Stop.SetBitmap(hBmp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressDlg::OnClose()
{
	CProgressDlg::OnCancel();
}

void CProgressDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rt;
	GetClientRect(&rt);
	dc.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));
	// Do not call CDialog::OnPaint() for painting messages
}
