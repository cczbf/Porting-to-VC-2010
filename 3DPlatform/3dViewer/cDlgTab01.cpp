// DlgTab01.cpp : implementation file
//

#include "stdafx.h"
#include "3dViewer.h"
#include "resource.h"
#include "CDlgTab01.h"
//#include "CDlgTab00.h"
#include "3dViewerDoc.h"
#include "CGlobals.h"
#include "COpenGL3DEx.h"

#include "CMeshOpti.h"
#include "CRelxationLabelling.h"

#include "2DMesh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTab01 dialog

BOOL	CDlgTab01::m_bLighting = FALSE;
BOOL    CDlgTab01::m_bAccelerating = FALSE;
BOOL    CDlgTab01::m_bMemoryBoostering = FALSE;
int     CDlgTab01::m_nAccMul = 0;
int     CDlgTab01::m_nXScale  = 50;
int     CDlgTab01::m_nYScale  = 50;
int     CDlgTab01::m_nZScale  = 50;
int     CDlgTab01::m_nSpeedValue = 50;
int     CDlgTab01::m_nRotationType = 0;
CLinkType CDlgTab01::m_LinkType = NoLink;
CMapType  CDlgTab01::m_ModeType = SURFACE;

COLORREF  CDlgTab01::BackColor = RGB(0,0,0);
COLORREF  CDlgTab01::AmbientColor = RGB(125,125,125);
COLORREF  CDlgTab01::DiffuseColor = RGB(125,125,125);	
COLORREF  CDlgTab01::SpecularColor = RGB(255,255,255);

CDlgTab01::CDlgTab01(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab01::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTab01)
	//}}AFX_DATA_INIT
	//m_bLighting = FALSE;
	

	//m_nRotationType = 0;
}


void CDlgTab01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTab01)
	DDX_Control(pDX, IDC_CHECK_MEMORYBOOSTER, m_ctrlMemoryBoosterCheckBox);
	DDX_Control(pDX, IDC_ACCMUL, m_AM);
	DDX_Control(pDX, IDC_CHECK_ACCELERATING, m_ctrlAccelerateCheckBox);
	DDX_Control(pDX, IDC_STATIC_AMBIENT, m_ControlColorLightAmbient);
	DDX_Control(pDX, IDC_STATIC_SPECULAR, m_ControlColorLightSpecular);
	DDX_Control(pDX, IDC_STATIC_DIFFUSE, m_ControlColorLightDiffuse);
	DDX_Control(pDX, IDC_STATIC_GROUND, m_ControlBackColor);
	DDX_Control(pDX, IDC_CHECK_LIGHTING, m_ctrlLightCheckBox);
	DDX_Control(pDX, IDC_RA2D, m_nRa2D);
	DDX_Control(pDX, IDC_RAGRID, m_nRaGrid);
	DDX_Control(pDX, IDC_RASURFACE, m_nRaSurface);
	DDX_Control(pDX, IDC_CRTYPE, m_CRType);
	DDX_Control(pDX, IDC_RCENTER, m_RCENTER);
	DDX_Control(pDX, IDC_RRANDOM, m_RRANDOM);
	DDX_Control(pDX, IDC_RMOVE, m_RMOVE);
	DDX_Control(pDX, IDC_RYZ, m_RYZ);
	DDX_Control(pDX, IDC_RXY, m_RXY);
	DDX_Control(pDX, IDC_RXYZ, m_RXYZ);
	DDX_Control(pDX, IDC_RXZ, m_RXZ);
	DDX_Control(pDX, IDC_RNO, m_RNO);
	DDX_Control(pDX, IDC_SSPEED, m_SSpeed);
	DDX_Control(pDX, IDC_SZ, m_SZ);
	DDX_Control(pDX, IDC_SY, m_SY);
	DDX_Control(pDX, IDC_SX, m_SX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTab01, CDialog)
	//{{AFX_MSG_MAP(CDlgTab01)
	ON_BN_CLICKED(IDC_RNO, OnRno)
	ON_BN_CLICKED(IDC_RXY, OnRxy)
	ON_BN_CLICKED(IDC_RXZ, OnRxz)
	ON_BN_CLICKED(IDC_RYZ, OnRyz)
	ON_BN_CLICKED(IDC_RXYZ, OnRxyz)
	ON_BN_CLICKED(IDC_RMOVE, OnRmove)
	ON_BN_CLICKED(IDC_RRANDOM, OnRrandom)
	ON_BN_CLICKED(IDC_RCENTER, OnRcenter)
	ON_CBN_SELCHANGE(IDC_CRTYPE, OnSelchangeCrtype)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RASURFACE, OnRasurface)
	ON_BN_CLICKED(IDC_RAGRID, OnRagrid)
	ON_BN_CLICKED(IDC_RA2D, OnRa2d)
	ON_BN_CLICKED(IDC_CHECK_LIGHTING, OnCheckLighting)
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_CHECK_ACCELERATING, OnCheckAccelerating)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ACCMUL, OnCustomdrawAccmul)
	ON_BN_CLICKED(IDC_CHECK_MEMORYBOOSTER, OnCheckMemoryBooster)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTab01 message handlers

