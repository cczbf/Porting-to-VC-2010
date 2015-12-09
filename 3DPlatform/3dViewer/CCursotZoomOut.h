//## begin module%369E45E501BE.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E45E501BE.cm

//## begin module%369E45E501BE.cp preserve=no
//## end module%369E45E501BE.cp

//## Module: CCursotZoomOut%369E45E501BE; Pseudo Package specification
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CCursotZoomOut.h

#ifndef CCursotZoomOut_h
#define CCursotZoomOut_h 1

//## begin module%369E45E501BE.additionalIncludes preserve=no
//## end module%369E45E501BE.additionalIncludes

//## begin module%369E45E501BE.includes preserve=yes
//## end module%369E45E501BE.includes

// CCursorBase
#include "CCursorBase.h"
//## begin module%369E45E501BE.additionalDeclarations preserve=yes
//## end module%369E45E501BE.additionalDeclarations


//## Class: CCursotZoomOut%369E45E501BE
//## Category: bmpviewer::Tools%369E44800341
//## Subsystem: bmpviewer::Tools%369E44800341
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CCursotZoomOut : public CCursorBase  //## Inherits: <unnamed>%369E45FB01AC
{
  //## begin CCursotZoomOut%369E45E501BE.initialDeclarations preserve=yes
  //## end CCursotZoomOut%369E45E501BE.initialDeclarations

  public:
    //## Constructors (generated)
      CCursotZoomOut();

    //## Destructor (generated)
      ~CCursotZoomOut();


    //## Other Operations (specified)
      //## Operation: CursorInside%916354663
      BOOL CursorInside (int x, int y);

      //## Operation: CursorOutside%916354664
      BOOL CursorOutside (int x, int y);

      //## Operation: CCursorZoomOut%916354665
      void CCursorZoomOut (CToolBase* pCToolBase);

    // Additional Public Declarations
      //## begin CCursotZoomOut%369E45E501BE.public preserve=yes
      //## end CCursotZoomOut%369E45E501BE.public

  protected:
    // Additional Protected Declarations
      //## begin CCursotZoomOut%369E45E501BE.protected preserve=yes
      //## end CCursotZoomOut%369E45E501BE.protected

  private:
    // Additional Private Declarations
      //## begin CCursotZoomOut%369E45E501BE.private preserve=yes
      //## end CCursotZoomOut%369E45E501BE.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin CCursotZoomOut%369E45E501BE.implementation preserve=yes
      //## end CCursotZoomOut%369E45E501BE.implementation

};

//## begin CCursotZoomOut%369E45E501BE.postscript preserve=yes
//## end CCursotZoomOut%369E45E501BE.postscript

// Class CCursotZoomOut 

//## begin module%369E45E501BE.epilog preserve=yes
//## end module%369E45E501BE.epilog


#endif
