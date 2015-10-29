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
	m_systemParamStatic.FormatW(CNewProjDlg::m_wLanguageRes[106]);
	m_windowParamStc.FormatW(CNewProjDlg::m_wLanguageRes[535]);
	m_mapParamStc.FormatW(CNewProjDlg::m_wLanguageRes[105]);
	m_videoParamStc.FormatW(CNewProjDlg::m_wLanguageRes[109]);
	m_fileAssStc.FormatW(CNewProjDlg::m_wLanguageRes[125]);
	//m_btn_Ok.SetWindowText((LPCTSTR)CNewProjDlg::m_wLanguageRes[1]);

	m_systemParamStatic.SetTextColor(RGB(200,200,200));
	m_systemParamStatic.SetFont("Arial", 14, FW_NORMAL);
	//m_systemParamStatic.FormatW(CNewProjDlg::m_wLanguageRes[231]);
	m_systemParamStatic.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_windowParamStc.SetTextColor(RGB(200,200,200));
	m_windowParamStc.SetFont("Arial", 14, FW_NORMAL);
	//m_systemParamStatic.FormatW(CNewProjDlg::m_wLanguageRes[231]);
	m_windowParamStc.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_mapParamStc.SetTextColor(RGB(200,200,200));
	m_mapParamStc.SetFont("Arial", 14, FW_NORMAL);
	//m_systemParamStatic.FormatW(CNewProjDlg::m_wLanguageRes[231]);
	m_mapParamStc.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_videoParamStc.SetTextColor(RGB(200,200,200));
	m_videoParamStc.SetFont("Arial", 14, FW_NORMAL);
	//m_systemParamStatic.FormatW(CNewProjDlg::m_wLanguageRes[231]);
	m_videoParamStc.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);
		
	m_fileAssStc.SetTextColor(RGB(200,200,200));
	m_fileAssStc.SetFont("Arial", 14, FW_NORMAL);
	//m_systemParamStatic.FormatW(CNewProjDlg::m_wLanguageRes[231]);
	m_fileAssStc.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_showGPSTrackBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bShowGPSTrack == TRUE) m_showGPSTrackBtn.SetCheck(1);
	else m_showGPSTrackBtn.SetCheck(0);
	m_waterMarkBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bWtrmkVerify == TRUE) m_waterMarkBtn.SetCheck(1);
	else m_waterMarkBtn.SetCheck(0);
	m_rgb16Btn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bUseRGB16 == TRUE) m_rgb16Btn.SetCheck(1);
	else m_rgb16Btn.SetCheck(0);
	m_ioAlarmBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bIOAlarm == TRUE) m_ioAlarmBtn.SetCheck(1);
	else m_ioAlarmBtn.SetCheck(0);
	m_GpsOffBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bGPSOffsetCorre == TRUE) m_GpsOffBtn.SetCheck(1);
	else m_GpsOffBtn.SetCheck(0);
	m_driverComBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bDriverComp == TRUE) m_driverComBtn.SetCheck(1);
	else m_driverComBtn.SetCheck(0);
	m_deviceCamBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bDevCamera == TRUE) m_deviceCamBtn.SetCheck(1);
	else m_deviceCamBtn.SetCheck(0);
	m_channelCheckBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bShowChanNum == TRUE) m_channelCheckBtn.SetCheck(1);
	else m_channelCheckBtn.SetCheck(0);
	m_aviCheckBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bAVIFile == 1) m_aviCheckBtn.SetCheck(1);
	else m_aviCheckBtn.SetCheck(0);
	m_autoplayCheckBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bAutoPlay == TRUE) m_autoplayCheckBtn.SetCheck(1);
	else m_autoplayCheckBtn.SetCheck(0);
	m_accCheck.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.bACCWinker == TRUE) m_accCheck.SetCheck(1);
	else m_accCheck.SetCheck(0);
	m_264Check.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);
	if(CNewProjDlg::g_config_Value_ST.b264File == 1) m_264Check.SetCheck(1);
	else m_264Check.SetCheck(0);
	m_showDateBtn.SetIcon(IDI_UNCHECKED, BS_LEFT, IDI_CHECKED);

	m_cmb_language.InsertString(0, _T("?ïÅê»Ûì"));
	m_cmb_language.InsertString(1, _T("English"));
	m_cmb_language.InsertString(2, _T("ÝÐïÎê»Ûì"));
	m_cmb_language.InsertString(3, _T("???????"));
	m_cmb_language.InsertString(4, _T("Espa?a"));
	m_cmb_language.InsertString(5, _T("portugu?s"));
	m_cmb_language.InsertString(6, _T("T?rkiye"));
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

	m_cmb_speed_unit.AddString("km/h");
	m_cmb_speed_unit.AddString("mph");
	m_cmb_speed_unit.SetCurSel(CNewProjDlg::g_config_Value_ST.nSpeedUnitID);
	m_cmb_io_alarm_posi.AddString("Top");
	m_cmb_io_alarm_posi.AddString("Below");
	m_cmb_io_alarm_posi.SetCurSel(CNewProjDlg::g_config_Value_ST.nIOAlarmCmb);
	m_cmb_acc_winker.AddString("Top");
	m_cmb_acc_winker.AddString("Below");
	m_cmb_acc_winker.SetCurSel(CNewProjDlg::g_config_Value_ST.nACCWinkerCmb);
	m_cmb_driver_company.AddString("Top");
	m_cmb_driver_company.AddString("Below");
	m_cmb_driver_company.SetCurSel(CNewProjDlg::g_config_Value_ST.nDriverCompCmb);
	m_cmb_device_camera.AddString("Top");
	m_cmb_device_camera.AddString("Below");
	m_cmb_device_camera.SetCurSel(CNewProjDlg::g_config_Value_ST.nDevCameraCmb);

	m_edit_wtmk_vert.SetTextColor(RGB(255, 255, 255));
	m_edit_wtmk_hori.SetTextColor(RGB(255, 255, 255));
	m_edit_watermask.SetTextColor(RGB(255, 255, 255));
	m_edit_alarm_vert.SetTextColor(RGB(255, 255, 255));
	m_edit_alarm_hori.SetTextColor(RGB(255, 255, 255));
	m_edit_dri_company_vert.SetTextColor(RGB(255, 255, 255));
	m_edit_dri_company_hori.SetTextColor(RGB(255, 255, 255));
	m_edit_dev_camera_vert.SetTextColor(RGB(255, 255, 255));
	m_edit_dev_camera_hori.SetTextColor(RGB(255, 255, 255));
	m_edit_acc_winker_vert.SetTextColor(RGB(255, 255, 255));
	m_edit_acc_winder_hori.SetTextColor(RGB(255, 255, 255));
	char buf[4];
	m_edit_watermask.SetWindowText(CNewProjDlg::g_config_Value_ST.chWtrmk);
	itoa(CNewProjDlg::g_config_Value_ST.nWtrmk_Hori, buf, 10);
	m_edit_wtmk_hori.SetWindowText(buf);
	itoa(CNewProjDlg::g_config_Value_ST.nWtrmk_Vert, buf, 10);
	m_edit_wtmk_vert.SetWindowText(buf);
	itoa(CNewProjDlg::g_config_Value_ST.nIOAlarm_Hori, buf, 10);
	m_edit_alarm_hori.SetWindowText(buf);
	itoa(CNewProjDlg::g_config_Value_ST.nIOAlarm_Vert, buf, 10);
	m_edit_alarm_vert.SetWindowText(buf);
	itoa(CNewProjDlg::g_config_Value_ST.nACCWinker_Hori, buf, 10);
	m_edit_acc_winder_hori.SetWindowText(buf);
	itoa(CNewProjDlg::g_config_Value_ST.nACCWinker_Vert, buf, 10);
	m_edit_acc_winker_vert.SetWindowText(buf);
	itoa(CNewProjDlg::g_config_Value_ST.nDriverComp_Hori, buf, 10);
	m_edit_dri_company_hori.SetWindowText(buf);
	itoa(CNewProjDlg::g_config_Value_ST.nDriverComp_Vert, buf, 10);
	m_edit_dri_company_vert.SetWindowText(buf);
	itoa(CNewProjDlg::g_config_Value_ST.nDevCamera_Hori, buf, 10);
	m_edit_dev_camera_hori.SetWindowText(buf);
	itoa(CNewProjDlg::g_config_Value_ST.nDevCamera_Vert, buf, 10);
	m_edit_dev_camera_vert.SetWindowText(buf);
	m_static_language.SetTextColor(RGB(200,200,200));
	m_static_language.SetFont("Arial", 14, FW_NORMAL);
	m_static_language.FormatW(CNewProjDlg::m_wLanguageRes[97]);
	m_static_language.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_windows.SetTextColor(RGB(200,200,200));
	m_static_windows.SetFont("Arial", 14, FW_NORMAL);
	m_static_windows.FormatW(CNewProjDlg::m_wLanguageRes[129]);
	m_static_windows.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_viewscale.SetTextColor(RGB(200,200,200));
	m_static_viewscale.SetFont("Arial", 14, FW_NORMAL);
	m_static_viewscale.FormatW(CNewProjDlg::m_wLanguageRes[231]);
	m_static_viewscale.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_speedunit.SetTextColor(RGB(200,200,200));
	m_static_speedunit.SetFont("Arial", 14, FW_NORMAL);
	m_static_speedunit.FormatW(CNewProjDlg::m_wLanguageRes[618]);
	m_static_speedunit.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_maptype.SetTextColor(RGB(200,200,200));
	m_static_maptype.SetFont("Arial", 14, FW_NORMAL);
	m_static_maptype.FormatW(CNewProjDlg::m_wLanguageRes[135]);
	m_static_maptype.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_watermask.SetTextColor(RGB(200,200,200));
	m_static_watermask.SetFont("Arial", 14, FW_NORMAL);
	m_static_watermask.FormatW(CNewProjDlg::m_wLanguageRes[273]);
	m_static_watermask.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_wtmkhori.SetTextColor(RGB(200,200,200));
	m_static_wtmkhori.SetFont("Arial", 14, FW_NORMAL);
	m_static_wtmkhori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_wtmkhori.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_wtmkvert.SetTextColor(RGB(200,200,200));
	m_static_wtmkvert.SetFont("Arial", 14, FW_NORMAL);
	m_static_wtmkvert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_wtmkvert.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_wtmkhoripix.SetTextColor(RGB(200,200,200));
	m_static_wtmkhoripix.SetFont("Arial", 14, FW_NORMAL);
	m_static_wtmkhoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_wtmkhoripix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_wtmkvertpix.SetTextColor(RGB(200,200,200));
	m_static_wtmkvertpix.SetFont("Arial", 14, FW_NORMAL);
	m_static_wtmkvertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_wtmkvertpix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_alarmposi.SetTextColor(RGB(200,200,200));
	m_static_alarmposi.SetFont("Arial", 14, FW_NORMAL);
	m_static_alarmposi.FormatW(CNewProjDlg::m_wLanguageRes[37]);
	m_static_alarmposi.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_alarmhori.SetTextColor(RGB(200,200,200));
	m_static_alarmhori.SetFont("Arial", 14, FW_NORMAL);
	m_static_alarmhori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_alarmhori.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_alarmvert.SetTextColor(RGB(200,200,200));
	m_static_alarmvert.SetFont("Arial", 14, FW_NORMAL);
	m_static_alarmvert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_alarmvert.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_alarmhoripix.SetTextColor(RGB(200,200,200));
	m_static_alarmhoripix.SetFont("Arial", 14, FW_NORMAL);
	m_static_alarmhoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_alarmhoripix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_alarmvertpix.SetTextColor(RGB(200,200,200));
	m_static_alarmvertpix.SetFont("Arial", 14, FW_NORMAL);
	m_static_alarmvertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_alarmvertpix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_accposi.SetTextColor(RGB(200,200,200));
	m_static_accposi.SetFont("Arial", 14, FW_NORMAL);
	m_static_accposi.FormatW(CNewProjDlg::m_wLanguageRes[37]);
	m_static_accposi.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_acchori.SetTextColor(RGB(200,200,200));
	m_static_acchori.SetFont("Arial", 14, FW_NORMAL);
	m_static_acchori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_acchori.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_accvert.SetTextColor(RGB(200,200,200));
	m_static_accvert.SetFont("Arial", 14, FW_NORMAL);
	m_static_accvert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_accvert.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_acchoripix.SetTextColor(RGB(200,200,200));
	m_static_acchoripix.SetFont("Arial", 14, FW_NORMAL);
	m_static_acchoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_acchoripix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_accvertpix.SetTextColor(RGB(200,200,200));
	m_static_accvertpix.SetFont("Arial", 14, FW_NORMAL);
	m_static_accvertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_accvertpix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_driposi.SetTextColor(RGB(200,200,200));
	m_static_driposi.SetFont("Arial", 14, FW_NORMAL);
	m_static_driposi.FormatW(CNewProjDlg::m_wLanguageRes[37]);
	m_static_driposi.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_drihori.SetTextColor(RGB(200,200,200));
	m_static_drihori.SetFont("Arial", 14, FW_NORMAL);
	m_static_drihori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_drihori.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_drivert.SetTextColor(RGB(200,200,200));
	m_static_drivert.SetFont("Arial", 14, FW_NORMAL);
	m_static_drivert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_drivert.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_drihoripix.SetTextColor(RGB(200,200,200));
	m_static_drihoripix.SetFont("Arial", 14, FW_NORMAL);
	m_static_drihoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_drihoripix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_drivertpix.SetTextColor(RGB(200,200,200));
	m_static_drivertpix.SetFont("Arial", 14, FW_NORMAL);
	m_static_drivertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_drivertpix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_devposi.SetTextColor(RGB(200,200,200));
	m_static_devposi.SetFont("Arial", 14, FW_NORMAL);
	m_static_devposi.FormatW(CNewProjDlg::m_wLanguageRes[37]);
	m_static_devposi.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_devhori.SetTextColor(RGB(200,200,200));
	m_static_devhori.SetFont("Arial", 14, FW_NORMAL);
	m_static_devhori.FormatW(CNewProjDlg::m_wLanguageRes[117]);
	m_static_devhori.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_devvert.SetTextColor(RGB(200,200,200));
	m_static_devvert.SetFont("Arial", 14, FW_NORMAL);
	m_static_devvert.FormatW(CNewProjDlg::m_wLanguageRes[118]);
	m_static_devvert.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_devhoripix.SetTextColor(RGB(200,200,200));
	m_static_devhoripix.SetFont("Arial", 14, FW_NORMAL);
	m_static_devhoripix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_devhoripix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_devvertpix.SetTextColor(RGB(200,200,200));
	m_static_devvertpix.SetFont("Arial", 14, FW_NORMAL);
	m_static_devvertpix.FormatW(CNewProjDlg::m_wLanguageRes[120]);
	m_static_devvertpix.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);
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
// 		pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));
	// Do not call CDialog::OnPaint() for painting messages]
	
}

