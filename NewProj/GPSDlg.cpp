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
//#define DEFAULT_IMAGE_NAME	_T("car-1.png")
#define DEFAULT_IMAGE_NAME	_T("")

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
	m_bGPSUpdated = false;

	m_bInitComplete = false;
	m_nWindowWidth = 0;
	m_nWindowHeight = 0;

	m_nSpeedUnit = SPEED_UNIT_MPH;

	GetSystemTime(&m_VideoDateTime);

	m_bTimerRunning = false;
}



void CGPSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGPSDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EXPLORER1, m_arrMapBrowser[0]);
	DDX_Control(pDX, IDC_EXPLORER2, m_arrMapBrowser[1]);
	DDX_Control(pDX, IDC_EXPLORER3, m_arrMapBrowser[2]);
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
	m_nWindowWidth = cx;
	m_nWindowHeight = cy;
	
	if (m_bInitComplete) 
	{
		for (int i = 0; i < GPS_BROWSER_NUMBER; i++) {
			if (i == m_nCurrMapBrowserId) {
				GetDlgItem(m_arrBrowserId[i])->MoveWindow(0, 0, cx, cy);
			}
			else {
				GetDlgItem(m_arrBrowserId[i])->MoveWindow(0, 0, 0, 0);
			}
		}
	}
}



