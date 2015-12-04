#if !defined(AFX_GPSDLG_H__07C392D0_A178_4920_B7F2_B0AB3CEC137E__INCLUDED_)
#define AFX_GPSDLG_H__07C392D0_A178_4920_B7F2_B0AB3CEC137E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GPSDlg.h : header file
//"
#include "CxStatic.h"
#include "EditTrans.h"
#include "264Spliter.h"
#include "explorer1.h"
#include "type.h"
/////////////////////////////////////////////////////////////////////////////
// CGPSDlg dialog
//typedef DWORD		(_cdecl* FuncMAP_FixGps)(double, double, int, int);


typedef enum MapCommand {
	MAP_CMD_SET_MARKER = 0,

	MAP_CMD_MAX,
} MapCommand_t;

typedef struct GpsLocation {
	float fLat;
	float fLon;
	float fSpeed;		// Unit : MPH
} GpsLocation_t;

typedef struct GpsMapInfo {
	GpsLocation_t	stGpsPos;			// GPS location information
	
	DWORD			dwOffsetSecs;		// Seconds from video played
	unsigned char	ubAlarmState;		// alarm state, bit[0] : Alarm-1, bit[1]: alarm-2, bit[2]:alarm-3,  other bits : reserved
	CString			strCarImageName;	// car image Name : 1.png ~  8.png
} GpsMapInfo_t;

// Alarm state value
#define		ALARM_STATE_NO_ALARM	0


class CGPSDlg : public CDialog
{
// Construction
public:
	void	DisplayGpsMap(MainBinaryData*, DWORD, DWORD, unsigned char);
	CGPSDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGPSDlg)
	enum { IDD = IDD_GPS_DIALOG };
	//FuncMAP_FixGps		m_MAP_FixGps;
	bool		m_bGPSDlg;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGPSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGPSDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	bool m_bInitComplete;


	SpeedUnit_t	m_nSpeedUnit;
	SYSTEMTIME m_VideoDateTime;

	CExplorer1		m_vwMapBrowser;
	GpsMapInfo_t	m_stInitGpsInfo;	// init GPS information
	GpsMapInfo_t	m_stPrevGpsInfo;	// previous GPS information
	GpsMapInfo_t	m_stCurrGpsInfo;	// current GPS information
	
	bool			m_bTimerRunning;	// true : timer running

	CString 		m_szImageName;		// 
	int				m_nIgnoreCounter;
	

	afx_msg void OnTimer(UINT_PTR nIDEvent); 


	int GetBearingLocation(float pos1Lat, float pos1Lng, float pos2Lat, float pos2Lng);
	CString GetCarImageName(int degree);
	
	void SetSpeedUnit(int nSpeedUnit);
	void SetVideoDateTime(SYSTEMTIME *pVideoDateTime);

	void ResetGpsMapInfo(GpsMapInfo_t *pInfo);
	void SetGpsMapInfo(GpsMapInfo_t *pInfo, 
			float fLat, float fLon, float fSpeed,
			DWORD dwOffsetSecs, unsigned char ubAlarmState, CString strCarImageName);	
	void SetGpsMapInfo(GpsMapInfo_t *pDstInfo, GpsMapInfo_t *pSrcInfo);

	void TimerStart();
	void TimerStop();

	void ResetGpsDialog(bool bResetMap);

	void LoadBrowser();
	void RunMapCommand(MapCommand_t command, GpsMapInfo_t* pGpsMapInfo);

	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPSDLG_H__07C392D0_A178_4920_B7F2_B0AB3CEC137E__INCLUDED_)
