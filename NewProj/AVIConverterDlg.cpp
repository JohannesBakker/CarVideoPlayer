// AVIConverterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "AVIConverterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TEXT_COLOR_GRAY			RGB(200, 200, 200)
#define TEXT_COLOR_WHITE		RGB(255, 255, 255)
#define TEXT_BK_COLOR_BLACK		RGB(47, 56, 66)
#define FONT_SIZE				14

/////////////////////////////////////////////////////////////////////////////
// CAVIConverterDlg dialog
XVID_HANDLE		CAVIConverterDlg::	m_lpHandle;
XVID_FRAME		CAVIConverterDlg::	m_lpFrame;
XVID_CREATE		CAVIConverterDlg::	m_lpCreate;
XVID_STATE		CAVIConverterDlg::	m_lpState;
SEEKPOS_FRAME	CAVIConverterDlg::	m_SeekPosFrame[0x20000];

CAVIConverterDlg::CAVIConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAVIConverterDlg::IDD, pParent)
	, m_bRadioOriginalSize(true)
	, m_bradio4_3(false)
	, m_bradio16_9(false)
{
	//{{AFX_DATA_INIT(CAVIConverterDlg)
	//}}AFX_DATA_INIT
	m_bStart = false;
	m_bEnd = true;
}


void CAVIConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAVIConverterDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_SELECT_AUDIO_COMBO, m_cmb_selectaudio);
	DDX_Control(pDX, IDC_TARGET_OPEN_BUTTON, m_btn_target_open);
	DDX_Control(pDX, IDC_START_BUTTON, m_btn_Start);
	DDX_Control(pDX, IDC_OUTPUT_OPEN_BUTTON, m_btn_out_open);
	DDX_Control(pDX, IDC_CHANGE_BUTTON, m_btn_Change);
	DDX_Control(pDX, IDC_FILE_LSTCONTLR, m_aviFileList);
	DDX_Control(pDX, IDC_CHECK_MP4_CONVERT, m_checkMp4Fmt);
	DDX_Control(pDX, IDC_CHECK_PICTURE_FLIP, m_checkPicFlip);
	DDX_Control(pDX, IDC_TARGET_EDIT, m_edit_target);
	DDX_Control(pDX, IDC_OUTPUT_EDIT, m_edit_output);
	DDX_Control(pDX, IDC_FILE_TIME_EDIT, m_edit_filetime);
	DDX_Control(pDX, IDC_DEST_FILENAME_EDIT, m_edit_destfilename);
	DDX_Control(pDX, IDC_TO_STATIC, m_static_to);
	DDX_Control(pDX, IDC_TARGET_STATIC, m_static_target);
	DDX_Control(pDX, IDC_STRING2_STATIC, m_static_string2);
	DDX_Control(pDX, IDC_STRING1_STATIC, m_static_string1);
	DDX_Control(pDX, IDC_SELECTAUDIO_STATIC, m_static_selectaudio);
	DDX_Control(pDX, IDC_PROGRESS_STATIC, m_static_progress);
	DDX_Control(pDX, IDC_OUTPUT_STATIC, m_static_output);
	DDX_Control(pDX, IDC_FILETIME_STATIC, m_static_filetime);
	DDX_Control(pDX, IDC_FILE_STATIC, m_static_file);
	DDX_Control(pDX, IDC_TIMERANGE_STATIC, m_static_timerange);
	DDX_Control(pDX, IDC_DESTFILENAME_STATIC, m_static_destfilename);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO_ORIGINAL_SIZE, m_RadioOriginalSize);
	DDX_Control(pDX, IDC_RADIO_43, m_radio4_3);
	DDX_Control(pDX, IDC_RADIO_16_9, m_radio16_9);
}


BEGIN_MESSAGE_MAP(CAVIConverterDlg, CDialog)
	//{{AFX_MSG_MAP(CAVIConverterDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartBtn)
	ON_BN_CLICKED(IDC_OUTPUT_OPEN_BUTTON, OnOutputOpenButton)
	ON_BN_CLICKED(IDC_TARGET_OPEN_BUTTON, OnTargetOpenButton)
	ON_BN_CLICKED(IDC_CHANGE_BUTTON, OnChangeButton)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_16_9, &CAVIConverterDlg::OnBnClickedRadio169)
	ON_BN_CLICKED(IDC_RADIO_ORIGINAL_SIZE, &CAVIConverterDlg::OnBnClickedRadioOriginalSize)
	ON_BN_CLICKED(IDC_RADIO_43, &CAVIConverterDlg::OnBnClickedRadio43)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAVIConverterDlg message handlers

void CAVIConverterDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rt;
	GetClientRect(&rt);
	dc.FillSolidRect(0, 0, rt.Width(), rt.Height(), TEXT_BK_COLOR_BLACK);
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CAVIConverterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HBITMAP hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_START));
	m_btn_Start.SetBitmap(hBmp);
	hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_CHANGE));
	m_btn_Change.SetBitmap(hBmp);
	hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_OPEN));
	m_btn_target_open.SetBitmap(hBmp);
	m_btn_out_open.SetBitmap(hBmp);

	char chHeader[1000];
	char str[100];

	memset(chHeader, 0, sizeof(chHeader));
	memset(str, 0, sizeof(str));
	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[4], str);
	strcpy(chHeader, str);
	strcat(chHeader, ",100 ;");
	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[23], str);
	strcat(chHeader, str);
	strcat(chHeader, ",100 ;");
	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[167], str);
	strcat(chHeader, str);
	strcat(chHeader, ",300 ;");
	m_aviFileList.SetHeadings(chHeader);

	m_aviFileList.SetCheckboxes(TRUE);
	m_aviFileList.SetBkColor(TEXT_BK_COLOR_BLACK);
	m_aviFileList.SetTextBkColor(TEXT_BK_COLOR_BLACK);
	m_aviFileList.SetTextColor(TEXT_COLOR_WHITE);
	m_aviFileList.SetGridLines(TRUE);
	
	m_edit_filetime.SetTextColor(TEXT_COLOR_GRAY);
	m_edit_filetime.SetBackColor(TEXT_BK_COLOR_BLACK);
	m_edit_output.SetTextColor(TEXT_COLOR_GRAY);
	m_edit_output.SetBackColor(TEXT_BK_COLOR_BLACK);
	m_edit_destfilename.SetTextColor(TEXT_COLOR_WHITE);
	m_edit_target.SetTextColor(TEXT_COLOR_GRAY);
	m_edit_target.SetBackColor(TEXT_BK_COLOR_BLACK);

	m_static_file.SetTextColor(TEXT_COLOR_GRAY);
	m_static_file.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_file.FormatW(CNewProjDlg::m_wLanguageRes[150]);
	m_static_file.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_filetime.SetTextColor(TEXT_COLOR_GRAY);
	m_static_filetime.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_filetime.FormatW(CNewProjDlg::m_wLanguageRes[151]);
	m_static_filetime.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_timerange.SetTextColor(TEXT_COLOR_GRAY);
	m_static_timerange.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_timerange.FormatW(CNewProjDlg::m_wLanguageRes[169]);
	m_static_timerange.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_to.SetTextColor(TEXT_COLOR_GRAY);
	m_static_to.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_to.FormatW(CNewProjDlg::m_wLanguageRes[173]);
	m_static_to.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_selectaudio.SetTextColor(TEXT_COLOR_GRAY);
	m_static_selectaudio.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_selectaudio.FormatW(CNewProjDlg::m_wLanguageRes[171]);
	m_static_selectaudio.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_progress.SetTextColor(TEXT_COLOR_GRAY);
	m_static_progress.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_progress.FormatW(CNewProjDlg::m_wLanguageRes[154]);
	m_static_progress.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_output.SetTextColor(TEXT_COLOR_GRAY);
	m_static_output.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_output.FormatW(CNewProjDlg::m_wLanguageRes[153]);
	m_static_output.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_destfilename.SetTextColor(TEXT_COLOR_GRAY);
	m_static_destfilename.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_destfilename.FormatW(CNewProjDlg::m_wLanguageRes[713]);
	m_static_destfilename.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_target.SetTextColor(TEXT_COLOR_GRAY);
	m_static_target.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_target.FormatW(CNewProjDlg::m_wLanguageRes[160]);
	m_static_target.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_string1.SetTextColor(TEXT_COLOR_GRAY);
	m_static_string1.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_string1.FormatW(CNewProjDlg::m_wLanguageRes[176]);
	m_static_string1.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);

	m_static_string2.SetTextColor(TEXT_COLOR_GRAY);
	m_static_string2.SetFont("Arial", FONT_SIZE, FW_NORMAL);
	m_static_string2.FormatW(CNewProjDlg::m_wLanguageRes[176]);
	m_static_string2.SetBkColor(TEXT_BK_COLOR_BLACK, 0, CxStatic::Normal);
	
	m_checkPicFlip.FormatW(CNewProjDlg::m_wLanguageRes[175]);
	m_checkMp4Fmt.FormatW(CNewProjDlg::m_wLanguageRes[170]);
	m_checkMp4Fmt.SetIcon(IDI_CHECKED,BS_LEFT,IDI_UNCHECKED);
	m_checkPicFlip.SetIcon(IDI_UNCHECKED,BS_LEFT,IDI_CHECKED);

	m_RadioOriginalSize.SetIcon(IDI_RADIO_OFF, BS_LEFT, IDI_RADIO_ON);
	m_radio16_9.SetIcon(IDI_RADIO_OFF, BS_LEFT, IDI_RADIO_ON);
	m_radio4_3.SetIcon(IDI_RADIO_OFF, BS_LEFT, IDI_RADIO_ON);

	m_bRadioOriginalSize = true;
	m_bradio16_9 = false;
	m_bradio4_3 = false;

	m_RadioOriginalSize.SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAVIConverterDlg::GetFiles(CFile* p1stFile, CFile* p2ndFile)
{
	m_progress.SetRange(0, 100);
	m_progress.SetPos(0);
	m_pTimeRangeCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_TIME_RANGE_SPIN);
	m_pToCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_TO_SPIN);
	m_aviFileList.DeleteAllItems();
	m_p1st264File = p1stFile;
	m_1stSpliter.Initialization(m_p1st264File);
	m_p1st264File->Seek(ASF_FILE_HEAD_SIZE, CFile::begin);
	
	//m_aviFileList.SetHeadings("Channel,100; Resolution, 100; File,300");
	m_aviFileList.InsertItem(0, _T("1"), _T("704 X 480"), m_1stFilePath);
	m_aviFileList.SetItemText(0, 1, _T("704 X 480"));
	m_aviFileList.SetItemText(0, 2, m_1stFilePath);
	m_aviFileList.SetCheck(0, true);

	char str[100];
	memset(str, 0, sizeof(str));
	Unichar2Ascii(CNewProjDlg::m_wLanguageRes[4], str);

	CString temp = str;
	m_cmb_selectaudio.AddString(temp + "1");

	if (m_2ndFilePath != "")
	{
		m_p2nd264File = p2ndFile;
		m_2ndSpliter.Initialization(m_p2nd264File);
		m_p2nd264File->Seek(ASF_FILE_HEAD_SIZE, CFile::begin);
		m_aviFileList.InsertItem(1, _T("2"), _T("704 X 480"), m_2ndFilePath);
		m_aviFileList.SetItemText(1, 1, _T("704 X 480"));
		m_aviFileList.SetItemText(1, 2, m_2ndFilePath);
		m_aviFileList.SetCheck(1, true);
		m_cmb_selectaudio.AddString(temp + "2");
	}
	m_cmb_selectaudio.SetCurSel(0);

	VERIFY(m_pTimeRangeCtrl->SetTime(&CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr]));
	m_systemTime = CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr];
	m_systemTime.wHour = (WORD)(m_systemTime.wHour + CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr] / 3600000) % 24;
	m_systemTime.wMinute = (WORD)(m_systemTime.wMinute + CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr] / 60000) % 60;
	m_systemTime.wSecond = (WORD)(m_systemTime.wSecond + CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr] / 1000) % 60;
	VERIFY(m_pToCtrl->SetTime(&m_systemTime));

