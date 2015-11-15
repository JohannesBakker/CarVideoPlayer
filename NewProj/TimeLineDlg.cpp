// TimeLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "TimeLineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMELINE_TIMER		20005

#define TIME_TEXT_COLOR		RGB(100, 100, 100)
#define TEXT_BK_COLOR		RGB(47, 56, 66)
#define MIN_TEXT_COLOR		RGB(200, 200, 200)
#define MIN_BK_COLOR_1		RGB(200, 0, 0)
#define MIN_BK_COLOR_2		RGB(200, 200, 0)
#define WHITE_TEXT_COLOR	RGB(255, 255, 255)
#define RECT_COLOR_1		RGB(37, 46, 56)
#define RECT_COLOR_2		RGB(57, 66, 76)
#define RED_COLOR			RGB(255, 0, 0)
#define GREEN_COLOR			RGB(0, 255, 0)
#define BLUE_COLOR			RGB(0, 0, 255)


/////////////////////////////////////////////////////////////////////////////
// CTimeLineDlg dialog

CxStatic	CTimeLineDlg::m_staticTimes[24];
bool		CTimeLineDlg::m_bTimeFlag;
float		CTimeLineDlg::m_fTimeSeekPos;
bool		CTimeLineDlg::m_bTimeSeekFlag = false;
CTimeLineDlg::CTimeLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeLineDlg)
	//}}AFX_DATA_INIT
	m_bTimeLineDlg = false;
	m_nLastSelStatic = 0;
	ZeroMemory(&m_StartTime, sizeof(m_StartTime));
	ZeroMemory(&m_EndTime, sizeof(m_EndTime));
	ZeroMemory(&m_HourRect, sizeof(m_HourRect));
	m_chBuf = new wchar_t[4];
}


void CTimeLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeLineDlg)
	DDX_Control(pDX, IDC_MINUTE_STATIC, m_static_minute);
	DDX_Control(pDX, IDC_HOUR_STATIC, m_static_hour);
	DDX_Control(pDX, IDC_CAMERA2_STATIC, m_static_camera2);
	DDX_Control(pDX, IDC_CAMERA1_STATIC, m_static_camera1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeLineDlg, CDialog)
	//{{AFX_MSG_MAP(CTimeLineDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeLineDlg message handlers

void CTimeLineDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if (m_bTimeLineDlg)
	{
		int i;
		for(i = 0; i < 24; i++)
			m_staticTimes[i].MoveWindow(50 + (int)((float)(cx - 50) / 24 * i), 0, (int)((float)(cx - 50) / 24) - 1, 15);
		m_static_hour.MoveWindow(2, 0, 48, 20);
		m_static_minute.MoveWindow(2, 20, 48, 20);
		m_static_camera1.MoveWindow(2, 41 + ((cy - 40) / 5 - 15) / 2, 48, 15);
		m_static_camera2.MoveWindow(2, 41 + (cy - 40) / 5 + ((cy - 40) / 5 - 15) / 2, 48, 15);
		for(i = 0; i < 13; i++)
			m_staticMinutes[i].MoveWindow(50 + (int)(((float)(cx - 60)/ 12 * i)) - 10, 18, 20, 10);
		GetDlgItem(IDC_STATIC_TIMELINE)->MoveWindow(0, 40, cx, cy - 40);
	}
//		GetDlgItem(IDC_TIMELINE_LIST)->MoveWindow(0, 0, cx, cy);
}

