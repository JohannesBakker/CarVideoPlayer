// InfoListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "InfoListDlg.h"
#include "NewProjDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoListDlg dialog

int CInfoListDlg::g_TabCurSel;
CInfoListDlg::CInfoListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoListDlg)
	//}}AFX_DATA_INIT
	m_bInfoList = false;
	m_alarmGraphWnd = NULL;
}


void CInfoListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoListDlg)
//	DDX_Control(pDX, IDC_STATIC_ALARM, m_alarmGraphWnd);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_speedGraphWnd);
	DDX_Control(pDX, IDC_STATIC_ACC, m_accGraphWnd);
	DDX_Control(pDX,IDC_FILE_LIST,m_fileList);
	DDX_Control(pDX,IDC_EVENT_LIST,m_eventList);
	DDX_Control(pDX,IDC_LOG_LIST,m_logList);
	DDX_Control(pDX,IDC_DIAGNOSIS_LIST,m_diagList);
	DDX_Control(pDX,IDC_TAB,m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoListDlg, CDialog)
	//{{AFX_MSG_MAP(CInfoListDlg)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_FILE_LIST, OnDblclkFileList)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoListDlg message handlers

void CInfoListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_bInfoList)
	{
		GetDlgItem(IDC_TAB)->MoveWindow(0, cy - 25, cx, 25);
		GetDlgItem(IDC_FILE_LIST)->MoveWindow(0, 0, cx, cy - 25);
		GetDlgItem(IDC_EVENT_LIST)->MoveWindow(0, 0, cx, cy - 25);
		GetDlgItem(IDC_LOG_LIST)->MoveWindow(0, 0, cx, cy - 25);
		GetDlgItem(IDC_DIAGNOSIS_LIST)->MoveWindow(0, 0, cx, cy - 25);
		GetDlgItem(IDC_STATIC_SPEED)->MoveWindow(0, 0, cx, cy * 2 / 3);
		GetDlgItem(IDC_STATIC_ACC)->MoveWindow(0, cy * 2 / 3, cx, cy / 3 - 25);
		//GetDlgItem(IDC_LIST_ALARM)->MoveWindow(0, 0, cx, cy - 25);		
		//GetDlgItem(IDC_STATIC_ALARM)->MoveWindow(0,0,cx,cy - 25);		
		m_alarmGraphWnd->MoveWindow(0,0,cx,cy - 25);
		m_tab.ShowWindow(SW_SHOW);
	}
}