BOOL CSystemSetupDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));
	return TRUE;//CDialog::OnEraseBkgnd(pDC);
}

void CSystemSetupDlg::OnOK() 
{
	// TODO: Add extra validation here

	if(m_264Check.GetCheck() == 1)	CNewProjDlg::g_config_Value_ST.b264File = TRUE; 
	else CNewProjDlg::g_config_Value_ST.b264File = FALSE;
	if(m_aviCheckBtn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bAVIFile = TRUE;
	else CNewProjDlg::g_config_Value_ST.bAVIFile = FALSE;
	if(m_showGPSTrackBtn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bShowGPSTrack = TRUE;
	else CNewProjDlg::g_config_Value_ST.bShowGPSTrack = FALSE;
	if(m_waterMarkBtn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bWtrmkVerify = TRUE;
	else CNewProjDlg::g_config_Value_ST.bWtrmkVerify = FALSE;
	if(m_rgb16Btn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bUseRGB16 = TRUE;
	else CNewProjDlg::g_config_Value_ST.bUseRGB16 = FALSE;
	if(m_ioAlarmBtn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bIOAlarm = TRUE;
	else CNewProjDlg::g_config_Value_ST.bIOAlarm = FALSE;
	if(m_GpsOffBtn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bGPSOffsetCorre = TRUE;
	else CNewProjDlg::g_config_Value_ST.bGPSOffsetCorre = FALSE;
	if(m_driverComBtn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bDriverComp = TRUE;
	else CNewProjDlg::g_config_Value_ST.bDriverComp = FALSE;
	if(m_deviceCamBtn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bDevCamera = TRUE;
	else CNewProjDlg::g_config_Value_ST.bDevCamera = FALSE;
	if(m_channelCheckBtn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bShowChanNum = TRUE;
	else CNewProjDlg::g_config_Value_ST.bShowChanNum = FALSE;
	if(m_autoplayCheckBtn.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bAutoPlay = TRUE;
	else CNewProjDlg::g_config_Value_ST.bAutoPlay = FALSE;
	if(m_accCheck.GetCheck() == 1) CNewProjDlg::g_config_Value_ST.bACCWinker = TRUE;
	else CNewProjDlg::g_config_Value_ST.bACCWinker = FALSE;
	
	if(m_cmb_windows.GetCurSel()== 0) CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	else CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	if(m_cmb_io_alarm_posi.GetCurSel() == 0) CNewProjDlg::g_config_Value_ST.nIOAlarmCmb = 0;
	else CNewProjDlg::g_config_Value_ST.nIOAlarmCmb = 1;
	if(m_cmb_acc_winker.GetCurSel() == 0) CNewProjDlg::g_config_Value_ST.nACCWinkerCmb = 0;
	else CNewProjDlg::g_config_Value_ST.nACCWinkerCmb = 1;
	if(m_cmb_driver_company.GetCurSel() == 0) CNewProjDlg::g_config_Value_ST.nDriverCompCmb = 0;
	else CNewProjDlg::g_config_Value_ST.nDriverCompCmb = 1;
	if(m_cmb_device_camera.GetCurSel() == 0) CNewProjDlg::g_config_Value_ST.nDevCameraCmb = 0;
	else CNewProjDlg::g_config_Value_ST.nDevCameraCmb = 1;
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
	CNewProjDlg::g_config_Value_ST.nWtrmk_Hori = atoi(str);
	m_edit_wtmk_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nWtrmk_Vert = atoi(str);
	m_edit_alarm_hori.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nIOAlarm_Hori = atoi(str);
	m_edit_alarm_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nIOAlarm_Vert = atoi(str);
	m_edit_acc_winder_hori.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nACCWinker_Hori = atoi(str);
	m_edit_acc_winker_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nACCWinker_Vert = atoi(str);
	m_edit_dri_company_hori.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nDriverComp_Hori = atoi(str);
	m_edit_dri_company_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nDriverComp_Vert = atoi(str);
	m_edit_dev_camera_hori.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nDevCamera_Hori = atoi(str);
	m_edit_dev_camera_vert.GetWindowText(str);
	CNewProjDlg::g_config_Value_ST.nDevCamera_Vert = atoi(str);

	m_edit_watermask.GetWindowText(str);
	memset(CNewProjDlg::g_config_Value_ST.chWtrmk, 0, 20);
	for(int i = 0; i < str.GetLength(); i++)
		memset(&CNewProjDlg::g_config_Value_ST.chWtrmk[i] , *_T(str.Mid(i, 1)), 1);
	if(m_cmb_speed_unit.GetCurSel() == 0) CNewProjDlg::g_config_Value_ST.nSpeedUnitID = 0;
	else CNewProjDlg::g_config_Value_ST.nSpeedUnitID = 1;
	CFile file;
	file.Open("config.sav", CFile::modeWrite);
	file.SeekToBegin();
	CNewProjDlg::g_config_Value_ST.nLanguageID = m_cmb_language.GetCurSel();
	file.Write(&CNewProjDlg::g_config_Value_ST, sizeof(Config_Datas));
	file.Close();
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	if(CNewProjDlg::g_config_Value_ST.nWndCounts == 1)
	{
		if(CNewProjDlg::g_config_Value_ST.nSelWndID == 1)
		{
			CNewProjDlg::m_View1Dlg.MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3 / 5, TRUE);
			CNewProjDlg::m_View1Dlg.ShowWindow(SW_SHOW);
			CNewProjDlg::m_View2Dlg.ShowWindow(SW_HIDE);
		}else{
			CNewProjDlg::m_View2Dlg.MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3 / 5, TRUE);
			CNewProjDlg::m_View2Dlg.ShowWindow(SW_SHOW);
			CNewProjDlg::m_View1Dlg.ShowWindow(SW_HIDE);
		}
	}else{
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
// 		if( pWnd->IsWindowEnabled( )) {
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
