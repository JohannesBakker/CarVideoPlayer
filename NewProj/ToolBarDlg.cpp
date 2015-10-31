// ToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "ToolBarDlg.h"
#include "TimeLineDlg.h"
#include <WinUser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BUTTON_FIXED_SIZE 28
#define PI 3.141592653
#define  HSLMAX   240	/* H,L, and S vary over 0-HSLMAX */
#define  RGBMAX   255   /* R,G, and B vary over 0-RGBMAX */
/* HSLMAX BEST IF DIVISIBLE BY 6 */
/* RGBMAX, HSLMAX must each fit in a byte. */
/* Hue is undefined if Saturation is 0 (grey-scale) */
/* This value determines where the Hue scrollbar is */
/* initially set for achromatic colors */
#define UNDEFINED (HSLMAX*2/3)
/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg dialog

#define TIMER_PLAYER1	20001
#define TIMER_PLAYER2	20002
#define TIMER_SLIDER	20008
#define TIMER_ALARM		20009
#define TIMER_RECORD_ALARM		20010


HWAVEOUT		g_hWaveOut;
#define VIDEOW 704
#define VIDEOH 480
#define BITMAPINFO_INIT\
	BITMAPINFO bmi;\
	bmi.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);\
	bmi.bmiHeader.biCompression     = BI_RGB;\
	bmi.bmiHeader.biWidth           = VIDEOW;\
	bmi.bmiHeader.biHeight          = VIDEOH;\
	bmi.bmiHeader.biPlanes          = 1;\
	bmi.bmiHeader.biBitCount        = 24;\
	bmi.bmiHeader.biSizeImage       = VIDEOW*VIDEOH*bmi.bmiHeader.biBitCount/8;\
	bmi.bmiHeader.biXPelsPerMeter   = 0;\
	bmi.bmiHeader.biYPelsPerMeter   = 0;\
	bmi.bmiHeader.biClrUsed         = 0;\
	bmi.bmiHeader.biClrImportant    = 0;

#define GRAPHSETTING\
	pDisPos->GetClientRect(&rect);\
	HDC	hDC, hMemDC;\
	HBITMAP hBmp, hOldBmp;\
	hDC = pDisPos->GetDC()->GetSafeHdc();\
	hMemDC = CreateCompatibleDC(hDC);\
	hBmp = CreateCompatibleBitmap(hDC, VIDEOW, VIDEOH);\
	hOldBmp = (HBITMAP)SelectObject(hMemDC,  (HGDIOBJ)hBmp);
CBatchConvertDlg			CToolBarDlg::m_BatchDlg;
TempBinData					CInfoListDlg::m_pBinData[100000];
CFile						CToolBarDlg::m_1stFile;
CFile						CToolBarDlg::m_2ndFile;
tagTIMESTAMP_STRUCT			CToolBarDlg::m_DateTime;
CString						CToolBarDlg::m_1stFilePath;
CString						CToolBarDlg::m_2ndFilePath;
DWORD						CToolBarDlg::m_duration;
char						CToolBarDlg::ss_buf[704 * 480 * 3];
char						CToolBarDlg::ss_buf2[1408 * 480 * 3];
bool						CToolBarDlg::g_bView1Turn_ST;
bool						CToolBarDlg::g_bView2Turn_ST;	
CString						CToolBarDlg::m_AviPathStr;
CFile*						CToolBarDlg::m_pAviFile;
CAVIConverterDlg			CToolBarDlg::m_AVIConverterDlg;
C264Spliter					CToolBarDlg::m_1stSpliter;
C264Spliter					CToolBarDlg::m_2ndSpliter;
int							CToolBarDlg::m_nStartPos;
int							CToolBarDlg::m_nEndPos;
OUT_DATAS					CToolBarDlg::m_1stOutDatas[10];
OUT_DATAS					CToolBarDlg::m_2ndOutDatas[10];
int							CToolBarDlg::m_nOutDatasPtr;
int							CToolBarDlg::m_nPlayPtr;
bool						CToolBarDlg::g_bSoundPlay = false;
DWORD						CToolBarDlg::g_dwCurDTS;
CPipe*						CToolBarDlg::m_pipe;
bool						CToolBarDlg::g_ErrRead;
//bool						CToolBarDlg::g_bAudioThread;
CToolBarDlg::CToolBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarDlg)
	//}}AFX_DATA_INIT
	m_bToolbar = FALSE;
	m_bVolume = FALSE;
	m_bCheckChannel = true;
	m_n1stTimeCount = 0;
	m_n2ndTimeCount = 0;
	m_nScaleMode = 0;
	m_nSpeed = 1000;
	m_bPlayer1 = false;
	m_bPlayer2 = false;
	m_i	= 0;
	g_bView1Updown_ST = false;
	g_bView2Updown_ST = false;
	g_bView1Turn_ST = false;
	g_bView2Turn_ST = false;
	m_bFullScreen = FALSE;
	m_bSignal = FALSE;
	m_bSeekSignal = FALSE;
	m_pbmpFileHeader = new BITMAPFILEHEADER;
	m_pbmpInfoHeader = new BITMAPINFOHEADER;
	m_AviPathStr = "";
	g_ErrRead = false;
	sprintf(m_statusStr,"%s", "Normal Play");
	ZeroMemory(&m_DateTime, sizeof(m_DateTime));
	//m_lpHdr = new WAVEHDR;

	m_bAlarmOn = false;
}

void CToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarDlg)
	DDX_Control(pDX, IDC_STATIC_NUMBER_TWO, m_number2);
	DDX_Control(pDX, IDC_STATIC_NUMBER_THREE, m_number3);
	DDX_Control(pDX, IDC_STATIC_NUMBER_ONE, m_number1);
	DDX_Control(pDX, IDC_VOLUME_PICTURE, m_picture_volume);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_slider_volume);
	DDX_Control(pDX, IDC_SLIDER_SEEK, m_Slider_Seek);
	DDX_Control(pDX, IDC_ADJUST, m_Btn_Adjust);
	DDX_Control(pDX, IDC_FAST, m_Btn_Fast);
	DDX_Control(pDX, IDC_START_CUT, m_Btn_StartCut);
	DDX_Control(pDX, IDC_SNAPSHOT, m_Btn_SnapShot);
	DDX_Control(pDX, IDC_SLOW, m_Btn_Slow);
	DDX_Control(pDX, IDC_REPAIR_FILE, m_Btn_RepairFile);
	DDX_Control(pDX, IDC_RECORD_END, m_Btn_RecordEnd);
	DDX_Control(pDX, IDC_RECORD_BEGIN, m_Btn_RecordBegin);
	DDX_Control(pDX, IDC_PREV_SEG, m_Btn_PrevSeg);
	DDX_Control(pDX, IDC_NORMAL, m_Btn_Normal);
	DDX_Control(pDX, IDC_NEXT_SEG, m_Btn_NextSeg);
	DDX_Control(pDX, IDC_FULLSCREEN, m_Btn_FullScreen);
	DDX_Control(pDX, IDC_FORWARD, m_Btn_Forward);
	DDX_Control(pDX, IDC_END_CUT, m_Btn_EndCut);
	DDX_Control(pDX, IDC_BACK, m_Btn_Back);
	DDX_Control(pDX, IDC_AVI, m_Btn_AVI);
	DDX_Control(pDX, IDC_2FRAME, m_Btn_2Frame);
	DDX_Control(pDX, IDC_1FRAME, m_Btn_1Frame);
	DDX_Control(pDX, IDC_STOP, m_btn_Stop);
	DDX_Control(pDX, IDC_PLAY, m_btn_Play);
	DDX_Control(pDX, IDC_STATUS_STATIC, m_Status_Static);
	//}}AFX_DATA_MAP
	
}

BEGIN_MESSAGE_MAP(CToolBarDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarDlg)
	ON_BN_CLICKED(IDC_PLAY, OnPlayBtnClick)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_AVI, OnAvi)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_VOLUME_PICTURE, OnVolumePicture)
	ON_BN_CLICKED(IDC_ADJUST, OnAdjust)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SEEK, OnCustomdrawSliderSeek)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SEEK, OnReleasedcaptureSliderSeek)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SLIDER_SEEK, OnOutofmemorySliderSeek)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_RECORD_BEGIN, OnRecordBegin)
	ON_BN_CLICKED(IDC_RECORD_END, OnRecordEnd)
	ON_BN_CLICKED(IDC_FAST, OnFast)
	ON_BN_CLICKED(IDC_SLOW, OnSlow)
	ON_BN_CLICKED(IDC_NORMAL, OnNormal)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_START_CUT, OnStartCut)
	ON_BN_CLICKED(IDC_END_CUT, OnEndCut)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SNAPSHOT, OnSnapshot)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_2FRAME, OnTwoWindow)
	ON_BN_CLICKED(IDC_1FRAME, OnOneWindow)
	ON_BN_CLICKED(IDC_FULLSCREEN, OnFullscreen)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_ONE_CAMERA, OnSnapshotOneCamera)
	ON_COMMAND(ID_WHOLE_CAMERA, OnSnapshotWholeCamera)
	//ON_THREAD_MESSAGE(MM_WOM_DONE, OnMM_WOM_DONE)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PREV_SEG, &CToolBarDlg::OnBnClickedPrevSeg)
	ON_BN_CLICKED(IDC_NEXT_SEG, &CToolBarDlg::OnBnClickedNextSeg)
	ON_BN_CLICKED(IDC_REPAIR_FILE, &CToolBarDlg::OnBnClickedRepairFile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg message handlers

UINT SoundPlayThread(LPVOID lParam)
{
	CToolBarDlg* pDlg = (CToolBarDlg*)lParam;
	LPWAVEHDR lpHdr = new WAVEHDR;
		if(lpHdr == NULL) return 0;
	while(pDlg->g_bSoundPlay == true)
	{
		
		ZeroMemory(lpHdr, sizeof(WAVEHDR));
		if(CNewProjDlg::g_config_Value_ST.nSelWndID == 1)
			lpHdr->lpData = pDlg->m_1stOutDatas[pDlg->m_nPlayPtr].buf_A;
		else 
			lpHdr->lpData = pDlg->m_2ndOutDatas[pDlg->m_nPlayPtr].buf_A;
		lpHdr->dwBufferLength = 1600;
		lpHdr->dwFlags = WHDR_DONE;
		pDlg->m_nPlayPtr ++;
		if(pDlg->m_nPlayPtr == 30) pDlg->m_nPlayPtr = 0;
		MMRESULT mmReturn = waveOutPrepareHeader(g_hWaveOut,lpHdr, sizeof(WAVEHDR));
		waveOutSetPlaybackRate(g_hWaveOut, 0x8000);
		if(mmReturn) return 0;
		mmReturn = waveOutWrite(g_hWaveOut, lpHdr, sizeof(WAVEHDR));
		Sleep(98);
	}
	waveOutClose(g_hWaveOut);
	//delete lpHdr;
	return 0;	
}
void FirstSoundPlay(HWAVEOUT hWave, char* buf)
{
	LPWAVEHDR lpHdr = new WAVEHDR;
	if(lpHdr == NULL) return;
	lpHdr->dwBufferLength = 1600;
	lpHdr->dwFlags = WHDR_DONE;
	
}
LPWAVEHDR g_lpHdr = new WAVEHDR;
void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, 
                           DWORD dw1, DWORD dw2)
{
	if(uMsg == WOM_OPEN)
	{
		int a = 0;
	}
	if (uMsg == WOM_DONE && CToolBarDlg::g_bSoundPlay == true) {
        //ReleaseSemaphore((HANDLE)dwUser, 1, NULL);
		//LPWAVEHDR lpHdr = new WAVEHDR;
		//if(lpHdr == NULL) return;
		ZeroMemory(g_lpHdr, sizeof(WAVEHDR));\
		if(CNewProjDlg::g_config_Value_ST.nSelWndID == 1)\
			g_lpHdr->lpData = CToolBarDlg::m_1stOutDatas[CToolBarDlg::m_nPlayPtr].buf_A;\
		else\
			g_lpHdr->lpData = CToolBarDlg::m_2ndOutDatas[CToolBarDlg::m_nPlayPtr].buf_A;\
		g_lpHdr->dwBufferLength = 1600;\
		g_lpHdr->dwFlags = WHDR_DONE;\
		CToolBarDlg::m_nPlayPtr++;\
		if(CToolBarDlg::m_nPlayPtr == 30)\
			CToolBarDlg::m_nPlayPtr = 0;\
		MMRESULT mmReturn = waveOutPrepareHeader(g_hWaveOut,g_lpHdr, sizeof(WAVEHDR));\
		waveOutSetPlaybackRate(g_hWaveOut, 0x8000);\
		if(mmReturn)\
			return;\
		mmReturn = waveOutWrite(g_hWaveOut, g_lpHdr, sizeof(WAVEHDR));\
		//delete lpHdr;
    }
}

void CToolBarDlg::OnPlayBtnClick()
{
	// TODO: Add your control notification handler code here
	m_bDrawSeek = FALSE;
	if(m_1stFilePath == "") return;
	HBITMAP hBmp;
	CBitmap bmp;
	HINSTANCE hInst;
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(20170), RT_BITMAP);
	GetDlgItem(IDC_STOP)->EnableWindow(true);
	Init_PlayOpertion(m_1stFilePath, m_2ndFilePath);
	Init_VideoOut(m_pView1Dlg, m_pView2Dlg);
	wchar_t buf[5];
	CString str;
	float fPos = (float)m_Slider_Seek.GetPos();
	GetDlgItemText(IDC_PLAY,buf, 5);