BOOL CGPSDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	// init browser id array
	m_arrBrowserId[0] = IDC_EXPLORER1;
	m_arrBrowserId[1] = IDC_EXPLORER2;
	m_arrBrowserId[2] = IDC_EXPLORER3;

	// Hide all browsers
	for (int i = 0; i < GPS_BROWSER_NUMBER; i++) {
		m_arrMapBrowser[i].Navigate(_T("about:blank"), NULL, NULL, NULL, NULL);
		m_arrMapBrowser[i].ShowScrollBar(SB_VERT, false);
		m_arrMapBrowser[i].ShowScrollBar(SB_HORZ, false);
	}

	// Reset Gps information variables
	ResetGpsMapInfo(&m_stInitGpsInfo);
	ResetGpsMapInfo(&m_stPrevGpsInfo);
	ResetGpsMapInfo(&m_stCurrGpsInfo);

	m_nCurrMapBrowserId = DEFAULT_MAP_BROWSER_ID;
	
	m_bInitComplete = true;

	ShowBrowser(m_nCurrMapBrowserId);

	// setting default GpsWindow data 
	SetBrowserDefaultContents(m_nCurrMapBrowserId);
	
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
		}

		// set GPS init information
		if (m_stInitGpsInfo.stGpsPos.fLat == 0 && m_stInitGpsInfo.stGpsPos.fLon == 0)
		{
			SetGpsMapInfo(&m_stInitGpsInfo, &m_stCurrGpsInfo);
			

			int nNewDataBrowser = GetNextBrowserId(m_nCurrMapBrowserId, GPS_BROWSER_NUMBER);

			// save current gps location infomation to next browser
			SetBrowserContents(nNewDataBrowser, &m_stCurrGpsInfo);
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
		{
#if 0
			static int nCounter = 0;

			nCounter ++;


			if (nCounter % 4 == 0)
			{
				CString strHtml = L"Javascript:displayAlert(); ";

				CHTMLWriter htmlWriter(m_arrMapBrowser[m_nCurrMapBrowserId].GetControlUnknown());
				htmlWriter.RunScript(strHtml);

				CDialog::OnTimer(nIDEvent);

				KillTimer(TIMER_GPS_MAP);
				return;
			}

			if (nCounter > 20)
				nCounter = 0;
#endif			

			GpsMapInfo_t mapInfo;
			bool bImageUsed = true;

			static bool bCalled = false;

			if (bCalled == false)
			{
				static int nCallCounter = 0;
				if (++nCallCounter > 5) {
					bCalled = true;

					CString strHtml = L"Javascript:initMaps(); " ;

					CHTMLWriter htmlWriter(m_arrMapBrowser[m_nCurrMapBrowserId].GetControlUnknown());
					htmlWriter.RunScript(strHtml);

					CDialog::OnTimer(nIDEvent);
					return;
				}
			}

			if (bCalled == true)
			{
				SetGpsMapInfo(&mapInfo, &m_stCurrGpsInfo);
			
			
				TCHAR szPath[_MAX_PATH];
				VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szPath, _MAX_PATH));

				if (mapInfo.strCarImageName.IsEmpty())
					bImageUsed = false;
				
				CString csPath(szPath);
				int nIndex  = csPath.ReverseFind(_T('\\'));
				if (nIndex > 0) {
					csPath = csPath.Left(nIndex);
				}
				else {
					csPath.Empty();
				}	

				csPath = L"file://" + csPath + L"/bmp2/Car/";
				if (mapInfo.ubAlarmState == ALARM_STATE_NO_ALARM)
					csPath += L"online/" + mapInfo.strCarImageName;
				else
					csPath += L"alarm/" + mapInfo.strCarImageName;

				csPath.Replace('\\', '/');

				CString strLatLon = L"";
				strLatLon.Format(L"%f, %f", mapInfo.stGpsPos.fLat + OFFSET_LAT, mapInfo.stGpsPos.fLon + OFFSET_LNG);

				CString strLat = L"";
				strLat.Format(L"%f", mapInfo.stGpsPos.fLat + OFFSET_LAT);

				CString strLon = L"";
				strLon.Format(L"%f", mapInfo.stGpsPos.fLon + OFFSET_LNG);
				
				ShowBrowser(m_nCurrMapBrowserId);

				CString strTime, strCurrTime, strSpeed;
				strTime = L"10:20";
				strCurrTime = L"2015-11-21 11:22:33";

				static int i = 0;
				strSpeed.Format(L"%d km/h", ++i);

				CString strInfoContents = L"";
				strInfoContents += L"'<div id=\"content\">'+";
				strInfoContents += L"'<div id=\"siteNotice\">'+";
				strInfoContents += L"'</div>'+";
				strInfoContents += L"'<div id=\"bodyContent\">'+";
//				strInfoContents += L"'<b>TIME</b> : " + strTime + L"<br/>' + ";
//				strInfoContents += L"'<b>Current Time</b> : " + strCurrTime + L"<br/>' + ";
//				strInfoContents += L"'<b>SPEED</b> : " + strSpeed + L"<br/>' + ";
				strInfoContents += L"'</div>'+";
				strInfoContents += L"'</div>'";

				strInfoContents = L"'<div id=\"content\">' '<div id=\"siteNotice\">' '</div>' '<div id=\"bodyContent\">' '</div>' '</div>'";
				//				strInfoContents += L"'<b>TIME</b> : " + strTime + L"<br/>' + ";
				//				strInfoContents += L"'<b>Current Time</b> : " + strCurrTime + L"<br/>' + ";
				//				strInfoContents += L"'<b>SPEED</b> : " + strSpeed + L"<br/>' + ";
//				strInfoContents += L"'</div>'+";
//				strInfoContents += L"'</div>'";
#if 0
				CString strHtml = L"Javascript:setMarker(\"" + strLat + L"\", \"" + strLon + L"\", \"" + csPath + L"\"); ";
#else 

				strInfoContents = L"My Project";
				// CString strHtml = L"Javascript:displayMarker(\"" + strLat + L"\", \"" + strLon + L"\", \"" + csPath + L"\"); ";
				CString strHtml = L"Javascript:setMarker(\"" + strLat + L"\", \"" + strLon + L"\", \"" + csPath + L"\", ";
				strHtml += "\"" + strTime + L"\", \"" + strCurrTime + L"\", \"" + strSpeed + L"\"";
				strHtml += L"); ";
#endif;
				
				
				CHTMLWriter htmlWriter(m_arrMapBrowser[m_nCurrMapBrowserId].GetControlUnknown());
				htmlWriter.RunScript(strHtml);
				
				CDialog::OnTimer(nIDEvent);
				return;
			}

		}

	
		m_nCurrMapBrowserId = GetNextBrowserId(m_nCurrMapBrowserId, GPS_BROWSER_NUMBER);
		
		int nNewDataBrowser = GetNextBrowserId(m_nCurrMapBrowserId, GPS_BROWSER_NUMBER);

		// show browser
		ShowBrowser(m_nCurrMapBrowserId);

		// save current gps location infomation to next browser
		SetBrowserContents(nNewDataBrowser, &m_stCurrGpsInfo);
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

