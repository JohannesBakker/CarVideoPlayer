// BurnToDiscDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "BurnToDiscDlg.h"
#include "DiscMaster.h"
#include "DiscRecorder.h"
#include "DiscFormatData.h"
#include "BaseObject.h"
#include "FileObject.h"
#include "DirObject.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IMAPI_MEDIA_TYPE_UNKNOWN				0x2A
#define IMAPI_MEDIA_TYPE_CDROM					0
#define	IMAPI_MEDIA_TYPE_CDR					1
#define IMAPI_MEDIA_TYPE_CDRW					2
#define IMAPI_MEDIA_TYPE_DVDROM					3
#define IMAPI_MEDIA_TYPE_DVDRAM					4
#define IMAPI_MEDIA_TYPE_DVDPLUSR				5
#define IMAPI_MEDIA_TYPE_DVDPLUSRW				6
#define IMAPI_MEDIA_TYPE_DVDPLUSR_DUALLAYER		7
#define	IMAPI_MEDIA_TYPE_DVDDASHR				8
#define IMAPI_MEDIA_TYPE_DVDDASHRW				9
#define IMAPI_MEDIA_TYPE_DVDDASHR_DUALLAYER		10
#define	IMAPI_MEDIA_TYPE_DISK					11
#define	IMAPI_MEDIA_TYPE_DVDPLUSRW_DUALLAYER	12
#define	IMAPI_MEDIA_TYPE_HDDVDROM				13
#define	IMAPI_MEDIA_TYPE_HDDVDR					14
#define	IMAPI_MEDIA_TYPE_HDDVDRAM				15
#define	IMAPI_MEDIA_TYPE_BDROM					16
#define	IMAPI_MEDIA_TYPE_BDR					17
#define	IMAPI_MEDIA_TYPE_BDRE					18
#define IMAPI_FORMAT2_DATA_WRITE_ACTION_VALIDATING_MEDIA		0
#define IMAPI_FORMAT2_DATA_WRITE_ACTION_FORMATTING_MEDIA		1
#define IMAPI_FORMAT2_DATA_WRITE_ACTION_INITIALIZING_HARDWARE	2
#define IMAPI_FORMAT2_DATA_WRITE_ACTION_CALIBRATING_POWER		3
#define IMAPI_FORMAT2_DATA_WRITE_ACTION_WRITING_DATA			4
#define	IMAPI_FORMAT2_DATA_WRITE_ACTION_FINALIZATION			5
#define IMAPI_FORMAT2_DATA_WRITE_ACTION_COMPLETED				6

#define IMAPI_E_FSI_INTERNAL_ERROR				0xC0AAB100

#define PBST_NORMAL		true
#define PBST_ERROR		false
#define CD_MEDIA		0
#define DVD_MEDIA		1
#define DL_DVD_MEDIA	2

#define WM_BURN_STATUS_MESSAGE	WM_APP+300
#define WM_BURN_FINISHED		WM_APP+301

#define CLIENT_NAME		_T("Record Player")
/////////////////////////////////////////////////////////////////////////////
// CBurnToDiscDlg dialog


CBurnToDiscDlg::CBurnToDiscDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBurnToDiscDlg::IDD, pParent)
, m_closeMedia(TRUE)
, m_cancelBurn(false)
, m_selectedMediaType(-1)
, m_isBurning(false)
, m_ejectWhenFinished(TRUE)
,m_nFileCounts(0)
,m_fileObjectList(NULL)
{
	//{{AFX_DATA_INIT(CBurnToDiscDlg)
	//}}AFX_DATA_INIT
}


void CBurnToDiscDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBurnToDiscDlg)
	DDX_Control(pDX, IDC_BURN_PROGRESS, m_progress_burn);
	DDX_Control(pDX, IDC_SEL_MEDIA_TYPE_COMBO, m_cmb_media_type);
	DDX_Control(pDX, IDC_BURNING_DRIVE_COMBO, m_cmb_select_drive);
	DDX_Control(pDX, IDC_BURN_START_BUTTON, m_btn_Start);
	DDX_Control(pDX, IDC_EXIT_BUTTON, m_btn_Cancel);
	DDX_Control(pDX, IDC_CAMERA_CHOOSE_STATIC, m_cameraChsStc);
	DDX_Control(pDX, IDC_EJECT_CHECK, m_checkEject);
	DDX_Control(pDX, IDC_LIST, m_burnListCtrl);
	DDX_Control(pDX, IDC_CHAN2_CHECK, m_checkChan2);
	DDX_Control(pDX, IDC_CHAN1_CHECK, m_checkChan1);
	DDX_Control(pDX, IDC_CHAN1_2_CHECK, m_checkChan1_2);
	DDX_Control(pDX, IDC_VOLUMELABEL_EDIT, m_edit_volumlabel);
	DDX_Control(pDX, IDC_BURN_STATIC, m_static_burn);
	DDX_Control(pDX, IDC_VOLUMELABEL_STATIC, m_static_volumelabel);
	DDX_Control(pDX, IDC_TOTALSIZE_STATIC, m_static_totalsize);
	DDX_Control(pDX, IDC_TIMELEFTVAL_STATIC, m_static_timeleftVal);
	DDX_Control(pDX, IDC_TIMELEFT_STATIC, m_static_timeleft);
	DDX_Control(pDX, IDC_TATOLSIZEVAL_STATIC, m_static_totalsizeVal);
	DDX_Control(pDX, IDC_STARTTIME_STATIC, m_static_starttime);
	DDX_Control(pDX, IDC_SELECTEDMEDIATYPE_STATIC, m_static_selectedmediatype);
	DDX_Control(pDX, IDC_ESTIMATEDTIMEVAL_STATIC, m_static_estimatedtimeVal);
	DDX_Control(pDX, IDC_ESTIMATEDTIME_STATIC, m_static_estimatedtime);
	DDX_Control(pDX, IDC_ENDTIME_STATIC, m_static_endtime);
	DDX_Control(pDX, IDC_BURNINGDRIVE_STATIC, m_static_burningdrive);
	DDX_Control(pDX, IDC_AVAILABLEVAL_STATIC, m_static_availableVal);
	DDX_Control(pDX, IDC_AVAILABLE_STATIC, m_static_available);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBurnToDiscDlg, CDialog)
	//{{AFX_MSG_MAP(CBurnToDiscDlg)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_BURNING_DRIVE_COMBO, OnCbnSelchangeDeviceCombo)
    ON_LBN_SELCHANGE(IDC_LIST, OnLbnSelchangeBurnFileList)
    ON_BN_CLICKED(IDC_ADD_FILE_BUTTON, OnBnClickedAddFilesButton)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BURN_START_BUTTON, OnBurnStartButton)
    ON_MESSAGE(WM_IMAPI_UPDATE, OnImapiUpdate)
    ON_MESSAGE(WM_BURN_STATUS_MESSAGE, OnBurnStatusMessage)
    ON_MESSAGE(WM_BURN_FINISHED, OnBurnFinished)
    ON_CBN_SELCHANGE(IDC_SEL_MEDIA_TYPE_COMBO, OnCbnSelchangeMediaTypeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurnToDiscDlg message handlers

void CBurnToDiscDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
		CRect rt;
		CDC*pDC = GetDC();
		GetClientRect(&rt);
		dc.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CBurnToDiscDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	HBITMAP hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_START));
	BITMAP bitmap;
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_Start.SetBitmap(hBmp);

	hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_CANCEL));
	::GetObject(hBmp,sizeof(bitmap),&bitmap);
	m_btn_Cancel.SetBitmap(hBmp);

	m_checkChan1.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);
	m_checkChan2.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);
	m_checkChan1_2.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);
	m_checkEject.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);

	m_burnListCtrl.SetHeadings("File Name, 120; File Size,120");
	m_burnListCtrl.SetBkColor(RGB(47,56,66));
	m_burnListCtrl.SetTextBkColor(RGB(47,56,66));
	m_burnListCtrl.SetTextColor(RGB(255,255,255));
	m_burnListCtrl.SetGridLines(TRUE);

	m_edit_volumlabel.SetTextColor(RGB(255, 255, 255));

	m_cameraChsStc.SetTextColor(RGB(200,200,200));
	m_cameraChsStc.SetFont("Arial", 14, FW_NORMAL);
	m_cameraChsStc.FormatW(CNewProjDlg::m_wLanguageRes[588]);
	m_cameraChsStc.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_burningdrive.SetTextColor(RGB(200,200,200));
	m_static_burningdrive.SetFont("Arial", 14, FW_NORMAL);
	m_static_burningdrive.FormatW(CNewProjDlg::m_wLanguageRes[588]);
	m_static_burningdrive.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_selectedmediatype.SetTextColor(RGB(200,200,200));
	m_static_selectedmediatype.SetFont("Arial", 14, FW_NORMAL);
	m_static_selectedmediatype.FormatW(CNewProjDlg::m_wLanguageRes[603]);
	m_static_selectedmediatype.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_volumelabel.SetTextColor(RGB(200,200,200));
	m_static_volumelabel.SetFont("Arial", 14, FW_NORMAL);
	m_static_volumelabel.FormatW(CNewProjDlg::m_wLanguageRes[589]);
	m_static_volumelabel.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_starttime.SetTextColor(RGB(200,200,200));
	m_static_starttime.SetFont("Arial", 14, FW_NORMAL);
	m_static_starttime.FormatW(CNewProjDlg::m_wLanguageRes[590]);
	m_static_starttime.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_endtime.SetTextColor(RGB(200,200,200));
	m_static_endtime.SetFont("Arial", 14, FW_NORMAL);
	m_static_endtime.FormatW(CNewProjDlg::m_wLanguageRes[591]);
	m_static_endtime.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_totalsize.SetTextColor(RGB(200,200,200));
	m_static_totalsize.SetFont("Arial", 14, FW_NORMAL);
	m_static_totalsize.FormatW(CNewProjDlg::m_wLanguageRes[304]);
	m_static_totalsize.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_totalsizeVal.SetTextColor(RGB(200,200,200));
	m_static_totalsizeVal.SetFont("Arial", 14, FW_NORMAL);
	m_static_totalsizeVal.FormatW(_T(""));
	m_static_totalsizeVal.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_available.SetTextColor(RGB(200,200,200));
	m_static_available.SetFont("Arial", 14, FW_NORMAL);
	m_static_available.FormatW(_T("Available"));
	m_static_available.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_availableVal.SetTextColor(RGB(200,200,200));
	m_static_availableVal.SetFont("Arial", 14, FW_NORMAL);
	m_static_availableVal.FormatW(_T(""));
	m_static_availableVal.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_estimatedtime.SetTextColor(RGB(200,200,200));
	m_static_estimatedtime.SetFont("Arial", 14, FW_NORMAL);
	m_static_estimatedtime.FormatW(CNewProjDlg::m_wLanguageRes[604]);
	m_static_estimatedtime.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_estimatedtimeVal.SetTextColor(RGB(200,200,200));
	m_static_estimatedtimeVal.SetFont("Arial", 14, FW_NORMAL);
	m_static_estimatedtimeVal.FormatW(_T(""));
	m_static_estimatedtimeVal.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_timeleft.SetTextColor(RGB(200,200,200));
	m_static_timeleft.SetFont("Arial", 14, FW_NORMAL);
	m_static_timeleft.FormatW(CNewProjDlg::m_wLanguageRes[605]);
	m_static_timeleft.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_timeleftVal.SetTextColor(RGB(200,200,200));
	m_static_timeleftVal.SetFont("Arial", 14, FW_NORMAL);
	m_static_timeleftVal.FormatW(_T(""));
	m_static_timeleftVal.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);

	m_static_burn.SetTextColor(RGB(200,200,200));
	m_static_burn.SetFont("Arial", 14, FW_NORMAL);
	m_static_burn.FormatW(CNewProjDlg::m_wLanguageRes[303]);
	m_static_burn.SetBkColor(RGB(47, 56, 66), 0, CxStatic::Normal);
	
    m_progress_burn.SetRange(0,100);
	AddRecordersToComboBox();
    OnLbnSelchangeBurnFileList();
    EnableBurnButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
