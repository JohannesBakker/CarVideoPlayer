// View1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "View1Dlg.h"
#include "ToolBarDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView1Dlg dialog
CMenu* CView1Dlg::m_pMenu;//= new CMenu;
bool CToolBarDlg::g_bView1Updown_ST;
bool CToolBarDlg::m_bFullScreen;

//bool CToolBarDlg::g_bView2Updown;
CView1Dlg::CView1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CView1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CView1Dlg)
	//}}AFX_DATA_INIT
	m_bView1Dlg = false;
	m_bAllUpdown = false;
	m_pMenu = new CMenu;
	m_bView1DB = FALSE;
	m_bTwoWindow = FALSE;
	m_bOneWindow = FALSE;
}


void CView1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CView1Dlg)
	DDX_Control(pDX, IDC_STATIC_VIEW1, m_static_view1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CView1Dlg, CDialog)
	//{{AFX_MSG_MAP(CView1Dlg)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MENU_UP_DOWN_MIRROR, OnMenuUpDown)
	ON_COMMAND(ID_MENU_ALL_UP_DOWN_MIRROR, OnMenuAllUpDown)
	ON_COMMAND(ID_MENU_SHOW_CHAN_NUM, OnMenuShowChanNum)
	ON_COMMAND(ID_MENU_USE_RGB16, OnMenuUseRgb16)
	ON_COMMAND(ID_MENU_WMRMK_VERIFY, OnMenuWmrmkVerify)
	ON_COMMAND(ID_MENU_VIEW_SCALE_16_9, OnMenuViewScale16_9)
	ON_COMMAND(ID_MENU_VIEW_SCALE_4_3, OnMenuViewScale4_3)
	ON_COMMAND(ID_MENU_VIEW_SCALE_AUTO, OnMenuViewScaleAuto)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MENU_ONE_WINDOW, OnMenuOneWindow)
	ON_COMMAND(ID_MENU_TWO_WINDOW, OnMenuTwoWindow)
	ON_COMMAND(ID_MENU_TURNING, OnMenuTurning)
	ON_COMMAND(ID_MENU_ALL_TURNING, OnMenuAllTurning)
	ON_COMMAND(ID_MENU_RESTORE_SIZE, OnMenuRestoreSize)
	ON_COMMAND(ID_MENU_RETORE_ALL_SIZE, OnMenuRetoreAllSize)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView1Dlg message handlers

BOOL CView1Dlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	if (CNewProjDlg::g_config_Value_ST.nSelWndID == 1) pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(255, 0, 0));
	else pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(0, 0, 255));
	CDC* pdc = GetDlgItem(IDC_STATIC_VIEW1)->GetDC();
	GetDlgItem(IDC_STATIC_VIEW1)->GetClientRect(&rt);
	pdc->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(0, 0, 0));
	return true;//CDialog::OnEraseBkgnd(pDC);
}

void CView1Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CView1Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_bView1Dlg)
		GetDlgItem(IDC_STATIC_VIEW1)->MoveWindow(2, 2, cx - 4, cy - 4);
}

