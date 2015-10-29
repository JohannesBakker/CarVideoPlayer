// IOLightSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "IOLightSetDlg.h"
#include "ToolBarDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIOLightSetDlg dialog

bool  CIOLightSetDlg::g_bShowIO_ST;
CIOLightSetDlg::CIOLightSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIOLightSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIOLightSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	g_bShowIO_ST = FALSE;
	m_bShowIOCheck = FALSE;
	m_chBuf = new WCHAR[4];
}


void CIOLightSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIOLightSetDlg)
	DDX_Control(pDX, IDC_IO_SIGNAL3_COMBO, m_cmbSignal3);
	DDX_Control(pDX, IDC_IO_SIGNAL2_COMBO, m_cmbSignal2);
	DDX_Control(pDX, IDC_IO_SIGNAL1_COMBO, m_cmbSignal1);
	DDX_Control(pDX, IDCANCEL, m_btn_cancel);
	DDX_Control(pDX, IDOK, m_btn_ok);
	DDX_Control(pDX, IDC_SHOW_IO_LIGHTS_CHECK, m_checkShowIO);
	DDX_Control(pDX, IDC_IO3_STATIC, m_static_io3);
	DDX_Control(pDX, IDC_IO2_STATIC, m_static_io2);
	DDX_Control(pDX, IDC_IO1_STATIC, m_static_io1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIOLightSetDlg, CDialog)
	//{{AFX_MSG_MAP(CIOLightSetDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SHOW_IO_LIGHTS_CHECK, OnShowIoLightsCheck)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIOLightSetDlg message handlers

void CIOLightSetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rt;
	GetClientRect(&rt);
	dc.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 55));

	m_checkShowIO.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);
	//m_checkShowIO.SetWindowText((LPCTSTR)CNewProjDlg::m_wLanguageRes[703]);
	m_static_io1.SetTextColor(RGB(200,200,200));
	m_static_io1.SetFont("Arial", 14, FW_NORMAL);
	m_static_io1.FormatW(CNewProjDlg::m_wLanguageRes[704]);
	m_static_io1.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_io2.SetTextColor(RGB(200,200,200));
	m_static_io2.SetFont("Arial", 14, FW_NORMAL);
	m_static_io2.FormatW(CNewProjDlg::m_wLanguageRes[705]);
	m_static_io2.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_io3.SetTextColor(RGB(200,200,200));
	m_static_io3.SetFont("Arial", 14, FW_NORMAL);
	m_static_io3.FormatW(CNewProjDlg::m_wLanguageRes[706]);
	m_static_io3.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CIOLightSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HBITMAP hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_OK));
	BITMAP bitmap;
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_ok.SetBitmap(hBmp);

	hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_CANCEL));
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_cancel.SetBitmap(hBmp);
	// TODO: Add extra initialization here
//	char* buf;
//	buf = new char[4];
	WCHAR chBuf[100];
	//memset(chBuf, 0, sizeof(chBuf));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[703], chBuf);
	m_checkShowIO.SetWindowText(CNewProjDlg::m_wLanguageRes[703]);
	for (int i = 1 ; i < 9 ; i ++)
	{
		wsprintf(m_chBuf,_T("IO_%d"),i);
		m_cmbSignal1.AddString(m_chBuf);
		m_cmbSignal2.AddString(m_chBuf);
		m_cmbSignal3.AddString(m_chBuf);
	}
	m_cmbSignal1.SetCurSel(0);
	m_cmbSignal2.SetCurSel(1);
	m_cmbSignal3.SetCurSel(2);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIOLightSetDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( m_bShowIOCheck == TRUE)
	{
		CNewProjDlg::g_io_Light_Value_ST.nIO1_Signal = m_cmbSignal1.GetCurSel();
		CNewProjDlg::g_io_Light_Value_ST.nIO2_Signal = m_cmbSignal2.GetCurSel();
		CNewProjDlg::g_io_Light_Value_ST.nIO3_Signal = m_cmbSignal3.GetCurSel();
	}
	else if( m_bShowIOCheck == FALSE )
	{
		CNewProjDlg::g_io_Light_Value_ST.nIO1_Signal = -1;
		CNewProjDlg::g_io_Light_Value_ST.nIO2_Signal = -1;
		CNewProjDlg::g_io_Light_Value_ST.nIO3_Signal = -1;
	}
	if(m_cmbSignal1.GetCurSel() == m_cmbSignal2.GetCurSel() || m_cmbSignal1.GetCurSel() == m_cmbSignal3.GetCurSel() || m_cmbSignal2.GetCurSel() == m_cmbSignal3.GetCurSel())
	{
		MessageBox(_T("different signals can not be set to the same IO,please re-set"), NULL, MB_OK);
		CNewProjDlg::g_io_Light_Value_ST.nIO1_Signal = -1;
		CNewProjDlg::g_io_Light_Value_ST.nIO2_Signal = -1;
		CNewProjDlg::g_io_Light_Value_ST.nIO3_Signal = -1;
	}
	CDialog::OnOK();
}

void CIOLightSetDlg::OnShowIoLightsCheck() 
{
	// TODO: Add your control notification handler code here
	m_bShowIOCheck = !m_bShowIOCheck;
	if (m_bShowIOCheck == TRUE)
	{
		m_checkShowIO.SetCheck(BST_CHECKED);
	}
	else{
		m_checkShowIO.SetCheck(BST_UNCHECKED);
	}
}

void CIOLightSetDlg::OnDestroy()
{
	CDialog::OnDestroy();
//	delete m_chBuf;
	// TODO: Add your message handler code here
}
