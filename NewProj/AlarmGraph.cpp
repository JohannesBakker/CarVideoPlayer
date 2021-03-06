// AlarmGraph.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "AlarmGraph.h"
#include "NewProjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define fixed_col_w 80
#define fixed_col_h 37

#define x_DataWidth		5
#define x_DataHeight	20

// Time step unit : ms
#define TIME_STEP		500


#define X_AXIS_END_OFFSET	100



/////////////////////////////////////////////////////////////////////////////
// CAlarmGraph
TempBinData CAlarmGraph::m_pAlarmBinData[100000];
CAlarmGraph::CAlarmGraph(CWnd* pWnd, CRect rt, UINT uID)
{
	DWORD dwFlags = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL;
	   
	SetScrollRange(SB_HORZ,0,100,false);
    SetScrollRange(SB_VERT,0,100,false);

	Create(NULL,NULL, dwFlags, rt, pWnd, uID);
	m_curPosX = m_curPosY = 0;

	m_dwSizePerData = 0;
	m_dwVideoStartFilePos = 0;
	m_dwCurretFilePos = 0;
	m_nOffsetX = 0;
	m_nStartNo = 0;

	m_arrBinData.RemoveAll();
	m_BinCount = 0;

	for (int i = 0; i < IO_ALARM_SIG_NUM; i++)
		m_bIoAlarmSigsON[i] = false;

	m_dwFirstDTS = 0;
	m_nTotalPoints = 0;
	m_nPointsPerPage = 0;

	m_nMaxX = 0;
}

CAlarmGraph::~CAlarmGraph()
{
}


BEGIN_MESSAGE_MAP(CAlarmGraph, CWnd)
	//{{AFX_MSG_MAP(CAlarmGraph)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
    ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmGraph message handlers

void CAlarmGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rt;
	GetClientRect(&rt);
	CDC memDC;
	CBitmap bmp, *poldbmp; 
	memDC.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rt.Width(), rt.Height());
	poldbmp = memDC.SelectObject(&bmp);

	CBrush hBrush(RGB(47, 56, 66));
	memDC.FillRect(&rt, &hBrush);

	wchar_t chBuf[6];
	CString buf;
	CFont font;
	VERIFY(font.CreateFontW(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

	CPen pen(PS_SOLID, 0, RGB(255, 255, 255));
	CPen dotPen(PS_DOT, 0, RGB(255, 255, 255));
	memDC.SelectObject(&pen);
	memDC.MoveTo(80 - m_curPosX, 0);
	memDC.LineTo(80 - m_curPosX, rt.Height());

	memDC.SelectObject(&font);
	memDC.SetBkColor(RGB(47, 57, 66));
	memDC.SetTextColor(RGB(185, 185, 255));

	//	Draw grid lines
	int nCols = rt.Width() / fixed_col_w;
	int nRows = rt.Height() / fixed_col_h;
	int x, y;

	for(y = 0; y<10 + 3; y++)
	{
		memDC.SelectObject(&pen);
		memDC.MoveTo(0, y*fixed_col_h-m_curPosY);
		memDC.LineTo(rt.Width(), y*fixed_col_h-m_curPosY);

		if (y >= IO_ALARM_SIG_NUM) 
			continue;

		//	Draw dotlines - example : ..................................
		memDC.SelectObject(&dotPen);
		memDC.MoveTo(fixed_col_w+45 - m_curPosX, y * fixed_col_h+19-m_curPosY);
		memDC.LineTo(rt.Width(), y*fixed_col_h+19-m_curPosY);
	}

	//	Draw Text
	char StrBuf[100];
	memset(StrBuf, 0, sizeof(StrBuf));
	for(y=0; y < IO_ALARM_SIG_NUM; y++)
	{
		itoa(y+1, StrBuf, 10);
		Ascii2Unichar(StrBuf, chBuf);
		buf.Format(_T("IO%s"), chBuf);
		memDC.TextOut(30 - m_curPosX, y*fixed_col_h + 10-m_curPosY, buf);
		//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[252], StrBuf);
		memDC.TextOut(fixed_col_w + 15 - m_curPosX, y*fixed_col_h+2-m_curPosY, CNewProjDlg::m_wLanguageRes[252]);
		//Unichar2Ascii(CNewProjDlg::m_wLanguageRes[253], StrBuf);
		memDC.TextOut(fixed_col_w + 10 - m_curPosX, y*fixed_col_h+20-m_curPosY, CNewProjDlg::m_wLanguageRes[253]);
	}

	BYTE alarms[8];
	int i= 0, j = 1;
	int dx = fixed_col_w + 45;
	CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));
	memDC.SelectObject(&redPen);
	CPoint oldPoint[8];

	for (i = 0; i < m_arrBinData.GetSize(); i++) {

		AlarmBinData_t oneData = m_arrBinData.GetAt(i);

		// get the alarm informations
		for (x = 0; x < IO_ALARM_SIG_NUM; x++) {
			alarms[x < 4 ? x: 11-x] = (oneData.alamBinData.alarmInfo & j << x) >> x;

			// Draw them
			// Attention!!! In the case of alarm is active, the height of wave of it is 20, that is
			int bAlarmOn = 0;
			if (oneData.alarmFlags & (1 << x))
				bAlarmOn = 1;

			int timeOffset = (oneData.alarmOffsetTime - m_dwFirstDTS) / TIME_STEP;
			int cx = cx = GetAbsolteXPos(&oneData) - m_curPosX;
			int cy = x * fixed_col_h + 28 - m_curPosY - bAlarmOn * x_DataHeight;

			if (i == 0) {
				memDC.MoveTo(dx - m_curPosX, cy);
			}
			else {
				memDC.MoveTo(oldPoint[x]);
				int yy = oldPoint[x].y;

				if (cy != yy) {
					memDC.LineTo(cx, yy);
					memDC.MoveTo(cx, yy);
				}

				memDC.LineTo(cx, cy);
				oldPoint[x] = CPoint(cx, cy);
			}

			memDC.LineTo(cx, cy);
			oldPoint[x] = CPoint(cx, cy);
		}
	}

	dc.BitBlt(rt.left, rt.top, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(poldbmp);
	memDC.DeleteDC();
	// TODO: Add your message handler code here

	// Do not call CStatic::OnPaint() for painting messages
}