void CDlgTab01::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CDlgTab01::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void CDlgTab01::OnRno() 
{
	// TODO: Add your control notification handler code here
//	void CZoomForm::OnRno() 
{
	m_RNO.SetCheck(1);
	m_RXY.SetCheck(0);
	m_RXZ.SetCheck(0);
	m_RYZ.SetCheck(0);
	m_RXYZ.SetCheck(0);

	m_SX.EnableWindow();
	m_SY.EnableWindow();
	m_SZ.EnableWindow();

//	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();

	m_LinkType = NoLink;

//	if (pView && pView->m_pOpenGL->m_pDS)
//		pView->m_pOpenGL->m_pDS->m_Link = NoLink;
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
		pDoc->m_pCOpenGL->m_pDS->m_Link = NoLink;

}

}

void CDlgTab01::OnRxy() 
{
	// TODO: Add your control notification handler code here

//	void CZoomForm::OnRxy() 
{
	m_RNO.SetCheck(0);
	m_RXY.SetCheck(1);
	m_RXZ.SetCheck(0);
	m_RYZ.SetCheck(0);
	m_RXYZ.SetCheck(0);

	m_SX.EnableWindow();
	m_SY.EnableWindow(FALSE);
	m_SZ.EnableWindow();
	m_SY.SetPos(m_SX.GetPos());

//	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
//	if (pView && pView->m_pOpenGL->m_pDS)
//		pView->m_pOpenGL->m_pDS->m_Link = XYLink;

	m_LinkType = XYLink;

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
		pDoc->m_pCOpenGL->m_pDS->m_Link = XYLink;

}

	
}

void CDlgTab01::OnRxz() 
{
	// TODO: Add your control notification handler code here
//void CZoomForm::OnRxz() 
{
	m_RNO.SetCheck(0);
	m_RXY.SetCheck(0);
	m_RXZ.SetCheck(1);
	m_RYZ.SetCheck(0);
	m_RXYZ.SetCheck(0);

	m_SX.EnableWindow();
	m_SY.EnableWindow();
	m_SZ.EnableWindow(FALSE);
	m_SZ.SetPos(m_SX.GetPos());
/*
	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
	if (pView && pView->m_pOpenGL->m_pDS)
		pView->m_pOpenGL->m_pDS->m_Link = XZLink;
*/
	m_LinkType = XZLink;

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
		pDoc->m_pCOpenGL->m_pDS->m_Link = XZLink;

}

	
}

void CDlgTab01::OnRyz() 
{
	// TODO: Add your control notification handler code here
//	void CZoomForm::OnRyz() 
{

//////////////////////

	m_RNO.SetCheck(0);
	m_RXY.SetCheck(0);
	m_RXZ.SetCheck(0);
	m_RYZ.SetCheck(1);
	m_RXYZ.SetCheck(0);

	m_SX.EnableWindow();
	m_SY.EnableWindow();
	m_SZ.EnableWindow(FALSE);
	m_SZ.SetPos(m_SY.GetPos());
/*
	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
	if (pView && pView->m_pOpenGL->m_pDS)
		pView->m_pOpenGL->m_pDS->m_Link = YZLink;
	
*/	
//////////////////////

	m_LinkType = YZLink;

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
		pDoc->m_pCOpenGL->m_pDS->m_Link = YZLink;
}

	
}

