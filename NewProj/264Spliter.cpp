// 264Spliter.cpp: implementation of the C264Spliter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewProj.h"
#include "264Spliter.h"
extern "C"
{
#include "Yuv2rgb.h"
}
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AudioHeader			g_AudioHeader;
AudioHeader			g_AudioHeader2;
unsigned char		g_subAudioBuf[SUB_AUDIO_FRAME_SIZE];
unsigned char		g_subAudioBuf2[SUB_AUDIO_FRAME_SIZE];
char				g_AudioBuf[320];
char				g_AudioBuf2[320];
OUT_FRAME			g_Output;
OUT_FRAME			g_Output2;
unsigned char		g_RGB_Buf[704 * 480 * 3];
unsigned char		g_RGB_Buf2[704 * 480 * 3];
OUT_FRAME			C264Spliter::m_OutFrame;
BYTE				C264Spliter::m_pY[0x65400];
BYTE				C264Spliter::m_pU[0x19500];
BYTE				C264Spliter::m_pV[0x19500];
BYTE				C264Spliter::m_pY2[0x65400];
BYTE				C264Spliter::m_pU2[0x19500];
BYTE				C264Spliter::m_pV2[0x19500];

OUT_DATAS			C264Spliter::m_1stOutDatas;
OUT_DATAS			C264Spliter::m_2ndOutDatas;
DWORD				C264Spliter::m_dw1stSeekTime;
DWORD				C264Spliter::m_dw2ndSeekTime;
DWORD				C264Spliter::m_dwFirstDTS;
C264Spliter::C264Spliter()
{
	
	m_dwSeekPos = 0;
	m_1stFrameNum = -1;
	m_2ndFrameNum = -1;
	m_1stEndFlag = false;
	m_2ndEndFlag = false;
	m_dw1stSeekTime = 0;
	m_dw2ndSeekTime = 0;
	m_Demuxer.Init_Hi264();
	(*m_Demuxer.Hi264DecGetInfo)(&m_Demuxer.m_DecInfo);
	ZeroMemory(&m_Demuxer.m_DecInfo, 0x34);
	m_Demuxer.m_DecInfo.dw24 = 0x2D;
	m_Demuxer.m_DecInfo.dw28 = 0x24;
	m_Demuxer.m_DecInfo.dw2C = 0x00;
	m_Demuxer.m_DecInfo.dw14 = 0;
	m_hDec = (*m_Demuxer.Hi264DecCreate)(&m_Demuxer.m_DecInfo);
	m_hDec2 = (*m_Demuxer.Hi264DecCreate)(&m_Demuxer.m_DecInfo);
}

C264Spliter::~C264Spliter()
{

}

bool C264Spliter::OpenFolder(CString StrFolderPath)
{
	return false;
}

int C264Spliter::OpenFile(CString pStrFileName, float SeekPos)
{
	int retval = 0;
	if(SeekPos == 0)
	{
		m_strFileName = pStrFileName;
		Seek(SeekPos);
		m_file->Seek(ASF_FILE_HEAD_SIZE, CFile::begin);
		ReadPacketHeader();
		return  retval;
	}else{
		m_strFileName = pStrFileName;
		m_file->Seek((long)(SeekPos * m_file->GetLength()), CFile::begin);
		return 0;
	}
}

DWORD C264Spliter::GetFileLength()
{
	return  m_file->GetLength();
}

void C264Spliter::CloseFile()
{
	m_file->Close();
}

DWORD C264Spliter::GetFilePosition()
{
	return m_file->GetPosition();
}
void C264Spliter::Init_Voice()
{
	ZeroMemory(&g_AudioHeader, sizeof(g_AudioHeader));
	DWORD dwResult = (*m_Demuxer.HI_VOICE_DecReset)(&g_AudioHeader, 7);
	ZeroMemory(&g_AudioHeader2, sizeof(g_AudioHeader2));
	(*m_Demuxer.HI_VOICE_DecReset)(&g_AudioHeader2, 7);
}
void C264Spliter::Initialization(CFile* file)
{
	m_file = file;
	m_strFileName = "";
	m_dwFileLength = 0;
	m_dwFilePos = 0;

// 	ZeroMemory(&g_AudioHeader, sizeof(g_AudioHeader));
// 	DWORD dwResult = (*m_Demuxer.HI_VOICE_DecReset)(&g_AudioHeader, 7);
// 	ZeroMemory(&g_AudioHeader2, sizeof(g_AudioHeader2));
// 	(*m_Demuxer.HI_VOICE_DecReset)(&g_AudioHeader2, 7);

	memset(m_byFrameData, 0, FRAME_MAX_SIZE);
	memset(m_buf, 0, PACKET_BODY_SIZE);
	memset(&m_PacketConstHeader, 0, sizeof(m_PacketConstHeader));
	memset(&m_Packet00Header, 0, sizeof(m_Packet00Header));
	memset(&m_Packet01Header, 0, sizeof(m_Packet01Header));
	memset(&m_Packet08Header, 0 , sizeof(m_Packet08Header));
	memset(&m_Packet09Header, 0, sizeof(m_Packet09Header));
	memset(&m_Packet10Header, 0, sizeof(m_Packet10Header));
	memset(&m_Packet11Header, 0, sizeof(m_Packet11Header));
	memset(&m_FrameHeader, 0, sizeof(m_FrameHeader));
	
	m_byCurPacketType = 0;
	m_byCurFrameType = 0;
	m_dwCurFrameSize = 0;
	m_dwCurReadSizeOfFrame = 0;
	m_dwCurPacketHeaderSize = 0;
	m_dwReadSize = 0;
	m_dwPacketReadSize = 0;	
}