int CGPSDlg::GetNextBrowserId(int nCurrentId, int nBrowserNum)
{
	int nNextId = (nCurrentId + 1) % nBrowserNum;
	return nNextId;
}

void CGPSDlg::ShowBrowser(int nBrowserId)
{
	for (int i = 0; i < GPS_BROWSER_NUMBER; i++)
	{
		if (i == nBrowserId) {
			GetDlgItem(m_arrBrowserId[i])->MoveWindow(0, 0, m_nWindowWidth, m_nWindowHeight);
		}
		else {
			GetDlgItem(m_arrBrowserId[i])->MoveWindow(0, 0, 0, 0);
		}
	}	
}

void CGPSDlg::SetBrowserContents(int nBrowserId, GpsMapInfo_t *pGpsMapInfo)
{
	if (pGpsMapInfo == NULL)
		return;

	GpsMapInfo_t mapInfo;
	bool bImageUsed = true;

	SetGpsMapInfo(&mapInfo, pGpsMapInfo);
	
	
	TCHAR szPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szPath, _MAX_PATH));

	if (pGpsMapInfo->strCarImageName.IsEmpty())
		bImageUsed = false;
	
	CString csPath(szPath);
	int nIndex  = csPath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		csPath = csPath.Left(nIndex);
	}
	else {
		csPath.Empty();
	}	

	csPath = L"file://" + csPath + L"/bmp2/Car/";
	if (mapInfo.ubAlarmState == ALARM_STATE_NO_ALARM)
		csPath += L"online/" + mapInfo.strCarImageName;
	else
		csPath += L"alarm/" + mapInfo.strCarImageName;

	csPath.Replace('\\', '/');

	CString strLatLon = L"";
	strLatLon.Format(L"%f, %f", mapInfo.stGpsPos.fLat + OFFSET_LAT, mapInfo.stGpsPos.fLon + OFFSET_LNG);

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
		
	


	CString strHtml = L"<!DOCTYPE html><html>";
	strHtml += L"<head>";
	strHtml += L"<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\"><meta charset=\"utf-8\">";
	strHtml += L"<title>Simple icons</title>";
	strHtml += L"<style>html, body {height: 100%;margin: 0px;padding: 0px} #map-canvas {height: 100%;margin: 0px;padding: 0px}</style>";	
	strHtml += L"</head>";

	strHtml += L"<body>";
	strHtml += L"<div id=\"map-canvas\"></div>";

	strHtml += L"<script src=\"https://maps.googleapis.com/maps/api/js?v=3.exp\"></script>";

#if 0	// add alert function for testing
	strHtml += L"<script> function displayAlert() { ";
	strHtml += L"alert(\"My project\");";
	strHtml += L"}";
	strHtml += L"</script>";
#endif

#if 0	// map setting function
	{
		CString szMarkerHtml = L"<script>";

		szMarkerHtml += L"function displayMarker(strLat, strLon, csPath) { ";


		szMarkerHtml += L"var mapOptions = {zoom: 16,center: new google.maps.LatLng(strLat, strLon)}; ";
		szMarkerHtml += L"var map = new google.maps.Map(document.getElementById('map-canvas'),mapOptions); ";
		szMarkerHtml += L"var image = csPath; ";
		szMarkerHtml += L"var myLatLng = new google.maps.LatLng(strLat, strLon); ";	
		szMarkerHtml += L"var beachMarker = new google.maps.Marker({position: myLatLng,map: map,icon: image});";


		szMarkerHtml += L"} ";
		szMarkerHtml += L"</script>";

		strHtml += szMarkerHtml;
	}
	
