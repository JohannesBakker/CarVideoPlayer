// TimeRangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "TimeRangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeRangeDlg dialog


CTimeRangeDlg::CTimeRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeRangeDlg)
	//}}AFX_DATA_INIT
	
}


void CTimeRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeRangeDlg)
	DDX_Control(pDX, IDC_CANCEL, m_btn_Cancel);
	DDX_Control(pDX, IDC_OK, m_btn_Ok);
	DDX_Control(pDX, IDC_END_YEAR_EDIT, m_editEndYear);
	DDX_Control(pDX, IDC_END_SECOND_EDIT, m_editEndSec);
	DDX_Control(pDX, IDC_END_MONTH_EDIT, m_editEndMonth);
	DDX_Control(pDX, IDC_END_MINUTE_EDIT, m_editEndMinu);
	DDX_Control(pDX, IDC_END_HOUR_EDIT, m_editEndHour);
	DDX_Control(pDX, IDC_END_DAY_EDIT, m_editEndDay);
	DDX_Control(pDX, IDC_BEGIN_YEAR_EDIT, m_editBeginYear);
	DDX_Control(pDX, IDC_BEGIN_SECOND_EDIT, m_editBeginSec);
	DDX_Control(pDX, IDC_BEGIN_MONTH_EDIT, m_editBeginMonth);
	DDX_Control(pDX, IDC_BEGIN_MINUTE_EDIT, m_editBeginMinu);
	DDX_Control(pDX, IDC_BEGIN_HOUR_EDIT, m_editBeginHour);
	DDX_Control(pDX, IDC_BEGIN_DAY_EDIT, m_editBeginDay);
	DDX_Control(pDX, IDC_RANGE_END_STATIC, m_static_rangeend);
	DDX_Control(pDX, IDC_RANGE_BEGIN_STATIC, m_static_rangebegin);
	DDX_Control(pDX, IDC_END_YEAR_STATIC, m_static_endyear);
	DDX_Control(pDX, IDC_END_SECOND_STATIC, m_static_endsecond);
	DDX_Control(pDX, IDC_END_MONTH_STATIC, m_static_endmonth);
	DDX_Control(pDX, IDC_END_MINUTE_STATIC, m_static_endminute);
	DDX_Control(pDX, IDC_END_HOUR_STATIC, m_static_endhour);
	DDX_Control(pDX, IDC_END_DAY_STATIC, m_static_endday);
	DDX_Control(pDX, IDC_BEGIN_YEAR_STATIC, m_static_beginyear);
	DDX_Control(pDX, IDC_BEGIN_SECOND_STATIC, m_static_beginsecond);
	DDX_Control(pDX, IDC_BEGIN_MONTH_STATIC, m_static_beginmonth);
	DDX_Control(pDX, IDC_BEGIN_MINUTE_STATIC, m_static_beginminute);
	DDX_Control(pDX, IDC_BEGIN_HOUR_STATIC, m_static_beginhour);
	DDX_Control(pDX, IDC_BEGIN_DAY_STATIC, m_static_beginday);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CTimeRangeDlg)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeRangeDlg message handlers

void CTimeRangeDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CTimeRangeDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	if(m_bRecordCurFlag == true)
	{
		wchar_t		szFilter[]= _T("AVI Files(*.avi)|*.avi|");
		CFileDialog	dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);
		CString path;
		if(dlg.DoModal() ==IDOK)
		{
			path = dlg.GetPathName();
			path += _T(".avi");
			m_ProgressDlg.m_AVIFilePath = path;
			if(CNewProjDlg::g_config_Value_ST.nWndCounts == 2)
				m_ProgressDlg.Init_AVI_Convert(m_1stFile, m_2ndFile, m_flt_1stStartPos, m_flt_1stEndPos, m_flt_2ndStartPos, m_flt_2ndEndPos, m_flt_2ndStopPos);
			else if(CNewProjDlg::g_config_Value_ST.nWndCounts == 1 && CNewProjDlg::g_config_Value_ST.nSelWndID == 1)
				m_ProgressDlg.Init_AVI_Convert(m_1stFile, NULL, m_flt_1stStartPos, m_flt_1stEndPos, 0, 0, 0);
			else if(CNewProjDlg::g_config_Value_ST.nWndCounts == 1 && CNewProjDlg::g_config_Value_ST.nSelWndID == 2)
				m_ProgressDlg.Init_AVI_Convert(NULL, m_2ndFile, 0, 0, m_flt_2ndStartPos, m_flt_2ndEndPos, m_flt_2ndEndPos);
		}
		m_ProgressDlg.ShowWindow(SW_SHOW | SW_SHOWNORMAL);
	}else{
		m_ProgressDlg.m_1stDestFilePath = m_1stFilePath;
		m_ProgressDlg.m_2ndDestFilePath = m_2ndFilePath;
		if(CNewProjDlg::g_config_Value_ST.nWndCounts == 2)
			m_ProgressDlg.Init_Progress(m_1stFile, m_2ndFile, m_dw1stStartPos, m_dw1stEndPos, m_dw2ndStartPos, m_dw2ndEndPos);
		else if(CNewProjDlg::g_config_Value_ST.nWndCounts == 1)
			m_ProgressDlg.Init_Progress(m_1stFile, NULL, m_dw1stStartPos, m_dw1stEndPos, 0, 0);
	}
	CDialog::OnOK();
}
void CTimeRangeDlg::UpdateWnd()
{
	wchar_t str[10];
	char	temp[10];
	itoa(m_StartDateTime.wYear, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_YEAR_EDIT, str);
	itoa(m_StartDateTime.wMonth, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_MONTH_EDIT, str);
	itoa(m_StartDateTime.wDay, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_DAY_EDIT, str);
	itoa(m_StartDateTime.wHour, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_HOUR_EDIT, str);
	itoa(m_StartDateTime.wMinute, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_MINUTE_EDIT, str);
	itoa(m_StartDateTime.wSecond, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_SECOND_EDIT, str);

	itoa(m_EndDateTime.wYear, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_YEAR_EDIT, str);
	itoa(m_EndDateTime.wMonth, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_MONTH_EDIT, str);
	itoa(m_EndDateTime.wDay, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_DAY_EDIT, str);
	itoa(m_EndDateTime.wHour, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_HOUR_EDIT, str);
	itoa(m_EndDateTime.wMinute, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_MINUTE_EDIT, str);
	itoa(m_EndDateTime.wSecond, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_SECOND_EDIT, str);
}
BOOL CTimeRangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_ProgressDlg.Create(IDD_PROGRESS_DIALOG, this);

	HBITMAP hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_OK));
	m_btn_Ok.SetBitmap(hBmp);
	hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_CANCEL));
	m_btn_Cancel.SetBitmap(hBmp);

	wchar_t str[10];
	char	temp[10];
	itoa(m_StartDateTime.wYear, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_YEAR_EDIT, str);
	itoa(m_StartDateTime.wMonth, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_MONTH_EDIT, str);
	itoa(m_StartDateTime.wDay, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_DAY_EDIT, str);
	itoa(m_StartDateTime.wHour, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_HOUR_EDIT, str);
	itoa(m_StartDateTime.wMinute, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_MINUTE_EDIT, str);
	itoa(m_StartDateTime.wSecond, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_BEGIN_SECOND_EDIT, str);

	itoa(m_EndDateTime.wYear, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_YEAR_EDIT, str);
	itoa(m_EndDateTime.wMonth, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_MONTH_EDIT, str);
	itoa(m_EndDateTime.wDay, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_DAY_EDIT, str);
	itoa(m_EndDateTime.wHour, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_HOUR_EDIT, str);
	itoa(m_EndDateTime.wMinute, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_MINUTE_EDIT, str);
	itoa(m_EndDateTime.wSecond, temp, 10);
	Ascii2Unichar(temp, str);
	SetDlgItemText(IDC_END_SECOND_EDIT, str);

	m_editBeginYear.SetTextColor(RGB( 255, 255, 255));
	m_editBeginMonth.SetTextColor(RGB(255, 255, 255));
	m_editBeginDay.SetTextColor(RGB( 255, 255, 255));
	m_editBeginHour.SetTextColor(RGB(255, 255, 255));
	m_editBeginMinu.SetTextColor(RGB( 255, 255, 255));
	m_editBeginSec.SetTextColor(RGB(255, 255, 255));
	
	m_editEndYear.SetTextColor(RGB( 255, 255, 255));
	m_editEndMonth.SetTextColor(RGB(255, 255, 255));
	m_editEndDay.SetTextColor(RGB( 255, 255, 255));
	m_editEndHour.SetTextColor(RGB(255, 255, 255));
	m_editEndMinu.SetTextColor(RGB( 255, 255, 255));
	m_editEndSec.SetTextColor(RGB(255, 255, 255));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTimeRangeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
	CRect rt;
	GetClientRect(&rt);
	dc.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));

	m_static_rangebegin.SetTextColor(RGB(200, 200, 200));
	m_static_rangebegin.SetFont("Arial", 14, FW_NORMAL);
	m_static_rangebegin.FormatW(CNewProjDlg::m_wLanguageRes[331]);
	m_static_rangebegin.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_rangeend.SetTextColor(RGB(200, 200, 200));
	m_static_rangeend.SetFont("Arial", 14, FW_NORMAL);
	m_static_rangeend.FormatW(CNewProjDlg::m_wLanguageRes[332]);
	m_static_rangeend.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_beginyear.SetTextColor(RGB(200, 200, 200));
	m_static_beginyear.SetFont("Arial", 14, FW_NORMAL);
	m_static_beginyear.FormatW(CNewProjDlg::m_wLanguageRes[333]);
	m_static_beginyear.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_beginmonth.SetTextColor(RGB(200, 200, 200));
	m_static_beginmonth.SetFont("Arial", 14, FW_NORMAL);
	m_static_beginmonth.FormatW(CNewProjDlg::m_wLanguageRes[334]);
	m_static_beginmonth.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_beginday.SetTextColor(RGB(200, 200, 200));
	m_static_beginday.SetFont("Arial", 14, FW_NORMAL);
	m_static_beginday.FormatW(CNewProjDlg::m_wLanguageRes[335]);
	m_static_beginday.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_beginhour.SetTextColor(RGB(200, 200, 200));
	m_static_beginhour.SetFont("Arial", 14, FW_NORMAL);
	m_static_beginhour.FormatW(CNewProjDlg::m_wLanguageRes[336]);
	m_static_beginhour.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_beginminute.SetTextColor(RGB(200, 200, 200));
	m_static_beginminute.SetFont("Arial", 14, FW_NORMAL);
	m_static_beginminute.FormatW(CNewProjDlg::m_wLanguageRes[337]);
	m_static_beginminute.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_beginsecond.SetTextColor(RGB(200, 200, 200));
	m_static_beginsecond.SetFont("Arial", 14, FW_NORMAL);
	m_static_beginsecond.FormatW(CNewProjDlg::m_wLanguageRes[338]);
	m_static_beginsecond.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_endyear.SetTextColor(RGB(200, 200, 200));
	m_static_endyear.SetFont("Arial", 14, FW_NORMAL);
	m_static_endyear.FormatW(CNewProjDlg::m_wLanguageRes[333]);
	m_static_endyear.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_endmonth.SetTextColor(RGB(200, 200, 200));
	m_static_endmonth.SetFont("Arial", 14, FW_NORMAL);
	m_static_endmonth.FormatW(CNewProjDlg::m_wLanguageRes[334]);
	m_static_endmonth.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_endday.SetTextColor(RGB(200, 200, 200));
	m_static_endday.SetFont("Arial", 14, FW_NORMAL);
	m_static_endday.FormatW(CNewProjDlg::m_wLanguageRes[335]);
	m_static_endday.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_endhour.SetTextColor(RGB(200, 200, 200));
	m_static_endhour.SetFont("Arial", 14, FW_NORMAL);
	m_static_endhour.FormatW(CNewProjDlg::m_wLanguageRes[336]);
	m_static_endhour.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_endminute.SetTextColor(RGB(200, 200, 200));
	m_static_endminute.SetFont("Arial", 14, FW_NORMAL);
	m_static_endminute.FormatW(CNewProjDlg::m_wLanguageRes[337]);
	m_static_endminute.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_endsecond.SetTextColor(RGB(200, 200, 200));
	m_static_endsecond.SetFont("Arial", 14, FW_NORMAL);
	m_static_endsecond.FormatW(CNewProjDlg::m_wLanguageRes[338]);
	m_static_endsecond.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);
	
	// Do not call CDialog::OnPaint() for painting messages
}