BOOL CTimeLineDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CWnd* pWnd = this;
	char* buf;
	buf = new char[4];
	int i;
	for(i = 0; i < 24; i++)
	{		
		m_staticTimes[i].Create(m_chBuf,SS_CENTER,CRect(i, 0, 0, 0), pWnd);
		m_staticTimes[i].ShowWindow(SW_SHOW | SW_NORMAL);
		m_staticTimes[i].SetTextColor(TIME_TEXT_COLOR);
		m_staticTimes[i].SetFont("Arial", 14, FW_NORMAL);		
		m_staticTimes[i].SetBkColor(TEXT_BK_COLOR, 0, CxStatic::Normal);
	}
	for(i = 0 ; i < 24; i++)
	{
		itoa(i, buf, 10);
		Ascii2Unichar(buf, m_chBuf);
		m_staticTimes[i].Format(m_chBuf);
	}
	for(i = 0; i < 13; i++)
	{
		m_staticMinutes[i].Create(_T("0"), SW_NORMAL, CRect(0, 0, 0, 0), this);
		m_staticMinutes[i].ShowWindow(SW_SHOW);
		m_staticMinutes[i].SetTextColor(MIN_TEXT_COLOR);
		m_staticMinutes[i].SetFont("Arial", 13, FW_NORMAL);
		m_staticMinutes[i].SetBkColor(RECT_COLOR_2, 0, CxStatic::Normal);
	}
	m_static_hour.SetTextColor(WHITE_TEXT_COLOR);
	m_static_hour.SetFont("Arial", 14, FW_NORMAL);
	m_static_hour.FormatW(CNewProjDlg::m_wLanguageRes[250]);
	m_static_hour.SetBkColor(TEXT_BK_COLOR, 0, CxStatic::Normal);
	m_static_minute.SetTextColor(WHITE_TEXT_COLOR);
	m_static_minute.SetFont("Arial", 14, FW_NORMAL);
	m_static_minute.FormatW(CNewProjDlg::m_wLanguageRes[251]);
	m_static_minute.SetBkColor(TEXT_BK_COLOR, 0, CxStatic::Normal);
	m_static_camera1.SetTextColor(WHITE_TEXT_COLOR);
	m_static_camera1.SetFont("Arial", 14, FW_NORMAL);
	m_static_camera1.FormatW(CNewProjDlg::m_wLanguageRes[234]);
	m_static_camera1.SetBkColor(TEXT_BK_COLOR, 0, CxStatic::Normal);
	m_static_camera2.SetTextColor(WHITE_TEXT_COLOR);
	m_static_camera2.SetFont("Arial", 14, FW_NORMAL);
	m_static_camera2.FormatW(CNewProjDlg::m_wLanguageRes[235]);
	m_static_camera2.SetBkColor(TEXT_BK_COLOR, 0, CxStatic::Normal);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTimeLineDlg::OnPaint() 
{
	int i;
	CPaintDC dc(this); // device context for painting
	//SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
	wchar_t buf[4];
	char temp[4];
	CRect rt;
	GetClientRect(&rt);
	CDC *pDC = GetDC();
	CPen pen(PS_SOLID, 0, WHITE_TEXT_COLOR);
	if (m_bPaint)
	{
		pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), TEXT_BK_COLOR);
		pDC->FillSolidRect(48, 0, rt.Width() - 50, 40, RECT_COLOR_1);
		m_bPaint = false;
	}
	pDC->FillSolidRect(48, 32, rt.Width() - 50, 8, RECT_COLOR_1);
	pDC->SelectObject(&pen);


		pDC->FillSolidRect(50, 17, rt.Width(), 15, RECT_COLOR_2);

		for(i = 0; i <= 60; i++)
		{
			pDC->MoveTo(50 + (int)((float)(rt.Width() - 60) / 60 * i), 32);
			if (i % 5 == 0) 
			{
				pDC->LineTo(50 + (int)((float)(rt.Width() - 60) / 60 * i), 28);
				itoa(i, temp, 10);
				Ascii2Unichar(temp, buf);
				m_staticMinutes[i / 5].SetWindowText(buf);
			}
			else pDC->LineTo(50 + (int)((float)(rt.Width() - 60) / 60 * i), 30);
		}
		
	ReleaseDC(pDC);

	//End Minute Line

	GetDlgItem(IDC_STATIC_TIMELINE)->GetClientRect(&rt);
	pDC = GetDlgItem(IDC_STATIC_TIMELINE)->GetDC();
	pDC->SelectObject(&pen);
	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), TEXT_BK_COLOR);
	ReleaseDC(pDC);


	//pDC = GetDC();
	GetClientRect(&rt);