UINT BurnThread(LPVOID lParam)
{
	CBurnToDiscDlg* pThis = (CBurnToDiscDlg*)lParam;

    //
    // Get the selected recording device from the combobox
    //
    int selectedIndex = pThis->m_cmb_select_drive.GetCurSel();
    ASSERT(selectedIndex >= 0);
    if (selectedIndex < 0)
    {
        pThis->SendMessage(WM_BURN_FINISHED, 0, (LPARAM)_T("Error: No Device Selected"));
        return 0;
    }

    CDiscRecorder* pOrigDiscRecorder = 
        (CDiscRecorder*)pThis->m_cmb_select_drive.GetItemDataPtr(selectedIndex);
    if (pOrigDiscRecorder == NULL)
    {
        //
        // This should never happen
        //
        pThis->SendMessage(WM_BURN_FINISHED, 0, (LPARAM)_T("Error: No Data for selected device"));
        return 0;
    }

    //
    // Did user cancel?
    //
    if (pThis->GetCancelBurning())
    {
        pThis->SendMessage(WM_BURN_FINISHED, 0, (LPARAM)_T("User Canceled!"));
        return 0;
    }

    pThis->SendMessage(WM_BURN_STATUS_MESSAGE, 0, (LPARAM)_T("Initializing Disc Recorder..."));

    //
    // Create another disc recorder because we're in a different thread
    //
    CDiscRecorder discRecorder;

    CString errorMessage;
    if (discRecorder.Initialize(pOrigDiscRecorder->GetUniqueId()))
    {
        //
        // 
        //
        if (discRecorder.AcquireExclusiveAccess(true, CLIENT_NAME))
        {
            CDiscFormatData discFormatData;
            if (discFormatData.Initialize(&discRecorder, CLIENT_NAME))
            {
                //
                // Get the media type currently in the recording device
                //
                DWORD mediaType = IMAPI_MEDIA_TYPE_UNKNOWN;
				void* lpDiscFormatData = discFormatData.GetInterface();
				HRESULT hRes;
				__asm{
					mov		esi, lpDiscFormatData;
					mov     eax, esi;
					mov     ecx, [eax];
					lea		edx, mediaType;
					push	edx;
					push	esi;
					mov     eax,dword ptr [ecx+4]
					call	eax;
					mov		hRes, eax;
				}
//                discFormatData.GetInterface()->get_CurrentPhysicalMediaType(&mediaType);

                //
                // Create the file system
                //
                IStream* dataStream = NULL;
                if (!pThis->CreateMediaFileSystem(pThis, mediaType, &dataStream))
                {	// CreateMediaFileSystem reported error to UI
                    //return 0;
					MessageBox(NULL, _T("Failed CreateMediaFileSystem!!!"), NULL, 0);
					exit(0);
                }

                discFormatData.SetCloseMedia(pThis->m_closeMedia ? true : false);

                //
                // Burn the data, this does all the work
                //
                discFormatData.Burn(pThis->m_hWnd, dataStream);

                //
                // Release the IStream after burning
                //
                dataStream->Release();

                //
                // Eject Media if they chose
                //
                if (pThis->m_ejectWhenFinished)
                {
                    discRecorder.EjectMedia();
                }

            }

            discRecorder.ReleaseExclusiveAccess();

            //
            // Finished Burning, GetHresult will determine if it was successful or not
            //
            pThis->SendMessage(WM_BURN_FINISHED, discFormatData.GetHresult(), 
                (LPARAM)(LPCTSTR)discFormatData.GetErrorMessage());
        }
        else
        {
            errorMessage.Format(_T("Failed: %s is exclusive owner"),
                (LPCTSTR)discRecorder.ExclusiveAccessOwner());
            pThis->SendMessage(WM_BURN_FINISHED, discRecorder.GetHresult(), 
                (LPARAM)(LPCTSTR)errorMessage);
        }
    }
    else
    {
        errorMessage.Format(_T("Failed to initialize recorder - Unique ID:%s"),
            (LPCTSTR)pOrigDiscRecorder->GetUniqueId());
        pThis->SendMessage(WM_BURN_FINISHED, discRecorder.GetHresult(), 
            (LPARAM)(LPCTSTR)errorMessage);
    }
	return 0;
}
void CBurnToDiscDlg::OnBurnStartButton() 
{
	// TODO: Add your control notification handler code here
	if (m_isBurning)
    {
        SetCancelBurning(true);
    }
    else
    {
        SetCancelBurning(false);
        m_isBurning = true;
        UpdateData();
        EnableUI(false);

        AfxBeginThread(BurnThread, this, THREAD_PRIORITY_NORMAL);
	}
}
void CBurnToDiscDlg::SetCancelBurning(bool bCancel)
{
    CSingleLock singleLock(&m_critSection);
    m_cancelBurn = bCancel;
}

