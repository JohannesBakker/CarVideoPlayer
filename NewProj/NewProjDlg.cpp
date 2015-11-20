// NewProjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewProjDlg dialog
CView1Dlg					CNewProjDlg::m_View1Dlg;
CView2Dlg					CNewProjDlg::m_View2Dlg;
bool						CNewProjDlg::g_bViewChange;

CRect						CNewProjDlg::m_AboutRt;
BYTE						CNewProjDlg::m_byScaleMode;
BYTE						CNewProjDlg::m_bPlayerSatus;
BYTE						CNewProjDlg::m_byWindowsCounts;
BYTE						CNewProjDlg::m_byWtrMkVerify;
BYTE						CNewProjDlg::m_bDateWindow;

WCHAR						CNewProjDlg::m_wLanguageRes[716][100];
ADJUST_CONFIG				CNewProjDlg::g_adjust_Value_ST;
IO_LIGHT_CONFIG				CNewProjDlg::g_io_Light_Value_ST;
Config_Datas				CNewProjDlg::g_config_Value_ST;
CFile						CNewProjDlg::m_ConfigFile;
CString						CNewProjDlg::m_FirstPathArrays[20];
CString						CNewProjDlg::m_SecondPathArrays[20];
int							CNewProjDlg::m_nPathCounts;
int							CNewProjDlg::m_nPathArraysPtr;
bool						CNewProjDlg::g_bResetFlag;
SYSTEMTIME					CNewProjDlg::m_DateTime[20];
DWORD						CNewProjDlg::m_dwDurations[20];
bool						CNewProjDlg::g_bOpenFlag;
float						CNewProjDlg::m_fSeekPos;
CString						CNewProjDlg::m_FolderName;
SYSTEMTIME					CNewProjDlg::m_FolderTime;
int							CNewProjDlg::g_nPlayState;

CInfoListDlg				CNewProjDlg::m_InfoListDlg;
CGPSDlg 					CNewProjDlg::m_GPSDlg;



CNewProjDlg::CNewProjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	LPSECURITY_ATTRIBUTES lp = new SECURITY_ATTRIBUTES;
	lp->bInheritHandle = FALSE;
	lp->lpSecurityDescriptor = NULL;
	lp->nLength = 100;
	BOOL bResult = CreateDirectory(_T("C:\\DVR264"), lp);
	if (bResult == TRUE) bResult = CreateDirectory(_T("C:\\DVR264\\Output"), lp);
	delete lp;
	m_FolderPath = "";
	m_FirstFilePath = "";
	m_SecondFilePath = "";
	m_bDlg = false;
	g_bViewChange = false;
	g_bResetFlag = false;
	g_bOpenFlag = false;
	if (!m_ConfigFile.Open(_T(CONFIG_FILE_PATH), CFile::modeReadWrite)){
		MessageBox(_T("Error Reading Config Datas"), NULL, 0);
		ZeroMemory(&g_config_Value_ST, sizeof(Config_Datas));
	}else{
		m_ConfigFile.SeekToBegin();
		//m_file.Write(&g_config_Value_ST, sizeof(Config_Datas));
		m_ConfigFile.Read(&g_config_Value_ST, sizeof(Config_Datas));
		m_bDateWindow = g_config_Value_ST.bDateTimeWnd;
	}
	GetLanguageResource(g_config_Value_ST.nLanguageID);
	CNewProjDlg::g_config_Value_ST.nWndCounts = 0;
	m_ConfigFile.Close();
	g_nPlayState = STATE_STOP;
}

void CNewProjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewProjDlg, CDialog)
	//{{AFX_MSG_MAP(CNewProjDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_OPEN, OnFileOpen)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_WM_SIZE()
	ON_COMMAND(IDM_SETTINGS, OnSettings)
	ON_COMMAND(IDM_ABOUT, OnAbout)
	ON_COMMAND(IDM_IO_LIGHT_SET, OnIoLightSet)
	ON_COMMAND(IDM_SEARCH, OnSearch)
	ON_COMMAND(IDM_BURN, OnBurn)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewProjDlg message handlers