#endif

#if 1	// Moving map with Marker
	{
		CString strMapHtml = L"<script>";

//		strMapHtml += L"window.mapobject = window.google || {}; ";
//		strMapHtml += L"mapobject.maps = google.maps || {}; ";
//		strMapHtml += L"mapobject.maps.MapTypeId = google.maps.MapTypeId||{}; ";

		// define global variables
		strMapHtml += L"var g_map; ";
		strMapHtml += L"var g_currPositionMarker; ";
		strMapHtml += L"var g_infowindow; ";

		//=======================
		// initMaps function
		//=======================
		strMapHtml += L"function initMaps() { ";

		// default location :::  lat: 40.8833, lng: -85.0167
		strMapHtml += L"var defaultLat = 40.8833; ";
		strMapHtml += L"var defaultLon = -85.0167; ";

		strMapHtml += L"var myLatLng = new google.maps.LatLng(defaultLat, defaultLon); ";
		strMapHtml += L"var mapOptions = {zoom: 16,center: new google.maps.LatLng(defaultLat, defaultLon)}; ";
		strMapHtml += L"g_map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions); ";
		strMapHtml += L"g_currPositionMarker = new google.maps.Marker({position: myLatLng, map: g_map, icon: \" \"}); ";
		strMapHtml += L"g_infowindow = new google.maps.InfoWindow( {";
		strMapHtml += L"content: \"\",";
		strMapHtml += L"maxWidth: 220";
		strMapHtml += L"} );";

		// add info window to marker
		strMapHtml += L"g_currPositionMarker.addListener('click', function() {";
		strMapHtml += L"g_infowindow.open(g_map, g_currPositionMarker); ";
		strMapHtml += L"} ); ";
		strMapHtml += L"g_infowindow.open(g_map, g_currPositionMarker); ";



		strMapHtml += L"} ";


		//=======================
		// setMarker function
		//=======================
#if 0
		strMapHtml += L"function setMarker(strLat, strLon, csPath) { ";
#else
		strMapHtml += L"function setMarker(strLat, strLon, csPath, strTime, strCurrTime, strSpeed) { ";
#endif

		strMapHtml += L"var mapOptions = {zoom: 16,center: new google.maps.LatLng(strLat, strLon)}; ";
		strMapHtml += L"var image = csPath; ";
		strMapHtml += L"var myLatLng = new google.maps.LatLng(strLat, strLon); ";	
		
		strMapHtml += L"g_map.setCenter(myLatLng); ";

		strMapHtml += L"g_currPositionMarker.setPosition(myLatLng); ";
		strMapHtml += L"g_currPositionMarker.setIcon(image); ";

#if 0
		// define infoWindow content
		strMapHtml += L"var contentString = '<div id=\"content\">'+";
		strMapHtml += L"'<div id=\"siteNotice\">'+";
		strMapHtml += L"'</div>'+";
		strMapHtml += L"'<div id=\"bodyContent\">'+";
		strMapHtml += L"'<b>TIME</b> : strTime<br/>' + ";
		strMapHtml += L"'<b>Current Time</b> : strCurrTime<br/>' + ";
		strMapHtml += L"'<b>SPEED</b> : strSpeed<br/>' + ";
		strMapHtml += L"'</div>'+";
		strMapHtml += L"'</div>';";
#endif		

		strMapHtml += L"var contentString = '<div id=\"content\">'+";
		strMapHtml += L"'<div id=\"siteNotice\">'+";
		strMapHtml += L"'</div>'+";
		strMapHtml += L"'<div id=\"bodyContent\">'+";
		strMapHtml += L"'<b>TIME</b> :' + strTime + '<br/>' + ";
		strMapHtml += L"'<b>Current Time</b> :' + strCurrTime + '<br/>' + ";
		strMapHtml += L"'<b>SPEED</b> :' + strSpeed + '<br/>' + ";
		strMapHtml += L"'</div>'+";
		strMapHtml += L"'</div>';";

//		strMapHtml += L"var contentString = strInfoContent; ";

		strMapHtml += L"g_infowindow.setContent(contentString); ";
//		strMapHtml += L"g_infowindow.open(g_map, g_currPositionMarker); ";

		strMapHtml += L"} ";

		strMapHtml += L"</script>";

		// add to prev html
		strHtml += strMapHtml;
	}
