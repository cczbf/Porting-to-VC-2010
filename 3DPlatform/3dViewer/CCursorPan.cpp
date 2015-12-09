//## begin module%3650E90500A0.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3650E90500A0.cm

//## begin module%3650E90500A0.cp preserve=no
//## end module%3650E90500A0.cp

//## Module: CCursorPan%3650E90500A0; Pseudo Package body
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CCursorPan.cpp

//## begin module%3650E90500A0.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%3650E90500A0.additionalIncludes

//## begin module%3650E90500A0.includes preserve=yes
//## end module%3650E90500A0.includes

// CCursorPan
#include "CCursorPan.h"
//## begin module%3650E90500A0.additionalDeclarations preserve=yes
#include "resource.h"
//## end module%3650E90500A0.additionalDeclarations


// Class CCursorPan 


CCursorPan::CCursorPan ()
  //## begin CCursorPan::CCursorPan%911272804.hasinit preserve=no
  //## end CCursorPan::CCursorPan%911272804.hasinit
  //## begin CCursorPan::CCursorPan%911272804.initialization preserve=yes
  //## end CCursorPan::CCursorPan%911272804.initialization
{
  //## begin CCursorPan::CCursorPan%911272804.body preserve=yes
  //## end CCursorPan::CCursorPan%911272804.body
}

CCursorPan::CCursorPan (CToolBase* pCToolBase)
  //## begin CCursorPan::CCursorPan%911272810.hasinit preserve=no
  //## end CCursorPan::CCursorPan%911272810.hasinit
  //## begin CCursorPan::CCursorPan%911272810.initialization preserve=yes
  //## end CCursorPan::CCursorPan%911272810.initialization
{
  //## begin CCursorPan::CCursorPan%911272810.body preserve=yes
	m_pCToolBase	= pCToolBase;
  //## end CCursorPan::CCursorPan%911272810.body
}



//## Other Operations (implementation)
BOOL CCursorPan::CursorInside (int x, int y)
{
  //## begin CCursorPan::CursorInside%911272802.body preserve=yes
	BOOL bRet;
	ASSERT(m_pCToolBase!=NULL);
	ASSERT(((CToolBase*)m_pCToolBase)->m_pCBmpViewerData != NULL);
	if (( ((CToolBase*)m_pCToolBase)->m_pCBmpViewerData->IsCursorInsideImage(x, y)))
	{
//		if ( get_m_nCurrentCCursor() != IDC_CURSOR1 )
		{
			SetCursor( LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR1) ) );
			ShowCursor( TRUE); 
 			set_m_nCurrentCCursor(IDC_CURSOR1);
		}
		bRet	= TRUE;
	}
	else
	{
		bRet	= FALSE;
	}
	return bRet;
  //## end CCursorPan::CursorInside%911272802.body
}

BOOL CCursorPan::CursorOutside (int x, int y)
{
  //## begin CCursorPan::CursorOutside%911272803.body preserve=yes
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
  //## end CCursorPan::CursorOutside%911272803.body
}

// Additional Declarations
  //## begin CCursorPan%3650E90500A0.declarations preserve=yes
  //## end CCursorPan%3650E90500A0.declarations

//## begin module%3650E90500A0.epilog preserve=yes
//## end module%3650E90500A0.epilog
