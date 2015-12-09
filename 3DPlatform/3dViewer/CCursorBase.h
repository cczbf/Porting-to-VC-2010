//## begin module%36510D3F01F4.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36510D3F01F4.cm

//## begin module%36510D3F01F4.cp preserve=no
//## end module%36510D3F01F4.cp

//## Module: CCursorBase%36510D3F01F4; Pseudo Package specification
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CCursorBase.h

#ifndef CCursorBase_h
#define CCursorBase_h 1

//## begin module%36510D3F01F4.additionalIncludes preserve=no
//## end module%36510D3F01F4.additionalIncludes

//## begin module%36510D3F01F4.includes preserve=yes
//## end module%36510D3F01F4.includes

// ctoolbase
#include "ctoolbase.h"
//## begin module%36510D3F01F4.additionalDeclarations preserve=yes
class CToolBase;
//## end module%36510D3F01F4.additionalDeclarations


//## Class: CCursorBase%36510D3F01F4
//## Category: bmpviewer::Tools%369E44800341
//## Subsystem: bmpviewer::Tools%369E44800341
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CCursorBase 
{
  //## begin CCursorBase%36510D3F01F4.initialDeclarations preserve=yes
  //## end CCursorBase%36510D3F01F4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CCursorBase%911272806
      CCursorBase (CToolBase* pCToolBase);

      //## Operation: CCursorBase%911272809
      CCursorBase ();


    //## Other Operations (specified)
      //## Operation: CursorInside%911272807
      virtual BOOL CursorInside (int x, int y) = 0;

      //## Operation: CursorOutside%911272808
      virtual BOOL CursorOutside (int x, int y) = 0;

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_nCurrentCCursor%36510D5B02DA
      const WORD get_m_nCurrentCCursor () const;
      void set_m_nCurrentCCursor (WORD value);

    // Data Members for Associations

      //## Association: <unnamed>%369D2F700319
      //## Role: CCursorBase::m_pCToolBase%369D2F710310
      //## begin CCursorBase::m_pCToolBase%369D2F710310.role preserve=no  public: CToolBase {1 -> 1RHGAN}
      CToolBase *m_pCToolBase;
      //## end CCursorBase::m_pCToolBase%369D2F710310.role

    // Additional Public Declarations
      //## begin CCursorBase%36510D3F01F4.public preserve=yes
      //## end CCursorBase%36510D3F01F4.public

  protected:
    // Additional Protected Declarations
      //## begin CCursorBase%36510D3F01F4.protected preserve=yes
      //## end CCursorBase%36510D3F01F4.protected

  private:
    // Additional Private Declarations
      //## begin CCursorBase%36510D3F01F4.private preserve=yes
      //## end CCursorBase%36510D3F01F4.private

  private:  //## implementation
    // Data Members for Class Attributes

      //## begin CCursorBase::m_nCurrentCCursor%36510D5B02DA.attr preserve=no  public: WORD {U} 
      WORD m_nCurrentCCursor;
      //## end CCursorBase::m_nCurrentCCursor%36510D5B02DA.attr

    // Additional Implementation Declarations
      //## begin CCursorBase%36510D3F01F4.implementation preserve=yes
      //## end CCursorBase%36510D3F01F4.implementation

};

//## begin CCursorBase%36510D3F01F4.postscript preserve=yes
//## end CCursorBase%36510D3F01F4.postscript

// Class CCursorBase 

//## Get and Set Operations for Class Attributes (inline)

inline const WORD CCursorBase::get_m_nCurrentCCursor () const
{
  //## begin CCursorBase::get_m_nCurrentCCursor%36510D5B02DA.get preserve=no
  return m_nCurrentCCursor;
  //## end CCursorBase::get_m_nCurrentCCursor%36510D5B02DA.get
}

inline void CCursorBase::set_m_nCurrentCCursor (WORD value)
{
  //## begin CCursorBase::set_m_nCurrentCCursor%36510D5B02DA.set preserve=no
  m_nCurrentCCursor = value;
  //## end CCursorBase::set_m_nCurrentCCursor%36510D5B02DA.set
}

//## begin module%36510D3F01F4.epilog preserve=yes
//## end module%36510D3F01F4.epilog


#endif
