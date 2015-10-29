// 264Demuxer.h: interface for the C264Demuxer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_264DEMUXER_H__824C5E12_A0D6_40B3_A847_097F5CB5DBFE__INCLUDED_)
#define AFX_264DEMUXER_H__824C5E12_A0D6_40B3_A847_097F5CB5DBFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define PACKET_BODY_SIZE 3195
#define PACKET_SIZE 3200
#define FRAME_MAX_SIZE 0x20000

struct AudioHeader{
	DWORD dwVal1;
	DWORD dwVal2;
	DWORD dwVal3;
	DWORD dwVal4;
	DWORD dwVal5;
	DWORD dwVal6;
	DWORD dwVal7;
	DWORD dwVal8;
	DWORD dwVal9;
	DWORD dwVal10;
	DWORD dwVal11;
	DWORD dwVal12;
	DWORD dwVal13;
	DWORD dwVal14;
	DWORD dwVal15;
	DWORD dwVal16;
	DWORD dwVal17;
	DWORD dwVal18;
	DWORD dwVal19;
	DWORD dwVal20;
	DWORD dwVal21;
	DWORD dwCal22;
	DWORD dwVal23;
	DWORD dwVal24;
	DWORD dwVal25;
	DWORD dwVal26;
	DWORD dwVal27;
// 	DWORD dwVal28;
// 	DWORD dwVal29;
// 	DWORD dwVal30;
};
struct  HiHandle{
	DWORD dwData1;
	DWORD dwData2;
	DWORD dwData3;
	DWORD dwData4;
	DWORD dwData5;
	DWORD dwData6;
	DWORD dwData7;
	DWORD dwData8;
	DWORD dwData9;
	DWORD dwData10;
	DWORD dwData11;
	DWORD dwData12;
	DWORD dwData13;
	unsigned char* chY[12];
	DWORD dwY1;
	DWORD dwY2;
	DWORD dwY3;
	DWORD dwY4;
	DWORD dwY5;
	DWORD dwY6;
	DWORD dwY7;
	DWORD dwY8;
	DWORD dwY9;
	DWORD dwY10;
	DWORD dwY11;
	DWORD dwY12;
	DWORD dwY13;
	DWORD dwY14;
	DWORD dwY15;
	DWORD dwY16;
	DWORD dwY17;
	DWORD dwY18;
	DWORD dwY19;
	DWORD dwY20;
	DWORD dwY21;
	DWORD dwY22;
	DWORD dwY23;
	DWORD dwY24;
	unsigned char* chU[12];
	DWORD dwU1;
	DWORD dwU2;
	DWORD dwU3;
	DWORD dwU4;
	DWORD dwU5;
	DWORD dwU6;
	DWORD dwU7;
	DWORD dwU8;
	DWORD dwU9;
	DWORD dwU10;
	DWORD dwU11;
	DWORD dwU12;
	DWORD dwU13;
	DWORD dwU14;
	DWORD dwU15;
	DWORD dwU16;
	DWORD dwU17;
	DWORD dwU18;
	DWORD dwU19;
	DWORD dwU20;
	DWORD dwU21;
	DWORD dwU22;
	DWORD dwU23;
	DWORD dwU24;
	unsigned char* chV[12];
	DWORD dwV1;
	DWORD dwV2;
	DWORD dwV3;
	DWORD dwV4;
	DWORD dwV5;
	DWORD dwV6;
	DWORD dwV7;
	DWORD dwV8;
	DWORD dwV9;
	DWORD dwV10;
	DWORD dwV11;
	DWORD dwV12;
	DWORD dwV13;
	DWORD dwV14;
	DWORD dwV15;
	DWORD dwV16;
	DWORD dwV17;
	DWORD dwV18;
	DWORD dwV19;
	DWORD dwV20;
	DWORD dwV21;
	DWORD dwV22;
	DWORD dwV23;
	DWORD dwV24;
};
struct DecInfo
{
	DWORD dw0;
	DWORD dw4;
	DWORD dw8;
	DWORD dwC;
	DWORD dw10;
	DWORD dw14;
	DWORD dw18;
	DWORD dw1C;
	DWORD dw20;
	DWORD dw24;
	DWORD dw28;
	DWORD dw2C;
	DWORD dw30;
};
struct OUT_FRAME
{
	unsigned char* ptr_uChY;
	unsigned char* ptr_uChU;
	unsigned char* ptr_uChV;
	DWORD		dwWidth;
	DWORD		dwHeigth;
	DWORD		dwStride_y;
	DWORD		dwStride_uv;
	DWORD		dwvar8;
	DWORD		dwvar9;
	DWORD		dwvar10;
	DWORD		dwVar11;
	DWORD		dwVar12;
	DWORD		dwVar13;
	DWORD		dwVar14;
	DWORD		dwVar15;
	DWORD		dwVar16;
	DWORD		dwVar17;
	DWORD		dwVar18;
	DWORD		dwVar19;
};
#define TYPE_VIDEO	0x01
#define TYPE_AUDIO	0x82
#define TYPE_BINARY 0x03


typedef DWORD		(_cdecl* FuncHI_VOICE_DecodeFrame)(AudioHeader*, unsigned char*, char*, DWORD*);
typedef DWORD		(_cdecl* FuncHI_VOICE_DecReset)(AudioHeader*, DWORD);

typedef HiHandle*	(_cdecl* FuncHi264DecCreate)(DecInfo*);
typedef DWORD		(_cdecl* FuncHi264DecGetInfo)(DecInfo*);
typedef DWORD		(_cdecl* FuncHi264DecFrame)(void*, unsigned char*, int, DWORD, DWORD, OUT_FRAME*, int);
typedef void		(_cdecl* FuncHi264DecDestroy)(HiHandle*);

//typedef DWORD		(_cdecl* FuncMAP_FixGps)(double, double, int, int);

class C264Demuxer  
{
public:
	C264Demuxer();
	virtual ~C264Demuxer();
	void Init_Hi264();
public:
	DecInfo						m_DecInfo;
//	static BYTE					m_byFrameData[FRAME_MAX_SIZE];
//	static BYTE					m_buf[PACKET_BODY_SIZE];
	FuncHI_VOICE_DecodeFrame	HI_VOICE_DecodeFrame;
	FuncHI_VOICE_DecReset		HI_VOICE_DecReset;

	FuncHi264DecCreate			Hi264DecCreate;
	FuncHi264DecGetInfo			Hi264DecGetInfo;
	FuncHi264DecFrame			Hi264DecFrame;
	FuncHi264DecDestroy			Hi264DecDestroy;

	//FuncMAP_FixGps		m_MAP_FixGps;
};

#endif // !defined(AFX_264DEMUXER_H__824C5E12_A0D6_40B3_A847_097F5CB5DBFE__INCLUDED_)
