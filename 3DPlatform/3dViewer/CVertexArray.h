//## begin module%40F34AA000ED.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40F34AA000ED.cm

//## begin module%40F34AA000ED.cp preserve=no
//## end module%40F34AA000ED.cp

//## Module: CVertexArray%40F34AA000ED; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CVertexArray.h

#ifndef CVertexArray_h
#define CVertexArray_h 1

//## begin module%40F34AA000ED.additionalIncludes preserve=no
//## end module%40F34AA000ED.additionalIncludes

//## begin module%40F34AA000ED.includes preserve=yes
#include <afxtempl.h>
#include "CVertex.h"
//## end module%40F34AA000ED.includes

// MFC 6\.0
#include "MFC 60.h"
//## begin module%40F34AA000ED.declarations preserve=no
//## end module%40F34AA000ED.declarations

//## begin module%40F34AA000ED.additionalDeclarations preserve=yes
//## end module%40F34AA000ED.additionalDeclarations


//## begin CVertexArray%3FC9BEFD00FA.preface preserve=yes
//## end CVertexArray%3FC9BEFD00FA.preface

//## Class: CVertexArray%3FC9BEFD00FA
//## Category: 3dViewer::Triangle Mesh%3FC9AFED02AE
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3FC9BF2703AD;CArray { -> }

class CVertexArray : public CArray< CVertex,CVertex >  //## Inherits: <unnamed>%3FC9C125032E
{
  //## begin CVertexArray%3FC9BEFD00FA.initialDeclarations preserve=yes
  //## end CVertexArray%3FC9BEFD00FA.initialDeclarations

  public:
    //## Constructors (generated)
      CVertexArray();

      CVertexArray(const CVertexArray &right);

    //## Destructor (generated)
      ~CVertexArray();

    //## Assignment Operation (generated)
      CVertexArray & operator=(const CVertexArray &right);

    //## Equality Operations (generated)
      int operator==(const CVertexArray &right) const;

      int operator!=(const CVertexArray &right) const;

    // Additional Public Declarations
      //## begin CVertexArray%3FC9BEFD00FA.public preserve=yes
      //## end CVertexArray%3FC9BEFD00FA.public

  protected:
    // Additional Protected Declarations
      //## begin CVertexArray%3FC9BEFD00FA.protected preserve=yes
      //## end CVertexArray%3FC9BEFD00FA.protected

  private:
    // Additional Private Declarations
      //## begin CVertexArray%3FC9BEFD00FA.private preserve=yes
      //## end CVertexArray%3FC9BEFD00FA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CVertexArray%3FC9BEFD00FA.implementation preserve=yes
      //## end CVertexArray%3FC9BEFD00FA.implementation

};

//## begin CVertexArray%3FC9BEFD00FA.postscript preserve=yes
//## end CVertexArray%3FC9BEFD00FA.postscript

// Class CVertexArray 

//## begin module%40F34AA000ED.epilog preserve=yes
//## end module%40F34AA000ED.epilog


#endif
