//## begin module%3FD56F10016F.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FD56F10016F.cm

//## begin module%3FD56F10016F.cp preserve=no
//## end module%3FD56F10016F.cp

//## Module: CL0%3FD56F10016F; Pseudo Package specification
//## Source file: C:\zbf\Research\3dViewer\CL0.h

#ifndef CL0_h
#define CL0_h 1

//## begin module%3FD56F10016F.additionalIncludes preserve=no
//## end module%3FD56F10016F.additionalIncludes

//## begin module%3FD56F10016F.includes preserve=yes
//## end module%3FD56F10016F.includes

// CArray2DInt
#include "CArray2DInt.h"
// CVertexArray
#include "CVertexArray.h"
// CLw
#include "CLw.h"
//## begin module%3FD56F10016F.additionalDeclarations preserve=yes
#pragma once
//## end module%3FD56F10016F.additionalDeclarations


//## begin CL0%3FD56F10016F.preface preserve=yes
//## end CL0%3FD56F10016F.preface

//## Class: CL0%3FD56F10016F
//## Category: Triangle Mesh%40F38ABC0112
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3FD5709C0002;CLw { -> }
//## Uses: <unnamed>%3FD6E66900D4;CVertexArray { -> }

class CL0 : public CArray2DFLT  //## Inherits: <unnamed>%4135796E0271
{
  //## begin CL0%3FD56F10016F.initialDeclarations preserve=yes
  //## end CL0%3FD56F10016F.initialDeclarations

  public:
    //## Constructors (generated)
      CL0();

    //## Destructor (generated)
      ~CL0();


    //## Other Operations (specified)
      //## Operation: Cal%3FD58D64003B
      void Cal (CLw* pCLw, float* x);

      //## Operation: Cal%3FD6E464011B
      void Cal (CLw* pCLw, CVertexArray* pVtx);

    // Data Members for Class Attributes

      //## Attribute: m_pVertexX%3FD5EBAE0292
      //## begin CL0::m_pVertexX%3FD5EBAE0292.attr preserve=no  public: float* {UA} NULL
      float* m_pVertexX;
      //## end CL0::m_pVertexX%3FD5EBAE0292.attr

      //## Attribute: m_pVertexY%3FD5EBB201B1
      //## begin CL0::m_pVertexY%3FD5EBB201B1.attr preserve=no  public: float* {UA} NULL
      float* m_pVertexY;
      //## end CL0::m_pVertexY%3FD5EBB201B1.attr

      //## Attribute: m_pVertexZ%3FD5EBBD012B
      //## begin CL0::m_pVertexZ%3FD5EBBD012B.attr preserve=no  public: float* {UA} NULL
      float* m_pVertexZ;
      //## end CL0::m_pVertexZ%3FD5EBBD012B.attr

    // Data Members for Associations

      //## Association: <unnamed>%3FD58E03031A
      //## Role: CL0::m_pCLw%3FD58E050074
      //## begin CL0::m_pCLw%3FD58E050074.role preserve=no  public: CLw { -> RHgAN}
      CLw *m_pCLw;
      //## end CL0::m_pCLw%3FD58E050074.role

    // Additional Public Declarations
      //## begin CL0%3FD56F10016F.public preserve=yes
      //## end CL0%3FD56F10016F.public

  protected:
    // Additional Protected Declarations
      //## begin CL0%3FD56F10016F.protected preserve=yes
      //## end CL0%3FD56F10016F.protected

  private:

    //## Other Operations (specified)
      //## Operation: Distance%3FD58C410109
      double Distance (int i, int j);

    // Additional Private Declarations
      //## begin CL0%3FD56F10016F.private preserve=yes
      //## end CL0%3FD56F10016F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CL0%3FD56F10016F.implementation preserve=yes
      //## end CL0%3FD56F10016F.implementation

};

//## begin CL0%3FD56F10016F.postscript preserve=yes
//## end CL0%3FD56F10016F.postscript

// Class CL0 

//## begin module%3FD56F10016F.epilog preserve=yes
//## end module%3FD56F10016F.epilog


#endif
