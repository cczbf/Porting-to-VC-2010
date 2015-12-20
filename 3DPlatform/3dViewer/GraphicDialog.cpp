// GraphicDialog.cpp : implementation file
//

#include "stdafx.h"
#include "3dviewer.h"
#include "3dviewerDoc.h"
#include "CGlobals.h"
#include "GraphicDialog.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicDialog dialog


CGraphicDialog::CGraphicDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphicDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGraphicDialog)
	m_bOverLay = FALSE;
	//}}AFX_DATA_INIT
	m_bMap1 = FALSE ;
	m_bMap2 = FALSE ;
//	m_bPaint1 = FALSE ;
//	m_bPaint2 = FALSE ;
}

void CGraphicDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphicDialog)
	DDX_Control(pDX, IDC_MMD, m_CStaticD);
	DDX_Control(pDX, IDC_AVERAGE, m_CStaticM);
	DDX_Control(pDX, IDC_YPOINT, m_CStaticY);
	DDX_Control(pDX, IDC_XPOINT, m_CStaticX);
	DDX_Check(pDX, IDC_OVERLAYED, m_bOverLay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphicDialog, CDialog)
	//{{AFX_MSG_MAP(CGraphicDialog)
	ON_BN_CLICKED(IDC_MAP1, OnMap1)
	ON_BN_CLICKED(IDC_MAP2, OnMap2)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_OVERLAYED, OnOverlayed)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicDialog message handlers

void CGraphicDialog::OnMap1() 
{
	// TODO: Add your control notification handler code here
//	m_bPaint1 = TRUE ;
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	ASSERT_VALID(pDoc);

    HANDLE	hDib0	= pDoc->m_CBmpViewerData->GetDIBHandle();
	DWORD	pdwHistoR[256],	pdwHistoG[256],	pdwHistoB[256];

	WORD nW = pDoc->m_CDIB.DIBWidth(hDib0);
	WORD nH = pDoc->m_CDIB.DIBHeight(hDib0);

	int size = nW*nH ;

	pDoc->m_CDIB.Histogram( hDib0, pdwHistoR, pdwHistoG, pdwHistoB);

	RECT rectClient ;
	CWnd * pWnd = GetDlgItem(IDC_GRAPHIC1) ;
	CDC * pControlDC = pWnd->GetDC() ;
//	pWnd->GetWindowRect(&rect) ;
	pWnd->GetClientRect(&rectClient) ;

	pWnd->Invalidate() ;
//	pWnd->InvalidateRect(&rectClient) ;
	pWnd->UpdateWindow() ;
	pControlDC->SetMapMode(MM_ANISOTROPIC) ;
	pControlDC->SetWindowExt(256,size) ;
	pControlDC->SetViewportExt(rectClient.right, -rectClient.bottom) ;
	pControlDC->SetViewportOrg(0, rectClient.bottom) ;
	
	double dCount = 0.0 ;
	for(int i = 0 ; i < 256 ; i ++)
	{
		pControlDC->MoveTo(i, 0) ;
		m_nShowAry1[i] = pdwHistoR[i] ;
		dCount = dCount + (double)(m_nShowAry1[i] * i) ;
		pdwHistoR[i] = pdwHistoR[i]*30 ;
		if((int)pdwHistoR[i]>=size)
		{
			pControlDC->LineTo(i, size) ;
		}
		else
		{
			pControlDC->LineTo(i, pdwHistoR[i]) ;
		}
	}
	dCount = dCount / ((double)(nW * nH)) ;

	CPen ColorPen1(PS_DASHDOT, 1, RGB(255, 0, 255)) ; 
	CPen * tmpPen1 = (CPen*)pControlDC->SelectObject(&ColorPen1) ;

	pControlDC->MoveTo((int)dCount, 0) ;
	pControlDC->LineTo((int)dCount, size) ;
	pControlDC->SelectObject(tmpPen1) ;

	CString str1 ;
	str1.Format("%5.2f ( %5.2f )",dCount, 255.0-dCount);
	this->m_CStaticM.SetWindowText("");
 	this->m_CStaticM.SetWindowText(str1);

	double dM = dCount ;
	dCount = 0 ;
	for(int i = 0 ; i < 256 ; i ++)
	{
		m_nShowAry1[i] ;
		dCount = dCount + (double)(m_nShowAry1[i] * (i-dM)*(i-dM)) ;
	}

	dCount = sqrt(dCount / ((double)(nW * nH))) ;

	CPen ColorPen2(PS_DASHDOT, 1, RGB(0, 255, 255)) ; 
	CPen * tmpPen2 = (CPen*)pControlDC->SelectObject(&ColorPen2) ;

	if((int)(dM-dCount) > 0)
	{
		pControlDC->MoveTo((int)(dM-dCount), 0) ;
		pControlDC->LineTo((int)(dM-dCount), size) ;
	}
	if((int)(dM + dCount) < 256 )
	{
		pControlDC->MoveTo((int)(dM+dCount), 0) ;
		pControlDC->LineTo((int)(dM+dCount), size) ;
	}
	pControlDC->SelectObject(tmpPen2) ;
	
	CString str2 ;
	str2.Format("%5.2f",dCount);
	this->m_CStaticD.SetWindowText("");
 	this->m_CStaticD.SetWindowText(str2);

	m_bMap1 = TRUE ;
	pWnd->ReleaseDC(pControlDC) ;
}

