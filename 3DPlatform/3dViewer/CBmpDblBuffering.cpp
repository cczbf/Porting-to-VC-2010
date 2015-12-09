//## begin module%364FD6E4033E.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%364FD6E4033E.cm

//## begin module%364FD6E4033E.cp preserve=no
//## end module%364FD6E4033E.cp

//## Module: CBmpDblBuffering%364FD6E4033E; Pseudo Package body
//## Subsystem: bmpviewer%364BBA5B0050
//## Source file: c:\windows\desktop\roselecture\bmpviewer\CBmpDblBuffering.cpp

//## begin module%364FD6E4033E.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%364FD6E4033E.additionalIncludes

//## begin module%364FD6E4033E.includes preserve=yes
//## end module%364FD6E4033E.includes

// CBmpDblBuffering
#include "CBmpDblBuffering.h"
//## begin module%364FD6E4033E.additionalDeclarations preserve=yes
//## end module%364FD6E4033E.additionalDeclarations


// Class CBmpDblBuffering 





CBmpDblBuffering::CBmpDblBuffering (HBITMAP hBmpBackBuf)
  //## begin CBmpDblBuffering::CBmpDblBuffering%911203803.hasinit preserve=no
      : m_hDestDC(NULL), m_hMemDC(NULL), m_hBmpBackBuf(NULL)
  //## end CBmpDblBuffering::CBmpDblBuffering%911203803.hasinit
  //## begin CBmpDblBuffering::CBmpDblBuffering%911203803.initialization preserve=yes
  //## end CBmpDblBuffering::CBmpDblBuffering%911203803.initialization
{
  //## begin CBmpDblBuffering::CBmpDblBuffering%911203803.body preserve=yes
	
	if ( hBmpBackBuf != NULL )
		m_hBmpBackBuf	= hBmpBackBuf;
	else
	{
		HDC hDC	= ::GetDC(NULL);
		m_hBmpBackBuf	= CreateCompatibleBitmap( hDC, 1024,1024 ); 
		ReleaseDC( NULL, hDC ); 
	}

  //## end CBmpDblBuffering::CBmpDblBuffering%911203803.body
}


CBmpDblBuffering::~CBmpDblBuffering ()
{
  //## begin CBmpDblBuffering::~CBmpDblBuffering%911203804.body preserve=yes
//	SelectObject ( m_hMemDC, m_hBmpBackBuf );
    if (m_hMemDC!=NULL)
	{
		DeleteDC( m_hMemDC ); 
		m_hMemDC	= NULL;
	}
	
	ASSERT( m_hDestDC == NULL );
	
	if (m_hBmpBackBuf != NULL )
	{
		DeleteObject(m_hBmpBackBuf);
		m_hBmpBackBuf	= NULL;
	}
  //## end CBmpDblBuffering::~CBmpDblBuffering%911203804.body
}



//## Other Operations (implementation)
HDC CBmpDblBuffering::BeginDblBuf (HDC hDestDC, HBITMAP hBmpBackBuf)
{
  //## begin CBmpDblBuffering::BeginDblBuf%911203808.body preserve=yes
	m_hMemDC	= CreateCompatibleDC ( hDestDC );
	if ( hBmpBackBuf == NULL )
	{
		m_hBmpBackBuf = (HBITMAP)SelectObject( m_hMemDC, m_hBmpBackBuf );
	}
	else
	{
		DeleteObject ( m_hBmpBackBuf );
		m_hBmpBackBuf = (HBITMAP)SelectObject( m_hMemDC, hBmpBackBuf );
	}
	m_hDestDC	= hDestDC;

	BitBlt( m_hMemDC, 0,0, 1024, 1024, 
			NULL,  0,0, WHITENESS);

	return m_hMemDC;
  //## end CBmpDblBuffering::BeginDblBuf%911203808.body
}

HDC CBmpDblBuffering::GetDC ()
{
  //## begin CBmpDblBuffering::GetDC%911203807.body preserve=yes
	return ( m_hMemDC );
  //## end CBmpDblBuffering::GetDC%911203807.body
}

void CBmpDblBuffering::EndDblBuf ()
{
  //## begin CBmpDblBuffering::EndDblBuf%911203809.body preserve=yes
	m_hBmpBackBuf	= (HBITMAP)SelectObject ( m_hMemDC, m_hBmpBackBuf );
	DeleteDC( m_hMemDC ); 
	m_hMemDC	= NULL;
	m_hDestDC	= NULL;

  //## end CBmpDblBuffering::EndDblBuf%911203809.body
}

void CBmpDblBuffering::Show ()
{
  //## begin CBmpDblBuffering::Show%911203806.body preserve=yes

	ASSERT(m_hDestDC!=NULL);
	ASSERT(m_hMemDC!=NULL);

	BitBlt( m_hDestDC, 0,0, 1024, 1024, 
			m_hMemDC,  0,0, SRCCOPY);

  //## end CBmpDblBuffering::Show%911203806.body
}

// Additional Declarations
  //## begin CBmpDblBuffering%364FD6E4033E.declarations preserve=yes
  //## end CBmpDblBuffering%364FD6E4033E.declarations

//## begin module%364FD6E4033E.epilog preserve=yes
//## end module%364FD6E4033E.epilog
