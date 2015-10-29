// ACCGraph.cpp : implementation file
//

#include "stdafx.h"
#include "NewProj.h"
#include "ACCGraph.h"
#include "NewProjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACCGraph
TempBinData CACCGraph::m_pACCBinData[100000];
CACCGraph::CACCGraph()
{
}

CACCGraph::~CACCGraph()
{
}


BEGIN_MESSAGE_MAP(CACCGraph, CStatic)
	//{{AFX_MSG_MAP(CACCGraph)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACCGraph message handlers

void CACCGraph::OnPaint() 
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
	memDC.MoveTo(80, 0);
	memDC.LineTo(80, rt.Height());
	memDC.MoveTo(0, rt.Height() - 3);
	memDC.LineTo(rt.Width(), rt.Height() - 3);
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
	memDC.SelectObject(&font);
	memDC.SetBkColor(RGB(47, 57, 66));
	memDC.SetTextColor(RGB(255, 255, 255));
	memDC.ExtTextOut(10, (rt.Height() - 5) / 2 - 10, ETO_OPAQUE, CRect(0, 0 , 0, 0), CNewProjDlg::m_wLanguageRes[86], NULL);
	char chBuf[6];
	CString buf;
	int i;
	for(i = 0; i < 5; i ++)
	{
		for(int j = 130; j < rt.Width(); j += 5)
		{	
			memDC.MoveTo(j, rt.Height() - 10 - i * (rt.Height() - 15) / 4);
			memDC.LineTo(j + 2, rt.Height() - 10 - i * (rt.Height() - 15) / 4);
		}
		itoa(i - 2, chBuf, 10);
		buf = chBuf;
		buf += "G";
		memDC.ExtTextOut(100, rt.Height() - 17 - i * (rt.Height() - 15) / 4, ETO_OPAQUE, CRect(0, 0, 0, 0), buf, NULL);
	}
	for(i = 1; i < 5; i++)
	{
		for(int j = 5; j < rt.Height() - 10; j += 5)
		{
			memDC.MoveTo(130 + i * (rt.Width() - 130) / 5, j);
			memDC.LineTo(130 + i * (rt.Width() - 130) / 5, j + 2);
		}
	}
	// 	CDC *pDCRed = GetDlgItem(IDC_STATIC_ACC)->GetDC();
	// 	CDC *pDCGreen = GetDlgItem(IDC_STATIC_ACC)->GetDC();
	// 	CDC *pDCBlue = GetDlgItem(IDC_STATIC_ACC)->GetDC();
	// 	CRect rt;
	// 	GetDlgItem(IDC_STATIC_ACC)->GetClientRect(&rt);
	CPen penRed(PS_SOLID, 0, RGB(255, 0, 0));
	CPen penGreen(PS_SOLID, 0, RGB(0, 255, 0));
	CPen penBlue(PS_SOLID, 0, RGB(0, 0, 255));
	// 
	// 	pDCRed->SelectObject(&penRed);
	// 	pDCGreen->SelectObject(&penGreen);
	// 	pDCBlue->SelectObject(&penBlue);

	int R_y,G_y,B_y,x;
	m_nAccSpeedCount = 0; 
	memDC.SelectObject(&penRed);
	m_BinCount = 0;
	for ( i = 0; i < m_BinCount; i++)
	{
		if(m_pACCBinData[i].nBinSize != 60) continue;
		m_nAccSpeedCount ++;
		if(m_nAccSpeedCount == 1)
		{
			x	= (int)	m_pACCBinData[i].dwCurPos  * (rt.Width() - 130) / m_dura + 130;
			if(m_bAcc && m_pACCBinData[i].mBinData.accSpeedData.accOSet_G != 0)
			{
				m_accId = (int)m_pACCBinData[i].mBinData.accSpeedData.acc_G / m_pACCBinData[i].mBinData.accSpeedData.accOSet_G;
				m_bAcc = FALSE;
			}
			
			switch (m_accId)
			{
			case 0:				
				R_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_R - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.3 + (rt.Height() - 15) * 2 / 4 + 5);
				break;
			case 1:				
				R_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_R - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.1 + (rt.Height() - 15) * 2 / 4  + 5);
				
				
				break;
			default:
				break;
			}
			
			memDC.MoveTo(x,R_y);
			
		}
		else
		{
			
			x	= (int)	m_pACCBinData[i].dwCurPos  * (rt.Width() - 130) / m_dura + 130;
			
			if(m_bAcc && m_pACCBinData[i].mBinData.accSpeedData.accOSet_G != 0)
			{
				m_accId = (int)m_pACCBinData[i].mBinData.accSpeedData.acc_G / m_pACCBinData[i].mBinData.accSpeedData.accOSet_G;
				m_bAcc = FALSE;
			}
			switch (m_accId)
			{
			case 0:
				
				R_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_R - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.3 + (rt.Height() - 15) * 2 / 4 + 5);
				
				
				break;
			case 1:
				
				R_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_R - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.1 + (rt.Height() - 15) * 2 / 4  + 5);
				
				
				break;
			default:
				break;
			}
			memDC.LineTo(x,R_y);
		}			
		
	}

	m_nAccSpeedCount = 0; 
	memDC.SelectObject(&penGreen);
	for ( i = 0; i < m_BinCount; i++)
	{
		if(m_pACCBinData[i].nBinSize != 60) continue;
		m_nAccSpeedCount ++;
		if(m_nAccSpeedCount == 1)
		{
			 x	= (int)	m_pACCBinData[i].dwCurPos  * (rt.Width() - 130) / m_dura + 130;
			if(m_bAcc && m_pACCBinData[i].mBinData.accSpeedData.accOSet_G != 0)
			{
				m_accId = (int)m_pACCBinData[i].mBinData.accSpeedData.acc_G / m_pACCBinData[i].mBinData.accSpeedData.accOSet_G;
				m_bAcc = FALSE;
			}
			switch (m_accId)
			{
			case 0:
				G_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_G - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.3 + (rt.Height() - 15) * 2 / 4 + 5);
				
				break;
			case 1:
				G_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_G - m_pACCBinData[i].mBinData.accSpeedData.accOSet_G - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.2 + (rt.Height() - 15) * (2+m_accId) / 4 + 5);
			
				break;
			default:
				break;
			}
			memDC.MoveTo(x,G_y);
						
		}
		else
		{
			
			 x	= (int)	m_pACCBinData[i].dwCurPos  * (rt.Width() - 130) / m_dura + 130;
			
			if(m_bAcc && m_pACCBinData[i].mBinData.accSpeedData.accOSet_G != 0)
			{
				m_accId = (int)m_pACCBinData[i].mBinData.accSpeedData.acc_G / m_pACCBinData[i].mBinData.accSpeedData.accOSet_G;
				m_bAcc = FALSE;
			}
			switch (m_accId)
			{
			case 0:
				G_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_G - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.3 + (rt.Height() - 15) * 2 / 4 + 5);
				break;
			case 1:
				G_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_G - m_pACCBinData[i].mBinData.accSpeedData.accOSet_G - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.2 + (rt.Height() - 15) * (2+m_accId) / 4 + 5);
				break;
			default:
				break;
			}
			memDC.LineTo(x,G_y);
		}			
		
	}
	m_nAccSpeedCount = 0; 
	memDC.SelectObject(&penBlue);
	for ( i = 0; i < m_BinCount; i++)
	{
		if(m_pACCBinData[i].nBinSize != 60) continue;
		m_nAccSpeedCount ++;
		if(m_nAccSpeedCount == 1)
		{
			x	= (int)	m_pACCBinData[i].dwCurPos  * (rt.Width() - 130) / m_dura + 130;
			if(m_bAcc && m_pACCBinData[i].mBinData.accSpeedData.accOSet_G != 0)
			{
				m_accId = (int)m_pACCBinData[i].mBinData.accSpeedData.acc_G / m_pACCBinData[i].mBinData.accSpeedData.accOSet_G;
				m_bAcc = FALSE;
			}
			
			switch (m_accId)
			{
			case 0:
				B_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_B - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.3 + (rt.Height() - 15) * 2 / 4 + 5);
				
				break;
			case 1:
				B_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_B - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.1 + (rt.Height() - 15) * 2 / 4  + 5);
				
				break;
			default:
				break;
			}
			
			memDC.MoveTo(x,B_y);
			
		}
		else
		{
			
		 x	= (int)	m_pACCBinData[i].dwCurPos  * (rt.Width() - 130) / m_dura + 130;
			
			if(m_bAcc && m_pACCBinData[i].mBinData.accSpeedData.accOSet_G != 0)
			{
				m_accId = (int)m_pACCBinData[i].mBinData.accSpeedData.acc_G / m_pACCBinData[i].mBinData.accSpeedData.accOSet_G;
				m_bAcc = FALSE;
			}
			
			switch (m_accId)
			{
			case 0:
				B_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_B - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.3 + (rt.Height() - 15) * 2 / 4 + 5);
				
				break;
			case 1:
				B_y = (int) ((m_pACCBinData[i].mBinData.accSpeedData.acc_B - m_pACCBinData[i].mBinData.accSpeedData.accTotalOffset) * 0.1 + (rt.Height() - 15) * 2 / 4  + 5);
				
				break;
			default:
				break;
			}
			memDC.LineTo(x,B_y);
		}			
		
	}
	dc.BitBlt(rt.left, rt.top, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(poldbmp);
	memDC.DeleteDC();
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CACCGraph::setDatas(DWORD dura,TempBinData* pBinData, int count )
{
	 m_dura =  dura;
	 memcpy((void*)&m_pACCBinData,(void*)pBinData,sizeof(TempBinData)*count);
	 m_BinCount = count;
	 m_nAccSpeedCount = 0;
	 //OnPaint();
}
