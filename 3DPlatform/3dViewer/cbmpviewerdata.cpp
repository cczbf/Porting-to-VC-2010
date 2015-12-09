//## begin module%364BA69102A8.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%364BA69102A8.cm

//## begin module%364BA69102A8.cp preserve=no
//## end module%364BA69102A8.cp

//## Module: CBmpViewerData%364BA69102A8; Pseudo Package body
//## Subsystem: bmpviewer::bmpviewer\:\:Core%387016280189
//## Source file: d:\desktop\halftonetest\bmpviewer\CBmpViewerData.cpp

//## begin module%364BA69102A8.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%364BA69102A8.additionalIncludes

//## begin module%364BA69102A8.includes preserve=yes
//## end module%364BA69102A8.includes

// CBmpViewerData
#include "CBmpViewerData.h"
//## begin module%364BA69102A8.additionalDeclarations preserve=yes
//## end module%364BA69102A8.additionalDeclarations


// Class CBmpViewerData 













CBmpViewerData::CBmpViewerData()
  //## begin CBmpViewerData::CBmpViewerData%.hasinit preserve=no
      : m_x0(0), m_y0(0), m_w0(0), m_h0(0), m_x1(0), m_y1(0), m_w1(0), m_h1(0), m_hDib(NULL), m_hDibUndo(NULL)
  //## end CBmpViewerData::CBmpViewerData%.hasinit
  //## begin CBmpViewerData::CBmpViewerData%.initialization preserve=yes
  //## end CBmpViewerData::CBmpViewerData%.initialization
{
  //## begin CBmpViewerData::CBmpViewerData%.body preserve=yes

	m_bSelected	= FALSE;
 
  //## end CBmpViewerData::CBmpViewerData%.body
}


CBmpViewerData::~CBmpViewerData()
{
  //## begin CBmpViewerData::~CBmpViewerData%.body preserve=yes


	SetDIBHandle(NULL);
	SetUndoHandle(NULL);

  //## end CBmpViewerData::~CBmpViewerData%.body
}



//## Other Operations (implementation)
HANDLE CBmpViewerData::SetDIBHandle (HANDLE hDib, int w, int h)
{
  //## begin CBmpViewerData::SetDIBHandle%364BA69300E0.body preserve=yes
	if( m_hDib!=NULL)
	{
//		GlobalFree( m_hDib );
		this->SetUndoHandle(m_hDib);
	}
	m_hDib	= hDib;
	if ( m_hDib!=NULL )
	{ 
		if( (w == 0)||(h== 0))
		{
			CDIB CDIB0;
			w = CDIB0.DIBWidth( m_hDib );	
			h = CDIB0.DIBHeight( m_hDib );
		}
		m_h1	= h;		m_w1	= w;//They are Image Size

		if ( !(m_h0&&m_w0) )// One of them is Zero -- 2000.1.11, by Frank
		{
				m_h0	= h;		
				m_w0	= w;//They are display size
		}
	}
	return m_hDib;
  //## end CBmpViewerData::SetDIBHandle%364BA69300E0.body
}

BOOL CBmpViewerData::IsCursorInsideImage (int x, int y)
{
  //## begin CBmpViewerData::IsCursorInsideImage%364F7090029E.body preserve=yes
	BOOL bRet	= (	(x>=m_x0)&&
					(x<=(m_x0+m_w0))&&
					(y>=m_y0)&&
					(y<=(m_y0+m_h0))
					);
	return bRet;
  //## end CBmpViewerData::IsCursorInsideImage%364F7090029E.body
}

BOOL CBmpViewerData::SetSelFlag (BOOL bSelected)
{
  //## begin CBmpViewerData::SetSelFlag%364F71100398.body preserve=yes
	return (m_bSelected = bSelected);
  //## end CBmpViewerData::SetSelFlag%364F71100398.body
}

BOOL CBmpViewerData::GetSelFlag ()
{
  //## begin CBmpViewerData::GetSelFlag%364F715003C0.body preserve=yes
	return (m_bSelected);

  //## end CBmpViewerData::GetSelFlag%364F715003C0.body
}

void CBmpViewerData::SetNewPosition (int x0, int y0, int w0, int h0, int x1, int y1, int w1, int h1)
{
  //## begin CBmpViewerData::SetNewPosition%364F975A017C.body preserve=yes
  m_x0	= x0;
  m_y0	= y0;
  m_w0	= w0;
  m_h0	= h0;
  m_x1	= x1;
  m_y1	= y1;
  m_w1	= w1;
  m_h1	= h1;

  //## end CBmpViewerData::SetNewPosition%364F975A017C.body
}

void CBmpViewerData::GetNewPosition (int& x0, int& y0, int& w0, int& h0, int& x1, int& y1, int& w1, int& h1)
{
  //## begin CBmpViewerData::GetNewPosition%364F987C0302.body preserve=yes
  x0	= m_x0;
  y0	= m_y0;
  w0	= m_w0;
  h0	= m_h0;
  x1	= m_x1;
  y1	= m_y1;
  w1	= m_w1;
  h1	= m_h1;
  //## end CBmpViewerData::GetNewPosition%364F987C0302.body
}

HANDLE CBmpViewerData::GetDIBHandle ()
{
  //## begin CBmpViewerData::GetDIBHandle%3682E1DB0302.body preserve=yes
	return get_m_hDib();
  //## end CBmpViewerData::GetDIBHandle%3682E1DB0302.body
}

HANDLE CBmpViewerData::CopyDibRawHandle (int* w, int* h)
{
  //## begin CBmpViewerData::CopyDibRawHandle%387BD69F0048.body preserve=yes
	CDIB CDIB;
	return CDIB.Dib2BWRaw16 (this->GetDIBHandle(),w, h);
  //## end CBmpViewerData::CopyDibRawHandle%387BD69F0048.body
}

void CBmpViewerData::SetUndoHandle (HANDLE hDib)
{
  //## begin CBmpViewerData::SetUndoHandle%38AA62A3037C.body preserve=yes
	if( m_hDibUndo!=NULL)
	{
		GlobalFree( m_hDibUndo );
		m_hDibUndo	= NULL;
	}

	m_hDibUndo	= m_hDib;

  //## end CBmpViewerData::SetUndoHandle%38AA62A3037C.body
}

void CBmpViewerData::Undo ()
{
  //## begin CBmpViewerData::Undo%38AA6315004B.body preserve=yes
	if( m_hDibUndo!=NULL)
	{
		HANDLE hDib;
		CDIB CDIB;
		hDib	= CDIB.CopyDib(m_hDibUndo);
		this->SetDIBHandle(hDib);
	}

  //## end CBmpViewerData::Undo%38AA6315004B.body
}

// Additional Declarations
  //## begin CBmpViewerData%364BA69102A8.declarations preserve=yes
  //## end CBmpViewerData%364BA69102A8.declarations

//## begin module%364BA69102A8.epilog preserve=yes
//## end module%364BA69102A8.epilog
