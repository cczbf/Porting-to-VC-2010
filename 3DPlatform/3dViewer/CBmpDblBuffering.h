//## begin module%364FD6E4033E.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%364FD6E4033E.cm

//## begin module%364FD6E4033E.cp preserve=no
//## end module%364FD6E4033E.cp

//## Module: CBmpDblBuffering%364FD6E4033E; Pseudo Package specification
//## Subsystem: bmpviewer%364BBA5B0050
//## Source file: c:\windows\desktop\roselecture\bmpviewer\CBmpDblBuffering.h

#ifndef CBmpDblBuffering_h
#define CBmpDblBuffering_h 1

//## begin module%364FD6E4033E.additionalIncludes preserve=no
//## end module%364FD6E4033E.additionalIncludes

//## begin module%364FD6E4033E.includes preserve=yes
//## end module%364FD6E4033E.includes

//## begin module%364FD6E4033E.additionalDeclarations preserve=yes
//## end module%364FD6E4033E.additionalDeclarations


//## Class: CBmpDblBuffering%364FD6E4033E
//## Category: bmpviewer%364BBA5A0028
//## Subsystem: bmpviewer%364BBA5B0050
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CBmpDblBuffering 
{
  //## begin CBmpDblBuffering%364FD6E4033E.initialDeclarations preserve=yes
  //## end CBmpDblBuffering%364FD6E4033E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CBmpDblBuffering%911203803
      CBmpDblBuffering (HBITMAP hBmpBackBuf = NULL);

    //## Destructor (specified)
      //## Operation: ~CBmpDblBuffering%911203804
      ~CBmpDblBuffering ();


    //## Other Operations (specified)
      //## Operation: BeginDblBuf%911203808
      HDC BeginDblBuf (HDC hDestDC, HBITMAP hBmpBackBuf = NULL);

      //## Operation: GetDC%911203807
      HDC GetDC ();

      //## Operation: EndDblBuf%911203809
      void EndDblBuf ();

      //## Operation: Show%911203806
      void Show ();

    // Additional Public Declarations
      //## begin CBmpDblBuffering%364FD6E4033E.public preserve=yes
      //## end CBmpDblBuffering%364FD6E4033E.public

  protected:
    // Additional Protected Declarations
      //## begin CBmpDblBuffering%364FD6E4033E.protected preserve=yes
      //## end CBmpDblBuffering%364FD6E4033E.protected

  private:
    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_hDestDC%364FD728033E
      const HDC get_m_hDestDC () const;
      void set_m_hDestDC (HDC value);

      //## Attribute: m_hMemDC%364FD74103DE
      const HDC get_m_hMemDC () const;
      void set_m_hMemDC (HDC value);

      //## Attribute: m_hBmpBackBuf%364FE26C0028
      const HBITMAP get_m_hBmpBackBuf () const;
      void set_m_hBmpBackBuf (HBITMAP value);

    // Additional Private Declarations
      //## begin CBmpDblBuffering%364FD6E4033E.private preserve=yes
      //## end CBmpDblBuffering%364FD6E4033E.private

  private:  //## implementation
    // Data Members for Class Attributes

      //## begin CBmpDblBuffering::m_hDestDC%364FD728033E.attr preserve=no  private: HDC {U} NULL
      HDC m_hDestDC;
      //## end CBmpDblBuffering::m_hDestDC%364FD728033E.attr

      //## begin CBmpDblBuffering::m_hMemDC%364FD74103DE.attr preserve=no  private: HDC {U} NULL
      HDC m_hMemDC;
      //## end CBmpDblBuffering::m_hMemDC%364FD74103DE.attr

      //## begin CBmpDblBuffering::m_hBmpBackBuf%364FE26C0028.attr preserve=no  private: HBITMAP {U} NULL
      HBITMAP m_hBmpBackBuf;
      //## end CBmpDblBuffering::m_hBmpBackBuf%364FE26C0028.attr

    // Additional Implementation Declarations
      //## begin CBmpDblBuffering%364FD6E4033E.implementation preserve=yes
      //## end CBmpDblBuffering%364FD6E4033E.implementation

};

//## begin CBmpDblBuffering%364FD6E4033E.postscript preserve=yes
//## end CBmpDblBuffering%364FD6E4033E.postscript

// Class CBmpDblBuffering 

//## Get and Set Operations for Class Attributes (inline)

inline const HDC CBmpDblBuffering::get_m_hDestDC () const
{
  //## begin CBmpDblBuffering::get_m_hDestDC%364FD728033E.get preserve=no
  return m_hDestDC;
  //## end CBmpDblBuffering::get_m_hDestDC%364FD728033E.get
}

inline void CBmpDblBuffering::set_m_hDestDC (HDC value)
{
  //## begin CBmpDblBuffering::set_m_hDestDC%364FD728033E.set preserve=no
  m_hDestDC = value;
  //## end CBmpDblBuffering::set_m_hDestDC%364FD728033E.set
}

inline const HDC CBmpDblBuffering::get_m_hMemDC () const
{
  //## begin CBmpDblBuffering::get_m_hMemDC%364FD74103DE.get preserve=no
  return m_hMemDC;
  //## end CBmpDblBuffering::get_m_hMemDC%364FD74103DE.get
}

inline void CBmpDblBuffering::set_m_hMemDC (HDC value)
{
  //## begin CBmpDblBuffering::set_m_hMemDC%364FD74103DE.set preserve=no
  m_hMemDC = value;
  //## end CBmpDblBuffering::set_m_hMemDC%364FD74103DE.set
}

inline const HBITMAP CBmpDblBuffering::get_m_hBmpBackBuf () const
{
  //## begin CBmpDblBuffering::get_m_hBmpBackBuf%364FE26C0028.get preserve=no
  return m_hBmpBackBuf;
  //## end CBmpDblBuffering::get_m_hBmpBackBuf%364FE26C0028.get
}

inline void CBmpDblBuffering::set_m_hBmpBackBuf (HBITMAP value)
{
  //## begin CBmpDblBuffering::set_m_hBmpBackBuf%364FE26C0028.set preserve=no
  m_hBmpBackBuf = value;
  //## end CBmpDblBuffering::set_m_hBmpBackBuf%364FE26C0028.set
}

//## begin module%364FD6E4033E.epilog preserve=yes
//## end module%364FD6E4033E.epilog


#endif
