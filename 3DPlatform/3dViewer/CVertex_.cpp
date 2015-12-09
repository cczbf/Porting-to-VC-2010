//## begin module%3FCA8D4200C6.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FCA8D4200C6.cm

//## begin module%3FCA8D4200C6.cp preserve=no
//## end module%3FCA8D4200C6.cp

//## Module: CVertex%3FCA8D4200C6; Pseudo Package body
//## Source file: C:\zbf\Research\Spherical Para\Foundmentals of SP\fsp\CVertex.cpp

//## begin module%3FCA8D4200C6.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%3FCA8D4200C6.additionalIncludes

//## begin module%3FCA8D4200C6.includes preserve=yes
//## end module%3FCA8D4200C6.includes

// CVertex
#include "CVertex_.h"
//## begin module%3FCA8D4200C6.additionalDeclarations preserve=yes
//## end module%3FCA8D4200C6.additionalDeclarations


// Class CVertex 




CVertex_::CVertex_()
  //## begin CVertex::CVertex%.hasinit preserve=no
  //## end CVertex::CVertex%.hasinit
  //## begin CVertex::CVertex%.initialization preserve=yes
  //## end CVertex::CVertex%.initialization
{
  //## begin CVertex::CVertex%.body preserve=yes
  //## end CVertex::CVertex%.body
}


CVertex_::~CVertex_()
{
  //## begin CVertex::~CVertex%.body preserve=yes
  //## end CVertex::~CVertex%.body
}


int CVertex_::operator==(const CVertex_ &right) const
{
  //## begin CVertex::operator==%.body preserve=yes

	return ( (this->dX == right.dX) &&(this->dY == right.dY)&&(this->dZ == right.dZ));
  //## end CVertex::operator==%.body
}

int CVertex_::operator!=(const CVertex_ &right) const
{
  //## begin CVertex::operator!=%.body preserve=yes
	return !((*this) == right);
  //## end CVertex::operator!=%.body
}


// Additional Declarations
  //## begin CVertex%3FCA8D4200C6.declarations preserve=yes
  //## end CVertex%3FCA8D4200C6.declarations

//## begin module%3FCA8D4200C6.epilog preserve=yes
//## end module%3FCA8D4200C6.epilog
