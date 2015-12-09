// ScrollDlg.cpp : implementation file
//

#include "stdafx.h"
#include "3dviewer.h"
#include "ScrollDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollDlg dialog

#define VERT_PTS 1

CScrollDlg::CScrollDlg(CWnd* pParent /*=NULL*/)
	//: CDialog(CScrollDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScrollDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScrollDlg::CScrollDlg(UINT nIDTemplate, CWnd* pParentWnd )
{
	CDialog::CDialog(nIDTemplate, pParentWnd);
}



void CScrollDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScrollDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScrollDlg, CDialog)
	//{{AFX_MSG_MAP(CScrollDlg)
	ON_WM_VSCROLL()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollDlg message handlers

void CScrollDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	// TODO: Add your message handler code here and/or call default
	
	int nInc; // The Increment required for Scrollbar from Current Position
	
	//Select the Mode of Scrolling and set the Position
	//Variables as below
    switch (nSBCode)
    {
        case SB_TOP:        nInc = -m_nVscrollPos;               break;
        case SB_BOTTOM:     nInc = m_nVscrollMax-m_nVscrollPos;  break;
        case SB_LINEUP:     nInc = -1;                           break;
        case SB_LINEDOWN:   nInc = 1;                            break;
        case SB_PAGEUP:     nInc = min(-1, -m_nVertInc)/10;         break;
        case SB_PAGEDOWN:   nInc = max(1, m_nVertInc)/10;           break;
        case SB_THUMBTRACK: nInc = nPos - m_nVscrollPos;         break;
        default:            nInc = 0;
    }

    //Fetch the Current Position and set the Increment Variable from
	// the Current Position
	nInc = max(-m_nVscrollPos, min(nInc, m_nVscrollMax - m_nVscrollPos));

    // If the Position is changed ,move the Scrollbar and the Window too
	if (nInc)
	{
			m_nVscrollPos += nInc;
			int iMove =-nInc;// -VERT_PTS * nInc;
			ScrollWindow(0, iMove, NULL, NULL);
			SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
	}

	Invalidate(FALSE);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CScrollDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here	
}

void CScrollDlg::SetupScrollbars()
{
	CRect tempRect;
    GetClientRect(&tempRect);
    
    //Max Vertical Scrolling is the difference between size
	//of the Whole Property Page with Controls and that with
	//the current one devided by the Indentation you set

	m_nVertInc = (m_ClientRect.Height() - tempRect.Height())/VERT_PTS;
	
	m_nVscrollMax = max(0, m_nVertInc);
    m_nVscrollPos = min(m_nVscrollPos, m_nVscrollMax);
    SetScrollRange(SB_VERT, 0, m_nVscrollMax, FALSE);
    SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);	

}

void CScrollDlg::ResetScrollbars()
{
	//Scroll the Window to its initial Position
	ScrollWindow(0, m_nVscrollPos*VERT_PTS, NULL, NULL);
    m_nVscrollPos = 0;

	//Setup and Initialize a Vertical Scroll bar
	//You can do the same for Horizontal Scroll bar
    SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
}
