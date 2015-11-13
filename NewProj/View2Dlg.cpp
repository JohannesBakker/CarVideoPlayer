// View2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "View2Dlg.h"
#include "ToolBarDlg.h"
#include "View1Dlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView2Dlg dialog

bool CToolBarDlg::g_bView2Updown_ST;
CView2Dlg::CView2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CView2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CView2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bView2Dlg	 = false;
	m_bAllUpdown = false;
	m_bView2DB = FALSE;
	m_bOneWindow = FALSE;
}


void CView2Dlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CView2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CView2Dlg)
	DDX_Control(pDX, IDC_STATIC_VIEW2, m_static_view2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CView2Dlg, CDialog)
	//{{AFX_MSG_MAP(CView2Dlg)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MENU_UP_DOWN_MIRROR, OnMenuUpDown)
	ON_COMMAND(ID_MENU_ALL_UP_DOWN_MIRROR, OnMenuAllUpDown)
	ON_COMMAND(ID_MENU_VIEW_SCALE_16_9, OnMenuViewScale16_9)
	ON_COMMAND(ID_MENU_VIEW_SCALE_4_3, OnMenuViewScale4_3)
	ON_COMMAND(ID_MENU_VIEW_SCALE_AUTO, OnMenuViewScaleAuto)
	ON_COMMAND(ID_MENU_ONE_WINDOW, OnMenuOneWindow)
	ON_COMMAND(ID_MENU_TWO_WINDOW, OnMenuTwoWindow)
	ON_BN_CLICKED(IDC_STATIC_VIEW2, OnStaticView2)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MENU_TURNING, OnMenuTurning)
	ON_COMMAND(ID_MENU_ALL_TURNING, OnMenuAllTurning)
	ON_COMMAND(ID_MENU_SHOW_CHAN_NUM, OnMenuShowChanNum)
	ON_COMMAND(ID_MENU_WMRMK_VERIFY, OnMenuWmrmkVerify)
	ON_COMMAND(ID_MENU_USE_RGB16, OnMenuUseRgb16)
	ON_COMMAND(ID_MENU_RESTORE_SIZE, OnMenuRestoreSize)
	ON_COMMAND(ID_MENU_RETORE_ALL_SIZE, OnMenuRetoreAllSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CView2Dlg, CDialog)
	//{{AFX_DISPATCH_MAP(CView2Dlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IView2Dlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {342A1786-C201-4426-9489-2FDF3D9B5AC0}
static const IID IID_IView2Dlg =
{ 0x342a1786, 0xc201, 0x4426, { 0x94, 0x89, 0x2f, 0xdf, 0x3d, 0x9b, 0x5a, 0xc0 } };

BEGIN_INTERFACE_MAP(CView2Dlg, CDialog)
	INTERFACE_PART(CView2Dlg, IID_IView2Dlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView2Dlg message handlers

BOOL CView2Dlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	if (CNewProjDlg::g_config_Value_ST.nSelWndID == 1) pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(0, 0, 255));
	else pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(255, 0, 0));
	CDC* pdc = GetDlgItem(IDC_STATIC_VIEW2)->GetDC();
	GetDlgItem(IDC_STATIC_VIEW2)->GetClientRect(&rt);
	pdc->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(0, 0, 0));
	return true;//CDialog::OnEraseBkgnd(pDC);
}

void CView2Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	if (m_bView2Dlg)
		GetDlgItem(IDC_STATIC_VIEW2)->MoveWindow(2, 2, cx - 4, cy - 4);
}