BOOL CNewProjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	RECT mainRT;
	GetWindowRect(&mainRT);

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	m_RecordPlayerAboutDlg.Create(IDD_ABOUT, this);
	m_RecordPlayerAboutDlg.SetWindowText(CNewProjDlg::m_wLanguageRes[266]);
	CRect rt;
	GetClientRect(&rt);
	int cx = rt.Width();
	int cy = rt.Height();

	m_View1Dlg.Create(IDD_VIEW1,this);
	m_View1Dlg.m_bView1Dlg = true;
	m_View1Dlg.MoveWindow(0, 0 , cx / 3, cy * 3 / 5, TRUE);
	m_View1Dlg.ShowWindow(SW_SHOW);

	m_View2Dlg.Create(IDD_VIEW2, this);
	m_View2Dlg.m_bView2Dlg = true;
	m_View2Dlg.MoveWindow(cx / 3, 0, cx / 3, cy * 3 / 5, TRUE);
	m_View2Dlg.ShowWindow(SW_SHOW);
	CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	if (m_SecondFilePath == "")
	{
		m_View2Dlg.ShowWindow(SW_HIDE);
		m_View1Dlg.MoveWindow(0, 0, cx * 2 / 3, cy * 3 / 5, TRUE);
		m_View1Dlg.ShowWindow(SW_SHOW);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	}
	m_ToolBarDlg.Create(IDD_TOOLBAR, this);
	m_ToolBarDlg.m_bToolbar = true;
	m_ToolBarDlg.MoveWindow(0, cy * 3 / 5, cx * 2 / 3,  50, TRUE);
	m_ToolBarDlg.ShowWindow(SW_SHOW);

	m_InfoListDlg.Create(IDD_INFO_DIALOG, this);
	m_InfoListDlg.m_bInfoList = true;
	m_InfoListDlg.MoveWindow(cx * 2 / 3, cy * 3 / 5, cx / 3, cy * 2 / 5);
	m_InfoListDlg.m_speedGraphWnd.SetSpeedUnit(CNewProjDlg::g_config_Value_ST.nSpeedUnitID);
	m_InfoListDlg.ShowWindow(SW_SHOW);
//Creation Windows
	m_GPSDlg.Create(IDD_GPS_DIALOG, this);
	m_SystemSetupDlg.Create(IDD_SYSTEM_SETUP, this);
	m_SearchDlg.Create(IDD_SEARCH, this);
	m_SearchDlg.SetWindowText(CNewProjDlg::m_wLanguageRes[21]);
	m_IOLightSetDlg.Create(IDD_IO_LIGHT_SET, this);
	m_IOLightSetDlg.SetWindowText(CNewProjDlg::m_wLanguageRes[702]);
	if (m_bDateWindow == TRUE)
	{
//		m_CalendarDlg.Create(IDD_CALENDAR_DIALOG, this);
//		m_CalendarDlg.m_bCalendarDlg = TRUE;
//		m_CalendarDlg.MoveWindow(0, cy * 3 / 5 + 50, cx / 4, cy * 2 / 5 - 50);
//		m_CalendarDlg.ShowWindow(SW_SHOW);
		//m_TimeLineDlg.Create(IDD_TIMELINE_DIALOG, this);
		m_TimeLineDlg.Create(IDD_TIMELINE_DIALOG,(CWnd*) this);
		m_TimeLineDlg.m_bPaint = true;
		m_TimeLineDlg.m_bTimeLineDlg = true;
		m_TimeLineDlg.MoveWindow(cx / 4, cy * 3 / 5 + 50, cx * 2 / 3 - cx / 4, cy * 2 / 5 - 50);
		m_TimeLineDlg.ShowWindow(SW_SHOW);
	}else{
		m_TimeLineDlg.m_bPaint = true;
		m_TimeLineDlg.Create(IDD_TIMELINE_DIALOG, this);
		m_TimeLineDlg.m_bTimeLineDlg = true;
		m_TimeLineDlg.MoveWindow(0, cy * 3 / 5 + 50, cx * 2 / 3, cy * 2 / 5 - 50);
		m_TimeLineDlg.ShowWindow(SW_SHOW);
	}
	m_GPSDlg.m_bGPSDlg = true;
	m_GPSDlg.MoveWindow(cx * 2 / 3, 0, cx / 3, cy * 3 / 5);
	m_GPSDlg.ShowWindow(SW_SHOW);
	m_GPSDlg.SetSpeedUnit(CNewProjDlg::g_config_Value_ST.nSpeedUnitID);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_bDlg = true;

	ShowWindow(SW_SHOWMAXIMIZED);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNewProjDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNewProjDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNewProjDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) 
{
	TCHAR szDir[MAX_PATH];
	switch(uMsg) 
	{
	case BFFM_INITIALIZED:
		if (GetCurrentDirectory(sizeof(szDir)/sizeof(TCHAR),szDir) )
		{
			memcpy(szDir, CNewProjDlg::g_config_Value_ST.lastpath, 260);
			SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
		}
		break;
	case BFFM_SELCHANGED: 
		if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir)) SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
		break;
	default:
		break;
	}
	return 0;
}