//	str = buf;
	m_nOutDatasPtr = 0;
	if(buf[1] != 'a')
	{
		CNewProjDlg::g_nPlayState = STATE_PLAY;
		SetDlgItemText(IDC_PLAY, _T("Pause"));
		hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_PAUSE), IMAGE_BITMAP, 0, 0, 0);
		m_btn_Play.SetBitmap(hBmp);
		if(m_nThreadCounts == 2)
		{
			
			if(m_1stSpliter.Seek(fPos / m_Slider_Seek.GetRangeMax()) == false) return;
			if(m_2ndSpliter.Seek(fPos / m_Slider_Seek.GetRangeMax()) == false) return;
			for(int  i = 0 ; i < 5; i++)
			{
				m_1stSpliter.GetDatas1();
				m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
				m_2ndSpliter.GetDatas2();
				m_2ndOutDatas[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas;
				if(CNewProjDlg::g_config_Value_ST.nSelWndID == 1)
					m_pipe->m_audioTrack[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas.buf_A;
				else
					m_pipe->m_audioTrack[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas.buf_A;
				m_nOutDatasPtr ++;

			}
			m_bPlayer1 = true;
			SetTimer(TIMER_PLAYER1, m_nSpeed / 10, NULL);
			m_bPlayer2 = true;
			SetTimer(TIMER_PLAYER2, m_nSpeed / 10, NULL);
			SetTimer(TIMER_SLIDER, 1000, NULL);
			KillTimer(TIMER_ALARM);
		}else if(m_nThreadCounts == 1)
		{
			m_1stSpliter.Seek(fPos / m_Slider_Seek.GetRangeMax());
			for(int i = 0 ; i < 5; i++)
			{
				m_1stSpliter.GetDatas1();
				m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
				m_pipe->m_audioTrack[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas.buf_A;
				m_nOutDatasPtr ++;
			}
			m_bPlayer1 = true;
			SetTimer(TIMER_PLAYER1, m_nSpeed / 10, NULL);
			SetTimer(TIMER_SLIDER, 1000, NULL);
			KillTimer(TIMER_ALARM);
		}
		SetTimer(TIMER_ALARM,100,NULL);
		m_nPlayPtr = 0;
		m_pipe->m_nBufPtr = 0;
		m_pipe->StartPlayingFromFile();
	}else{
		CNewProjDlg::g_nPlayState = STATE_PAUSE;
		m_pipe->StopPlayingFromFile();
		SetDlgItemText(IDC_PLAY, _T("Play"));
		hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_PLAY), IMAGE_BITMAP, 0, 0, 0);
		m_btn_Play.SetBitmap(hBmp);
		g_bSoundPlay = false;
		CTimeLineDlg::m_bTimeFlag = false;
		if(m_nThreadCounts == 2)
		{
			KillTimer(TIMER_SLIDER);
			KillTimer(TIMER_PLAYER1);
			m_bPlayer1 = false;
			//m_1stFile.Close();
			KillTimer(TIMER_PLAYER2);
			KillTimer(TIMER_ALARM);
			m_bPlayer2 = false;
			//m_2ndFile.Close();
		}else if(m_nThreadCounts == 1)
		{
			KillTimer(TIMER_SLIDER);
			KillTimer(TIMER_PLAYER1);
			KillTimer(TIMER_ALARM);
			m_bPlayer1 = false;
			//m_1stFile.Close();
		}
		Video1Out(m_1stOutDatas[m_nPlayPtr].buf_V, m_nScaleMode);
		Video2Out(m_2ndOutDatas[m_nPlayPtr].buf_V, m_nScaleMode);
	}
	
	//SetTimer(20005, 100, NULL);
}

BOOL CToolBarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	binDataNum = 0;
	// TODO: Add extra initialization here
	CToolBarDlg::m_BatchDlg.Create(IDD_BATCH_CONVERT, this);
	m_RecordPlayerAboutDlg.Create(IDD_ABOUT, this);
	m_AVIConverterDlg.Create(IDD_AVI_CONVERTER, this);
	m_AdjustDlg.Create(IDD_ADJUST_VIDEO_PARAM, this);
	m_TimeRangeDlg.Create(IDD_TIME_RANGE, this);
	

	SetDlgItemText(IDC_STATUS_STATIC, _T(" Normal "));
	GetDlgItem(IDC_RECORD_END)->EnableWindow(false);
	GetDlgItem(IDC_END_CUT)->EnableWindow(false);
	CRect rt;
	GetClientRect(&rt);
	int cx = rt.Width();
	int cy = rt.Height();
	m_Slider_Seek.SetTic(100);
	m_Slider_Seek.SetPageSize(1);
	m_slider_volume.SetBitmapChannel( IDB_VOLUME_SLIDER,IDB_VOLUME_SLIDER_ACTIVE);
	m_slider_volume.SetBitmapThumb( IDB_VOLUME_THUMB_ACTIVE, IDB_VOLUME_THUMB_ACTIVE, TRUE );
	m_slider_volume.DrawFocusRect( FALSE );	
	
	m_slider_volume.SetRange( 0, 100 );
	m_slider_volume.SetPos( 100 );
	m_bitItem = new CBitItem(IDB_SEEK_SLIDER, 10, 200, CBitItem::kBitReadMode);
	
 	m_Slider_Seek.BuildThumbItem(IDB_SEEK_CURSOR, 11, 17);
 	m_Slider_Seek.BuildBackItem(m_bitItem, m_bitItem);
 	m_Slider_Seek.SetTopOffset(-5);	
 	m_Slider_Seek.SetLeftOffset(0);
  	m_Slider_Seek.SetLineSize(0);
	
	m_pipe = new CPipe;
	m_pipe->m_audioTrack[0] = this->m_1stOutDatas[this->m_nPlayPtr].buf_A;

	// init record alarm
	m_bAlarmOn = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolBarDlg::OnMove(int x, int y) 
{
	// TODO: Add your message handler code here
	CDialog::OnMove(x, y);
}

void CToolBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if(m_bToolbar)
	{
		if((cx - BUTTON_FIXED_SIZE * 20 - 100) < 180) m_Status_Static.ShowWindow(SW_HIDE);
		else
		{
			m_Status_Static.ShowWindow(SW_SHOW);
			GetDlgItem(IDC_PLAYER_PICTURE)->MoveWindow(cx - 180, cy - BUTTON_FIXED_SIZE, 180, BUTTON_FIXED_SIZE);	
		}
		GetDlgItem(IDC_VOLUME_PICTURE)->MoveWindow(cx - 120, 2, cx, cy - BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_SLIDER_SEEK)->MoveWindow(10, 0, (cx - 170), cy - BUTTON_FIXED_SIZE - 1);
		GetDlgItem(IDC_SLIDER_VOLUME)->MoveWindow(cx - 120 + 20, 2, 80, cy - BUTTON_FIXED_SIZE - 1);	
		GetDlgItem(IDC_PLAY)->MoveWindow(0, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_STOP)->MoveWindow(BUTTON_FIXED_SIZE, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_SLOW)->MoveWindow(BUTTON_FIXED_SIZE * 2, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_NORMAL)->MoveWindow(BUTTON_FIXED_SIZE * 3, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_FAST)->MoveWindow(BUTTON_FIXED_SIZE * 4, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_FORWARD)->MoveWindow(BUTTON_FIXED_SIZE * 5, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_BACK)->MoveWindow(BUTTON_FIXED_SIZE * 6, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_PREV_SEG)->MoveWindow(BUTTON_FIXED_SIZE * 7, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_NEXT_SEG)->MoveWindow(BUTTON_FIXED_SIZE * 8, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_RECORD_BEGIN)->MoveWindow(BUTTON_FIXED_SIZE * 9, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_RECORD_END)->MoveWindow(BUTTON_FIXED_SIZE * 10, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_START_CUT)->MoveWindow(BUTTON_FIXED_SIZE * 11, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_END_CUT)->MoveWindow(BUTTON_FIXED_SIZE * 12, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_1FRAME)->MoveWindow(BUTTON_FIXED_SIZE * 13, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_2FRAME)->MoveWindow(BUTTON_FIXED_SIZE *14, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_SNAPSHOT)->MoveWindow(BUTTON_FIXED_SIZE * 15, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_AVI)->MoveWindow(BUTTON_FIXED_SIZE * 16, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_ADJUST)->MoveWindow(BUTTON_FIXED_SIZE * 17, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_FULLSCREEN)->MoveWindow(BUTTON_FIXED_SIZE * 18, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_REPAIR_FILE)->MoveWindow(BUTTON_FIXED_SIZE * 19, cy - BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE, BUTTON_FIXED_SIZE);
		GetDlgItem(IDC_STATIC_NUMBER_ONE)->MoveWindow(BUTTON_FIXED_SIZE * 20,cy - BUTTON_FIXED_SIZE,15,15);
		GetDlgItem(IDC_STATIC_NUMBER_TWO)->MoveWindow(BUTTON_FIXED_SIZE * 20 + 15,cy - BUTTON_FIXED_SIZE,15,15);
		GetDlgItem(IDC_STATIC_NUMBER_THREE)->MoveWindow(BUTTON_FIXED_SIZE * 20 + 30,cy - BUTTON_FIXED_SIZE,15,15);
		GetDlgItem(IDC_STATUS_STATIC)->MoveWindow(BUTTON_FIXED_SIZE * 20, cy - BUTTON_FIXED_SIZE / 2, 100, BUTTON_FIXED_SIZE / 2);
		Init_VideoOut(m_pView1Dlg, m_pView2Dlg);
		CNewProjDlg::g_bViewChange = true;
		OnPaint();
	}
	// TODO: Add your message handler code here
}

void CToolBarDlg::OnAvi() 
{
	// TODO: Add your control notification handler code here
	if(m_1stFilePath == "") return;
	OnStop();
	m_AVIConverterDlg.m_bPaint = true;
	m_AVIConverterDlg.m_1stFilePath = m_1stFilePath;
	m_AVIConverterDlg.m_2ndFilePath = m_2ndFilePath;
	memset(&m_AVIConverterDlg.m_systemTime, 0, sizeof(SYSTEMTIME));
	m_AVIConverterDlg.m_systemTime.wYear = m_DateTime.year;
	m_AVIConverterDlg.m_systemTime.wMonth = m_DateTime.month;
	m_AVIConverterDlg.m_systemTime.wDay = m_DateTime.day;
	m_AVIConverterDlg.m_systemTime.wHour = (WORD)m_DateTime.hour;
	m_AVIConverterDlg.m_systemTime.wMinute = (WORD)m_DateTime.minute;
	m_AVIConverterDlg.m_systemTime.wSecond = (WORD)m_DateTime.second;
	m_AVIConverterDlg.m_dwDuration = m_duration;
	if(m_2ndFilePath == "") m_AVIConverterDlg.GetFiles(&m_1stFile, NULL);
	else m_AVIConverterDlg.GetFiles(&m_1stFile, &m_2ndFile);
	m_AVIConverterDlg.ShowWindow(SW_SHOW);
}

void CToolBarDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	//wchar_t chBuf[100];
	//memset(chBuf, 0, sizeof(chBuf));
	//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[266], chBuf);
	if (point.x > 610)
	{
		m_RecordPlayerAboutDlg.SetWindowText(CNewProjDlg::m_wLanguageRes[266]);
		m_RecordPlayerAboutDlg.GetClientRect(&rt);
		m_RecordPlayerAboutDlg.MoveWindow(CNewProjDlg::m_AboutRt);
		m_RecordPlayerAboutDlg.ShowWindow(SW_SHOW);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CToolBarDlg::OnVolumePicture() 
{
	// TODO: Add your control notification handler code here
	HINSTANCE hInst;
	HBITMAP hBmp;
	if(m_bVolume == FALSE)
	{

		hInst = AfxFindResourceHandle(MAKEINTRESOURCE(20162), RT_BITMAP);
		hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_VOL_ON), IMAGE_BITMAP, 0, 0, 0);
		m_picture_volume.SetBitmap(hBmp);
		g_bSoundPlay=false;
	}else{
		hInst = AfxFindResourceHandle(MAKEINTRESOURCE(20163), RT_BITMAP);
		hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_VOL_OFF), IMAGE_BITMAP, 0, 0, 0);
		m_picture_volume.SetBitmap(hBmp);
		g_bSoundPlay=true;
	}
	m_bVolume =! m_bVolume;
}

void CToolBarDlg::OnAdjust()
{
	// TODO: Add your control notification handler code here
	m_AdjustDlg.ShowWindow(SW_SHOW);
}

