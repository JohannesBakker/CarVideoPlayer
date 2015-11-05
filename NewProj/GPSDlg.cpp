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

#define VIEW_DIFF	3
#define TIMER_INTERVAL	500

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

	for (int i = 0; i < BROWSER_LIST_SIZE; i++)
		m_bViewUpdatedArray[i] = false;
}

void CGPSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGPSDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EXPLORER1, m_WebBrowser);
	DDX_Control(pDX, IDC_EXPLORER2, m_WebBrowser2);
	DDX_Control(pDX, IDC_EXPLORER3, m_WebBrowser3);
	DDX_Control(pDX, IDC_EXPLORER4, m_WebBrowser4);
	DDX_Control(pDX, IDC_EXPLORER5, m_WebBrowser5);
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
	
	if (m_bInitComplete) {
		for (int i = 0; i < BROWSER_LIST_SIZE; i++) {
			GetDlgItem(m_BrowserIdArray[i])->MoveWindow(0, 0, cx, cy);
		}
	}
	
}

BOOL CGPSDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	ResetGpsInfo();

	m_BrowerArray[0] = &m_WebBrowser;
	m_BrowerArray[1] = &m_WebBrowser2;
	m_BrowerArray[2] = &m_WebBrowser3;
	m_BrowerArray[3] = &m_WebBrowser4;
	m_BrowerArray[4] = &m_WebBrowser5;

	m_BrowserIdArray[0] = IDC_EXPLORER1;
	m_BrowserIdArray[1] = IDC_EXPLORER2;
	m_BrowserIdArray[2] = IDC_EXPLORER3;
	m_BrowserIdArray[3] = IDC_EXPLORER4;
	m_BrowserIdArray[4] = IDC_EXPLORER5;

	m_nCurrentBrowserId = 0;
	m_nLastGpsWinId = 0;
	m_bInitedGpsInfo = false;

	for (int i = 0; i < BROWSER_LIST_SIZE; i++)
	{
		m_BrowerArray[i]->Navigate(_T("about:blank"), NULL, NULL, NULL, NULL);

		m_BrowerArray[i]->ShowScrollBar(SB_VERT, false);
		m_BrowerArray[i]->ShowScrollBar(SB_HORZ, false);

		GetDlgItem(m_BrowserIdArray[i])->MoveWindow(0, 0, 0, 0, true);

		CHTMLWriter htmlWriter(m_BrowerArray[i]->GetControlUnknown());
		htmlWriter.Write(L"<!DOCTYPE html><html><head><style type=\"text/css\">html, body, #map-canvas { height: 100%; margin: 0; padding: 0;}</style><script type=\"text/javascript\"src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyA1bMjkIhG61jnA6MefByFaSVglbR5kvjw\"></script><script type=\"text/javascript\">function initialize() {var mapOptions = {center: { lat: 40.8833, lng: -85.0167},zoom: 5};var map = new google.maps.Map(document.getElementById('map-canvas'),mapOptions);}google.maps.event.addDomListener(window, 'load', initialize);</script></head><body><div id=\"map-canvas\"></div></body></html>");
	}

	GetDlgItem(m_BrowserIdArray[m_nCurrentBrowserId])->MoveWindow(0, 0, m_nWindowWidth, m_nWindowHeight, true);

	SetTimer(0, TIMER_INTERVAL, NULL);

	m_bInitComplete = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGPSDlg::ShowGPS_Pos(MainBinaryData * pMBDatas)
{
	WCHAR buf[40];
 	double dbl_Ntemp, dbl_Wtemp;
 	double dbl_NResult, dbl_WResult;
 	
	if(pMBDatas->dwGPSW_Down < 0)
		pMBDatas->dwGPSW_Down + 0x100000000;
 	dbl_Wtemp = (DWORD)((double)pMBDatas->dwGPSW_Down / 3600.0 + (double)pMBDatas->byGPSW_Lw_Up / 60.0 * 10000000.0);
 	if ( pMBDatas->dwGPSN_Down < 0 )
		pMBDatas->dwGPSN_Down = pMBDatas->dwGPSN_Down + 0x100000000;
 	dbl_Ntemp = ((double)pMBDatas->dwGPSN_Down / 3600.0 + (double)pMBDatas->byGPSN_Lw_Up / 60.0 * 10000000.0);
 	dbl_WResult = (dbl_Wtemp * 0.0000001 + (double)pMBDatas->byGPSW_Hi_Up);
	if(pMBDatas->byEW_Flag != 0x45)
		dbl_WResult = -1 * dbl_WResult;
 	dbl_NResult = (dbl_Ntemp * 0.0000001 + (double)pMBDatas->byGPSN_Hi_Up);
	if(pMBDatas->byNS_Flag != 0x57)
		dbl_NResult = -1 * dbl_NResult;
	
	int nHighSpeed = pMBDatas->speed1 / 100;
	int nlowSpeed = pMBDatas->speed1 % 100;

	m_gpsCurInfo.fLat = dbl_NResult;
	m_gpsCurInfo.fLon = dbl_WResult;
	m_gpsCurInfo.fSpeed = (float)pMBDatas->speed1 / 100.0f;

	if (memcmp(&m_gpsCurInfo, &m_gpsOldInfo, sizeof(GPS_INFO)))
	{
		CString szImageName = _T("car-1.png");

		if (m_gpsOldInfo.fLat == 0 && m_gpsOldInfo.fLon == 0) {

		}
		else {
			int deg = GetBearingLocation(m_gpsOldInfo.fLat, m_gpsOldInfo.fLon, m_gpsCurInfo.fLat, m_gpsCurInfo.fLon);
			szImageName = GetCarImageName(deg);
		}

		m_gpsOldInfo = m_gpsCurInfo;
		m_bGPSUpdated = true;

		if (m_bInitedGpsInfo == false)
		{
			m_gpsInitInfo = m_gpsCurInfo;
			m_bInitedGpsInfo = true;
		}

		m_nLastGpsWinId = GetNextBrowserId(m_nLastGpsWinId);

		SetGpsBrowser(m_gpsCurInfo, m_nLastGpsWinId, szImageName);
	}
}


void CGPSDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bGPSUpdated == true)
	{
		m_bGPSUpdated = false;

		int nNextBrowserId = GetNextBrowserId(m_nLastGpsWinId);

		nNextBrowserId = m_nLastGpsWinId - VIEW_DIFF;
		if (nNextBrowserId < 0) {
			nNextBrowserId = BROWSER_LIST_SIZE - (VIEW_DIFF - m_nLastGpsWinId);
		}

		if (m_bViewUpdatedArray[nNextBrowserId] == false) {
			for (int i = 1; i < BROWSER_LIST_SIZE; i++)
			{
				nNextBrowserId = (nNextBrowserId + 1) % BROWSER_LIST_SIZE;
				if (m_bViewUpdatedArray[nNextBrowserId] == true) {
					break;
				}
			}			
		}
		
		if (m_bViewUpdatedArray[nNextBrowserId] == true) {
			if (nNextBrowserId == m_nCurrentBrowserId)
				return;			
		}

		SetActiveBrowser(nNextBrowserId);
	}
	CDialog::OnTimer(nIDEvent);
}

int CGPSDlg::GetNextBrowserId(int nCurrentId)
{
	int nNextId = nCurrentId + 1;
	
	if (nNextId >= BROWSER_LIST_SIZE)
		nNextId = 0;

	return nNextId;
}