void CDlgTab01::OnRxyz() 
{
	// TODO: Add your control notification handler code here
//	void CZoomForm::OnRxyz() 
{
	m_RNO.SetCheck(0);
	m_RXY.SetCheck(0);
	m_RXZ.SetCheck(0);
	m_RYZ.SetCheck(0);
	m_RXYZ.SetCheck(1);

	m_SX.EnableWindow();
	m_SY.EnableWindow(FALSE);
	m_SZ.EnableWindow(FALSE);
	m_SY.SetPos(m_SX.GetPos());
	m_SZ.SetPos(m_SX.GetPos());

//	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
//	if (pView && pView->m_pOpenGL->m_pDS)
//		pView->m_pOpenGL->m_pDS->m_Link = XYZLink;

	m_LinkType = XYZLink;

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
		pDoc->m_pCOpenGL->m_pDS->m_Link = XYZLink;
}

}

void CDlgTab01::OnRmove() 
{
	// TODO: Add your control notification handler code here
//	void CZoomForm::OnRmove() 
{
	m_RMOVE.SetCheck(1);
	m_RRANDOM.SetCheck(0);
	m_RCENTER.SetCheck(0);

	m_CRType.EnableWindow(FALSE);
	m_SSpeed.EnableWindow(FALSE);

//	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();

/*
	if (pView && pView->m_pOpenGL->m_pDS)
	{
		pView->m_pOpenGL->KillOpenGLTimer(m_CRType.GetCurSel());
		pView->m_pOpenGL->m_pDS->m_Move = MOVE;
		pView->m_pOpenGL->m_nD			= 30;
		pView->m_pOpenGL->m_pDS->m_zTrans = -4;
		pView->InvalidateRect(NULL, FALSE);
	}
	*/
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
	{
		pDoc->m_pCOpenGL->KillOpenGLTimer(m_CRType.GetCurSel());
		pDoc->m_pCOpenGL->m_pDS->m_Move	= MOVE;
		pDoc->m_pCOpenGL->m_nD			= 30;
		pDoc->m_pCOpenGL->m_pDS->m_zTrans = -4;
		pDoc->UpdateView();
	}
}

}

void CDlgTab01::OnRrandom() 
{
	// TODO: Add your control notification handler code here
//void CZoomForm::OnRrandom() 
{
	m_RMOVE.SetCheck(0);
	m_RRANDOM.SetCheck(1);
	m_RCENTER.SetCheck(0);

	m_CRType.EnableWindow(TRUE);
	m_SSpeed.EnableWindow(TRUE);
/*
	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
	if (pView && pView->m_pOpenGL->m_pDS)
	{
		pView->m_pOpenGL->m_pDS->m_Move = RANDROTAT;
		pView->m_pOpenGL->SetOpenGLTimer(m_CRType.GetCurSel());
		pView->m_pOpenGL->m_nD		    = 30;
		pView->m_pOpenGL->m_pDS->m_zTrans = -4;
		pView->InvalidateRect(NULL, FALSE);
	}
*/

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();

/*
	if (pView && pView->m_pOpenGL->m_pDS)
	{
		pView->m_pOpenGL->m_pDS->m_Move = RANDROTAT;
		pView->m_pOpenGL->SetOpenGLTimer(m_CRType.GetCurSel());
		pView->m_pOpenGL->m_nD		    = 30;
		pView->m_pOpenGL->m_pDS->m_zTrans = -4;
		pView->InvalidateRect(NULL, FALSE);
	}
	*/
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
	{

		pDoc->m_pCOpenGL->m_pDS->m_Move = RANDROTAT;
		pDoc->m_pCOpenGL->SetOpenGLTimer(m_CRType.GetCurSel());
		pDoc->m_pCOpenGL->m_nD		    = 30;
		pDoc->m_pCOpenGL->m_pDS->m_zTrans = -4;
		pDoc->UpdateView();

	}

}
	
}

