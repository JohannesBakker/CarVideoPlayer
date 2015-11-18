#ifndef __TYPE_H_
#define __TYPE_H_

typedef enum {
	SPEED_UNIT_KM_H		= 0,
	SPEED_UNIT_MPH,
	SPEED_UNIT_NUM,
} SpeedUnit_t;


struct Config_Datas
{
	bool	bDateTimeWnd;
	int		nLanguageID;
	int		nWndCounts;
	int		nSelWndID;
	int		nViewScaleID;
	int		nSpeedUnitID;
	bool	bGPSOffsetCorre;
	int		nMapTypeID;
	bool	bShowGPSTrack;
	bool	bUseRGB16;
	bool	bAutoPlay;
	bool	bShowChanNum;

	bool	bWtrmkVerify;
	wchar_t	chWtrmk[20];
	int		nWtrmk_Hori;
	int		nWtrmk_Vert;

	bool	bIOAlarm;
	int		nIOAlarm_Hori;
	int		nIOAlarm_Vert;
	int		nIOAlarmCmb;

	bool	bACCWinker;
	int		nACCWinker_Hori;
	int		nACCWinker_Vert;
	int		nACCWinkerCmb;

	bool	bDriverComp;
	int		nDriverComp_Hori;
	int		nDriverComp_Vert;
	int		nDriverCompCmb;

	bool	bDevCamera;
	int		nDevCamera_Hori;
	int		nDevCamera_Vert;
	int		nDevCameraCmb;

	bool	bAVIFile;
	bool	b264File;
	wchar_t	chLastPath1[255];
	wchar_t	chLastPath2[255];
	wchar_t	chLastPath3[255];
	wchar_t	chLastPath4[255];
	wchar_t	lastpath[260];
};
struct ADJUST_CONFIG
{
	BYTE	nHue;
	BYTE	nSaturation;
	int		nLight;
	int		nContract;
	bool	bVideoRealtimely;
	bool	bTranscoding1;
	bool	bTranscoding2;
};
struct IO_LIGHT_CONFIG
{
	int		nIO1_Signal;
	int		nIO2_Signal;
	int		nIO3_Signal;
};
struct AVI_CONFIG
{
	bool	bChan1;
	bool	bChan2;
	int		nAVIConvSelAudio;
	bool	bPictureFlip;
	int		nAVIConvScaleMode;
};

#endif





