bool CBurnToDiscDlg::GetCancelBurning()
{
    CSingleLock singleLock(&m_critSection);
    return m_cancelBurn;
}
void CBurnToDiscDlg::EnableUI(BOOL bEnable)
{
    m_cmb_select_drive .EnableWindow(bEnable);
    m_burnListCtrl.EnableWindow(bEnable);
    m_cmb_media_type.EnableWindow(bEnable);
    GetDlgItem(IDC_ADD_FILE_BUTTON)->EnableWindow(bEnable);
    
    
    GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(bEnable);
    GetDlgItem(IDC_EJECT_CHECK)->EnableWindow(bEnable);
    GetDlgItem(IDC_VOLUMELABEL_EDIT)->EnableWindow(bEnable);

    GetDlgItem(IDC_BURN_START_BUTTON)->SetWindowText(bEnable ? 
        _T("Burn") : _T("Cancel"));
}
void CBurnToDiscDlg::AddRecordersToComboBox()
{
    CDiscMaster			discMaster;

    //
    // Cleanup old data on combobox
    //
    int itemCount = m_cmb_select_drive.GetCount();
    for (int itemIndex = 0; itemIndex < itemCount; itemIndex++)
    {
        delete (CDiscRecorder*)m_cmb_select_drive.GetItemData(itemIndex);
    }
    m_cmb_select_drive.ResetContent();


    if (!discMaster.Initialize())
    {
        AfxMessageBox(discMaster.GetErrorMessage(), MB_OK|MB_ICONERROR);
        EndDialog(IDOK);
        return;
    }

    //
    // Add Devices to ComboBox
    //
    long totalDevices = discMaster.GetTotalDevices();
    if (totalDevices == 0 && FAILED(discMaster.GetHresult()))
    {
        AfxMessageBox(discMaster.GetErrorMessage(), MB_OK|MB_ICONERROR);
    }

    for (long deviceIndex = 0; deviceIndex < totalDevices; deviceIndex++)
    {
        CString recorderUniqueID = discMaster.GetDeviceUniqueID(deviceIndex);
        if (recorderUniqueID.IsEmpty())
        {
            CString errorMessage(discMaster.GetErrorMessage());
            if (!errorMessage.IsEmpty())
            {
                AfxMessageBox(errorMessage, MB_OK|MB_ICONERROR);
                continue;
            }
        }

        //
        // Create an IDiscRecorder2
        //
        CDiscRecorder* pDiscRecorder = new CDiscRecorder();
        ASSERT(pDiscRecorder != NULL);
        if (pDiscRecorder == NULL)
            continue;

        if (!pDiscRecorder->Initialize(recorderUniqueID))
        {
            if (totalDevices == 1 && FAILED(pDiscRecorder->GetHresult()))
            {
                CString errorMessage;
                errorMessage.Format(_T("Failed to initialize recorder - Error:0x%08x\n\nRecorder Unique ID:%s"),
                    pDiscRecorder->GetHresult(), (LPCTSTR)recorderUniqueID);
                AfxMessageBox(errorMessage, MB_OK|MB_ICONERROR);
            }
            delete pDiscRecorder;
            continue;
        }

        //
        // Get the volume path(s). usually just 1
        //
        CString volumeList;
        ULONG totalVolumePaths = pDiscRecorder->GetTotalVolumePaths();
        for (ULONG volIndex = 0; volIndex < totalVolumePaths; volIndex++)
        {
            if (volIndex)
                volumeList += _T(",");
            volumeList += pDiscRecorder->GetVolumePath(volIndex);
        }

        //
        // Add Drive to combo and IDiscRecorder as data
        //
        CString productId = pDiscRecorder->GetProductID();
        CString strName;
        strName.Format(_T("%s [%s]"), (LPCTSTR)volumeList, (LPCTSTR)productId);
        int comboBoxIndex = m_cmb_select_drive.AddString(strName);
        m_cmb_select_drive.SetItemDataPtr(comboBoxIndex, pDiscRecorder);
    }

    if (totalDevices > 0)
    {
        m_cmb_select_drive.SetCurSel(0);
        OnCbnSelchangeDeviceCombo();
    }
}
void CBurnToDiscDlg::OnCbnSelchangeDeviceCombo()
{
    m_isCdromSupported = false;
    m_isDvdSupported = false;
    m_isDualLayerDvdSupported = false;

    m_cmb_media_type.ResetContent();

    int selectedIndex = m_cmb_select_drive.GetCurSel();
    ASSERT(selectedIndex >= 0);
    if (selectedIndex < 0)
    {
        return;
    }

    CDiscRecorder* discRecorder = 
        (CDiscRecorder*)m_cmb_select_drive.GetItemDataPtr(selectedIndex);
    if (discRecorder != NULL)
    {
        CDiscFormatData discFormatData;
        if  (!discFormatData.Initialize(discRecorder, CLIENT_NAME))//453893
        {
            return;
        }

        //
        // Display Supported Media Types
        //
        CString supportedMediaTypes;
        ULONG totalMediaTypes = discFormatData.GetTotalSupportedMediaTypes();
        for (ULONG volIndex = 0; volIndex < totalMediaTypes; volIndex++)
        {
            int mediaType = discFormatData.GetSupportedMediaType(volIndex);
            if (volIndex > 0)
                supportedMediaTypes += _T(", ");
            supportedMediaTypes += GetMediaTypeString(mediaType);
        }
         //
        // Add Media Selection
        //
        if (m_isCdromSupported)
        {
            int stringIndex = m_cmb_media_type.AddString(_T("700MB CD Media"));
            m_cmb_media_type.SetItemData(stringIndex, CD_MEDIA);
        }
        if (m_isDvdSupported)
        {
            int stringIndex = m_cmb_media_type.AddString(_T("4.7GB DVD Media"));
            m_cmb_media_type.SetItemData(stringIndex, DVD_MEDIA);
        }
        if (m_isDualLayerDvdSupported)
        {
            int stringIndex = m_cmb_media_type.AddString(_T("8.5GB Dual-Layer DVD"));
            m_cmb_media_type.SetItemData(stringIndex, DL_DVD_MEDIA);
        }
        m_cmb_media_type.SetCurSel(0);
        OnCbnSelchangeMediaTypeCombo();
    }
}