bool bFirstOpen = true;
void CNewProjDlg::OnFileOpen() 
{
	if (m_nPathCounts != 0)
		g_bOpenFlag = true;

	CTimeLineDlg::m_bTimeFlag = false;
	CToolBarDlg::g_bSoundPlay = false;

	Sleep(50);
	// TODO: Add your command handler code here
	CString str = "";
	CString strInfo = "";
	m_FirstFilePath = "";
	m_SecondFilePath = "";

	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = 0;
	bi.pidlRoot = 0;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;

	LPITEMIDLIST idl = new ITEMIDLIST;
	idl = SHBrowseForFolder(&bi);
	
	if (idl == NULL) 
	{
		MessageBox(_T("Retry Open Folder!!!"), NULL, 0);
		m_FirstFilePath = "";
		m_SecondFilePath = "";
		CNewProjDlg::g_config_Value_ST.nWndCounts = 0;
		return;
	}

	SHGetPathFromIDList(idl, str.GetBuffer(260));
	str.ReleaseBuffer();
	CString timeTemp;
	m_FolderPath = str;
	timeTemp = m_FolderPath.Mid(m_FolderPath.GetLength() - 10, 4);
	
	m_FolderTime.wYear = wtoi(timeTemp);
	timeTemp = m_FolderPath.Mid(m_FolderPath.GetLength() - 5, 2);
	m_FolderTime.wMonth = wtoi(timeTemp);
	timeTemp = m_FolderPath.Mid(m_FolderPath.GetLength() - 2, 2);	
	m_FolderTime.wDay = wtoi(timeTemp);
	
	CFile file;
	if (!file.Open(_T(CONFIG_FILE_PATH), CFile::modeReadWrite)) 
	{
		// clear last path
		str.Empty();
		memcpy(CNewProjDlg::g_config_Value_ST.lastpath, str, 260);
		return;
	}

	file.Write(&CNewProjDlg::g_config_Value_ST, sizeof(CNewProjDlg::g_config_Value_ST));
	file.Close();

	strInfo ="D264\\mdvr.log"; 
	if (str.GetAt(str.GetLength() - 1) !='\\') 
		m_FolderPath += "\\";

	CString strTempPath[40];
	int i;
	for(i = 0; i < 40; i++)
		strTempPath[i] = "";

	CFileFind finder;
	BOOL bWorking = finder.FindFile(m_FolderPath + "*.*");
	int FileCount = 0;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) 
			continue;

		if (m_FirstFilePath == "")
		{
			CString strFileName;
			strFileName = finder.GetFileName();
			if (strFileName.Mid(0, 1) == "H" && strFileName.Mid(9, 1) == "-" && strFileName.Mid(16, 2) == "N1" && strFileName.Mid(strFileName.GetLength() - 4, 4) == ".264")
			{
				strTempPath[FileCount] = finder.GetFilePath();
				FileCount++;
			}
		}
	}
	
	finder.Close();

	if (FileCount == 0) 
	{
		MessageBox(_T("No Exist 264 Files!"), NULL, 0);

		// clear last path
		str.Empty();
		memcpy(CNewProjDlg::g_config_Value_ST.lastpath, str, 260);
		return;
	}

	// save last path
	memcpy(CNewProjDlg::g_config_Value_ST.lastpath, str, 260);	

	for(i = 0 ; i < 20; i++)
	{
		m_FirstPathArrays[i] = "";
		m_SecondPathArrays[i] = "";
		ZeroMemory(&m_DateTime[i], sizeof(SYSTEMTIME));
	}

	m_nPathCounts = 0;

	bool bf = false;
	int j = 0;

	do {
		if (strTempPath[j].Mid(strTempPath[j].GetLength() - 8, 4) == "A1A0")
		{
			m_nPathCounts++;
			m_FirstPathArrays[m_nPathCounts - 1] = strTempPath[j];
			m_DateTime[m_nPathCounts - 1].wYear = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 25, 4));
			m_DateTime[m_nPathCounts - 1].wMonth = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 21, 2));
			m_DateTime[m_nPathCounts - 1].wDay = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 19, 2));
			m_DateTime[m_nPathCounts - 1].wHour = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 16, 2));
			m_DateTime[m_nPathCounts - 1].wMinute = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 14, 2));
			m_DateTime[m_nPathCounts - 1].wSecond = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 12, 2));
			bf = true;
		}
		else
		{
			if (bf == false) 
				m_nPathCounts++;

			m_SecondPathArrays[m_nPathCounts - 1] = strTempPath[j];
			
			if (bf == false)
			{
				m_DateTime[m_nPathCounts - 1].wYear = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 25, 4));
				m_DateTime[m_nPathCounts - 1].wMonth = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 21, 2));
				m_DateTime[m_nPathCounts - 1].wDay = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 19, 2));
				m_DateTime[m_nPathCounts - 1].wHour = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 16, 2));
				m_DateTime[m_nPathCounts - 1].wMinute = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 14, 2));
				m_DateTime[m_nPathCounts - 1].wSecond = wtoi(strTempPath[j].Mid(strTempPath[j].GetLength() - 12, 2))-1;
			}
			
			bf = false;
		}

		j++;
	} while (j < FileCount);

	m_ToolBarDlg.m_1stFilePath = m_FirstPathArrays[0];
	m_ToolBarDlg.m_2ndFilePath = m_SecondPathArrays[0];
	m_ToolBarDlg.m_pTimeLineDlg = &m_TimeLineDlg;
	
	if (m_FirstPathArrays[0] == "" && m_SecondPathArrays[0] == "") 
		m_ToolBarDlg.m_nThreadCounts = 0;

	if ((m_FirstPathArrays[0] != "" && m_SecondPathArrays[0] == "") || (m_FirstPathArrays[0] == "" && m_SecondPathArrays[0] != "")) 
		m_ToolBarDlg.m_nThreadCounts = 1;

	if (m_FirstPathArrays[0] != "" && m_SecondPathArrays[0] != "") 
		m_ToolBarDlg.m_nThreadCounts = 2;

	m_ToolBarDlg.m_bCheckChannel = true;
	memcpy(&m_ToolBarDlg.m_DateTime, &m_DateTime[0], sizeof(SYSTEMTIME));
	
	if (bFirstOpen) 
		bFirstOpen = false;
	else 
		m_InfoListDlg.m_fileList.DeleteAllItems();

	for(i = m_nPathCounts-1; i >=0 ; i--)
	{
		m_InfoListDlg.load_event_list(strInfo,m_DateTime[i]);
	
		CFile file;
		if (m_FirstPathArrays[i] != "")
		{
			
			if (!file.Open(m_FirstPathArrays[i], CFile::modeRead)) 
				return;

			m_dwDurations[i] = GetDuration(&file);
			if (m_SecondPathArrays[i] != "")
			{
				CFile _2ndFile;

				if (!_2ndFile.Open(m_SecondPathArrays[i], CFile ::modeRead)) 
					return;

				char temp[256];
				sprintf(temp, " %d-%02d-%02d \n", m_DateTime[i].wYear, m_DateTime[i].wMonth, m_DateTime[i].wDay);
				m_InfoListDlg.load_file_list(m_dwDurations[i], m_DateTime[i], 2, file.GetLength(), _2ndFile.GetLength(), i);
				_2ndFile.Close();
			}
			else 
				m_InfoListDlg.load_file_list(m_dwDurations[i], m_DateTime[i], 1, file.GetLength(), 0, i);

			file.Close();
		}
		else
		{ 
			if (!file.Open(m_SecondPathArrays[0], CFile::modeRead)) 
				return;
			m_dwDurations[i] = GetDuration(&file);
			m_InfoListDlg.load_file_list(m_dwDurations[i], m_DateTime[i], 1, 0, file.GetLength(), i);
			file.Close();
		}
	}

	m_nPathArraysPtr = 0;
	m_ToolBarDlg.ReSet();

	CRect rt;
	GetClientRect(&rt);

	CNewProjDlg::g_config_Value_ST.nWndCounts = 0;
	if (m_FirstPathArrays[0] == "")
	{
		m_View1Dlg.ShowWindow(SW_HIDE);
		m_View2Dlg.MoveWindow(0, 0 , rt.Width() * 2 / 3, rt.Height() * 3 / 5);
		m_ToolBarDlg.Init_VideoOut(&m_View1Dlg, &m_View2Dlg);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	}

	if (m_SecondPathArrays[0] == "")
	{	
		m_View2Dlg.ShowWindow(SW_HIDE);
		m_View1Dlg.MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3 / 5);
		m_ToolBarDlg.Init_VideoOut(&m_View1Dlg, &m_View2Dlg);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	}
	else
	{
		m_View1Dlg.MoveWindow(0, 0, rt.Width() / 3, rt.Height() * 3 / 5);
		m_View2Dlg.MoveWindow(rt.Width() / 3, 0, rt.Width() / 3, rt.Height() * 3 / 5);
		m_View2Dlg.ShowWindow(SW_SHOW);
		m_ToolBarDlg.Init_VideoOut(&m_View1Dlg, &m_View2Dlg);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	}

	for(int i = 0 ; i < CNewProjDlg::m_nPathCounts; i++)
		m_ToolBarDlg.m_pInfoListDlg->m_fileList.UnSelectItem(i);

	m_ToolBarDlg.m_pInfoListDlg->m_fileList.SelectItem(CNewProjDlg::m_nPathArraysPtr, true);
	m_InfoListDlg.m_fileList.SetItemText(0, 7, _T("Played"));

	m_GPSDlg.ResetGpsDialog(true);
}

