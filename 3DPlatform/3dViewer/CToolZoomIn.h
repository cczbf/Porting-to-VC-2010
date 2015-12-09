//## begin module%369E27A000A3.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E27A000A3.cm

//## begin module%369E27A000A3.cp preserve=no
//## end module%369E27A000A3.cp

//## Module: CToolZoomIn%369E27A000A3; Pseudo Package specification
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CToolZoomIn.h

#ifndef CToolZoomIn_h
#define CToolZoomIn_h 1

//## begin module%369E27A000A3.additionalIncludes preserve=no
//## end module%369E27A000A3.additionalIncludes

//## begin module%369E27A000A3.includes preserve=yes
//## end module%369E27A000A3.includes

// ctoolbase
#include "ctoolbase.h"
// CCursorZoomIn
#include "CCursorZoomIn.h"
//## begin module%369E27A000A3.additionalDeclarations preserve=yes
//## end module%369E27A000A3.additionalDeclarations


//## Class: CToolZoomIn%369E27A000A3
//	m_nToolID mustbe set to 1.
//## Category: bmpviewer::Tools%369E44800341
//## Subsystem: bmpviewer::Tools%369E44800341
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%369E7725002D;CCursorZoomIn { -> }

class CToolZoomIn : public CToolBase  //## Inherits: <unnamed>%369E28200116
{
  //## begin CToolZoomIn%369E27A000A3.initialDeclarations preserve=yes
  //## end CToolZoomIn%369E27A000A3.initialDeclarations

  public:
    //## Constructors (generated)
      CToolZoomIn();

    //## Destructor (generated)
      ~CToolZoomIn();


    //## Other Operations (specified)
      //## Operation: OnLButtonDown%916340333
      BOOL OnLButtonDown (UINT nFlags, CPoint point);

      //## Operation: OnLButtonUp%916340335
      BOOL OnLButtonUp (UINT nFlags, CPoint point);

	  //## Operation: OnLButtonDown%916340333
      BOOL OnMButtonDown (UINT nFlags, CPoint point);
      //## Operation: OnLButtonUp%916340335
      BOOL OnMButtonUp (UINT nFlags, CPoint point);

      //## Operation: OnMouseMove%916340336
      BOOL OnMouseMove (UINT nFlags, CPoint point);
  	  //## Operation: OnLButtonUp%916266560
      BOOL OnRButtonUp (UINT nFlags, CPoint point);
      //## Operation: OnLButtonDown%916266561
      BOOL OnRButtonDown (UINT nFlags, CPoint point);


      //## Operation: OnLBDblClick%916340337
      BOOL OnLBDblClick (UINT nFlags, CPoint point);

    // Additional Public Declarations
      //## begin CToolZoomIn%369E27A000A3.public preserve=yes
      //## end CToolZoomIn%369E27A000A3.public

  protected:
    // Additional Protected Declarations
      //## begin CToolZoomIn%369E27A000A3.protected preserve=yes
      //## end CToolZoomIn%369E27A000A3.protected

  private:
    // Additional Private Declarations
      //## begin CToolZoomIn%369E27A000A3.private preserve=yes
      //## end CToolZoomIn%369E27A000A3.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin CToolZoomIn%369E27A000A3.implementation preserve=yes
      //## end CToolZoomIn%369E27A000A3.implementation

};

//## begin CToolZoomIn%369E27A000A3.postscript preserve=yes
//## end CToolZoomIn%369E27A000A3.postscript

// Class CToolZoomIn 

//## begin module%369E27A000A3.epilog preserve=yes
//## end module%369E27A000A3.epilog


#endif