void CView2Dlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	CRect rtThis;
	CNewProjDlg::g_config_Value_ST.nSelWndID = 2;
	WndsBK_Paint(&CNewProjDlg::m_View1Dlg, &CNewProjDlg::m_View2Dlg, CNewProjDlg::g_config_Value_ST.nSelWndID);
	GetClientRect(&rtThis);
	AfxGetMainWnd()->GetWindowRect(&rt);
	if (CNewProjDlg::g_config_Value_ST.nWndCounts == 1) 
	{
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_ONE_WINDOW, MF_CHECKED);
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_TWO_WINDOW, MF_UNCHECKED);
	}else{
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_ONE_WINDOW, MF_UNCHECKED);
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_TWO_WINDOW, MF_CHECKED);
	}
	switch(CNewProjDlg::g_config_Value_ST.nViewScaleID)
	{
	case 0:
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_16_9, MF_CHECKED);
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_4_3, MF_UNCHECKED);
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_AUTO, MF_UNCHECKED);
		break;
	case 1:
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_16_9, MF_UNCHECKED);
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_4_3, MF_CHECKED);
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_AUTO, MF_UNCHECKED);
		break;
	case 2:
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_16_9, MF_UNCHECKED);
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_4_3, MF_UNCHECKED);
		CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_VIEW_SCALE_AUTO, MF_CHECKED);
		break;
	default:
		break;
	}
	if (CToolBarDlg::g_bView2Turn_ST == true) CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_TURNING, MF_CHECKED);
	else CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_TURNING, MF_UNCHECKED);
	if (CToolBarDlg::g_bView2Updown_ST == true) CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_UP_DOWN_MIRROR, MF_CHECKED);
	else CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_UP_DOWN_MIRROR, MF_UNCHECKED);
	if (CNewProjDlg::g_config_Value_ST.bShowChanNum == true) CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_SHOW_CHAN_NUM, MF_CHECKED);
	else CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_SHOW_CHAN_NUM, MF_UNCHECKED);
	if (CNewProjDlg::g_config_Value_ST.bUseRGB16 == true) CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_USE_RGB16, MF_CHECKED);
	else CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_USE_RGB16, MF_UNCHECKED);
	if (CNewProjDlg::g_config_Value_ST.bWtrmkVerify == true) CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_WMRMK_VERIFY, MF_CHECKED);
	else CView1Dlg::m_pMenu->CheckMenuItem(ID_MENU_WMRMK_VERIFY, MF_UNCHECKED);
	ASSERT(CView1Dlg::m_pMenu );
	CView1Dlg::m_pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, rt.left + point.x + rtThis.Width() * (CNewProjDlg::g_config_Value_ST.nWndCounts - 1) , rt.top + point.y + 50, this, 0);
	CDialog::OnRButtonDown(nFlags, point);
}

void CView2Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CNewProjDlg::g_config_Value_ST.nSelWndID = 2;
	WndsBK_Paint(&CNewProjDlg::m_View1Dlg, &CNewProjDlg::m_View2Dlg, CNewProjDlg::g_config_Value_ST.nSelWndID);
	CDialog::OnLButtonDown(nFlags, point);
}

void CView2Dlg::OnMenuUpDown() 
{
	// TODO: Add your command handler code here
	CToolBarDlg::g_bView2Updown_ST = !CToolBarDlg::g_bView2Updown_ST;
}

void CView2Dlg::OnMenuAllUpDown() 
{
	// TODO: Add your command handler code here
	CToolBarDlg::g_bView1Updown_ST = !CToolBarDlg::g_bView1Updown_ST;
	CToolBarDlg::g_bView2Updown_ST = !CToolBarDlg::g_bView2Updown_ST;	
}

void CView2Dlg::OnMenuViewScale16_9() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.nViewScaleID = 0;
	WndsBK_Paint(&CNewProjDlg::m_View1Dlg, this, 2);
}

void CView2Dlg::OnMenuViewScale4_3() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.nViewScaleID = 1;
	WndsBK_Paint(&CNewProjDlg::m_View1Dlg, this, 2);
}

void CView2Dlg::OnMenuViewScaleAuto() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.nViewScaleID = 2;
	WndsBK_Paint(&CNewProjDlg::m_View1Dlg, this, 2);
}