BOOL CInfoListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect alarmRt;
	GetDlgItem(IDC_STATIC_ALARM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ALARM)->GetWindowRect(&alarmRt);
	m_alarmGraphWnd = new CAlarmGraph(this, alarmRt, IDC_STATIC_ALARM);
	
	// TODO: Add extra initialization here
	CRect rt;
	GetClientRect(&rt);
	m_tab.SetMinTabWidth(rt.Width()/6);
	//char str[100];
	//memset(str, 0, sizeof(str));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[88], str);
	m_tab.InsertItem(0, CNewProjDlg::m_wLanguageRes[88], IDB_BITMAP_BKCOLOR);
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[248], str);
	m_tab.InsertItem(1, CNewProjDlg::m_wLanguageRes[248]);
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[56], str);
	m_tab.InsertItem(2, CNewProjDlg::m_wLanguageRes[56]);
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[20], str);
	m_tab.InsertItem(3, CNewProjDlg::m_wLanguageRes[20]);
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[288], str);
	m_tab.InsertItem(4, CNewProjDlg::m_wLanguageRes[288]);
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[365], str);
	m_tab.InsertItem(5, CNewProjDlg::m_wLanguageRes[365]);
	m_tab.SetCurSel(2);
	
	m_fileList.SetBkColor(RGB(47, 56, 66));
	m_fileList.SetTextBkColor(RGB(47, 56, 66));
	m_fileList.SetTextColor(RGB(255, 255, 255));
	WCHAR chHeader[1000];
	memset(chHeader, 0, sizeof(chHeader));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[57], str);
	wcscpy(chHeader, CNewProjDlg::m_wLanguageRes[57]);
	wcscat(chHeader, _T(",60; "));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[58], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[58]);
	wcscat(chHeader, _T(",60; "));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[67], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[67]);
	wcscat(chHeader, _T(",60; "));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[61], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[61]);
	wcscat(chHeader, _T(",60; "));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[210], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[210]);
	wcscat(chHeader, _T(",60; "));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[59], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[59]);
	wcscat(chHeader, _T(",60; "));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[606], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[606]);
	wcscat(chHeader, _T(",60; "));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[608], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[608]);
	wcscat(chHeader, _T(",60; "));
	m_fileList.SetHeadings((chHeader));
	m_fileList.SetGridLines(TRUE);
	m_fileList.ShowWindow(SW_HIDE);
	
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[22], str);
	wcscpy(chHeader, CNewProjDlg::m_wLanguageRes[22]);
	wcscat(chHeader, _T(",160; "));
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[18], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[18]);
	wcscat(chHeader, _T(",180; "));
	m_eventList.SetBkColor(RGB(47, 56, 66));
	m_eventList.SetTextBkColor(RGB(47, 56, 66));
	m_eventList.SetTextColor(RGB(255,255,255));
	m_eventList.SetHeadings((chHeader));
	m_eventList.SetGridLines(TRUE);	
	m_eventList.ShowWindow(SW_HIDE);
	
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[675], str);
	wcscpy(chHeader, CNewProjDlg::m_wLanguageRes[675]);
	wcscat(chHeader, _T(",60; "));
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[672], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[672]);
	wcscat(chHeader, _T(",90; "));
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[673], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[673]);
	wcscat(chHeader, _T(",50; "));
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[674], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[674]);
	wcscat(chHeader, _T(",100; "));
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[670], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[670]);
	wcscat(chHeader, _T(",60; "));
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[671], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[671]);
	wcscat(chHeader, _T(",60; "));
	m_logList.SetBkColor(RGB(47, 56, 66));
	m_logList.SetTextBkColor(RGB(47, 56, 66));
	m_logList.SetTextColor(RGB(255,255,255));
	m_logList.SetHeadings((chHeader));
	m_logList.SetGridLines(TRUE);
	m_logList.ShowWindow(SW_HIDE);
	
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[675], str);
	wcscpy(chHeader, CNewProjDlg::m_wLanguageRes[675]);
	wcscat(chHeader, _T(",120; "));
//	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[365], str);
	wcscat(chHeader, CNewProjDlg::m_wLanguageRes[365]);
	wcscat(chHeader, _T(",100; "));
	m_diagList.SetBkColor(RGB(47, 56, 66));
	m_diagList.SetTextBkColor(RGB(47, 56, 66));
	m_diagList.SetTextColor(RGB(255,255,255));
	m_diagList.SetHeadings((chHeader));
	m_diagList.SetGridLines(TRUE);
	m_diagList.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ACC)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_STATIC_ALARM)->ShowWindow(SW_HIDE);
	m_alarmGraphWnd->ShowWindow(SW_HIDE);
	m_fileList.ShowWindow(SW_SHOW);
	m_eventList.ShowWindow(SW_HIDE);
	m_logList.ShowWindow(SW_HIDE);
	m_diagList.ShowWindow(SW_HIDE);

	m_accGraphWnd.Invalidate();
	g_TabCurSel = m_tab.GetCurSel();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInfoListDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
 	return CDialog::OnEraseBkgnd(pDC);
}

void CInfoListDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	//m_fileList.SetBkColor(RGB(47, 56, 66));
// 	HBITMAP hBmp;
// 	HINSTANCE hInst;
// 	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
// 	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
// 	m_eventList.SetBkImage(hBmp,false,100,100);
// 	char chBuf[6];
// 	CString buf;
// 	CFont font;
// 	VERIFY(font.CreateFont(
// 		14,                        // nHeight
// 		0,                         // nWidth
// 		0,                         // nEscapement
// 		0,                         // nOrientation
// 		FW_NORMAL,                 // nWeight
// 		FALSE,                     // bItalic
// 		FALSE,                     // bUnderline
// 		0,                         // cStrikeOut
// 		ANSI_CHARSET,              // nCharSet
// 		OUT_DEFAULT_PRECIS,        // nOutPrecision
// 		CLIP_DEFAULT_PRECIS,       // nClipPrecision
// 		DEFAULT_QUALITY,           // nQuality
// 		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
// 		"Arial"));                 // lpszFacename
// 	// TODO: Add your message handler code here
// 	CRect rt;
// 	CDC* pDC = GetDC();
// 	GetClientRect(&rt);
//	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 57, 66));
// 	pDC = GetDlgItem(IDC_STATIC_SPEED)->GetDC();
// 	GetDlgItem(IDC_STATIC_SPEED)->GetClientRect(&rt);
// 	CPen pen(PS_SOLID, 0, RGB(255, 255, 255));
// 	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 57, 66));
// 	pDC->SelectObject(&pen);
// 	pDC->SelectObject(&font);
// 	pDC->SetBkColor(RGB(47, 57, 66));
// 	pDC->SetTextColor(RGB(255, 255, 255));
// 	pDC->ExtTextOut(23, (rt.Height() - 5) / 2 - 10, ETO_OPAQUE, CRect(0, 0 , 0, 0), "Speed", NULL);
// 	for(int i = 0; i < 7; i ++)
// 	{
// 		for(int j = 130; j < rt.Width(); j += 5)
// 		{	
// 			pDC->MoveTo(j, rt.Height() - 10 - i * (rt.Height() - 20) / 6);
// 			pDC->LineTo(j + 2, rt.Height() - 10 - i * (rt.Height() - 20) / 6);
// 		}
// 		itoa(i * 20, chBuf, 10);
// 		buf = chBuf;
// 		buf += "km/h";
// 		pDC->ExtTextOut(90, rt.Height() - 18 - i * (rt.Height() - 20) / 6, ETO_OPAQUE, CRect(0, 0, 0, 0), buf, NULL);
// 	}
// 	for(i = 1; i < 5; i++)
// 	{
// 		for(int j = 10; j < rt.Height() - 10; j += 5)
// 		{
// 			pDC->MoveTo(130 + i * (rt.Width() - 130) / 5, j);
// 			pDC->LineTo(130 + i * (rt.Width() - 130) / 5, j + 2);
// 		}
// 	}
// 	pDC->MoveTo(80, 0);
// 	pDC->LineTo(80, rt.Height());
// 	pDC->MoveTo(0, rt.Height() - 5);
// 	pDC->LineTo(rt.Width(), rt.Height() - 5);
// 
// 	pDC = GetDlgItem(IDC_STATIC_ACC)->GetDC();
// 	GetDlgItem(IDC_STATIC_ACC)->GetClientRect(&rt);
// 	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 57, 66));
// 	pDC->SelectObject(&pen);
// 	pDC->SelectObject(&font);
// 	pDC->SetBkColor(RGB(47, 57, 66));
// 	pDC->SetTextColor(RGB(255, 255, 255));
// 	pDC->ExtTextOut(10, (rt.Height() - 5) / 2 - 10, ETO_OPAQUE, CRect(0, 0 , 0, 0), "Acceleration", NULL);
// 	for(i = 0; i < 5; i ++)
// 	{
// 		for(int j = 130; j < rt.Width(); j += 5)
// 		{	
// 			pDC->MoveTo(j, rt.Height() - 10 - i * (rt.Height() - 15) / 4);
// 			pDC->LineTo(j + 2, rt.Height() - 10 - i * (rt.Height() - 15) / 4);
// 		}
// 		itoa(i - 2, chBuf, 10);
// 		buf = chBuf;
// 		buf += "G";
// 		pDC->ExtTextOut(100, rt.Height() - 17 - i * (rt.Height() - 15) / 4, ETO_OPAQUE, CRect(0, 0, 0, 0), buf, NULL);
// 	}
// 	for(i = 1; i < 5; i++)
// 	{
// 		for(int j = 5; j < rt.Height() - 10; j += 5)
// 		{
// 			pDC->MoveTo(130 + i * (rt.Width() - 130) / 5, j);
// 			pDC->LineTo(130 + i * (rt.Width() - 130) / 5, j + 2);
// 		}
// 	}
// 	pDC->MoveTo(80, 0);
// 	pDC->LineTo(80, rt.Height());
// 	pDC->MoveTo(0, rt.Height() - 5);
// 	pDC->LineTo(rt.Width(), rt.Height() - 5);

