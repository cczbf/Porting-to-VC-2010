//## begin module%3FD56FFD02D0.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FD56FFD02D0.cm

//## begin module%3FD56FFD02D0.cp preserve=no
//## end module%3FD56FFD02D0.cp

//## Module: CL%3FD56FFD02D0; Pseudo Package specification
//## Source file: C:\zbf\Research\Spherical Para\Foundmentals of SP\fsp\CL.h

#ifndef CL_h_
#define CL_h_ 1

//## begin module%3FD56FFD02D0.additionalIncludes preserve=no
//## end module%3FD56FFD02D0.additionalIncludes

//## begin module%3FD56FFD02D0.includes preserve=yes
//## end module%3FD56FFD02D0.includes

// CArray2DInt
#include "CArray2DInt1.h"
// CVertexArray
#include "CVertexArray.h"
// CL0
#include "CL0_.h"
//## begin module%3FD56FFD02D0.additionalDeclarations preserve=yes
#pragma once
//## end module%3FD56FFD02D0.additionalDeclarations


//## begin CL%3FD56FFD02D0.preface preserve=yes
//## end CL%3FD56FFD02D0.preface

//## Class: CL%3FD56FFD02D0
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3FD5F24B02AA;CL0 { -> }
//## Uses: <unnamed>%3FD6E66900D4;CVertexArray { -> }

class CL_ : public CArray2DInt  //## Inherits: <unnamed>%3FD57004010E
{
  //## begin CL%3FD56FFD02D0.initialDeclarations preserve=yes
  //## end CL%3FD56FFD02D0.initialDeclarations

  public:
    //## Constructors (generated)
      CL_();

    //## Destructor (generated)
      ~CL_();


    //## Other Operations (specified)
      //## Operation: Cal%3FD5EFE1028D
      void Cal (CL0_* pCL0, CL0_* pCL1);

    // Additional Public Declarations
      //## begin CL%3FD56FFD02D0.public preserve=yes
      //## end CL%3FD56FFD02D0.public

  protected:
    // Additional Protected Declarations
      //## begin CL%3FD56FFD02D0.protected preserve=yes
      //## end CL%3FD56FFD02D0.protected

  private:
    // Additional Private Declarations
      //## begin CL%3FD56FFD02D0.private preserve=yes
      //## end CL%3FD56FFD02D0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CL%3FD56FFD02D0.implementation preserve=yes
      //## end CL%3FD56FFD02D0.implementation

};

//## begin CL%3FD56FFD02D0.postscript preserve=yes
//## end CL%3FD56FFD02D0.postscript

// Class CL 

//## begin module%3FD56FFD02D0.epilog preserve=yes
//## end module%3FD56FFD02D0.epilog


#endif