#endif


	strHtml += L"<script>function initialize() { ";
	strHtml += L"var mapOptions = {zoom: 16,center: new google.maps.LatLng(" + strLatLon + L")}; ";
	strHtml += L"var map = new google.maps.Map(document.getElementById('map-canvas'),mapOptions); ";
	strHtml += L"var image = '" + csPath + L"'; ";
	strHtml += L"var myLatLng = new google.maps.LatLng(" + strLatLon + L"); ";
	strHtml += L"var beachMarker = new google.maps.Marker({position: myLatLng,map: map,icon: image});";

	if (bImageUsed) 
	{
		// define infoWindow content
		strHtml += L"var contentString = '<div id=\"content\">'+";
		strHtml += L"'<div id=\"siteNotice\">'+";
		strHtml += L"'</div>'+";
		strHtml += L"'<div id=\"bodyContent\">'+";
		strHtml += L"'<b>TIME</b> : " + strTime + L"<br/>' + ";
		strHtml += L"'<b>Current Time</b> : " + strCurrTime + L"<br/>' + ";
		strHtml += L"'<b>SPEED</b> : " + strSpeed + L"<br/>' + ";
		strHtml += L"'</div>'+";
		strHtml += L"'</div>';";
		
		
		// define info window
		strHtml += L"var infowindow = new google.maps.InfoWindow( {";
		strHtml += L"content: contentString,";
		strHtml += L"maxWidth: 220";
		
		strHtml += L"} );";
		
		// add info window to marker
		strHtml += L"beachMarker.addListener('click', function() {";
		strHtml += L"infowindow.open(map, beachMarker);";
		strHtml += L"} );";
		
		
		// always show info window
		strHtml += L"infowindow.open(map, beachMarker);";		
	}
	

	// end of function 
	strHtml += L"} ";

	// register function for starting	
	strHtml += L"google.maps.event.addDomListener(window, 'load', initialize);";

	strHtml += L"</script>";
	
	strHtml += L"</body></html>";


	CHTMLWriter htmlWriter(m_arrMapBrowser[nBrowserId].GetControlUnknown());
	htmlWriter.Write(strHtml);
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

		SetBrowserDefaultContents(m_nCurrMapBrowserId);

		ShowBrowser(m_nCurrMapBrowserId);
	}
}

void CGPSDlg::SetBrowserDefaultContents(int nBrowserId)
{
	// setting default GpsWindow data 
	CHTMLWriter htmlWriter(m_arrMapBrowser[nBrowserId].GetControlUnknown());
	htmlWriter.Write(L"<!DOCTYPE html><html><head><style type=\"text/css\">html, body, #map-canvas { height: 100%; margin: 0; padding: 0;}</style><script type=\"text/javascript\"src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyA1bMjkIhG61jnA6MefByFaSVglbR5kvjw\"></script><script type=\"text/javascript\">function initialize() {var mapOptions = {center: { lat: 40.8833, lng: -85.0167},zoom: 5};var map = new google.maps.Map(document.getElementById('map-canvas'),mapOptions);}google.maps.event.addDomListener(window, 'load', initialize);</script></head><body><div id=\"map-canvas\"></div></body></html>");
}






