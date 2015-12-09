//## begin module%369E45E501BE.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E45E501BE.cm

//## begin module%369E45E501BE.cp preserve=no
//## end module%369E45E501BE.cp

//## Module: CCursorZoomOut%369E45E501BE; Pseudo Package body
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CCursorZoomOut.cpp

//## begin module%369E45E501BE.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%369E45E501BE.additionalIncludes

//## begin module%369E45E501BE.includes preserve=yes
//## end module%369E45E501BE.includes

// CCursorZoomOut
#include "CCursorZoomOut.h"
//## begin module%369E45E501BE.additionalDeclarations preserve=yes
//## end module%369E45E501BE.additionalDeclarations


// Class CCursorZoomOut 

CCursorZoomOut::CCursorZoomOut()
  //## begin CCursorZoomOut::CCursorZoomOut%.hasinit preserve=no
  //## end CCursorZoomOut::CCursorZoomOut%.hasinit
  //## begin CCursorZoomOut::CCursorZoomOut%.initialization preserve=yes
  //## end CCursorZoomOut::CCursorZoomOut%.initialization
{
  //## begin CCursorZoomOut::CCursorZoomOut%.body preserve=yes
  //## end CCursorZoomOut::CCursorZoomOut%.body
}

CCursorZoomOut::CCursorZoomOut (CToolBase* pCToolBase)
  //## begin CCursorZoomOut::CCursorZoomOut%916354665.hasinit preserve=no
  //## end CCursorZoomOut::CCursorZoomOut%916354665.hasinit
  //## begin CCursorZoomOut::CCursorZoomOut%916354665.initialization preserve=yes
  //## end CCursorZoomOut::CCursorZoomOut%916354665.initialization
{
  //## begin CCursorZoomOut::CCursorZoomOut%916354665.body preserve=yes
	m_pCToolBase	= pCToolBase;
  //## end CCursorZoomOut::CCursorZoomOut%916354665.body
}


CCursorZoomOut::~CCursorZoomOut()
{
  //## begin CCursorZoomOut::~CCursorZoomOut%.body preserve=yes
  //## end CCursorZoomOut::~CCursorZoomOut%.body
}



//## Other Operations (implementation)
BOOL CCursorZoomOut::CursorInside (int x, int y)
{
  //## begin CCursorZoomOut::CursorInside%916354663.body preserve=yes

	BOOL bRet;
	ASSERT(m_pCToolBase!=NULL);
	ASSERT(((CToolBase*)m_pCToolBase)->m_pCBmpViewerData != NULL);
	if (( ((CToolBase*)m_pCToolBase)->m_pCBmpViewerData->IsCursorInsideImage(x, y)))
	{
//		if ( get_m_nCurrentCCursor() != IDC_CURSOR1 )
		{
			SetCursor( LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ZOOMOUT) ) );
			ShowCursor( TRUE); 
 			set_m_nCurrentCCursor(IDC_ZOOMOUT);
		}
		bRet	= TRUE;
	}
	else
	{
		bRet	= FALSE;
	}
	return bRet;
	
  //## end CCursorZoomOut::CursorInside%916354663.body
}

BOOL CCursorZoomOut::CursorOutside (int x, int y)
{
  //## begin CCursorZoomOut::CursorOutside%916354664.body preserve=yes
	
	BOOL bRet;

	if ( !(((CToolBase*)m_pCToolBase)->m_pCBmpViewerData->IsCursorInsideImage(x, y)))
	{
//		if ( get_m_nCurrentCCursor() != IDC_POINTER )
		{
			SetCursor( LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_POINTER)) );
			set_m_nCurrentCCursor( IDC_POINTER );
			ShowCursor( TRUE); 
		}
		bRet	= TRUE;
	}
	else
	{
		bRet	= FALSE;
	}
	return bRet;

  //## end CCursorZoomOut::CursorOutside%916354664.body
}

// Additional Declarations
  //## begin CCursorZoomOut%369E45E501BE.declarations preserve=yes
  //## end CCursorZoomOut%369E45E501BE.declarations

//## begin module%369E45E501BE.epilog preserve=yes
//## end module%369E45E501BE.epilog