BOOL C264Spliter::ReadFrameData(void)
{	
	do{		
		switch(m_byCurPacketType){
		case PACKET00_TYPE:
			m_dwReadSize = m_file->Read((void *)m_buf, PACKET_SIZE - sizeof(m_Packet00Header) - sizeof(m_PacketConstHeader));
			memcpy((m_byFrameData + m_dwCurFrameSize), m_buf, m_dwReadSize);
			m_dwCurFrameSize += m_dwReadSize;
			ReadPacketHeader();
			break;
		case PACKET01_TYPE:			
			m_dwReadSize = m_file->Read((void *)m_buf, m_FrameHeader.wdTailFrameSize);
			memcpy((void*)(m_byFrameData + m_dwCurFrameSize), (void*)m_buf, m_dwReadSize);
			m_dwCurFrameSize += m_dwReadSize;
			if(m_FrameHeader.dwFrameSize <= m_dwCurFrameSize) {
				m_byCurFrameType = m_FrameHeader.byFrameType;
				if((m_file->GetPosition() - ASF_FILE_HEAD_SIZE) % PACKET_SIZE == 0)
				{
					ReadPacketHeader();
					break;
				}else{
					ReadFrameHeader();
					return TRUE;
				}
			}
			ReadPacketHeader();
			break;
		case PACKET08_TYPE:
			m_dwReadSize = m_file->Read((void *)m_buf, (m_FrameHeader.dwFrameSize - m_dwCurFrameSize));
			m_file->Seek(PACKET_SIZE - sizeof(m_Packet08Header) - sizeof(m_PacketConstHeader) - (m_FrameHeader.dwFrameSize - m_dwCurFrameSize), CFile::current);
			memcpy((void *)(m_byFrameData + m_dwCurFrameSize), (void *)m_buf, m_dwReadSize);
			m_dwCurFrameSize += m_dwReadSize;
			m_byCurFrameType = m_FrameHeader.byFrameType;
			ReadPacketHeader();
			DWORD dwTemp;
			m_file->Read(&dwTemp, 4);

			if(dwTemp == 0x01000000 || (dwTemp & 0xFF) == 0x03 || (dwTemp & 0xFF) == 0x0F || dwTemp == 0x00320100)
			{
				m_file->Seek(-4, CFile::current);
			}else{
				if((dwTemp & 0xFFFF) == 0x8200 || (dwTemp & 0xFFFF) == 0x0200)
				{
					m_file->Seek(-3, CFile::current);
					ReadFrameHeader();
				}else{
					m_file->Seek(-4, CFile::current);
				}
			}
			return TRUE;
		case PACKET09_TYPE:
			m_dwReadSize = m_file->Read((void *)m_buf, m_FrameHeader.wdTailFrameSize);
			memcpy((void*)(m_byFrameData + m_dwCurFrameSize), (void*)m_buf, m_dwReadSize);
			m_dwCurFrameSize += m_dwReadSize;
			m_dwPacketReadSize += m_dwReadSize;
			m_byCurFrameType = m_FrameHeader.byFrameType;
			if(m_FrameHeader.dwFrameSize <= m_dwCurFrameSize)
			{
				if((PACKET_SIZE - sizeof(m_Packet09Header) - m_dwPacketReadSize - sizeof(m_PacketConstHeader)) <= sizeof(m_FrameHeader))
				{
					m_file->Read((void*)m_buf, PACKET_SIZE - sizeof(m_Packet09Header) - m_dwPacketReadSize - sizeof(m_PacketConstHeader));
					ReadPacketHeader();
				}else{
					ReadFrameHeader();
					m_dwPacketReadSize += sizeof(m_FrameHeader);
				}
				//m_byCurFrameType = m_FrameHeader.byFrameType;
				return TRUE;
			}else{		////////New Appended Codes
				m_file->Seek(PACKET_SIZE - sizeof(m_PacketConstHeader) - sizeof(m_Packet09Header) - m_dwPacketReadSize, CFile::current);
				ReadPacketHeader();
				break; 
			}			////////End
			break;
		case PACKET10_TYPE:
			m_dwReadSize = m_file->Read((void *)m_buf, PACKET_SIZE - sizeof(m_Packet10Header) - sizeof(m_PacketConstHeader));
			memcpy((void *)(m_byFrameData + m_dwCurFrameSize), (void *)m_buf, m_dwReadSize);
			m_dwCurFrameSize += m_dwReadSize;
			m_1stEndFlag = true;
			m_2ndEndFlag = true;
			return TRUE;
		case PACKET11_TYPE:
			m_dwReadSize = m_file->Read((void *)m_buf, m_FrameHeader.wdTailFrameSize);
			if(m_FrameHeader.byHeaderId != 0x08) return FALSE;
			memcpy((void*)(m_byFrameData + m_dwCurFrameSize), (void*)m_buf, m_dwReadSize);
			m_dwCurFrameSize += m_dwReadSize;
			if(m_FrameHeader.dwFrameSize <= m_dwCurFrameSize) {
				m_byCurFrameType = m_FrameHeader.byFrameType;
				ReadFrameHeader();
				m_1stEndFlag = true;
				m_2ndEndFlag = true;
				return TRUE;
			}
			m_1stEndFlag = true;
			m_2ndEndFlag = true;
			break;
		default:
			return FALSE;
		}
	} while(GetFilePosition() < GetFileLength());
	return TRUE;
}