void CNewProjDlg::OnExit() 
{
	// TODO: Add your command handler code here
	CFile file;
	file.Open(_T(CONFIG_FILE_PATH), CFile::modeWrite);
	file.Write(&g_config_Value_ST, sizeof(Config_Datas));
	file.Close();
	
	CDialog::OnOK();
}

void CNewProjDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (m_bDlg)
	{
		if (m_SecondFilePath == "")
		{	
			m_View2Dlg.ShowWindow(SW_HIDE);
			m_View1Dlg.MoveWindow(0, 0, cx * 2 / 3, cy * 3  / 5);
			m_View1Dlg.ShowWindow(SW_SHOW);
		}else{
			m_View1Dlg.MoveWindow(0, 0, cx / 3, cy * 3 / 5);
			m_View2Dlg.MoveWindow(cx / 3, 0, cx / 3, cy * 3 / 5);
			m_View2Dlg.ShowWindow(SW_SHOW);
		}
		m_GPSDlg.m_bGPSDlg = true;
		m_GPSDlg.MoveWindow(cx * 2 / 3, 0, cx / 3, cy * 3 / 5);
		m_ToolBarDlg.MoveWindow(0, cy * 3 / 5, cx * 2 / 3, 50);
		m_InfoListDlg.MoveWindow(cx * 2 / 3, cy * 3 / 5, cx / 3, cy * 2 / 5);
		if (m_bDateWindow == TRUE)
		{
//			m_CalendarDlg.MoveWindow(0, cy * 3 / 5 + 50, cx / 4, cy * 2 / 5 - 50);
			m_TimeLineDlg.m_bPaint = true;
			m_TimeLineDlg.MoveWindow(cx / 4, cy * 3 / 5 + 50, cx * 2 / 3 - cx / 4, cy * 2 / 5 - 50);
			m_TimeLineDlg.ShowWindow(SW_SHOW);
		}else{
			m_TimeLineDlg.m_bPaint = true;
			m_TimeLineDlg.MoveWindow(0, cy * 3 / 5 + 50, cx * 2 / 3, cy * 2 / 5 - 50);
			m_TimeLineDlg.ShowWindow(SW_SHOW);
		}
		CRect rt;
		GetClientRect(&rt);
		m_AboutRt.left = rt.Width() / 2 - 200;
		m_AboutRt.right = rt.Width() / 2 + 200;
		m_AboutRt.top = rt.Height() / 2 - 100;
		m_AboutRt.bottom = rt.Height() / 2 + 100;
		if (CNewProjDlg::g_config_Value_ST.nWndCounts == 1)
		{
			m_View2Dlg.ShowWindow(SW_HIDE);
			m_View1Dlg.MoveWindow(0, 0, cx * 2 / 3, cy * 3  / 5);
			m_View1Dlg.ShowWindow(SW_SHOW);
		}
		else if (CNewProjDlg::g_config_Value_ST.nWndCounts == 2)
		{
			m_View1Dlg.MoveWindow(0, 0, cx / 3, cy * 3  / 5);
			m_View2Dlg.MoveWindow(cx / 3, 0, cx / 3, cy * 3 / 5);
			m_View1Dlg.ShowWindow(SW_SHOW);
			m_View2Dlg.ShowWindow(SW_SHOW);			
		}
		m_ToolBarDlg.ShowWindow(SW_SHOW);
		m_GPSDlg.ShowWindow(SW_SHOW);
		m_InfoListDlg.ShowWindow(SW_SHOW);
		m_ToolBarDlg.m_bFullScreen = FALSE;
		
	}
	m_ToolBarDlg.SetViewDlgs(&m_View1Dlg, &m_View2Dlg,&m_InfoListDlg,&m_GPSDlg,&m_TimeLineDlg,&m_ToolBarDlg);
	// TODO: Add your message handler code here
}

