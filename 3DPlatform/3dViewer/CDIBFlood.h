//## begin module%4213ECF40393.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%4213ECF40393.cm

//## begin module%4213ECF40393.cp preserve=no
//## end module%4213ECF40393.cp

//## Module: CDIBFlood%4213ECF40393; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CDIBFlood.h

#ifndef CDIBFlood_h
#define CDIBFlood_h 1

//## begin module%4213ECF40393.additionalIncludes preserve=no
//## end module%4213ECF40393.additionalIncludes

//## begin module%4213ECF40393.includes preserve=yes
//## end module%4213ECF40393.includes

// CDIB
#include "CDIB.h"
//## begin module%4213ECF40393.declarations preserve=no
//## end module%4213ECF40393.declarations

//## begin module%4213ECF40393.additionalDeclarations preserve=yes
//## end module%4213ECF40393.additionalDeclarations


//## begin stParam%4213EC9C0350.preface preserve=yes
//## end stParam%4213EC9C0350.preface

//## Class: stParam%4213EC9C0350
//## Category: 3dViewer::CDIB_%419D48190195
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct stParam 
{
  //## begin stParam%4213EC9C0350.initialDeclarations preserve=yes
  //## end stParam%4213EC9C0350.initialDeclarations

    // Data Members for Class Attributes

      //## Attribute: dblKey%4213ED41031B
      //## begin stParam::dblKey%4213ED41031B.attr preserve=no  public: double {UA} 
      double dblKey;
      //## end stParam::dblKey%4213ED41031B.attr

      //## Attribute: x%4213ED560073
      //## begin stParam::x%4213ED560073.attr preserve=no  public: double {UA} 
      double x;
      //## end stParam::x%4213ED560073.attr

      //## Attribute: y%4213ED5C02A2
      //## begin stParam::y%4213ED5C02A2.attr preserve=no  public: double {UA} 
      double y;
      //## end stParam::y%4213ED5C02A2.attr

  public:
    // Additional Public Declarations
      //## begin stParam%4213EC9C0350.public preserve=yes
      //## end stParam%4213EC9C0350.public

  protected:
    // Additional Protected Declarations
      //## begin stParam%4213EC9C0350.protected preserve=yes
      //## end stParam%4213EC9C0350.protected

  private:
    // Additional Private Declarations
      //## begin stParam%4213EC9C0350.private preserve=yes
      //## end stParam%4213EC9C0350.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin stParam%4213EC9C0350.implementation preserve=yes
      //## end stParam%4213EC9C0350.implementation

};

//## begin stParam%4213EC9C0350.postscript preserve=yes
//## end stParam%4213EC9C0350.postscript

//## begin stStack%42143E6B017F.preface preserve=yes
//## end stStack%42143E6B017F.preface

//## Class: stStack%42143E6B017F
//## Category: 3dViewer::CDIB_%419D48190195
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct stStack 
{
  //## begin stStack%42143E6B017F.initialDeclarations preserve=yes
  //## end stStack%42143E6B017F.initialDeclarations

    // Data Members for Class Attributes

      //## Attribute: x%42143EAF01B9
      //## begin stStack::x%42143EAF01B9.attr preserve=no  public: int {UA} 
      int x;
      //## end stStack::x%42143EAF01B9.attr

      //## Attribute: y%42143EC10179
      //## begin stStack::y%42143EC10179.attr preserve=no  public: int {UA} 
      int y;
      //## end stStack::y%42143EC10179.attr

      //## Attribute: dMin%42143EC70394
      //## begin stStack::dMin%42143EC70394.attr preserve=no  public: double {UA} 
      double dMin;
      //## end stStack::dMin%42143EC70394.attr

      //## Attribute: dMax%42143ED20001
      //## begin stStack::dMax%42143ED20001.attr preserve=no  public: double {UA} 
      double dMax;
      //## end stStack::dMax%42143ED20001.attr

  public:
    // Additional Public Declarations
      //## begin stStack%42143E6B017F.public preserve=yes
      //## end stStack%42143E6B017F.public

  protected:
    // Additional Protected Declarations
      //## begin stStack%42143E6B017F.protected preserve=yes
      //## end stStack%42143E6B017F.protected

  private:
    // Additional Private Declarations
      //## begin stStack%42143E6B017F.private preserve=yes
      //## end stStack%42143E6B017F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin stStack%42143E6B017F.implementation preserve=yes
      //## end stStack%42143E6B017F.implementation

};

