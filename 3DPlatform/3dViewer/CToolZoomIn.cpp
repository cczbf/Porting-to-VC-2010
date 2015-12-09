//## begin module%369E27A000A3.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E27A000A3.cm

//## begin module%369E27A000A3.cp preserve=no
//## end module%369E27A000A3.cp

//## Module: CToolZoomIn%369E27A000A3; Pseudo Package body
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CToolZoomIn.cpp

//## begin module%369E27A000A3.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%369E27A000A3.additionalIncludes

//## begin module%369E27A000A3.includes preserve=yes
//## end module%369E27A000A3.includes

// CToolZoomIn
#include "CToolZoomIn.h"
//## begin module%369E27A000A3.additionalDeclarations preserve=yes
//## end module%369E27A000A3.additionalDeclarations


// Class CToolZoomIn 

CToolZoomIn::CToolZoomIn()
  //## begin CToolZoomIn::CToolZoomIn%.hasinit preserve=no
  //## end CToolZoomIn::CToolZoomIn%.hasinit
  //## begin CToolZoomIn::CToolZoomIn%.initialization preserve=yes
  //## end CToolZoomIn::CToolZoomIn%.initialization
{
  //## begin CToolZoomIn::CToolZoomIn%.body preserve=yes
	this->set_m_nToolID(1);
	set_m_pCBmpViewerData(NULL);
	CToolBase *pTBS	= (CToolBase*)this;
	m_pCursorBase	= new CCursorZoomIn(pTBS);
  //## end CToolZoomIn::CToolZoomIn%.body
}


CToolZoomIn::~CToolZoomIn()
{
  //## begin CToolZoomIn::~CToolZoomIn%.body preserve=yes
	delete (CCursorZoomIn*)m_pCursorBase;
  //## end CToolZoomIn::~CToolZoomIn%.body
}



//## Other Operations (implementation)
BOOL CToolZoomIn::OnLButtonDown (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonDown%916340333.body preserve=yes

	CBmpViewerData * pData	= (CBmpViewerData *)get_m_pCBmpViewerData();
	BOOL bRet = FALSE;
	if ( pData != NULL ) 
	{
		if (pData->IsCursorInsideImage( point.x, point.y ))
		{
			int x0, y0, w0, h0, x1, y1, w1, h1;
			pData->GetNewPosition (	x0, y0, w0, h0, 
									x1, y1, w1, h1);
		
			w0 *= 2;
			h0 *= 2;

			x0 = (x0 - point.x)*2 + point.x;
			y0 = (y0 - point.y)*2 + point.y;

			if ( w0/w1 <=128 )
				pData->SetNewPosition (	x0, y0, w0, h0, 
										x1, y1, w1, h1);
			bRet = TRUE;
		}

	}

	return TRUE;//Need to redraw

  //## end CToolZoomIn::OnLButtonDown%916340333.body
}

BOOL CToolZoomIn::OnLButtonUp (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return FALSE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolZoomIn::OnMButtonUp (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolZoomIn::OnMButtonDown (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolZoomIn::OnRButtonUp (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolZoomIn::OnRButtonDown (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolZoomIn::OnMouseMove (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnMouseMove%916340336.body preserve=yes
	return 1;
  //## end CToolZoomIn::OnMouseMove%916340336.body
}

BOOL CToolZoomIn::OnLBDblClick (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLBDblClick%916340337.body preserve=yes
	return 1;
  //## end CToolZoomIn::OnLBDblClick%916340337.body
}

// Additional Declarations
  //## begin CToolZoomIn%369E27A000A3.declarations preserve=yes
  //## end CToolZoomIn%369E27A000A3.declarations

//## begin module%369E27A000A3.epilog preserve=yes
//## end module%369E27A000A3.epilog