void CToolBarDlg::Init_PlayOpertion(CString path1, CString path2)
{	
	DWORD dwFirstDTS, dwLastDTS;
	if(m_1stFile.m_hFile != INVALID_HANDLE_VALUE) m_1stFile.Close();
	if(m_2ndFile.m_hFile != INVALID_HANDLE_VALUE) m_2ndFile.Close();
	m_1stFilePath = path1;
	m_2ndFilePath = path2;
	m_1stFile.Open(m_1stFilePath, CFile::modeRead);
	m_1stSpliter.Init_Voice();
	m_1stSpliter.Initialization(&m_1stFile);
	m_dwFirstDTS = m_1stSpliter.GetFirstDTS();
	dwLastDTS = m_1stSpliter.GetLastDTS();
	m_1stSpliter.m_dwLastDTS = dwLastDTS;
	m_duration = dwLastDTS - m_dwFirstDTS;
	m_Slider_Seek.SetRange(0, dwLastDTS - m_dwFirstDTS, TRUE);
	m_pTimeLineDlg->InitTimeLine();
	if(path1!="" && path2 != "") 
	{
		m_2ndFile.Open(m_2ndFilePath, CFile::modeRead);
		m_2ndSpliter.Initialization(&m_2ndFile);
		CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	}
}
FILE* fp = fopen("c:\\temp1.bin", "wb");
void CToolBarDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int  a = m_Slider_Seek.GetPos();
	int b= m_Slider_Seek.GetRangeMax();

	if(CTimeLineDlg::m_bTimeSeekFlag == true)
	{
		OnStop();
		KillTimer(TIMER_PLAYER1);
		KillTimer(TIMER_PLAYER2);
		KillTimer(TIMER_SLIDER);
		KillTimer(TIMER_ALARM);
		CTimeLineDlg::m_bTimeSeekFlag = false;
		TimeSeek(CTimeLineDlg::m_fTimeSeekPos, CNewProjDlg::m_nPathArraysPtr);
	}
	if(CNewProjDlg::g_bOpenFlag == true) 
	{
		OnStop();
		KillTimer(TIMER_PLAYER1);
		KillTimer(TIMER_PLAYER2);
		KillTimer(TIMER_SLIDER);
		KillTimer(TIMER_ALARM);
		g_bSoundPlay = false;
		if(m_1stFile.m_hFile != INVALID_HANDLE_VALUE) m_1stFile.Close();
		if(m_2ndFile.m_hFile != INVALID_HANDLE_VALUE) m_2ndFile.Close();
		g_bSoundPlay = false;
		CNewProjDlg::g_bOpenFlag = false;
		return;
	}
	if(CNewProjDlg::g_bResetFlag == true)
	{
		OnStop();
		CNewProjDlg::g_bResetFlag = false;
		g_bSoundPlay = false;
		m_1stFilePath = CNewProjDlg::m_FirstPathArrays[CNewProjDlg::m_nPathArraysPtr];
		m_2ndFilePath = CNewProjDlg::m_SecondPathArrays[CNewProjDlg::m_nPathArraysPtr];
		Init_PlayOpertion(m_1stFilePath, m_2ndFilePath);
		OnPlayBtnClick();
	}
	if(CTimeLineDlg::m_bTimeFlag == false) 
	{
		OnStop();
		return;
	}

	if(nIDEvent == TIMER_PLAYER1)
	{
		//m_nPlayPtr ++;
		//if(m_nPlayPtr == 30) m_nPlayPtr = 0;
		char buf[100];
		CString str;
		int nSecs = (m_1stSpliter.m_1stOutDatas.dwDTS_V - m_dwFirstDTS) / 1000;
		str.Format(_T("%02d:%02d  Current Time %04d-%02d-%02d %02d:%02d:%02d"), nSecs / 60, 
			nSecs % 60, CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wYear,
			CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wMonth, CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wDay,
			CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wHour + nSecs / 3600, 
			(CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wMinute + (nSecs % 3600) / 60) % 60, 
			(CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wSecond + nSecs % 60) % 60);
		CNewProjDlg::m_fSeekPos = (float)m_Slider_Seek.GetPos() / m_Slider_Seek.GetRangeMax();
		m_Slider_Seek.strTipText = str;
		if(!g_bSoundPlay) waveOutSetVolume(g_hWaveOut, (DWORD)((float)m_slider_volume.GetPos() / 100 * 0xFFFF));
		else waveOutSetVolume(g_hWaveOut, (DWORD)((float)m_slider_volume.GetPos() / 100 * 0x0000));
		if(m_1stSpliter.GetDatas1() == -1) 
			g_ErrRead = true;
		
		m_1stOutDatas[m_nOutDatasPtr]= m_1stSpliter.m_1stOutDatas;
		if(CNewProjDlg::g_config_Value_ST.nWndCounts == 2)
		{
			if(m_2ndSpliter.GetDatas2() == -1) 
				g_ErrRead = true;
		}
		//fwrite(m_2ndSpliter.m_2ndOutDatas.buf_A, 800, 1, fp);
		m_2ndOutDatas[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas;
		if(CNewProjDlg::g_config_Value_ST.nSelWndID == 1)
			m_pipe->m_audioTrack[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas.buf_A;
		else
			m_pipe->m_audioTrack[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas.buf_A;
		m_nPlayPtr ++;
		if(m_nPlayPtr == 10) m_nPlayPtr = 0;
		m_nOutDatasPtr ++;
		if(m_nOutDatasPtr == 10) m_nOutDatasPtr = 0;
		if(m_AdjustDlg.m_bSel1 != TRUE)
		{
			Video1Out(m_1stOutDatas[m_nPlayPtr].buf_V, m_nScaleMode);
 		}
		else
		{
			double hue,saturation,rTemp,gTemp,bTemp,hTemp,lTemp,sTemp, light;
			
			hue			= (double)CNewProjDlg::g_adjust_Value_ST.nHue ;
			saturation	= (double)CNewProjDlg::g_adjust_Value_ST.nSaturation ;
			light		= (double)CNewProjDlg::g_adjust_Value_ST.nLight ;
			
			for (int i = 0; i < 704*480*3 ; i += 3)
			{
				
				rTemp	= m_1stSpliter.m_1stOutDatas.buf_V[i];
				gTemp	= m_1stSpliter.m_1stOutDatas.buf_V[i + 1];
				bTemp	= m_1stSpliter.m_1stOutDatas.buf_V[i + 2];
				RGBQUAD rgbValue = {(BYTE)bTemp, (BYTE)gTemp, (BYTE)rTemp,0 };
				RGBQUAD hslValue = RGBtoHSL(rgbValue);
				hTemp	= hslValue.rgbRed - hue;
				sTemp	= hslValue.rgbGreen - saturation;
				lTemp	= hslValue.rgbBlue - light;
				hslValue.rgbRed = (BYTE)hTemp;
				hslValue.rgbGreen = (BYTE)sTemp;
				hslValue.rgbBlue = (BYTE)lTemp;
				hslValue.rgbReserved = 0;
				rgbValue = HSLtoRGB(hslValue);
				m_1stSpliter.m_1stOutDatas.buf_V[i]     = rgbValue.rgbRed;
				m_1stSpliter.m_1stOutDatas.buf_V[i + 1] = rgbValue.rgbGreen;
				m_1stSpliter.m_1stOutDatas.buf_V[i + 2] = rgbValue.rgbBlue;

			}

			Video1Out(m_1stOutDatas[m_nPlayPtr].buf_V, m_nScaleMode);
		}
		m_1stSpliter.m_1stOutDatas.bBuf = false;
		if (m_1stSpliter.m_1stOutDatas.nB_Size == 60 && m_bDrawSeek ==FALSE)
		{			
			m_pGpsDlg->ShowGPS_Pos(&m_1stSpliter.m_1stOutDatas.mB_Data);
			m_pInfoListDlg->m_pBinData[binDataNum].mBinData =   m_1stSpliter.m_1stOutDatas.mB_Data;
			m_pInfoListDlg->m_pBinData[binDataNum].nBinSize =   m_1stSpliter.m_1stOutDatas.nB_Size;
			m_pInfoListDlg->m_pBinData[binDataNum].dwCurPos =   m_Slider_Seek.GetPos();
			binDataNum ++;
			m_pInfoListDlg->m_BinDataCount = binDataNum;
			m_pInfoListDlg->m_dura = m_duration;
			 
			m_pInfoListDlg->DrawGraph(&m_1stSpliter.m_1stOutDatas.mB_Data,m_Slider_Seek.GetPos(),CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr],m_1stSpliter.m_1stOutDatas.nB_Size, m_1stSpliter.m_file->GetLength(),m_bDrawSeek );
			if(m_prevPos != m_Slider_Seek.GetPos())
			{
				m_prevPos = m_Slider_Seek.GetPos();
				m_i ++;
// 				m_pInfoListDlg->m_pos[m_i].iPos = m_prevPos;
// 				m_pInfoListDlg->m_pos[m_i].count = m_i;
			}						
		}
		else if(m_1stSpliter.m_1stOutDatas.nB_Size != 0 && m_bDrawSeek == TRUE)
		{
			
			m_pInfoListDlg->DrawGraph(&m_1stSpliter.m_1stOutDatas.mB_Data,m_Slider_Seek.GetPos(),CNewProjDlg::m_dwDurations[CNewProjDlg::m_nPathArraysPtr],m_1stSpliter.m_1stOutDatas.nB_Size ,m_1stSpliter.m_file->GetLength(),m_bDrawSeek);

			m_bDrawSeek = FALSE;	
		}
	}

	if(nIDEvent == TIMER_PLAYER2)
	{
		//m_2ndSpliter.GetDatas2();
		if(m_AdjustDlg.m_bSel2 != TRUE)
		{
			Video2Out(m_2ndOutDatas[m_nPlayPtr].buf_V, m_nScaleMode);
		}
		else
		{
			double hue,saturation,rTemp,gTemp,bTemp,hTemp,lTemp,sTemp, light;
			
			hue			= (double)CNewProjDlg::g_adjust_Value_ST.nHue ;
			saturation	= (double)CNewProjDlg::g_adjust_Value_ST.nSaturation;
			light		= (double)CNewProjDlg::g_adjust_Value_ST.nLight ;
			
			for (int i = 0; i < 704*480*3 ; i += 3)
			{
				
				rTemp	= m_2ndSpliter.m_2ndOutDatas.buf_V[i];
				gTemp	= m_2ndSpliter.m_2ndOutDatas.buf_V[i + 1];
				bTemp	= m_2ndSpliter.m_2ndOutDatas.buf_V[i + 2];
				RGBQUAD rgbValue = {(BYTE)bTemp, (BYTE)gTemp,(BYTE)rTemp,0 };
				RGBQUAD hslValue = RGBtoHSL(rgbValue);
				hTemp	= hslValue.rgbRed - hue;
				sTemp	= hslValue.rgbGreen - saturation;
				lTemp	= hslValue.rgbBlue - light;
				hslValue.rgbRed = (BYTE)hTemp;
				hslValue.rgbGreen = (BYTE)sTemp;
				hslValue.rgbBlue = (BYTE)lTemp;
				hslValue.rgbReserved = 0;
				rgbValue = HSLtoRGB(hslValue);
				m_2ndSpliter.m_2ndOutDatas.buf_V[i]     = rgbValue.rgbRed;
				m_2ndSpliter.m_2ndOutDatas.buf_V[i + 1] = rgbValue.rgbGreen;
				m_2ndSpliter.m_2ndOutDatas.buf_V[i + 2] = rgbValue.rgbBlue;
				
			}
			
			Video2Out(m_2ndOutDatas[m_nPlayPtr].buf_V, m_nScaleMode);
		}		
		m_2ndSpliter.m_2ndOutDatas.bBuf = false;
	}

	if(nIDEvent == TIMER_SLIDER)
	{
		m_Slider_Seek.SetPos((int)((float)m_1stSpliter.m_file->GetPosition() / m_1stSpliter.m_file->GetLength() * m_Slider_Seek.GetRangeMax()));
	}

	if(nIDEvent == TIMER_PLAYER1)
	{
		if((m_Slider_Seek.GetPos() > m_Slider_Seek.GetRangeMax() - 200) || m_1stSpliter.m_1stEndFlag == true || m_2ndSpliter.m_2ndEndFlag == true)
		{
			//g_bSoundPlay = false;
			//CTimeLineDlg::m_bTimeFlag = false;
			KillTimer(TIMER_PLAYER1);
			KillTimer(TIMER_PLAYER2);
			KillTimer(TIMER_SLIDER);
			g_bSoundPlay = false;
			CTimeLineDlg::m_bTimeFlag = false;
			m_Slider_Seek.SetPos(m_1stSpliter.m_1stOutDatas.dwDTS_V - m_1stSpliter.m_dwFirstDTS);
			m_1stSpliter.m_file->Close();
			m_2ndSpliter.m_file->Close();
			OnStop();
			if(CNewProjDlg::m_nPathArraysPtr < CNewProjDlg::m_nPathCounts - 1)
			{
				
				CNewProjDlg::m_nPathArraysPtr++;
				CNewProjDlg::g_bResetFlag = false;
				m_1stFilePath = CNewProjDlg::m_FirstPathArrays[CNewProjDlg::m_nPathArraysPtr];
				m_2ndFilePath = CNewProjDlg::m_SecondPathArrays[CNewProjDlg::m_nPathArraysPtr];
				m_pInfoListDlg->m_fileList.SetItemText(CNewProjDlg::m_nPathArraysPtr, 7, _T("Played"));
				for(int i = 0 ; i < CNewProjDlg::m_nPathCounts; i++)
					m_pInfoListDlg->m_fileList.UnSelectItem(i);
				m_pInfoListDlg->m_fileList.SelectItem(CNewProjDlg::m_nPathArraysPtr, true);
				Init_PlayOpertion(m_1stFilePath, m_2ndFilePath);
				OnPlayBtnClick();
			}

			m_pGpsDlg->ResetMapInfo(false);

		}
	}

	
	if (nIDEvent == TIMER_ALARM)
	{	
			if((m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x01) == 0x01) 
//				m_number1.SetTextColor(RGB(255, 0, 0));	
				m_number1.SetBitmap(IDB_NUMBER_ONE_RED);
			else if((m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x01) == 0x00)
//				m_number1.SetTextColor(RGB(255, 255, 255));
				m_number1.SetBitmap(IDB_NUMBER_ONE_WHITE);

			if((m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x02) == 0x02) 
//				m_number2.SetTextColor(RGB(255, 0, 0));
				m_number2.SetBitmap(IDB_NUMBER_TWO_RED);
			else if((m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x02) == 0x00)
//				m_number2.SetTextColor(RGB(255, 255, 255));
				m_number2.SetBitmap(IDB_NUMBER_TWO_WHITE);

			if((m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x04) == 0x04) 
//				m_number3.SetTextColor(RGB(255, 0, 0));
				m_number3.SetBitmap(IDB_NUMBER_THREE_RED);
			else if((m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x04) == 0x00)
//				m_number3.SetTextColor(RGB(255, 255, 255));
				m_number3.SetBitmap(IDB_NUMBER_THREE_WHITE);
	}
	

	if (nIDEvent == TIMER_RECORD_ALARM) 
	{
		OnAlarmSwitch(m_bAlarmOn);
		m_bAlarmOn = !m_bAlarmOn;
	}
	CDialog::OnTimer(nIDEvent);
}

void CToolBarDlg::SetViewDlgs(CView1Dlg* pDlg1, CView2Dlg* pDlg2,CInfoListDlg* pDlg3,CGPSDlg*  pDlg4,CTimeLineDlg* pDlg5,CToolBarDlg* pDlg6)
{
	m_pView1Dlg = pDlg1;
	m_pView2Dlg = pDlg2;
	m_pInfoListDlg= pDlg3;
	m_pGpsDlg =  pDlg4;
	m_pTimeLineDlg  = pDlg5;
	m_pToolBarDlg = pDlg6;
}

void CToolBarDlg::Init_VideoOut(CView1Dlg* pDlg1, CView2Dlg* pDlg2)
{
	pDisPos1 = pDlg1->GetDlgItem(IDC_STATIC_VIEW1);
	pDisPos2 = pDlg2->GetDlgItem(IDC_STATIC_VIEW2);
	bmi1.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);\
	bmi1.bmiHeader.biCompression     = BI_RGB;\
	bmi1.bmiHeader.biWidth           = VIDEOW;\
	bmi1.bmiHeader.biHeight          = VIDEOH;\
	bmi1.bmiHeader.biPlanes          = 1;\
	bmi1.bmiHeader.biBitCount        = 24;\
	bmi1.bmiHeader.biSizeImage       = VIDEOW*VIDEOH*bmi1.bmiHeader.biBitCount/8;\
	bmi1.bmiHeader.biXPelsPerMeter   = 0;\
	bmi1.bmiHeader.biYPelsPerMeter   = 0;\
	bmi1.bmiHeader.biClrUsed         = 0;\
	bmi1.bmiHeader.biClrImportant    = 0;

	pDisPos1->GetClientRect(&rect1);\
	hDC1 = pDisPos1->GetDC()->GetSafeHdc();\
	hMemDC1 = CreateCompatibleDC(hDC1);\
	hBmp1 = CreateCompatibleBitmap(hDC1, VIDEOW, VIDEOH);\
	hOldBmp1 = (HBITMAP)SelectObject(hMemDC1,  (HGDIOBJ)hBmp1);

	bmi2.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);\
	bmi2.bmiHeader.biCompression     = BI_RGB;\
	bmi2.bmiHeader.biWidth           = VIDEOW;\
	bmi2.bmiHeader.biHeight          = VIDEOH;\
	bmi2.bmiHeader.biPlanes          = 1;\
	bmi2.bmiHeader.biBitCount        = 24;\
	bmi2.bmiHeader.biSizeImage       = VIDEOW*VIDEOH*bmi2.bmiHeader.biBitCount/8;\
	bmi2.bmiHeader.biXPelsPerMeter   = 0;\
	bmi2.bmiHeader.biYPelsPerMeter   = 0;\
	bmi2.bmiHeader.biClrUsed         = 0;\
	bmi2.bmiHeader.biClrImportant    = 0;

	pDisPos2->GetClientRect(&rect2);\
	hDC2= pDisPos2->GetDC()->GetSafeHdc();\
	hMemDC2 = CreateCompatibleDC(hDC2);\
	hBmp2 = CreateCompatibleBitmap(hDC2, VIDEOW, VIDEOH);\
	hOldBmp2 = (HBITMAP)SelectObject(hMemDC2,  (HGDIOBJ)hBmp2);
	WndsBK_Paint(&CNewProjDlg::m_View1Dlg, &CNewProjDlg::m_View2Dlg, CNewProjDlg::g_config_Value_ST.nSelWndID);
	CNewProjDlg::g_bViewChange = false;
}
char* vBuf1;
void CToolBarDlg::Video1Out(char* buf, int nScaleMode)
{
//	if(CNewProjDlg::g_bViewChange == true) 
//		Init_VideoOut(&CNewProjDlg::m_View1Dlg, &CNewProjDlg:: m_View2Dlg);
		
	CRect rt;
	int nX, nY;
	int nWidth, nHeight;
	AfxGetMainWnd()->GetClientRect(&rt);
	hOldBmp1 = (HBITMAP)SelectObject(hMemDC1,  (HGDIOBJ)hBmp1);				
	if(CToolBarDlg::g_bView1Turn_ST == true)
	{
		vBuf1 = new char[VIDEOW * VIDEOH * 3];
		for(int i = 0; i < VIDEOH; i++)
			for(int j = 0; j < VIDEOW; j++)
				memcpy((char*)(vBuf1 + i * VIDEOW * 3 + j * 3), (char*)(buf + i * VIDEOW * 3 + (VIDEOW - j - 1) * 3), 3);
		SetDIBitsToDevice(hMemDC1, 0, 0, VIDEOW, VIDEOH, 0, 0, 0, VIDEOH, vBuf1, &bmi1, 0);
	}else SetDIBitsToDevice(hMemDC1, 0, 0, VIDEOW, VIDEOH, 0, 0, 0, VIDEOH, buf, &bmi1, 0);
	SetStretchBltMode(hDC1,HALFTONE);
//View Scale Auto
	if(CNewProjDlg::g_config_Value_ST.nViewScaleID == 0)
	{
		if(rect1.Height() * 16 >rect1.Width() * 9) {nWidth = rect1.Width(); nHeight = (int)((float)rect1.Width() * 9 / 16); nX = 0; nY = rect1.Height() - (rect1.Height() - nHeight) / 2;}
		else {nWidth = (int)((float)rect1.Height() * 16 / 9); nHeight = rect1.Height(); nX = (rect1.Width() - nWidth) / 2; nY = rect1.Height();}
	}else if(CNewProjDlg::g_config_Value_ST.nViewScaleID == 1){
		if(rect1.Height() * 4 >rect1.Width() * 3) {nWidth = rect1.Width(); nHeight = (int)((float)rect1.Width() * 3 / 4); nX = 0; nY = rect1.Height() - (rect1.Height() - nHeight) / 2;}
		else {nWidth = (int)((float)rect1.Height() * 4 / 3); nHeight = rect1.Height(); nX = (rect1.Width() - nWidth) / 2; nY = rect1.Height();}
	}else if(CNewProjDlg::g_config_Value_ST.nViewScaleID == 2){
		nWidth = rect1.Width(); nX = 0;
		nHeight = rect1.Height(); nY = rect1.Height();
	}
	int nXWidth = 20;
	if(CToolBarDlg::g_bView1Updown_ST == true) {nY -= nHeight; nHeight *= -1;}
	StretchBlt(hDC1, nX, nY, nWidth, - nHeight, hMemDC1, 0, 0, VIDEOW, VIDEOH, SRCCOPY);
	CDC* pDC = pDisPos1->GetDC();
	pDC->SetBkMode(RGB(50, 50, 50));
	if(CNewProjDlg::g_config_Value_ST.bShowChanNum == true)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->ExtTextOut(nX + nWidth - 80, nY - 20, ETO_OPAQUE, CRect(0, 0, 0, 0), "ch1", NULL);
	}
	//ReleaseDC(pDC);
	pDC->SetTextColor(RGB(255, 0, 0));
	if(CToolBarDlg::m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x01 == 0x01)
	{
		pDC->ExtTextOut(nX + 10, nY - nHeight + 30, ETO_OPAQUE, CRect(0, 0, 0, 0), "Light", NULL);
	}
	if((CToolBarDlg::m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x02) == 0x02)
	{
		pDC->ExtTextOut(nX + 10, nY - nHeight + 45, ETO_OPAQUE, CRect(0, 0, 0, 0), "Man", NULL);
	}
	if((CToolBarDlg::m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x04) == 0x04)
	{
		pDC->ExtTextOut(nX + 10, nY - nHeight + 60, ETO_OPAQUE, CRect(0, 0, 0, 0), "2.4", NULL);
	}
	ReleaseDC(pDC);
 	SelectObject(hMemDC1, (HGDIOBJ)hOldBmp1);
}
char* vBuf2;
void CToolBarDlg::Video2Out(char* buf, int nScaleMode)
{
	if(CNewProjDlg::g_bViewChange == true) 
		Init_VideoOut(&CNewProjDlg::m_View1Dlg, &CNewProjDlg:: m_View2Dlg);
	CRect rt;
	int nX, nY;
	int nWidth, nHeight;
	AfxGetMainWnd()->GetClientRect(&rt);
 	hOldBmp2 = (HBITMAP)SelectObject(hMemDC2,  (HGDIOBJ)hBmp2);
	if(CToolBarDlg::g_bView2Turn_ST == true)
	{
		vBuf2 = new char[VIDEOW * VIDEOH * 3];
		for(int i = 0; i < VIDEOH; i++)
			for(int j = 0; j < VIDEOW; j++)
				memcpy((char*)(vBuf2 + i * VIDEOW * 3 + j * 3), (char*)(buf + i * VIDEOW * 3 + (VIDEOW - j - 1) * 3), 3);
		SetDIBitsToDevice(hMemDC2, 0, 0, VIDEOW, VIDEOH, 0, 0, 0, VIDEOH, vBuf2, &bmi2, 0);
	}else SetDIBitsToDevice(hMemDC2, 0, 0, VIDEOW, VIDEOH, 0, 0, 0, VIDEOH, buf, &bmi2, 0);
	SetStretchBltMode(hDC2,HALFTONE);
//View Scale Auto
	if(CNewProjDlg::g_config_Value_ST.nViewScaleID == 0)
	{
		if(rect2.Height() * 16 >rect2.Width() * 9) {nWidth = rect2.Width(); nHeight = (int)((float)rect2.Width() * 9 / 16); nX = 0; nY = rect2.Height() - (rect2.Height() - nHeight) / 2;}
		else {nWidth = (int)((float)rect2.Height() * 16 / 9); nHeight = rect2.Height(); nX = (rect2.Width() - nWidth) / 2; nY = rect2.Height();}
	}else if(CNewProjDlg::g_config_Value_ST.nViewScaleID == 1){
		if(rect2.Height() * 4 >rect2.Width() * 3) {nWidth = rect2.Width(); nHeight = (int)((float)rect2.Width() * 3 / 4); nX = 0; nY = rect2.Height() - (rect2.Height() - nHeight) / 2;}
		else {nWidth = (int)((float)rect2.Height() * 4 / 3); nHeight = rect2.Height(); nX = (rect2.Width() - nWidth) / 2; nY = rect2.Height();}
	}else if(CNewProjDlg::g_config_Value_ST.nViewScaleID == 2){
		nWidth = rect2.Width(); nX = 0;
		nHeight = rect2.Height(); nY = rect2.Height();
	}
	if(CToolBarDlg::g_bView2Updown_ST == true) {nY -= nHeight; nHeight *= -1;}
	StretchBlt(hDC2, nX, nY, nWidth, - nHeight, hMemDC2, 0, 0, VIDEOW, VIDEOH, SRCCOPY);
	CDC* pDC = pDisPos2->GetDC();
	pDC->SetBkMode(RGB(50, 50, 50));
	if(CNewProjDlg::g_config_Value_ST.bShowChanNum == true)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->ExtTextOut(nX + nWidth - 80, nY - 20, ETO_OPAQUE, CRect(0, 0, 0, 0), "ch2", NULL);
	}
	//ReleaseDC(pDC);
	pDC->SetTextColor(RGB(255, 0, 0));
	if(CToolBarDlg::m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x01 == 0x01)
	{
		pDC->ExtTextOut(nX + 10, nY - nHeight + 30, ETO_OPAQUE, CRect(0, 0, 0, 0), "Light", NULL);
	}
	if((CToolBarDlg::m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x02) == 0x02)
	{
		pDC->ExtTextOut(nX + 10, nY - nHeight + 45, ETO_OPAQUE, CRect(0, 0, 0, 0), "Man", NULL);
	}
	if((CToolBarDlg::m_1stSpliter.m_1stOutDatas.mB_Data.alarmInfo & 0x04) == 0x04)
	{
		pDC->ExtTextOut(nX + 10, nY - nHeight + 60, ETO_OPAQUE, CRect(0, 0, 0, 0), "2.4", NULL);
	}
	ReleaseDC(pDC);
 	SelectObject(hMemDC2, (HGDIOBJ)hOldBmp2);
}
void CToolBarDlg::OnCustomdrawSliderSeek(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CToolBarDlg::OnReleasedcaptureSliderSeek(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//if(g_hWaveOut) waveOutReset(g_hWaveOut);
	if(CNewProjDlg::g_config_Value_ST.nWndCounts == 0) return;
	if(m_nThreadCounts == 2) 
	{
		KillTimer(TIMER_PLAYER1);
		KillTimer(TIMER_PLAYER2);
	}else KillTimer(TIMER_PLAYER1);
	KillTimer(TIMER_SLIDER);
	KillTimer(TIMER_ALARM);
	m_pipe->StopPlayingFromFile();
	Sleep(50);
	//OnStop();
	
	m_Slider_Seek.SetPos(m_Slider_Seek.m_nRealCurPos);
	int nSeekPos = m_Slider_Seek.m_nRealCurPos;
	if(m_bSignal == TRUE) m_bSeekSignal = TRUE;
	// TODO: Add your control notification handler code here
	if(m_nThreadCounts == 0) return;
	
	if(m_nThreadCounts == 2)
	{
		KillTimer(TIMER_PLAYER1);
		KillTimer(TIMER_PLAYER2);
		m_bPlayer1 = false;
		m_bPlayer2 = false;
		if(m_1stSpliter.m_file->m_hFile != INVALID_HANDLE_VALUE /*0xFFFFFFFF*/)
		m_1stSpliter.m_file->Close();
		if(m_2ndSpliter.m_file->m_hFile != INVALID_HANDLE_VALUE /*0xFFFFFFFF*/)
		m_2ndSpliter.m_file->Close();
		m_nThreadCounts = 0;
//gsi
		for (int i = 0; i < binDataNum ; i++)
		{
			memset(&m_pInfoListDlg->m_pBinData[i],0,sizeof(TempBinData));
		}
		binDataNum = 0;
	}else if(m_nThreadCounts == 1)
	{
		m_bPlayer1 = false;
		m_nThreadCounts = 0;
		KillTimer(TIMER_PLAYER1);
		if(m_1stSpliter.m_file->m_hFile != INVALID_HANDLE_VALUE /*0xFFFFFFFF*/)
		m_1stSpliter.m_file->Close();
	}
	float fPos = (float)nSeekPos;
	if(m_2ndFilePath != "") m_nThreadCounts = 2;
	else m_nThreadCounts = 1;
	m_nOutDatasPtr = 0;
	if(m_nThreadCounts == 2)
	{
		m_1stFile.Open(CNewProjDlg::m_FirstPathArrays[CNewProjDlg::m_nPathArraysPtr], CFile::modeRead);
		m_1stSpliter.Initialization(&m_1stFile);
		m_1stSpliter.m_file->SeekToBegin();
		m_2ndFile.Open(CNewProjDlg::m_SecondPathArrays[CNewProjDlg::m_nPathArraysPtr], CFile::modeRead);
		m_2ndSpliter.Initialization(&m_2ndFile);
		m_2ndSpliter.m_file->SeekToBegin();
		SetTimer(TIMER_SLIDER, 1000, NULL);
		if(m_1stSpliter.Seek(fPos / m_Slider_Seek.GetRangeMax()) == false) return;
		if(m_2ndSpliter.Seek(fPos / m_Slider_Seek.GetRangeMax(), m_1stSpliter.m_FrameHeader.dwDTS) == false) return;
		bool bExit = false;
		if(m_2ndSpliter.m_dw2ndSeekTime > m_1stSpliter.m_dw1stSeekTime)
		{
			m_2ndSpliter.m_file->Seek(ASF_FILE_HEAD_SIZE + PACKET_SIZE, CFile::begin);//-100 * PACKET_SIZE, CFile::current);
			//m_2ndSpliter.m_file->Seek(-300 * PACKET_SIZE, CFile::current);//-100 * PACKET_SIZE, CFile::current);
			do{
				m_2ndSpliter.m_file->Seek(PACKET_SIZE * 2, CFile::current);
				bExit = m_2ndSpliter.ReSeek();
				
			}while(!bExit);
		}
		if(m_2ndSpliter.m_dw2ndSeekTime < m_1stSpliter.m_dw1stSeekTime)
		{
			m_2ndSpliter.m_file->Seek(ASF_FILE_HEAD_SIZE + PACKET_SIZE, CFile::begin);
			do{
				m_2ndSpliter.m_file->Seek(PACKET_SIZE * 2, CFile::current);
				bExit = m_2ndSpliter.ReSeek();
			}while(!bExit);
		}
		m_bPlayer1 = true;
		SetTimer(TIMER_PLAYER1, m_nSpeed / 10, NULL);
		m_bPlayer2 = true;
		SetTimer(TIMER_PLAYER2, m_nSpeed / 10, NULL);
		for(int i = 0 ; i < 5; i++)
		{
			m_1stSpliter.GetDatas1();
			m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
			m_2ndSpliter.GetDatas2();
			m_2ndOutDatas[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas;
			if(CNewProjDlg::g_config_Value_ST.nSelWndID == 1)
				m_pipe->m_audioTrack[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas.buf_A;
			else
				m_pipe->m_audioTrack[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas.buf_A;
			m_nOutDatasPtr ++;
		}
	}else if(m_nThreadCounts == 1)
	{
		SetTimer(TIMER_SLIDER, 1000, NULL);
		m_1stFile.Open(CNewProjDlg::m_FirstPathArrays[CNewProjDlg::m_nPathArraysPtr], CFile::modeRead);
		m_1stSpliter.Initialization(&m_1stFile);
		m_1stSpliter.m_file->SeekToBegin();
		m_1stSpliter.Seek(fPos / m_Slider_Seek.GetRangeMax());
		m_bPlayer1 = true;
		for(int i = 0; i < 5; i++)
		{
			m_1stSpliter.GetDatas1();
			m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
			m_pipe->m_audioTrack[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas.buf_A;
			m_nOutDatasPtr ++;
		}
		SetTimer(TIMER_PLAYER1, m_nSpeed / 10, NULL);
	}
	SetTimer(TIMER_ALARM, 100, NULL);
	m_pipe->m_nBufPtr = 0;
	m_pipe->StartPlayingFromFile();
	//AfxBeginThread(SoundPlayThread, this);
	*pResult = 0;
}

void CToolBarDlg::OnOutofmemorySliderSeek(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

BOOL CToolBarDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));
	return CDialog::OnEraseBkgnd(pDC);
}

void CToolBarDlg::OnRecordBegin() 
{
	// TODO: Add your control notification handler code here
	if(!m_bPlayer1) return;
	m_dw1stStartPos = m_1stSpliter.m_file->GetPosition();
	if(m_2ndFilePath != "") m_dw2ndStartPos = m_2ndSpliter.m_file->GetPosition();
	m_nStartPos = m_Slider_Seek.GetPos();
	GetDlgItem(IDC_PLAY)->EnableWindow(false);
	GetDlgItem(IDC_STOP)->EnableWindow(false);
	GetDlgItem(IDC_FAST)->EnableWindow(false);
	GetDlgItem(IDC_NORMAL)->EnableWindow(false);
	GetDlgItem(IDC_SLOW)->EnableWindow(false);
	GetDlgItem(IDC_PREV_SEG)->EnableWindow(false);
	GetDlgItem(IDC_NEXT_SEG)->EnableWindow(false);
	GetDlgItem(IDC_FORWARD)->EnableWindow(false);
	GetDlgItem(IDC_BACK)->EnableWindow(false);
	GetDlgItem(IDC_RECORD_BEGIN)->EnableWindow(false);
	GetDlgItem(IDC_RECORD_END)->EnableWindow(true);
	GetDlgItem(IDC_START_CUT)->EnableWindow(false);
	GetDlgItem(IDC_END_CUT)->EnableWindow(false);
	GetDlgItem(IDC_1FRAME)->EnableWindow(false);
	GetDlgItem(IDC_2FRAME)->EnableWindow(false);
	GetDlgItem(IDC_SNAPSHOT)->EnableWindow(false);
	GetDlgItem(IDC_AVI)->EnableWindow(false);
	GetDlgItem(IDC_ADJUST)->EnableWindow(false);
	GetDlgItem(IDC_FULLSCREEN)->EnableWindow(false);
	GetDlgItem(IDC_REPAIR_FILE)->EnableWindow(false);

	// start record alarm
	m_bAlarmOn = true;
	KillTimer(TIMER_ALARM);
	SetTimer(TIMER_RECORD_ALARM, 400, NULL);
}
int k = 0;
UINT AviRecordThreadProc(LPVOID pParam);
void CToolBarDlg::OnRecordEnd() 
{
	// TODO: Add your control notification handler code here

	// stop record alarm
	m_number1.SetBitmap(IDB_NUMBER_ONE_WHITE);
	m_number2.SetBitmap(IDB_NUMBER_TWO_WHITE);
	m_number3.SetBitmap(IDB_NUMBER_THREE_WHITE);
	KillTimer(TIMER_RECORD_ALARM);
	SetTimer(TIMER_ALARM,100,NULL);
	m_bAlarmOn = false;


	m_nEndPos = m_Slider_Seek.GetPos();
	memcpy(&m_TimeRangeDlg.m_StartDateTime, &CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], sizeof(SYSTEMTIME));
	DWORD dwTime = CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wHour * 3600 + CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wMinute * 60 + CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wSecond + m_nStartPos / 1000;
	m_TimeRangeDlg.m_StartDateTime.wHour = (WORD)(dwTime / 3600) % 24;
	m_TimeRangeDlg.m_StartDateTime.wMinute = (WORD)(dwTime / 60) % 60;
	m_TimeRangeDlg.m_StartDateTime.wSecond = (WORD)(dwTime % 60);
	memcpy(&m_TimeRangeDlg.m_EndDateTime, &CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], sizeof(SYSTEMTIME));
	dwTime = CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wHour * 3600 + CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wMinute * 60 + CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wSecond + m_nEndPos / 1000;
	m_TimeRangeDlg.m_EndDateTime.wHour = (WORD)(dwTime / 3600) % 24;
	m_TimeRangeDlg.m_EndDateTime.wMinute = (WORD)(dwTime / 60) % 60;
	m_TimeRangeDlg.m_EndDateTime.wSecond = (WORD)(dwTime % 60);
	m_TimeRangeDlg.m_bRecordCurFlag = true;
	OnStop();
	m_TimeRangeDlg.UpdateWnd();
	m_TimeRangeDlg.m_1stFile = &m_1stFile;
	if(CNewProjDlg::g_config_Value_ST.nWndCounts == 2)
		m_TimeRangeDlg.m_2ndFile = &m_2ndFile;
	m_TimeRangeDlg.m_flt_1stStartPos = (float)m_nStartPos / m_Slider_Seek.GetRangeMax();
	m_TimeRangeDlg.m_flt_1stEndPos = (float)m_nEndPos / m_Slider_Seek.GetRangeMax();
	m_TimeRangeDlg.m_flt_2ndStartPos = (float) m_nStartPos / m_Slider_Seek.GetRangeMax();
	m_TimeRangeDlg.m_flt_2ndEndPos = (float)m_nEndPos / m_Slider_Seek.GetRangeMax();
	m_TimeRangeDlg.ShowWindow(SW_SHOW);
	
#if 0
		KillTimer(TIMER_PLAYER1);
		KillTimer(TIMER_PLAYER2);
		KillTimer(TIMER_SLIDER);
		OnStop();
		wchar_t		szFilter[]= _T("AVI Files(*.avi)|*.avi|");
		CFileDialog	dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);
		if(dlg.DoModal() ==IDOK)
		{
			m_AviPathStr = dlg.GetPathName();
			m_AviPathStr += _T(".avi");
			m_ProgressDlg.m_AVIFilePath = m_AviPathStr;
			if(CNewProjDlg::g_config_Value_ST.nWndCounts == 2)
				m_ProgressDlg.Init_AVI_Convert(&m_1stFile, &m_2ndFile, (float)m_nStartPos / m_Slider_Seek.GetRangeMax(), (float)m_nEndPos / m_Slider_Seek.GetRangeMax(),(float) m_nStartPos / m_Slider_Seek.GetRangeMax() , (float)m_nEndPos / m_Slider_Seek.GetRangeMax());
			else if(CNewProjDlg::g_config_Value_ST.nWndCounts == 1 && CNewProjDlg::g_config_Value_ST.nSelWndID == 1)
				m_ProgressDlg.Init_AVI_Convert(&m_1stFile, NULL, (float)m_nStartPos / m_Slider_Seek.GetRangeMax(), (float)m_nEndPos / m_Slider_Seek.GetRangeMax(), 0, 0);
			else if(CNewProjDlg::g_config_Value_ST.nWndCounts == 1 && CNewProjDlg::g_config_Value_ST.nSelWndID == 2)
				m_ProgressDlg.Init_AVI_Convert(NULL, &m_2ndFile, 0, 0, (float)m_nStartPos / m_Slider_Seek.GetRangeMax(), (float)m_nEndPos / m_Slider_Seek.GetRangeMax());
		}
		m_ProgressDlg.ShowWindow(SW_SHOW | SW_SHOWNORMAL);
#endif
	GetDlgItem(IDC_PLAY)->EnableWindow(true);
	SetDlgItemText(IDC_PLAY, _T("Play"));
	HBITMAP hBmp;
	HINSTANCE hInst;
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_PLAY), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	m_btn_Play.SetBitmap(hBmp);
	GetDlgItem(IDC_STOP)->EnableWindow(true);
	GetDlgItem(IDC_FAST)->EnableWindow(true);
	GetDlgItem(IDC_NORMAL)->EnableWindow(true);
	GetDlgItem(IDC_SLOW)->EnableWindow(true);
	GetDlgItem(IDC_PREV_SEG)->EnableWindow(false);
	GetDlgItem(IDC_NEXT_SEG)->EnableWindow(false);
	GetDlgItem(IDC_FORWARD)->EnableWindow(false);
	GetDlgItem(IDC_BACK)->EnableWindow(false);
	GetDlgItem(IDC_RECORD_BEGIN)->EnableWindow(true);
	GetDlgItem(IDC_RECORD_END)->EnableWindow(false);
	GetDlgItem(IDC_START_CUT)->EnableWindow(true);
	GetDlgItem(IDC_END_CUT)->EnableWindow(false);
	GetDlgItem(IDC_1FRAME)->EnableWindow(true);
	GetDlgItem(IDC_2FRAME)->EnableWindow(true);
	GetDlgItem(IDC_SNAPSHOT)->EnableWindow(true);
	GetDlgItem(IDC_AVI)->EnableWindow(true);
	GetDlgItem(IDC_ADJUST)->EnableWindow(true);
	GetDlgItem(IDC_FULLSCREEN)->EnableWindow(true);
	GetDlgItem(IDC_REPAIR_FILE)->EnableWindow(true);
	OnStop();
}

