// GPSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "GPSDlg.h"
#include "HTMLWriter.h"
#include <windef.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_INTERVAL	1500
#define DEFAULT_IMAGE_NAME	_T("1.png")
//#define DEFAULT_IMAGE_NAME	_T("")

#define OFFSET_LAT	-0.0003
#define OFFSET_LNG	0.0000

#define TIMER_GPS_MAP	30000

#define DEFAULT_MAP_BROWSER_ID	0

#define IGNORE_GPS_INFO_NUM		2


typedef struct {
	int degree;
	CString imageName;
} AngleImage;

/////////////////////////////////////////////////////////////////////////////
// CGPSDlg dialog
CGPSDlg::CGPSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGPSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGPSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bGPSDlg = false;

	m_bInitComplete = false;

	m_nSpeedUnit = SPEED_UNIT_MPH;

	GetSystemTime(&m_VideoDateTime);

	m_bTimerRunning = false;
}



void CGPSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGPSDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EXPLORER1, m_vwMapBrowser);
	
	
}


BEGIN_MESSAGE_MAP(CGPSDlg, CDialog)
	//{{AFX_MSG_MAP(CGPSDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPSDlg message handlers

void CGPSDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_bInitComplete == true)
		GetDlgItem(IDC_EXPLORER1)->MoveWindow(0, 0, cx, cx);
}



BOOL CGPSDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	// init map browser
	m_vwMapBrowser.Navigate(_T("about:blank"), NULL, NULL, NULL, NULL);
	m_vwMapBrowser.ShowScrollBar(SB_VERT, false);
	m_vwMapBrowser.ShowScrollBar(SB_HORZ, false);

	// Reset Gps information variables
	ResetGpsMapInfo(&m_stInitGpsInfo);
	ResetGpsMapInfo(&m_stPrevGpsInfo);
	ResetGpsMapInfo(&m_stCurrGpsInfo);

	m_bInitComplete = true;

	// setting default GpsWindow data 