void CGraphicDialog::OnMap2() 
{
	// TODO: Add your control notification handler code here
//	m_bPaint2 = TRUE ;
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	ASSERT_VALID(pDoc);

    HANDLE	hDib0	= pDoc->m_CBmpViewerData->GetDIBHandle();
	DWORD	pdwHistoR[256],	pdwHistoG[256],	pdwHistoB[256];

	WORD nW = pDoc->m_CDIB.DIBWidth(hDib0);
	WORD nH = pDoc->m_CDIB.DIBHeight(hDib0);

	int size = nW*nH ;


	pDoc->m_CDIB.Histogram( hDib0, pdwHistoR, pdwHistoG, pdwHistoB);

	RECT rectClient ;

	CPen ColorPen(PS_SOLID, 1, RGB(255, 0, 0)) ; 

	CWnd * pWnd = GetDlgItem(IDC_GRAPHIC2) ;
	CDC * pControlDC = pWnd->GetDC() ;
	pWnd->GetClientRect(&rectClient) ;

	pWnd->Invalidate() ;
	pWnd->UpdateWindow() ;
	pControlDC->SetMapMode(MM_ANISOTROPIC) ;
	pControlDC->SetWindowExt(256,256) ;
	pControlDC->SetViewportExt(rectClient.right, -rectClient.bottom) ;
	pControlDC->SetViewportOrg(0, rectClient.bottom) ;

	pControlDC->MoveTo(0, 0) ;
	pControlDC->LineTo(256, 256) ;
	
	CPen * tmpPen = (CPen*)pControlDC->SelectObject(&ColorPen) ;
	pControlDC->MoveTo(0, 0) ;
	int sum = 0 ;
	for(int i = 0 ; i < 256 ; i ++)
	{
		sum += pdwHistoR[i] ;
		m_nShowAry2[i] = sum*255/(size-1) ;
		pControlDC->LineTo(i, sum*255/(size-1)) ;
		pControlDC->MoveTo(i, sum*255/(size-1)) ;
	}

	pControlDC->SelectObject(tmpPen) ;
	pWnd->ReleaseDC(pControlDC) ;
	m_bMap2 = TRUE ;
}

void CGraphicDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CGraphicDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	ASSERT_VALID(pDoc);

    HANDLE	hDib0	= pDoc->m_CBmpViewerData->GetDIBHandle();

	WORD nW = pDoc->m_CDIB.DIBWidth(hDib0);
	WORD nH = pDoc->m_CDIB.DIBHeight(hDib0);

	int size = nW*nH ;
	if(size == 0)
		size = 256 ;

	CPoint point1, point2 ;
	this->ClientToScreen(&point) ;
	point1 = point ;
	point2 = point ;

	CRect rectWnd1, rectClient1,rectDlg1 ;
	CRect rectWnd2, rectClient2,rectDlg2 ;

	CWnd * pWnd1 = GetDlgItem(IDC_GRAPHIC1) ;
	CWnd * pWnd2 = GetDlgItem(IDC_GRAPHIC2) ;
	CDC * pControlDC1 = pWnd1->GetDC() ;
	CDC * pControlDC2 = pWnd2->GetDC() ;

	pWnd1->GetClientRect(&rectClient1) ;
	pWnd2->GetClientRect(&rectClient2) ;