#if 10
	pDC = GetDlgItem(IDC_STATIC_TIMELINE)->GetDC();
	GetDlgItem(IDC_STATIC_TIMELINE)->GetClientRect(&rt);
	pDC->SelectObject(&pen);
	for(i = 0; i < 5; i++)
	{
		pDC->MoveTo(0, (rt.Height()) / 5 * i);
		pDC->LineTo(rt.Width(), (rt.Height()) / 5 * i);
	}
	pDC->MoveTo(50, 0);
	pDC->LineTo(50, rt.Height());
	m_static_camera1.FormatW(CNewProjDlg::m_wLanguageRes[234]);
	m_static_camera2.FormatW(CNewProjDlg::m_wLanguageRes[235]);
#endif
	pen.DeleteObject();
	ReleaseDC(pDC);


	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CTimeLineDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (CNewProjDlg::m_DateTime->wDay == 0) return;
	
	//KillTimer(TIMELINE_TIMER);
	Sleep(100);
	int i;
	wchar_t buf[4];
	char	temp[4];
	// TODO: Add your message handler code here and/or call default
	CPen pen(PS_SOLID, 0, WHITE_TEXT_COLOR);
	float flt_X;
	float flt_Width;
	flt_X = (float)point.x;
	CRect rt;
	GetClientRect(&rt);
	flt_Width = (float)rt.Width();
	CDC* pDC = GetDC();
	pDC->SelectObject(&pen);
	if (point.y <= 16)//hour buttons click
	{		
		OnPaint();
		PaintHourMinuteLine();

		m_staticTimes[m_nLastSelStatic].SetTextColor(TIME_TEXT_COLOR);
		m_nLastSelStatic = (int)((flt_X - 50) / ((flt_Width - 50) / 24));
		m_staticTimes[m_nLastSelStatic].SetTextColor(WHITE_TEXT_COLOR);

		if (CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wHour == m_nLastSelStatic)
		{
			GetHourRect(CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr]);
			pDC->MoveTo(m_HourRect.left, 40);
			pDC->LineTo(m_HourRect.left, rt.Height());
			m_bTimeFlag = true;

			GetMinuteRect(CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr]);
			pDC->MoveTo(m_MinuteRect.left, 17);
			pDC->LineTo(m_MinuteRect.left, 40);
		}
		else
		{
			pDC->MoveTo(50 + (int)((flt_Width - 50) / 24 * m_nLastSelStatic), 40);
			pDC->LineTo(50 + (int)((flt_Width - 50) / 24 * m_nLastSelStatic), rt.Height());
			m_bTimeFlag = false;
			float j = (flt_Width - 60) * ((float)m_nLastSelStatic / 24) + 50;		
			
			pDC->MoveTo(50, 17);
			pDC->LineTo(50, 40);
		}

		if (m_nCurHour == m_nLastSelStatic) 
			m_staticTimes[m_nCurHour].SetBkColor(MIN_BK_COLOR_1, 0, CxStatic::Normal);
		else 
			m_staticTimes[m_nCurHour].SetBkColor(MIN_BK_COLOR_2, 0, CxStatic::Normal);
	}
	else if (point.y >= 40)		//hour line click
	{
		if (point.x > 50)
		{	
			OnPaint();
			m_staticTimes[m_nLastSelStatic].SetTextColor(TIME_TEXT_COLOR);
			pDC->MoveTo(point.x, 40);
			pDC->LineTo(point.x, rt.Height());
			m_nLastSelStatic = (int)((flt_X - 50) / ((flt_Width - 50) / 24));
			m_staticTimes[m_nLastSelStatic].SetTextColor(WHITE_TEXT_COLOR);

			float j = ((flt_X - 50) - ((flt_Width - 50) / 24) * m_nLastSelStatic) / ((flt_Width - 50) / 24) * (flt_Width - 60);
			PaintHourMinuteLine();
			pDC->MoveTo((int)j + 50, 17);
			pDC->LineTo((int)j + 50, 40);

			if (point.x >= m_HourRect.left && point.x <= m_HourRect.left + m_HourRect.right)
				m_bTimeFlag = true;
			else
			{
				m_bTimeFlag = false;
				KillTimer(TIMELINE_TIMER);
			}

			if (m_nCurHour == m_nLastSelStatic) 
				m_staticTimes[m_nCurHour].SetBkColor(MIN_BK_COLOR_1, 0, CxStatic::Normal);
			else 
				m_staticTimes[m_nCurHour].SetBkColor(MIN_BK_COLOR_2, 0, CxStatic::Normal);
		}
	}
	else	//minute line click
	{
		if (point.x  > 50)
		{
			pDC->FillSolidRect(50, 17, rt.Width(), 15, RECT_COLOR_2);
			for (i = 0; i <= 60; i++)
			{
				pDC->MoveTo(50 + (int)((float)(rt.Width() - 60) / 60 * i), 32);
				if (i % 5 == 0) 
				{
					pDC->LineTo(50 + (int)((float)(rt.Width() - 60) / 60 * i), 28);
					itoa(i, temp, 10);
					Ascii2Unichar(temp, buf);
					m_staticMinutes[i / 5].SetWindowText(buf);
				}
				else {
					pDC->LineTo(50 + (int)((float)(rt.Width() - 60) / 60 * i), 30);
				}
			}
			PaintHourMinuteLine();
			pDC->MoveTo(point.x, 17);
			pDC->LineTo(point.x, 40);
			for(i = 0 ; i< CNewProjDlg::m_nPathCounts; i++)
			{
				GetMinuteRect(CNewProjDlg::m_DateTime[i], CNewProjDlg::m_dwDurations[i]);
				if (point.x >= m_MinuteRect.left && point.x <= m_MinuteRect.left + m_MinuteRect.right)
				{
					int nSeekVal = point.x - m_MinuteRect.left;
					m_bTimeFlag = true;
					m_fTimeSeekPos = (float)nSeekVal / m_MinuteRect.right;
					CNewProjDlg::m_nPathArraysPtr = i;
					//CToolBarDlg::TimeSeek(m_fTimeSeekPos, CNewProjDlg::m_nPathArraysPtr);
					m_bTimeSeekFlag = true;
					return;
				}
			}
// 			if (m_nLastSelStatic == CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wHour && (point.x >= m_MinuteRect.left && point.x <= m_MinuteRect.left + m_MinuteRect.right))
// 				m_bTimeFlag = true;
//			else 
				m_bTimeFlag = false;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);

}

void CTimeLineDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (CNewProjDlg::m_DateTime[0].wDay == 0) return;
	
	int i;
	float flt_X, flt_Width;
	wchar_t buf[4];
	char	temp[4];
	CRect rt;
	GetClientRect(&rt);
	flt_Width  = (float)rt.Width();
	flt_X = (float)point.x;
	flt_Width = (float)rt.Width();
	CDC* pDC = GetDC();
	CPen pen(PS_SOLID, 1, WHITE_TEXT_COLOR);
	pDC->SelectObject(&pen);
	
	// TODO: Add your message handler code here and/or call default

	if (nFlags == 0) 
		return;

	if (point.y >= 40)
	{
		if (point.x > 50)
		{	
			OnPaint();
			m_staticTimes[m_nLastSelStatic].SetTextColor(TIME_TEXT_COLOR);
			pDC->MoveTo(point.x, 40);
			pDC->LineTo(point.x, rt.Height());
			m_nLastSelStatic = (int)((flt_X - 50) / ((flt_Width - 50) / 24));
			m_staticTimes[m_nLastSelStatic].SetTextColor(WHITE_TEXT_COLOR);

			float j = ((flt_X - 50) - ((flt_Width - 50) / 24) * m_nLastSelStatic) / ((flt_Width - 50) / 24) * (flt_Width - 60);
			PaintHourMinuteLine();
			pDC->MoveTo((int)j + 50, 17);
			pDC->LineTo((int)j + 50, 40);

			if (m_nCurHour == m_nLastSelStatic) 
				m_staticTimes[m_nCurHour].SetBkColor(MIN_BK_COLOR_1, 0, CxStatic::Normal);
			else 
				m_staticTimes[m_nCurHour].SetBkColor(MIN_BK_COLOR_2, 0, CxStatic::Normal);
		}
	}else if (point.y > 16 && point.y < 40){
		if (point.x > 50)
		{
			pDC->FillSolidRect(50, 17, rt.Width(), 15, RECT_COLOR_2);
			for (i = 0; i <= 60; i++)
			{
				pDC->MoveTo(50 + (int)((float)(rt.Width() - 60) / 60 * i), 32);
				if (i % 5 == 0) 
				{
					pDC->LineTo(50 + (int)((float)(rt.Width() - 60) / 60 * i), 28);
					itoa(i, temp, 10);
					Ascii2Unichar(temp, buf);
					m_staticMinutes[i / 5].SetWindowText(buf);
				}
				else {
					pDC->LineTo(50 + (int)((float)(rt.Width() - 60) / 60 * i), 30);
				}
			}

			PaintHourMinuteLine();
			pDC->MoveTo(point.x, 17);
			pDC->LineTo(point.x, 40);
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

bool CTimeLineDlg::SetTimeRects(tagTIMESTAMP_STRUCT startTime, DWORD dwDuration, int nChannels)
{
	m_StartTime = startTime;
	m_dwDuration = dwDuration;
	m_nChannels = nChannels;
	return false;
}

void CTimeLineDlg::GetHourRect(SYSTEMTIME s_time, DWORD dwDuration)
{
	CRect rt;
	float flt_Width;
	GetClientRect(&rt);
	flt_Width = (float)rt.Width();
	m_HourRect.left = 50 + (int)((flt_Width - 50) / 24 * s_time.wHour 
						+ (flt_Width - 50) / 1440 * s_time.wMinute
						+ (flt_Width - 50) / 86400 * s_time.wSecond);
	m_HourRect.top = 40;
	m_HourRect.right = (int)((flt_Width - 50) / 86400 / 24 * (dwDuration / 1000));
	if (m_HourRect.right < 3) 
		m_HourRect.right = 3;

	if (m_nChannels == 2) 
		m_HourRect.bottom = (rt.Height() - 40) / 5 * 2;
	else 
		m_HourRect.bottom = (rt.Height() - 40) / 5;
}

void CTimeLineDlg::GetMinuteRect(SYSTEMTIME s_time, DWORD dwDuration)
{
	CRect rt;
	float flt_Width;
	GetClientRect(&rt);
	flt_Width = (float)rt.Width();
	m_MinuteRect.left = (long)(50 + (flt_Width - 60) / 60 * s_time.wMinute + (flt_Width - 60) / 3600 * s_time.wSecond);
	m_MinuteRect.top = 32;
	m_MinuteRect.right = (long)(((dwDuration / 1000) % 3600) * (flt_Width - 50) / 3600);
	m_MinuteRect.bottom = 8;
}

void CTimeLineDlg::InitTimeLine()
{
	OnPaint();
	CPen pen(PS_SOLID, 0, WHITE_TEXT_COLOR);
	CDC* pDC = GetDC();
	pDC->SelectObject(&pen);
	CRect rt;
	GetClientRect(&rt);
	PaintHourMinuteLine();
	SetTimer(TIMELINE_TIMER, 200, NULL);
	m_bTimeFlag = true;
//	pDC->FillSolidRect(m_MinuteRect.left, m_MinuteRect.top, m_MinuteRect.right, m_MinuteRect.bottom, RGB(0, 255, 0));
	m_nLastSelStatic = CNewProjDlg::m_DateTime[0].wHour;
	for(int i = 0; i< 24; i++)
	{
		m_staticTimes[i].SetTextColor(TIME_TEXT_COLOR);
		m_staticTimes[i].SetBkColor(TEXT_BK_COLOR,0, CxStatic::Normal);
	}
	m_staticTimes[m_nLastSelStatic].SetTextColor(WHITE_TEXT_COLOR);
	m_nCurHour = m_nLastSelStatic;
	m_staticTimes[m_nLastSelStatic].SetBkColor(MIN_BK_COLOR_1, 0, CxStatic::Normal);
	GetHourRect(CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr]);
	GetMinuteRect(CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr]);
	pDC->MoveTo(m_HourRect.left, 40);
	pDC->LineTo(m_HourRect.left, rt.Height());
	pDC->MoveTo(m_MinuteRect.left, 17);
	pDC->LineTo(m_MinuteRect.left, 40);
	pen.DeleteObject();
	ReleaseDC(pDC);
}

