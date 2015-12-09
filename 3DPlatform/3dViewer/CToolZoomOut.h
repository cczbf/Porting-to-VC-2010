//## begin module%369E3E180253.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E3E180253.cm

//## begin module%369E3E180253.cp preserve=no
//## end module%369E3E180253.cp

//## Module: CToolZoomOut%369E3E180253; Pseudo Package specification
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CToolZoomOut.h

#ifndef CToolZoomOut_h
#define CToolZoomOut_h 1

//## begin module%369E3E180253.additionalIncludes preserve=no
//## end module%369E3E180253.additionalIncludes

//## begin module%369E3E180253.includes preserve=yes
//## end module%369E3E180253.includes

// ctoolbase
#include "ctoolbase.h"
// CCursorZoomOut
#include "CCursorZoomOut.h"
//## begin module%369E3E180253.additionalDeclarations preserve=yes
//## end module%369E3E180253.additionalDeclarations


//## Class: CToolZoomOut%369E3E180253
//	m_nToolID mustbe set to 2.
//## Category: bmpviewer::Tools%369E44800341
//## Subsystem: bmpviewer::Tools%369E44800341
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%369E772A00A2;CCursorZoomOut { -> }

class CToolZoomOut : public CToolBase  //## Inherits: <unnamed>%369E3E3700F9
{
  //## begin CToolZoomOut%369E3E180253.initialDeclarations preserve=yes
  //## end CToolZoomOut%369E3E180253.initialDeclarations

  public:
    //## Constructors (generated)
      CToolZoomOut();

    //## Destructor (generated)
      ~CToolZoomOut();


    //## Other Operations (specified)
      //## Operation: OnLButtonDown%916340334
      BOOL OnLButtonDown (UINT nFlags, CPoint point);

      //## Operation: OnLButtonUp%916340338
      BOOL OnLButtonUp (UINT nFlags, CPoint point);

	  BOOL OnMButtonDown (UINT nFlags, CPoint point);

      //## Operation: OnLButtonUp%916340338
      BOOL OnMButtonUp (UINT nFlags, CPoint point);

      //## Operation: OnMouseMove%916340339
      BOOL OnMouseMove (UINT nFlags, CPoint point);

	  //## Operation: OnLButtonUp%916266560
      BOOL OnRButtonUp (UINT nFlags, CPoint point);
      //## Operation: OnLButtonDown%916266561
      BOOL OnRButtonDown (UINT nFlags, CPoint point);

      //## Operation: OnLBDblClick%916340340
      BOOL OnLBDblClick (UINT nFlags, CPoint point);

    // Additional Public Declarations
      //## begin CToolZoomOut%369E3E180253.public preserve=yes
      //## end CToolZoomOut%369E3E180253.public

  protected:
    // Additional Protected Declarations
      //## begin CToolZoomOut%369E3E180253.protected preserve=yes
      //## end CToolZoomOut%369E3E180253.protected

  private:
    // Additional Private Declarations
      //## begin CToolZoomOut%369E3E180253.private preserve=yes
      //## end CToolZoomOut%369E3E180253.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin CToolZoomOut%369E3E180253.implementation preserve=yes
      //## end CToolZoomOut%369E3E180253.implementation

};

//## begin CToolZoomOut%369E3E180253.postscript preserve=yes
//## end CToolZoomOut%369E3E180253.postscript

// Class CToolZoomOut 

//## begin module%369E3E180253.epilog preserve=yes
//## end module%369E3E180253.epilog


#endif