//	m_RadioOriginalSize.SetIcon(IDI_RADIO_ON,BS_LEFT,IDI_RADIO_OFF);
//	m_radio4_3.SetIcon(IDI_RADIO_OFF,BS_LEFT,IDI_RADIO_ON);
//	m_radio16_9.SetIcon(IDI_RADIO_OFF,BS_LEFT,IDI_RADIO_ON);

	m_nScaleFlag = 0;
	wchar_t buf[4];
	CTime t;
	t = t.GetCurrentTime();
	wsprintf(buf, _T("%02d"), t.GetHour());
	temp = _T("-");
	temp += buf;
	wsprintf(buf, _T("%02d"), t.GetMinute());
	temp += buf;
	wsprintf(buf, _T("%02d"), t.GetSecond());
	temp += buf;
	temp += _T(".avi");
	m_FolderPath = m_1stFilePath.Mid(0, m_1stFilePath.GetLength() - 26);
	m_edit_output.SetWindowText(_T("C:\\DVR264\\Output\\"));
	m_edit_destfilename.SetWindowText(m_1stFilePath.Mid(m_1stFilePath.GetLength() - 26, 22) + temp);
	m_AVIFilePath = _T("C:\\DVR264\\Output\\" + m_1stFilePath.Mid(m_1stFilePath.GetLength() - 26, 22) + temp);
	m_AVIFileName = m_1stFilePath.Mid(m_1stFilePath.GetLength() - 26, 22) + temp;
	m_edit_target.SetWindowText(m_AVIFilePath);
}

