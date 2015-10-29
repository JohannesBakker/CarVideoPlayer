// CalendarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "CalendarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalendarDlg dialog


CCalendarDlg::CCalendarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalendarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalendarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCalendarDlg = FALSE;
}


void CCalendarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalendarDlg)
	DDX_Control(pDX, IDC_CALENDAR, m_calendar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalendarDlg, CDialog)
	//{{AFX_MSG_MAP(CCalendarDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalendarDlg message handlers

BOOL CCalendarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rt;
	GetClientRect(&rt);
	GetDlgItem(IDC_CALENDAR)->MoveWindow(0, 0, rt.Width(), rt.Height());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalendarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(m_bCalendarDlg)
		GetDlgItem(IDC_CALENDAR)->MoveWindow(0, 0, cx, cy);
}

void CCalendarDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rt;
	CDC*pDC = GetDC();
	GetClientRect(&rt);
	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));
	// Do not call CDialog::OnPaint() for painting messages
}
