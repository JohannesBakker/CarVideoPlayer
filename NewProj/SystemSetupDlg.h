#if !defined(AFX_SYSTEMSETUPDLG_H__997DD831_5DF1_4256_92DE_6A4D5DC3D320__INCLUDED_)
#define AFX_SYSTEMSETUPDLG_H__997DD831_5DF1_4256_92DE_6A4D5DC3D320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemSetupDlg.h : header file
//
#include "CxStatic.h"
#include "EditTrans.h"
#include "NewProjDlg.h"
#include "xCheckButton.h"
#include "BitComboBox.h"
#include "ImageTextButton.h"
/////////////////////////////////////////////////////////////////////////////
// CSystemSetupDlg dialog

class CSystemSetupDlg : public CDialog
{
// Construction
public:
	CSystemSetupDlg(CWnd* pParent = NULL);   // standard constructor
public:
// Dialog Data
	//{{AFX_DATA(CSystemSetupDlg)
	enum { IDD = IDD_SYSTEM_SETUP };
	CButton	m_btn_Cancel;
	CButton	m_btn_Ok;
	CxStatic	m_fileAssStc;
	CxStatic	m_videoParamStc;
	CxStatic	m_mapParamStc;
	CxStatic	m_windowParamStc;
	CxStatic	m_systemParamStatic;
	CxCheckButton	m_showDateBtn;
	CxCheckButton	m_waterMarkBtn;
	CxCheckButton	m_rgb16Btn;
	CxCheckButton	m_ioAlarmBtn;
	CxCheckButton	m_GpsOffBtn;
	CxCheckButton	m_driverComBtn;
	CxCheckButton	m_deviceCamBtn;
	CxCheckButton	m_channelCheckBtn;
	CxCheckButton	m_aviCheckBtn;
	CxCheckButton	m_autoplayCheckBtn;
	CxCheckButton	m_accCheck;
	CxCheckButton	m_264Check;
	CxCheckButton	m_showGPSTrackBtn;
	CEditTrans	m_edit_wtmk_vert;
	CEditTrans	m_edit_wtmk_hori;
	CEditTrans	m_edit_watermask;
	CEditTrans	m_edit_alarm_vert;
	CEditTrans	m_edit_alarm_hori;
	CEditTrans	m_edit_dri_company_vert;
	CEditTrans	m_edit_dri_company_hori;
	CEditTrans	m_edit_dev_camera_vert;
	CEditTrans	m_edit_dev_camera_hori;
	CEditTrans	m_edit_acc_winker_vert;
	CEditTrans	m_edit_acc_winder_hori;
	CxStatic	m_static_wtmkvertpix;
	CxStatic	m_static_wtmkvert;
	CxStatic	m_static_wtmkhoripix;
	CxStatic	m_static_wtmkhori;
	CxStatic	m_static_windows;
	CxStatic	m_static_watermask;
	CxStatic	m_static_viewscale;
	CxStatic	m_static_speedunit;
	CxStatic	m_static_maptype;
	CxStatic	m_static_language;
	CxStatic	m_static_drivertpix;
	CxStatic	m_static_drivert;
	CxStatic	m_static_driposi;
	CxStatic	m_static_drihoripix;
	CxStatic	m_static_drihori;
	CxStatic	m_static_devvertpix;
	CxStatic	m_static_devvert;
	CxStatic	m_static_devposi;
	CxStatic	m_static_devhoripix;
	CxStatic	m_static_devhori;
	CxStatic	m_static_alarmvertpix;
	CxStatic	m_static_alarmvert;
	CxStatic	m_static_alarmposi;
	CxStatic	m_static_alarmhoripix;
	CxStatic	m_static_alarmhori;
	CxStatic	m_static_accvertpix;
	CxStatic	m_static_accvert;
	CxStatic	m_static_accposi;
	CxStatic	m_static_acchoripix;
	CxStatic	m_static_acchori;
	CComboBox	m_cmb_speed_unit;
	CComboBox	m_cmb_driver_company;
	CComboBox	m_cmb_device_camera;
	CButton	m_btn_clear_password;
	CComboBox	m_cmb_acc_winker;
	CComboBox	m_cmb_io_alarm_posi;
	CComboBox	m_cmb_language;
	CComboBox	m_cmb_map_type;
	CComboBox	m_cmb_player_view_scale;
	CComboBox	m_cmb_windows;
	BOOL	m_chk_wtrmk;
	BOOL	m_chk_use_rgb16;
	BOOL	m_chk_show_data_window;
	BOOL	m_chk_io_alarm;
	BOOL	m_chk_264_file;
	BOOL	m_chk_acc_winker;
	BOOL	m_chk_avi_file;
	BOOL	m_chk_autoplay;
	BOOL	m_chk_channel_number;
	BOOL	m_chk_device_camera;
	BOOL	m_chk_driver_company;
	BOOL	m_chk_gps_offset_corre;
	BOOL	m_chk_show_gps_track;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowChannelNumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMSETUPDLG_H__997DD831_5DF1_4256_92DE_6A4D5DC3D320__INCLUDED_)
