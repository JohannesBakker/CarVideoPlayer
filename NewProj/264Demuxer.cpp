// 264Demuxer.cpp: implementation of the C264Demuxer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewProj.h"
#include "264Demuxer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//BYTE C264Demuxer::m_byFrameData[FRAME_MAX_SIZE];
//BYTE C264Demuxer::m_buf[PACKET_BODY_SIZE];
C264Demuxer::C264Demuxer()
{

}

C264Demuxer::~C264Demuxer()
{

}


void C264Demuxer::Init_Hi264()
{
	HINSTANCE hAudioInt = LoadLibrary(_T("lib_VoiceEngine_dll.dll"));
	HI_VOICE_DecodeFrame = (FuncHI_VOICE_DecodeFrame)GetProcAddress(hAudioInt, "HI_VOICE_DecodeFrame");
	HI_VOICE_DecReset = (FuncHI_VOICE_DecReset)GetProcAddress(hAudioInt, "HI_VOICE_DecReset");

	HINSTANCE hVideoInst = LoadLibrary(_T("hi_h264dec_w.dll"));
	Hi264DecCreate = (FuncHi264DecCreate)GetProcAddress(hVideoInst, "Hi264DecCreate");
	Hi264DecGetInfo = (FuncHi264DecGetInfo)GetProcAddress(hVideoInst, "Hi264DecGetInfo");
	Hi264DecFrame = (FuncHi264DecFrame)GetProcAddress(hVideoInst, "Hi264DecFrame");
	Hi264DecDestroy = (FuncHi264DecDestroy)GetProcAddress(hVideoInst, "Hi264DecDestroy");
}
