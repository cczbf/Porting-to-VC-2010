//## begin module%3FD56FFD02D0.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FD56FFD02D0.cm

//## begin module%3FD56FFD02D0.cp preserve=no
//## end module%3FD56FFD02D0.cp

//## Module: CL%3FD56FFD02D0; Pseudo Package body
//## Source file: C:\zbf\Research\3dViewer\CL.cpp

//## begin module%3FD56FFD02D0.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%3FD56FFD02D0.additionalIncludes

//## begin module%3FD56FFD02D0.includes preserve=yes
#include <float.h>
#include <math.h>

//## end module%3FD56FFD02D0.includes

// CL
#include "CL.h"
//## begin module%3FD56FFD02D0.additionalDeclarations preserve=yes
//## end module%3FD56FFD02D0.additionalDeclarations


// Class CL 


CL::CL()
  //## begin CL::CL%.hasinit preserve=no
  //## end CL::CL%.hasinit
  //## begin CL::CL%.initialization preserve=yes
  //## end CL::CL%.initialization
{
  //## begin CL::CL%.body preserve=yes
  //## end CL::CL%.body
}


CL::~CL()
{
  //## begin CL::~CL%.body preserve=yes
  //## end CL::~CL%.body
}



//## Other Operations (implementation)
void CL::Cal (CL0* pCL0, CL0* pCL1)
{
  //## begin CL::Cal%3FD5EFE1028D.body preserve=yes
	int i, j;

	for (i=0; i< this->m_nRowDim; i++)
		for ( j = 0; j< this->m_nColDim; j++)
		{
			if ( i != j)
				(*this)[i][j] = (float)fabs((*pCL0)[i][j] - (*pCL1)[i][j]);
			else
				(*this)[i][j] = FLT_MIN;
		}

//		float *t = (float*)malloc(sizeof(float)*this->m_nRowDim);
		for (i=0; i< this->m_nRowDim; i++)
		{
		for ( j = 0; j< this->m_nColDim; j++)
		{
            	(*this)[i][i] = max((*this)[i][i],(*this)[i][j]);
		}
//		t[i] =	(*this)[i][i];
//		printf("%f ", t[i] );
		}
//	printf("\n" );
	return;
  //## end CL::Cal%3FD5EFE1028D.body
}

// Additional Declarations
  //## begin CL%3FD56FFD02D0.declarations preserve=yes
  //## end CL%3FD56FFD02D0.declarations

//## begin module%3FD56FFD02D0.epilog preserve=yes
//## end module%3FD56FFD02D0.epilog