// 	pDC = GetDlgItem(IDC_STATIC_ALARM)->GetDC();
// 	GetDlgItem(IDC_STATIC_ALARM)->GetClientRect(&rt);
// 	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 57, 66));
// //	pDC->SelectObject(&pen);
// 	pDC->MoveTo(80, 0);
// 	pDC->LineTo(80, rt.Height());
// 	pDC->SelectObject(&font);
// 	pDC->SetBkColor(RGB(47, 57, 66));
// 	pDC->SetTextColor(RGB(255, 255, 255));
// 	for(int i = 0; i < 8; i++)
// 	{
// 		for(int j = 130; j < rt.Width(); j += 5)
// 		{
// 			pDC->MoveTo(j, (rt.Height() - 10) / 10 * i + (rt.Height() - 10) / 20);
// 			pDC->LineTo(j + 2, (rt.Height() - 10) / 10 * i + (rt.Height() - 10) / 20);
// 		}
// 		itoa(i, chBuf, 10);
// 		buf = chBuf;
// 		buf = "IO/" + buf;
// 		pDC->ExtTextOut(30, (rt.Height() - 10) / 10 * i + (rt.Height() - 10) / 20 - 7, ETO_OPAQUE, CRect(0, 0, 0, 0), buf, NULL);
// 		pDC->ExtTextOut(90, (rt.Height() - 10) / 10 * i, ETO_OPAQUE, CRect(0, 0, 0, 0), "  Signal", NULL);
// 		pDC->ExtTextOut(90, (rt.Height() - 10) / 10 * (i + 1) - 14, ETO_OPAQUE, CRect(0, 0, 0, 0), "NonSig", NULL);
// 	}
// 	for(i = 0; i < 11; i++)
// 	{
// 		pDC->MoveTo(0, (rt.Height() - 10) / 10 * i);
// 		pDC->LineTo(rt.Width(), (rt.Height() - 10) / 10 * i);
// 	}

// 	m_fileList.Init();
// 	m_eventList.Init();
// 	m_logList.Init();
// 	m_diagList.Init();
// 	
// 	m_fileList.SetBkColor(RGB(47, 56, 66));
// 	m_fileList.SetTextColor(RGB(222,222,222));
// 	
// 	m_eventList.SetBkColor(RGB(47, 56, 66));
// 	m_eventList.SetTextColor(RGB(222,222,222));
// 	
// 	m_logList.SetBkColor(RGB(47, 56, 66));
// 	m_logList.SetTextColor(RGB(222,222,222));
// 	
// 	m_diagList.SetBkColor(RGB(47, 56, 66));
// 	m_diagList.SetTextColor(RGB(222,222,222));
// 	
// 	LOGFONT lf;
// 	memset(&lf, 0, sizeof(LOGFONT));
// 	lf.lfHeight = 15;
// 	strcpy(lf.lfFaceName, "Arial");
// 	m_font.CreateFontIndirect(&lf);
// 	
// 	m_fileList.SetFont(&m_font, TRUE);
// 	m_eventList.SetFont(&m_font, TRUE);
// 	m_logList.SetFont(&m_font,	TRUE);
// 	m_diagList.SetFont(&m_font,TRUE);
// 	
// 	
// 	m_fileList.InsertColumn(0,	"Date", LVCFMT_LEFT, 60);
// 	m_fileList.InsertColumn(1,	"Start Time", LVCFMT_LEFT, 60);
// 	m_fileList.InsertColumn(2,	"End Time", LVCFMT_LEFT, 60);
// 	m_fileList.InsertColumn(3,	"Duration", LVCFMT_LEFT, 60);
// 	m_fileList.InsertColumn(4,  "File Size", LVCFMT_LEFT, 60);
// 	m_fileList.InsertColumn(5,	"Description", LVCFMT_LEFT, 60);
// 	m_fileList.InsertColumn(6,	"Channel Count", LVCFMT_LEFT, 60);
// 	m_fileList.InsertColumn(7,	"Use State", LVCFMT_LEFT, 60);
// 	
// 	// 	m_eventList.SetHeadings(_T("Event Time, 120; Event Type, 100"));
// 	
// 	m_eventList.InsertColumn(0,	"Event Time", LVCFMT_LEFT, 120);
// 	m_eventList.InsertColumn(1,	"Event Type", LVCFMT_LEFT, 100);
// 	
// 	m_logList.InsertColumn(0,	"Date", LVCFMT_LEFT, 60);
// 	m_logList.InsertColumn(1,	"Software user name", LVCFMT_LEFT, 90);
// 	m_logList.InsertColumn(2,	"Event", LVCFMT_LEFT, 50);
// 	m_logList.InsertColumn(3,	"Detail", LVCFMT_LEFT, 100);
// 	m_logList.InsertColumn(4,  "ComputerName", LVCFMT_LEFT, 60);
// 	m_logList.InsertColumn(5,	"User Name", LVCFMT_LEFT, 60);
// 	
// 	m_diagList.InsertColumn(0,	"Date", LVCFMT_LEFT, 120);
// 	m_diagList.InsertColumn(1,	"Diagnosis", LVCFMT_LEFT, 100);
// 	
	
