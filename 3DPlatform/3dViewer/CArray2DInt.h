//## begin module%41353EA502E1.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%41353EA502E1.cm

//## begin module%41353EA502E1.cp preserve=no
//## end module%41353EA502E1.cp

//## Module: CArray2DInt%41353EA502E1; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CArray2DInt.h

#ifndef CArray2DInt_h
#define CArray2DInt_h 1

//## begin module%41353EA502E1.additionalIncludes preserve=no
//## end module%41353EA502E1.additionalIncludes

//## begin module%41353EA502E1.includes preserve=yes
//## end module%41353EA502E1.includes

// CIntArray
#include "CIntArray.h"
// CArray
#include "CArray.h"
//## begin module%41353EA502E1.declarations preserve=no
//## end module%41353EA502E1.declarations

//## begin module%41353EA502E1.additionalDeclarations preserve=yes
//## end module%41353EA502E1.additionalDeclarations


//## begin CArray2DFLT%3FCA9785023C.preface preserve=yes
//## end CArray2DFLT%3FCA9785023C.preface

//## Class: CArray2DFLT%3FCA9785023C
//## Category: Triangle Mesh%40F38ABC0112
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%413571190039;CFLTArray { -> }

class CArray2DFLT : public CArray< CFLTArray,CFLTArray >  //## Inherits: <unnamed>%4135792C0316
{
  //## begin CArray2DFLT%3FCA9785023C.initialDeclarations preserve=yes
  //## end CArray2DFLT%3FCA9785023C.initialDeclarations

  public:
    //## Constructors (generated)
      CArray2DFLT();

    //## Destructor (generated)
      ~CArray2DFLT();


    //## Other Operations (specified)
      //## Operation: CArray2DInt%3FCA99EC0156
      void CArray2DInt (int nRowDim, int nColDim);

      //## Operation: GetDim%3FCAB05D03DF
      void GetDim (int* nRow, int* nCol);

      //## Operation: SetDim%3FCAB2FA01AB
      void SetDim (int nRow, int nCol);

    // Data Members for Class Attributes

      //## Attribute: m_nRowDim%3FCA9A660288
      //## begin CArray2DFLT::m_nRowDim%3FCA9A660288.attr preserve=no  public: int {UA} 0
      int m_nRowDim;
      //## end CArray2DFLT::m_nRowDim%3FCA9A660288.attr

      //## Attribute: m_nColDim%3FCA9A6F017B
      //## begin CArray2DFLT::m_nColDim%3FCA9A6F017B.attr preserve=no  public: int {UA} 9
      int m_nColDim;
      //## end CArray2DFLT::m_nColDim%3FCA9A6F017B.attr

    // Additional Public Declarations
      //## begin CArray2DFLT%3FCA9785023C.public preserve=yes
      //## end CArray2DFLT%3FCA9785023C.public

  protected:
    // Additional Protected Declarations
      //## begin CArray2DFLT%3FCA9785023C.protected preserve=yes
      //## end CArray2DFLT%3FCA9785023C.protected

  private:
    // Additional Private Declarations
      //## begin CArray2DFLT%3FCA9785023C.private preserve=yes
      //## end CArray2DFLT%3FCA9785023C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CArray2DFLT%3FCA9785023C.implementation preserve=yes
      //## end CArray2DFLT%3FCA9785023C.implementation

};

//## begin CArray2DFLT%3FCA9785023C.postscript preserve=yes
//## end CArray2DFLT%3FCA9785023C.postscript

// Class CArray2DFLT 

//## begin module%41353EA502E1.epilog preserve=yes
//## end module%41353EA502E1.epilog


#endif