CString	CBurnToDiscDlg::GetMediaTypeString(int mediaType)
{
    switch (mediaType)
    {
    case IMAPI_MEDIA_TYPE_UNKNOWN:
    default:
        return _T("Unknown Media Type");

    case IMAPI_MEDIA_TYPE_CDROM:
        m_isCdromSupported = true;
        return _T("CD-ROM or CD-R/RW media");

    case IMAPI_MEDIA_TYPE_CDR:
        m_isCdromSupported = true;
        return _T("CD-R");

    case IMAPI_MEDIA_TYPE_CDRW:
        m_isCdromSupported = true;
        return _T("CD-RW");

    case IMAPI_MEDIA_TYPE_DVDROM:
        m_isDvdSupported = true;
        return _T("DVD ROM");

    case IMAPI_MEDIA_TYPE_DVDRAM:
        m_isDvdSupported = true;
        return _T("DVD-RAM");

    case IMAPI_MEDIA_TYPE_DVDPLUSR:
        m_isDvdSupported = true;
        return _T("DVD+R");

    case IMAPI_MEDIA_TYPE_DVDPLUSRW:
        m_isDvdSupported = true;
        return _T("DVD+RW");

    case IMAPI_MEDIA_TYPE_DVDPLUSR_DUALLAYER:
        m_isDualLayerDvdSupported = true;
        return _T("DVD+R Dual Layer");

    case IMAPI_MEDIA_TYPE_DVDDASHR:
        m_isDvdSupported = true;
        return _T("DVD-R");

    case IMAPI_MEDIA_TYPE_DVDDASHRW:
        m_isDvdSupported = true;
        return _T("DVD-RW");

    case IMAPI_MEDIA_TYPE_DVDDASHR_DUALLAYER:
        m_isDualLayerDvdSupported = true;
        return _T("DVD-R Dual Layer");

    case IMAPI_MEDIA_TYPE_DISK:
        return _T("random-access writes");

    case IMAPI_MEDIA_TYPE_DVDPLUSRW_DUALLAYER:
        m_isDualLayerDvdSupported = true;
        return _T("DVD+RW DL");

    case IMAPI_MEDIA_TYPE_HDDVDROM:
        return _T("HD DVD-ROM");

    case IMAPI_MEDIA_TYPE_HDDVDR:
        return _T("HD DVD-R");

    case IMAPI_MEDIA_TYPE_HDDVDRAM:
        return _T("HD DVD-RAM");

    case IMAPI_MEDIA_TYPE_BDROM:
        return _T("Blu-ray DVD (BD-ROM)");

    case IMAPI_MEDIA_TYPE_BDR:
        return _T("Blu-ray media");

    case IMAPI_MEDIA_TYPE_BDRE:
        return _T("Blu-ray Rewritable media");
    }
}
void CBurnToDiscDlg::OnCbnSelchangeMediaTypeCombo()
{
    int selectedIndex = m_cmb_media_type.GetCurSel();
    if (selectedIndex == -1)
    {
        m_selectedMediaType = -1;
    }
    else
    {
        m_selectedMediaType = (int)m_cmb_media_type.GetItemData(selectedIndex);
    }

    UpdateCapacity();
}
void CBurnToDiscDlg::UpdateCapacity()
{
    //
    // Set the selected media type data
    //
    ULONGLONG totalSize = 0;
    CString maxText;
    if (m_selectedMediaType == CD_MEDIA)
    {
        maxText = _T("700MB");
        totalSize = 700000000;
    }
    else if (m_selectedMediaType == DVD_MEDIA)
    {
        maxText = _T("4.7GB");
        totalSize = 4700000000;
    }
    else if (m_selectedMediaType == DL_DVD_MEDIA)
    {
        maxText = _T("8.5GB");
        totalSize = 8500000000;
    }
//    m_maxText.SetWindowText(maxText);

    //
    // Calculate the size of the files
    //
    ULONGLONG mediaSize = 0;
    int itemCount = m_burnListCtrl.GetItemCount();
    for (list <CFileObject*>::iterator itemIndex = m_fileObjectList.begin(); itemIndex != m_fileObjectList.end(); itemIndex++)
    {
        //CBaseObject* pObject = (CBaseObject*)m_fileObjectList
        mediaSize += (*itemIndex)->GetSizeOnDisc();
	}
    m_progress_burn.SetRange(0,100);
    if (mediaSize == 0)
    {
        m_progress_burn.SetPos(0);
#if 0// _MFC_VER >= 0x0900
        m_progress_burn.SetState(PBST_NORMAL);
#endif
    }
    else
    {
        int percent = (int)((mediaSize*100)/totalSize);
        if (percent > 100)
        {
            m_progress_burn.SetPos(100);
#if 0//_MFC_VER >= 0x0900
            m_progress_burn.SetState(PBST_ERROR);
#endif
        }
        else
        {
            m_progress_burn.SetPos(percent);
#if 0//_MFC_VER >= 0x0900
            m_progress_burn.SetState(PBST_NORMAL);
#endif
        }

    }
}
void CBurnToDiscDlg::EnableBurnButton()
{
    GetDlgItem(IDC_BURN_BUTTON)->EnableWindow(m_burnListCtrl.GetItemCount()>0);
}