//	LoadBrowser(TYPE_DEFAULT_BROWSER);
	LoadBrowser();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CGPSDlg::DisplayGpsMap(MainBinaryData * pMBDatas, DWORD dwFirstDTS, DWORD dwCurrDTS, unsigned char alarmFlag)
{
	WCHAR buf[40];
 	double dbl_Ntemp, dbl_Wtemp;
 	double dbl_NResult, dbl_WResult;
 	
	if (pMBDatas->dwGPSW_Down < 0)
		pMBDatas->dwGPSW_Down + 0x100000000;
 	dbl_Wtemp = (DWORD)((double)pMBDatas->dwGPSW_Down / 3600.0 + (double)pMBDatas->byGPSW_Lw_Up / 60.0 * 10000000.0);
 	if ( pMBDatas->dwGPSN_Down < 0 )
		pMBDatas->dwGPSN_Down = pMBDatas->dwGPSN_Down + 0x100000000;
 	dbl_Ntemp = ((double)pMBDatas->dwGPSN_Down / 3600.0 + (double)pMBDatas->byGPSN_Lw_Up / 60.0 * 10000000.0);
 	dbl_WResult = (dbl_Wtemp * 0.0000001 + (double)pMBDatas->byGPSW_Hi_Up);
	if (pMBDatas->byEW_Flag != 0x45)
		dbl_WResult = -1 * dbl_WResult;
 	dbl_NResult = (dbl_Ntemp * 0.0000001 + (double)pMBDatas->byGPSN_Hi_Up);
	if (pMBDatas->byNS_Flag != 0x57)
		dbl_NResult = -1 * dbl_NResult;
	
	// save current GPS info to prev GPS info
	SetGpsMapInfo(&m_stPrevGpsInfo, &m_stCurrGpsInfo);

	// set current GPS information
	m_stCurrGpsInfo.stGpsPos.fLat = dbl_NResult;
	m_stCurrGpsInfo.stGpsPos.fLon = dbl_WResult;
	m_stCurrGpsInfo.stGpsPos.fSpeed = (float)pMBDatas->speed1 / 100.0f;
	m_stCurrGpsInfo.dwOffsetSecs = (dwCurrDTS - dwFirstDTS) / 1000;
	m_stCurrGpsInfo.ubAlarmState = alarmFlag;

	m_stCurrGpsInfo.strCarImageName = m_stPrevGpsInfo.strCarImageName;

	if (memcmp(&m_stCurrGpsInfo.stGpsPos, &m_stPrevGpsInfo.stGpsPos, sizeof(GpsLocation_t)))
	{
		GpsLocation_t *pPrevGps = &m_stPrevGpsInfo.stGpsPos;
		GpsLocation_t *pCurrGps = &m_stCurrGpsInfo.stGpsPos;
		
		if (pPrevGps->fLat == 0 && pPrevGps->fLon == 0)
		{
		}
		else
		{
			static int nCount = 0;
			nCount ++;

			if (nCount <= IGNORE_GPS_INFO_NUM)
				return;


			int deg = GetBearingLocation(pPrevGps->fLat, pPrevGps->fLon, pCurrGps->fLat, pCurrGps->fLon);
			CString szImageName = GetCarImageName(deg);

			if (!szImageName.IsEmpty())
				m_stCurrGpsInfo.strCarImageName = szImageName;

			if (m_stCurrGpsInfo.strCarImageName.IsEmpty())
				m_stCurrGpsInfo.strCarImageName = DEFAULT_IMAGE_NAME;

			//LoadBrowser(TYPE_MAP_BROWSER);
			//RunMapCommand(MAP_CMD_INIT_MAP, &m_stCurrGpsInfo);
		}

		// set GPS init information
		if (m_stInitGpsInfo.stGpsPos.fLat == 0 && m_stInitGpsInfo.stGpsPos.fLon == 0)
		{
			SetGpsMapInfo(&m_stInitGpsInfo, &m_stCurrGpsInfo);
		}
	}

	if (m_bTimerRunning == false)
	{
		// Start Map browser timer
		TimerStart();
	}
		
}

void CGPSDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_GPS_MAP)
	{	
		RunMapCommand(MAP_CMD_SET_MARKER, &m_stCurrGpsInfo);		
	}

	
	CDialog::OnTimer(nIDEvent);
}


int CGPSDlg::GetBearingLocation(float pos1Lat, float pos1Lng, float pos2Lat, float pos2Lng)
{
	float dLon = pos2Lng - pos1Lng;
	float y = sin(dLon) * cos(pos2Lat);
	float x = cos(pos1Lat) * sin(pos2Lat) - sin(pos1Lat)*cos(pos2Lat)*cos(dLon);
	float brng = atan2(y, x);

	brng = brng * 180 / 3.141592;

	int deg = (int)brng;
	return deg;
}

CString CGPSDlg::GetCarImageName(int degree)
{
	static AngleImage arrAngleImages[] = {
		{0,		_T("1.png")},
		{-45,	_T("2.png")},
		{-90,	_T("3.png")},
		{-135,	_T("4.png")},
		{-179,	_T("5.png")},
		{179,	_T("5.png")},
		{135,	_T("6.png")},
		{90,	_T("7.png")},
		{45,	_T("8.png")},
	};

	int i;
	int selId = 0;

	if (degree == -180 || degree == 180 || degree == 0) {
		return _T("");
	}

	for (i = 1; i < sizeof(arrAngleImages)/sizeof(AngleImage); i++)
	{
		int d1 = abs(arrAngleImages[i].degree - degree);
		int d2 = abs(arrAngleImages[selId].degree - degree);

		if (d1 < d2)
			selId = i;
	}

	CString imageName = arrAngleImages[selId].imageName;
	return imageName;
}