void CAVIConverterDlg::InitHeaderDatas(int nWidth, int nHeight)
{	
	HINSTANCE hXvidEnc = LoadLibrary(_T("xvidcore.dll"));
	m_Func_XvidEnc = (FuncAVI_Xvid_EnCore)GetProcAddress(hXvidEnc, "xvid_encore");
	HINSTANCE hXvidCreate = LoadLibrary(_T("xvidcore.dll"));
	m_Func_XvidCreate = (FuncAVI_Xvid_Create)GetProcAddress(hXvidCreate, "xvid_encore");
	HINSTANCE hXvidGlobal = LoadLibrary(_T("xvidcore.dll"));
	m_Func_XvidGlobal = (FuncAVI_Xvid_Global)GetProcAddress(hXvidGlobal, "xvid_global");
////////////////////////////

	ZeroMemory(&m_lpCreate, sizeof(XVID_CREATE));
	m_lpCreate.nConst = 0x10202;
	m_lpCreate.Unk1 = 3;
	m_lpCreate.Unk2 = nWidth;
	m_lpCreate.Unk3 = nHeight;
	m_lpCreate.Unk4 = 0;
	m_lpCreate.Unk5 = 0;
	m_lpCreate.Unk6 = 0;
	m_lpCreate.Unk7 = 0;
	m_lpCreate.Unk8 = 0;
	m_lpCreate.Unk9 = 0;
	m_lpCreate.Unk10 = 0;
	m_lpCreate.PushData = 1;
	m_lpCreate.Unk11 = 0x19;
	m_lpCreate.Unk12 = 0xFA;
	m_lpCreate.Unk13 = 0;
	m_lpCreate.nWidth = 0x96;
	m_lpCreate.Unk15 = 0x64;
	m_lpCreate.Unk16 = 0;
	m_lpCreate.Unk17 = 0;
	m_lpCreate.Unk18 = 0;
	m_lpCreate.Unk19 = 0;
	m_lpCreate.Unk20 = 0;
	m_lpCreate.PopData = 0;

//////////////////////////////////
	ZeroMemory(&m_lpHandle, sizeof(XVID_HANDLE));
	m_lpHandle.m_nWidth = nWidth;
	m_lpHandle.m_nHeight = nHeight;
	if (nWidth == 704) m_lpHandle.Unk1 = 0x340;
	else m_lpHandle.Unk1 = 0x600;
	m_lpHandle.Unk2 = 0x260;
	if (nWidth == 704) m_lpHandle.Unk3 = 0x2C;
	else m_lpHandle.Unk3 = 0x58;
	m_lpHandle.Unk4 = 0x1E;
	m_lpHandle.Unk5 = 0x0;
	m_lpHandle.Unk6 = 0x1;
	m_lpHandle.Unk7 = 0x19;
	m_lpHandle.Unk8 = 0x3;
	m_lpHandle.Unk9 = 0x0;
	m_lpHandle.Unk10 = 0x96;
	m_lpHandle.Unk11 = 0x64;
	m_lpHandle.Unk12 = 0x0;
	m_lpHandle.Unk13 = 0x2;
	m_lpHandle.Unk14 = 0x2;
	m_lpHandle.Unk15 = 0x2;
	m_lpHandle.Unk16 = 0x1F;
	m_lpHandle.Unk17 = 0x1F;
	m_lpHandle.Unk18 = 0x1F;
	m_lpHandle.Unk19 = 0x0;
	m_lpHandle.Unk20 = 0x0;
	m_lpHandle.Unk21 = 0x0;
	m_lpHandle.Unk22 = 0xFA;
	m_lpHandle.Unk23 = 0x0;

////////////////////////////////////////
	ZeroMemory(&m_lpState, sizeof(XVID_STATE));
	m_lpState.nConst = 0x10202;
	m_lpState.nUnk1 = 0;
	m_lpState.nUnk2 = 0;
	m_lpState.nUnk3 = 0;
	m_lpState.nUnk4 = 0;
	m_lpState.nEncSize = 0;
	m_lpState.nUnk6 = 0;
	m_lpState.nUnk7 = 0;
	m_lpState.nUnk8 = 0;
	m_lpState.nUnk9 = 0;

///////////////////////////////////////
//	ZeroMemory(&m_lpFrame, sizeof(XVID_FRAME));
	m_lpFrame.nConst = 0x10202;
	m_lpFrame.Unk1 = 0;
	m_lpFrame.Unk2 = 0;
	m_lpFrame.Unk3 = 0;
	m_lpFrame.Unk4 = 0;
	m_lpFrame.Unk5 = 0;
	m_lpFrame.Unk6 = 0;
	m_lpFrame.Unk7 = 0;
	m_lpFrame.Unk8 = 6;
	m_lpFrame.Unk9 = 0x3053;
	m_lpFrame.Unk10 = 2;
	
	m_lpFrame.Unk11 = 0;
	m_lpFrame.Unk12 = 0;
	m_lpFrame.Unk13 = 0;
	m_lpFrame.nWidth = nWidth;
	m_lpFrame.Unk15 = 0;
	m_lpFrame.Unk16 = 0;
	m_lpFrame.Unk17 = 0;
	m_lpFrame.Unk18 = 0;
	m_lpFrame.Unk19 = 3;
	m_lpFrame.Unk20 = 0;
	m_lpFrame.Unk21 = 0xFFFFFFFF;
	m_lpFrame.Unk22 = 0;


	ZeroMemory(&m_Riff, sizeof(RIFF));
	m_Riff.chRiff[0] = 'R';
	m_Riff.chRiff[1] = 'I';
	m_Riff.chRiff[2] = 'F';
	m_Riff.chRiff[3] = 'F';

	ZeroMemory(&m_Junk, sizeof(JUNK));
	m_Junk.chJunk[0] = 'J';
	m_Junk.chJunk[1] = 'U';
	m_Junk.chJunk[2] = 'N';
	m_Junk.chJunk[3] = 'K';
	m_Junk.dwJunkSize = 0x660;

	ZeroMemory(&m_InfoList, sizeof(INFOLIST));
	m_InfoList.chList[0] = 'L';
	m_InfoList.chList[1] = 'I';
	m_InfoList.chList[2] = 'S';
	m_InfoList.chList[3] = 'T';
	m_InfoList.dwInfoListFrameSize = 0x4C;
	m_InfoList.chInfoList[0] = 'I';
	m_InfoList.chInfoList[1] = 'N';
	m_InfoList.chInfoList[2] = 'F';
	m_InfoList.chInfoList[3] = 'O';
	m_InfoList.chInfoList[4] = 'I';
	m_InfoList.chInfoList[5] = 'S';
	m_InfoList.chInfoList[6] = 'F';
	m_InfoList.chInfoList[7] = 'T';
	m_InfoList.dwInfoSize = 0x40;
	
	ZeroMemory(&m_AviList, sizeof(AVI_LIST));
	m_AviList.chAvi_List[0] = 'A';
	m_AviList.chAvi_List[1] = 'V';
	m_AviList.chAvi_List[2] = 'I';
	m_AviList.chAvi_List[3] = ' ';
	m_AviList.chAvi_List[4] = 'L';
	m_AviList.chAvi_List[5] = 'I';
	m_AviList.chAvi_List[6] = 'S';
	m_AviList.chAvi_List[7] = 'T';
	m_AviList.dwAviListStructSize = 0x124;
	m_AviList.m_AviListHeader.chAviListHeader[0] = 'h';
	m_AviList.m_AviListHeader.chAviListHeader[1] = 'd';
	m_AviList.m_AviListHeader.chAviListHeader[2] = 'r';
	m_AviList.m_AviListHeader.chAviListHeader[3] = 'l';
	m_AviList.m_AviListHeader.chAviListHeader[4] = 'a';
	m_AviList.m_AviListHeader.chAviListHeader[5] = 'v';
	m_AviList.m_AviListHeader.chAviListHeader[6] = 'i';
	m_AviList.m_AviListHeader.chAviListHeader[7] = 'h';
	m_AviList.m_AviListHeader.dwHeaderSize = 0x38;
	m_AviList.m_AviListHeader.dwSecPerFrames = 0x186A0;
	m_AviList.m_AviListHeader.dwConst1[2] = 0x110;
	m_AviList.m_AviListHeader.dwConst2[1] = 2;
	m_AviList.m_AviListHeader.nVideoWidth = nWidth;
	m_AviList.m_AviListHeader.nVideoHeight = nHeight;
	memcpy(m_AviList.m_VideoList.chList, m_InfoList.chList, 4);
	m_AviList.m_VideoList.dwVideoListSize = 0x74;
	m_AviList.m_VideoList.chStringHeader[0] = 's';
	m_AviList.m_VideoList.chStringHeader[1] = 't';
	m_AviList.m_VideoList.chStringHeader[2] = 'r';
	m_AviList.m_VideoList.chStringHeader[3] = 'l';
	m_AviList.m_VideoList.chStringHeader[4] = 's';
	m_AviList.m_VideoList.chStringHeader[5] = 't';
	m_AviList.m_VideoList.chStringHeader[6] = 'r';
	m_AviList.m_VideoList.chStringHeader[7] = 'h';
	m_AviList.m_VideoList.dwHeaderSize = 0x38;
	m_AviList.m_VideoList.chVideo[0] = 'v';
	m_AviList.m_VideoList.chVideo[1] = 'i';
	m_AviList.m_VideoList.chVideo[2] = 'd';
	m_AviList.m_VideoList.chVideo[3] = 's';
	m_AviList.m_VideoList.chVideo[4] = 'x';
	m_AviList.m_VideoList.chVideo[5] = 'v';
	m_AviList.m_VideoList.chVideo[6] = 'i';
	m_AviList.m_VideoList.chVideo[7] = 'd';
	m_AviList.m_VideoList.dwConst[0] = 0xF4240;
	m_AviList.m_VideoList.dwConst[1] = 0x989680;
	m_AviList.m_VideoList.dwReserved2[1] = 0xFFFFFFFF;
	m_AviList.m_VideoList.m_VideoFrame.chFrame[0] = 's';
	m_AviList.m_VideoList.m_VideoFrame.chFrame[1] = 't';
	m_AviList.m_VideoList.m_VideoFrame.chFrame[2] = 'r';
	m_AviList.m_VideoList.m_VideoFrame.chFrame[3] = 'f';
	m_AviList.m_VideoList.m_VideoFrame.dwFrameSize1 = 0x28;
	m_AviList.m_VideoList.m_VideoFrame.dwFrameSize2 = 0x28;
	m_AviList.m_VideoList.m_VideoFrame.nVideoWidth = nWidth;
	m_AviList.m_VideoList.m_VideoFrame.nVideoHeight = nHeight;
	m_AviList.m_VideoList.m_VideoFrame.dwUnk[0] = 0x180001;
	m_AviList.m_VideoList.m_VideoFrame.dwUnk[1] = 0x64697678;
	m_AviList.m_VideoList.m_VideoFrame.dwUnk[2] = nWidth * nHeight * 3;
	memcpy(m_AviList.m_AudioList.chList, m_InfoList.chList, 4);
	m_AviList.m_AudioList.dwAudioListSize = 0x5C;
	memcpy(m_AviList.m_AudioList.m_StringHeader.chStringHeader, m_AviList.m_VideoList.chStringHeader, 8);
	m_AviList.m_AudioList.m_StringHeader.dwHeaderSize = 0x38;
	m_AviList.m_AudioList.m_StringHeader.chAudios[0] = 'a';
	m_AviList.m_AudioList.m_StringHeader.chAudios[1] = 'u';
	m_AviList.m_AudioList.m_StringHeader.chAudios[2] = 'd';
	m_AviList.m_AudioList.m_StringHeader.chAudios[3] = 's';
	m_AviList.m_AudioList.m_StringHeader.nChannelCounts = 1;
	m_AviList.m_AudioList.m_StringHeader.dwSecPerbits = 0x3E80;
	m_AviList.m_AudioList.m_StringHeader.dwReserved2[1] = 0x6FE00;
	m_AviList.m_AudioList.m_StringHeader.dwReserved2[3] = 0xFFFFFFFF;
	m_AviList.m_AudioList.m_StringHeader.dwReserved2[4] = 1;
	memcpy(m_AviList.m_AudioList.m_AudioFrame.chFrame, m_AviList.m_VideoList.m_VideoFrame.chFrame, 4);
	m_AviList.m_AudioList.m_AudioFrame.dwFrameSize = 0x10;
	m_AviList.m_AudioList.m_AudioFrame.dwConst1 = 0x10001;
	m_AviList.m_AudioList.m_AudioFrame.dwHz = 0x1F40;
	m_AviList.m_AudioList.m_AudioFrame.dwSecPerbits = 0x3E80;
	m_AviList.m_AudioList.m_AudioFrame.dwConst2 = 0x100001;
	
	ZeroMemory(&m_ListFrameHeader, sizeof(LIST_FRAME_HEADER));
	m_ListFrameHeader.dwVideoID = 0x62643030;
	m_ListFrameHeader.dwB0_HeaderID = 0xB0010000;
	m_ListFrameHeader.chB0_Data = 3;
	m_ListFrameHeader.dwB5_HeaderID = 0xB5010000;
	m_ListFrameHeader.dwB5_Datas[0] = 0x01000009;
	m_ListFrameHeader.dwB5_Datas[1] = 0x01000000;
	m_ListFrameHeader.dwB5_Datas[2] = 0xC4860020;
	m_ListFrameHeader.dwB5_Datas[3] = 0x580C6700;
	m_ListFrameHeader.dwB5_Datas[4] = 0x8F51F010;
	m_ListFrameHeader.dwB2_HeaderID = 0xB2010000;
	m_ListFrameHeader.dwB2_Datas[0] = 0x44697658;
	m_ListFrameHeader.dwB2_Datas[1] = 0x30353030;
	m_ListFrameHeader.dwB6_HeaderID = 0xB6010000;

	ZeroMemory(&m_B6FrameHeader, sizeof(B6_FRAME_HEADER));
	m_B6FrameHeader.dwB6_HeaderID = 0xB6010000;
	
	ZeroMemory(&m_AudioFrameHeader, sizeof(AUDIO_FRAME_HEADER));
	m_AudioFrameHeader.dwAudioHeaderID = 0x62773130;

	ZeroMemory(&m_TrackFrame, sizeof(TRACK_FRAME));
	memcpy(m_TrackFrame.chList, m_InfoList.chList, 4);
	m_TrackFrame.chMovie[0] = 'm';
	m_TrackFrame.chMovie[1] = 'o';
	m_TrackFrame.chMovie[2] = 'v';
	m_TrackFrame.chMovie[3] = 'i';

	ZeroMemory(&m_SeekPosFrame, sizeof(SEEKPOS_FRAME));
}

