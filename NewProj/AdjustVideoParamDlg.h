#if !defined(AFX_ADJUSTVIDEOPARAMDLG_H__F040ABCA_B084_4D2B_A2C8_6A9B8DABB6DB__INCLUDED_)
#define AFX_ADJUSTVIDEOPARAMDLG_H__F040ABCA_B084_4D2B_A2C8_6A9B8DABB6DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjustVideoParamDlg.h : header file
//
#include "CxStatic.h"
#include "EditTrans.h"
#include "ImageTextButton.h"
#include "xCheckButton.h"
#include "BitSlider.h"
/////////////////////////////////////////////////////////////////////////////
// CAdjustVideoParamDlg dialog
class CAdjustVideoParamDlg : public CDialog
{
// Construction
public:
	CAdjustVideoParamDlg(CWnd* pParent = NULL);   // standard constructor
public:
	bool		m_bSel12;
	bool		m_bSel1;
	bool		m_bSel2;
// Dialog Data
	//{{AFX_DATA(CAdjustVideoParamDlg)
	enum { IDD = IDD_ADJUST_VIDEO_PARAM };
	CBitSlider	m_sat_slider;
	CBitSlider	m_cont_slider;
	CBitSlider	m_light_slider;
	CBitSlider	m_hue_slider;
	CButton	m_btn_Ok;
	CButton	m_btn_Cancel;
	CButton	m_btn_Restore;
	CxStatic		m_cameraChsStc;
	CxCheckButton	m_checkChan1_2;
	CxCheckButton	m_checkChan2;
	CxCheckButton	m_checkChan1;
	CxCheckButton	m_checkTrans2;
	CxCheckButton	m_checkTrans1;
	CxCheckButton	m_checkRealTime;
	CEditTrans	m_edit_saturation;
	CEditTrans	m_edit_light;
	CEditTrans	m_edit_hue;
	CEditTrans	m_edit_contract;
	CxStatic	m_static_saturation;
	CxStatic	m_static_light;
	CxStatic	m_static_hue;
	CxStatic	m_static_contract;
	BYTE	m_hueValue;
	int		m_lightValue;
	BYTE	m_satValue;
	int		m_contValue;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjustVideoParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdjustVideoParamDlg)
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnRestoreButton();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSel1_2Check();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
// private:
// 	CImageTextButton	m_btn_restore;
// 	CImageTextButton	m_btn_ok;
// 	CImageTextButton	m_btn_cancel;
public:
	afx_msg void OnBnClickedRealtimelyCheck();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJUSTVIDEOPARAMDLG_H__F040ABCA_B084_4D2B_A2C8_6A9B8DABB6DB__INCLUDED_)
