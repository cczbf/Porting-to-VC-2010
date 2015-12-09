//## begin module%364F6E8B017C.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%364F6E8B017C.cm

//## begin module%364F6E8B017C.cp preserve=no
//## end module%364F6E8B017C.cp

//## Module: CToolPan%364F6E8B017C; Pseudo Package specification
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CToolPan.h

#ifndef CToolPan_h
#define CToolPan_h 1

//## begin module%364F6E8B017C.additionalIncludes preserve=no
//## end module%364F6E8B017C.additionalIncludes

//## begin module%364F6E8B017C.includes preserve=yes
//## end module%364F6E8B017C.includes

// ctoolbase
#include "ctoolbase.h"
// CCursorPan
#include "CCursorPan.h"
//## begin module%364F6E8B017C.additionalDeclarations preserve=yes
//class CCursorPan;
//## end module%364F6E8B017C.additionalDeclarations


//## Class: CToolPan%364F6E8B017C
//	m_nToolID mustbe set to 1
//## Category: bmpviewer::Tools%369E44800341
//## Subsystem: bmpviewer::Tools%369E44800341
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3651182F0032;CCursorPan { -> }

class CToolPan : public CToolBase  //## Inherits: <unnamed>%369D30F101D2
{
  //## begin CToolPan%364F6E8B017C.initialDeclarations preserve=yes
  //## end CToolPan%364F6E8B017C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CToolPan%911175330
      CToolPan ();

    //## Destructor (specified)
      //## Operation: ~CToolPan%911175331
      ~CToolPan ();


    //## Other Operations (specified)
      //## Operation: OnLButtonUp%911175332
      BOOL OnLButtonUp (UINT nFlags, CPoint point);

      //## Operation: OnLButtonDown%911175333
      BOOL OnLButtonDown (UINT nFlags, CPoint point);

	  BOOL OnMButtonUp (UINT nFlags, CPoint point);

      //## Operation: OnLButtonDown%911175333
      BOOL OnMButtonDown (UINT nFlags, CPoint point);

	  //## Operation: OnLButtonUp%916266560
      BOOL OnRButtonUp (UINT nFlags, CPoint point) ;
      //## Operation: OnLButtonDown%916266561
      BOOL OnRButtonDown (UINT nFlags, CPoint point) ;


      //## Operation: OnMouseMove%911175334
      BOOL OnMouseMove (UINT nFlags, CPoint point);

      //## Operation: OnLBDblClick%916274419
      BOOL OnLBDblClick (UINT nFlags, CPoint point);

    // Additional Public Declarations
      //## begin CToolPan%364F6E8B017C.public preserve=yes
      //## end CToolPan%364F6E8B017C.public

  protected:
    // Additional Protected Declarations
      //## begin CToolPan%364F6E8B017C.protected preserve=yes
      //## end CToolPan%364F6E8B017C.protected

  private:
    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_cur_pos%364F9ED60154
      const POINT get_m_cur_pos () const;
      void set_m_cur_pos (POINT value);

    // Additional Private Declarations
      //## begin CToolPan%364F6E8B017C.private preserve=yes
      //## end CToolPan%364F6E8B017C.private

  private:  //## implementation
    // Data Members for Class Attributes

      //## begin CToolPan::m_cur_pos%364F9ED60154.attr preserve=no  private: POINT {U} 
      POINT m_cur_pos;
      //## end CToolPan::m_cur_pos%364F9ED60154.attr

    // Additional Implementation Declarations
      //## begin CToolPan%364F6E8B017C.implementation preserve=yes
      //## end CToolPan%364F6E8B017C.implementation

};

//## begin CToolPan%364F6E8B017C.postscript preserve=yes
//## end CToolPan%364F6E8B017C.postscript

// Class CToolPan 

//## Get and Set Operations for Class Attributes (inline)

inline const POINT CToolPan::get_m_cur_pos () const
{
  //## begin CToolPan::get_m_cur_pos%364F9ED60154.get preserve=no
  return m_cur_pos;
  //## end CToolPan::get_m_cur_pos%364F9ED60154.get
}

inline void CToolPan::set_m_cur_pos (POINT value)
{
  //## begin CToolPan::set_m_cur_pos%364F9ED60154.set preserve=no
  m_cur_pos = value;
  //## end CToolPan::set_m_cur_pos%364F9ED60154.set
}

//## begin module%364F6E8B017C.epilog preserve=yes
//## end module%364F6E8B017C.epilog


#endif