//	pWnd1->Invalidate() ;
//	pWnd1->UpdateWindow() ;
	pControlDC1->SetMapMode(MM_ANISOTROPIC) ;
	pControlDC1->SetWindowExt(256,size) ;
	pControlDC1->SetViewportExt(rectClient1.right, -rectClient1.bottom) ;
	pControlDC1->SetViewportOrg(0, rectClient1.bottom) ;

//	pWnd2->Invalidate() ;
//	pWnd2->UpdateWindow() ;
	pControlDC2->SetMapMode(MM_ANISOTROPIC) ;
	pControlDC2->SetWindowExt(256,256) ;
	pControlDC2->SetViewportExt(rectClient2.right, -rectClient2.bottom) ;
	pControlDC2->SetViewportOrg(0, rectClient2.bottom) ;

	pControlDC1->DPtoLP(&rectClient1) ;
	pControlDC2->DPtoLP(&rectClient2) ;
	pWnd1->ScreenToClient(&point1) ;
	pWnd2->ScreenToClient(&point2) ;
	pControlDC1->DPtoLP(&point1) ;
	pControlDC2->DPtoLP(&point2) ;
	this->GetWindowRect(rectWnd1) ;
	this->GetWindowRect(rectWnd2) ;

	pWnd1->GetWindowRect(&rectDlg1) ;
	pWnd2->GetWindowRect(&rectDlg2) ;
/*	
	int OffsetX1 = rectDlg1.left - rectWnd1.left-2 ;
	int OffsetX2 = rectDlg2.left - rectWnd2.left-2 ;
	int OffsetY1 = rectDlg1.top - rectWnd1.top-22 ;
	int OffsetY2 = rectDlg2.top - rectWnd2.top-22 ;

	CPoint point1, point2 ;
	point1.x = tmpPoint1.x - OffsetX1 ;
	point1.y = tmpPoint1.y - OffsetY1 ;

	point2.x = tmpPoint2.x - OffsetX2 ;
	point2.y = tmpPoint2.y - OffsetY2 ;

	CPoint point11 , point22;
	point11.x = point1.x ;
	point11.y = (rectClient1.bottom - point1.y) ;

	point22.x = point2.x ;
	point22.y = (rectClient2.bottom - point2.y) ;
*/
	rectClient1.NormalizeRect();
	rectClient2.NormalizeRect();
	if(rectClient1.PtInRect(point1))
	{
		if(m_bMap1)
			DisplayPoint1(point1) ;
		else
		{
			CString str ;
			str.Format("%d",0);
			this->m_CStaticX.SetWindowText("");
 			this->m_CStaticX.SetWindowText(str);
			
			str.Format("%d",0);
			this->m_CStaticY.SetWindowText("");
 			this->m_CStaticY.SetWindowText(str);
		}
	}
	else if(rectClient2.PtInRect(point2))
	{
		if(m_bMap2)
			DisplayPoint2(point2) ;
		else
		{
			CString str ;
			str.Format("%d",0);
			this->m_CStaticX.SetWindowText("");
 			this->m_CStaticX.SetWindowText(str);
			
			str.Format("%d",0);
			this->m_CStaticY.SetWindowText("");
 			this->m_CStaticY.SetWindowText(str);
		}
	}
	else
	{
		CString str ;
		str.Format("%d",0);
		this->m_CStaticX.SetWindowText("");
 		this->m_CStaticX.SetWindowText(str);
		
		str.Format("%d",0);
		this->m_CStaticY.SetWindowText("");
 		this->m_CStaticY.SetWindowText(str);

	}

	pWnd1->ReleaseDC(pControlDC1) ;
	pWnd2->ReleaseDC(pControlDC2) ;
}

void CGraphicDialog::DisplayPoint1(CPoint point)
{
	CString strX, strY;

	strX.Format("%d",point.x);
	this->m_CStaticX.SetWindowText("");
 	this->m_CStaticX.SetWindowText(strX);
	
	strY.Format("%d",m_nShowAry1[point.x]);
	this->m_CStaticY.SetWindowText("");
 	this->m_CStaticY.SetWindowText(strY);

}

void CGraphicDialog::DisplayPoint2(CPoint point)
{
	CString strX, strY;

	strX.Format("%d",point.x);
	this->m_CStaticX.SetWindowText("");
 	this->m_CStaticX.SetWindowText(strX);
	
	strY.Format("%d",m_nShowAry2[point.x]);
	this->m_CStaticY.SetWindowText("");
 	this->m_CStaticY.SetWindowText(strY);
}