void CNewProjDlg::OnSettings()
{
	// TODO: Add your command handler code here
	m_SystemSetupDlg.m_cmb_windows.SetCurSel(CNewProjDlg::g_config_Value_ST.nWndCounts - 1);
	m_SystemSetupDlg.m_cmb_player_view_scale.SetCurSel(CNewProjDlg::g_config_Value_ST.nViewScaleID);
	if (CNewProjDlg::g_config_Value_ST.bShowChanNum == true) m_SystemSetupDlg.m_channelCheckBtn.SetCheck(1);
	else m_SystemSetupDlg.m_channelCheckBtn.SetCheck(0);
	if (CNewProjDlg::g_config_Value_ST.bUseRGB16 == true) m_SystemSetupDlg.m_rgb16Btn.SetCheck(1);
	else m_SystemSetupDlg.m_rgb16Btn.SetCheck(0);
	if (CNewProjDlg::g_config_Value_ST.bWtrmkVerify == true) m_SystemSetupDlg.m_waterMarkBtn.SetCheck(1);
	else m_SystemSetupDlg.m_waterMarkBtn.SetCheck(0);

	m_SystemSetupDlg.ShowWindow(SW_SHOW);
}

void CNewProjDlg::OnAbout() 
{
	// TODO: Add your command handler code here
	CRect rt;
	GetClientRect(&rt);
	m_AboutRt.left = rt.left + rt.Width() / 2 - 200;
	m_AboutRt.right = rt.Width() / 2 + 200;
	m_AboutRt.top = rt.top + rt.Height() / 2 - 100;
	m_AboutRt.bottom = rt.Height() / 2 + 100;
	m_RecordPlayerAboutDlg.MoveWindow(m_AboutRt);
	m_RecordPlayerAboutDlg.ShowWindow(SW_SHOW);
}