void CToolBarDlg::OnFast() 
{
	// TODO: Add your control notification handler code here
	if(m_nSpeed == 125 || m_bPlayer1 == false) return;
	m_nSpeed /= 2;
	switch(m_nSpeed)
	{
	case 500:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed * 2"));
		break;
	case 250:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed * 4"));
		break;
	case 125:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed * 8"));
		break;
	case 1000:
		SetDlgItemText(IDC_STATUS_STATIC, _T(" Normal "));
		break;
	case 2000:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed / 2"));
		break;
	case 4000:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed / 4"));
		break;
	case 8000:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed / 8"));
		break;
	}
	SetTimer(TIMER_PLAYER1, m_nSpeed /10, NULL);
	if(m_bPlayer2 == true) SetTimer(TIMER_PLAYER2, m_nSpeed /10, NULL);
}

void CToolBarDlg::OnSlow() 
{
	// TODO: Add your control notification handler code here
	if(m_nSpeed == 8000 || m_bPlayer1 == false) return;
	m_nSpeed *= 2;
	switch(m_nSpeed)
	{
	case 500:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed * 2"));
		break;
	case 250:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed * 4"));
		break;
	case 125:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed * 8"));
		break;
	case 1000:
		SetDlgItemText(IDC_STATUS_STATIC, _T(" Normal "));
		break;
	case 2000:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed / 2"));
		break;
	case 4000:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed / 4"));
		break;
	case 8000:
		SetDlgItemText(IDC_STATUS_STATIC, _T("Speed / 8"));
		break;
	}
	SetTimer(TIMER_PLAYER1, m_nSpeed /10, NULL);
	if(m_bPlayer2 == true) SetTimer(TIMER_PLAYER2, m_nSpeed /10, NULL);
}

