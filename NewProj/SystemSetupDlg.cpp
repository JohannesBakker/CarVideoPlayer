// SystemSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "SystemSetupDlg.h"
#include "ToolBarDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 
#define TEXT_COLOR_GRAY		RGB(200, 200, 200)
#define TEXT_COLOR			RGB(255, 255, 255)
#define BK_COLOR			RGB(47, 56, 66)
#define FONT_SIZE			14

/////////////////////////////////////////////////////////////////////////////
// CSystemSetupDlg dialog


CSystemSetupDlg::CSystemSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemSetupDlg)
	m_chk_wtrmk = FALSE;
	m_chk_use_rgb16 = FALSE;
	m_chk_show_data_window = FALSE;
	m_chk_io_alarm = FALSE;
	m_chk_264_file = FALSE;
	m_chk_acc_winker = FALSE;
	m_chk_avi_file = FALSE;
	m_chk_autoplay = FALSE;
	m_chk_channel_number = FALSE;
	m_chk_device_camera = FALSE;
	m_chk_driver_company = FALSE;
	m_chk_gps_offset_corre = FALSE;
	m_chk_show_gps_track = FALSE;
	//}}AFX_DATA_INIT
}


void CSystemSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemSetupDlg)
	DDX_Control(pDX, IDCANCEL, m_btn_Cancel);
	DDX_Control(pDX, IDOK, m_btn_Ok);
	DDX_Control(pDX, IDC_FILE_ASSOCIATION_STATIC, m_fileAssStc);
	DDX_Control(pDX, IDC_VIDEO_PARAM_STATIC, m_videoParamStc);
	DDX_Control(pDX, IDC_MAP_PARAM_STATIC, m_mapParamStc);
	DDX_Control(pDX, IDC_WINDOW_PARAM_STATIC, m_windowParamStc);
	DDX_Control(pDX, IDC_SYSTEMPARAMS_STATIC, m_systemParamStatic);
	DDX_Control(pDX, IDC_SHOW_DATE_WINDOW_CHECK, m_showDateBtn);
	DDX_Control(pDX, IDC_WATERMARK_CHECK, m_waterMarkBtn);
	DDX_Control(pDX, IDC_USE_RGB16_CHECK, m_rgb16Btn);
	DDX_Control(pDX, IDC_IO_ALARM_CHECK, m_ioAlarmBtn);
	DDX_Control(pDX, IDC_GPS_OFFSET_CORRE_CHECK, m_GpsOffBtn);
	DDX_Control(pDX, IDC_DRIVER_COMPANY_CHECK, m_driverComBtn);
	DDX_Control(pDX, IDC_DEVICE_CAMERA_CHECK, m_deviceCamBtn);
	DDX_Control(pDX, IDC_CHANNEL_NUMBER_CHECK, m_channelCheckBtn);
	DDX_Control(pDX, IDC_AVI_FILE_CHECK, m_aviCheckBtn);
	DDX_Control(pDX, IDC_AUTOPLAY_CHECK, m_autoplayCheckBtn);
	DDX_Control(pDX, IDC_ACC_WINKER_CHECK, m_accCheck);
	DDX_Control(pDX, IDC_264_FILE_CHECK, m_264Check);
	DDX_Control(pDX, IDC_SHOW_GPS_TRACK_CHECK, m_showGPSTrackBtn);
	DDX_Control(pDX, IDC_WTRMK_VERT_EDIT, m_edit_wtmk_vert);
	DDX_Control(pDX, IDC_WTRMK_HORI_EDIT, m_edit_wtmk_hori);
	DDX_Control(pDX, IDC_WATERMASK_EDIT, m_edit_watermask);
	DDX_Control(pDX, IDC_IO_ALARM_VERT_EDIT, m_edit_alarm_vert);
	DDX_Control(pDX, IDC_IO_ALARM_HORI_EDIT, m_edit_alarm_hori);
	DDX_Control(pDX, IDC_DRIVER_COMPANY_VERT_EDIT, m_edit_dri_company_vert);
	DDX_Control(pDX, IDC_DRIVER_COMPANY_HORI_EDIT, m_edit_dri_company_hori);
	DDX_Control(pDX, IDC_DEVICE_CAMERA_VERT_EDIT, m_edit_dev_camera_vert);
	DDX_Control(pDX, IDC_DEVICE_CAMERA_HORI_EDIT, m_edit_dev_camera_hori);
	DDX_Control(pDX, IDC_ACC_WINKER_VERT_EDIT, m_edit_acc_winker_vert);
	DDX_Control(pDX, IDC_ACC_WINKER_HORI_EDIT, m_edit_acc_winder_hori);
	DDX_Control(pDX, IDC_WTMKVERTPIX_STATIC, m_static_wtmkvertpix);
	DDX_Control(pDX, IDC_WTMKVERT_STATIC, m_static_wtmkvert);
	DDX_Control(pDX, IDC_WTMKHORIPIX_STATIC, m_static_wtmkhoripix);
	DDX_Control(pDX, IDC_WTMKHORI_STATIC, m_static_wtmkhori);
	DDX_Control(pDX, IDC_WINDOWS_STATIC, m_static_windows);
	DDX_Control(pDX, IDC_WATERMASK_STATIC, m_static_watermask);
	DDX_Control(pDX, IDC_VIEWSCALE_STATIC, m_static_viewscale);
	DDX_Control(pDX, IDC_SPEEDUNIT_STATIC, m_static_speedunit);
	DDX_Control(pDX, IDC_MAPTYPE_STATIC, m_static_maptype);
	DDX_Control(pDX, IDC_LANGUAGE_STATIC, m_static_language);
	DDX_Control(pDX, IDC_DRIVERTPIX_STATIC, m_static_drivertpix);
	DDX_Control(pDX, IDC_DRIVERT_STATIC, m_static_drivert);
	DDX_Control(pDX, IDC_DRIPOSI_STATIC, m_static_driposi);
	DDX_Control(pDX, IDC_DRIHORIPIX_STATIC, m_static_drihoripix);
	DDX_Control(pDX, IDC_DRIHORI_STATIC, m_static_drihori);
	DDX_Control(pDX, IDC_DEVVERTPIX_STATIC, m_static_devvertpix);
	DDX_Control(pDX, IDC_DEVVERT_STATIC, m_static_devvert);
	DDX_Control(pDX, IDC_DEVPOSI_STATIC, m_static_devposi);
	DDX_Control(pDX, IDC_DEVHORIPIX_STATIC, m_static_devhoripix);
	DDX_Control(pDX, IDC_DEVHORI_STATIC, m_static_devhori);
	DDX_Control(pDX, IDC_ALARMVERTPIX_STATIC, m_static_alarmvertpix);
	DDX_Control(pDX, IDC_ALARMVERT_STATIC, m_static_alarmvert);
	DDX_Control(pDX, IDC_ALARMPOSI_STATIC, m_static_alarmposi);
	DDX_Control(pDX, IDC_ALARMHORIPIX_STATIC, m_static_alarmhoripix);
	DDX_Control(pDX, IDC_ALARMHORI_STATIC, m_static_alarmhori);
	DDX_Control(pDX, IDC_ACCVERTPIX_STATIC, m_static_accvertpix);
	DDX_Control(pDX, IDC_ACCVERT_STATIC, m_static_accvert);
	DDX_Control(pDX, IDC_ACCPOSI_STATIC, m_static_accposi);
	DDX_Control(pDX, IDC_ACCHORIPIX_STATIC, m_static_acchoripix);
	DDX_Control(pDX, IDC_ACCHORI_STATIC, m_static_acchori);
	DDX_Control(pDX, IDC_SPEED_UNIT_COMBO, m_cmb_speed_unit);
	DDX_Control(pDX, IDC_DRIVER_COMPANY_COMBO, m_cmb_driver_company);
	DDX_Control(pDX, IDC_DEVICE_CAMERA_COMBO, m_cmb_device_camera);
	DDX_Control(pDX, IDC_CLEAR_PASSWORD_BUTTON, m_btn_clear_password);
	DDX_Control(pDX, IDC_ACC_WINKER_COMBO, m_cmb_acc_winker);
	DDX_Control(pDX, IDC_IO_ALARM_POSI_COMBO, m_cmb_io_alarm_posi);
	DDX_Control(pDX, IDC_LANGUAGE_COMBO, m_cmb_language);
	DDX_Control(pDX, IDC_MAP_TYPE_COMBO, m_cmb_map_type);
	DDX_Control(pDX, IDC_PLAYER_VIEW_SCALE_COMBO, m_cmb_player_view_scale);
	DDX_Control(pDX, IDC_WINDOWS_COMBO, m_cmb_windows);
	DDX_Check(pDX, IDC_WATERMARK_CHECK, m_chk_wtrmk);
	DDX_Check(pDX, IDC_USE_RGB16_CHECK, m_chk_use_rgb16);
	DDX_Check(pDX, IDC_SHOW_DATA_WINDOW_CHECK, m_chk_show_data_window);
	DDX_Check(pDX, IDC_IO_ALARM_CHECK, m_chk_io_alarm);
	DDX_Check(pDX, IDC_264_FILE_CHECK, m_chk_264_file);
	DDX_Check(pDX, IDC_ACC_WINKER_CHECK, m_chk_acc_winker);
	DDX_Check(pDX, IDC_AVI_FILE_CHECK, m_chk_avi_file);
	DDX_Check(pDX, IDC_AUTOPLAY_CHECK, m_chk_autoplay);
	DDX_Check(pDX, IDC_CHANNEL_NUMBER_CHECK, m_chk_channel_number);
	DDX_Check(pDX, IDC_DEVICE_CAMERA_CHECK, m_chk_device_camera);
	DDX_Check(pDX, IDC_DRIVER_COMPANY_CHECK, m_chk_driver_company);
	DDX_Check(pDX, IDC_GPS_OFFSET_CORRE_CHECK, m_chk_gps_offset_corre);
	DDX_Check(pDX, IDC_SHOW_GPS_TRACK_CHECK, m_chk_show_gps_track);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSystemSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemSetupDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemSetupDlg message handlers