//## begin stStack%42143E6B017F.postscript preserve=yes
//## end stStack%42143E6B017F.postscript

//## begin CDIBFlood%4213EC310194.preface preserve=yes
//## end CDIBFlood%4213EC310194.preface

//## Class: CDIBFlood%4213EC310194
//## Category: 3dViewer::CDIB_%419D48190195
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CDIBFlood : public CDIB  //## Inherits: <unnamed>%4213EC3D02DB
{
  //## begin CDIBFlood%4213EC310194.initialDeclarations preserve=yes
  //## end CDIBFlood%4213EC310194.initialDeclarations

  public:
    //## Constructors (generated)
      CDIBFlood();

    //## Destructor (generated)
      ~CDIBFlood();


    //## Other Operations (specified)
      //## Operation: FloodFill4%4213EDAB03D2
      long FloodFill4 (HANDLE hDIB, int nSeedX, int nSeedY, int nROld, int nGOld, int nBOld, int nRNew, int nGNew, int nBNew, double dMin, double dMax);

      //## Operation: BoundryFill4%4213EDC7036E
      void BoundryFill4 (HANDLE hDIB, int nSeedX, int nSeedY, int nRNew, int nGNew, int nBNew);

      //## Operation: BoundryFill4_Stack%42143E260085
      void BoundryFill4_Stack (HANDLE hDIB, int nSeedX, int nSeedY, int nRNew, int nGNew, int nBNew);

    // Data Members for Class Attributes

      //## Attribute: m_lParamLength%4213F2AC02F8
      //## begin CDIBFlood::m_lParamLength%4213F2AC02F8.attr preserve=no  public: long {UA} 0
      long m_lParamLength;
      //## end CDIBFlood::m_lParamLength%4213F2AC02F8.attr

      //## Attribute: m_lStackTop%42143FAA02FA
      //## begin CDIBFlood::m_lStackTop%42143FAA02FA.attr preserve=no  public: long {UA} -1
      long m_lStackTop;
      //## end CDIBFlood::m_lStackTop%42143FAA02FA.attr

    // Data Members for Associations

      //## Association: 3dViewer::CDIB_::<unnamed>%4213ED1D008F
      //## Role: CDIBFlood::m_pParam%4213ED1D039C
      //## begin CDIBFlood::m_pParam%4213ED1D039C.role preserve=no  public: stParam { -> RHgAN}
      stParam *m_pParam;
      //## end CDIBFlood::m_pParam%4213ED1D039C.role

      //## Association: 3dViewer::CDIB_::<unnamed>%42143EFC035E
      //## Role: CDIBFlood::m_pstStack%42143EFD0247
      //## begin CDIBFlood::m_pstStack%42143EFD0247.role preserve=no  public: stStack { -> RHgAN}
      stStack *m_pstStack;
      //## end CDIBFlood::m_pstStack%42143EFD0247.role

    // Additional Public Declarations
      //## begin CDIBFlood%4213EC310194.public preserve=yes
      //## end CDIBFlood%4213EC310194.public

  protected:
    // Additional Protected Declarations
      //## begin CDIBFlood%4213EC310194.protected preserve=yes
      //## end CDIBFlood%4213EC310194.protected

  private:

    //## Other Operations (specified)
      //## Operation: Push%421440F8007A
      void Push (int x, int y, double dMin, double dMax);

      //## Operation: Pop%4214475F004E
      void Pop (int* x, int* y, double* dMin, double* dMax);

      //## Operation: IsInterior%421445B0018F
      BOOL IsInterior (HANDLE hDIB, int nSeedX, int nSeedY, WORD nR, WORD nG, WORD nB);

      //## Operation: IsBorder%4214693501DA
      BOOL IsBorder (HANDLE hDIB, int nSeedX, int nSeedY, WORD nRNew, WORD nGNew, WORD nBNew);

      //## Operation: StackEmpty%421446610284
      BOOL StackEmpty ();

    // Additional Private Declarations
      //## begin CDIBFlood%4213EC310194.private preserve=yes
      //## end CDIBFlood%4213EC310194.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CDIBFlood%4213EC310194.implementation preserve=yes
      //## end CDIBFlood%4213EC310194.implementation

};

//## begin CDIBFlood%4213EC310194.postscript preserve=yes
//## end CDIBFlood%4213EC310194.postscript

// Class stParam 

// Class stStack 

// Class CDIBFlood 

//## begin module%4213ECF40393.epilog preserve=yes
//## end module%4213ECF40393.epilog


#endif
