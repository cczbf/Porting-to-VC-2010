//## begin module%3FCA9785023C.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FCA9785023C.cm

//## begin module%3FCA9785023C.cp preserve=no
//## end module%3FCA9785023C.cp

//## Module: CArray2DInt%3FCA9785023C; Pseudo Package specification
//## Source file: C:\zbf\Research\Spherical Para\Foundmentals of SP\fsp\CArray2DInt.h

#ifndef CArray2DInt_h_
#define CArray2DInt_h_ 1

//## begin module%3FCA9785023C.additionalIncludes preserve=no
//## end module%3FCA9785023C.additionalIncludes

//## begin module%3FCA9785023C.includes preserve=yes
//## end module%3FCA9785023C.includes

// MFC 6\.0
#include "MFC 60.h"
// CIntArray
#include "CIntArray_.h"
//## begin module%3FCA9785023C.additionalDeclarations preserve=yes
//## end module%3FCA9785023C.additionalDeclarations


//## begin CArray2DInt%3FCA9785023C.preface preserve=yes
//## end CArray2DInt%3FCA9785023C.preface

//## Class: CArray2DInt%3FCA9785023C
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3FCAB18301E3;CIntArray { -> }

class CArray2DInt : public CArray< CIntArray_,CIntArray_ >  //## Inherits: <unnamed>%3FCA978D0321
{
  //## begin CArray2DInt%3FCA9785023C.initialDeclarations preserve=yes
  //## end CArray2DInt%3FCA9785023C.initialDeclarations

  public:
    //## Constructors (generated)
      CArray2DInt();

    //## Constructors (specified)
      //## Operation: CArray2DInt%3FCA99EC0156
      CArray2DInt (int nRowDim, int nColDim);

    //## Destructor (generated)
      ~CArray2DInt();


    //## Other Operations (specified)
      //## Operation: GetDim%3FCAB05D03DF
      void GetDim (int* nRow, int* nCol);

      //## Operation: SetDim%3FCAB2FA01AB
      void SetDim (int nRow, int nCol);

    // Data Members for Class Attributes

      //## Attribute: m_nRowDim%3FCA9A660288
      //## begin CArray2DInt::m_nRowDim%3FCA9A660288.attr preserve=no  public: int {UA} 0
      int m_nRowDim;
      //## end CArray2DInt::m_nRowDim%3FCA9A660288.attr

      //## Attribute: m_nColDim%3FCA9A6F017B
      //## begin CArray2DInt::m_nColDim%3FCA9A6F017B.attr preserve=no  public: int {UA} 0
      int m_nColDim;
      //## end CArray2DInt::m_nColDim%3FCA9A6F017B.attr

    // Additional Public Declarations
      //## begin CArray2DInt%3FCA9785023C.public preserve=yes
      //## end CArray2DInt%3FCA9785023C.public

  protected:
    // Additional Protected Declarations
      //## begin CArray2DInt%3FCA9785023C.protected preserve=yes
      //## end CArray2DInt%3FCA9785023C.protected

  private:
    // Additional Private Declarations
      //## begin CArray2DInt%3FCA9785023C.private preserve=yes
      //## end CArray2DInt%3FCA9785023C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CArray2DInt%3FCA9785023C.implementation preserve=yes
      //## end CArray2DInt%3FCA9785023C.implementation

};

//## begin CArray2DInt%3FCA9785023C.postscript preserve=yes
//## end CArray2DInt%3FCA9785023C.postscript

// Class CArray2DInt 

//## begin module%3FCA9785023C.epilog preserve=yes
//## end module%3FCA9785023C.epilog


#endif