UINT AVICnvertingThreadProc(LPVOID lParam)
{
	CAVIConverterDlg* pDlg = (CAVIConverterDlg*)lParam;
	pDlg->m_bEnd = false;

	if (pDlg->m_2ndFilePath != "")
	{
		if (pDlg->m_aviFileList.GetCheck(0) == FALSE && pDlg->m_aviFileList.GetCheck(1) == FALSE)
		{
			pDlg->m_nChannel = 0;
			return 0;
		}

		if (pDlg->m_aviFileList.GetCheck(0) == TRUE && pDlg->m_aviFileList.GetCheck(1) == FALSE)
		{
			if (pDlg->m_bRadioOriginalSize) 
				pDlg->InitHeaderDatas(704, 480);

			if (pDlg->m_bradio4_3) 
				pDlg->InitHeaderDatas(704, 528);

			if (pDlg->m_bradio16_9) 
				pDlg->InitHeaderDatas(704, 396);

			pDlg->m_nChannel = 1;
		}

		if (pDlg->m_aviFileList.GetCheck(0) == FALSE && pDlg->m_aviFileList.GetCheck(1) == TRUE)
		{
			if (pDlg->m_bRadioOriginalSize) 
				pDlg->InitHeaderDatas(704, 480);

			if (pDlg->m_bradio4_3) 
				pDlg->InitHeaderDatas(704, 528);

			if (pDlg->m_bradio16_9) 
				pDlg->InitHeaderDatas(704, 396);

			pDlg->m_nChannel = 2;
		}

		if (pDlg->m_aviFileList.GetCheck(0) == TRUE && pDlg->m_aviFileList.GetCheck(1) == TRUE)
		{
			if (pDlg->m_bRadioOriginalSize) 
				pDlg->InitHeaderDatas(1408, 480);

			if (pDlg->m_bradio4_3) 
				pDlg->InitHeaderDatas(1408, 1440);

			if (pDlg->m_bradio16_9) 
				pDlg->InitHeaderDatas(1408, 960);

			pDlg->m_nChannel = 3;
		}

	} else {
		if (pDlg->m_aviFileList.GetCheck(0) == FALSE) 
			return 0;
		pDlg->InitHeaderDatas(704, 480);
		pDlg->m_nChannel = 1;
	}

	DWORD dwVideoID = 0x62643030;
	DWORD dwAudioID = 0x62773130;

	pDlg->m_1stSpliter.OpenFile("", 0);
	if (pDlg->m_2ndFilePath != "")
		pDlg->m_2ndSpliter.OpenFile("", 0);

	CString str;
	if (!pDlg->m_AVIFile.Open(LPCTSTR(pDlg->m_AVIFilePath), CFile::modeWrite | CFile::modeCreate))
	{
		MessageBox(NULL, _T("Error Create AVI FIle!"), NULL, 0);
		return 0;
	}

	pDlg->m_AVIFile.SetLength(0);
	pDlg->m_AVIFile.Write(&pDlg->m_Riff, sizeof(RIFF));
	pDlg->m_AVIFile.Write(&pDlg->m_AviList, sizeof(AVI_LIST));
	pDlg->m_AVIFile.Write(&pDlg->m_InfoList, sizeof(INFOLIST));
	pDlg->m_AVIFile.Write(&pDlg->m_Junk, sizeof(JUNK));
	pDlg->m_AVIFile.Write(&pDlg->m_TrackFrame, sizeof(TRACK_FRAME));

	DWORD dwFilePos = pDlg->m_AVIFile.GetPosition();
	if (pDlg->m_nChannel == 3) 
	{
		if (pDlg->m_bRadioOriginalSize) 
			pDlg->m_lpFrame.PushData = new BYTE[0x12FC00];

		if (pDlg->m_bradio4_3) 
			pDlg->m_lpFrame.PushData = new BYTE[0x5CD000];

		if (pDlg->m_bradio16_9) 
			pDlg->m_lpFrame.PushData = new BYTE[0x3DE000];
	}
	else 
	{
		if (pDlg->m_bRadioOriginalSize) 
			pDlg->m_lpFrame.PushData = new BYTE[0x97E00];

		if (pDlg->m_bradio4_3) 
			pDlg->m_lpFrame.PushData = new BYTE[0xA4400];

		if (pDlg->m_bradio16_9) 
			pDlg->m_lpFrame.PushData = new BYTE[0x82380];
	}

	pDlg->m_lpFrame.PopData = new BYTE[0x40000]; 
	DWORD dwGlobal[4];
	memset(dwGlobal, 0, 16);
	dwGlobal[0] = 0x10202;

	int nCounts = 0;
	DWORD dwRetVal = (*pDlg->m_Func_XvidGlobal)(0, 0, dwGlobal, 0);
	dwRetVal = (*pDlg->m_Func_XvidCreate)(0, 0, &pDlg->m_lpCreate, 0);
	dwRetVal = 1;

	int		nFrmaeID = 0;
	pDlg->m_1stSpliter.GetDatas1();
	pDlg->m_1stSpliter.GetDatas1();
	pDlg->m_2ndSpliter.GetDatas2();
	pDlg->m_2ndSpliter.GetDatas2();

	do {
		pDlg->m_progress.SetPos((int)((float)pDlg->m_1stSpliter.m_file->GetPosition() / pDlg->m_1stSpliter.m_file->GetLength() * 100));
		pDlg->m_1stSpliter.GetDatas1();

		if (pDlg->m_1stSpliter.m_file->GetPosition() > pDlg->m_1stSpliter.m_file->GetLength() - ASF_FILE_HEAD_SIZE - 3200 * 2) 
			break;

		pDlg->m_2ndSpliter.GetDatas2();
		if (pDlg->m_1stSpliter.m_pY == 0) 
			continue;

		if (pDlg->m_nChannel == 1)
		{
			if (pDlg->m_bRadioOriginalSize) 
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x97e00);
				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_1stSpliter.m_pY, 0x52800);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x52800), pDlg->m_1stSpliter.m_pU, 0x14A00);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x67200), pDlg->m_1stSpliter.m_pV, 0x14A00);
			}

			if (pDlg->m_bradio4_3)
			{
				memset(pDlg->m_lpFrame.PushData, 0, 0xA4400);
				char temp[0x4200] = {0};
				memcpy(pDlg->m_lpFrame.PushData, temp, 0x4200);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x4200), pDlg->m_1stSpliter.m_pY, 0x52800);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x56A00), temp, 0x4200);

				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x5AC00), temp, 0x1080);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x5BC80), pDlg->m_1stSpliter.m_pU, 0x14A00);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x70680), temp, 0x1080);

				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x71700), temp, 0x1080);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x72780), pDlg->m_1stSpliter.m_pV, 0x14A00);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x87180), temp, 0x1080);
			}

			if (pDlg->m_bradio16_9)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x82380);
				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_1stSpliter.m_pY, 0x44100);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x44100), pDlg->m_1stSpliter.m_pU, 0x11040);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x55140), pDlg->m_1stSpliter.m_pV, 0x11040);
			}
		} 
		else if (pDlg->m_nChannel == 2)
		{
			if (pDlg->m_bRadioOriginalSize) 
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x97e00);
				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_1stSpliter.m_pY, 0x52800);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x52800), pDlg->m_1stSpliter.m_pU, 0x14A00);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x67200), pDlg->m_1stSpliter.m_pV, 0x14A00);
			}

			if (pDlg->m_bradio4_3)
			{
				char temp[0x4200] = {0x80};
				memcpy(pDlg->m_lpFrame.PushData, temp, 0x4200);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x4200), pDlg->m_1stSpliter.m_pY2, 0x52800);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x56A00), temp, 0x4200);

				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x5AC00), temp, 0x1080);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x5BC80), pDlg->m_1stSpliter.m_pU2, 0x14A00);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x70680), temp, 0x1080);

				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x71700), temp, 0x1080);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x72780), pDlg->m_1stSpliter.m_pV2, 0x14A00);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x87180), temp, 0x1080);
			}

			if (pDlg->m_bradio16_9)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x82380);
				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_1stSpliter.m_pY2, 0x44100);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x44100), pDlg->m_1stSpliter.m_pU2, 0x11040);
				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x55140), pDlg->m_1stSpliter.m_pV2, 0x11040);
			}
		}
		else if (pDlg->m_nChannel == 3)
		{
			if (pDlg->m_bRadioOriginalSize)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x12FC00);
				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_1stSpliter.m_pY, 0x52800);
				for(int j = 0; j < 480; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 1408 * j),(char*)(pDlg->m_1stSpliter.m_pY + 704 * j), 704);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 1408 * j + 704), (char*)(pDlg->m_1stSpliter.m_pY2 + 704 * j) , 704);
				}

				int kount = 0;
				for(int j = 0; j < 240; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0xA5000 + kount),(char*)(pDlg->m_1stSpliter.m_pU + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0xA5000 + kount + 352), (char*)(pDlg->m_1stSpliter.m_pU2 + 352 * j) , 352);
					kount += 0x2C0;//704
				}

				kount = 0;
				for(int j = 0; j < 240; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0xCE400 + kount),(char*)(pDlg->m_1stSpliter.m_pV + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0xCE400 + kount + 352), (char*)(pDlg->m_1stSpliter.m_pV2 + 352 * j) , 352);
					kount += 0x2C0;//704
				}
			}

			if (pDlg->m_bradio16_9)
			{
				char temp[0x35A00] = {0};
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x198600);
//				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_1stSpliter.m_pY, 0x88200);

				/*display Y*/