BOOL CTimeLineDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), TEXT_BK_COLOR);
	ReleaseDC(pDC);
	return TRUE;
}

void CTimeLineDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
 	if (nIDEvent == TIMELINE_TIMER && m_bTimeFlag == true)
 	{
 		OnPaint();
		
 		PaintHourMinuteLine();
 		CRect rt;
 		GetClientRect(&rt);
 		GetHourRect(CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr]);

 		int xHour  = m_HourRect.left + (int)(CNewProjDlg::m_fSeekPos * m_HourRect.right);
 		GetMinuteRect(CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr]);

 		int xMinute = m_MinuteRect.left + (int)(CNewProjDlg::m_fSeekPos * m_MinuteRect.right);
 		CPen pen(PS_SOLID, 0, RED_COLOR);
 		CDC* pDC = GetDC();
 		pDC->SelectObject(&pen);
 		pDC->MoveTo(xHour, 40);
 		pDC->LineTo(xHour, rt.Height());
 		pDC->MoveTo(xMinute, 17);
 		pDC->LineTo(xMinute, 40);
		pen.DeleteObject();
 		ReleaseDC(pDC);
 	}
	
	CDialog::OnTimer(nIDEvent);
	
}

void CTimeLineDlg::PaintHourMinuteLine()
{
	CPen pen(PS_SOLID, 0, WHITE_TEXT_COLOR);
	CDC* pDC = GetDC();
	pDC->SelectObject(&pen);
	CRect rt;

	GetClientRect(&rt);
	pDC->FillSolidRect(48, 32, rt.Width() - 50, 8, RECT_COLOR_1);

	for (int i = 0 ; i < CNewProjDlg::m_nPathCounts; i++)
	{
		GetMinuteRect(CNewProjDlg::m_DateTime[i], CNewProjDlg::m_dwDurations[i]);
		pDC->FillSolidRect(m_MinuteRect.left, m_MinuteRect.top, m_MinuteRect.right, m_MinuteRect.bottom, GREEN_COLOR);
		GetHourRect(CNewProjDlg::m_DateTime[i], CNewProjDlg::m_dwDurations[i]);
		pDC->FillSolidRect(m_HourRect.left, 40, m_HourRect.right, (rt.Height() - 40) / 5 * 2, BLUE_COLOR);
	}
	pen.DeleteObject();
	ReleaseDC(pDC);
}

void CTimeLineDlg::OnDestroy()
{
	delete m_chBuf;
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}
