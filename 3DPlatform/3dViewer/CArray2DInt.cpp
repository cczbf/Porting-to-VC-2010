//## begin module%41353EA801EB.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%41353EA801EB.cm

//## begin module%41353EA801EB.cp preserve=no
//## end module%41353EA801EB.cp

//## Module: CArray2DInt%41353EA801EB; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CArray2DInt.cpp

//## begin module%41353EA801EB.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%41353EA801EB.additionalIncludes

//## begin module%41353EA801EB.includes preserve=yes
#include "progressbar.h"
//## end module%41353EA801EB.includes

// CArray2DInt
#include "CArray2DInt.h"
//## begin module%41353EA801EB.declarations preserve=no
//## end module%41353EA801EB.declarations

//## begin module%41353EA801EB.additionalDeclarations preserve=yes
//## end module%41353EA801EB.additionalDeclarations



// Class CArray2DFLT 



CArray2DFLT::CArray2DFLT()
  //## begin CArray2DFLT::CArray2DFLT%.hasinit preserve=no
      : m_nRowDim(0), m_nColDim(9)
  //## end CArray2DFLT::CArray2DFLT%.hasinit
  //## begin CArray2DFLT::CArray2DFLT%.initialization preserve=yes
  //## end CArray2DFLT::CArray2DFLT%.initialization
{
  //## begin CArray2DFLT::CArray2DFLT%.body preserve=yes
  //## end CArray2DFLT::CArray2DFLT%.body
}


CArray2DFLT::~CArray2DFLT()
{
  //## begin CArray2DFLT::~CArray2DFLT%.body preserve=yes
  //## end CArray2DFLT::~CArray2DFLT%.body
}



//## Other Operations (implementation)
void CArray2DFLT::CArray2DInt (int nRowDim, int nColDim)
{
  //## begin CArray2DFLT::CArray2DInt%3FCA99EC0156.body preserve=yes
  //## end CArray2DFLT::CArray2DInt%3FCA99EC0156.body
}

void CArray2DFLT::GetDim (int* nRow, int* nCol)
{
  //## begin CArray2DFLT::GetDim%3FCAB05D03DF.body preserve=yes
	*nRow	= this->m_nRowDim;
	*nCol	= this->m_nColDim;
  //## end CArray2DFLT::GetDim%3FCAB05D03DF.body
}

void CArray2DFLT::SetDim (int nRow, int nCol)
{
  //## begin CArray2DFLT::SetDim%3FCAB2FA01AB.body preserve=yes
	this->SetSize(nRow);
	
    CProgressBar * pbar = NULL;
    pbar = new CProgressBar(_T("Progress"), 100, 100, TRUE);
    pbar->SetText("Set Dimension...");
    float step = 0;
	float prog = 0;
	step = float(100.0)/(float)(nRow);
	
	for (int i = 0; i < nRow; i++ )
	{
		prog += step;
		pbar->SetPos((int)prog);
	
		(*this)[i].SetSize(nCol);
	}
	delete pbar;

	this->m_nColDim	= nCol;
	this->m_nRowDim	= nRow;

  //## end CArray2DFLT::SetDim%3FCAB2FA01AB.body
}

// Additional Declarations
  //## begin CArray2DFLT%3FCA9785023C.declarations preserve=yes
  //## end CArray2DFLT%3FCA9785023C.declarations

//## begin module%41353EA801EB.epilog preserve=yes
//## end module%41353EA801EB.epilog