// 				memcpy(pDlg->m_lpFrame.PushData, temp, 0x35A00);

				for(int j = 0; j < 480; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 2816 * j),(char*)(pDlg->m_1stSpliter.m_pY + 704 * j), 704);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 2816 * j + 704), (char*)(pDlg->m_1stSpliter.m_pY2 + 704 * j) , 704);

					memcpy((char*)(pDlg->m_lpFrame.PushData + 2816 * j + 1408), (char*)(pDlg->m_1stSpliter.m_pY + 704 * j), 704);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 2816 * j + 2112), (char*)(pDlg->m_1stSpliter.m_pY2 + 704 * j) , 704);
				}
// 				memcpy((char*)(pDlg->m_lpFrame.PushData + 0xDAA00), temp, 0x35A00);

				/*display U*/
//				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x110400), temp, 0xD680); 
				int kount = 0;
				for(int j = 0; j < 240 ; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x14A000 + kount),(char*)(pDlg->m_1stSpliter.m_pU + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x14A000 + kount + 352), (char*)(pDlg->m_1stSpliter.m_pU2 + 352 * j) , 352);

					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x14A000 + kount + 704),(char*)(pDlg->m_1stSpliter.m_pU + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x14A000 + kount + 1056), (char*)(pDlg->m_1stSpliter.m_pU2 + 352 * j) , 352);

					kount += 1408;
				}