void CToolBarDlg::OnNormal() 
{
	// TODO: Add your control notification handler code here
	if(m_bPlayer1 == false) return;
	m_nSpeed = 1000;
	SetDlgItemText(IDC_STATUS_STATIC, _T(" Normal "));
	SetTimer(TIMER_PLAYER1, m_nSpeed / 10, NULL);
	if(m_bPlayer2 == true) SetTimer(TIMER_PLAYER2, m_nSpeed / 10, NULL);
}

void CToolBarDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	CNewProjDlg::g_nPlayState = STATE_STOP;
	m_Slider_Seek.SetPos(0);
	m_nSpeed = 1000;
	m_i = 0;
	m_1stSpliter.m_1stEndFlag = false;
	m_1stSpliter.m_2ndEndFlag = false;
	m_2ndSpliter.m_2ndEndFlag = false;
	m_2ndSpliter.m_1stEndFlag = false;
	g_bSoundPlay = false;
	if(g_hWaveOut) 
		waveOutReset(g_hWaveOut);
	CTimeLineDlg::m_bTimeFlag = false;
	m_pipe->StopPlayingFromFile();
	//SetDlgItemText(IDC_STATUS_STATIC, " Normal ");
	HBITMAP hBmp;
	CBitmap bmp;
	HINSTANCE hInst;
	SetDlgItemText(IDC_PLAY, _T("Play"));
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(20170), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_PLAY), IMAGE_BITMAP, 0, 0, 0);
	m_btn_Play.SetBitmap(hBmp);
	g_bSoundPlay = false;
	CTimeLineDlg::m_bTimeFlag = false;
	KillTimer(TIMER_PLAYER1);
	KillTimer(TIMER_PLAYER2);
	KillTimer(TIMER_SLIDER);
	//Init_PlayOpertion(m_1stFilePath, m_2ndFilePath);
	//Init_VideoOut(m_pView1Dlg, m_pView2Dlg);
	GetDlgItem(IDC_STOP)->EnableWindow(false);
//gsi
	for (int i = 0; i < binDataNum ; i++)
	{
		memset(&m_pInfoListDlg->m_pBinData[i],0,sizeof(TempBinData));
	}
		binDataNum = 0;

	m_pGpsDlg->ResetMapInfo(true);
}

void CToolBarDlg::OnStartCut() 
{
	// TODO: Add your control notification handler code here
	if(!m_bPlayer1) return;
	m_dw1stStartPos = m_1stSpliter.m_file->GetPosition();
	if(m_2ndFilePath != "") m_dw2ndStartPos = m_2ndSpliter.m_file->GetPosition();
	m_nStartPos = m_Slider_Seek.GetPos();
	GetDlgItem(IDC_PLAY)->EnableWindow(false);
	GetDlgItem(IDC_STOP)->EnableWindow(false);
	GetDlgItem(IDC_FAST)->EnableWindow(false);
	GetDlgItem(IDC_NORMAL)->EnableWindow(false);
	GetDlgItem(IDC_SLOW)->EnableWindow(false);
	GetDlgItem(IDC_PREV_SEG)->EnableWindow(false);
	GetDlgItem(IDC_NEXT_SEG)->EnableWindow(false);
	GetDlgItem(IDC_FORWARD)->EnableWindow(false);
	GetDlgItem(IDC_BACK)->EnableWindow(false);
	GetDlgItem(IDC_RECORD_BEGIN)->EnableWindow(false);
	GetDlgItem(IDC_RECORD_END)->EnableWindow(false);
	GetDlgItem(IDC_START_CUT)->EnableWindow(false);
	GetDlgItem(IDC_END_CUT)->EnableWindow(true);
	GetDlgItem(IDC_1FRAME)->EnableWindow(false);
	GetDlgItem(IDC_2FRAME)->EnableWindow(false);
	GetDlgItem(IDC_SNAPSHOT)->EnableWindow(false);
	GetDlgItem(IDC_AVI)->EnableWindow(false);
	GetDlgItem(IDC_ADJUST)->EnableWindow(false);
	GetDlgItem(IDC_FULLSCREEN)->EnableWindow(false);
	GetDlgItem(IDC_REPAIR_FILE)->EnableWindow(false);
}

