#if !defined(AFX_BURNTODISCDLG_H__A3CCF75D_9CDD_4923_B419_CE882C0B7E2D__INCLUDED_)
#define AFX_BURNTODISCDLG_H__A3CCF75D_9CDD_4923_B419_CE882C0B7E2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurnToDiscDlg.h : header file
//
#include "CxStatic.h"
#include "EditTrans.h"
#include "xCheckButton.h"
#include "ReportCtrl.h"
#include <afxmt.h>
#include "FileObject.h"
#include <list>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CBurnToDiscDlg dialog

class CBurnToDiscDlg : public CDialog
{
// Construction
public:
	CBurnToDiscDlg(CWnd* pParent = NULL);   // standard constructor
public:
	void	SetCancelBurning(bool bCancel);
	bool	GetCancelBurning();

	void	EnableUI(BOOL bEnable);
	void	AddRecordersToComboBox();
	CString	GetMediaTypeString(int mediaType);
	void	UpdateCapacity();
	void	EnableBurnButton();
	void	ClearListControl();
static bool CreateMediaFileSystem(CBurnToDiscDlg* pThis, /*IMAPI_MEDIA_PHYSICAL_TYPE*/DWORD mediaType, IStream** dataStream);

	void UpdateTimes(LONG totalTime, LONG remainingTime);
	void UpdateBuffer(LONG usedSystemBuffer, LONG totalSystemBuffer);
	void UpdateProgress(LONG writtenSectors, LONG totalSectors);

public:
	list <CFileObject*> m_fileObjectList;
	bool	m_isCdromSupported;
	bool	m_isDvdSupported;
	bool	m_isDualLayerDvdSupported;
	int		m_selectedMediaType;
	bool	m_isBurning;
	bool	m_cancelBurn;
	int		m_nFileCounts;
	CCriticalSection m_critSection;

	CString m_volumeLabel;
	BOOL m_closeMedia;
	BOOL m_ejectWhenFinished;
// Dialog Data
	//{{AFX_DATA(CBurnToDiscDlg)
	enum { IDD = IDD_BURN_TO_DISC };
	CProgressCtrl	m_progress_burn;
	CComboBox	m_cmb_media_type;
	CComboBox	m_cmb_select_drive;
	CButton	m_btn_Start;
	CButton	m_btn_Cancel;
	CxStatic	m_cameraChsStc;
	CxCheckButton	m_checkEject;
	CReportCtrl		m_burnListCtrl;
	CxCheckButton	m_checkChan2;
	CxCheckButton	m_checkChan1;
	CxCheckButton	m_checkChan1_2;
	CEditTrans	m_edit_volumlabel;
	CxStatic	m_static_burn;
	CxStatic	m_static_volumelabel;
	CxStatic	m_static_totalsize;
	CxStatic	m_static_timeleftVal;
	CxStatic	m_static_timeleft;
	CxStatic	m_static_totalsizeVal;
	CxStatic	m_static_starttime;
	CxStatic	m_static_selectedmediatype;
	CxStatic	m_static_estimatedtimeVal;
	CxStatic	m_static_estimatedtime;
	CxStatic	m_static_endtime;
	CxStatic	m_static_burningdrive;
	CxStatic	m_static_availableVal;
	CxStatic	m_static_available;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBurnToDiscDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeDeviceCombo();
	afx_msg void OnLbnSelchangeBurnFileList();
	afx_msg void OnBnClickedAddFilesButton();
	afx_msg void OnDestroy();

	afx_msg void OnBurnStartButton();
	afx_msg LRESULT OnImapiUpdate(WPARAM, LPARAM);
	afx_msg LRESULT OnBurnStatusMessage(WPARAM, LPARAM);
	afx_msg LRESULT OnBurnFinished(WPARAM, LPARAM);

	afx_msg void OnCbnSelchangeMediaTypeCombo();
	afx_msg void OnBnClickedRemoveFilesButton();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBurnToDiscDlg)


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURNTODISCDLG_H__A3CCF75D_9CDD_4923_B419_CE882C0B7E2D__INCLUDED_)