void CDlgTab01::OnRcenter() 
{
	// TODO: Add your control notification handler code here
//void CZoomForm::OnRcenter() 
{
	m_RMOVE.SetCheck(0);
	m_RRANDOM.SetCheck(0);
	m_RCENTER.SetCheck(1);

	m_CRType.EnableWindow(TRUE);
	m_SSpeed.EnableWindow(TRUE);
/*
	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
	if (pView && pView->m_pOpenGL->m_pDS)
	{
		pView->m_pOpenGL->m_pDS->m_Move = CENTERROTAT;
		pView->m_pOpenGL->m_nD			= 20;
		pView->m_pOpenGL->m_pDS->m_zTrans = -2.5f;
		pView->m_pOpenGL->SetOpenGLTimer(m_CRType.GetCurSel());
		pView->InvalidateRect(NULL, FALSE);
	}
*/
CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();

/*
	if (pView && pView->m_pOpenGL->m_pDS)
	{
		pView->m_pOpenGL->KillOpenGLTimer(m_CRType.GetCurSel());
		pView->m_pOpenGL->m_pDS->m_Move = MOVE;
		pView->m_pOpenGL->m_nD			= 30;
		pView->m_pOpenGL->m_pDS->m_zTrans = -4;
		pView->InvalidateRect(NULL, FALSE);
	}
	*/
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
	{
		/*
		pDoc->m_pCOpenGL->KillOpenGLTimer(m_CRType.GetCurSel());
		pDoc->m_pCOpenGL->m_pDS->m_Move	= MOVE;
		pDoc->m_pCOpenGL->m_nD			= 30;
		pDoc->m_pCOpenGL->m_pDS->m_zTrans = -4;
		pDoc->UpdateView();
*/
		pDoc->m_pCOpenGL->m_pDS->m_Move = CENTERROTAT;
		pDoc->m_pCOpenGL->m_nD			= 20;
		pDoc->m_pCOpenGL->m_pDS->m_zTrans = -2.5f;
		pDoc->m_pCOpenGL->SetOpenGLTimer(m_CRType.GetCurSel());
		pDoc->UpdateView();

	}

  }
	
}

void CDlgTab01::OnSelchangeCrtype() 
{
	// TODO: Add your control notification handler code here
//void CZoomForm::OnSelchangeCrtype() 
{
	/*
	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
	if (!pView || !pView->m_pOpenGL->m_pDS)
		return;

	int n = m_CRType.GetCurSel();
	if (pView->m_pOpenGL->m_pDS->m_Rotate != NONE)
		pView->m_pOpenGL->KillOpenGLTimer(pView->m_pOpenGL->m_pDS->m_Rotate);
	if (n != (int)NONE)
		pView->m_pOpenGL->SetOpenGLTimer(n);
	pView->m_pOpenGL->m_pDS->m_Rotate = (CRotateType)n;
	*/

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();

	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
	{
		int n = m_CRType.GetCurSel();
		
		//added by xie dong hai
		m_nRotationType = n;
		//////////////////////////////////////////////////////////////////////////
		
/*
		if (pDoc->m_pCOpenGL->m_pDS->m_Rotate != NONE)
			pDoc->m_pCOpenGL->KillOpenGLTimer(pDoc->m_pCOpenGL->m_pDS->m_Rotate);
*/
		for (int i = RX; i<=RXYZ; i++)
			pDoc->m_pCOpenGL->KillOpenGLTimer(i);

		if (n != (int)NONE)
			pDoc->m_pCOpenGL->SetOpenGLTimer(n);

		pDoc->m_pCOpenGL->m_pDS->m_Rotate = (CRotateType)n;
	}

}
	
}

