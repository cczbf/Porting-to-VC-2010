//## begin module%3650E90500A0.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3650E90500A0.cm

//## begin module%3650E90500A0.cp preserve=no
//## end module%3650E90500A0.cp

//## Module: CCursorPan%3650E90500A0; Pseudo Package specification
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CCursorPan.h

#ifndef CCursorPan_h
#define CCursorPan_h 1

//## begin module%3650E90500A0.additionalIncludes preserve=no
//## end module%3650E90500A0.additionalIncludes

//## begin module%3650E90500A0.includes preserve=yes
//## end module%3650E90500A0.includes

// CCursorBase
#include "CCursorBase.h"
//## begin module%3650E90500A0.additionalDeclarations preserve=yes
class CToolPan;
//## end module%3650E90500A0.additionalDeclarations


//## Class: CCursorPan%3650E90500A0
//## Category: bmpviewer::Tools%369E44800341
//## Subsystem: bmpviewer::Tools%369E44800341
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CCursorPan : public CCursorBase  //## Inherits: <unnamed>%36510DED030C
{
  //## begin CCursorPan%3650E90500A0.initialDeclarations preserve=yes
  //## end CCursorPan%3650E90500A0.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CCursorPan%911272804
      CCursorPan ();

      //## Operation: CCursorPan%911272810
      CCursorPan (CToolBase* pCToolBase);


    //## Other Operations (specified)
      //## Operation: CursorInside%911272802
      BOOL CursorInside (int x, int y);

      //## Operation: CursorOutside%911272803
      BOOL CursorOutside (int x, int y);

    // Additional Public Declarations
      //## begin CCursorPan%3650E90500A0.public preserve=yes
      //## end CCursorPan%3650E90500A0.public

  protected:
    // Additional Protected Declarations
      //## begin CCursorPan%3650E90500A0.protected preserve=yes
      //## end CCursorPan%3650E90500A0.protected

  private:
    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: rose%3651151401B8
      const int get_rose () const;
      void set_rose (int value);

    // Additional Private Declarations
      //## begin CCursorPan%3650E90500A0.private preserve=yes
      //## end CCursorPan%3650E90500A0.private

  private:  //## implementation
    // Data Members for Class Attributes

      //## begin CCursorPan::rose%3651151401B8.attr preserve=no  private: int {U} 
      int rose;
      //## end CCursorPan::rose%3651151401B8.attr

    // Additional Implementation Declarations
      //## begin CCursorPan%3650E90500A0.implementation preserve=yes
      //## end CCursorPan%3650E90500A0.implementation

};

//## begin CCursorPan%3650E90500A0.postscript preserve=yes
//## end CCursorPan%3650E90500A0.postscript

// Class CCursorPan 

//## Get and Set Operations for Class Attributes (inline)

inline const int CCursorPan::get_rose () const
{
  //## begin CCursorPan::get_rose%3651151401B8.get preserve=no
  return rose;
  //## end CCursorPan::get_rose%3651151401B8.get
}

inline void CCursorPan::set_rose (int value)
{
  //## begin CCursorPan::set_rose%3651151401B8.set preserve=no
  rose = value;
  //## end CCursorPan::set_rose%3651151401B8.set
}

//## begin module%3650E90500A0.epilog preserve=yes
//## end module%3650E90500A0.epilog


#endif
