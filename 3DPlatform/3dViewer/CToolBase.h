//## begin module%369D2EB00056.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369D2EB00056.cm

//## begin module%369D2EB00056.cp preserve=no
//## end module%369D2EB00056.cp

//## Module: CToolBase%369D2EB00056; Pseudo Package specification
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CToolBase.h

#ifndef CToolBase_h
#define CToolBase_h 1

//## begin module%369D2EB00056.additionalIncludes preserve=no
//## end module%369D2EB00056.additionalIncludes

//## begin module%369D2EB00056.includes preserve=yes
//## end module%369D2EB00056.includes

// cbmpviewerdata
#include "cbmpviewerdata.h"
// CCursorBase
#include "CCursorBase.h"
//## begin module%369D2EB00056.additionalDeclarations preserve=yes
class CCursorBase;

//## end module%369D2EB00056.additionalDeclarations


//## Class: CToolBase%369D2EB00056
//## Category: bmpviewer::Tools%369E44800341
//## Subsystem: bmpviewer::Tools%369E44800341
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CToolBase 
{
  //## begin CToolBase%369D2EB00056.initialDeclarations preserve=yes
  //## end CToolBase%369D2EB00056.initialDeclarations

  public:
    //## Constructors (generated)
      CToolBase();


    //## Other Operations (specified)
      //## Operation: OnLButtonUp%916266560
      virtual BOOL OnLButtonUp (UINT nFlags, CPoint point) = 0;

      //## Operation: OnLButtonDown%916266561
      virtual BOOL OnLButtonDown (UINT nFlags, CPoint point) = 0;

       //## Operation: OnLButtonUp%916266560
      virtual BOOL OnMButtonUp (UINT nFlags, CPoint point) = 0;
      //## Operation: OnLButtonDown%916266561
      virtual BOOL OnMButtonDown (UINT nFlags, CPoint point) = 0;

	  //## Operation: OnLButtonUp%916266560
      virtual BOOL OnRButtonUp (UINT nFlags, CPoint point) = 0;
      //## Operation: OnLButtonDown%916266561
      virtual BOOL OnRButtonDown (UINT nFlags, CPoint point) = 0;

      //## Operation: OnMouseMove%916266562
      virtual BOOL OnMouseMove (UINT nFlags, CPoint point) = 0;

      //## Operation: OnLBDblClick%916274418
      virtual BOOL OnLBDblClick (UINT nFlags, CPoint point) = 0;

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_nToolID%369D2ED6005A
      const int get_m_nToolID () const;
      void  set_m_nToolID (int value);

    //## Get and Set Operations for Associations (generated)

      //## Association: <unnamed>%369D30C5025B
      //## Role: CToolBase::m_pCBmpViewerData%369D30C602D4
      const CBmpViewerData * get_m_pCBmpViewerData () const;
      void set_m_pCBmpViewerData (CBmpViewerData * value);

    // Data Members for Associations

      //## Association: <unnamed>%369D2F700319
      //## Role: CToolBase::m_pCursorBase%369D2F710306
      //## begin CToolBase::m_pCursorBase%369D2F710306.role preserve=no  public: CCursorBase {1 -> 1RHAN}
      CCursorBase *m_pCursorBase;
      //## end CToolBase::m_pCursorBase%369D2F710306.role

      //## Association: <unnamed>%369D30C5025B
      //## begin CToolBase::m_pCBmpViewerData%369D30C602D4.role preserve=no  public: CBmpViewerData { -> 1RHAN}
      CBmpViewerData *m_pCBmpViewerData;
      //## end CToolBase::m_pCBmpViewerData%369D30C602D4.role

    // Additional Public Declarations
      //## begin CToolBase%369D2EB00056.public preserve=yes
      //## end CToolBase%369D2EB00056.public

  protected:
    // Additional Protected Declarations
      //## begin CToolBase%369D2EB00056.protected preserve=yes
      //## end CToolBase%369D2EB00056.protected

  private:
    // Additional Private Declarations
      //## begin CToolBase%369D2EB00056.private preserve=yes
      //## end CToolBase%369D2EB00056.private

  private:  //## implementation
    // Data Members for Class Attributes

      //## begin CToolBase::m_nToolID%369D2ED6005A.attr preserve=no  public: int {U} 00
      int m_nToolID;
      //## end CToolBase::m_nToolID%369D2ED6005A.attr

    // Additional Implementation Declarations
      //## begin CToolBase%369D2EB00056.implementation preserve=yes
      //## end CToolBase%369D2EB00056.implementation

};

//## begin CToolBase%369D2EB00056.postscript preserve=yes
//## end CToolBase%369D2EB00056.postscript

// Class CToolBase 

//## Get and Set Operations for Class Attributes (inline)

inline const int CToolBase::get_m_nToolID () const
{
  //## begin CToolBase::get_m_nToolID%369D2ED6005A.get preserve=no
  return m_nToolID;
  //## end CToolBase::get_m_nToolID%369D2ED6005A.get
}

inline void CToolBase::set_m_nToolID (int value)
{
  //## begin CToolBase::set_m_nToolID%369D2ED6005A.set preserve=no
  m_nToolID = value;
  //## end CToolBase::set_m_nToolID%369D2ED6005A.set
}

//## Get and Set Operations for Associations (inline)

inline const CBmpViewerData * CToolBase::get_m_pCBmpViewerData () const
{
  //## begin CToolBase::get_m_pCBmpViewerData%369D30C602D4.get preserve=no
  return m_pCBmpViewerData;
  //## end CToolBase::get_m_pCBmpViewerData%369D30C602D4.get
}

inline void CToolBase::set_m_pCBmpViewerData (CBmpViewerData * value)
{
  //## begin CToolBase::set_m_pCBmpViewerData%369D30C602D4.set preserve=no
  m_pCBmpViewerData = value;
  //## end CToolBase::set_m_pCBmpViewerData%369D30C602D4.set
}

//## begin module%369D2EB00056.epilog preserve=yes
//## end module%369D2EB00056.epilog


#endif