BOOL CSystemSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HBITMAP hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_OK));
	BITMAP bitmap;
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_Ok.SetBitmap(hBmp);

	hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_CANCEL));
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_Cancel.SetBitmap(hBmp);
	
	m_GpsOffBtn.FormatW(CNewProjDlg::m_wLanguageRes[100]);
	m_showDateBtn.FormatW(CNewProjDlg::m_wLanguageRes[369]);
	m_showGPSTrackBtn.FormatW(CNewProjDlg::m_wLanguageRes[101]);
	m_waterMarkBtn.FormatW(CNewProjDlg::m_wLanguageRes[110]);
	m_rgb16Btn.FormatW(CNewProjDlg::m_wLanguageRes[108]);
	m_autoplayCheckBtn.FormatW(CNewProjDlg::m_wLanguageRes[715]);
	m_channelCheckBtn.FormatW(CNewProjDlg::m_wLanguageRes[616]);
	m_aviCheckBtn.FormatW(CNewProjDlg::m_wLanguageRes[124]);
	m_264Check.FormatW(CNewProjDlg::m_wLanguageRes[123]);

	m_systemParamStatic.SetTextColor(TEXT_COLOR_GRAY);
	m_systemParamStatic.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_systemParamStatic.FormatW(CNewProjDlg::m_wLanguageRes[106]);
	m_systemParamStatic.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_windowParamStc.SetTextColor(TEXT_COLOR_GRAY);
	m_windowParamStc.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_windowParamStc.FormatW(CNewProjDlg::m_wLanguageRes[535]);
	m_windowParamStc.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_mapParamStc.SetTextColor(TEXT_COLOR_GRAY);
	m_mapParamStc.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_mapParamStc.FormatW(CNewProjDlg::m_wLanguageRes[105]);
	m_mapParamStc.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_videoParamStc.SetTextColor(TEXT_COLOR_GRAY);
	m_videoParamStc.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_videoParamStc.FormatW(CNewProjDlg::m_wLanguageRes[109]);
	m_videoParamStc.SetBkColor(BK_COLOR, 0, CxStatic::Normal);
		
	m_fileAssStc.SetTextColor(TEXT_COLOR_GRAY);
	m_fileAssStc.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_fileAssStc.FormatW(CNewProjDlg::m_wLanguageRes[125]);
	m_fileAssStc.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_showGPSTrackBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);	
	if (CNewProjDlg::g_config_Value_ST.bShowGPSTrack == TRUE) 
		m_showGPSTrackBtn.SetCheck(1);
	else 
		m_showGPSTrackBtn.SetCheck(0);

	m_waterMarkBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bWtrmkVerify == TRUE) 
		m_waterMarkBtn.SetCheck(1);
	else 
		m_waterMarkBtn.SetCheck(0);

	m_rgb16Btn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bUseRGB16 == TRUE) 
		m_rgb16Btn.SetCheck(1);
	else 
		m_rgb16Btn.SetCheck(0);

	m_ioAlarmBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bIOAlarm == TRUE) 
		m_ioAlarmBtn.SetCheck(1);
	else 
		m_ioAlarmBtn.SetCheck(0);

	m_GpsOffBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bGPSOffsetCorre == TRUE) 
		m_GpsOffBtn.SetCheck(1);
	else 
		m_GpsOffBtn.SetCheck(0);

	m_driverComBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bDriverComp == TRUE) 
		m_driverComBtn.SetCheck(1);
	else 
		m_driverComBtn.SetCheck(0);

	m_deviceCamBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bDevCamera == TRUE) 
		m_deviceCamBtn.SetCheck(1);
	else 
		m_deviceCamBtn.SetCheck(0);

	m_channelCheckBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bShowChanNum == TRUE) 
		m_channelCheckBtn.SetCheck(1);
	else 
		m_channelCheckBtn.SetCheck(0);

	m_aviCheckBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bAVIFile == 1) 
		m_aviCheckBtn.SetCheck(1);
	else 
		m_aviCheckBtn.SetCheck(0);

	m_autoplayCheckBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bAutoPlay == TRUE) 
		m_autoplayCheckBtn.SetCheck(1);
	else 
		m_autoplayCheckBtn.SetCheck(0);

	m_accCheck.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.bACCWinker == TRUE) 
		m_accCheck.SetCheck(1);
	else 
		m_accCheck.SetCheck(0);

	m_264Check.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if (CNewProjDlg::g_config_Value_ST.b264File == 1) 
		m_264Check.SetCheck(1);
	else 
		m_264Check.SetCheck(0);

	m_showDateBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);

	m_cmb_language.InsertString(0, _T("简体中文"));
	m_cmb_language.InsertString(1, _T("English"));
	m_cmb_language.InsertString(2, _T("繁體中文"));
	m_cmb_language.InsertString(3, _T("ภาษาไทย"));
	m_cmb_language.InsertString(4, _T("España"));
	m_cmb_language.InsertString(5, _T("português"));
	m_cmb_language.InsertString(6, _T("Türkiye"));
	m_cmb_language.SetCurSel(CNewProjDlg::g_config_Value_ST.nLanguageID);

	m_cmb_windows.AddString(_T("One Window"));
	m_cmb_windows.AddString(_T("Two Window"));
	m_cmb_windows.SetCurSel(CNewProjDlg::g_config_Value_ST.nWndCounts - 1);

	m_cmb_player_view_scale.AddString(_T("16 : 9"));
	m_cmb_player_view_scale.AddString(_T("4 : 3"));
	m_cmb_player_view_scale.AddString(_T("Auto"));
	m_cmb_player_view_scale.SetCurSel(CNewProjDlg::g_config_Value_ST.nViewScaleID);

	m_cmb_map_type.AddString(_T("Google"));
	m_cmb_map_type.AddString(_T("MapInfo"));
	m_cmb_map_type.AddString(_T("BaiDu"));
	m_cmb_map_type.SetCurSel(CNewProjDlg::g_config_Value_ST.nMapTypeID);

	m_cmb_speed_unit.AddString(_T("km/h"));
	m_cmb_speed_unit.AddString(_T("mph"));
	m_cmb_speed_unit.SetCurSel(CNewProjDlg::g_config_Value_ST.nSpeedUnitID);

	m_cmb_io_alarm_posi.AddString(_T("Top"));
	m_cmb_io_alarm_posi.AddString(_T("Below"));
	m_cmb_io_alarm_posi.SetCurSel(CNewProjDlg::g_config_Value_ST.nIOAlarmCmb);

	m_cmb_acc_winker.AddString(_T("Top"));
	m_cmb_acc_winker.AddString(_T("Below"));
	m_cmb_acc_winker.SetCurSel(CNewProjDlg::g_config_Value_ST.nACCWinkerCmb);

	m_cmb_driver_company.AddString(_T("Top"));
	m_cmb_driver_company.AddString(_T("Below"));
	m_cmb_driver_company.SetCurSel(CNewProjDlg::g_config_Value_ST.nDriverCompCmb);

	m_cmb_device_camera.AddString(_T("Top"));
	m_cmb_device_camera.AddString(_T("Below"));
	m_cmb_device_camera.SetCurSel(CNewProjDlg::g_config_Value_ST.nDevCameraCmb);

	m_edit_wtmk_vert.SetTextColor(TEXT_COLOR);
	m_edit_wtmk_hori.SetTextColor(TEXT_COLOR);
	m_edit_watermask.SetTextColor(TEXT_COLOR);
	m_edit_alarm_vert.SetTextColor(TEXT_COLOR);
	m_edit_alarm_hori.SetTextColor(TEXT_COLOR);
	m_edit_dri_company_vert.SetTextColor(TEXT_COLOR);
	m_edit_dri_company_hori.SetTextColor(TEXT_COLOR);
	m_edit_dev_camera_vert.SetTextColor(TEXT_COLOR);
	m_edit_dev_camera_hori.SetTextColor(TEXT_COLOR);
	m_edit_acc_winker_vert.SetTextColor(TEXT_COLOR);
	m_edit_acc_winder_hori.SetTextColor(TEXT_COLOR);

	wchar_t buf[4];
	char	temp[4];
	int n = sizeof(CNewProjDlg::g_config_Value_ST);

	m_edit_watermask.SetWindowText(CNewProjDlg::g_config_Value_ST.chWtrmk);
	itoa(CNewProjDlg::g_config_Value_ST.nWtrmk_Hori, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_wtmk_hori.SetWindowText(buf);

	itoa(CNewProjDlg::g_config_Value_ST.nWtrmk_Vert, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_wtmk_vert.SetWindowText(buf);

	itoa(CNewProjDlg::g_config_Value_ST.nIOAlarm_Hori, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_alarm_hori.SetWindowText(buf);

	itoa(CNewProjDlg::g_config_Value_ST.nIOAlarm_Vert, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_alarm_vert.SetWindowText(buf);

	itoa(CNewProjDlg::g_config_Value_ST.nACCWinker_Hori, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_acc_winder_hori.SetWindowText(buf);

	itoa(CNewProjDlg::g_config_Value_ST.nACCWinker_Vert, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_acc_winker_vert.SetWindowText(buf);

	itoa(CNewProjDlg::g_config_Value_ST.nDriverComp_Hori, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_dri_company_hori.SetWindowText(buf);

	itoa(CNewProjDlg::g_config_Value_ST.nDriverComp_Vert, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_dri_company_vert.SetWindowText(buf);

	itoa(CNewProjDlg::g_config_Value_ST.nDevCamera_Hori, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_dev_camera_hori.SetWindowText(buf);

	itoa(CNewProjDlg::g_config_Value_ST.nDevCamera_Vert, temp, 10);
	Ascii2Unichar(temp, buf);
	m_edit_dev_camera_vert.SetWindowText(buf);

	m_static_language.SetTextColor(TEXT_COLOR_GRAY);
	m_static_language.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_language.FormatW(CNewProjDlg::m_wLanguageRes[97]);
	m_static_language.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_windows.SetTextColor(TEXT_COLOR_GRAY);
	m_static_windows.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_windows.FormatW(CNewProjDlg::m_wLanguageRes[129]);
	m_static_windows.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_viewscale.SetTextColor(TEXT_COLOR_GRAY);
	m_static_viewscale.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_viewscale.FormatW(CNewProjDlg::m_wLanguageRes[231]);
	m_static_viewscale.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_speedunit.SetTextColor(TEXT_COLOR_GRAY);
	m_static_speedunit.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_speedunit.FormatW(CNewProjDlg::m_wLanguageRes[618]);
	m_static_speedunit.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_maptype.SetTextColor(TEXT_COLOR_GRAY);
	m_static_maptype.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_maptype.FormatW(CNewProjDlg::m_wLanguageRes[135]);
	m_static_maptype.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_watermask.SetTextColor(TEXT_COLOR_GRAY);
	m_static_watermask.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_watermask.FormatW(CNewProjDlg::m_wLanguageRes[273]);
	m_static_watermask.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_wtmkhori.SetTextColor(TEXT_COLOR_GRAY);
	m_static_wtmkhori.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_wtmkhori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_wtmkhori.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_wtmkvert.SetTextColor(TEXT_COLOR_GRAY);
	m_static_wtmkvert.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_wtmkvert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_wtmkvert.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_wtmkhoripix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_wtmkhoripix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_wtmkhoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_wtmkhoripix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_wtmkvertpix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_wtmkvertpix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_wtmkvertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_wtmkvertpix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_alarmposi.SetTextColor(TEXT_COLOR_GRAY);
	m_static_alarmposi.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_alarmposi.FormatW(CNewProjDlg::m_wLanguageRes[37]);
	m_static_alarmposi.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_alarmhori.SetTextColor(TEXT_COLOR_GRAY);
	m_static_alarmhori.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_alarmhori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_alarmhori.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_alarmvert.SetTextColor(TEXT_COLOR_GRAY);
	m_static_alarmvert.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_alarmvert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_alarmvert.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_alarmhoripix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_alarmhoripix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_alarmhoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_alarmhoripix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_alarmvertpix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_alarmvertpix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_alarmvertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_alarmvertpix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_accposi.SetTextColor(TEXT_COLOR_GRAY);
	m_static_accposi.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_accposi.FormatW(CNewProjDlg::m_wLanguageRes[37]);
	m_static_accposi.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_acchori.SetTextColor(TEXT_COLOR_GRAY);
	m_static_acchori.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_acchori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_acchori.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_accvert.SetTextColor(TEXT_COLOR_GRAY);
	m_static_accvert.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_accvert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_accvert.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_acchoripix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_acchoripix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_acchoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_acchoripix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_accvertpix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_accvertpix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_accvertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_accvertpix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_driposi.SetTextColor(TEXT_COLOR_GRAY);
	m_static_driposi.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_driposi.FormatW(CNewProjDlg::m_wLanguageRes[37]);
	m_static_driposi.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_drihori.SetTextColor(TEXT_COLOR_GRAY);
	m_static_drihori.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_drihori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_drihori.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_drivert.SetTextColor(TEXT_COLOR_GRAY);
	m_static_drivert.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_drivert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_drivert.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_drihoripix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_drihoripix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_drihoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_drihoripix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_drivertpix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_drivertpix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_drivertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_drivertpix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_devposi.SetTextColor(TEXT_COLOR_GRAY);
	m_static_devposi.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_devposi.FormatW(CNewProjDlg::m_wLanguageRes[37]);
	m_static_devposi.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_devhori.SetTextColor(TEXT_COLOR_GRAY);
	m_static_devhori.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_devhori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_devhori.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_devvert.SetTextColor(TEXT_COLOR_GRAY);
	m_static_devvert.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_devvert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_devvert.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_devhoripix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_devhoripix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_devhoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_devhoripix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);

	m_static_devvertpix.SetTextColor(TEXT_COLOR_GRAY);
	m_static_devvertpix.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_devvertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_devvertpix.SetBkColor(BK_COLOR, 0, CxStatic::Normal);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

void CSystemSetupDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CSystemSetupDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
// 		CRect rt;
// 		CDC* pDC = GetDC();
// 		GetClientRect(&rt);
// 		pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), BK_COLOR);
	// Do not call CDialog::OnPaint() for painting messages]
	
}

BOOL CSystemSetupDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), BK_COLOR);
	return TRUE;//CDialog::OnEraseBkgnd(pDC);
}

void CSystemSetupDlg::OnOK() 
{
	// TODO: Add extra validation here

	if (m_264Check.GetCheck() == 1)	
		CNewProjDlg::g_config_Value_ST.b264File = TRUE; 
	else 
		CNewProjDlg::g_config_Value_ST.b264File = FALSE;

	if (m_aviCheckBtn.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bAVIFile = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bAVIFile = FALSE;

	if (m_showGPSTrackBtn.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bShowGPSTrack = TRUE;
	else
		CNewProjDlg::g_config_Value_ST.bShowGPSTrack = FALSE;

	if (m_waterMarkBtn.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bWtrmkVerify = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bWtrmkVerify = FALSE;

	if (m_rgb16Btn.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bUseRGB16 = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bUseRGB16 = FALSE;

	if (m_ioAlarmBtn.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bIOAlarm = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bIOAlarm = FALSE;

	if (m_GpsOffBtn.GetCheck() == 1)
		CNewProjDlg::g_config_Value_ST.bGPSOffsetCorre = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bGPSOffsetCorre = FALSE;

	if (m_driverComBtn.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bDriverComp = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bDriverComp = FALSE;

	if (m_deviceCamBtn.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bDevCamera = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bDevCamera = FALSE;

	if (m_channelCheckBtn.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bShowChanNum = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bShowChanNum = FALSE;

	if (m_autoplayCheckBtn.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bAutoPlay = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bAutoPlay = FALSE;

	if (m_accCheck.GetCheck() == 1) 
		CNewProjDlg::g_config_Value_ST.bACCWinker = TRUE;
	else 
		CNewProjDlg::g_config_Value_ST.bACCWinker = FALSE;
	
	if (m_cmb_windows.GetCurSel()== 0) 
		CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	else 
		CNewProjDlg::g_config_Value_ST.nWndCounts = 2;

	if (m_cmb_io_alarm_posi.GetCurSel() == 0) 
		CNewProjDlg::g_config_Value_ST.nIOAlarmCmb = 0;
	else 
		CNewProjDlg::g_config_Value_ST.nIOAlarmCmb = 1;

	if (m_cmb_acc_winker.GetCurSel() == 0) 
		CNewProjDlg::g_config_Value_ST.nACCWinkerCmb = 0;
	else 
		CNewProjDlg::g_config_Value_ST.nACCWinkerCmb = 1;

	if (m_cmb_driver_company.GetCurSel() == 0) 
		CNewProjDlg::g_config_Value_ST.nDriverCompCmb = 0;
	else 
		CNewProjDlg::g_config_Value_ST.nDriverCompCmb = 1;

	if (m_cmb_device_camera.GetCurSel() == 0) 
		CNewProjDlg::g_config_Value_ST.nDevCameraCmb = 0;
	else 
		CNewProjDlg::g_config_Value_ST.nDevCameraCmb = 1;

	switch(m_cmb_player_view_scale.GetCurSel())
	{
	case 0:
		CNewProjDlg::g_config_Value_ST.nViewScaleID = 0;//16 : 9
		break;
	case 1:
		CNewProjDlg::g_config_Value_ST.nViewScaleID = 1;//4 : 3
		break;
	case 2:
		CNewProjDlg::g_config_Value_ST.nViewScaleID = 2;//Auto
		break;
	default:
		break;
	}

	switch(m_cmb_map_type.GetCurSel())
	{
	case 0:
		CNewProjDlg::g_config_Value_ST.nMapTypeID = 0;
		break;
	case 1:
		CNewProjDlg::g_config_Value_ST.nMapTypeID = 1;
		break;
	case 2:
		CNewProjDlg::g_config_Value_ST.nMapTypeID = 2;
		break;
	default:
		break;
	}

	CString str;
	m_edit_wtmk_hori.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nWtrmk_Hori = wtoi(str);

	m_edit_wtmk_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nWtrmk_Vert = wtoi(str);

	m_edit_alarm_hori.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nIOAlarm_Hori = wtoi(str);

	m_edit_alarm_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nIOAlarm_Vert = wtoi(str);

	m_edit_acc_winder_hori.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nACCWinker_Hori = wtoi(str);

	m_edit_acc_winker_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nACCWinker_Vert = wtoi(str);

	m_edit_dri_company_hori.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nDriverComp_Hori = wtoi(str);

	m_edit_dri_company_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nDriverComp_Vert = wtoi(str);

	m_edit_dev_camera_hori.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nDevCamera_Hori = wtoi(str);

	m_edit_dev_camera_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nDevCamera_Vert = wtoi(str);

	m_edit_watermask.GetWindowText(str);
	memset(CNewProjDlg::g_config_Value_ST.chWtrmk, 0, 20);
// 	for(int i = 0; i < str.GetLength(); i++)
// 		memset(CNewProjDlg::g_config_Value_ST.chWtrmk[i] , str.Mid(i, 1), 1);
	wcscpy(CNewProjDlg::g_config_Value_ST.chWtrmk, str);
	if (m_cmb_speed_unit.GetCurSel() == 0) 
		CNewProjDlg::g_config_Value_ST.nSpeedUnitID = 0;
	else 
		CNewProjDlg::g_config_Value_ST.nSpeedUnitID = 1;

	CFile file;
	file.Open(_T(CONFIG_FILE_PATH), CFile::modeWrite);
	file.SeekToBegin();

	CNewProjDlg::g_config_Value_ST.nLanguageID = m_cmb_language.GetCurSel();
	file.Write(&CNewProjDlg::g_config_Value_ST, sizeof(Config_Datas));
	file.Close();

	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);

	if (CNewProjDlg::g_config_Value_ST.nWndCounts == 1)
	{
		if (CNewProjDlg::g_config_Value_ST.nSelWndID == 1)
		{
			CNewProjDlg::m_View1Dlg.MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3 / 5, TRUE);
			CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
			CNewProjDlg::m_View2Dlg.ShowWindow(SW_HIDE);
		}
		else
		{
			CNewProjDlg::m_View2Dlg.MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3 / 5, TRUE);
			CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
			CNewProjDlg::m_View1Dlg.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		CNewProjDlg::m_View1Dlg.MoveWindow(0, 0, rt.Width() / 3, rt.Height() * 3 / 5, TRUE);
		CNewProjDlg::m_View2Dlg.MoveWindow(rt.Width() / 3, 0, rt.Width() / 3, rt.Height() * 3 / 5, TRUE);
		CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::m_View2Dlg,ShowWindow(SW_SHOW);
	}

	WndsBK_Paint(&CNewProjDlg::m_View1Dlg, &CNewProjDlg::m_View2Dlg, CNewProjDlg::g_config_Value_ST.nSelWndID);
	CNewProjDlg::g_bViewChange = true;
	CDialog::OnOK();
}

void CSystemSetupDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}
// HBRUSH CSystemSetupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
// {
// 	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
// 	int nItem = pWnd->GetDlgCtrlID();
// 	switch (nItem)
// 	{
// 	case IDC_LANGUAGE_COMBO:
// 		if ( pWnd->IsWindowEnabled( )) {
// 		//pDC->SetTextColor(m_clrDLText);
// 		pDC->SetBkColor(RGB(47,56,66));
// 		}
// 		break;
// 	}
// 	// TODO: Change any attributes of the DC here
// 	
// 	// TODO: Return a different brush if the default is not desired
// 	return hbr;
// }

void CSystemSetupDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonUp(nFlags, point);
}