void CGPSDlg::SetSpeedUnit(int nSpeedUnit)
{
	m_nSpeedUnit = (SpeedUnit_t)nSpeedUnit;
}

void CGPSDlg::SetVideoDateTime(SYSTEMTIME *pVideoDateTime)
{
	if (pVideoDateTime)
		memcpy(&m_VideoDateTime, pVideoDateTime, sizeof(SYSTEMTIME));
}

void CGPSDlg::ResetGpsMapInfo(GpsMapInfo_t *pInfo)
{
	if (pInfo)
	{
		pInfo->stGpsPos.fLat = 0;
		pInfo->stGpsPos.fLon = 0;
		pInfo->stGpsPos.fSpeed = 0;
		pInfo->dwOffsetSecs = 0;
		pInfo->ubAlarmState = ALARM_STATE_NO_ALARM;
		pInfo->strCarImageName = _T("");
	}
}

void CGPSDlg::SetGpsMapInfo(GpsMapInfo_t *pInfo, float fLat, float fLon, float fSpeed,
			DWORD dwOffsetSecs, unsigned char ubAlarmState, CString strCarImageName)
{
	if (pInfo)
	{
		pInfo->stGpsPos.fLat = fLat;
		pInfo->stGpsPos.fLon = fLon;
		pInfo->stGpsPos.fSpeed = fSpeed;
		pInfo->dwOffsetSecs = dwOffsetSecs;
		pInfo->ubAlarmState = ubAlarmState;
		pInfo->strCarImageName = strCarImageName;
	}
}

void CGPSDlg::SetGpsMapInfo(GpsMapInfo_t *pDstInfo, GpsMapInfo_t *pSrcInfo)
{
	if (pDstInfo && pSrcInfo)
	{
		pDstInfo->stGpsPos.fLat 	= pSrcInfo->stGpsPos.fLat;
		pDstInfo->stGpsPos.fLon 	= pSrcInfo->stGpsPos.fLon;
		pDstInfo->stGpsPos.fSpeed 	= pSrcInfo->stGpsPos.fSpeed;
		
		pDstInfo->dwOffsetSecs 		= pSrcInfo->dwOffsetSecs;
		pDstInfo->ubAlarmState 		= pSrcInfo->ubAlarmState;
		pDstInfo->strCarImageName 	= pSrcInfo->strCarImageName;
	}
}

void CGPSDlg::TimerStart()
{
	if (m_bTimerRunning == false)
		SetTimer(TIMER_GPS_MAP, TIMER_INTERVAL, NULL);
	m_bTimerRunning = true;
}

void CGPSDlg::TimerStop()
{
	KillTimer(TIMER_GPS_MAP);
	m_bTimerRunning = false;
}

void CGPSDlg::ResetGpsDialog(bool bResetMap)
{
	// stop GPS browser timer
	TimerStop();

	if (bResetMap)
	{
		// Reset Gps information variables
		ResetGpsMapInfo(&m_stInitGpsInfo);
		ResetGpsMapInfo(&m_stPrevGpsInfo);
		ResetGpsMapInfo(&m_stCurrGpsInfo);

		m_nCurrMapBrowserId = DEFAULT_MAP_BROWSER_ID;

		m_bTimerRunning = false;

		

//		if (m_currBrowserType != TYPE_DEFAULT_BROWSER)
//			LoadBrowser(TYPE_DEFAULT_BROWSER);

		LoadBrowser();
		
	}
}