//	m_fileList.SetRedraw(FALSE);
	// 	CString cszItem;
	// 	for(int i=0; i<8000; i++)
	// 	{
	// 		cszItem.Format("%d - %s", i, "Matthew Good - Near Fantastica");
	// 		
	// 		m_SkinList.InsertItem(i, cszItem);
	// 		m_SkinList.SetItemText(i, 1, cszItem);
	// 		m_SkinList.SetItemText(i, 2, "Matthew Good");
	// 		m_SkinList.SetItemText(i, 3, "Rock");
	// 	}
	// 	
//	m_fileList.SetRedraw(TRUE);
	
//	ListView_SetExtendedListViewStyle(m_fileList.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
//	ListView_SetExtendedListViewStyle(m_eventList.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CInfoListDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
 	CDC *pDC = GetDC();
	CRect rt;
	GetClientRect(&rt);
//	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 57, 66));
	switch (m_tab.GetCurSel())
 	{
	case 0:
		GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ACC)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_STATIC_ALARM)->ShowWindow(SW_HIDE);
		m_alarmGraphWnd->ShowWindow(SW_HIDE);
		m_fileList.ShowWindow(SW_HIDE);
		m_eventList.ShowWindow(SW_HIDE);
		m_logList.ShowWindow(SW_HIDE);
		m_diagList.ShowWindow(SW_HIDE);		
		break;
	case 1:
		GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACC)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_ALARM)->ShowWindow(SW_SHOW);
		m_alarmGraphWnd->ShowWindow(SW_SHOW);
		m_fileList.ShowWindow(SW_HIDE);
		m_eventList.ShowWindow(SW_HIDE);
		m_logList.ShowWindow(SW_HIDE);
		m_diagList.ShowWindow(SW_HIDE);
		break;
	case 2:
		GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACC)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_ALARM)->ShowWindow(SW_HIDE);
		m_alarmGraphWnd->ShowWindow(SW_HIDE);
		m_fileList.ShowWindow(SW_SHOW);
		m_eventList.ShowWindow(SW_HIDE);
		m_logList.ShowWindow(SW_HIDE);
		m_diagList.ShowWindow(SW_HIDE);
		
		break;
	case 3:
		
		GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACC)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_ALARM)->ShowWindow(SW_HIDE);
		m_alarmGraphWnd->ShowWindow(SW_HIDE);
		m_fileList.ShowWindow(SW_HIDE);
		m_eventList.ShowWindow(SW_SHOW);
		m_logList.ShowWindow(SW_HIDE);
		m_diagList.ShowWindow(SW_HIDE);
		//m_fileList.ShowWindow(SW_HIDE);
		
		break;
	case 4:
		
		GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACC)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_ALARM)->ShowWindow(SW_HIDE);
		m_alarmGraphWnd->ShowWindow(SW_HIDE);
		m_fileList.ShowWindow(SW_HIDE);
		m_eventList.ShowWindow(SW_HIDE);
		m_logList.ShowWindow(SW_SHOW);
		m_diagList.ShowWindow(SW_HIDE);
		break;
	case 5:
		
		GetDlgItem(IDC_STATIC_SPEED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACC)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_ALARM)->ShowWindow(SW_HIDE);
		m_alarmGraphWnd->ShowWindow(SW_HIDE);
		m_fileList.ShowWindow(SW_HIDE);
		m_eventList.ShowWindow(SW_HIDE);
		m_logList.ShowWindow(SW_HIDE);
		m_diagList.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}	
	g_TabCurSel = m_tab.GetCurSel();
	*pResult = 0;
}

