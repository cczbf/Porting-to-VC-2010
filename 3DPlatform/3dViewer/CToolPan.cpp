//## begin module%364F6E8B017C.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%364F6E8B017C.cm

//## begin module%364F6E8B017C.cp preserve=no
//## end module%364F6E8B017C.cp

//## Module: CToolPan%364F6E8B017C; Pseudo Package body
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CToolPan.cpp

//## begin module%364F6E8B017C.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%364F6E8B017C.additionalIncludes

//## begin module%364F6E8B017C.includes preserve=yes
//## end module%364F6E8B017C.includes

// CToolPan
#include "CToolPan.h"
//## begin module%364F6E8B017C.additionalDeclarations preserve=yes
//## end module%364F6E8B017C.additionalDeclarations


// Class CToolPan 


CToolPan::CToolPan ()
  //## begin CToolPan::CToolPan%911175330.hasinit preserve=no
  //## end CToolPan::CToolPan%911175330.hasinit
  //## begin CToolPan::CToolPan%911175330.initialization preserve=yes
  //## end CToolPan::CToolPan%911175330.initialization
{
  //## begin CToolPan::CToolPan%911175330.body preserve=yes
	set_m_nToolID(0);
	set_m_pCBmpViewerData(NULL);
	CToolBase *pTBS	= (CToolBase*)this;
	m_pCursorBase	= new CCursorPan(pTBS);
//		m_pCursorBase	= new CCursorPan;
  //## end CToolPan::CToolPan%911175330.body
}


CToolPan::~CToolPan ()
{
  //## begin CToolPan::~CToolPan%911175331.body preserve=yes
		delete (CCursorPan*)m_pCursorBase;
  //## end CToolPan::~CToolPan%911175331.body
}



//## Other Operations (implementation)
BOOL CToolPan::OnLButtonUp (UINT nFlags, CPoint point)
{
  //## begin CToolPan::OnLButtonUp%911175332.body preserve=yes
	CBmpViewerData * pData	= (CBmpViewerData *)get_m_pCBmpViewerData();
	if ( pData != NULL ) 
	{
		if (pData->GetSelFlag())
		{
			pData->SetSelFlag(FALSE);
		}

	}
	return FALSE;//Do not need to redraw
  //## end CToolPan::OnLButtonUp%911175332.body
}

BOOL CToolPan::OnLButtonDown (UINT nFlags, CPoint point)
{
  //## begin CToolPan::OnLButtonDown%911175333.body preserve=yes
	CBmpViewerData * pData	= (CBmpViewerData *)get_m_pCBmpViewerData();
	if ( pData != NULL ) 
	{
		if (pData->IsCursorInsideImage( point.x, point.y ))
		{
			pData->SetSelFlag(TRUE);
			set_m_cur_pos(point);
		}

	}
	return FALSE;//Do not need to redraw

  //## end CToolPan::OnLButtonDown%911175333.body
}


BOOL CToolPan::OnRButtonUp (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolPan::OnRButtonDown (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}


BOOL CToolPan::OnMButtonUp (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolPan::OnMButtonDown (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}


BOOL CToolPan::OnMouseMove (UINT nFlags, CPoint point)
{
  //## begin CToolPan::OnMouseMove%911175334.body preserve=yes

	CBmpViewerData * pData	= (CBmpViewerData *)get_m_pCBmpViewerData();
	BOOL bRet = FALSE;
	if ( pData != NULL ) 
	{
		if (pData->GetSelFlag())
		{
			int x0, y0, w0, h0, x1, y1, w1, h1;
			pData->GetNewPosition (	x0, y0, w0, h0, 
									x1, y1, w1, h1);
			POINT ptLast = get_m_cur_pos();

			x0 += point.x - ptLast.x;
			y0 += point.y - ptLast.y;
			pData->SetNewPosition (	x0, y0, w0, h0, 
									x1, y1, w1, h1);

			set_m_cur_pos(point);
			bRet = TRUE;
		}

	}
//	this->m_pCCursor->CursorInside(point.x, point.y);
//	this->m_pCCursor->CursorOutside(point.x, point.y);

	return bRet;
  //## end CToolPan::OnMouseMove%911175334.body
}

BOOL CToolPan::OnLBDblClick (UINT nFlags, CPoint point)
{
  //## begin CToolPan::OnLBDblClick%916274419.body preserve=yes

	CBmpViewerData * pData	= (CBmpViewerData *)get_m_pCBmpViewerData();
	BOOL bRet = FALSE;
	if ( pData != NULL ) 
	{
			int x0, y0, w0, h0, x1, y1, w1, h1;
			pData->GetNewPosition (	x0, y0, w0, h0, 
									x1, y1, w1, h1);
			pData->SetNewPosition (	0, 0, w1, h1, 
									x1, y1, w1, h1);
			bRet = TRUE;
	}
	return bRet;
  //## end CToolPan::OnLBDblClick%916274419.body
}

// Additional Declarations
  //## begin CToolPan%364F6E8B017C.declarations preserve=yes
  //## end CToolPan%364F6E8B017C.declarations

//## begin module%364F6E8B017C.epilog preserve=yes
//## end module%364F6E8B017C.epilog