CString CGPSDlg::GetBrowserContent()
{
	CString strHtml = L"";

	
	strHtml = L"<!DOCTYPE html><html>";

	// html Head
	strHtml += L"<head>";
	strHtml += L"<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\"><meta charset=\"utf-8\">";
	strHtml += L"<title>Simple icons</title>";
	strHtml += L"<style>html, body {height: 100%;margin: 0px;padding: 0px} #map-canvas {height: 100%;margin: 0px;padding: 0px}</style>";	
	strHtml += L"</head>";

	// html body
	strHtml += L"<body>";
	strHtml += L"<div id=\"map-canvas\"></div>";

	strHtml += L"<script src=\"https://maps.googleapis.com/maps/api/js?sensor=false\"></script>";

	// functions
	strHtml += L"<script type=\"text/javascript\">";
	{
		

		// define global variables
		strHtml += L"var g_map; ";
		strHtml += L"var g_currPositionMarker; ";
		strHtml += L"var g_infowindow; ";


		//=======================
		// initialize function
		//=======================
		{
			// initial location : (40.8833, -85.0167), zoom : 5
			strHtml += L"function initialize() { ";
			
			strHtml += L"var mapOptions = {center: { lat: 40.8833, lng: -85.0167},zoom: 5}; ";
			strHtml += L"var myLatLng = new google.maps.LatLng(40.8833, -85.0167); ";
			
			strHtml += L"g_map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions); ";
			strHtml += L"g_currPositionMarker = new google.maps.Marker({position: myLatLng, map: g_map, icon: \" \"}); ";
			strHtml += L"g_infowindow = new google.maps.InfoWindow( {";
			strHtml += L"content: \"\",";
			strHtml += L"maxWidth: 220";
			strHtml += L"} );";

			// add info window to marker
			strHtml += L"g_currPositionMarker.addListener('click', function() {";
			strHtml += L"g_infowindow.open(g_map, g_currPositionMarker); ";
			strHtml += L"} ); ";

			strHtml += L"} ";

			// register load function with "initialize"
			strHtml += L"google.maps.event.addDomListener(window, 'load', initialize); ";
		}

		//=======================
		// setMarker function
		//=======================
		{
			// function begin
			strHtml += L"function setMarker(strLat, strLon, strImage, strTime, strCurrTime, strSpeed) { ";

			// define variables
			strHtml += L"var _zoom = g_map.getZoom(); ";
			strHtml += L"var image = strImage; ";
			strHtml += L"var myLatLng = new google.maps.LatLng(strLat, strLon); ";
			strHtml += L"var contentString = '<div id=\"content\">'+";
			strHtml += L"'<div id=\"siteNotice\">'+";
			strHtml += L"'</div>'+";
			strHtml += L"'<div id=\"bodyContent\">'+";
			strHtml += L"'<b>TIME</b> :' + strTime + '<br/>' + ";
			strHtml += L"'<b>Current Time</b> :' + strCurrTime + '<br/>' + ";
			strHtml += L"'<b>SPEED</b> :' + strSpeed + '<br/>' + ";
			strHtml += L"'</div>'+";
			strHtml += L"'</div>';";

			// set values of map, marker, infowindow			
			strHtml += L"g_map.setCenter(myLatLng); ";
			strHtml += L"if (_zoom != 16) {";
			strHtml += L"g_map.setZoom(16); } ";
			
			strHtml += L"g_currPositionMarker.setPosition(myLatLng); ";
			strHtml += L"g_currPositionMarker.setIcon(image); ";
			strHtml += L"g_infowindow.setContent(contentString); ";
			strHtml += L"g_infowindow.open(g_map, g_currPositionMarker); ";

			// function end
			strHtml += L"} ";
		}
	}
	strHtml += L"</script>";
	
	strHtml += L"</body></html>";
	
	return strHtml;
}


void CGPSDlg::LoadBrowser()
{
	CString strContent = GetBrowserContent();

	CHTMLWriter htmlWriter(m_vwMapBrowser.GetControlUnknown());
	htmlWriter.Write(strContent);
}



