// AdjustVideoParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "AdjustVideoParamDlg.h"
//#include "ToolBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdjustVideoParamDlg dialog



CAdjustVideoParamDlg::CAdjustVideoParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdjustVideoParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdjustVideoParamDlg)
	m_hueValue = 0;
	m_lightValue = 0;
	m_satValue = 0;
	m_contValue = 0;
	m_bSel1 = FALSE;
	m_bSel12 =FALSE;
	m_bSel2 = FALSE;
	//}}AFX_DATA_INIT
}


void CAdjustVideoParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjustVideoParamDlg)
	DDX_Control(pDX, IDC_SATURATION_SLIDER, m_sat_slider);
	DDX_Control(pDX, IDC_CONTRACT_SLIDER, m_cont_slider);
	DDX_Control(pDX, IDC_LIGHT_SLIDER, m_light_slider);
	DDX_Control(pDX, IDC_HUE_SLIDER, m_hue_slider);
	DDX_Control(pDX, IDOK_ADJUST, m_btn_Ok);
	DDX_Control(pDX, IDCANCEL_ADJUST, m_btn_Cancel);
	DDX_Control(pDX, IDC_RESTORE_BUTTON, m_btn_Restore);
	DDX_Control(pDX, IDC_CAMERA_CHOOSE_STATIC, m_cameraChsStc);
	DDX_Control(pDX, IDC_SEL_01_02_CHECK, m_checkChan1_2);
	DDX_Control(pDX, IDC_CHAN2_CHECK, m_checkChan2);
	DDX_Control(pDX, IDC_CHAN1_CHECK, m_checkChan1);
	DDX_Control(pDX, IDC_TRANSCODING2_CHECK, m_checkTrans2);
	DDX_Control(pDX, IDC_TRANSCODING1_CHECK, m_checkTrans1);
	DDX_Control(pDX, IDC_REALTIMELY_CHECK, m_checkRealTime);
	DDX_Control(pDX, IDC_SATURATION_EDIT, m_edit_saturation);
	DDX_Control(pDX, IDC_LIGHT_EDIT, m_edit_light);
	DDX_Control(pDX, IDC_HUE_EDIT, m_edit_hue);
	DDX_Control(pDX, IDC_CONTRACT_EDIT, m_edit_contract);
	DDX_Control(pDX, IDC_SATURATION_STATIC, m_static_saturation);
	DDX_Control(pDX, IDC_LIGHT_STATIC, m_static_light);
	DDX_Control(pDX, IDC_HUE_STATIC, m_static_hue);
	DDX_Control(pDX, IDC_CONTRACT_STATIC, m_static_contract);
	DDX_Text(pDX, IDC_HUE_EDIT, m_hueValue);
	DDX_Text(pDX, IDC_LIGHT_EDIT, m_lightValue);
	DDX_Text(pDX, IDC_SATURATION_EDIT, m_satValue);
	DDX_Text(pDX, IDC_CONTRACT_EDIT, m_contValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdjustVideoParamDlg, CDialog)
	//{{AFX_MSG_MAP(CAdjustVideoParamDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RESTORE_BUTTON, OnRestoreButton)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK_ADJUST, OnOK)
	ON_BN_CLICKED(IDCANCEL_ADJUST, OnCancel)
	ON_BN_CLICKED(IDC_SEL_01_02_CHECK, OnSel1_2Check)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_REALTIMELY_CHECK, &CAdjustVideoParamDlg::OnBnClickedRealtimelyCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdjustVideoParamDlg message handlers

void CAdjustVideoParamDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}

void CAdjustVideoParamDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rt;
		
		GetClientRect(&rt);
		dc.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CAdjustVideoParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_hue_slider.BuildThumbItem(IDB_VIDEO_PARAM_THUMB, 17, 24);
	m_hue_slider.BuildBackItem(IDB_VIDEO_PARAM_SLIDER, IDB_VIDEO_PARAM_SLIDER);
	m_hue_slider.SetTopOffset(0);
	m_hue_slider.SetLeftOffset(0);
	m_hue_slider.SetRange(0, 255);
	m_hue_slider.SetLineSize(0);
	m_hue_slider.SetPos(0);

	m_light_slider.BuildThumbItem(IDB_VIDEO_PARAM_THUMB, 17, 24);
	m_light_slider.BuildBackItem(IDB_VIDEO_PARAM_SLIDER, IDB_VIDEO_PARAM_SLIDER);
	m_light_slider.SetTopOffset(0);
	m_light_slider.SetLeftOffset(0);
	m_light_slider.SetRange(-255, 255);
	m_light_slider.SetLineSize(0);
	m_light_slider.SetPos(0);

	m_sat_slider.BuildThumbItem(IDB_VIDEO_PARAM_THUMB, 17, 24);
	m_sat_slider.BuildBackItem(IDB_VIDEO_PARAM_SLIDER, IDB_VIDEO_PARAM_SLIDER);
	m_sat_slider.SetTopOffset(0);
	m_sat_slider.SetLeftOffset(0);
	m_sat_slider.SetRange(0, 255);
	m_sat_slider.SetLineSize(0);
	m_sat_slider.SetPos(0);

	m_cont_slider.BuildThumbItem(IDB_VIDEO_PARAM_THUMB, 17, 24);
	m_cont_slider.BuildBackItem(IDB_VIDEO_PARAM_SLIDER, IDB_VIDEO_PARAM_SLIDER);
	m_cont_slider.SetTopOffset(0);
	m_cont_slider.SetLeftOffset(0);
	m_cont_slider.SetRange(-100, 100);
	m_cont_slider.SetLineSize(0);
	m_cont_slider.SetPos(0);

	m_checkRealTime.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);
	m_checkTrans1.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);
	m_checkTrans2.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);
	m_checkChan1.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);
	m_checkChan2.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);
	m_checkChan1_2.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);

	HBITMAP hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_RESTORE));
	BITMAP bitmap;
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_Restore.SetBitmap(hBmp);

	hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_OK));
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_Ok.SetBitmap(hBmp);

	hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_CANCEL));
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_Cancel.SetBitmap(hBmp);

	m_edit_hue.SetTextColor(RGB(200, 200, 200));
	m_edit_hue.SetBackColor(RGB(47, 56, 66));
	m_edit_saturation.SetTextColor(RGB(200, 200, 200));
	m_edit_saturation.SetBackColor(RGB(47, 56, 66));
	m_edit_light.SetTextColor(RGB(200, 200, 200));
	m_edit_light.SetBackColor(RGB(47, 56, 66));
	m_edit_contract.SetTextColor(RGB(200, 200, 200));
	m_edit_contract.SetBackColor(RGB(47, 56, 66));
	
	m_cameraChsStc.SetTextColor(RGB(200,200,200));
	m_cameraChsStc.SetFont("Arial", 14, FW_NORMAL);
	m_cameraChsStc.FormatW(CNewProjDlg::m_wLanguageRes[254]);
	//m_cameraChsStc.SetWindowText("Camera Choose");
	m_cameraChsStc.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_hue.SetTextColor(RGB(200,200,200));
	m_static_hue.SetFont("Arial", 14, FW_NORMAL);
	m_static_hue.FormatW(CNewProjDlg::m_wLanguageRes[254]);
	m_static_hue.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_saturation.SetTextColor(RGB(200,200,200));
	m_static_saturation.SetFont("Arial", 14, FW_NORMAL);
	m_static_saturation.FormatW(CNewProjDlg::m_wLanguageRes[255]);
	m_static_saturation.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_light.SetTextColor(RGB(200,200,200));
	m_static_light.SetFont("Arial", 14, FW_NORMAL);
	m_static_light.FormatW(CNewProjDlg::m_wLanguageRes[256]);
	m_static_light.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_contract.SetTextColor(RGB(200,200,200));
	m_static_contract.SetFont("Arial", 14, FW_NORMAL);
	m_static_contract.FormatW(CNewProjDlg::m_wLanguageRes[257]);
	m_static_contract.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdjustVideoParamDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	if ((m_checkChan1.GetCheck()== BST_CHECKED) && (m_checkChan2.GetCheck() == BST_CHECKED))
	{
		m_bSel12 = TRUE;
		m_bSel1 =TRUE;
		m_bSel2 = TRUE;
	}
	if (m_checkChan1.GetCheck() == BST_CHECKED)
	{
		m_bSel1 = TRUE;
	}
	if (m_checkChan2.GetCheck() == BST_CHECKED)
	{
		m_bSel2 = TRUE;
	}

	CDialog::OnOK();
}

void CAdjustVideoParamDlg::OnRestoreButton() 
{
	// TODO: Add your control notification handler code here
	m_hue_slider.SetPos(0);
	m_light_slider.SetPos(0);
	m_sat_slider.SetPos(0);
	m_cont_slider.SetPos(0);
	m_hueValue = 0;
	m_satValue = 0;
	m_lightValue = 0;
	m_contValue = 0;
	UpdateData(false);
}

void CAdjustVideoParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	// TODO: Add your message handler code here and/or call default
	if ((void *)pScrollBar == (void *)&m_hue_slider)
	{
		nPos = ((CSliderCtrl*)pScrollBar)->GetPos();
		//m_hueEdit.SetWindowText((LPCTSTR)itoa(nPos,buf,10));
		m_hueValue = nPos;
		CNewProjDlg::g_adjust_Value_ST.nHue = nPos;
	}
	else if ((void *)pScrollBar == (void *)&m_sat_slider)
	{
		nPos = ((CSliderCtrl*)pScrollBar)->GetPos();
		m_satValue = nPos;
		CNewProjDlg::g_adjust_Value_ST.nSaturation = nPos;
		
	}
	else if ((void *)pScrollBar == (void *)&m_light_slider)
	{
		nPos = ((CSliderCtrl*)pScrollBar)->GetPos();
		m_lightValue = nPos;
		CNewProjDlg::g_adjust_Value_ST.nLight = nPos;
	}
	else if ((void *)pScrollBar == (void *)&m_cont_slider)
	{
		nPos = ((CSliderCtrl*)pScrollBar)->GetPos();
		m_contValue = nPos;
		CNewProjDlg::g_adjust_Value_ST.nContract = nPos;
	}
	UpdateData(false);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CAdjustVideoParamDlg::OnSel1_2Check() 
{
	// TODO: Add your control notification handler code here
	if (m_checkChan1_2.GetCheck() == BST_CHECKED )
	{
		m_checkChan1.SetCheck(BST_CHECKED);
		m_checkChan2.SetCheck(BST_CHECKED);
	}
	else if (m_checkChan1_2.GetCheck() == BST_UNCHECKED)
	{
		m_checkChan1.SetCheck(BST_UNCHECKED);
		m_checkChan2.SetCheck(BST_UNCHECKED);
	}
}

void CAdjustVideoParamDlg::OnBnClickedRealtimelyCheck()
{
	// TODO: Add your control notification handler code here
}