BOOL CDlgTab01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_CGlobals.m_pDlgTab01 = this;
	// TODO: Add extra initialization here
	m_SX.SetRange(1,100,TRUE);
	m_SY.SetRange(1,100,TRUE);
	m_SZ.SetRange(1,100,TRUE);

	//revised by xie dong hai,2004,10,20
	m_SX.SetPos(m_nXScale);
	m_SY.SetPos(m_nYScale);
	m_SZ.SetPos(m_nZScale);
	
	switch (m_LinkType)
	{
	case NoLink:
		m_RNO.SetCheck(1);
		m_RXZ.SetCheck(0);
		m_RYZ.SetCheck(0);
		m_RXYZ.SetCheck(0);	
		m_RXY.SetCheck(0);		
		break;
	case XYLink :
		m_RNO.SetCheck(0);
		m_RXZ.SetCheck(0);
		m_RYZ.SetCheck(0);
		m_RXYZ.SetCheck(0);	
		m_RXY.SetCheck(1);
		break;
	case XZLink :		
		m_RNO.SetCheck(0);
		m_RXZ.SetCheck(1);
		m_RYZ.SetCheck(0);
		m_RXYZ.SetCheck(0);	
		m_RXY.SetCheck(0);
		break;
	case YZLink :		
		m_RNO.SetCheck(0);
		m_RXZ.SetCheck(0);
		m_RYZ.SetCheck(1);
		m_RXYZ.SetCheck(0);	
		m_RXY.SetCheck(0);
		break;
    case XYZLink :		
		m_RNO.SetCheck(0);
		m_RXZ.SetCheck(0);
		m_RYZ.SetCheck(0);
		m_RXYZ.SetCheck(1);	
		m_RXY.SetCheck(0);
		break;
	default:
		break;
	}

	switch (m_ModeType)
	{
	case SURFACE :
		m_nRaSurface.SetCheck(1);
		m_nRaGrid.SetCheck(0);	
		m_nRa2D.SetCheck(0);
		break;
	case GRID :
		m_nRaSurface.SetCheck(0);
		m_nRaGrid.SetCheck(1);	
		m_nRa2D.SetCheck(0);
		break;
	case PTS :
		m_nRaSurface.SetCheck(0);
		m_nRaGrid.SetCheck(0);	
		m_nRa2D.SetCheck(1);
		break;
	default:
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	
	m_SSpeed.SetRange(0, 100, TRUE);

	m_AM.SetRange(1,10);
	m_nAccMul=g_CGlobals.m_pDlgTab00->m_nNumOfAccSpeed;
	m_AM.SetPos(m_nAccMul);
    m_AM.SetPageSize(1);


	//revised by xie dong hai,2004,10,20
	m_SSpeed.SetPos(m_nSpeedValue);
	m_CRType.SetCurSel(m_nRotationType);
	//////////////////////////////////////////////////////////////////////////
	m_ctrlLightCheckBox.SetCheck(m_bLighting);
   
	m_bAccelerating=g_CGlobals.m_pDlgTab00->m_bAccelerated;
    m_ctrlAccelerateCheckBox.SetCheck(m_bAccelerating);

	m_bMemoryBoostering=g_CGlobals.m_pDlgTab00->m_bMemoryBoostered;
    m_ctrlMemoryBoosterCheckBox.SetCheck(m_bMemoryBoostering);
	
	if(g_CGlobals.m_pDlgTab00 != NULL)
		BackColor = g_CGlobals.m_pDlgTab00->GetBckColor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTab01::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
//	void CZoomForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	/*
	CGLDemoView* pView = (CGLDemoView*)GetOpenGLView();
	if (!pView || !(pView->m_pOpenGL->m_pDS))
	{
		CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}
*/
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();

	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
	{
		switch(nSBCode)
		{
		case TB_PAGEDOWN:

		case TB_PAGEUP:

		case TB_THUMBTRACK:
			if (pScrollBar == (CScrollBar*)&m_SSpeed)
			{
			/*
			pView->m_pOpenGL->m_nTimerSpeed = 2010 - m_SSpeed.GetPos();
			pView->m_pOpenGL->KillOpenGLTimer(pView->m_pOpenGL->m_pDS->m_Rotate);
			pView->m_pOpenGL->SetOpenGLTimer(pView->m_pOpenGL->m_pDS->m_Rotate);
				*/
				pDoc->m_pCOpenGL->m_nTimerSpeed = m_SSpeed.GetPos();

				pDoc->m_pCOpenGL->KillOpenGLTimer(pDoc->m_pCOpenGL->m_pDS->m_Rotate);
				pDoc->m_pCOpenGL->SetOpenGLTimer(pDoc->m_pCOpenGL->m_pDS->m_Rotate);
				
				//Added by xie dong hai,2004,10,20
				m_nSpeedValue = m_SSpeed.GetPos();
				//////////////////////////////////////////////////////////////////////////
				
			}
			else
			{
			/*
			pView->m_pOpenGL->m_pDS->m_xScale = (float)m_SX.GetPos()/50.0f;
			pView->m_pOpenGL->m_pDS->m_yScale = (float)m_SY.GetPos()/50.0f;
			pView->m_pOpenGL->m_pDS->m_zScale = (float)m_SZ.GetPos()/50.0f;
				*/
				pDoc->m_pCOpenGL->m_pDS->m_xScale = (float)m_SX.GetPos()/50.0f;
				pDoc->m_pCOpenGL->m_pDS->m_yScale = (float)m_SY.GetPos()/50.0f;
				pDoc->m_pCOpenGL->m_pDS->m_zScale = (float)m_SZ.GetPos()/50.0f;
				
				//Added by xie dong hai,2004,10,20
				m_nXScale = m_SX.GetPos();
				m_nYScale = m_SY.GetPos();
				m_nZScale = m_SZ.GetPos();		
				////////////////////////////////////
				
				//			switch(pView->m_pOpenGL->m_pDS->m_Link)
				switch(pDoc->m_pCOpenGL->m_pDS->m_Link)
				{
				case XYLink:
					m_SY.SetPos(m_SX.GetPos());
					//				pView->m_pOpenGL->m_pDS->m_yScale = pView->m_pOpenGL->m_pDS->m_xScale;
					pDoc->m_pCOpenGL->m_pDS->m_yScale = pDoc->m_pCOpenGL->m_pDS->m_xScale;
					break;
				case XZLink:
					m_SZ.SetPos(m_SX.GetPos());
					//				pView->m_pOpenGL->m_pDS->m_zScale = pView->m_pOpenGL->m_pDS->m_xScale;
					pDoc->m_pCOpenGL->m_pDS->m_zScale = pDoc->m_pCOpenGL->m_pDS->m_xScale;
					break;
				case YZLink:
					m_SZ.SetPos(m_SY.GetPos());
					//				pView->m_pOpenGL->m_pDS->m_zScale = pView->m_pOpenGL->m_pDS->m_yScale;
					pDoc->m_pCOpenGL->m_pDS->m_zScale = pDoc->m_pCOpenGL->m_pDS->m_yScale;
					break;
				case XYZLink:
					m_SY.SetPos(m_SX.GetPos());
					m_SZ.SetPos(m_SX.GetPos());
					//				pView->m_pOpenGL->m_pDS->m_zScale = pView->m_pOpenGL->m_pDS->m_yScale = pView->m_pOpenGL->m_pDS->m_xScale;
					pDoc->m_pCOpenGL->m_pDS->m_zScale = pDoc->m_pCOpenGL->m_pDS->m_yScale = pDoc->m_pCOpenGL->m_pDS->m_xScale;
					break;
				}
			}
			//		pView->InvalidateRect(NULL, FALSE); 
			pDoc->UpdateView();
			break;
		}

//	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}
}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgTab01::OnRasurface() 
{
	// TODO: Add your control notification handler code here
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
	{
		
		int m_State = m_nRaSurface.GetCheck();
		if(m_State)
		{
			//pDoc->m_pCOpenGL->m_pDS->m_MapType = SURFACE;
			
			m_nRaGrid.SetCheck(0);	
			m_nRa2D.SetCheck(0);	
			
			pDoc->m_pCOpenGL->m_pDS->m_bBase = 1;
			pDoc->m_pCOpenGL->m_pDS->m_bSide = 1;
		}

		m_ModeType = SURFACE;
		((COpenGL3DEx*)(pDoc->m_pCOpenGL))->SetFaceMode();

		pDoc->UpdateView();
	}
	

}

void CDlgTab01::OnRagrid() 
{
	// TODO: Add your control notification handler code here

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc && pDoc->m_pCOpenGL->m_pDS)
	{
		
		int m_State = m_nRaGrid.GetCheck();
		if(m_State)
		{
			//pDoc->m_pCOpenGL->m_pDS->m_MapType = GRID;
			
			m_nRaSurface.SetCheck(0);	
			m_nRa2D.SetCheck(0);	
			
			pDoc->m_pCOpenGL->m_pDS->m_bBase = 0;
			pDoc->m_pCOpenGL->m_pDS->m_bSide = 0;
			if (!pDoc->m_pCOpenGL->m_pDS->m_bBorder)
				pDoc->m_pCOpenGL->m_pDS->m_bAxes = 0;
		}
		
		m_ModeType = GRID;
    	((COpenGL3DEx*)(pDoc->m_pCOpenGL))->SetWireFrameMode();

		pDoc->UpdateView();
	}
	

}