void C264Spliter::ReadFrameHeader()
{
	m_file->Read((void*)&m_FrameHeader, sizeof(m_FrameHeader));
}

BYTE C264Spliter::ReadOneFrame(BYTE*& pByPtrFrameOut, DWORD *pDwFrameSize)
{	
	memset(m_byFrameData, 0, sizeof(m_byFrameData));
	
	m_byCurFrameType = m_FrameHeader.byFrameType;
	m_dwCurFrameSize = 0;
	m_byCurFrameNum = m_FrameHeader.byFrameNum;
	m_dwCurDTS = m_FrameHeader.dwDTS;
	if(!ReadFrameData()) 
		return 0xff;
	pByPtrFrameOut = m_byFrameData;
	*pDwFrameSize = m_dwCurFrameSize;
	return m_byCurFrameType;
}

void C264Spliter::ReadPacketHeader()
{
	m_file->Read((void*)&m_PacketConstHeader, sizeof(m_PacketConstHeader));
	m_byCurPacketType = m_PacketConstHeader.byType;
	int n = m_file->GetPosition();
	if(m_PacketConstHeader.byHead != 0x82 || m_PacketConstHeader.wdConst != 0x0000 || m_PacketConstHeader.byConst != 0x5d)
		int  a = 0;
	switch(m_byCurPacketType) {
	case PACKET00_TYPE:
		m_file->Read((void*)&m_Packet00Header, sizeof(m_Packet00Header));
		m_FrameHeader.byFrameType = m_Packet00Header.byFrameType;
		m_FrameHeader.byFrameNum = m_Packet00Header.byFrameNum;
		m_FrameHeader.dwDTS = m_Packet00Header.dwDTS;
		m_FrameHeader.dwFrameSize = m_Packet00Header.dwFrameSize;
		break;
	case PACKET01_TYPE:
		m_file->Read((void*)&m_Packet01Header, sizeof(m_Packet01Header));
		m_FrameHeader.byFrameType = m_Packet01Header.byFrameType;
		m_FrameHeader.byFrameNum = m_Packet01Header.byFrameNum;
		m_FrameHeader.dwDTS = m_Packet01Header.dwDTS;
		m_FrameHeader.dwFrameSize = m_Packet01Header.dwFrameSize;
		m_FrameHeader.wdTailFrameSize = m_Packet01Header.wdTailFrameSize;
		break;
	case PACKET08_TYPE:
		m_file->Read((void*)&m_Packet08Header, sizeof(m_Packet08Header));
		m_FrameHeader.byFrameType = m_Packet08Header.byFrameType;
		m_FrameHeader.byFrameNum = m_Packet08Header.byFrameNum;
		m_FrameHeader.dwDTS = m_Packet08Header.dwDTS;
		m_FrameHeader.dwFrameSize = m_Packet08Header.dwFrameSize;
		break;
	case PACKET09_TYPE:
		m_file->Read((void*)&m_Packet09Header, sizeof(m_Packet09Header));
		m_dwPacketReadSize = 0;
		m_FrameHeader.byFrameType = m_Packet09Header.byFrameType;
		m_FrameHeader.byFrameNum = m_Packet09Header.byFrameNum;
		m_FrameHeader.dwDTS = m_Packet09Header.dwDTS;
		m_FrameHeader.dwFrameSize = m_Packet09Header.dwFrameSize;
		m_FrameHeader.wdTailFrameSize = m_Packet09Header.wdTailFrameSize;
		break;
	case PACKET10_TYPE:
		m_file->Read((void*)&m_Packet10Header, sizeof(m_Packet10Header));
		m_FrameHeader.byFrameType = m_Packet10Header.byFrameType;
		m_FrameHeader.byFrameNum = m_Packet10Header.byFrameNum;
		m_FrameHeader.dwDTS = m_Packet10Header.dwDTS;
		m_FrameHeader.dwFrameSize = m_Packet10Header.dwFrameSize;
		break;
	case PACKET11_TYPE:
		m_file->Read((void*)&m_Packet11Header, sizeof(m_Packet11Header));
		m_FrameHeader.byFrameType = m_Packet11Header.byFrameType;
		m_FrameHeader.byFrameNum = m_Packet11Header.byFrameNum;
		m_FrameHeader.dwDTS = m_Packet11Header.dwDTS;
		m_FrameHeader.dwFrameSize = m_Packet11Header.dwFrameSize;
		m_FrameHeader.wdTailFrameSize = m_Packet11Header.wdTailFrameSize;
		break;
	}
}
char chAudioTemp1[0x208], chAudioTemp2[0x208];
int C264Spliter::GetDatas1()
{
	int nResult;
	BYTE  *pByPtrFrameOut;
	DWORD pDwFrameSize;
	BYTE byFrameType = ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);

	if (byFrameType == 0xff) 
		return -1;
	
	if ((*m_Demuxer.Hi264DecFrame)(m_hDec, pByPtrFrameOut, pDwFrameSize, 0, 0, &g_Output, 0) == 0)
	{
		memcpy(m_pY, g_Output.ptr_uChY, 0x65400);
		memcpy(m_pU, g_Output.ptr_uChU, 0x19500);
		memcpy(m_pV, g_Output.ptr_uChV, 0x19500);
		yuv2rgb_24(g_Output.ptr_uChY, g_Output.dwStride_y, g_Output.ptr_uChU, g_Output.ptr_uChV,
			g_Output.dwStride_uv, g_RGB_Buf, g_Output.dwWidth, g_Output.dwHeigth, g_Output.dwStride_y);
		
		do { 
			nResult = (*m_Demuxer.Hi264DecFrame)(m_hDec, NULL, 0, 0, 0, &g_Output, 0); 
		} while(nResult == 0);
	}
	
	m_OutFrame = g_Output;
	m_1stOutDatas.dwDTS_V = m_dwCurDTS;
	memcpy(m_1stOutDatas.buf_V, g_RGB_Buf, 704 * 480 * 3);
	m_1stOutDatas.bBuf = true;
	DWORD dwAudioSize = 0;

	if(m_FrameHeader.byFrameType == TYPE_AUDIO)
	{
		byFrameType = ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);
		if (byFrameType == 0xff) 
			return -1;
	
	memset(m_1stOutDatas.buf_A, 0x80, 1600);
	if(pDwFrameSize != 0x208)
	{
		do{
			byFrameType = ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);
			if(byFrameType == 0xff) return -1;
			if(m_1stEndFlag == true) return -1;
		}while(pDwFrameSize != 0x208 || byFrameType != 0x82);
	}
	if(pDwFrameSize != 0x208)
		memcpy(pByPtrFrameOut, chAudioTemp1, 0x208);
	if(pDwFrameSize == 0x208)
	{
		for( int j = 0; j < (int)pDwFrameSize / SUB_AUDIO_FRAME_SIZE; j++)
		{
			memset(g_subAudioBuf, 0, 104);
			memset(g_AudioBuf, 0, 1600);
			memcpy((void*)g_subAudioBuf, (void*)(pByPtrFrameOut + SUB_AUDIO_FRAME_SIZE*j), SUB_AUDIO_FRAME_SIZE);
			memset(g_AudioBuf, 0, sizeof(g_AudioBuf));
			if((*m_Demuxer.HI_VOICE_DecodeFrame)(&g_AudioHeader, g_subAudioBuf, g_AudioBuf, &dwAudioSize) == 0)
			{			
				m_1stOutDatas.dwDTS_A = m_dwCurDTS;
				memcpy((void*)(m_1stOutDatas.buf_A + dwAudioSize * j * 2), g_AudioBuf, dwAudioSize * 2);
			}else{
					nResult = (*m_Demuxer.HI_VOICE_DecodeFrame)(&g_AudioHeader, NULL, g_AudioBuf, 0);
				continue;
			}
		}
	}
	}else{
		if(m_FrameHeader.byFrameType == 0x02)
		{
			ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);
			if(m_FrameHeader.byFrameType != 0x03)
			{
				memset(m_1stOutDatas.buf_A, 0, 1600);
				return 0;
			}
		}else{
		memcpy(pByPtrFrameOut, chAudioTemp1, 0x208);
		for( int j = 0; j < (int)pDwFrameSize / SUB_AUDIO_FRAME_SIZE; j++)
		{
			memset(g_subAudioBuf, 0, 104);
			memset(g_AudioBuf, 0, 1600);
			memcpy((void*)g_subAudioBuf, (void*)(pByPtrFrameOut + SUB_AUDIO_FRAME_SIZE*j), SUB_AUDIO_FRAME_SIZE);
			memset(g_AudioBuf, 0, sizeof(g_AudioBuf));
			if((*m_Demuxer.HI_VOICE_DecodeFrame)(&g_AudioHeader, g_subAudioBuf, g_AudioBuf, &dwAudioSize) == 0)
			{			
				m_1stOutDatas.dwDTS_A = m_dwCurDTS;
				memcpy((void*)(m_1stOutDatas.buf_A + dwAudioSize * j * 2), g_AudioBuf, dwAudioSize * 2);
			}else{
				nResult = (*m_Demuxer.HI_VOICE_DecodeFrame)(&g_AudioHeader, NULL, g_AudioBuf, 0);
				continue;
			}
		}
		}
	}
	memcpy(chAudioTemp1, pByPtrFrameOut, 0x208);
	if(m_FrameHeader.byFrameType != TYPE_BINARY && m_FrameHeader.byFrameType != TYPE_AUDIO) return 0;
	byFrameType = ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);
	m_1stOutDatas.nB_Size = pDwFrameSize;
	DWORD dwPos = m_file->GetPosition();
	memcpy(&m_1stOutDatas.mB_Data, pByPtrFrameOut,pDwFrameSize);
	return 0;
}