void CGPSDlg::SetGpsBrowser(GPS_INFO gpsCurInfo, int browserId,  CString szCarImageName)
{
	TCHAR szPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szPath, _MAX_PATH));

	CString csPath(szPath);
	int nIndex  = csPath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		csPath = csPath.Left(nIndex);
	}
	else {
		csPath.Empty();
	}

	csPath = L"file://" + csPath + L"/bmp2/" + szCarImageName;
	csPath.Replace('\\', '/');

	CString strLatLon = L"";
	strLatLon.Format(L"%f, %f", gpsCurInfo.fLat, gpsCurInfo.fLon);
	CString strHtml = L"<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\"><meta charset=\"utf-8\"><title>Simple icons</title><style>html, body, #map-canvas {height: 100%;margin: 0px;padding: 0px}</style><script src=\"https://maps.googleapis.com/maps/api/js?v=3.exp\"></script><script>function initialize() { var mapOptions = {zoom: 16,center: new google.maps.LatLng(" + strLatLon + L")}; var map = new google.maps.Map(document.getElementById('map-canvas'),mapOptions); var image = '" + csPath + L"'; var myLatLng = new google.maps.LatLng(" + strLatLon + L"); var beachMarker = new google.maps.Marker({position: myLatLng,map: map,icon: image});} google.maps.event.addDomListener(window, 'load', initialize);</script></head><body><div id=\"map-canvas\"></div></body></html>";
	CHTMLWriter htmlWriter(m_BrowerArray[browserId]->GetControlUnknown());
	htmlWriter.Write(strHtml);

	m_bViewUpdatedArray[browserId] = true;
}

void CGPSDlg::SetActiveBrowser(int browserId)
{
	GetDlgItem(m_BrowserIdArray[browserId])->MoveWindow(0, 0, m_nWindowWidth, m_nWindowHeight);	

	if (browserId != m_nCurrentBrowserId)
		GetDlgItem(m_BrowserIdArray[m_nCurrentBrowserId])->MoveWindow(0, 0, 0, 0);

	m_nCurrentBrowserId = browserId;
}

void CGPSDlg::ResetMapInfo(bool bResetMap)
{
	m_nCurrentBrowserId = 0;
	m_nLastGpsWinId = 0;
	m_bInitComplete = false;

	GetDlgItem(m_BrowserIdArray[0])->MoveWindow(0, 0, m_nWindowWidth, m_nWindowHeight);	
	for (int i = 1; i < BROWSER_LIST_SIZE; i++) 
	{
		GetDlgItem(m_BrowserIdArray[i])->MoveWindow(0, 0, 0, 0);
		m_bViewUpdatedArray[i] = false;
	}

	//m_bViewUpdatedArray[0] = true;

	ResetGpsInfo();
	
	if (bResetMap)
	{
		SetActiveBrowser(m_nLastGpsWinId);
		
		//SetGpsBrowser(m_gpsInitInfo, m_nLastGpsWinId);
	}
}

int CGPSDlg::GetBearingLocation(float pos1Lat, float pos1Lng, float pos2Lat, float pos2Lng)
{
	float dLon = pos2Lng - pos1Lng;
	float y = sin(dLon) * cos(pos2Lat);
	float x = cos(pos1Lat) * sin(pos2Lat) - sin(pos1Lat)*cos(pos2Lat)*cos(dLon);
	float brng = atan2(y, x);

	brng = brng * 180 / 3.141592;

	int deg = (int)brng;
	deg = 360 - ((deg + 360) % 360);

	return deg;
}

CString CGPSDlg::GetCarImageName(int degree)
{
	static AngleImage arrAngleImages[] = {
		{0,		_T("car-1.png")},
		{45,	_T("car-2.png")},
		{90,	_T("car-3.png")},
		{135,	_T("car-4.png")},
		{180,	_T("car-5.png")},
		{225,	_T("car-6.png")},
		{270,	_T("car-7.png")},
		{315,	_T("car-8.png")},
		{360,	_T("car-1.png")},
	};

	int i;
	int selId = 0;

	for (i = 1; i < sizeof(arrAngleImages)/sizeof(AngleImage); i++)
	{
		int d1 = abs(arrAngleImages[i].degree - degree);
		int d2 = abs(arrAngleImages[selId].degree - degree);

		/*
		d1 = d1 % 360;
		d1 = d1 > 180 ? (360 - d1): d1;

		d2 = d2 % 360;
		d2 = d2 > 180 ? (360 - d2): d2;
		*/

		if (d1 < d2)
			selId = i;
	}

	CString imageName = arrAngleImages[selId].imageName;
	return imageName;
}

void CGPSDlg::ResetGpsInfo()
{
	memset(&m_gpsOldInfo, 0, sizeof(GPS_INFO));
	memset(&m_gpsCurInfo, 0, sizeof(GPS_INFO));
}