void CView1Dlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CNewProjDlg::g_config_Value_ST.nSelWndID = 1;
	WndsBK_Paint(&CNewProjDlg::m_View1Dlg, &CNewProjDlg::m_View2Dlg, CNewProjDlg::g_config_Value_ST.nSelWndID);
	CRect rt;
	AfxGetMainWnd()->GetWindowRect(&rt);
	if (CNewProjDlg::g_config_Value_ST.nWndCounts == 1) 
	{
		m_pMenu->CheckMenuItem(ID_MENU_ONE_WINDOW, MF_CHECKED);
		m_pMenu->CheckMenuItem(ID_MENU_TWO_WINDOW, MF_UNCHECKED);
	}else{
		m_pMenu->CheckMenuItem(ID_MENU_ONE_WINDOW, MF_UNCHECKED);
		m_pMenu->CheckMenuItem(ID_MENU_TWO_WINDOW, MF_CHECKED);
	}
	switch(CNewProjDlg::g_config_Value_ST.nViewScaleID)
	{
	case 0:
		m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_16_9, MF_CHECKED);
		m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_4_3, MF_UNCHECKED);
		m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_AUTO, MF_UNCHECKED);
		break;
	case 1:
		m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_16_9, MF_UNCHECKED);
		m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_4_3, MF_CHECKED);
		m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_AUTO, MF_UNCHECKED);
		break;
	case 2:
		m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_16_9, MF_UNCHECKED);
		m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_4_3, MF_UNCHECKED);
		m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_AUTO, MF_CHECKED);
		break;
	default:
		break;
	}
	if (CToolBarDlg::g_bView1Turn_ST == true) m_pMenu->CheckMenuItem(ID_MENU_TURNING, MF_CHECKED);
	else m_pMenu->CheckMenuItem(ID_MENU_TURNING, MF_UNCHECKED);
	if (CToolBarDlg::g_bView1Updown_ST == true) m_pMenu->CheckMenuItem(ID_MENU_UP_DOWN_MIRROR, MF_CHECKED);
	else m_pMenu->CheckMenuItem(ID_MENU_UP_DOWN_MIRROR, MF_UNCHECKED);
	if (CNewProjDlg::g_config_Value_ST.bShowChanNum == true) m_pMenu->CheckMenuItem(ID_MENU_SHOW_CHAN_NUM, MF_CHECKED);
	else m_pMenu->CheckMenuItem(ID_MENU_SHOW_CHAN_NUM, MF_UNCHECKED);
	if (CNewProjDlg::g_config_Value_ST.bUseRGB16 == true) m_pMenu->CheckMenuItem(ID_MENU_USE_RGB16, MF_CHECKED);
	else m_pMenu->CheckMenuItem(ID_MENU_USE_RGB16, MF_UNCHECKED);
	if (CNewProjDlg::g_config_Value_ST.bWtrmkVerify == true) m_pMenu->CheckMenuItem(ID_MENU_WMRMK_VERIFY, MF_CHECKED);
	else m_pMenu->CheckMenuItem(ID_MENU_WMRMK_VERIFY, MF_UNCHECKED);
	m_pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, rt.left + point.x, rt.top + point.y + 50, this, 0);
	CDialog::OnRButtonDown(nFlags, point);
}

void CView1Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CNewProjDlg::g_config_Value_ST.nSelWndID = 1;
	WndsBK_Paint(&CNewProjDlg::m_View1Dlg, &CNewProjDlg::m_View2Dlg, CNewProjDlg::g_config_Value_ST.nSelWndID);
	CDialog::OnLButtonDown(nFlags, point);
}

void CView1Dlg::OnMenuUpDown() 
{
	// TODO: Add your command handler code here
	CToolBarDlg::g_bView1Updown_ST = !CToolBarDlg::g_bView1Updown_ST;
}

void CView1Dlg::OnMenuAllUpDown() 
{
	// TODO: Add your command handler code here
	CToolBarDlg::g_bView1Updown_ST = !CToolBarDlg::g_bView1Updown_ST;
	CToolBarDlg::g_bView2Updown_ST = !CToolBarDlg::g_bView2Updown_ST;	
}

void CView1Dlg::OnMenuShowChanNum() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.bShowChanNum = ! CNewProjDlg::g_config_Value_ST.bShowChanNum;
}

void CView1Dlg::OnMenuUseRgb16() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.bUseRGB16 = !CNewProjDlg::g_config_Value_ST.bUseRGB16;
}

void CView1Dlg::OnMenuWmrmkVerify() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.bWtrmkVerify = ! CNewProjDlg::g_config_Value_ST.bWtrmkVerify;
}

void CView1Dlg::OnMenuViewScale16_9() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.nViewScaleID = 0;
	WndsBK_Paint(this, &CNewProjDlg::m_View2Dlg, 1);
}

void CView1Dlg::OnMenuViewScale4_3() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.nViewScaleID = 1;
	WndsBK_Paint(this, &CNewProjDlg::m_View2Dlg, 1);
}

void CView1Dlg::OnMenuViewScaleAuto() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.nViewScaleID = 2;
	WndsBK_Paint(this, &CNewProjDlg::m_View2Dlg, 1);
}

