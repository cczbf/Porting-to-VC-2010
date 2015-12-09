 //## begin module%418B2B360125.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%418B2B360125.cm

//## begin module%418B2B360125.cp preserve=no
//## end module%418B2B360125.cp

//## Module: BitAdjacentMatrix%418B2B360125; Pseudo Package body
//## Source file: d:\Program Files\Rational\Rose 98i\C++\source\BitAdjacentMatrix.cpp

//## begin module%418B2B360125.additionalIncludes preserve=no
//## end module%418B2B360125.additionalIncludes

//## begin module%418B2B360125.includes preserve=yes
#include "stdafx.h"
//## end module%418B2B360125.includes

// BitAdjacentMatrix
#include "BitAdjacentMatrix.h"
//## begin module%418B2B360125.additionalDeclarations preserve=yes
//## end module%418B2B360125.additionalDeclarations


// Class BitAdjacentMatrix 





BitAdjacentMatrix::BitAdjacentMatrix()
  //## begin BitAdjacentMatrix::BitAdjacentMatrix%.hasinit preserve=no
  //## end BitAdjacentMatrix::BitAdjacentMatrix%.hasinit
  //## begin BitAdjacentMatrix::BitAdjacentMatrix%.initialization preserve=yes
  //## end BitAdjacentMatrix::BitAdjacentMatrix%.initialization
{
  //## begin BitAdjacentMatrix::BitAdjacentMatrix%.body preserve=yes
	m_pBytes = NULL;
	m_nHei = 0;
	m_nWid = 0;


  //## end BitAdjacentMatrix::BitAdjacentMatrix%.body
}

BitAdjacentMatrix::BitAdjacentMatrix(const BitAdjacentMatrix &right)
  //## begin BitAdjacentMatrix::BitAdjacentMatrix%copy.hasinit preserve=no
  //## end BitAdjacentMatrix::BitAdjacentMatrix%copy.hasinit
  //## begin BitAdjacentMatrix::BitAdjacentMatrix%copy.initialization preserve=yes
  //## end BitAdjacentMatrix::BitAdjacentMatrix%copy.initialization
{
  //## begin BitAdjacentMatrix::BitAdjacentMatrix%copy.body preserve=yes
	
  //## end BitAdjacentMatrix::BitAdjacentMatrix%copy.body
}


BitAdjacentMatrix::~BitAdjacentMatrix()
{
  //## begin BitAdjacentMatrix::~BitAdjacentMatrix%.body preserve=yes
	if(m_pBytes != NULL)
		delete m_pBytes;
  //## end BitAdjacentMatrix::~BitAdjacentMatrix%.body
}


BitAdjacentMatrix & BitAdjacentMatrix::operator=(const BitAdjacentMatrix &right)
{
  //## begin BitAdjacentMatrix::operator=%.body preserve=yes

	return *this;
	
  //## end BitAdjacentMatrix::operator=%.body
}


int BitAdjacentMatrix::operator==(const BitAdjacentMatrix &right) const
{
  //## begin BitAdjacentMatrix::operator==%.body preserve=yes
	return 1;
  //## end BitAdjacentMatrix::operator==%.body
}

int BitAdjacentMatrix::operator!=(const BitAdjacentMatrix &right) const
{
  //## begin BitAdjacentMatrix::operator!=%.body preserve=yes
	return 1;
  //## end BitAdjacentMatrix::operator!=%.body
}

//---------------------------------------------------
//Name: SetPos()
//Desc: Set the the bit of ( row=j col=i ) to 1 
//---------------------------------------------------
//## Other Operations (implementation)
void BitAdjacentMatrix::SetPos (int i, int j)
{
  //## begin BitAdjacentMatrix::SetPos%418B301300EA.body preserve=yes
  
	int index = j*m_nBytesOfRow + i/8; 
	int nOff = i%8;
	m_pBytes[index] |= 128 >> nOff;

  //## end BitAdjacentMatrix::SetPos%418B301300EA.body
}

bool BitAdjacentMatrix::IsPosAdjacent (int i, int j)
{
  //## begin BitAdjacentMatrix::IsPosAdjacent%418B305C02CB.body preserve=yes

	int index = j*m_nBytesOfRow + i/8; 
	int nOff = i%8;

	bool bIsTrue = TRUE;
	if ( ( m_pBytes[index] & (128>>nOff) ) > 0 )
		bIsTrue = TRUE;
	else
		bIsTrue = FALSE;

	return bIsTrue;
  //## end BitAdjacentMatrix::IsPosAdjacent%418B305C02CB.body
}

bool BitAdjacentMatrix::Init (LONG wid, LONG hei)
{
  //## begin BitAdjacentMatrix::Init%418B319F0074.body preserve=yes
	m_nWid = wid;
	m_nHei = hei;
	m_nBytesOfRow = wid/8 + 1;

	if(m_pBytes != NULL)
	{
		delete m_pBytes;
		m_pBytes = NULL;
	}

	double dHei = hei;
	double dBytesOfRow = m_nBytesOfRow;

	LONG nMb = (LONG)( (dHei * dBytesOfRow) / (1024 * 1024) );

	//if the memory is more than 100MB, skip
	if (nMb > 10)
	{
		AfxMessageBox(" Bit Matrix is too big, the calculation of the number of Edge is skipped ! ");
		return FALSE;
	}

    m_pBytes = new unsigned char[ hei * m_nBytesOfRow ];
	memset( m_pBytes, 0, sizeof(unsigned char)*hei*m_nBytesOfRow );
	
	return TRUE;
  //## end BitAdjacentMatrix::Init%418B319F0074.body
}

bool BitAdjacentMatrix::IsInit ()
{
  //## begin BitAdjacentMatrix::IsInit%418B7AA9015D.body preserve=yes

	if(m_pBytes != NULL)
		return TRUE;
	else
		return FALSE;
  //## end BitAdjacentMatrix::IsInit%418B7AA9015D.body
}

// Additional Declarations
  //## begin BitAdjacentMatrix%418B2B360125.declarations preserve=yes
  //## end BitAdjacentMatrix%418B2B360125.declarations

//## begin module%418B2B360125.epilog preserve=yes
//## end module%418B2B360125.epilog