void CInfoListDlg::DrawGraph(MainBinaryData* pData,DWORD curPos,DWORD dura,DWORD nBinSize,DWORD totalSize,bool bSeek, 
		DWORD dwFristDTS, DWORD currDTS, unsigned char alarmFlag)
{
	switch(nBinSize)
	{
	case 24:
		DrawAccelSpeedGraph( pData, curPos,  dura, totalSize);
		break;

	case 60:
		DrawAccelSpeedGraph( pData, curPos,  dura, totalSize);
		DrawSpeedGraph( pData, curPos,  dura, totalSize,bSeek);
		DrawAlarmGraph(dwFristDTS, currDTS, &pData[nBinSize - 1], alarmFlag);
		break;
	default:
		return;
	}
}



void CInfoListDlg::DrawAccelSpeedGraph(MainBinaryData* pData,DWORD curPos,DWORD dura,DWORD totalSize)
{
	m_accGraphWnd.setDatas(dura,m_pBinData,m_BinDataCount);
	m_accGraphWnd.Invalidate();
}
void CInfoListDlg::DrawSpeedGraph(MainBinaryData* pData,DWORD curPos,DWORD dura, DWORD totalSize,bool bSeek)
{
	m_speedGraphWnd.setDatas(dura,m_pBinData,m_BinDataCount);
	m_speedGraphWnd.Invalidate();

}

void CInfoListDlg::DrawAlarmGraph(DWORD dwFristDTS, DWORD currDTS, MainBinaryData* pData, unsigned char alarmFlag)
{
	m_alarmGraphWnd->setDatasWithTime(dwFristDTS, currDTS, pData, alarmFlag);
	//m_alarmGraphWnd->Invalidate();
}



void CInfoListDlg::ClearAlarmGraph()
{

	m_alarmGraphWnd->ClearAlarmGraph();
}

void CInfoListDlg::load_event_list(CString infoFile,SYSTEMTIME dateTime)
{
	m_eventList.DeleteAllItems();

	
	Hi264Time eventTime;
	WCHAR time[256];
	BYTE temp[42];
	int i=0;
	
	CFile eventFile(infoFile,CFile::modeRead);
	eventFile.Seek(32,CFile::begin);
	DWORD dwSeekPos = 32;
	do{
		eventFile.Seek(dwSeekPos, CFile::begin);
		eventFile.Read(&eventTime,sizeof(Hi264Time));
		eventFile.Seek(dwSeekPos + 6, CFile::begin);
		eventFile.Read(temp,42);
		dwSeekPos += 48;
		int a = temp[7];
		if ((eventTime.year + 2000)==(int)CNewProjDlg::m_FolderTime.wYear && eventTime.month == (int)CNewProjDlg::m_FolderTime.wMonth && eventTime.day ==(int)CNewProjDlg::m_FolderTime.wDay)		
		{
			switch(a){
			case 0x01:
				wsprintf(time, _T(" %d-%02d-%02d    %02d:%02d:%02d"),eventTime.year + 2000,eventTime.month,eventTime.day,eventTime.hour,eventTime.minute,eventTime.second);
				m_eventList.InsertItem(i, time, _T("Lights"));
				break;
			case 0x02:
				wsprintf(time, _T(" %d-%02d-%02d    %02d:%02d:%02d"),eventTime.year + 2000,eventTime.month,eventTime.day,eventTime.hour,eventTime.minute,eventTime.second);
				m_eventList.InsertItem(i, time, _T("MAN"));
				break;
			case 0x04:
				wsprintf(time, _T(" %d-%02d-%02d    %02d:%02d:%02d"),eventTime.year + 2000,eventTime.month,eventTime.day,eventTime.hour,eventTime.minute,eventTime.second);
				m_eventList.InsertItem(i, time, _T("2.4"));
				break;
			case 0x08:
				wsprintf(time, _T(" %d-%02d-%02d    %02d:%02d:%02d"),eventTime.year + 2000,eventTime.month,eventTime.day,eventTime.hour,eventTime.minute,eventTime.second);
				m_eventList.InsertItem(i, time, _T("INPUT4"));
				break;
			case 0x10:	
				wsprintf(time, _T(" %d-%02d-%02d    %02d:%02d:%02d"),eventTime.year + 2000,eventTime.month,eventTime.day,eventTime.hour,eventTime.minute,eventTime.second);
				m_eventList.InsertItem(i, time, _T("INPUT5"));
				break;
			case 0x20:
				wsprintf(time, _T(" %d-%02d-%02d    %02d:%02d:%02d"),eventTime.year + 2000,eventTime.month,eventTime.day,eventTime.hour,eventTime.minute,eventTime.second);
				m_eventList.InsertItem(i, time, _T("INPUT6"));
				break;
			case 0x40:
				wsprintf(time, _T(" %d-%02d-%02d    %02d:%02d:%02d"),eventTime.year + 2000,eventTime.month,eventTime.day,eventTime.hour,eventTime.minute,eventTime.second);
				m_eventList.InsertItem(i, time, _T("I/O 7"));
				break;
			case 0x80:
				wsprintf(time, _T(" %d-%02d-%02d    %02d:%02d:%02d"),eventTime.year + 2000,eventTime.month,eventTime.day,eventTime.hour,eventTime.minute,eventTime.second);
				m_eventList.InsertItem(i, time, _T("I/O 8"));
				break;
			default:
				break;
			}
			i++;
		}
	} while(eventFile.GetPosition()<eventFile.GetLength());
	
	eventFile.Close();
	
}