//				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x146E80), temp, 0xD680);

				/*display V*/
//				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x154500), temp, 0xD680);
				kount = 0;
				for(int j = 0; j < 240; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x19C800 + kount),(char*)(pDlg->m_1stSpliter.m_pV + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x19C800 + kount + 352), (char*)(pDlg->m_1stSpliter.m_pV2 + 352 * j) , 352);

					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x19C800 + kount + 704),(char*)(pDlg->m_1stSpliter.m_pV + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x19C800 + kount + 1056), (char*)(pDlg->m_1stSpliter.m_pV2 + 352 * j) , 352);

					kount += 1408;
				}
			}

			if (pDlg->m_bradio4_3)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x220800);
				char temp[0x10800] = {0};
//				memcpy(pDlg->m_lpFrame.PushData, pDlg->m_1stSpliter.m_pY, 0xB5800);

				/*display Y*/
//				memcpy(pDlg->m_lpFrame.PushData, temp, 0x10800);

				for(int j = 0; j < 480; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 4224 * j),(char*)(pDlg->m_1stSpliter.m_pY + 704 * j), 704);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 4224 * j + 704), (char*)(pDlg->m_1stSpliter.m_pY2 + 704 * j) , 704);

					memcpy((char*)(pDlg->m_lpFrame.PushData + 4224 * j + 1408), (char*)(pDlg->m_1stSpliter.m_pY + 704 * j), 704);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 4224 * j + 2112), (char*)(pDlg->m_1stSpliter.m_pY2 + 704 * j) , 704);

					memcpy((char*)(pDlg->m_lpFrame.PushData + 4224 * j + 2816), (char*)(pDlg->m_1stSpliter.m_pY + 704 * j), 704);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 4224 * j + 3520), (char*)(pDlg->m_1stSpliter.m_pY2 + 704 * j) , 704);

				}
//				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x15A800), temp, 0x10800);
				
				/*display U*/
				int kount = 0;
//				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x16B000), temp, 0x4200);
				for(int j = 0; j < 240; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x1EF000 + kount),(char*)(pDlg->m_1stSpliter.m_pU + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x1EF000 + kount + 352), (char*)(pDlg->m_1stSpliter.m_pU2 + 352 * j) , 352);

					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x1EF000 + kount + 704),(char*)(pDlg->m_1stSpliter.m_pU + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x1EF000 + kount + 1056), (char*)(pDlg->m_1stSpliter.m_pU2 + 352 * j) , 352);

					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x1EF000 + kount + 1408),(char*)(pDlg->m_1stSpliter.m_pU + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x1EF000 + kount + 1760), (char*)(pDlg->m_1stSpliter.m_pU2 + 352 * j) , 352);

					kount += 2112;
				}
//				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x1C1A00), temp, 0x4200);

				/*display V*/
				kount = 0;
//				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x1C5C00), temp, 0x4200);
				for(int j = 0; j < 240; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x26AC00 + kount),(char*)(pDlg->m_1stSpliter.m_pV + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x26AC00 + kount + 352), (char*)(pDlg->m_1stSpliter.m_pV2 + 352 * j) , 352);

					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x26AC00 + kount + 704),(char*)(pDlg->m_1stSpliter.m_pV + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x26AC00 + kount + 1056), (char*)(pDlg->m_1stSpliter.m_pV2 + 352 * j) , 352);

					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x26AC00 + kount + 1408),(char*)(pDlg->m_1stSpliter.m_pV + 352 * j), 352);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 0x26AC00 + kount + 1760), (char*)(pDlg->m_1stSpliter.m_pV2 + 352 * j) , 352);
					kount += 2112;//704
				}
