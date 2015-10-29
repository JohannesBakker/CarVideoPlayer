// SpeedGraph.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "NewProjDlg.h"
#include "SpeedGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpeedGraph
TempBinData CSpeedGraph::m_pSpeedBinData[100000];
CSpeedGraph::CSpeedGraph()
{
}

CSpeedGraph::~CSpeedGraph()
{
}


BEGIN_MESSAGE_MAP(CSpeedGraph, CStatic)
	//{{AFX_MSG_MAP(CSpeedGraph)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeedGraph message handlers

void CSpeedGraph::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rt;
	GetClientRect(&rt);
	CDC memDC;
	CBitmap bmp, *poldbmp; 
	memDC.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rt.Width(), rt.Height());
	poldbmp = memDC.SelectObject(&bmp);
	//¹É°Ò»°°û¶®±¨ 
	CBrush hBrush(RGB(47, 56, 66));
	memDC.FillRect(&rt, &hBrush);
	//memDC.FillSolidRect(0, 0, rt.Width(), rt.Height(), RGB(47, 57, 66));
	CPen pen(PS_SOLID,0,RGB(255,255,255));
	memDC.SelectObject(&pen);
	char chBuf[6];
	CString buf;
	CFont font;
	VERIFY(font.CreateFont(
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
	// TODO: Add your message handler code here
	

	
	memDC.SelectObject(&font);
	memDC.SetBkColor(RGB(47, 57, 66));
	memDC.SetTextColor(RGB(255, 255, 255));
	memDC.ExtTextOut(23, (rt.Height() - 5) / 2 - 10, ETO_OPAQUE, CRect(0, 0 , 0, 0), CNewProjDlg::m_wLanguageRes[88], NULL);
	memDC.MoveTo(80, 0);
	memDC.LineTo(80, rt.Height());
	memDC.MoveTo(0, rt.Height() - 3);
	memDC.LineTo(rt.Width(), rt.Height() - 3);
	int i;
	for(i = 0; i < 7; i ++)
	{
		for(int j = 130; j < rt.Width(); j += 5)
		{	
			memDC.MoveTo(j, rt.Height() - 10 - i * (rt.Height() - 20) / 6);
			memDC.LineTo(j + 2, rt.Height() - 10 - i * (rt.Height() - 20) / 6);
		}
		itoa(i * 20, chBuf, 10);
		buf = chBuf;
		buf += "km/h";
		memDC.ExtTextOut(90, rt.Height() - 18 - i * (rt.Height() - 20) / 6, ETO_OPAQUE, CRect(0, 0, 0, 0), buf, NULL);
	}
	for(i = 1; i < 5; i++)
	{
		for(int j = 10; j < rt.Height() - 10; j += 5)
		{
			memDC.MoveTo(130 + i * (rt.Width() - 130) / 5, j);
			memDC.LineTo(130 + i * (rt.Width() - 130) / 5, j + 2);
		}
	}
	
	CPen speedPen(PS_SOLID,0,RGB(0,255,200));

	m_nSpeedCount = 0;
	memDC.SelectObject(&speedPen);
		for ( i = 0; i < m_BinCount; i++)
		{
			if(m_pSpeedBinData[i].nBinSize != 60) continue;
			m_nSpeedCount ++;
			if ( m_nSpeedCount == 1)
			{				
				float speedId = (float) m_pSpeedBinData[i].mBinData.speed1 / 100;			
				int x	= (int)	(float) m_pSpeedBinData[i].dwCurPos * (rt.Width() - 130) / m_dura + 130;
				int y	= (int) ((rt.Height() - (rt.Height() - 20) * speedId / 120 - 10) );
				memDC.MoveTo(x,y);
			}
			else
			{				
				float speedId = (float) m_pSpeedBinData[i].mBinData.speed1 / 100;				
				int x	= (int)	(float) m_pSpeedBinData[i].dwCurPos * (rt.Width() - 130) / m_dura + 130;
				int	y	= (int) ((rt.Height() - (rt.Height() - 20) * speedId / 120 - 10) );				
				memDC.LineTo(x,y);				
			}
		}
	dc.BitBlt(rt.left, rt.top, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(poldbmp);
	memDC.DeleteDC();
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}
void CSpeedGraph::setDatas(DWORD dura,TempBinData* pBinData, int count )
{
	m_dura =  dura;
	memcpy((void*)&m_pSpeedBinData,(void*)pBinData,sizeof(TempBinData)*count);
	m_BinCount = count;
	m_nSpeedCount = 0;
	//OnPaint();
}