void CBurnToDiscDlg::ClearListControl()
{
	m_burnListCtrl.DeleteAllItems();

}
bool CBurnToDiscDlg::CreateMediaFileSystem(CBurnToDiscDlg* pThis, /*IMAPI_MEDIA_PHYSICAL_TYPE*/DWORD mediaType, IStream** ppDataStream)
{
    void*		image = NULL;//IFileSystemImage
    void*		imageResult = NULL;//IFileSystemImageResult
    void*		rootItem = NULL;//IFsiDirectoryItem
    CString					message;
    bool					returnVal = false;
	CLSID		rclsid;
	IID			riid;
	rclsid.Data1 = 0x2C941FC5;
	rclsid.Data2 = 0x975B;
	rclsid.Data3 = 0x59BE;
	rclsid.Data4[0] = 0xA9;
	rclsid.Data4[1] = 0x60;
	rclsid.Data4[2] = 0x9A;
	rclsid.Data4[3] = 0x2A;
	rclsid.Data4[4] = 0x26;
	rclsid.Data4[5] = 0x28;
	rclsid.Data4[6] = 0x53;
	rclsid.Data4[7] = 0xA5;
	riid.Data1 = 0x2C941FE1;
	riid.Data2 = 0x975B;
	riid.Data3 = 0x59BE;
	riid.Data4[0] = 0xA9;
	riid.Data4[1] = 0x60;
	riid.Data4[2] = 0x9A;
	riid.Data4[3] = 0x2A;
	riid.Data4[4] = 0x26;
	riid.Data4[5] = 0x28;
	riid.Data4[6] = 0x53;
	riid.Data4[7] = 0xA5;
    HRESULT hr = CoCreateInstance(rclsid,
        NULL, CLSCTX_ALL, riid, (void**)&image);
    if (FAILED(hr) || (image == NULL))
    {
        pThis->SendMessage(WM_BURN_FINISHED, hr, 
            (LPARAM)_T("Failed to create IFileSystemImage Interface"));
        return false;
    }

    pThis->SendMessage(WM_BURN_STATUS_MESSAGE, 0, (LPARAM)_T("Creating File System..."));
	__asm{
		mov		esi, image;
		mov     eax, esi;
		mov     ecx, [eax];
		push	3;
		push	eax;
		mov     eax,dword ptr [ecx+74h]
		call	eax;
	}
//    image->put_FileSystemsToCreate((FsiFileSystems)(FsiFileSystemJoliet|FsiFileSystemISO9660));
	BSTR bStr = pThis->m_volumeLabel.AllocSysString();
	__asm{
		mov		esi, image;
		mov     eax, esi;
		mov     ecx, [eax];
		push	[bStr];
		push	eax;
		mov     eax,dword ptr [ecx+3Ch]
		call	eax;
	}
//    image->put_VolumeName(pThis->m_volumeLabel.AllocSysString());
	__asm{
		mov		esi, image;
		mov     eax, esi;
		mov     ecx, [eax];
		push	[mediaType];
		push	eax;
		mov     eax,dword ptr [ecx+8Ch]
		call	eax;
	}
//    image->ChooseImageDefaultsForMediaType(mediaType);

    //
    // Get the image root
    //
	__asm{
		mov		esi, image;
		mov     eax, esi;
		mov     ecx, [eax];
		push	rootItem;
		push	eax;
		mov     eax,dword ptr [ecx+54h]
		call	eax;
		mov		hr, eax;
	}
//    hr = image->get_Root(&rootItem);
    if (SUCCEEDED(hr))
    {
        //
        // Add Files and Directories to File System Image
        //
        int itemCount = pThis->m_burnListCtrl.GetItemCount();
        for (int itemIndex = 0; itemIndex < itemCount; itemIndex++)
        {
            CBaseObject* pObject = (CBaseObject*)pThis->m_burnListCtrl.GetItemData(itemIndex);
            ASSERT(pObject != NULL);
            if (pObject == NULL)
                continue;

            CString fileName = pObject->GetName();
            message.Format(_T("Adding \"%s\" to file system..."), (LPCTSTR)fileName);
            pThis->SendMessage(WM_BURN_STATUS_MESSAGE, 0, (LPARAM)(LPCTSTR)message);

            if (pObject->IsKindOf(RUNTIME_CLASS(CFileObject)))
            {
                CFileObject* pFileObject = (CFileObject*)pObject;
                IStream* fileStream = pFileObject->GetStream();
                if (fileStream != NULL)
                {
					BSTR strName = pFileObject->GetName().AllocSysString();
					__asm{
						mov		esi, rootItem;
						mov     eax, esi;
						mov     ecx, [eax];
						push	fileStream;
						push	strName;
						push	eax;
						mov     eax,dword ptr [ecx+60h]
						call	eax;
						mov		hr, eax;
					}
//                  hr = rootItem->AddFile(pFileObject->GetName().AllocSysString(), fileStream);
                    if (FAILED(hr))
                    {
                        // IMAPI_E_IMAGE_SIZE_LIMIT 0xc0aab120
                        message.Format(_T("Failed IFsiDirectoryItem->AddFile(%s)!"), 
                            (LPCTSTR)pFileObject->GetName());
                        pThis->SendMessage(WM_BURN_FINISHED, hr, (LPARAM)(LPCTSTR)message);
                        break;
                    }
                }
            }
            else if (pObject->IsKindOf(RUNTIME_CLASS(CDirObject)))
            {
                CDirObject* pDirObject = (CDirObject*)pObject;
				BSTR strPath = pDirObject->GetName().AllocSysString();
				__asm{
					mov		esi, rootItem;
					mov     eax, esi;
					mov     ecx, [eax];
					push	0xFFFFFFFF;
					push	strPath;
					push	eax;
					mov     eax,dword ptr [ecx+64h]
					call	eax;
					mov		hr, eax;
				}
//                hr = rootItem->AddTree(pDirObject->GetPath().AllocSysString(), VARIANT_TRUE);

                if (FAILED(hr))
                {
                    // IMAPI_E_IMAGE_SIZE_LIMIT 0xc0aab120
                    message.Format(_T("Failed IFsiDirectoryItem->AddTree(%s)!"), 
                        (LPCTSTR)pDirObject->GetName());
                    pThis->SendMessage(WM_BURN_FINISHED, hr, (LPARAM)(LPCTSTR)message);
                    break;
                }
            }

            //
            // Did user cancel?
            //
            if (pThis->GetCancelBurning())
            {
                pThis->SendMessage(WM_BURN_FINISHED, 0, (LPARAM)_T("User Canceled!"));
                hr = IMAPI_E_FSI_INTERNAL_ERROR;
            }
        }

        if (SUCCEEDED(hr))
        {
            // Create the result image
			__asm{
				mov		esi, image;
				mov     eax, esi;
				mov     ecx, [eax];
				lea		edx, [imageResult];
				push	edx;
				push	eax;
				mov     eax,dword ptr [ecx+9Ch]
				call	eax;
				mov		hr, eax;
			}
//            hr = image->CreateResultImage(&imageResult);
            if (SUCCEEDED(hr))
            {
                //
                // Get the stream
                //
				__asm{
					mov		esi, imageResult;
					mov     eax, esi;
					mov     ecx, [eax];
					push	ppDataStream;
					push	eax;
					mov     eax,dword ptr [ecx+1Ch]
					call	eax;
					mov		hr, eax;
				}
//                hr = imageResult->get_ImageStream(ppDataStream);
                if (SUCCEEDED(hr))
                {
                    returnVal = true;
                }
                else
                {
                    pThis->SendMessage(WM_BURN_FINISHED, hr, 
                        (LPARAM)_T("Failed IFileSystemImageResult->get_ImageStream!"));
                }

            }
            else
            {
                pThis->SendMessage(WM_BURN_FINISHED, hr, 
                    (LPARAM)_T("Failed IFileSystemImage->CreateResultImage!"));
            }
        }
    }
    else
    {
        pThis->SendMessage(WM_BURN_FINISHED, hr, (LPARAM)_T("Failed IFileSystemImage->getRoot"));
    }

    //
    // Cleanup
    //
    if (image != NULL)
	{
		__asm{
			mov		esi, image;
			mov     eax, esi;
			mov     ecx, [eax];
			push	eax;
			mov     eax,dword ptr [ecx+8h]
			call	eax;
		}
//        image->Release();
	}
    if (imageResult != NULL)
	{
		__asm{
			mov		esi, imageResult;
			mov     eax, esi;
			mov     ecx, [eax];
			push	eax;
			mov     eax,dword ptr [ecx+8h]
			call	eax;
		}
//        imageResult->Release();
	}
    if (rootItem != NULL)
	{
		__asm{
			mov		esi, rootItem;
			mov     eax, esi;
			mov     ecx, [eax];
			push	eax;
			mov     eax,dword ptr [ecx+8h]
			call	eax;
		}
//        rootItem->Release();
	}

    return returnVal;
}