void CNewProjDlg::OnIoLightSet() 
{
	// TODO: Add your command handler code here
	m_IOLightSetDlg.ShowWindow(SW_SHOW);
}

void CNewProjDlg::OnSearch() 
{
	// TODO: Add your command handler code here
	m_SearchDlg.ShowWindow(SW_SHOW);
}

void CNewProjDlg::OnBurn() 
{
	// TODO: Add your command handler code here
	m_BurnToDiscDlg.DoModal();
}

BOOL CNewProjDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));
	return CDialog::OnEraseBkgnd(pDC);
}

bool CNewProjDlg::GetLanguageResource(int nLanguageID)
{
	CString		strLangPath;
	LPCWSTR		dumpline = NULL;
	WCHAR		wStr[100], wCh;
	if (nLanguageID < 0 && nLanguageID > 6) return false;
	switch(nLanguageID)
	{
	case 0:
		strLangPath = "Language\\chinese";
		break;
	case 1:
		strLangPath = "Language\\english";
		break;
	case 2:
		strLangPath = "Language\\chineseF";
		break;
	case 3:
		strLangPath = "Language\\thai";
		break;
	case 4:
		strLangPath = "Language\\spain";
		break;
	case 5:
		strLangPath = "Language\\Portugal";
		break;
	case 6:
		strLangPath = "Language\\Turkish";
		break;
	}
	CFile file;
	if (!file.Open(strLangPath, CFile::modeRead))
		MessageBox(_T("Error Reading Language Resource!"));
	file.Read(&wStr, 2);
	int i = 0;
	int j = 0;
	while(file.Read(&wCh, 2) > 0)
	{
		if (wCh == 0x000D) continue;
		if (wCh == 0x000A)
		{
			j++;
			i = 0;
			continue;
		}
		if (i > 5) m_wLanguageRes[j][i - 6] = wCh;
		i++;
	}
	return true;
}
void WndsBK_Paint(CView1Dlg* pDlg1, CView2Dlg* pDlg2, int nSelWnd)
{
	CDC* pDC1 = pDlg1->GetDC();
	CRect rt1;
	pDlg1->GetWindowRect(&rt1);
	if (nSelWnd == 1) pDC1->FillSolidRect(0, 0, rt1.Width(), rt1.Height(), RGB(255, 0, 0));
	else pDC1->FillSolidRect(0, 0, rt1.Width(), rt1.Height(), RGB(0, 0, 255));
	pDC1 = pDlg1->GetDlgItem(IDC_STATIC_VIEW1)->GetDC();
	pDlg1->GetDlgItem(IDC_STATIC_VIEW1)->GetClientRect(&rt1);
	pDC1->FillSolidRect(0, 0, rt1.Width(), rt1.Height(), RGB(0, 0, 0));

	CDC*pDC2 = pDlg2->GetDC();
	CRect rt2;
	pDlg2->GetWindowRect(&rt2);
	if (nSelWnd == 1) pDC2->FillSolidRect(0, 0, rt2.Width(), rt2.Height(), RGB(0, 0, 255));
	else pDC2->FillSolidRect(0, 0, rt2.Width(), rt2.Height(), RGB(255, 0, 0));
	pDC2 = pDlg2->GetDlgItem(IDC_STATIC_VIEW2)->GetDC();
	pDlg2->GetDlgItem(IDC_STATIC_VIEW2)->GetClientRect(&rt2);
	pDC2->FillSolidRect(0, 0, rt2.Width(), rt2.Height(), RGB(0, 0, 0));
}