void CToolBarDlg::OnEndCut() 
{
	// TODO: Add your control notification handler code here
	CString path;
	char	buf[8];
	m_nEndPos = m_Slider_Seek.GetPos();
	memcpy(&m_TimeRangeDlg.m_StartDateTime, &CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], sizeof(SYSTEMTIME));
	m_TimeRangeDlg.m_StartDateTime.wHour = CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wHour + (m_nStartPos / 3600000);
	m_TimeRangeDlg.m_StartDateTime.wHour %= 24;
	m_TimeRangeDlg.m_StartDateTime.wMinute = CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wMinute + (m_nStartPos / 60000);
	m_TimeRangeDlg.m_StartDateTime.wMinute %= 60;
	m_TimeRangeDlg.m_StartDateTime.wSecond = CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wSecond + (m_nStartPos / 1000);
	m_TimeRangeDlg.m_StartDateTime.wSecond %= 60;
	m_TimeRangeDlg.m_dw1stStartPos = m_dw1stStartPos;
	m_TimeRangeDlg.m_dw2ndStartPos = m_dw2ndStartPos;
	memcpy(&m_TimeRangeDlg.m_EndDateTime, &CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr], sizeof(SYSTEMTIME));
	m_TimeRangeDlg.m_EndDateTime.wHour = CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wHour + (m_nEndPos / 3600000);
	m_TimeRangeDlg.m_EndDateTime.wHour %= 24;
	m_TimeRangeDlg.m_EndDateTime.wMinute = CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wMinute + (m_nEndPos / 60000);
	m_TimeRangeDlg.m_EndDateTime.wMinute %= 60;
	m_TimeRangeDlg.m_EndDateTime.wSecond = CNewProjDlg::m_DateTime[CNewProjDlg::m_nPathArraysPtr].wSecond + (m_nEndPos / 1000);
	m_TimeRangeDlg.m_EndDateTime.wSecond %= 60;
	path = "H";
	itoa(m_TimeRangeDlg.m_StartDateTime.wYear, buf, 10);
	path += buf;
	itoa(m_TimeRangeDlg.m_StartDateTime.wMonth, buf, 10);
	if(m_TimeRangeDlg.m_StartDateTime.wMonth < 10) path +="0";
	path += buf;
	itoa(m_TimeRangeDlg.m_StartDateTime.wDay, buf, 10);
	if(m_TimeRangeDlg.m_StartDateTime.wDay < 10) path += "0";
	path += buf;
	path += "-";
	itoa(m_TimeRangeDlg.m_StartDateTime.wHour, buf, 10);
	if(m_TimeRangeDlg.m_StartDateTime.wHour < 10) path += "0";
	path += buf;
	itoa(m_TimeRangeDlg.m_StartDateTime.wMinute, buf, 10);
	if(m_TimeRangeDlg.m_StartDateTime.wMinute < 10) path += "0";
	path += buf;
	itoa(m_TimeRangeDlg.m_StartDateTime.wSecond, buf, 10);
	if(m_TimeRangeDlg.m_StartDateTime.wSecond < 10) path += "0";
	path += buf;
	OnStop();
	m_TimeRangeDlg.m_bRecordCurFlag = false;
	m_TimeRangeDlg.UpdateWnd();
	m_TimeRangeDlg.m_dw1stStartPos = m_dw1stStartPos;
	m_TimeRangeDlg.m_dw1stEndPos = m_1stSpliter.m_file->GetPosition();
	m_TimeRangeDlg.m_1stFilePath = path + "N1A1A0.264";
	m_TimeRangeDlg.m_1stFile = &m_1stFile;
	if(m_2ndFilePath != "")	
	{
		m_TimeRangeDlg.m_2ndFilePath = path + "N2A2A0.264";
		m_TimeRangeDlg.m_2ndFile = &m_2ndFile;
		m_TimeRangeDlg.m_dw2ndStartPos = m_dw2ndStartPos;
		m_TimeRangeDlg.m_dw2ndEndPos = m_2ndSpliter.m_file->GetPosition();
	}
	m_TimeRangeDlg.ShowWindow(SW_SHOW);

#if 0
	m_ProgressDlg.m_1stDestFilePath = path + "N1A1A0.264";
	if(m_2ndFilePath != "")	m_ProgressDlg.m_2ndDestFilePath = path + "N2A2A0.264";
	if(CNewProjDlg::g_config_Value_ST.nWndCounts == 2)
		m_ProgressDlg.Init_Progress(&m_1stFile, &m_2ndFile, m_dw1stStartPos, m_1stSpliter.m_file->GetPosition(),
			m_dw2ndStartPos, m_2ndSpliter.m_file->GetPosition());
	else if(CNewProjDlg::g_config_Value_ST.nWndCounts == 1)
		m_ProgressDlg.Init_Progress(m_1stSpliter.m_file, NULL, m_dw1stStartPos, m_1stSpliter.m_file->GetPosition(), 0, 0);
	if(m_TimeRangeDlg.DoModal() == IDOK)
	{
		OnStop();	
	}
	MessageBox(_T("Success Spliting!"), NULL, 0);
#endif
	GetDlgItem(IDC_PLAY)->EnableWindow(true);
	SetDlgItemText(IDC_PLAY, _T("Play"));
	HBITMAP hBmp;
	HINSTANCE hInst;
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_PLAY), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	m_btn_Play.SetBitmap(hBmp);
	
	GetDlgItem(IDC_STOP)->EnableWindow(true);
	GetDlgItem(IDC_FAST)->EnableWindow(true);
	GetDlgItem(IDC_NORMAL)->EnableWindow(true);
	GetDlgItem(IDC_SLOW)->EnableWindow(true);
	GetDlgItem(IDC_PREV_SEG)->EnableWindow(false);
	GetDlgItem(IDC_NEXT_SEG)->EnableWindow(false);
	GetDlgItem(IDC_FORWARD)->EnableWindow(false);
	GetDlgItem(IDC_BACK)->EnableWindow(false);
	GetDlgItem(IDC_RECORD_BEGIN)->EnableWindow(true);
	GetDlgItem(IDC_RECORD_END)->EnableWindow(false);
	GetDlgItem(IDC_START_CUT)->EnableWindow(true);
	GetDlgItem(IDC_END_CUT)->EnableWindow(false);
	GetDlgItem(IDC_1FRAME)->EnableWindow(true);
	GetDlgItem(IDC_2FRAME)->EnableWindow(true);
	GetDlgItem(IDC_SNAPSHOT)->EnableWindow(true);
	GetDlgItem(IDC_AVI)->EnableWindow(true);
	GetDlgItem(IDC_ADJUST)->EnableWindow(true);
	GetDlgItem(IDC_FULLSCREEN)->EnableWindow(true);
	GetDlgItem(IDC_REPAIR_FILE)->EnableWindow(true);
	OnStop();
}

void CToolBarDlg::OnPaint() 
{
	//m_Slider_Seek.SetPos(m_Slider_Seek.GetPos());
	if(CNewProjDlg::g_nPlayState == STATE_PAUSE)
	{
		CNewProjDlg::g_bViewChange = true;
		Video2Out(m_2ndOutDatas[m_nPlayPtr].buf_V, m_nScaleMode);
		Video1Out(m_1stOutDatas[m_nPlayPtr].buf_V, m_nScaleMode);
	}
	m_slider_volume.SetPos(m_slider_volume.GetPos());
	CPaintDC dc(this); // device context for painting
	CDC* pDC = GetDC();
	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 56, 66));
	GetDlgItem(IDC_SLIDER_SEEK)->MoveWindow(10, 0, (rt.Width() - 170), rt.Height() - BUTTON_FIXED_SIZE - 1);
	HBITMAP hBmp;
	HINSTANCE hInst;
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_PLAY), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	m_btn_Play.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_STOP), IMAGE_BITMAP, 0, 0, 0);
	m_btn_Stop.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_SLOW), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_Slow.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_NORMAL), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_Normal.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_FAST), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_Fast.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_FORWARD), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_Forward.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_BACK), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_Back.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_PREVSEG), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_PrevSeg.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_NEXTSEG), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_NextSeg.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_RECSTART), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_RecordBegin.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_RECEND), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_RecordEnd.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_CUTSTART), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_StartCut.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_CUTEND), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_EndCut.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_ONEWND), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_1Frame.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_TWOWND), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_2Frame.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_SNAPSHOT), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_SnapShot.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_AVI), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_AVI.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_ADJUST), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_Adjust.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_FULLSCR), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_FullScreen.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_REPAIR), IMAGE_BITMAP, 0, 0, 0);
	m_Btn_RepairFile.SetBitmap(hBmp);
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR), RT_BITMAP);
	hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP_VOL_ON), IMAGE_BITMAP, 0, 0, 0);
	m_picture_volume.SetBitmap(hBmp);

	hBmp = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_BITMAP_BKCOLOR));
	m_Status_Static.SetBitmap(hBmp);
	m_Status_Static.SetTextColor( RGB(255,255,255) );
	m_Status_Static.SetFont("Arial", 15, FW_NORMAL);	
	m_Status_Static.SetBkColor( RGB(47, 56, 66), 0/*RGB(47, 56, 66)*/, CxStatic::Normal );

	m_number1.SetBkColor(RGB(47, 56, 66));
	m_number2.SetBkColor(RGB(47, 56, 66));
	m_number3.SetBkColor(RGB(47, 56, 66));
	m_number1.SetBitmap(IDB_NUMBER_ONE_WHITE);
	m_number2.SetBitmap(IDB_NUMBER_TWO_WHITE);
	m_number3.SetBitmap(IDB_NUMBER_THREE_WHITE);

	ReleaseDC(pDC);
}

void CToolBarDlg::ReSet()
{
	if(m_1stFilePath == "") return;
	OnStop();
	if(CNewProjDlg::g_config_Value_ST.bAutoPlay == true) OnPlayBtnClick();
}