void CInfoListDlg::load_file_list(DWORD duration, SYSTEMTIME dateTime, int chanelCount, DWORD len1, DWORD len2, int nListIndex)
{
	m_fileList.SetRedraw(TRUE);
	WCHAR chDay[256];
	WCHAR chSTime[100];
	WCHAR chETime[100];
	WCHAR chDuration[100];
	WCHAR chFileSize[100];
	memset(chFileSize, 0 , sizeof(chFileSize));
	WCHAR chDescription[100];
	WCHAR chChannel[100];
	WCHAR chUsed[100];
	wsprintf(chDay, _T(" %04d-%02d-%02d \n"),	dateTime.wYear,	dateTime.wMonth, dateTime.wDay);
	wsprintf(chSTime, _T(" %02d:%02d:%02d"), dateTime.wHour, dateTime.wMinute, dateTime.wSecond);
	DWORD secondTemp =  dateTime.wSecond + duration / 1000;
	int minute = secondTemp / 60;
	if(minute)
		minute += dateTime.wMinute;
	secondTemp %= 60;
	wsprintf(chETime, _T(" %02d:%02d:%02d"), dateTime.wHour, minute, secondTemp-1);
	wsprintf(chDuration, _T(" %02d:%02d"), (duration / 1000 - 1) / 60, (duration / 1000  -1) % 60);
	float fileSize =(float) (len1 + len2) / 1024 / 1024;
	int nUp = (int)fileSize;
	fileSize = (fileSize - nUp) * 100;
	int nDown = (int)fileSize;
	wsprintf(chFileSize, _T("%d.%dM"), nUp, nDown);
	wsprintf(chDescription, _T("%s"), _T("Alarm"));
	wsprintf(chChannel, _T(" %d"), chanelCount);
	wsprintf(chUsed, _T("%s") , _T(""));
	m_fileList.InsertItem(0, chDay, chSTime, chETime, chDuration, chFileSize, chDescription, chChannel, chUsed);

	//m_fileList.SetRedraw(TRUE);
}

void CInfoListDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetWindowRect(&rt);
	if(m_tab.GetCurSel() == 2)
	{
		CMenu menu; 
		menu.LoadMenu(IDR_BATCH_CONVERT);	
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, rt.left + point.x, rt.top + point.y , this, 0);
	}
	CDialog::OnRButtonDown(nFlags, point);
}


void CInfoListDlg::OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < m_fileList.GetItemCount(); i++)
	{
		if(m_fileList.GetItemState(i, 1) == 1) CNewProjDlg::m_nPathArraysPtr = i;
	}
	CNewProjDlg::g_bResetFlag = true;
	for(int i = 0 ; i < CNewProjDlg::m_nPathCounts; i++)
		m_fileList.UnSelectItem(i);
	m_fileList.SelectItem(CNewProjDlg::m_nPathArraysPtr, true);
	m_fileList.SetItemText(CNewProjDlg::m_nPathArraysPtr, 7, _T("Played"));
	*pResult = 0;
}

void CInfoListDlg::OnDestroy()
{
	CDialog::OnDestroy();
	delete m_alarmGraphWnd;
	// TODO: Add your message handler code here
}