int C264Spliter::GetDatas2()
{
	int nResult;
	BYTE  *pByPtrFrameOut; 
	DWORD pDwFrameSize;
	DWORD	dwPos;
	nResult = m_file->GetPosition();
	BYTE byFrameType = ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);
	if(byFrameType == 0xff) return -1;
	if((*m_Demuxer.Hi264DecFrame)(m_hDec2, pByPtrFrameOut, pDwFrameSize, 0, 0, &g_Output2, 0) == 0)
	{
		memcpy(m_pY2, g_Output2.ptr_uChY, 0x65400);
		memcpy(m_pU2, g_Output2.ptr_uChU, 0x19500);
		memcpy(m_pV2, g_Output2.ptr_uChV, 0x19500);
		yuv2rgb_24(g_Output2.ptr_uChY, g_Output2.dwStride_y, g_Output2.ptr_uChU, g_Output2.ptr_uChV,
			g_Output2.dwStride_uv, g_RGB_Buf2, g_Output2.dwWidth, g_Output2.dwHeigth, g_Output2.dwStride_y);
		do{ nResult = (*m_Demuxer.Hi264DecFrame)(m_hDec2, NULL, 0, 0, 0, &g_Output2, 0); } while(nResult == 0);		
	}
	m_2ndOutDatas.dwDTS_V = m_dwCurDTS;
	memcpy(m_2ndOutDatas.buf_V, g_RGB_Buf2, 704 * 480 * 3);
	m_2ndOutDatas.bBuf = true;
	DWORD dwAudioSize = 0;
	if(m_FrameHeader.byFrameType == TYPE_AUDIO)
	{
	byFrameType = ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);
	if(byFrameType == 0xff) return -1;
	
	memset(m_2ndOutDatas.buf_A, 0x80, 1600);
	nResult = m_file->GetPosition();
	if(pDwFrameSize != 0x208)
	{
		do{
			byFrameType = ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);
			if(byFrameType == 0xff) return  -1;
			if(m_2ndEndFlag == true) return -1;
		}while(pDwFrameSize != 0x208 || byFrameType != 0x82);
	}
	if(pDwFrameSize != 0x208)
		memcpy(pByPtrFrameOut, chAudioTemp1, 0x208);
	if(pDwFrameSize == 0x208)
	{
		for( int j = 0; j < (int)pDwFrameSize / SUB_AUDIO_FRAME_SIZE; j++)
		{
			memset(g_subAudioBuf2, 0, sizeof(g_subAudioBuf2));
			memcpy((void*)g_subAudioBuf2, (void*)(pByPtrFrameOut + SUB_AUDIO_FRAME_SIZE*j), SUB_AUDIO_FRAME_SIZE);
			memset(g_AudioBuf2, 0, sizeof(g_AudioBuf2));
			if((*m_Demuxer.HI_VOICE_DecodeFrame)(&g_AudioHeader2, g_subAudioBuf2, g_AudioBuf2, &dwAudioSize) == 0)
			{			
				m_2ndOutDatas.dwDTS_A = m_dwCurDTS;
				memcpy((void*)(m_2ndOutDatas.buf_A + dwAudioSize * j * 2), g_AudioBuf2, dwAudioSize * 2);
			}else{
				nResult = (*m_Demuxer.HI_VOICE_DecodeFrame)(&g_AudioHeader2, NULL, g_AudioBuf, 0);
				continue;;
			}
		}
	}
	}else{
		if(m_FrameHeader.byFrameType == 0x02)
		{
			ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);
			if(m_FrameHeader.byFrameType != 0x03) 
			{
				memset(m_2ndOutDatas.buf_A, 0, 1600);
				return 0;
			}
		}else{
		memcpy(pByPtrFrameOut, chAudioTemp2, 0x208);
		for( int j = 0; j < (int)pDwFrameSize / SUB_AUDIO_FRAME_SIZE; j++)
		{
			memset(g_subAudioBuf2, 0, sizeof(g_subAudioBuf2));
			memcpy((void*)g_subAudioBuf2, (void*)(pByPtrFrameOut + SUB_AUDIO_FRAME_SIZE*j), SUB_AUDIO_FRAME_SIZE);
			memset(g_AudioBuf2, 0, sizeof(g_AudioBuf2));
			if((*m_Demuxer.HI_VOICE_DecodeFrame)(&g_AudioHeader2, g_subAudioBuf2, g_AudioBuf2, &dwAudioSize) == 0)
			{			
				m_2ndOutDatas.dwDTS_A = m_dwCurDTS;
				memcpy((void*)(m_2ndOutDatas.buf_A + dwAudioSize * j * 2), g_AudioBuf2, dwAudioSize * 2);
			}else{
				nResult = (*m_Demuxer.HI_VOICE_DecodeFrame)(&g_AudioHeader2, NULL, g_AudioBuf, 0);
				continue;;
			}
		}
		}
	}
	memcpy(chAudioTemp2, pByPtrFrameOut, 0x208);
	if(m_FrameHeader.byFrameType != TYPE_BINARY && m_FrameHeader.byFrameType != TYPE_AUDIO) return 0;
	byFrameType = ReadOneFrame(pByPtrFrameOut, &pDwFrameSize);
	return 0;
}