void CGPSDlg::RunMapCommand(MapCommand_t command, GpsMapInfo_t* pGpsMapInfo)
{
	GpsMapInfo_t mapInfo;
	bool bImageUsed = true;

	SetGpsMapInfo(&mapInfo, pGpsMapInfo);
	
	
	TCHAR szPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szPath, _MAX_PATH));

	if (mapInfo.strCarImageName.IsEmpty())
		return;

	if (mapInfo.strCarImageName.IsEmpty())
		bImageUsed = false;
	
	CString strImagePath(szPath);
	int nIndex  = strImagePath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		strImagePath = strImagePath.Left(nIndex);
	}
	else {
		strImagePath.Empty();
	}	

	strImagePath = L"file://" + strImagePath + L"/bmp2/Car/";
	if (mapInfo.ubAlarmState == ALARM_STATE_NO_ALARM)
		strImagePath += L"online/" + mapInfo.strCarImageName;
	else
		strImagePath += L"alarm/" + mapInfo.strCarImageName;

	strImagePath.Replace('\\', '/');

	CString strLat, strLon;
	strLat.Format(L"%f", mapInfo.stGpsPos.fLat + OFFSET_LAT);
	strLon.Format(L"%f", mapInfo.stGpsPos.fLon + OFFSET_LNG);

	CString strTime = L"", strCurrTime = L"", strSpeed = L"";

	int nOffsetDay, nOffsetHours, nOffsetMins, nOffsetSecs;
	DWORD dwSecs = mapInfo.dwOffsetSecs;

	nOffsetDay = dwSecs / (3600 * 24);
	dwSecs = dwSecs - (nOffsetDay * 3600 * 24);

	nOffsetHours = dwSecs / 3600;
	dwSecs = dwSecs - (nOffsetHours * 3600);

	nOffsetMins = dwSecs / 60;
	dwSecs = dwSecs - (nOffsetMins * 60);

	nOffsetSecs = dwSecs % 60;

	if (nOffsetHours > 0)
		strTime.Format(L"%02d:%02d:%02d", nOffsetHours, nOffsetMins, nOffsetSecs);
	else
		strTime.Format(L"%02d:%02d", nOffsetMins, nOffsetSecs);

	CTime currTime(m_VideoDateTime, 0);
	currTime -= CTimeSpan(0, 0, 0, 30);
	currTime += CTimeSpan(nOffsetDay, nOffsetHours, nOffsetMins, nOffsetSecs);

	strCurrTime.Format(L"%04d-%02d-%02d %02d:%02d:%02d", 
		currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(), 
		currTime.GetHour(), currTime.GetMinute(), currTime.GetSecond());

	float fSpeed = mapInfo.stGpsPos.fSpeed;
	if (m_nSpeedUnit == SPEED_UNIT_KMH)
		fSpeed = fSpeed * 1.609344;

	if (fSpeed < 1)
	{
		fSpeed = 0;
		if (m_nSpeedUnit == SPEED_UNIT_KMH)
			strSpeed = L"0 km/h";
		else
			strSpeed = L"0 mph";
	}
	else
	{
		strSpeed.Format(L"%.1f ", fSpeed);

		if (m_nSpeedUnit == SPEED_UNIT_KMH)
			strSpeed += L"km/h";
		else
			strSpeed += L"mph";
	}


	CString strHtml = L"";

	switch (command)
	{
		case MAP_CMD_INIT_MAP:
			strHtml = L"Javascript:initMaps(\"" + strLat + L"\", \"" + strLon + L"\", \"" + strImagePath + L"\", ";
			strHtml += "\"" + strTime + L"\", \"" + strCurrTime + L"\", \"" + strSpeed + L"\"";
			strHtml += L"); ";
			break;

		case MAP_CMD_SET_MARKER:
			strHtml = L"Javascript:setMarker(\"" + strLat + L"\", \"" + strLon + L"\", \"" + strImagePath + L"\", ";
			strHtml += "\"" + strTime + L"\", \"" + strCurrTime + L"\", \"" + strSpeed + L"\"";
			strHtml += L"); ";
			break;

		default:
			return;
	}

	CHTMLWriter htmlWriter(m_vwMapBrowser.GetControlUnknown());
	htmlWriter.RunScript(strHtml);
}


