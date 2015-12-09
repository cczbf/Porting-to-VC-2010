//## begin module%369E45C80194.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E45C80194.cm

//## begin module%369E45C80194.cp preserve=no
//## end module%369E45C80194.cp

//## Module: CCursorZoomIn%369E45C80194; Pseudo Package body
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CCursorZoomIn.cpp

//## begin module%369E45C80194.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%369E45C80194.additionalIncludes

//## begin module%369E45C80194.includes preserve=yes
//## end module%369E45C80194.includes

// CCursorZoomIn
#include "CCursorZoomIn.h"
//## begin module%369E45C80194.additionalDeclarations preserve=yes
//## end module%369E45C80194.additionalDeclarations


// Class CCursorZoomIn 

CCursorZoomIn::CCursorZoomIn()
  //## begin CCursorZoomIn::CCursorZoomIn%.hasinit preserve=no
  //## end CCursorZoomIn::CCursorZoomIn%.hasinit
  //## begin CCursorZoomIn::CCursorZoomIn%.initialization preserve=yes
  //## end CCursorZoomIn::CCursorZoomIn%.initialization
{
  //## begin CCursorZoomIn::CCursorZoomIn%.body preserve=yes
  //## end CCursorZoomIn::CCursorZoomIn%.body
}

CCursorZoomIn::CCursorZoomIn (CToolBase* pCToolBase)
  //## begin CCursorZoomIn::CCursorZoomIn%916354668.hasinit preserve=no
  //## end CCursorZoomIn::CCursorZoomIn%916354668.hasinit
  //## begin CCursorZoomIn::CCursorZoomIn%916354668.initialization preserve=yes
  //## end CCursorZoomIn::CCursorZoomIn%916354668.initialization
{
  //## begin CCursorZoomIn::CCursorZoomIn%916354668.body preserve=yes
	m_pCToolBase	= pCToolBase;
	//## end CCursorZoomIn::CCursorZoomIn%916354668.body
}


CCursorZoomIn::~CCursorZoomIn()
{
  //## begin CCursorZoomIn::~CCursorZoomIn%.body preserve=yes
  //## end CCursorZoomIn::~CCursorZoomIn%.body
}



//## Other Operations (implementation)
BOOL CCursorZoomIn::CursorInside (int x, int y)
{
  //## begin CCursorZoomIn::CursorInside%916354666.body preserve=yes

	BOOL bRet;
	ASSERT(m_pCToolBase!=NULL);
	ASSERT(((CToolBase*)m_pCToolBase)->m_pCBmpViewerData != NULL);
	if (( ((CToolBase*)m_pCToolBase)->m_pCBmpViewerData->IsCursorInsideImage(x, y)))
	{
//		if ( get_m_nCurrentCCursor() != IDC_CURSOR1 )
		{
			SetCursor( LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ZOOMIN) ) );
			ShowCursor( TRUE); 
 			set_m_nCurrentCCursor(IDC_ZOOMIN);
		}
		bRet	= TRUE;
	}
	else
	{
		bRet	= FALSE;
	}
	return bRet;
  //## end CCursorZoomIn::CursorInside%916354666.body
}

BOOL CCursorZoomIn::CursorOutside (int x, int y)
{
  //## begin CCursorZoomIn::CursorOutside%916354667.body preserve=yes

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

  //## end CCursorZoomIn::CursorOutside%916354667.body
}

// Additional Declarations
  //## begin CCursorZoomIn%369E45C80194.declarations preserve=yes
  //## end CCursorZoomIn%369E45C80194.declarations

//## begin module%369E45C80194.epilog preserve=yes
//## end module%369E45C80194.epilog