void CDlgTab01::OnRa2d() 
{
	// TODO: Add your control notification handler code here
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc)
	{
		m_nRaSurface.SetCheck(0);
		m_nRaGrid.SetCheck(0);

		m_ModeType = PTS;

		((COpenGL3DEx*)(pDoc->m_pCOpenGL))->SetVertexMode();
		pDoc->UpdateView();
	}
}

void CDlgTab01::OnCheckLighting() 
{
	// TODO: Add your control notification handler code here
	
	m_bLighting = !m_bLighting;
	m_ctrlLightCheckBox.SetCheck(m_bLighting);

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc)
	{
		((COpenGL3DEx*)(pDoc->m_pCOpenGL))->LightingOn(m_bLighting);
		//((COpenGL3DEx*)(pDoc->m_pCOpenGL))->SetLight(m_bLighting);
		pDoc->UpdateView();
	}

	//g_CGlobals.m_pCMainFrame->ShowLightCtrlPanel(m_bLighting);		
}

void CDlgTab01::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		
	CRect rect;
	// Color back
	m_ControlBackColor.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CBrush BrushBack(BackColor);
	dc.FillRect(&rect,&BrushBack);
	
	// Color light ambient
	m_ControlColorLightAmbient.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightAmbient(AmbientColor);
	dc.FillRect(&rect,&BrushLightAmbient);

	// Color light diffuse
	m_ControlColorLightDiffuse.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightDiffuse(DiffuseColor);
	dc.FillRect(&rect,&BrushLightDiffuse);

	// Color light specular
	m_ControlColorLightSpecular.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightSpecular(SpecularColor);
	dc.FillRect(&rect,&BrushLightSpecular);
	
	// TODO: Add your message handler code here
	//m_ctrlLightCheckBox.SetCheck(m_bLighting);
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgTab01::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CDlgTab01::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();

	CRect rect;
	
	// back color
	m_ControlBackColor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	
	if(rect.PtInRect(point))
	{

		CColorDialog dlg(BackColor);

		if(dlg.DoModal()==IDOK)
		{
			BackColor = dlg.GetColor();	

			g_CGlobals.m_pDlgTab00->SetBckColor(BackColor);

			if (pDoc)
				pDoc->m_pCOpenGL->SetBckGroundColor(BackColor);
		}
	}

	// ambient light color
	m_ControlColorLightAmbient.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(AmbientColor);

		if(dlg.DoModal()==IDOK)
		{
			AmbientColor = dlg.GetColor();	

			if (pDoc)
				pDoc->m_pCOpenGL->SetAmbient(AmbientColor);
		}
	}		
	
	// specular light color
	m_ControlColorLightSpecular.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(SpecularColor);
		if(dlg.DoModal()==IDOK)
		{
			SpecularColor = dlg.GetColor();	

			if (pDoc)
				pDoc->m_pCOpenGL->SetSpecular(SpecularColor);
		}
	}		

	// diffuse light color
	m_ControlColorLightDiffuse.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(DiffuseColor);

		if(dlg.DoModal()==IDOK)
		{
			DiffuseColor = dlg.GetColor();	
			
			if (pDoc)
				pDoc->m_pCOpenGL->SetDiffuse(DiffuseColor);
		}
	}		

	pDoc->UpdateView();

	Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgTab01::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CDialog::OnCaptureChanged(pWnd);
}

