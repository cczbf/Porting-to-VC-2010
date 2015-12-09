//## begin module%369E45C80194.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E45C80194.cm

//## begin module%369E45C80194.cp preserve=no
//## end module%369E45C80194.cp

//## Module: CCursorZoomIn%369E45C80194; Pseudo Package specification
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CCursorZoomIn.h

#ifndef CCursorZoomIn_h
#define CCursorZoomIn_h 1

//## begin module%369E45C80194.additionalIncludes preserve=no
//## end module%369E45C80194.additionalIncludes

//## begin module%369E45C80194.includes preserve=yes
//## end module%369E45C80194.includes

// CCursorBase
#include "CCursorBase.h"
//## begin module%369E45C80194.additionalDeclarations preserve=yes
//## end module%369E45C80194.additionalDeclarations


//## Class: CCursorZoomIn%369E45C80194
//## Category: bmpviewer::Tools%369E44800341
//## Subsystem: bmpviewer::Tools%369E44800341
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CCursorZoomIn : public CCursorBase  //## Inherits: <unnamed>%369E45F50370
{
  //## begin CCursorZoomIn%369E45C80194.initialDeclarations preserve=yes
  //## end CCursorZoomIn%369E45C80194.initialDeclarations

  public:
    //## Constructors (generated)
      CCursorZoomIn();

    //## Constructors (specified)
      //## Operation: CCursorZoomIn%916354668
      CCursorZoomIn (CToolBase* pCToolBase);

    //## Destructor (generated)
      ~CCursorZoomIn();


    //## Other Operations (specified)
      //## Operation: CursorInside%916354666
      BOOL CursorInside (int x, int y);

      //## Operation: CursorOutside%916354667
      BOOL CursorOutside (int x, int y);

    // Additional Public Declarations
      //## begin CCursorZoomIn%369E45C80194.public preserve=yes
      //## end CCursorZoomIn%369E45C80194.public

  protected:
    // Additional Protected Declarations
      //## begin CCursorZoomIn%369E45C80194.protected preserve=yes
      //## end CCursorZoomIn%369E45C80194.protected

  private:
    // Additional Private Declarations
      //## begin CCursorZoomIn%369E45C80194.private preserve=yes
      //## end CCursorZoomIn%369E45C80194.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin CCursorZoomIn%369E45C80194.implementation preserve=yes
      //## end CCursorZoomIn%369E45C80194.implementation

};

//## begin CCursorZoomIn%369E45C80194.postscript preserve=yes
//## end CCursorZoomIn%369E45C80194.postscript

// Class CCursorZoomIn 

//## begin module%369E45C80194.epilog preserve=yes
//## end module%369E45C80194.epilog


#endif
