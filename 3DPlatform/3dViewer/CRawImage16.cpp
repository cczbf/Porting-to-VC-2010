//## begin module%3875951801DC.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3875951801DC.cm

//## begin module%3875951801DC.cp preserve=no
//## end module%3875951801DC.cp

//## Module: CRawImage16%3875951801DC; Pseudo Package body
//## Source file: d:\desktop\halftonetest\bmpviewer\CRawImage16.cpp

//## begin module%3875951801DC.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%3875951801DC.additionalIncludes

//## begin module%3875951801DC.includes preserve=yes
//## end module%3875951801DC.includes
#include "math.h"
// CRawImage16
#include "CRawImage16.h"
//## begin module%3875951801DC.additionalDeclarations preserve=yes
//## end module%3875951801DC.additionalDeclarations


// Class CRawImage16 








CRawImage16::CRawImage16()
  //## begin CRawImage16::CRawImage16%.hasinit preserve=no
      : m_nWidth(256), m_nHeight(256), m_lpPixels(NULL), m_hPixels(NULL), m_nMaxPixelValue(255), m_nMinPixelValue(0)
  //## end CRawImage16::CRawImage16%.hasinit
  //## begin CRawImage16::CRawImage16%.initialization preserve=yes
  //## end CRawImage16::CRawImage16%.initialization
{
  //## begin CRawImage16::CRawImage16%.body preserve=yes
  //## end CRawImage16::CRawImage16%.body
}

CRawImage16::CRawImage16 (unsigned _int32 nWidth, unsigned _int32 nHeight)
  //## begin CRawImage16::CRawImage16%3879414A02F5.hasinit preserve=no
      : m_nWidth(256), m_nHeight(256), m_lpPixels(NULL), m_hPixels(NULL), m_nMaxPixelValue(255), m_nMinPixelValue(0)
  //## end CRawImage16::CRawImage16%3879414A02F5.hasinit
  //## begin CRawImage16::CRawImage16%3879414A02F5.initialization preserve=yes
  //## end CRawImage16::CRawImage16%3879414A02F5.initialization
{
  //## begin CRawImage16::CRawImage16%3879414A02F5.body preserve=yes
  //## end CRawImage16::CRawImage16%3879414A02F5.body
}


CRawImage16::~CRawImage16()
{
  //## begin CRawImage16::~CRawImage16%.body preserve=yes
	this->DestroyPixelBuffer();
  //## end CRawImage16::~CRawImage16%.body
}



//## Other Operations (implementation)
void CRawImage16::Copy (UINT *lpPixels)
{
  //## begin CRawImage16::Copy%38759640000B.body preserve=yes
  //## end CRawImage16::Copy%38759640000B.body
}

void CRawImage16::Copy (HANDLE hRaw, unsigned _int32 nW, unsigned _int32 nH)
{
  //## begin CRawImage16::Copy%387BF64A01B3.body preserve=yes


	if ( m_hPixels != NULL )
		if( nW != m_nWidth || nH != m_nHeight )
	{
		this->DestroyPixelBuffer();
		ASSERT (m_hPixels	== NULL);
		ASSERT (m_lpPixels	== NULL);
	}

	if ( m_hPixels == NULL )
	{
		m_nWidth	= nW;
		m_nHeight	= nH;

		m_hPixels = GlobalAlloc ( 
			GHND, 
			m_nWidth * m_nHeight * sizeof(unsigned _int16) 
			);
	}


	if ( m_lpPixels == NULL )
		if ( m_hPixels != NULL )
			m_lpPixels = (unsigned _int16*)GlobalLock (m_hPixels);

	if ( m_lpPixels != NULL )
	{
		int k = 0;
		unsigned _int16 *lpRaw = (unsigned _int16 *)GlobalLock( hRaw );

		memcpy( m_lpPixels, 
				lpRaw,  
				m_nWidth * m_nHeight * sizeof(unsigned _int16));
		GlobalUnlock(hRaw);
		GlobalUnlock(m_hPixels);
		m_lpPixels = NULL;
	}




  //## end CRawImage16::Copy%387BF64A01B3.body
}