void CDlgTab01::OnButtonReset() 
{
	// TODO: Add your control notification handler code here

    m_SX.SetPos(50);
	m_SY.SetPos(50);
	m_SZ.SetPos(50);
    m_SSpeed.SetPos(55);
	m_AM.SetPos(0);

	Invalidate();

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc)
	{		
		((COpenGL3DEx*)(pDoc->m_pCOpenGL))->Reset();
		pDoc->UpdateView();
	}	
}

void CDlgTab01::OnCheckAccelerating() 
{
	// TODO: Add your control notification handler code here

	m_bAccelerating = !m_bAccelerating;
	m_ctrlAccelerateCheckBox.SetCheck(m_bAccelerating);
	g_CGlobals.m_pDlgTab00->m_bAccelerated=m_bAccelerating;

}

void CDlgTab01::OnCustomdrawAccmul(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	 UpdateData(TRUE);
     m_nAccMul=m_AM.GetPos();
	 g_CGlobals.m_pDlgTab00->m_nNumOfAccSpeed=m_nAccMul;
     UpdateData(FALSE);
	 *pResult = 0;
}

void CDlgTab01::OnCheckMemoryBooster() 
{
	// TODO: Add your control notification handler code here
	m_bMemoryBoostering = !m_bMemoryBoostering;
	m_ctrlMemoryBoosterCheckBox.SetCheck(m_bMemoryBoostering);
	g_CGlobals.m_pDlgTab00->m_bMemoryBoostered=m_bMemoryBoostering;

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	//release the old mesh
	if(  pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] != NULL)
	{
		delete pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] ;
		pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] = NULL;
	}
		
	//produce a new mesh
	pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex]  = new CMesh;
	((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh = ( pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] ); 
	((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->m_nType = 0;
			
	//load the new mesh
	pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex]->Load(g_CGlobals.m_pDlgTab00->m_cstrFName);

	//normalize the mesh
	((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->Normalize(0.5);
	//update the mesh
	pDoc->UpdateView();
}
