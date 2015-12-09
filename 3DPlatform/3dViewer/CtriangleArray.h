//## begin module%3FCA8DCE0321.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FCA8DCE0321.cm

//## begin module%3FCA8DCE0321.cp preserve=no
//## end module%3FCA8DCE0321.cp

//## Module: CTriangleArray%3FCA8DCE0321; Pseudo Package specification
//## Source file: C:\zbf\Research\Spherical Para\Foundmentals of SP\fsp\CTriangleArray.h

#ifndef CTriangleArray_h
#define CTriangleArray_h 1

//## begin module%3FCA8DCE0321.additionalIncludes preserve=no
//## end module%3FCA8DCE0321.additionalIncludes

//## begin module%3FCA8DCE0321.includes preserve=yes
#include "stdafx.h"
#include <afxtempl.h>
//## end module%3FCA8DCE0321.includes

// MFC 6\.0
#include "MFC 60.h"
// CTriangle
#include "CTriangle.h"
//## begin module%3FCA8DCE0321.additionalDeclarations preserve=yes
//## end module%3FCA8DCE0321.additionalDeclarations


//## begin CTriangleArray%3FCA8DCE0321.preface preserve=yes
//## end CTriangleArray%3FCA8DCE0321.preface

//## Class: CTriangleArray%3FCA8DCE0321
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3FCA955C026F;CTriangle { -> }

class CTriangleArray : public CArray< CTriangle,CTriangle >  //## Inherits: <unnamed>%3FCA951A02C5
{
  //## begin CTriangleArray%3FCA8DCE0321.initialDeclarations preserve=yes
  //## end CTriangleArray%3FCA8DCE0321.initialDeclarations

  public:
    //## Constructors (generated)
      CTriangleArray();

    //## Destructor (generated)
      ~CTriangleArray();

    // Additional Public Declarations
      //## begin CTriangleArray%3FCA8DCE0321.public preserve=yes
      //## end CTriangleArray%3FCA8DCE0321.public

  protected:
    // Additional Protected Declarations
      //## begin CTriangleArray%3FCA8DCE0321.protected preserve=yes
      //## end CTriangleArray%3FCA8DCE0321.protected

  private:
    // Additional Private Declarations
      //## begin CTriangleArray%3FCA8DCE0321.private preserve=yes
      //## end CTriangleArray%3FCA8DCE0321.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CTriangleArray%3FCA8DCE0321.implementation preserve=yes
      //## end CTriangleArray%3FCA8DCE0321.implementation

};

//## begin CTriangleArray%3FCA8DCE0321.postscript preserve=yes
//## end CTriangleArray%3FCA8DCE0321.postscript

// Class CTriangleArray 

//## begin module%3FCA8DCE0321.epilog preserve=yes
//## end module%3FCA8DCE0321.epilog


#endif