void CRawImage16::Copy (char* szFileName)
{
  //## begin CRawImage16::Copy%38759C080257.body preserve=yes
  //## end CRawImage16::Copy%38759C080257.body
}

void CRawImage16::Copy (UINT nDensity, unsigned _int32 nW, unsigned _int32 nH)
{
  //## begin CRawImage16::Copy%38759D9901FA.body preserve=yes
	if ( m_hPixels != NULL )
		if( nW != m_nWidth || nH != m_nHeight )
	{
		this->DestroyPixelBuffer();
		ASSERT (m_hPixels	== NULL);
		ASSERT (m_lpPixels	== NULL);
	}

	if ( m_hPixels == NULL )
	{
		if ( (nW != 0) && (nH != 0))
		{
			m_nWidth	= nW;
			m_nHeight	= nH;
		}

		m_hPixels = GlobalAlloc ( 
			GHND, 
			m_nWidth * m_nHeight * sizeof(unsigned _int16) 
			);
	}


	if ( m_lpPixels == NULL )
		if ( m_hPixels != NULL )
			m_lpPixels = (unsigned _int16*)GlobalLock (m_hPixels);

	if ( m_lpPixels != NULL )
	{
		int k = 0;
		for ( unsigned _int32 j = 0; j < m_nHeight; j++ )
			for ( unsigned _int32 i = 0; i < m_nWidth; i++ )
		{
			m_lpPixels[k++]	= nDensity;
//			m_lpPixels[k++]	= (j*255)/m_nHeight;

		}
		GlobalUnlock(m_hPixels);
		m_lpPixels = NULL;
	}
  //## end CRawImage16::Copy%38759D9901FA.body
}

void CRawImage16::ExpandToPowerOf2Dim (unsigned _int32* nW, unsigned _int32* nH)
{
  //## begin CRawImage16::ExpandToPowerOf2Dim%3CAAEC53022A.body preserve=yes
	unsigned _int32 m,n;
	m = (long)(log((double)m_nWidth)/log(2.0));
	if (((1<<m)-m_nWidth)!=0)
		m++;
	n = (long)(log((double)m_nHeight)/log(2.0));
	if (((1<<n)-m_nHeight)!=0)
		n++;
	m = (1<<m);
	n = (1<<n);

	if (m==m_nWidth)
		if (n==m_nHeight)
		return;

	HANDLE hPixels = m_hPixels;
	unsigned _int32 w,h;
	w = m_nWidth;
	h = m_nHeight;
	unsigned __int16 *lpPixels = (unsigned __int16 *)GlobalLock(hPixels);
	m_hPixels = NULL;

	this->Copy ((UINT)0, m, n);
	*nW = m;
	*nH = n;
	m_lpPixels = (unsigned __int16 *)GlobalLock(m_hPixels);

	int i0,j0;
	for ( unsigned _int32 i = 0; i < n; i++ )
		for( unsigned _int32 j = 0; j <m; j++ )
	{
		if( i < h )
			i0 = i;
		else
			i0 = h - 1 - (i%h);

		if( j < w )
			j0 = j;
		else
			j0 = w - 1 - (j%w);

		m_lpPixels[i*m+j]= lpPixels[i0*w+j0];
	}

	GlobalUnlock(hPixels);
	GlobalFree(hPixels);

	GlobalUnlock(m_hPixels);
	m_lpPixels	= NULL;


  //## end CRawImage16::ExpandToPowerOf2Dim%3CAAEC53022A.body
}

void CRawImage16::DestroyPixelBuffer ()
{
  //## begin CRawImage16::DestroyPixelBuffer%387A83A50262.body preserve=yes
	if ( this->m_hPixels != NULL )
	{
		if( this->m_lpPixels != NULL )
		{
			GlobalUnlock(this->m_hPixels);
			this->m_lpPixels = NULL;
		}
		GlobalFree( this->m_hPixels );
		this->m_hPixels	= NULL;
	}

  //## end CRawImage16::DestroyPixelBuffer%387A83A50262.body
}

// Additional Declarations
  //## begin CRawImage16%3875951801DC.declarations preserve=yes
  //## end CRawImage16%3875951801DC.declarations

//## begin module%3875951801DC.epilog preserve=yes
//## end module%3875951801DC.epilog
