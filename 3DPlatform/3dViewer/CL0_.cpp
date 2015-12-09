//## begin module%3FD56F10016F.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FD56F10016F.cm

//## begin module%3FD56F10016F.cp preserve=no
//## end module%3FD56F10016F.cp

//## Module: CL0%3FD56F10016F; Pseudo Package body
//## Source file: C:\zbf\Research\Spherical Para\Foundmentals of SP\fsp\CL0.cpp

//## begin module%3FD56F10016F.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%3FD56F10016F.additionalIncludes

//## begin module%3FD56F10016F.includes preserve=yes
#include <math.h>
#include <float.h>

//## end module%3FD56F10016F.includes

// CL0
#include "CL0_.h"
//## begin module%3FD56F10016F.additionalDeclarations preserve=yes
//## end module%3FD56F10016F.additionalDeclarations


// Class CL0 







CL0_::CL0_()
  //## begin CL0::CL0%.hasinit preserve=no
      : m_pVertexX(NULL), m_pVertexY(NULL), m_pVertexZ(NULL)
  //## end CL0::CL0%.hasinit
  //## begin CL0::CL0%.initialization preserve=yes
  //## end CL0::CL0%.initialization
{
  //## begin CL0::CL0%.body preserve=yes
	this->m_pCLw			= NULL;
  //## end CL0::CL0%.body
}


CL0_::~CL0_()
{
  //## begin CL0::~CL0%.body preserve=yes
  //## end CL0::~CL0%.body
}



//## Other Operations (implementation)
double CL0_::Distance (int i, int j)
{
  //## begin CL0::Distance%3FD58C410109.body preserve=yes

	double d =	(m_pVertexX[i] - m_pVertexX[j])*(m_pVertexX[i] - m_pVertexX[j]) + 
				(m_pVertexY[i] - m_pVertexY[j])*(m_pVertexY[i] - m_pVertexY[j]) +
				(m_pVertexZ[i] - m_pVertexZ[j])*(m_pVertexZ[i] - m_pVertexZ[j]);
	d = sqrt (d);
	return (d);


  //## end CL0::Distance%3FD58C410109.body
}

void CL0_::Cal (CLw_* pCLw, float* x)
{
  //## begin CL0::Cal%3FD58D64003B.body preserve=yes

	this->m_pCLw			= pCLw;

	int nCol, nRow;
	this->GetDim(&nRow, &nCol);
//  this->SetDim(nRow, nCol);

	this->m_pVertexX = x+1;
	this->m_pVertexY = this->m_pVertexX + nCol;
	this->m_pVertexZ = this->m_pVertexY + nCol;

	float e0 = FLT_MIN;
	int i, j;

	for ( i = 0; i < this->m_nRowDim; i++ )
		for ( j =0; j < this->m_nColDim; j++)
	{
		if (( ((*(this->m_pCLw))[i][j]) != 0.0 ) && (i != j))
			e0 = (float)max(e0,(float)((*this)[i][j] = (float)this->Distance(i,j)));	
		else
			(*this)[i][j] = 0.0;
	}
	
	for ( i = 0; i < this->m_nRowDim; i++ )
		for ( j =0; j < this->m_nColDim; j++)
	{
		(*this)[i][j] = (*this)[i][j] / e0;	
	}
	

  //## end CL0::Cal%3FD58D64003B.body
}

void CL0_::Cal (CLw_* pCLw, CVertexArray_* pVtx)
{
  //## begin CL0::Cal%3FD6E464011B.body preserve=yes

	float *x	= (float*)malloc(sizeof(float)*(pCLw->m_nColDim*3+1));
	int i, j=1;

	for ( i = 0; i < pCLw->m_nColDim; i++)
	{
		x[j++] = (float)((*pVtx)[i]).dX;
	}
	for ( i = 0; i < pCLw->m_nColDim; i++)
	{
		x[j++] = (float)((*pVtx)[i]).dY;
	}
	for ( i = 0; i < pCLw->m_nColDim; i++)
	{
		x[j++] = (float)((*pVtx)[i]).dZ;
	}

	this->Cal(pCLw,x);
	free(x);
  //## end CL0::Cal%3FD6E464011B.body
}

// Additional Declarations
  //## begin CL0%3FD56F10016F.declarations preserve=yes
  //## end CL0%3FD56F10016F.declarations

//## begin module%3FD56F10016F.epilog preserve=yes
//## end module%3FD56F10016F.epilog
