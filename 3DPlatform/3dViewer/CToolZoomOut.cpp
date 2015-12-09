//## begin module%369E3E180253.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E3E180253.cm

//## begin module%369E3E180253.cp preserve=no
//## end module%369E3E180253.cp

//## Module: CToolZoomOut%369E3E180253; Pseudo Package body
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CToolZoomOut.cpp

//## begin module%369E3E180253.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%369E3E180253.additionalIncludes

//## begin module%369E3E180253.includes preserve=yes
//## end module%369E3E180253.includes

// CToolZoomOut
#include "CToolZoomOut.h"
//## begin module%369E3E180253.additionalDeclarations preserve=yes
//## end module%369E3E180253.additionalDeclarations


// Class CToolZoomOut 

CToolZoomOut::CToolZoomOut()
  //## begin CToolZoomOut::CToolZoomOut%.hasinit preserve=no
  //## end CToolZoomOut::CToolZoomOut%.hasinit
  //## begin CToolZoomOut::CToolZoomOut%.initialization preserve=yes
  //## end CToolZoomOut::CToolZoomOut%.initialization
{
  //## begin CToolZoomOut::CToolZoomOut%.body preserve=yes
	this->set_m_nToolID(2);
	set_m_pCBmpViewerData(NULL);
	CToolBase *pTBS	= (CToolBase*)this;
	m_pCursorBase	= new CCursorZoomOut(pTBS);



  //## end CToolZoomOut::CToolZoomOut%.body
}


CToolZoomOut::~CToolZoomOut()
{
  //## begin CToolZoomOut::~CToolZoomOut%.body preserve=yes
	delete (CCursorZoomOut*)m_pCursorBase;
  //## end CToolZoomOut::~CToolZoomOut%.body
}



//## Other Operations (implementation)
BOOL CToolZoomOut::OnLButtonDown (UINT nFlags, CPoint point)
{
  //## begin CToolZoomOut::OnLButtonDown%916340334.body preserve=yes

	
	CBmpViewerData * pData	= (CBmpViewerData *)get_m_pCBmpViewerData();
	BOOL bRet = FALSE;
	if ( pData != NULL ) 
	{
		if (pData->IsCursorInsideImage( point.x, point.y ))
		{
			int x0, y0, w0, h0, x1, y1, w1, h1;
			pData->GetNewPosition (	x0, y0, w0, h0, 
									x1, y1, w1, h1);
		
			w0 /= 2;
			h0 /= 2;

			x0 = (x0 - point.x)/2 + point.x;
			y0 = (y0 - point.y)/2 + point.y;

			if ( w0/w1 <=128 )
				pData->SetNewPosition (	x0, y0, w0, h0, 
										x1, y1, w1, h1);
			bRet = TRUE;
		}

	}
	
	return TRUE;//Need to redraw

	//## end CToolZoomOut::OnLButtonDown%916340334.body
}

BOOL CToolZoomOut::OnLButtonUp (UINT nFlags, CPoint point)
{
  //## begin CToolZoomOut::OnLButtonUp%916340338.body preserve=yes
	return FALSE;//Do not need to redraw
	//## end CToolZoomOut::OnLButtonUp%916340338.body
}

BOOL CToolZoomOut::OnMouseMove (UINT nFlags, CPoint point)
{
  //## begin CToolZoomOut::OnMouseMove%916340339.body preserve=yes
	return 1;
  //## end CToolZoomOut::OnMouseMove%916340339.body
}


BOOL CToolZoomOut::OnMButtonUp (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolZoomOut::OnMButtonDown (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}


BOOL CToolZoomOut::OnRButtonUp (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolZoomOut::OnRButtonDown (UINT nFlags, CPoint point)
{
  //## begin CToolZoomIn::OnLButtonUp%916340335.body preserve=yes
	return TRUE;//Do not need to redraw

//## end CToolZoomIn::OnLButtonUp%916340335.body
}

BOOL CToolZoomOut::OnLBDblClick (UINT nFlags, CPoint point)
{
  //## begin CToolZoomOut::OnLBDblClick%916340340.body preserve=yes
	return 1;
  //## end CToolZoomOut::OnLBDblClick%916340340.body
}



// Additional Declarations
  //## begin CToolZoomOut%369E3E180253.declarations preserve=yes
  //## end CToolZoomOut%369E3E180253.declarations

//## begin module%369E3E180253.epilog preserve=yes
//## end module%369E3E180253.epilog