void CBurnToDiscDlg::UpdateTimes(LONG totalTime, LONG remainingTime)
{
    //
    // Set the estimated total time
    //
    CString strText;
    if (totalTime > 0)
    {
        strText.Format(_T("%d:%02d"), totalTime / 60, totalTime % 60);
    }
    else
    {
        strText = _T("0:00");
    }
    m_static_estimatedtimeVal.SetWindowText(strText);

    //
    // Set the estimated remaining time
    //
    if (remainingTime > 0)
    {
        strText.Format(_T("%d:%02d"), remainingTime / 60, remainingTime % 60);
    }
    else
    {
        strText = _T("0:00");
    }
    m_static_timeleftVal.SetWindowText(strText);
}

void CBurnToDiscDlg::UpdateBuffer(LONG usedSystemBuffer, LONG totalSystemBuffer)
{
    CString text;

//    if (usedSystemBuffer && totalSystemBuffer)
//    {
//        m_bufferCtrl.SetRange32(0, totalSystemBuffer);
//        m_bufferCtrl.SetPos(usedSystemBuffer);
//        text.Format(_T("Buffer: %d%%"), (100*usedSystemBuffer) / totalSystemBuffer);
//    }
//    else
//    {
//        text = _T("Buffer Empty");
//        m_bufferCtrl.SetPos(0);
//    }
//
//    m_bufferText.SetWindowText(text);
}