void CToolBarDlg::OnSnapshot() 
{
	// TODO: Add your control notification handler code here
	CRect rt;
	GetDlgItem(IDC_SNAPSHOT)->GetWindowRect(&rt);
	CMenu menu ;
	menu.LoadMenu(IDR_MENU_SNAPSHOT);
	ASSERT(menu.GetSubMenu(0));
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, rt.left, rt.bottom, this, 0);
}
RGBQUAD CToolBarDlg::RGBtoHSL(RGBQUAD lRGBColor)
{
	BYTE R,G,B;					/* input RGB values */
	BYTE H,L,S;					/* output HSL values */
	BYTE cMax,cMin;				/* max and min RGB values */
	WORD Rdelta,Gdelta,Bdelta;	/* intermediate value: % of spread from max*/
	
	R = lRGBColor.rgbRed;	/* get R, G, and B out of DWORD */
	G = lRGBColor.rgbGreen;
	B = lRGBColor.rgbBlue;
	
	cMax = max( max(R,G), B);	/* calculate lightness */
	cMin = min( min(R,G), B);
	L = (((cMax+cMin)*HSLMAX)+RGBMAX)/(2*RGBMAX);
	
	if (cMax==cMin){			/* r=g=b --> achromatic case */
		S = 0;					/* saturation */
		H = UNDEFINED;			/* hue */
	} else {					/* chromatic case */
		if (L <= (HSLMAX/2))	/* saturation */
			S = (((cMax-cMin)*HSLMAX)+((cMax+cMin)/2))/(cMax+cMin);
		else
			S = (((cMax-cMin)*HSLMAX)+((2*RGBMAX-cMax-cMin)/2))/(2*RGBMAX-cMax-cMin);
		/* hue */
		Rdelta = ( ((cMax-R)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
		Gdelta = ( ((cMax-G)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
		Bdelta = ( ((cMax-B)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
		
		if (R == cMax)
			H = Bdelta - Gdelta;
		else if (G == cMax)
			H = (HSLMAX/3) + Rdelta - Bdelta;
		else /* B == cMax */
			H = ((2*HSLMAX)/3) + Gdelta - Rdelta;
		
		if (H < 0) H += HSLMAX;
		if (H > HSLMAX) H -= HSLMAX;
	}
	RGBQUAD hsl={L,S,H,0};
	return hsl;
}
RGBQUAD CToolBarDlg::HSLtoRGB(RGBQUAD lHSLColor)
{ 
	WORD hue,lum,sat;
	BYTE R,G,B;					/* RGB component values */
	WORD Magic1,Magic2;			/* calculated magic numbers (really!) */
	
	hue = lHSLColor.rgbRed;	/* get H, S, and L out of DWORD */
	sat = lHSLColor.rgbGreen;
	lum = lHSLColor.rgbBlue;
	
	if (sat == 0) {            /* achromatic case */
		R=G=B=(lum*RGBMAX)/HSLMAX;
	} else {                   /* chromatic case */
		/* set up magic numbers */
		if (lum <= (HSLMAX/2))
			Magic2 = (lum*(HSLMAX + sat) + (HSLMAX/2))/HSLMAX;
		else
			Magic2 = lum + sat - ((lum*sat) + (HSLMAX/2))/HSLMAX;
		Magic1 = 2*lum-Magic2;	
		/* get RGB, change units from HSLMAX to RGBMAX */
		R = (HueToRGB(Magic1,Magic2,(WORD)(hue+(HSLMAX/3)))*RGBMAX+(HSLMAX/2))/HSLMAX; 
		G = (HueToRGB(Magic1,Magic2,hue)*RGBMAX + (HSLMAX/2)) / HSLMAX;
		B = (HueToRGB(Magic1,Magic2,(WORD)(hue-(HSLMAX/3)))*RGBMAX+(HSLMAX/2))/HSLMAX; 
	}
	RGBQUAD rgb={B,G,R,0};
	return rgb;
}
WORD CToolBarDlg::HueToRGB(WORD n1,WORD n2,WORD hue)
{ 
	/* range check: note values passed add/subtract thirds of range */
	if (hue < 0) hue += HSLMAX;
	if (hue > HSLMAX) hue -= HSLMAX;
	
	/* return r,g, or b value from this tridrant */
	if (hue < (HSLMAX/6))
		return ( n1 + (((n2-n1)*hue+(HSLMAX/12))/(HSLMAX/6)) );
	if (hue < (HSLMAX/2))
		return ( n2 );
	if (hue < ((HSLMAX*2)/3))
		return ( n1 + (((n2-n1)*(((HSLMAX*2)/3)-hue)+(HSLMAX/12))/(HSLMAX/6))); 
	else
		return ( n1 );
} 

void CToolBarDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CToolBarDlg::OnTwoWindow() 
{
	// TODO: Add your control notification handler code here
	if(CNewProjDlg::m_SecondPathArrays[CNewProjDlg::m_nPathArraysPtr] == "") return;
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	m_pView1Dlg->MoveWindow(0,0,rt.Width() * 1 / 3, rt.Height() * 3  / 5);
	m_pView2Dlg->MoveWindow(rt.Width() * 1 / 3, 0 ,rt.Width() * 1/ 3, rt.Height() * 3 / 5);
	m_pView1Dlg->ShowWindow(SW_SHOW);
	m_pView2Dlg->ShowWindow(SW_SHOW);
	CNewProjDlg::g_config_Value_ST.nWndCounts = 2;
	CNewProjDlg::g_bViewChange = true;
}

void CToolBarDlg::OnOneWindow() 
{
	// TODO: Add your control notification handler code here
	m_pView2Dlg->ShowWindow(SW_HIDE);	
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	if(CNewProjDlg::g_config_Value_ST.nSelWndID == 1)
	{
		m_pView1Dlg->MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3 / 5);
		m_pView1Dlg->ShowWindow(SW_SHOW);
		m_pView2Dlg->ShowWindow(SW_HIDE);
	}else{
		m_pView2Dlg->MoveWindow(0, 0, rt.Width() * 2 / 3, rt.Height() * 3 / 5);
		m_pView2Dlg->ShowWindow(SW_SHOW);
		m_pView1Dlg->ShowWindow(SW_HIDE);
	}
	CNewProjDlg::g_config_Value_ST.nWndCounts = 1;
	CNewProjDlg::g_bViewChange = true;
}

void CToolBarDlg::OnFullscreen() 
{
	// TODO: Add your control notification handler code here
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	m_bFullScreen = TRUE;
	if(CNewProjDlg::g_config_Value_ST.nWndCounts == 1)
	{
		m_pView1Dlg->MoveWindow(0,0,rt.Width(),rt.Height());
		m_pTimeLineDlg->ShowWindow(SW_HIDE);
		m_pGpsDlg->ShowWindow(SW_HIDE);
		m_pToolBarDlg->ShowWindow(SW_HIDE);
		m_pInfoListDlg->ShowWindow(SW_HIDE);
		m_pView1Dlg->ShowWindow(SW_SHOW);
	}
	else if(CNewProjDlg::g_config_Value_ST.nWndCounts == 2)
	{
		m_pView1Dlg->MoveWindow(0, 0, rt.Width() / 2, rt.Height());
		m_pView2Dlg->MoveWindow( rt.Width() / 2, 0 ,rt.Width() / 2, rt.Height());
		m_pTimeLineDlg->ShowWindow(SW_HIDE);
		m_pGpsDlg->ShowWindow(SW_HIDE);
		m_pToolBarDlg->ShowWindow(SW_HIDE);
		m_pInfoListDlg->ShowWindow(SW_HIDE);
		m_pView1Dlg->ShowWindow(SW_SHOW);
		m_pView2Dlg->ShowWindow(SW_SHOW);
	}
	CNewProjDlg::g_bViewChange = true;
}


void CToolBarDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_pView1Dlg->ShowWindow(SW_HIDE);
	CRect rt;
	AfxGetMainWnd()->GetClientRect(&rt);
	m_pView1Dlg->MoveWindow(0,0,rt.Width() / 3,rt.Height() * 3 / 5);
	m_pView2Dlg->MoveWindow(rt.Width() / 3,0,rt.Width() / 3,rt.Height() * 3 / 5);

	m_pView1Dlg->ShowWindow(SW_SHOW);
	m_pView2Dlg->ShowWindow(SW_SHOW);
	m_pInfoListDlg->ShowWindow(SW_SHOW);
	m_pGpsDlg->ShowWindow(SW_SHOW);
	m_pTimeLineDlg->ShowWindow(SW_SHOW);
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CToolBarDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnNcMouseMove(nFlags,point);
}

void CToolBarDlg::OnSnapshotOneCamera() 
{
	// TODO: Add your command handler code here
	m_bSnapshotChoice = 1;
	wchar_t		szFilter[] = _T("JPG Files(*.jpg)|*.jpg|");
	CString savePath = " ";
	CFileDialog	dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);
	if(dlg.DoModal() ==IDOK)
	{
		savePath = dlg.GetPathName();
		savePath += ".jpg";
		CFile file(savePath, CFile::modeCreate | CFile::modeWrite);
		file.SeekToBegin();		
		WriteBmpHeader(m_pbmpFileHeader,m_pbmpInfoHeader,m_bSnapshotChoice);
		file.Write(m_pbmpFileHeader, sizeof(BITMAPFILEHEADER));
		file.Write(m_pbmpInfoHeader, sizeof(BITMAPINFOHEADER));
		for(int i  = 0; i < VIDEOH; i++)
		{
			memcpy((void *)(ss_buf+ i * VIDEOW * 3), (char*)(m_1stSpliter.m_1stOutDatas.buf_V + (VIDEOH - i - 1) * VIDEOW * 3), VIDEOW * 3); 
		}
		file.Write(ss_buf, 0xf7800);
		file.Close();		
	}
}
void CToolBarDlg::WriteBmpHeader(BITMAPFILEHEADER* pOutBmpFileHeader ,BITMAPINFOHEADER* pOutBmpInfoHeader,int nChoice)
{
	BITMAPFILEHEADER bmpFileHead;
	BITMAPINFOHEADER bmpInfoHead;
	if(nChoice == 1)
	{
		bmpFileHead.bfType = BFT_BITMAP;
		bmpFileHead.bfSize = 0x000f7836;
		bmpFileHead.bfReserved1 = bmpFileHead.bfReserved2 = 0;
		bmpFileHead.bfOffBits = 0x36;
		memcpy(pOutBmpFileHeader,&bmpFileHead,sizeof(BITMAPFILEHEADER));
		
		bmpInfoHead.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfoHead.biWidth = 704;         // fill in width from parameter
		bmpInfoHead.biHeight = 480;       // fill in height from parameter
		bmpInfoHead.biPlanes = 1;              // must be 1
		bmpInfoHead.biBitCount = 24;    // from parameter
		bmpInfoHead.biCompression = BI_RGB;    
		bmpInfoHead.biSizeImage = 704 * 480 * 3;
		bmpInfoHead.biXPelsPerMeter = 0;
		bmpInfoHead.biYPelsPerMeter = 0;
		bmpInfoHead.biClrUsed = 0;
		bmpInfoHead.biClrImportant = 0;
		memcpy(pOutBmpInfoHeader,&bmpInfoHead,sizeof(BITMAPINFOHEADER));
	}
	if(nChoice == 2)
	{
		bmpFileHead.bfType = BFT_BITMAP;
		bmpFileHead.bfSize = 0x001ef036;
		bmpFileHead.bfReserved1 = bmpFileHead.bfReserved2 = 0;
		bmpFileHead.bfOffBits = 0x36;
		memcpy(pOutBmpFileHeader,&bmpFileHead,sizeof(BITMAPFILEHEADER));
		
		bmpInfoHead.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfoHead.biWidth = 1408;         // fill in width from parameter
		bmpInfoHead.biHeight = 480;       // fill in height from parameter
		bmpInfoHead.biPlanes = 1;              // must be 1
		bmpInfoHead.biBitCount = 24;    // from parameter
		bmpInfoHead.biCompression = BI_RGB;    
		bmpInfoHead.biSizeImage = 1408 * 480 * 3;
		bmpInfoHead.biXPelsPerMeter = 0;
		bmpInfoHead.biYPelsPerMeter = 0;
		bmpInfoHead.biClrUsed = 0;
		bmpInfoHead.biClrImportant = 0;
		memcpy(pOutBmpInfoHeader,&bmpInfoHead,sizeof(BITMAPINFOHEADER));		
	}
}

void CToolBarDlg::OnSnapshotWholeCamera() 
{
	// TODO: Add your command handler code here
	m_bSnapshotChoice = 2;
	
	wchar_t		szFilter[]= _T("JPG Files(*.jpg)|*.jpg|");
	CString savePath = " ";
	CFileDialog	dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);
	if(dlg.DoModal() ==IDOK)
	{
		savePath = dlg.GetPathName();
		savePath += ".jpg";
		CFile file(savePath, CFile::modeCreate | CFile::modeWrite);
		file.SeekToBegin();		
		WriteBmpHeader(m_pbmpFileHeader,m_pbmpInfoHeader,m_bSnapshotChoice);
		file.Write(m_pbmpFileHeader, sizeof(BITMAPFILEHEADER));
		file.Write(m_pbmpInfoHeader, sizeof(BITMAPINFOHEADER));
		for(int i  = 0; i < VIDEOH; i++)
		{
			memcpy((void *)(ss_buf2+ i * VIDEOW * 6), (char*)(m_1stSpliter.m_1stOutDatas.buf_V + (VIDEOH - i - 1) * VIDEOW * 3), VIDEOW * 3); 
			memcpy((void *)(ss_buf2+ i * VIDEOW * 6 + VIDEOW * 3), (char*)(m_2ndSpliter.m_2ndOutDatas.buf_V + (VIDEOH - i - 1) * VIDEOW * 3), VIDEOW * 3); 
		}
		file.Write(ss_buf2, 0x001ef000);
		file.Close();		
	}
}

void CToolBarDlg::WriteAviHeader(int nWidth, int nHeight)
{
	m_pAviFile->SeekToBegin();
	m_AVIConverterDlg.InitHeaderDatas(nWidth,nHeight);

	m_pAviFile->Write(&m_AVIConverterDlg.m_Riff, sizeof(RIFF));
	m_pAviFile->Write(&m_AVIConverterDlg.m_AviList, sizeof(AVI_LIST));
	m_pAviFile->Write(&m_AVIConverterDlg.m_InfoList, sizeof(INFOLIST));
	m_pAviFile->Write(&m_AVIConverterDlg.m_Junk, sizeof(JUNK));
	m_pAviFile->Write(&m_AVIConverterDlg.m_TrackFrame, sizeof(TRACK_FRAME));
}

UINT AviRecordThreadProc(LPVOID pParam)
{
	CAVIConverterDlg* pDlg = (CAVIConverterDlg*)&CToolBarDlg::m_AVIConverterDlg;
	if(CToolBarDlg::m_2ndFilePath != "")
	{
		pDlg->InitHeaderDatas(1408, 480);
		pDlg->m_nChannel = 2;
	}else{
		
		pDlg->InitHeaderDatas(704, 480);
		pDlg->m_nChannel = 1;
	}
	DWORD dwVideoID = 0x62643030;
	DWORD dwAudioID = 0x62773130;
	CToolBarDlg::m_1stSpliter.OpenFile("", 0);
	if(CToolBarDlg::m_2ndFilePath != "")CToolBarDlg::m_2ndSpliter.OpenFile("", 0);
	DWORD dwFilePos = CToolBarDlg::m_pAviFile->GetPosition();
	if(pDlg->m_nChannel == 3) pDlg->m_lpFrame.PushData = new BYTE[0x12FC00];
	else pDlg->m_lpFrame.PushData = new BYTE[0x97E00];
	pDlg->m_lpFrame.PopData = new BYTE[0x20000]; 
	DWORD dwGlobal[4];
	memset(dwGlobal, 0, 16);
	dwGlobal[0] = 0x10202;
	int nCounts = 0;
	DWORD dwRetVal = (*pDlg->m_Func_XvidGlobal)(0, 0, dwGlobal, 0);
	dwRetVal = (*pDlg->m_Func_XvidCreate)(0, 0, &pDlg->m_lpCreate, 0);
	dwRetVal = 1;
	int		nFrmaeID = 0;
	CToolBarDlg::m_1stSpliter.GetDatas1();
	CToolBarDlg::m_1stSpliter.GetDatas1();
	CToolBarDlg::m_2ndSpliter.GetDatas2();
	CToolBarDlg::m_2ndSpliter.GetDatas2();

		do{
		//	pDlg->m_progress.SetPos((int)((float)CToolBarDlg::m_1stSpliter.m_file->GetPosition() / pDlg->m_1stSpliter.m_file->GetLength() * 100));
			CToolBarDlg::m_1stSpliter.GetDatas1();
			CToolBarDlg::m_2ndSpliter.GetDatas2();
			if(CToolBarDlg::m_1stSpliter.m_pY == 0) continue;
			if(pDlg->m_nChannel == 1)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x97e00);
				memcpy(pDlg->m_lpFrame.PushData, CToolBarDlg::m_1stSpliter.m_pY, 0x52800);
				//memcpy((char*)(m_lpFrame.PushData + 0x65400), m_1stSpliter.m_pU, 0x19500);
				//memcpy((char*)(m_lpFrame.PushData + 0x7E900), m_1stSpliter.m_pV, 0x19500);	
			}else if(pDlg->m_nChannel == 2)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x97e00);
				memcpy(pDlg->m_lpFrame.PushData, CToolBarDlg::m_2ndSpliter.m_pY2, 0x52800);
//				memcpy((char*)(m_lpFrame.PushData + 0x65400), m_2ndSpliter.m_pU2, 0x19500);
//				memcpy((char*)(m_lpFrame.PushData + 0x7E900), m_2ndSpliter.m_pV2, 0x19500);	
			}else if(pDlg->m_nChannel == 3)
			{
				memset(pDlg->m_lpFrame.PushData, 0x80, 0x12FC00);
				
				memcpy(pDlg->m_lpFrame.PushData, CToolBarDlg::m_1stSpliter.m_pY, 0x52800);
				for(int j = 0; j < 480; j++)
				{
					memcpy((char*)(pDlg->m_lpFrame.PushData + 1408 * j), (char*)(CToolBarDlg::m_1stSpliter.m_pY + 704 * j), 704);
					memcpy((char*)(pDlg->m_lpFrame.PushData + 1408 * j + 704), (char*)(CToolBarDlg::m_1stSpliter.m_pY2 + 704 * j), 704);
				}
				//memcpy((char*)(m_lpFrame.PushData + 0xCA800), m_1stSpliter.m_pU, 0x19500);
				//memcpy((char*)(m_lpFrame.PushData + 0xFD200), m_1stSpliter.m_pV, 0x19500);	
				//memcpy((char*)(m_lpFrame.PushData + 0x52800), m_2ndSpliter.m_pY, 0x52800);
				//memcpy((char*)(m_lpFrame.PushData + 0xE3D00), m_2ndSpliter.m_pU2, 0x19500);
				//memcpy((char*)(m_lpFrame.PushData + 0x116700), m_2ndSpliter.m_pV2, 0x19500);	
			}
			//memset((char*)(m_lpFrame.PushData + 0x7BC00), 0, 0x97E00 - 0x7BC00);
			memset(&pDlg->m_lpState, 0, sizeof(XVID_STATE));
			pDlg->m_lpState.nConst = 0x10202;
			dwRetVal = (*pDlg->m_Func_XvidEnc)((DWORD*)(pDlg->m_lpCreate.Unk21), 2, &pDlg->m_lpFrame, &pDlg->m_lpState);
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwVideoID;
			if(nCounts % 250 == 0)pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0x10;
			else pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = CToolBarDlg::m_pAviFile->GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			nFrmaeID ++;
			CToolBarDlg::m_pAviFile->Write(&dwVideoID, 4);
			CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
			CToolBarDlg::m_pAviFile->Write(pDlg->m_lpFrame.PopData, dwRetVal);
			nCounts++;

			dwRetVal = 0x140;
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
			pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
			pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = CToolBarDlg::m_pAviFile->GetPosition();
			pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
			if(1)
			{
				CToolBarDlg::m_pAviFile->Write(&dwAudioID, 4);
				CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
				CToolBarDlg::m_pAviFile->Write(CToolBarDlg::m_1stSpliter.m_1stOutDatas.buf_A, 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = CToolBarDlg::m_pAviFile->GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				CToolBarDlg::m_pAviFile->Write(&dwAudioID, 4);
				CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
				CToolBarDlg::m_pAviFile->Write((char*)(CToolBarDlg::m_1stSpliter.m_1stOutDatas.buf_A + 320), 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = CToolBarDlg::m_pAviFile->GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				CToolBarDlg::m_pAviFile->Write(&dwAudioID, 4);
				CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
				CToolBarDlg::m_pAviFile->Write((char*)(CToolBarDlg::m_1stSpliter.m_1stOutDatas.buf_A + 320 * 2), 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = CToolBarDlg::m_pAviFile->GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				CToolBarDlg::m_pAviFile->Write(&dwAudioID, 4);
				CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
				CToolBarDlg::m_pAviFile->Write((char*)(CToolBarDlg::m_1stSpliter.m_1stOutDatas.buf_A + 320 * 3), 320);
				nFrmaeID ++;
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameHeaderID = dwAudioID;
				pDlg->m_SeekPosFrame[nFrmaeID].dwListHeaderID = 0;
				pDlg->m_SeekPosFrame[nFrmaeID].dwSeekPos = CToolBarDlg::m_pAviFile->GetPosition();
				pDlg->m_SeekPosFrame[nFrmaeID].dwFrameSize = dwRetVal;
				CToolBarDlg::m_pAviFile->Write(&dwAudioID, 4);
				CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
				CToolBarDlg::m_pAviFile->Write((char*)(CToolBarDlg::m_1stSpliter.m_1stOutDatas.buf_A + 320 * 4), 320);
				nFrmaeID ++;
			}else{
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
			if(CToolBarDlg::m_1stSpliter.m_file->GetPosition() >= CToolBarDlg::m_1stSpliter.m_file->GetLength() - 0xC80) break;
			if(pDlg->m_bEnd == true) break;
		}while(dwRetVal > 0);
		dwRetVal = pDlg->m_AVIFile.GetPosition() - 0x7FC;
		CToolBarDlg::m_pAviFile->Seek(0x7F8, CFile::begin);
		CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
		CToolBarDlg::m_pAviFile->SeekToEnd();
		dwRetVal = 0x31786469;
		CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
		dwRetVal = nFrmaeID * sizeof(SEEKPOS_FRAME);
		CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
		if(pDlg->m_bEnd == false)	
		for(int i = 0; i < nFrmaeID; i++)
		{
			CToolBarDlg::m_pAviFile->Write(&pDlg->m_SeekPosFrame[i], sizeof(SEEKPOS_FRAME));
		}
		dwRetVal = CToolBarDlg::m_pAviFile->GetPosition() - 8;
		CToolBarDlg::m_pAviFile->Seek(4, CFile::begin);
		CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
		CToolBarDlg::m_pAviFile->Seek(0x30, CFile::begin);
		CToolBarDlg::m_pAviFile->Write(&nCounts, 4);
		CToolBarDlg::m_pAviFile->Seek(0x8C, CFile::begin);
		CToolBarDlg::m_pAviFile->Write(&nCounts, 4);
		dwRetVal = dwRetVal / 10;
		CToolBarDlg::m_pAviFile->Write(&dwRetVal, 4);
		CToolBarDlg::m_pAviFile->Close();
		MessageBox(NULL, _T("AVI Conversion Success!!!"), NULL, 0);
		pDlg->m_bEnd = false;
//		if(pDlg->m_bEnd == false) pDlg->OnClose();
	return 0;
}

void CToolBarDlg::TimeSeek(float fSeekPos, int nCurPath)
{
	
	m_Slider_Seek.SetPos((int)fSeekPos * m_Slider_Seek.GetRangeMax());
	if(m_nThreadCounts == 2) 
	{
		KillTimer(TIMER_PLAYER1);
		KillTimer(TIMER_PLAYER2);
	}else KillTimer(TIMER_PLAYER1);
	KillTimer(TIMER_SLIDER);
	KillTimer(TIMER_ALARM);
	Sleep(50);
	//OnStop();
	
	m_Slider_Seek.SetPos(m_Slider_Seek.m_nRealCurPos);
	int nSeekPos = (fSeekPos * m_Slider_Seek.GetRangeMax());
	if(m_bSignal == TRUE) m_bSeekSignal = TRUE;
	// TODO: Add your control notification handler code here
	if(m_nThreadCounts == 0) return;
	
	if(m_nThreadCounts == 2)
	{
		KillTimer(TIMER_PLAYER1);
		KillTimer(TIMER_PLAYER2);
		m_bPlayer1 = false;
		m_bPlayer2 = false;
		if(m_1stSpliter.m_file->m_hFile != INVALID_HANDLE_VALUE /*0xFFFFFFFF*/)
		m_1stSpliter.m_file->Close();
		if(m_2ndSpliter.m_file->m_hFile != INVALID_HANDLE_VALUE /*0xFFFFFFFF*/)
		m_2ndSpliter.m_file->Close();
		m_nThreadCounts = 0;
//gsi
		for (int i = 0; i < binDataNum ; i++)
		{
			memset(&m_pInfoListDlg->m_pBinData[i],0,sizeof(TempBinData));
		}
		binDataNum = 0;
	}else if(m_nThreadCounts == 1)
	{
		m_bPlayer1 = false;
		m_nThreadCounts = 0;
		KillTimer(TIMER_PLAYER1);
		if(m_1stSpliter.m_file->m_hFile != INVALID_HANDLE_VALUE /*0xFFFFFFFF*/)
		m_1stSpliter.m_file->Close();
	}
	float fPos = (float)nSeekPos;
	if(m_2ndFilePath != "") m_nThreadCounts = 2;
	else m_nThreadCounts = 1;
	m_nOutDatasPtr = 0;
	if(m_nThreadCounts == 2)
	{
		m_1stFile.Open(CNewProjDlg::m_FirstPathArrays[CNewProjDlg::m_nPathArraysPtr], CFile::modeRead);
		m_1stSpliter.Initialization(&m_1stFile);
		m_1stSpliter.m_file->SeekToBegin();
		m_2ndFile.Open(CNewProjDlg::m_SecondPathArrays[CNewProjDlg::m_nPathArraysPtr], CFile::modeRead);
		m_2ndSpliter.Initialization(&m_2ndFile);
		m_2ndSpliter.m_file->SeekToBegin();
		SetTimer(TIMER_SLIDER, 1000, NULL);
		if(m_1stSpliter.Seek(fPos / m_Slider_Seek.GetRangeMax()) == false) return;
		if(m_2ndSpliter.Seek(fPos / m_Slider_Seek.GetRangeMax(), m_1stSpliter.m_FrameHeader.dwDTS) == false) return;
		m_bPlayer1 = true;
		SetTimer(TIMER_PLAYER1, m_nSpeed / 10, NULL);
		m_bPlayer2 = true;
		SetTimer(TIMER_PLAYER2, m_nSpeed / 10, NULL);

		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
		m_2ndSpliter.GetDatas2();
		m_2ndOutDatas[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas;
		m_nOutDatasPtr ++;
		
		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
		m_2ndSpliter.GetDatas2();
		m_2ndOutDatas[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas;
		m_nOutDatasPtr ++;
		
		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
		m_2ndSpliter.GetDatas2();
		m_2ndOutDatas[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas;
		m_nOutDatasPtr ++;
		
		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
		m_2ndSpliter.GetDatas2();
		m_2ndOutDatas[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas;
		m_nOutDatasPtr ++;
		
		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
		m_2ndSpliter.GetDatas2();
		m_2ndOutDatas[m_nOutDatasPtr] = m_2ndSpliter.m_2ndOutDatas;
			m_nOutDatasPtr ++;

	}else if(m_nThreadCounts == 1)
	{
		SetTimer(TIMER_SLIDER, 1000, NULL);
		m_1stFile.Open(CNewProjDlg::m_FirstPathArrays[CNewProjDlg::m_nPathArraysPtr], CFile::modeRead);
		m_1stSpliter.Initialization(&m_1stFile);
		m_1stSpliter.m_file->SeekToBegin();
		m_1stSpliter.Seek(fPos / m_Slider_Seek.GetRangeMax());
		m_bPlayer1 = true;

		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
		m_nOutDatasPtr ++;
		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
		m_nOutDatasPtr ++;
		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
		m_nOutDatasPtr ++;
		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
		m_nOutDatasPtr ++;
		m_1stSpliter.GetDatas1();
		m_1stOutDatas[m_nOutDatasPtr] = m_1stSpliter.m_1stOutDatas;
			m_nOutDatasPtr ++;
		SetTimer(TIMER_PLAYER1, m_nSpeed / 10, NULL);
	}
	waveOutClose(g_hWaveOut);
	Sleep(150);
	g_bSoundPlay = true;
	if(g_hWaveOut) waveOutClose(g_hWaveOut);
	m_nPlayPtr = 0;
	WAVEFORMATEX wFormat;
	wFormat.wFormatTag = WAVE_FORMAT_1M08;
	wFormat.nChannels = 1;
	wFormat.nSamplesPerSec = 8000;
	wFormat.nAvgBytesPerSec = 16000;
	wFormat.nBlockAlign = 2;
	wFormat.wBitsPerSample = 16;
	wFormat.cbSize = 0;
	waveOutOpen(&g_hWaveOut, WAVE_MAPPER, &wFormat, (DWORD)NULL, (DWORD)this, CALLBACK_NULL);
	//AfxBeginThread(SoundPlayThread, this);
	CTimeLineDlg::m_bTimeFlag = true;
}
void CToolBarDlg::OnDestroy()
{
	CDialog::OnDestroy();
	delete m_pbmpInfoHeader;
	delete m_pbmpFileHeader;
	delete m_bitItem;
	// TODO: Add your message handler code here
}

void CToolBarDlg::OnBnClickedPrevSeg()
{
	// TODO: Add your control notification handler code here
	CNewProjDlg::m_nPathArraysPtr --;
	CNewProjDlg::m_nPathArraysPtr = (CNewProjDlg::m_nPathArraysPtr + CNewProjDlg::m_nPathCounts) % CNewProjDlg::m_nPathCounts;
	for(int i = 0 ; i < CNewProjDlg::m_nPathCounts; i++)
		m_pInfoListDlg->m_fileList.UnSelectItem(i);
	m_pInfoListDlg->m_fileList.SelectItem(CNewProjDlg::m_nPathArraysPtr, true);
	CNewProjDlg::g_bResetFlag = true;
	m_pInfoListDlg->m_fileList.SetItemText(CNewProjDlg::m_nPathArraysPtr, 7, _T("Played"));
}

void CToolBarDlg::OnBnClickedNextSeg()
{
	// TODO: Add your control notification handler code here
	CNewProjDlg::m_nPathArraysPtr ++;
	CNewProjDlg::m_nPathArraysPtr = CNewProjDlg::m_nPathArraysPtr % CNewProjDlg::m_nPathCounts;
	for(int i = 0 ; i < CNewProjDlg::m_nPathCounts; i++)
		m_pInfoListDlg->m_fileList.UnSelectItem(i);
	m_pInfoListDlg->m_fileList.SelectItem(CNewProjDlg::m_nPathArraysPtr, true);
	CNewProjDlg::g_bResetFlag = true;
	m_pInfoListDlg->m_fileList.SetItemText(CNewProjDlg::m_nPathArraysPtr, 7, _T("Played"));
}
int CALLBACK RepairBrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) 
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
void CToolBarDlg::WriteFile(CFile* file, BYTE* pByFrame, DWORD dwFrameSize, BYTE byFraneType)
{
	if(byFraneType == 0x82)
	{
	}
}
int CToolBarDlg::Repairing(CFile* _1stFile, CFile* _2ndFile)
{
	m_1stSpliter.m_file->SeekToBegin();
	m_1stSpliter.m_file->Read(&m_fileHeader, sizeof(_264_FILE_HEADER));
	if(m_fileHeader.dwRepairFlag == -1){
		MessageBox(_T("Already Repaired File"), NULL, NULL);
		return 0;
	}
	m_fileHeader.dwRepairFlag = -1;
	int len;
	char chTemp[512];
	_1stFile->Write(&m_fileHeader, sizeof(_264_FILE_HEADER));
	m_1stSpliter.m_file->Seek(sizeof(_264_FILE_HEADER), CFile::begin);
	do{
		len = m_1stSpliter.m_file->Read(chTemp, 512);
		_1stFile->Write(chTemp, len);
	}while(len == 512);
	_1stFile->Close();
	if(_2ndFile == NULL) 
	{
		MessageBox(_T("Success Repairing!!!"), NULL, NULL);
		return 0;
	}
	m_2ndSpliter.m_file->SeekToBegin();
	m_2ndSpliter.m_file->Read(&m_fileHeader, sizeof(_264_FILE_HEADER));
	if(m_fileHeader.dwRepairFlag == -1)
	{
		MessageBox(_T("Already Repaired File"), NULL, NULL);
		return 0;
	}
	m_fileHeader.dwRepairFlag = -1;
	_2ndFile->Write(&m_fileHeader, sizeof(_264_FILE_HEADER));
	m_2ndSpliter.m_file->Seek(sizeof(_264_FILE_HEADER), CFile::begin);
	do{
		len = m_2ndSpliter.m_file->Read(chTemp, 512);
		_2ndFile->Write(chTemp, len);
	}while(len == 512);
	_2ndFile->Close();
	MessageBox(_T("Success Repairing!!!"), NULL, NULL);
	return 0;
}
void CToolBarDlg::OnBnClickedRepairFile()
{
	// TODO: Add your control notification handler code here
	CFile _1stRepairFile;
	CFile _2ndRepairFile;
	CString str = "";
	CString str1stRepairPath, str2ndRepairPath;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = 0;
	bi.pidlRoot = 0;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = RepairBrowseCallbackProc;

	LPITEMIDLIST idl = new ITEMIDLIST;
	idl = SHBrowseForFolder(&bi);

	if(idl == NULL) 
	{
		return;
	}
	SHGetPathFromIDList(idl, str.GetBuffer(260));
	str.ReleaseBuffer();
	str = str + str.Mid(2, 1);
	str1stRepairPath = str + CNewProjDlg::m_FirstPathArrays[CNewProjDlg::m_nPathArraysPtr].Mid(
		CNewProjDlg::m_FirstPathArrays[CNewProjDlg::m_nPathArraysPtr].GetLength() - 26);
	if(CNewProjDlg::m_SecondPathArrays[CNewProjDlg::m_nPathArraysPtr] != "")
	{
		str2ndRepairPath = str + CNewProjDlg::m_SecondPathArrays[CNewProjDlg::m_nPathArraysPtr].Mid(
			CNewProjDlg::m_FirstPathArrays[CNewProjDlg::m_nPathArraysPtr].GetLength() - 26);
	}
	if(_1stRepairFile.Open(str1stRepairPath, CFile::modeCreate | CFile::modeWrite))
	{
		if(!_2ndRepairFile.Open(str2ndRepairPath, CFile::modeCreate | CFile::modeWrite))
		{
			MessageBox(_T("Cannot Create Repair Second File"), NULL, NULL);
			return;
		}
	}else{
		MessageBox(_T("Cannot Create Repair File"),NULL, NULL);
		return;
	}
	OnStop();
	Sleep(100);
	if(CNewProjDlg::m_SecondPathArrays[CNewProjDlg::m_nPathArraysPtr] != "")
		Repairing(&_1stRepairFile, &_2ndRepairFile);
	else
		Repairing(&_1stRepairFile, NULL);
}

void CToolBarDlg::OnAlarmSwitch(bool bON)
{
	if (bON == true) {
		m_number1.SetBitmap(IDB_NUMBER_RED_CIRCLE);
		m_number2.SetBitmap(IDB_NUMBER_RED_CIRCLE);
		m_number3.SetBitmap(IDB_NUMBER_RED_CIRCLE);
	}
	else {
		m_number1.SetBitmap(IDB_NUMBER_ONE_WHITE);
		m_number2.SetBitmap(IDB_NUMBER_TWO_WHITE);
		m_number3.SetBitmap(IDB_NUMBER_THREE_WHITE);
	}
}