void CView2Dlg::OnMenuOneWindow() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	CNewProjDlg::g_config_Value_ST.nSelWndID = 2;
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	if (CToolBarDlg::m_bFullScreen == false)	CNewProjDlg::m_View2Dlg.MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3 / 5, TRUE);
	else CNewProjDlg::m_View2Dlg.MoveWindow(0, 0, rt.Width(), rt.Height(), TRUE);
	CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
	CNewProjDlg::m_View1Dlg.ShowWindow(SW_HIDE);
	CNewProjDlg::g_bViewChange = true;
}

void CView2Dlg::OnMenuTwoWindow() 
{
	// TODO: Add your command handler code here
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
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

void CView2Dlg::OnStaticView2() 
{
	// TODO: Add your control notification handler code here
	CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
}

void CView2Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rt;
	GetClientRect(&rt);
	if (CNewProjDlg::g_config_Value_ST.nSelWndID == 1) dc.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(0, 0, 255));
	else dc.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(255, 0, 0));
	CDC* pdc = GetDlgItem(IDC_STATIC_VIEW2)->GetDC();
	GetDlgItem(IDC_STATIC_VIEW2)->GetClientRect(&rt);
	pdc->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(0, 0, 0));
	// Do not call CDialog::OnPaint() for painting messages
}

void CView2Dlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bView2DB = ! m_bView2DB;
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	if (m_bView2DB && !CToolBarDlg::m_bFullScreen)
	{	
		CNewProjDlg::m_View1Dlg.ShowWindow(SW_HIDE);
		CNewProjDlg::m_View2Dlg.MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3  / 5);
		CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	}
	else if (!m_bView2DB && !CToolBarDlg::m_bFullScreen){
		CNewProjDlg::m_View1Dlg.MoveWindow(0,0,rt.Width() * 1 / 3, rt.Height() * 3  / 5);
		CNewProjDlg::m_View2Dlg.MoveWindow(rt.Width() * 1 / 3, 0 ,rt.Width() * 1/ 3, rt.Height() * 3 / 5);
		CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	}
	if (m_bView2DB && CToolBarDlg::m_bFullScreen)
	{	
		CNewProjDlg::m_View1Dlg.ShowWindow(SW_HIDE);
		CNewProjDlg::m_View2Dlg.MoveWindow(0, 0, rt.Width() , rt.Height() );
		CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	}
	else if (!m_bView2DB && CToolBarDlg::m_bFullScreen){
		CNewProjDlg::m_View1Dlg.MoveWindow(0,0,rt.Width() / 2, rt.Height());
		CNewProjDlg::m_View2Dlg.MoveWindow(rt.Width() / 2, 0 ,rt.Width() / 2, rt.Height());
		CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	}
	CNewProjDlg::g_bViewChange = true;
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CView2Dlg::OnMenuTurning() 
{
	// TODO: Add your command handler code here
	CToolBarDlg::g_bView2Turn_ST = !CToolBarDlg::g_bView2Turn_ST;
}

void CView2Dlg::OnMenuAllTurning() 
{
	// TODO: Add your command handler code here
	CToolBarDlg::g_bView1Turn_ST = !CToolBarDlg::g_bView1Turn_ST;
	CToolBarDlg::g_bView2Turn_ST = !CToolBarDlg::g_bView2Turn_ST;
}

void CView2Dlg::OnMenuShowChanNum() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.bShowChanNum = ! CNewProjDlg::g_config_Value_ST.bShowChanNum;
}

void CView2Dlg::OnMenuWmrmkVerify() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.bWtrmkVerify = ! CNewProjDlg::g_config_Value_ST.bWtrmkVerify;
}

void CView2Dlg::OnMenuUseRgb16() 
{
	// TODO: Add your command handler code here
	CNewProjDlg::g_config_Value_ST.bUseRGB16 = !CNewProjDlg::g_config_Value_ST.bUseRGB16;	
}

void CView2Dlg::OnMenuRestoreSize() 
{
	// TODO: Add your command handler code here	
}

void CView2Dlg::OnMenuRetoreAllSize() 
{
	// TODO: Add your command handler code here
}
