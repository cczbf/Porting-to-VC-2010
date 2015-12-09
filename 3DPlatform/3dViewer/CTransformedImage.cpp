//## begin module%3CAA59290349.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CAA59290349.cm

//## begin module%3CAA59290349.cp preserve=no
//## end module%3CAA59290349.cp

//## Module: CTransformedImage%3CAA59290349; Package body
//## Subsystem: bmpviewer::Globals%38869EB800D6
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: D:\H\desktop\HalfToneTest\bmpviewer--nxn\CTransformedImage.cpp

//## begin module%3CAA59290349.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%3CAA59290349.additionalIncludes

//## begin module%3CAA59290349.includes preserve=yes
//## end module%3CAA59290349.includes

// CTransformedImage
#include "CTransformedImage.h"
//## begin module%3CAA59290349.declarations preserve=no
//## end module%3CAA59290349.declarations

//## begin module%3CAA59290349.additionalDeclarations preserve=yes
//## end module%3CAA59290349.additionalDeclarations


// Class CTransformedImage 






CTransformedImage::CTransformedImage()
  //## begin CTransformedImage::CTransformedImage%.hasinit preserve=no
      : m_lpData(NULL), m_nTransformFilterID(0)
  //## end CTransformedImage::CTransformedImage%.hasinit
  //## begin CTransformedImage::CTransformedImage%.initialization preserve=yes
  //## end CTransformedImage::CTransformedImage%.initialization
{
  //## begin CTransformedImage::CTransformedImage%.body preserve=yes
  //## end CTransformedImage::CTransformedImage%.body
}


CTransformedImage::~CTransformedImage ()
{
  //## begin CTransformedImage::~CTransformedImage%3CAA6A99021A.body preserve=yes
	if (m_lpData != NULL)
		free(m_lpData);
	m_lpData	= NULL;

  //## end CTransformedImage::~CTransformedImage%3CAA6A99021A.body
}



//## Other Operations (implementation)
void CTransformedImage::SetData (float* lpData, __int32 nW, __int32 nH)
{
  //## begin CTransformedImage::SetData%3CAA66570291.body preserve=yes
	if (m_lpData != NULL)
		free(m_lpData);
	m_lpData = (float*)malloc( sizeof(float)*nW*nH );
	for ( int i = 0; i < nW; i++)
		for ( int j = 0; j < nH; j++)
			m_lpData[j*nW + i] = lpData[j*nW + i];
	m_nW = nW;
	m_nH = nH;

  //## end CTransformedImage::SetData%3CAA66570291.body
}

void CTransformedImage::GetData (float* lpData, __int32* nW, __int32* nH)
{
  //## begin CTransformedImage::GetData%3CAA69EB0334.body preserve=yes
	for ( int i = 0; i < m_nW; i++)
		for ( int j = 0; j < m_nH; j++)
			lpData[j*m_nW + i] = m_lpData[j*m_nW + i];
	*nW = m_nW;
	*nH = m_nH;

  //## end CTransformedImage::GetData%3CAA69EB0334.body
}

float* CTransformedImage::GetData (__int32* nW, __int32* nH)
{
  //## begin CTransformedImage::GetData%3CAACC3F03BF.body preserve=yes
	*nW = m_nW;
	*nH = m_nH;
    return (m_lpData);

  //## end CTransformedImage::GetData%3CAACC3F03BF.body
}

// Additional Declarations
  //## begin CTransformedImage%3CAA5889035E.declarations preserve=yes
  //## end CTransformedImage%3CAA5889035E.declarations

//## begin module%3CAA59290349.epilog preserve=yes
//## end module%3CAA59290349.epilog