//				memcpy((char*)(pDlg->m_lpFrame.PushData + 0x21C600), temp, 0x4200);
				
			}
		}

		memset(&pDlg->m_lpState, 0, sizeof(XVID_STATE));
		pDlg->m_lpState.nConst = 0x10202;
		dwRetVal = (*pDlg->m_Func_XvidEnc)((DWORD*)(pDlg->m_lpCreate.Unk21), 2, &pDlg->m_lpFrame, &pDlg->m_lpState);
		pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwVideoID;

		if (nCounts % 250 == 0)
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0x10;
		else 
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;

		pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
		pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
		nFrmaeID ++;
		pDlg->m_AVIFile.Write(&dwVideoID, 4);
		pDlg->m_AVIFile.Write(&dwRetVal, 4);
		pDlg->m_AVIFile.Write(pDlg->m_lpFrame.PopData, dwRetVal);
		nCounts++;

		dwRetVal = 0x140;
		pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
		pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
		pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
		pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;

		if (pDlg->m_cmb_selectaudio.GetCurSel() == 0)
		{
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write(pDlg->m_1stSpliter.m_1stOutDatas.buf_A, 320);
			nFrmaeID ++;

			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write((char*)(pDlg->m_1stSpliter.m_1stOutDatas.buf_A + 320), 320);
			nFrmaeID ++;

			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write((char*)(pDlg->m_1stSpliter.m_1stOutDatas.buf_A + 320 * 2), 320);
			nFrmaeID ++;

			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write((char*)(pDlg->m_1stSpliter.m_1stOutDatas.buf_A + 320 * 3), 320);
			nFrmaeID ++;

			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write((char*)(pDlg->m_1stSpliter.m_1stOutDatas.buf_A + 320 * 4), 320);
			nFrmaeID ++;
		}
		else
		{
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A, 320);
			nFrmaeID ++;

			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write((char*)(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A + 320), 320);
			nFrmaeID ++;

			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write((char*)(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A + 320 * 2), 320);
			nFrmaeID ++;

			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write((char*)(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A + 320 * 3), 320);
			nFrmaeID ++;

			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = pDlg->m_AVIFile.GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			pDlg->m_AVIFile.Write(&dwAudioID, 4);
			pDlg->m_AVIFile.Write(&dwRetVal, 4);
			pDlg->m_AVIFile.Write((char*)(pDlg->m_2ndSpliter.m_2ndOutDatas.buf_A + 320 * 4), 320);
			nFrmaeID ++;
		}

		if (pDlg->m_1stSpliter.m_file->GetPosition() >= pDlg->m_1stSpliter.m_file->GetLength() - 0xC80) 
			break;

		if (pDlg->m_bEnd == true) 
			break;

	} while(dwRetVal > 0);

	dwRetVal = pDlg->m_AVIFile.GetPosition() - 0x7FC;
	pDlg->m_AVIFile.Seek(0x7F8, CFile::begin);
	pDlg->m_AVIFile.Write(&dwRetVal, 4);
	pDlg->m_AVIFile.SeekToEnd();
	dwRetVal = 0x31786469;
	pDlg->m_AVIFile.Write(&dwRetVal, 4);
	dwRetVal = nFrmaeID * sizeof(SEEKPOS_FRAME);
	pDlg->m_AVIFile.Write(&dwRetVal, 4);

	if (pDlg->m_bEnd == false)	
	{
		for(int i = 0; i < nFrmaeID; i++)
		{
			pDlg->m_AVIFile.Write(&pDlg->m_SeekPosFrame[i], sizeof(SEEKPOS_FRAME));
		}
	}

	dwRetVal = pDlg->m_AVIFile.GetPosition() - 8;
	pDlg->m_AVIFile.Seek(4, CFile::begin);
	pDlg->m_AVIFile.Write(&dwRetVal, 4);
	pDlg->m_AVIFile.Seek(0x30, CFile::begin);
	pDlg->m_AVIFile.Write(&nCounts, 4);
	pDlg->m_AVIFile.Seek(0x8C, CFile::begin);
	pDlg->m_AVIFile.Write(&nCounts, 4);

	dwRetVal = dwRetVal / 10;
	pDlg->m_AVIFile.Write(&dwRetVal, 4);

	dwRetVal = 0;
	pDlg->m_AVIFile.Seek(0x90,CFile::begin);
	pDlg->m_AVIFile.Write(&dwRetVal,4);

	dwRetVal = 0x100001;
	pDlg->m_AVIFile.Seek(0x134,CFile::begin);
	pDlg->m_AVIFile.Write(&dwRetVal,4);
	pDlg->m_AVIFile.Close();

	MessageBox(NULL, _T("AVI Conversion Success!!!"), NULL, 0);
	pDlg->OnStartBtn();
	pDlg->m_bEnd = false;
	SendMessage(pDlg->GetSafeHwnd(), WM_CLOSE, (WPARAM)pDlg, NULL);
	return 0;
}

void CAVIConverterDlg::OnStartBtn() 
{
	HBITMAP hBmp;
	if (m_bStart == true)	
	{
		hBmp = ::LoadBitmap(AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BUTTON_START));
		m_btn_Start.SetBitmap(hBmp);
		m_progress.SetPos(0);
		m_bEnd = true;
		m_bStart = false;
	}
	else
	{
		if (m_bradio16_9 || m_bradio4_3 || m_bRadioOriginalSize) 
		{
			AfxBeginThread(AVICnvertingThreadProc, this);
			hBmp = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_BITMAP_AVI_STOP));
			m_btn_Start.SetBitmap(hBmp);
			m_bEnd = false;
			m_bStart = true;
		}
		else 
		{
			MessageBox(L"Please choose the rate options", NULL, NULL);
		}
	}
	// TODO: Add your control notification handler code here
}

void CAVIConverterDlg::OnOutputOpenButton() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_edit_output.GetWindowText(str);
	ShellExecute(m_hWnd, _T("explore"), str, NULL, NULL, SW_SHOWNORMAL);
}

void CAVIConverterDlg::OnTargetOpenButton() 
{
	// TODO: Add your control notification handler code here
	CFileFind finder;
	BOOL bWorking = finder.FindFile(m_FolderPath + "*.*");
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;
		if (m_AVIFileName == finder.GetFileName())
		{
			CString str;
			m_edit_output.GetWindowText(str);
			ShellExecute(m_hWnd, _T("explore"), str, NULL, NULL, SW_SHOWNORMAL);
		}
	}
	finder.Close();
	MessageBox(_T("Cannot DestFile!"), NULL, 0);
}


void CAVIConverterDlg::OnChangeButton() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi;
	CString str;
	wchar_t name[260];
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = name;
	bi.lpszTitle = _T("Select Folder");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);

	if (idl == NULL) 
	{
		MessageBox(_T("Retry Open Folder!!!"), NULL, 0);
		return;
	}

	SHGetPathFromIDList(idl, str.GetBuffer(260));
	str.ReleaseBuffer();
	m_FolderPath = str;
	if (str.GetAt(str.GetLength() - 1) !='\\') m_FolderPath += "\\";
	m_edit_output.SetWindowText(m_FolderPath);
	m_edit_target.SetWindowText(m_FolderPath + m_AVIFileName);
	m_AVIFilePath = m_FolderPath + m_AVIFileName;
}

void CAVIConverterDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_bEnd = true;
	CDialog::OnClose();
}

void CAVIConverterDlg::OnBnClickedRadio169()
{
	m_bRadioOriginalSize = false;
	m_bradio16_9 = true;
	m_bradio4_3 = false;
}

void CAVIConverterDlg::OnBnClickedRadioOriginalSize()
{	
	m_bRadioOriginalSize = true;
	m_bradio16_9 = false;
	m_bradio4_3 = false;
}

void CAVIConverterDlg::OnBnClickedRadio43()
{
	m_bRadioOriginalSize = false;
	m_bradio16_9 = false;
	m_bradio4_3 = true;
}
