// 264Decoder.h: interface for the C264Decoder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_264DECODER_H__292D8330_98B4_444A_AA7F_2AD4FFBCA7E5__INCLUDED_)
#define AFX_264DECODER_H__292D8330_98B4_444A_AA7F_2AD4FFBCA7E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define VIDEOW 704
#define VIDEOH 480
#define SUB_AUDIO_FRAME_SIZE 0x68
#define MAX_BUF_LEN 1024 * 64
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#define BITMAPINFO_INIT\
	BITMAPINFO bmi;\
	bmi.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);\
	bmi.bmiHeader.biCompression     = BI_RGB;\
	bmi.bmiHeader.biWidth           = VIDEOW;\
	bmi.bmiHeader.biHeight          = VIDEOH;\
	bmi.bmiHeader.biPlanes          = 1;\
	bmi.bmiHeader.biBitCount        = 24;\
	bmi.bmiHeader.biSizeImage       = VIDEOW*VIDEOH*bmi.bmiHeader.biBitCount/8;\
	bmi.bmiHeader.biXPelsPerMeter   = 0;\
	bmi.bmiHeader.biYPelsPerMeter   = 0;\
	bmi.bmiHeader.biClrUsed         = 0;\
bmi.bmiHeader.biClrImportant    = 0;

#define GRAPHSETTING\
	pDisPos->GetWindowRect(&rect);\
	HDC	hDC, hMemDC;\
	HBITMAP hBmp, hOldBmp;\
	hDC = pDisPos->GetDC()->GetSafeHdc();\
	hMemDC = CreateCompatibleDC(hDC);\
	hBmp = CreateCompatibleBitmap(hDC, VIDEOW, VIDEOH);\
	hOldBmp = (HBITMAP)SelectObject(hMemDC,  (HGDIOBJ)hBmp);
class C264Decoder  
{
public:
	C264Decoder();
	virtual ~C264Decoder();

};

#endif // !defined(AFX_264DECODER_H__292D8330_98B4_444A_AA7F_2AD4FFBCA7E5__INCLUDED_)