void CView1Dlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bView1DB = ! m_bView1DB;
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	if (m_bView1DB && !CToolBarDlg::m_bFullScreen)
	{	
		CNewProjDlg::m_View2Dlg.ShowWindow(SW_HIDE);
		CNewProjDlg::m_View1Dlg.MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3  / 5);
		CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	}
	else if (!m_bView1DB && !CToolBarDlg::m_bFullScreen){
		CNewProjDlg::m_View1Dlg.MoveWindow(0,0,rt.Width() * 1 / 3, rt.Height() * 3  / 5);
		CNewProjDlg::m_View2Dlg.MoveWindow(rt.Width() * 1 / 3, 0 ,rt.Width() * 1/ 3, rt.Height() * 3 / 5);
		CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	}
	if (m_bView1DB && CToolBarDlg::m_bFullScreen)
	{	
		CNewProjDlg::m_View2Dlg.ShowWindow(SW_HIDE);
		CNewProjDlg::m_View1Dlg.MoveWindow(0, 0, rt.Width() , rt.Height() );
		CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	}
	else if (!m_bView1DB && CToolBarDlg::m_bFullScreen){
		CNewProjDlg::m_View1Dlg.MoveWindow(0,0,rt.Width() / 2, rt.Height());
		CNewProjDlg::m_View2Dlg.MoveWindow(rt.Width() / 2, 0 ,rt.Width() / 2, rt.Height());
		CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	}
	CNewProjDlg::g_bViewChange = true;
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CView1Dlg::OnMenuOneWindow() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	CNewProjDlg::g_config_Value_ST.nSelWndID = 1;
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	if (CToolBarDlg::m_bFullScreen == false) CNewProjDlg::m_View1Dlg.MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3 / 5, TRUE);
	else CNewProjDlg::m_View1Dlg.MoveWindow(0, 0, rt.Width(), rt.Height(), TRUE);
	CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
	CNewProjDlg::m_View2Dlg.ShowWindow(SW_HIDE);
	CNewProjDlg::g_bViewChange = true;
}

void CView1Dlg::OnMenuTwoWindow() 
{
	// TODO: Add your command handler code here
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	if (CNewProjDlg::m_SecondPathArrays[CNewProjDlg::m_nPathArraysPtr] == "") return;
	CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	if (CToolBarDlg::m_bFullScreen == false)
	{
		CNewProjDlg::m_View1Dlg.MoveWindow(0, 0, rt.Width() / 3, rt.Height() * 3 / 5, TRUE);
		CNewProjDlg::m_View2Dlg.MoveWindow(rt.Width() / 3, 0, rt.Width() / 3, rt.Height() *  3 / 5, TRUE);
	}else{
		CNewProjDlg::m_View1Dlg.MoveWindow(0, 0, rt.Width() / 2, rt.Height(), TRUE);
		CNewProjDlg::m_View2Dlg.MoveWindow(rt.Width() / 2, 0, rt.Width() / 2, rt.Height(), TRUE);
	}
	CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
	CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
	CNewProjDlg::g_bViewChange = true;
}
CMenu* menu;
BOOL CView1Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	menu = new CMenu;
	menu->LoadMenu(IDR_MENU_VIEW1);
	m_pMenu = menu->GetSubMenu(0);
	
	ASSERT(m_pMenu);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CView1Dlg::OnMenuTurning() 
{
	// TODO: Add your command handler code here
	CToolBarDlg::g_bView1Turn_ST = !CToolBarDlg::g_bView1Turn_ST;
}

void CView1Dlg::OnMenuAllTurning() 
{
	// TODO: Add your command handler code here
	CToolBarDlg::g_bView1Turn_ST = !CToolBarDlg::g_bView1Turn_ST;
	CToolBarDlg::g_bView2Turn_ST = !CToolBarDlg::g_bView2Turn_ST;
}

void CView1Dlg::OnMenuRestoreSize() 
{
	// TODO: Add your command handler code here
}

void CView1Dlg::OnMenuRetoreAllSize() 
{
	// TODO: Add your command handler code here
}

void CView1Dlg::OnDestroy()
{
	delete menu;
	CDialog::OnDestroy();
	
	//delete m_pMenu;
	// TODO: Add your message handler code here
}
