// RecordPlayerAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "RecordPlayerAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordPlayerAboutDlg dialog


CRecordPlayerAboutDlg::CRecordPlayerAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordPlayerAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordPlayerAboutDlg)
	//}}AFX_DATA_INIT
}


void CRecordPlayerAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordPlayerAboutDlg)
	DDX_Control(pDX, IDC_OK_BUTTON, m_btn_Ok);
	DDX_Control(pDX, IDC_ICON_STATIC, m_static_icon);
	DDX_Control(pDX, IDC_COPYRIGHT_STATIC, m_static_copyright);
	DDX_Control(pDX, IDC_VERSION_STATIC, m_static_version);
	DDX_Control(pDX, IDC_ABOUT_STATIC, m_static_about);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordPlayerAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CRecordPlayerAboutDlg)
	ON_BN_CLICKED(IDC_OK_BUTTON, OnOkButton)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordPlayerAboutDlg message handlers

void CRecordPlayerAboutDlg::OnOkButton() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CRecordPlayerAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
	CRect rt;
	CDC*pDC = GetDC();
	GetClientRect(&rt);
	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));

	m_static_version.SetTextColor(RGB(200, 200, 200));
	m_static_version.SetFont("Arial", 14, FW_NORMAL);
	//m_static_version.FormatW(CNewProjDlg::m_wLanguageRes[150]);
	m_static_version.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_copyright.SetTextColor(RGB(200, 200, 200));
	m_static_copyright.SetFont("Arial", 14, FW_NORMAL);
	m_static_copyright.FormatW(CNewProjDlg::m_wLanguageRes[268]);
	m_static_copyright.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_about.SetTextColor(RGB(200, 200, 200));
	m_static_about.SetFont("Arial", 14, FW_NORMAL);
	//m_static_about.FormatW(CNewProjDlg::m_wLanguageRes[627]);
	m_static_about.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_icon.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);
	HBITMAP hBmp;
	HINSTANCE hInst;
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_ICON),IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	m_static_icon.SetBitmap(hBmp);
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CRecordPlayerAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HBITMAP hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_OK));
	BITMAP bitmap;
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_Ok.SetBitmap(hBmp);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