void CBurnToDiscDlg::UpdateProgress(LONG writtenSectors, LONG totalSectors)
{
    static LONG prevTotalSector = 0;
    CString text;

    if (totalSectors && (totalSectors != prevTotalSector))
    {
        prevTotalSector = totalSectors;
        m_progress_burn.SetRange32(0, totalSectors);
    }
    m_progress_burn.SetPos(writtenSectors);

    if (writtenSectors && totalSectors)
    {
        text.Format(_T("Progress: %d%%"), (100*writtenSectors) / totalSectors);
    }
    else
    {
        text = _T("Progress");
    }
//    m_static_.SetWindowText(text);
}
void CBurnToDiscDlg::OnLbnSelchangeBurnFileList()
{
    //GetDlgItem(IDC_REMOVE_FILES_BUTTON)->EnableWindow(m_fileListbox.GetCurSel()!=-1);
}
void CBurnToDiscDlg::OnBnClickedAddFilesButton()
{
	if(m_cmb_media_type.GetCount() == 0)
	{
		MessageBox(_T("Cannot Get Media Type!!!"), NULL, 0);
		return;
	}
    CFileDialog fileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("All Files (*.*)|*.*||"), NULL);
    if (fileDialog.DoModal() == IDOK)
    {
        CFileObject* pFileObject = new CFileObject(fileDialog.GetPathName());
		char buf[2];
		wchar_t wbuf[2];
		itoa(m_nFileCounts, buf, 10);
		Ascii2Unichar(buf, wbuf);
		int addIndex = m_burnListCtrl.InsertItem(m_nFileCounts, wbuf);
        m_fileObjectList.push_back(pFileObject);
		m_burnListCtrl.SetItemText(m_nFileCounts, 1, pFileObject->GetName());
        
		m_nFileCounts ++;
        UpdateCapacity();
        EnableBurnButton();
    }
}

void CBurnToDiscDlg::OnDestroy()
{
    int itemCount = m_burnListCtrl.GetItemCount();
	int itemIndex;
    for (itemIndex = 0; itemIndex < itemCount; itemIndex++)
    {
        delete (CBaseObject*)m_burnListCtrl.GetItemData(itemIndex);
    }

    itemCount = m_cmb_select_drive.GetCount();
    for (itemIndex = 0; itemIndex < itemCount; itemIndex++)
    {
        delete (CDiscRecorder*)m_cmb_select_drive.GetItemDataPtr(itemIndex);
    }


    CDialog::OnDestroy();
}
void CBurnToDiscDlg::OnBnClickedRemoveFilesButton()
{
    int currentSelection = m_burnListCtrl.GetSelectedItemCount();
    ASSERT(currentSelection != -1);
    if (currentSelection == -1)
        return;

    CBaseObject* pBaseObject = (CBaseObject*)m_burnListCtrl.GetItemData(currentSelection);
    if (pBaseObject == NULL)
        return;

    CString message;
    message.Format(_T("Are you sure you want to remove \"%s\"?"), (LPCTSTR)pBaseObject->GetName());

    if (AfxMessageBox(message, MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
        m_burnListCtrl.DeleteItem(currentSelection);
        delete pBaseObject;

        OnLbnSelchangeBurnFileList();
        EnableBurnButton();
        UpdateCapacity();
    }
}
LRESULT CBurnToDiscDlg::OnBurnFinished(WPARAM hResult, LPARAM lpMessage)
{
    if (lpMessage != NULL)
    {
        if (SUCCEEDED((HRESULT)hResult))
        {
//            m_progressText.SetWindowText((LPCTSTR)lpMessage);
        }
        else
        {
            CString text;
            text.Format(_T("%s - Error:0x%08X"), (LPCTSTR)lpMessage, hResult);
//            m_progressText.SetWindowText(text);
        }
    }
    else
    {
        if (SUCCEEDED((HRESULT)hResult))
        {
//            m_progressText.SetWindowText(_T("Burn completed successfully!"));
        }
        else
        {
            CString message;
            message.Format(_T("Burn failed! Error: 0x%08x"), hResult);
//            m_progressText.SetWindowText(message);
        }
    }

    EnableUI(TRUE);
    return 0;
}

LRESULT CBurnToDiscDlg::OnBurnStatusMessage(WPARAM, LPARAM lpMessage)
{
    if (lpMessage != NULL)
    {
//        m_progressText.SetWindowText((LPCTSTR)lpMessage);
    }
    return 0;
}
LRESULT CBurnToDiscDlg::OnImapiUpdate(WPARAM wParam, LPARAM lParam)
{
    DWORD currentAction = 
        (DWORD)wParam;
    PIMAPI_STATUS pImapiStatus = (PIMAPI_STATUS)lParam;

    switch (currentAction)
    {
    case IMAPI_FORMAT2_DATA_WRITE_ACTION_VALIDATING_MEDIA:
//        m_progressText.SetWindowText(_T("Validating current media..."));
        break;

    case IMAPI_FORMAT2_DATA_WRITE_ACTION_FORMATTING_MEDIA:
//        m_progressText.SetWindowText(_T("Formatting media..."));
        break;

    case IMAPI_FORMAT2_DATA_WRITE_ACTION_INITIALIZING_HARDWARE:
//        m_progressText.SetWindowText(_T("Initializing hardware..."));
        break;

    case IMAPI_FORMAT2_DATA_WRITE_ACTION_CALIBRATING_POWER:
//        m_progressText.SetWindowText(_T("Optimizing laser intensity..."));
        break;

    case IMAPI_FORMAT2_DATA_WRITE_ACTION_WRITING_DATA:
        UpdateTimes(pImapiStatus->totalTime, pImapiStatus->remainingTime);
        UpdateBuffer(pImapiStatus->usedSystemBuffer, pImapiStatus->totalSystemBuffer);
        UpdateProgress(pImapiStatus->lastWrittenLba-pImapiStatus->startLba, pImapiStatus->sectorCount);
        break;

    case IMAPI_FORMAT2_DATA_WRITE_ACTION_FINALIZATION:
//        m_progressText.SetWindowText(_T("Finalizing writing..."));
        break;

    case IMAPI_FORMAT2_DATA_WRITE_ACTION_COMPLETED:
//        m_progressText.SetWindowText(_T("Completed!"));
        break;
    }

    return GetCancelBurning() ? RETURN_CANCEL_WRITE : RETURN_CONTINUE;
}
