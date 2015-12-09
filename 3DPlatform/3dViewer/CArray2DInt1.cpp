//## begin module%3FCA9785023C.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FCA9785023C.cm

//## begin module%3FCA9785023C.cp preserve=no
//## end module%3FCA9785023C.cp

//## Module: CArray2DInt%3FCA9785023C; Pseudo Package body
//## Source file: C:\zbf\Research\Spherical Para\Foundmentals of SP\fsp\CArray2DInt.cpp

//## begin module%3FCA9785023C.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%3FCA9785023C.additionalIncludes

//## begin module%3FCA9785023C.includes preserve=yes
//## end module%3FCA9785023C.includes

// CArray2DInt
#include "CArray2DInt1.h"
//## begin module%3FCA9785023C.additionalDeclarations preserve=yes
//## end module%3FCA9785023C.additionalDeclarations


// Class CArray2DInt 



CArray2DInt::CArray2DInt()
  //## begin CArray2DInt::CArray2DInt%.hasinit preserve=no
      : m_nRowDim(0), m_nColDim(0)
  //## end CArray2DInt::CArray2DInt%.hasinit
  //## begin CArray2DInt::CArray2DInt%.initialization preserve=yes
  //## end CArray2DInt::CArray2DInt%.initialization
{
  //## begin CArray2DInt::CArray2DInt%.body preserve=yes
  //## end CArray2DInt::CArray2DInt%.body
}

CArray2DInt::CArray2DInt (int nRowDim, int nColDim)
  //## begin CArray2DInt::CArray2DInt%3FCA99EC0156.hasinit preserve=no
      : m_nRowDim(0), m_nColDim(0)
  //## end CArray2DInt::CArray2DInt%3FCA99EC0156.hasinit
  //## begin CArray2DInt::CArray2DInt%3FCA99EC0156.initialization preserve=yes
  //## end CArray2DInt::CArray2DInt%3FCA99EC0156.initialization
{
  //## begin CArray2DInt::CArray2DInt%3FCA99EC0156.body preserve=yes
  //## end CArray2DInt::CArray2DInt%3FCA99EC0156.body
}


CArray2DInt::~CArray2DInt()
{
  //## begin CArray2DInt::~CArray2DInt%.body preserve=yes
  //## end CArray2DInt::~CArray2DInt%.body
}



//## Other Operations (implementation)
void CArray2DInt::GetDim (int* nRow, int* nCol)
{
  //## begin CArray2DInt::GetDim%3FCAB05D03DF.body preserve=yes
	*nRow	= this->m_nRowDim;
	*nCol	= this->m_nColDim;
  //## end CArray2DInt::GetDim%3FCAB05D03DF.body
}

void CArray2DInt::SetDim (int nRow, int nCol)
{
  //## begin CArray2DInt::SetDim%3FCAB2FA01AB.body preserve=yes
	this->SetSize(nRow);
	for (int i = 0; i < nRow; i++ )
		(*this)[i].SetSize(nCol);
	this->m_nColDim	= nCol;
	this->m_nRowDim	= nRow;

  //## end CArray2DInt::SetDim%3FCAB2FA01AB.body
}

// Additional Declarations
  //## begin CArray2DInt%3FCA9785023C.declarations preserve=yes
  //## end CArray2DInt%3FCA9785023C.declarations

//## begin module%3FCA9785023C.epilog preserve=yes
//## end module%3FCA9785023C.epilog