void CGraphicDialog::OnOverlayed() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	if(m_bOverLay)
	{
		CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
		ASSERT_VALID(pDoc);

		HANDLE	hDib0	= pDoc->m_CBmpViewerData->GetDIBHandle();
		DWORD	pdwHistoR[256],	pdwHistoG[256],	pdwHistoB[256];

		WORD nW = pDoc->m_CDIB.DIBWidth(hDib0);
		WORD nH = pDoc->m_CDIB.DIBHeight(hDib0);

		int size = nW*nH ;


		pDoc->m_CDIB.Histogram( hDib0, pdwHistoR, pdwHistoG, pdwHistoB);

		RECT rectClient ;

		CPen ColorPen(PS_SOLID, 1, RGB(255,0, 0)) ; 

		CWnd * pWnd = GetDlgItem(IDC_GRAPHIC1) ;
		CDC * pControlDC = pWnd->GetDC() ;
		pWnd->GetClientRect(&rectClient) ;

	//	pWnd->Invalidate() ;
	//	pWnd->UpdateWindow() ;

		pControlDC->SetMapMode(MM_ANISOTROPIC) ;
		pControlDC->SetWindowExt(256,256) ;
		pControlDC->SetViewportExt(rectClient.right, -rectClient.bottom) ;
		pControlDC->SetViewportOrg(0, rectClient.bottom) ;

//		pControlDC->MoveTo(0, 0) ;
//		pControlDC->LineTo(256, 256) ;
		
		CPen * tmpPen = (CPen*)pControlDC->SelectObject(&ColorPen) ;
		pControlDC->MoveTo(0, 0) ;
		int sum = 0 ;
		for(int i = 0 ; i < 256 ; i ++)
		{
			sum += pdwHistoR[i] ;
			pControlDC->LineTo(i, sum*255/(size-1)) ;
			pControlDC->MoveTo(i, sum*255/(size-1)) ;
		}

		pControlDC->SelectObject(tmpPen) ;
		pWnd->ReleaseDC(pControlDC) ;
	}
	else
	{
		CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
		ASSERT_VALID(pDoc);

		HANDLE	hDib0	= pDoc->m_CBmpViewerData->GetDIBHandle();
		DWORD	pdwHistoR[256],	pdwHistoG[256],	pdwHistoB[256];

		WORD nW = pDoc->m_CDIB.DIBWidth(hDib0);
		WORD nH = pDoc->m_CDIB.DIBHeight(hDib0);

		int size = nW*nH ;


		pDoc->m_CDIB.Histogram( hDib0, pdwHistoR, pdwHistoG, pdwHistoB);

		RECT rectClient ;

		CPen ColorPen(PS_SOLID, 1, RGB(255,0, 0)) ; 

		CWnd * pWnd = GetDlgItem(IDC_GRAPHIC1) ;
		CDC * pControlDC = pWnd->GetDC() ;
		pWnd->GetClientRect(&rectClient) ;

	//	pWnd->Invalidate() ;
	//	pWnd->UpdateWindow() ;

		pControlDC->SetMapMode(MM_ANISOTROPIC) ;
		pControlDC->SetWindowExt(256,256) ;
		pControlDC->SetViewportExt(rectClient.right, -rectClient.bottom) ;
		pControlDC->SetViewportOrg(0, rectClient.bottom) ;

//		pControlDC->MoveTo(0, 0) ;
//		pControlDC->LineTo(256, 256) ;
		
		CPen * tmpPen = (CPen*)pControlDC->SelectObject(&ColorPen) ;
		int nOldMapMode = pControlDC->SetROP2(R2_NOTCOPYPEN) ;
		pControlDC->MoveTo(0, 0) ;
		int sum = 0 ;
		for(int i = 0 ; i < 256 ; i ++)
		{
			sum += pdwHistoR[i] ;
			pControlDC->LineTo(i, sum*255/(size-1)) ;
			pControlDC->MoveTo(i, sum*255/(size-1)) ;
		}

		pControlDC->SelectObject(tmpPen) ;
		pControlDC->SetROP2(nOldMapMode) ;
		pWnd->ReleaseDC(pControlDC) ;
		OnMap1() ;

	}
	
}

void CGraphicDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	if(m_bMap1)
		OnMap1() ;
	if(m_bOverLay)
		OnOverlayed() ;
//	if(m_bMap2)
		OnMap2() ;
	// Do not call CDialog::OnPaint() for painting messages
}