void CAlarmGraph::setDatas(DWORD dura,TempBinData* pBinData, int count )
{
	m_dura =  dura;
	memcpy((void*)&m_pAlarmBinData,(void*)pBinData,sizeof(TempBinData)*count);
	m_BinCount = count;
	m_nAlarmCount = 0;
	UpdateScrollSizes();
	Invalidate();
}


void CAlarmGraph::setDatasWithTime(DWORD dwFirstDTS, DWORD currDTS, MainBinaryData* pBinData, unsigned char alarmFlag)
{
	AlarmBinData_t newAlarm;

	newAlarm.alarmOffsetTime = currDTS;
	newAlarm.alarmFlags = alarmFlag;
	memcpy(&newAlarm.alamBinData, pBinData, sizeof(MainBinaryData));

	m_arrBinData.Add(newAlarm);
	m_BinCount = m_arrBinData.GetSize();

	m_dwFirstDTS = dwFirstDTS;
	m_nTotalPoints = (newAlarm.alarmOffsetTime - m_dwFirstDTS) / TIME_STEP;

	UpdateScrollSizesExt();
	Invalidate();

}

void CAlarmGraph::ClearAlarmGraph()
{
	m_dura = 0;
	m_BinCount = 0;
	m_nAlarmCount = 0;
	
	m_dwCurretFilePos = 0;
	m_dwVideoStartFilePos = 0;
	m_dwSizePerData = 1;

	m_arrBinData.RemoveAll();
	m_nTotalPoints = 0;

	for (int i = 0; i < IO_ALARM_SIG_NUM; i++)
		m_bIoAlarmSigsON[i] = false;
	
	UpdateScrollSizesExt();
	Invalidate();
}


void CAlarmGraph::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
#if 10
	CRect client;
	GetClientRect(client);

	int maxY = fixed_col_h * 10 - client.Height();
	
	switch(nSBCode)
	{
	case SB_LINEDOWN:		//Scroll one line down.
		if (m_curPosY < maxY)
			m_curPosY+=70;
		break;
		
	case SB_LINEUP:			//Scroll one line up.
		if (m_curPosY > 0)
			m_curPosY -= 70;
		break;
		
	case SB_BOTTOM:			//Scroll to bottom.
		m_curPosY = maxY;
		break;
	case SB_PAGEDOWN:		//Scroll one page down.
		m_curPosY += 140;
		break;
	case SB_PAGEUP:			//Scroll one page up.
		m_curPosY -= 140;
		m_curPosY = max(0, m_curPosY);
		break;
	case SB_THUMBPOSITION:	//Scroll to the absolute position. The current position is provided in nPos.
	case SB_THUMBTRACK:		//Drag scroll box to specified position. The current position is provided in nPos.	
		if ((UINT)m_curPosY != nPos)
			m_curPosY = nPos;
		break;
	case SB_TOP:			//Scroll to top.
		m_curPosY = 0;
		break;
	case SB_ENDSCROLL:		//End scroll.
		break;
	default:
		break;
	}
	
	// redraw now!	
	SetScrollPos(SB_VERT, (int)(((float)m_curPosY / maxY) * 100), true );
	Invalidate();
#endif
}

