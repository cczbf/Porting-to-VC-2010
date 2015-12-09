//## begin module%3693F3C400EF.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3693F3C400EF.cm

//## begin module%3693F3C400EF.cp preserve=no
//## end module%3693F3C400EF.cp

//## Module: CDIBOrg%3693F3C400EF; Pseudo Package specification
//## Subsystem: bmpviewer::Core%369F6C81018D
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CDIBOrg.h

#ifndef CDIBOrg_h
#define CDIBOrg_h 1

//## begin module%3693F3C400EF.additionalIncludes preserve=no
//## end module%3693F3C400EF.additionalIncludes

//## begin module%3693F3C400EF.includes preserve=yes
//## end module%3693F3C400EF.includes

// cdib
#include "cdib.h"
//## begin module%3693F3C400EF.additionalDeclarations preserve=yes
//## end module%3693F3C400EF.additionalDeclarations


//## Class: CDIBOrg%3693F3C400EF
//## Category: bmpviewer::Core%369F6C81018D
//## Subsystem: bmpviewer::Core%369F6C81018D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CDIBOrg : public CDIB  //## Inherits: <unnamed>%3693F478001B
{
  //## begin CDIBOrg%3693F3C400EF.initialDeclarations preserve=yes
  //## end CDIBOrg%3693F3C400EF.initialDeclarations

  public:
    //## Constructors (generated)
      CDIBOrg();

    //## Destructor (generated)
      ~CDIBOrg();


    //## Other Operations (specified)
      //## Operation: FreeDib%916955276
      void FreeDib ();

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_hDib%3693F3DF0170
      const HANDLE get_m_hDib () const;
      void set_m_hDib (HANDLE value);

    // Data Members for Class Attributes

      //## begin CDIBOrg::m_hDib%3693F3DF0170.attr preserve=no  public: HANDLE {UA} NULL
      HANDLE m_hDib;
      //## end CDIBOrg::m_hDib%3693F3DF0170.attr

    // Additional Public Declarations
      //## begin CDIBOrg%3693F3C400EF.public preserve=yes
      //## end CDIBOrg%3693F3C400EF.public

  protected:
    // Additional Protected Declarations
      //## begin CDIBOrg%3693F3C400EF.protected preserve=yes
      //## end CDIBOrg%3693F3C400EF.protected

  private:
    // Additional Private Declarations
      //## begin CDIBOrg%3693F3C400EF.private preserve=yes
      //## end CDIBOrg%3693F3C400EF.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin CDIBOrg%3693F3C400EF.implementation preserve=yes
      //## end CDIBOrg%3693F3C400EF.implementation

};

//## begin CDIBOrg%3693F3C400EF.postscript preserve=yes
//## end CDIBOrg%3693F3C400EF.postscript

// Class CDIBOrg 

//## Get and Set Operations for Class Attributes (inline)

inline const HANDLE CDIBOrg::get_m_hDib () const
{
  //## begin CDIBOrg::get_m_hDib%3693F3DF0170.get preserve=no
  return m_hDib;
  //## end CDIBOrg::get_m_hDib%3693F3DF0170.get
}

inline void CDIBOrg::set_m_hDib (HANDLE value)
{
  //## begin CDIBOrg::set_m_hDib%3693F3DF0170.set preserve=no
  m_hDib = value;
  //## end CDIBOrg::set_m_hDib%3693F3DF0170.set
}

//## begin module%3693F3C400EF.epilog preserve=yes
//## end module%3693F3C400EF.epilog


#endif