bool C264Spliter::Seek(float fSeekPos)
{
	DWORD	dwPacketInPos;
	DWORD	dwBuf[2];
	int		nPacketCounts;
	DWORD dwPos = (DWORD)(m_file->GetLength() * fSeekPos) + ASF_FILE_HEAD_SIZE;
	nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
	nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE - nPacketCounts) / PACKET_SIZE;
	dwPos = ASF_FILE_HEAD_SIZE + nPacketCounts * PACKET_SIZE;
	m_file->Seek(dwPos, CFile::begin);
	do{
		m_file->Read(&m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		switch(m_PacketConstHeader.byType)
		{
		case PACKET00_TYPE:
			m_file->Read(&m_Packet00Header, sizeof(PACKET00_HEADER));
			if(m_Packet00Header.byFrameType == 0x81)
			{
				m_file->Read(dwBuf, 8);
				if(dwBuf[0] == 0x01000000 && dwBuf[1] == 0x1EE04267)
				{
					m_dwSeekPos = m_file->GetPosition() - sizeof(PACKET00_HEADER) - sizeof(PACKET_CONST_HEADER) - 8;
					m_file->Seek(m_dwSeekPos, CFile::begin);
					m_byCurPacketType = PACKET00_TYPE;
					ReadPacketHeader();
					m_dwSeekDTS = m_FrameHeader.dwDTS;
					m_dw1stSeekTime = (m_dwSeekDTS - m_dwFirstDTS) / 1000;
					return true;
				}
				m_file->Seek(-8, CFile::current);
			}
			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER) - sizeof(PACKET00_HEADER), CFile::current);
			break;
		case PACKET01_TYPE:
			m_file->Read(&m_Packet01Header, sizeof(PACKET01_HEADER));
			dwPacketInPos = sizeof(PACKET_CONST_HEADER) + sizeof(PACKET01_HEADER);
			m_file->Seek(m_Packet01Header.wdTailFrameSize, CFile::current);
			dwPacketInPos += m_Packet01Header.wdTailFrameSize;
			do{
				m_file->Read(&m_FrameHeader, sizeof(FRAME_HEADER));
				dwPacketInPos += sizeof(FRAME_HEADER);
				if(m_FrameHeader.byFrameType == 0x81)
				{
					m_dwSeekPos = m_file->GetPosition() - sizeof(FRAME_HEADER);
					m_byCurPacketType = PACKET01_TYPE;
					m_dwSeekDTS = m_FrameHeader.dwDTS;
					m_dw1stSeekTime = (m_dwSeekDTS - m_dwFirstDTS) / 1000;
					return true;
				}
				m_file->Seek(m_FrameHeader.wdTailFrameSize, CFile::current);
				dwPacketInPos += m_FrameHeader.wdTailFrameSize;
			}while(dwPacketInPos < PACKET_SIZE);
			break;
		case PACKET08_TYPE:
			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER), CFile::current);
			break;
		case PACKET09_TYPE:
			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER), CFile::current);
			break;
		case PACKET10_TYPE:
			return false;
		case PACKET11_TYPE:
			return false;
		default:
			break;
		}
	}while(1);
	return false;
}
bool C264Spliter::Seek(float fSeekPos, int nPlayedTime)
{
	DWORD	dwTempDTS;
	DWORD	dwPacketInPos;
	int		nPacketCounts;
	DWORD	dwBuf[2];
	DWORD dwPos = (DWORD)(m_file->GetLength() * fSeekPos) + ASF_FILE_HEAD_SIZE;
	nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
	nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE - nPacketCounts) / PACKET_SIZE;
	dwPos = ASF_FILE_HEAD_SIZE + nPacketCounts * PACKET_SIZE;
	m_file->Seek(dwPos, CFile::begin);
	do{
		m_file->Read(&m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		switch(m_PacketConstHeader.byType)
		{
		case PACKET00_TYPE:
			m_file->Read(&m_Packet00Header, sizeof(PACKET00_HEADER));
			if(m_Packet00Header.byFrameType == 0x81)
			{
				m_file->Read(dwBuf, 8);
				if(dwBuf[0] == 0x01000000 && dwBuf[1] == 0x1EE04267)
				{
					m_dwSeekPos = m_file->GetPosition() - sizeof(PACKET00_HEADER) - sizeof(PACKET_CONST_HEADER) - 8;
					m_file->Seek(m_dwSeekPos, CFile::begin);
					m_byCurPacketType = PACKET00_TYPE;
					ReadPacketHeader();
					m_dwSeekDTS = m_FrameHeader.dwDTS;
					m_dw2ndSeekTime = (m_dwSeekDTS - m_dwFirstDTS) / 1000;
					return true;
				}
				m_file->Seek(-8, CFile::current);
			}
			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER) - sizeof(PACKET00_HEADER), CFile::current);
			break;
		case PACKET01_TYPE:
			m_file->Read(&m_Packet01Header, sizeof(PACKET01_HEADER));
			dwPacketInPos = sizeof(PACKET_CONST_HEADER) + sizeof(PACKET01_HEADER);
			m_file->Seek(m_Packet01Header.wdTailFrameSize, CFile::current);
			dwPacketInPos += m_Packet01Header.wdTailFrameSize;
			do{
				dwTempDTS = m_file->GetPosition();
				m_file->Read(&m_FrameHeader, sizeof(FRAME_HEADER));
				dwPacketInPos += sizeof(FRAME_HEADER);
				if(m_FrameHeader.byFrameType == 0x81)
				{
					m_dwSeekPos = m_file->GetPosition() - sizeof(FRAME_HEADER);
					m_byCurPacketType = PACKET01_TYPE;
					m_dwSeekDTS = m_FrameHeader.dwDTS;
					m_dw2ndSeekTime = (m_dwSeekDTS - m_dwFirstDTS) / 1000;
					return true;
				}
				m_file->Seek(m_FrameHeader.wdTailFrameSize, CFile::current);
				dwPacketInPos += m_FrameHeader.wdTailFrameSize;
			}while(dwPacketInPos < PACKET_SIZE);
			break;
		case PACKET08_TYPE:
			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER), CFile::current);
			break;
		case PACKET09_TYPE:
			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER), CFile::current);
			break;
		case PACKET10_TYPE:
			return false;
		case PACKET11_TYPE:
			return false;
		default:
			break;
		}
	}while(1);
	return false;
}
DWORD C264Spliter::GetLastDTS()
{
	int n = 0;
	PACKET_CONST_HEADER pch;
	PACKET00_HEADER p00h;
	PACKET01_HEADER p01h;
	PACKET08_HEADER p08h;
	PACKET09_HEADER p09h;
	PACKET10_HEADER p10h;
	PACKET11_HEADER p11h;
	m_file->Seek(ASF_FILE_HEAD_SIZE, CFile::begin);
	m_file->Read(&pch, sizeof(PACKET_CONST_HEADER));
	m_file->Read(&p00h, sizeof(PACKET00_HEADER));
	DWORD dwFirstDTS =  p00h.dwDTS;

	DWORD m_dwLastDTS;
	int PacketCounts = (int)(m_file->GetLength() - ASF_FILE_HEAD_SIZE) / PACKET_SIZE;
	do{
		m_file->Seek((PacketCounts - n) * PACKET_SIZE + ASF_FILE_HEAD_SIZE, CFile::begin);
		m_file->Read(&pch, sizeof(PACKET_CONST_HEADER));
		if(pch.byHead == 0x82 && pch.wdConst == 0x0000 && pch.byConst == 0x5d)
		{
			switch(pch.byType)
			{
			case PACKET10_TYPE:
				m_file->Read(&p10h, sizeof(PACKET10_HEADER));
				m_dwLastDTS = p10h.dwDTS;
				return m_dwLastDTS - dwFirstDTS;
			case PACKET11_TYPE:
				m_file->Read(&p11h, sizeof(PACKET11_HEADER));
				m_dwLastDTS = p11h.dwDTS;
				return m_dwLastDTS - dwFirstDTS;
			case PACKET00_TYPE:
				m_file->Read(&p00h, sizeof(PACKET00_HEADER));
				m_dwLastDTS = p00h.dwDTS;
				if(m_dwLastDTS != 0 && m_dwLastDTS > dwFirstDTS) return m_dwLastDTS - dwFirstDTS;
				break;
			case PACKET01_TYPE:
				m_file->Read(&p01h, sizeof(PACKET01_HEADER));
				m_dwLastDTS = p01h.dwDTS;
				if(m_dwLastDTS != 0 && m_dwLastDTS > dwFirstDTS) return m_dwLastDTS - dwFirstDTS;
				break;
			case PACKET08_TYPE:
				m_file->Read(&p08h, sizeof(PACKET08_HEADER));
				m_dwLastDTS = p08h.dwDTS;
				if(m_dwLastDTS != 0 && m_dwLastDTS > dwFirstDTS) return m_dwLastDTS - dwFirstDTS;
				break;
			case PACKET09_TYPE:
				m_file->Read(&p09h, sizeof(PACKET09_HEADER));
				m_dwLastDTS = p09h.dwDTS;
				if(m_dwLastDTS != 0 && m_dwLastDTS > dwFirstDTS) return m_dwLastDTS - dwFirstDTS;
				break;
			default:
				break;
			}
		}
		n++;
	}while(1);
// 	m_file->SeekToEnd();
// 	m_file->Seek(-(0x80 + PACKET_SIZE), CFile::current);
// 	m_file->Read(&m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
// 	switch(m_PacketConstHeader.byType)
// 	{
// 	case PACKET00_TYPE:
// 		m_file->Read(&m_Packet00Header, sizeof(PACKET00_HEADER));
// 		m_dwLastDTS = m_Packet00Header.dwDTS;
// 		break;
// 	case PACKET01_TYPE:
// 		m_file->Read(&m_Packet01Header, sizeof(PACKET01_HEADER));
// 		m_dwLastDTS = m_Packet01Header.dwDTS;
// 		break;
// 	case PACKET08_TYPE:
// 		m_file->Read(&m_Packet08Header, sizeof(PACKET08_HEADER));
// 		m_dwLastDTS = m_Packet08Header.dwDTS;
// 		break;
// 	case PACKET09_TYPE:
// 		m_file->Read(&m_Packet09Header, sizeof(PACKET09_HEADER));
// 		m_dwLastDTS = m_Packet09Header.dwDTS;
// 		break;
// 	case PACKET10_TYPE:
// 		m_file->Read(&m_Packet10Header, sizeof(PACKET10_HEADER));
// 		m_dwLastDTS = m_Packet10Header.dwDTS;
// 		break;
// 	case PACKET11_TYPE:
// 		m_file->Read(&m_Packet11Header, sizeof(PACKET11_HEADER));
// 		m_dwLastDTS = m_Packet11Header.dwDTS;
// 		break;
// 	}
// 	return m_dwLastDTS;
}

