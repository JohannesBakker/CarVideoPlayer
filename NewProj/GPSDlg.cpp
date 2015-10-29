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
}

void CGPSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGPSDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EXPLORER1, m_WebBrowser);
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
	if (m_WebBrowser.GetSafeHwnd() != NULL)
	{
		m_WebBrowser.MoveWindow(0, 0, cx, cy);
	}
}

BOOL CGPSDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	memset(&m_gpsOldInfo, 0, sizeof(GPS_INFO));
	memset(&m_gpsCurInfo, 0, sizeof(GPS_INFO));

	m_WebBrowser.Navigate(_T("about:blank"), NULL, NULL, NULL, NULL);

	m_WebBrowser.ShowScrollBar(SB_VERT, false);
	m_WebBrowser.ShowScrollBar(SB_HORZ, false);

	CHTMLWriter htmlWriter(m_WebBrowser.GetControlUnknown());
	htmlWriter.Write(L"<!DOCTYPE html><html><head><style type=\"text/css\">html, body, #map-canvas { height: 100%; margin: 0; padding: 0;}</style><script type=\"text/javascript\"src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyA1bMjkIhG61jnA6MefByFaSVglbR5kvjw\"></script><script type=\"text/javascript\">function initialize() {var mapOptions = {center: { lat: 40.8833, lng: -85.0167},zoom: 5};var map = new google.maps.Map(document.getElementById('map-canvas'),mapOptions);}google.maps.event.addDomListener(window, 'load', initialize);</script></head><body><div id=\"map-canvas\"></div></body></html>");

	SetTimer(0, 3000, NULL);

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
		m_gpsOldInfo = m_gpsCurInfo;
		m_bGPSUpdated = true;
	}
}


void CGPSDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bGPSUpdated == true)
	{
		m_bGPSUpdated = false;
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

		csPath = L"file://" + csPath + L"/bmp2/car.png";
		csPath.Replace('\\', '/');

		CString strLatLon = L"";
		strLatLon.Format(L"%f, %f", m_gpsCurInfo.fLat, m_gpsCurInfo.fLon);
		CString strHtml = L"<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\"><meta charset=\"utf-8\"><title>Simple icons</title><style>html, body, #map-canvas {height: 100%;margin: 0px;padding: 0px}</style><script src=\"https://maps.googleapis.com/maps/api/js?v=3.exp\"></script><script>function initialize() { var mapOptions = {zoom: 16,center: new google.maps.LatLng(" + strLatLon + L")}; var map = new google.maps.Map(document.getElementById('map-canvas'),mapOptions); var image = '" + csPath + L"'; var myLatLng = new google.maps.LatLng(" + strLatLon + L"); var beachMarker = new google.maps.Marker({position: myLatLng,map: map,icon: image});} google.maps.event.addDomListener(window, 'load', initialize);</script></head><body><div id=\"map-canvas\"></div></body></html>";
		CHTMLWriter htmlWriter(m_WebBrowser.GetControlUnknown());
		htmlWriter.Write(strHtml);
	}
	CDialog::OnTimer(nIDEvent);
}
