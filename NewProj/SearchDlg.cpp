// SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "SearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg dialog

//CReportCtrl	   CSearchDlg::m_eventType;
CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchDlg)
	//}}AFX_DATA_INIT
	//m_chBuf = new wchar_t[5];
}


void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchDlg)
	DDX_Control(pDX, IDCANCEL, m_btn_Cancel);
	DDX_Control(pDX, IDOK, m_btn_ok);
	DDX_Control(pDX, IDC_EVENT_TYPE_LSTCNTLR, m_eventType);
	DDX_Control(pDX, IDC_STATIC_SEARCH_BEGIN, m_begin);
	DDX_Control(pDX, IDC_SEARCH_DATE_STATIC, m_static_date);
	DDX_Control(pDX, IDC_SEARCH_END_STATIC, m_static_end);
	DDX_Control(pDX, IDC_SEARCH_PATH_STATIC, m_static_path);
	DDX_Control(pDX, IDC_SEARCH_SEL_EVENT_TYPE_STATIC, m_static_selecteventtype);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(HDN_ITEMCLICK, IDC_EVENT_TYPE_LSTCNTLR, OnItemEventTypeClk)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_EVENT_TYPE_LSTCNTLR, OnColumnclickEventTypeLstcntlr)
	ON_NOTIFY(NM_CLICK, IDC_EVENT_TYPE_LSTCNTLR, OnClickEventTypeLstcntlr)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg message handlers

void CSearchDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rt;
	GetClientRect(&rt);
	dc.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));

	m_static_path.SetTextColor(RGB(200, 200, 200));
	m_static_path.SetFont("Arial", 14, FW_NORMAL);
	m_static_path.FormatW(CNewProjDlg::m_wLanguageRes[177]);
	m_static_path.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_date.SetTextColor(RGB(200, 200, 200));
	m_static_date.SetFont("Arial", 14, FW_NORMAL);
	m_static_date.FormatW(CNewProjDlg::m_wLanguageRes[178]);
	m_static_date.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	

	m_static_end.SetTextColor(RGB(200, 200, 200));
	m_static_end.SetFont("Arial", 14, FW_NORMAL);
	m_static_end.FormatW(CNewProjDlg::m_wLanguageRes[180]);
	m_static_end.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_selecteventtype.SetTextColor(RGB(200, 200, 200));
	m_static_selecteventtype.SetFont("Arial", 14, FW_NORMAL);
	m_static_selecteventtype.FormatW(CNewProjDlg::m_wLanguageRes[17]);
	m_static_selecteventtype.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_begin.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);
	m_begin.SetTextColor(RGB(200, 200, 200));
	m_begin.SetFont("Arial", 14, FW_NORMAL);
	m_begin.FormatW(CNewProjDlg::m_wLanguageRes[179]);
 	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	wchar_t m_chBuf[5];
	memset(m_chBuf, 0, sizeof(m_chBuf));
	HBITMAP hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_OK));
	BITMAP bitmap;
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_ok.SetBitmap(hBmp);

	hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_CANCEL));
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_Cancel.SetBitmap(hBmp);

	CString temp = CNewProjDlg::m_wLanguageRes[18];
	m_eventType.SetHeadings(temp + _T(", 150"));
	m_eventType.SetBkColor(RGB(47,56,66));
	m_eventType.SetTextBkColor(RGB(47,56,66));
	m_eventType.SetTextColor(RGB(255,255,255));
	m_eventType.SetCheckboxes(TRUE);
	
	for (int i = 0; i < 8 ; i++)
	{
		wsprintf(m_chBuf,_T("I/O_%d"),i+1);
		m_eventType.InsertItem(i, (LPCTSTR)(m_chBuf));
	}	
	m_eventType.InsertItem(8, CNewProjDlg::m_wLanguageRes[24]);
	m_eventType.InsertItem(9, CNewProjDlg::m_wLanguageRes[25]);
	m_eventType.InsertItem(10, CNewProjDlg::m_wLanguageRes[26]);
	m_eventType.InsertItem(11, CNewProjDlg::m_wLanguageRes[38]);
	m_eventType.InsertItem(12, CNewProjDlg::m_wLanguageRes[19]);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonUp(nFlags, point);
}

void CSearchDlg::OnItemEventTypeClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CSearchDlg::OnColumnclickEventTypeLstcntlr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CSearchDlg::OnClickEventTypeLstcntlr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	NMLISTVIEW * pLst = (NMLISTVIEW *)(void*)pNMHDR;
	if (m_eventType.GetCheck(pLst->iItem) != 0)
	{
		m_eventType.SetCheck(pLst->iItem,FALSE);
	}
	else
		m_eventType.SetCheck(pLst->iItem,TRUE);
	if (pLst->iItem == 12)
	{
		if (m_eventType.GetCheck(12) != false)
		{
			m_eventType.CheckAllItems();
			m_eventType.SetCheck(12,TRUE);
		}		
		else
			m_eventType.UnCheckAllItems();
	}
	*pResult = 0;
}

void CSearchDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}