void CAlarmGraph::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	CRect client;
	GetClientRect(client);

	int nArrSize = m_arrBinData.GetSize();
	if (nArrSize <= 0)
		return;

	AlarmBinData_t lastBinData = m_arrBinData.GetAt(nArrSize - 1);

	switch(nSBCode)
	{
	case SB_LINELEFT:		//Scroll left.
		if (m_curPosX > 0)
			m_curPosX -= 20;
		m_curPosX = max(0, m_curPosX);
		break;

	case SB_LINERIGHT:		//Scroll right one line.
		if (m_curPosX < m_nMaxX)
			m_curPosX += 20;
		m_curPosX = min(m_nMaxX, m_curPosX);
		break;

	case SB_LEFT:			//Scroll to far left.
		m_curPosX = 0;
		break;
	case SB_PAGELEFT:		//Scroll one page left.
		m_curPosX -= 50;
		m_curPosX = max(0, m_curPosX);
		break;		
	case SB_RIGHT:			//Scroll to far right.
		m_curPosX = m_nMaxX;
		break;
	case SB_PAGERIGHT:		//Scroll one page right.
		m_curPosX += 50;
		m_curPosX = min(m_nMaxX, m_curPosX);		
		break;

	case SB_THUMBPOSITION:	//Scroll to absolute position. The current position is specified by the nPos parameter.
	case SB_THUMBTRACK:		//Drag scroll box to specified position. The current position is specified by the nPos parameter.
		if (nPos!=(UINT)m_curPosX)
			m_curPosX = nPos;
		break;
	case SB_ENDSCROLL:		//End scroll.
	default:
		break;
	}

	SetScrollPos(SB_HORZ, m_curPosX, true);
	Invalidate();
}

void CAlarmGraph::UpdateScrollSizes()
{
#if 10  // variable sized thumb for scroll bars. 
	// patch from Dave Williss via CodeProject
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_DISABLENOSCROLL;
	info.nMin = 0;
	info.nMax = 100;
	info.nPage = 50;
	info.nPos =  m_curPosY;
	SetScrollInfo(SB_VERT, &info);

	CRect rt;
	GetClientRect(&rt);
	info.nMax = 100;
	if (rt.Width() == 0) return;
	int npages;

	if (m_BinCount < rt.Width() && m_BinCount > rt.Width() - fixed_col_w - 45)
		npages = m_BinCount / (rt.Width() - fixed_col_w - 45);
	else
		npages = m_BinCount / rt.Width();
	if (m_BinCount % rt.Width() > 0)
		npages++;
	if (npages == 0) npages = 1;
	info.nPage = 100 / npages;
	info.nPos = (int)(((float)m_curPosX / abs(m_BinCount - rt.Width())) * 100);
	SetScrollInfo(SB_HORZ, &info);

#endif
}

int CAlarmGraph::GetAbsolteXPos(AlarmBinData_t *pBinData)
{
	int dx = fixed_col_w + 45;
	int timeParts = (pBinData->alarmOffsetTime - m_dwFirstDTS) / TIME_STEP;

	int xPos = dx + timeParts * x_DataWidth;
	return xPos;
}

void CAlarmGraph::UpdateScrollSizesExt()
{
	// patch from Dave Williss via CodeProject
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_DISABLENOSCROLL;
	info.nMin = 0;
	info.nMax = 100;
	info.nPage = 50;
	info.nPos =  m_curPosY;
	SetScrollInfo(SB_VERT, &info);

	CRect rt;
	GetClientRect(&rt);

	if (rt.Width() <= 0)
		return;
	
	int nArrSize = m_arrBinData.GetSize();
	if (nArrSize <= 0) 
	{
		info.nMin = 0;
		info.nMax = 0;
		info.nPage = 0;
		info.nPos = 0;

		m_curPosX = 0;
	}
	else 
	{
		AlarmBinData_t lastBinData = m_arrBinData.GetAt(nArrSize - 1);		

		m_nMaxX = GetAbsolteXPos(&lastBinData) - rt.Width();
		m_nMaxX = max(0, m_nMaxX);
		m_nMaxX += X_AXIS_END_OFFSET;
		
		info.nMin = 0;
		info.nMax = m_nMaxX;
		//		info.nPage = rt.Width() - fixed_col_w - 45;
		info.nPage = 50;
		info.nPos = max(0, m_nMaxX - X_AXIS_END_OFFSET - (int)info.nPage);
		info.nPos = max(0, m_nMaxX - X_AXIS_END_OFFSET);

		m_curPosX = info.nPos;
		
	}
	

	SetScrollInfo(SB_HORZ, &info);
}



void CAlarmGraph::OnSize(UINT nType, int cx, int cy)
{
//	UpdateScrollSizes();
	UpdateScrollSizesExt();
}

BOOL CAlarmGraph::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{	
	OnVScroll(zDelta > 0 ? SB_LINEDOWN : SB_LINEUP, abs(zDelta), NULL);
	return TRUE;
}