DWORD C264Spliter::GetFirstDTS()
{
	m_file->Seek(ASF_FILE_HEAD_SIZE, CFile::begin);
	m_file->Read(&m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
	m_file->Read(&m_Packet00Header, sizeof(PACKET00_HEADER));
	m_dwFirstDTS =  m_Packet00Header.dwDTS;
	return m_dwFirstDTS;
}
bool C264Spliter::ReSeek(void)
{
	DWORD	dwPacketInPos;
	DWORD	dwBuf[2];
	int		nPacketCounts;
	DWORD dwPos = (DWORD)m_file->GetPosition();
	nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE) % PACKET_SIZE;
	nPacketCounts = (dwPos - ASF_FILE_HEAD_SIZE - nPacketCounts) / PACKET_SIZE;
	dwPos = ASF_FILE_HEAD_SIZE + nPacketCounts * PACKET_SIZE;
	m_file->Seek(dwPos, CFile::begin);
	do{
		m_file->Read(&m_PacketConstHeader, sizeof(PACKET_CONST_HEADER));
		switch(m_PacketConstHeader.byType)
		{
		case PACKET00_TYPE:
			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER), CFile::current);
			break;
// 			m_file->Read(&m_Packet00Header, sizeof(PACKET00_HEADER));
// 			if(m_Packet00Header.byFrameType == 0x81)
// 			{
// 				m_file->Read(dwBuf, 8);
// 				if(dwBuf[0] == 0x01000000 && dwBuf[1] == 0x1EE04267)
// 				{
// 					m_dwSeekPos = m_file->GetPosition() - sizeof(PACKET00_HEADER) - sizeof(PACKET_CONST_HEADER) - 8;
// 					m_file->Seek(m_dwSeekPos, CFile::begin);
// 					m_byCurPacketType = PACKET00_TYPE;
// 					ReadPacketHeader();
// 					m_dwSeekDTS = m_FrameHeader.dwDTS;
// 					m_dw2ndSeekTime = (m_dwSeekDTS - m_dwFirstDTS) / 1000;
// 					if(m_dw2ndSeekTime != m_dw1stSeekTime) return false;
// 					return true;
// 				}
// 				m_file->Seek(-8, CFile::current);
// 			}
// 			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER) - sizeof(PACKET00_HEADER), CFile::current);
			break;
		case PACKET01_TYPE:
			m_file->Read(&m_Packet01Header, sizeof(PACKET01_HEADER));
			dwPacketInPos = sizeof(PACKET_CONST_HEADER) + sizeof(PACKET01_HEADER);
			m_file->Seek(m_Packet01Header.wdTailFrameSize, CFile::current);
			dwPacketInPos += m_Packet01Header.wdTailFrameSize;
			do{
				m_file->Read(&m_FrameHeader, sizeof(FRAME_HEADER));
				dwPacketInPos += sizeof(FRAME_HEADER);
				if(m_FrameHeader.byFrameType == 0x81)
				{
					m_dwSeekPos = m_file->GetPosition() - sizeof(FRAME_HEADER);
					m_byCurPacketType = PACKET01_TYPE;
					m_dwSeekDTS = m_FrameHeader.dwDTS;//m_FrameHeader.dwDTS;
					m_dw2ndSeekTime = (m_dwSeekDTS - m_dwFirstDTS) / 1000 + 1;
					if((m_dw2ndSeekTime / 3) != (m_dw1stSeekTime / 3)) return false;
					return true;
				}
				m_file->Seek(m_FrameHeader.wdTailFrameSize, CFile::current);
				dwPacketInPos += m_FrameHeader.wdTailFrameSize;
			}while(dwPacketInPos < PACKET_SIZE);
			break;
		case PACKET08_TYPE:
			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER), CFile::current);
			break;
		case PACKET09_TYPE:
			m_file->Seek(PACKET_SIZE - sizeof(PACKET_CONST_HEADER), CFile::current);
			break;
		case PACKET10_TYPE:
			return false;
		case PACKET11_TYPE:
			return false;
		default:
			break;
		}
	}while(1);
	return false;
}