DWORD CNewProjDlg::GetDuration(CFile* m_file)
{
	int n = 0;
	PACKET_CONST_HEADER pch;
	PACKET00_HEADER p00h;
	PACKET01_HEADER p01h;
	PACKET08_HEADER p08h;
	PACKET09_HEADER p09h;
	PACKET10_HEADER p10h;
	PACKET11_HEADER p11h;
	m_file->Seek(ASF_FILE_HEAD_SIZE, CFile::begin);
	m_file->Read(&pch, sizeof(PACKET_CONST_HEADER));
	m_file->Read(&p00h, sizeof(PACKET00_HEADER));
	DWORD dwFirstDTS =  p00h.dwDTS;
	
	DWORD m_dwLastDTS;
	int PacketCounts = (int)(m_file->GetLength() - ASF_FILE_HEAD_SIZE) / PACKET_SIZE;
	do{
		m_file->Seek((PacketCounts - n) * PACKET_SIZE + ASF_FILE_HEAD_SIZE, CFile::begin);
		m_file->Read(&pch, sizeof(PACKET_CONST_HEADER));
		if (pch.byHead == 0x82 && pch.wdConst == 0x0000 && pch.byConst == 0x5d)
		{
			switch(pch.byType)
			{
			case PACKET10_TYPE:
				m_file->Read(&p10h, sizeof(PACKET10_HEADER));
				m_dwLastDTS = p10h.dwDTS;
				return m_dwLastDTS - dwFirstDTS;
			case PACKET11_TYPE:
				m_file->Read(&p11h, sizeof(PACKET11_HEADER));
				m_dwLastDTS = p11h.dwDTS;
				return m_dwLastDTS - dwFirstDTS;
			case PACKET00_TYPE:
				m_file->Read(&p00h, sizeof(PACKET00_HEADER));
				m_dwLastDTS = p00h.dwDTS;
				if (m_dwLastDTS != 0 && m_dwLastDTS > dwFirstDTS) return m_dwLastDTS - dwFirstDTS;
				break;
			case PACKET01_TYPE:
				m_file->Read(&p01h, sizeof(PACKET01_HEADER));
				m_dwLastDTS = p01h.dwDTS;
				if (m_dwLastDTS != 0 && m_dwLastDTS > dwFirstDTS) return m_dwLastDTS - dwFirstDTS;
				break;
			case PACKET08_TYPE:
				m_file->Read(&p08h, sizeof(PACKET08_HEADER));
				m_dwLastDTS = p08h.dwDTS;
				if (m_dwLastDTS != 0 && m_dwLastDTS > dwFirstDTS) return m_dwLastDTS - dwFirstDTS;
				break;
			case PACKET09_TYPE:
				m_file->Read(&p09h, sizeof(PACKET09_HEADER));
				m_dwLastDTS = p09h.dwDTS;
				if (m_dwLastDTS != 0 && m_dwLastDTS > dwFirstDTS) return m_dwLastDTS - dwFirstDTS;
				break;
			default:
				break;
			}
		}
		n++;
	}while(1);
}

void CNewProjDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	// TODO: Add your message handler code here
}

void CNewProjDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags,point);
}

int Unichar2Ascii(WCHAR *unichar, char* asciichar)
{
	int i_len;
	i_len = WideCharToMultiByte(CP_ACP,	WC_NO_BEST_FIT_CHARS, unichar, wcslen(unichar),	asciichar, MAX_PATH, NULL, NULL);
	asciichar [i_len] = 0;
	return i_len;
}

int Ascii2Unichar(char* asciichar, WCHAR *unichar)
{
	int i_len;
	i_len = MultiByteToWideChar(CP_ACP,	MB_PRECOMPOSED,	asciichar, -1, unichar,	MAX_PATH);
	unichar[i_len] = 0;
	return i_len;
}

int wtoi(CString str)
{
	char chAscii[100];
	memset(chAscii, 0, sizeof(chAscii));
	WideCharToMultiByte(CP_ACP,	WC_NO_BEST_FIT_CHARS, str, wcslen(str), chAscii, MAX_PATH, NULL, NULL);
	int nRet = atoi(chAscii);
	return nRet;
}

void CNewProjDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	// TODO: Add your command handler code here
	CFile file;
	file.Open(_T(CONFIG_FILE_PATH), CFile::modeWrite);
	file.Write(&g_config_Value_ST, sizeof(Config_Datas));
	file.Close();

	//CDialog::OnClose();
	CDialog::OnCancel();
}

void CNewProjDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	// comment for none closing
}


void CNewProjDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// comment for none closing
}
