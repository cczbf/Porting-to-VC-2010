//## begin module%368281740297.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%368281740297.cm

//## begin module%368281740297.cp preserve=no
//## end module%368281740297.cp

//## Module: CHtArray2Dib%368281740297; Package specification
//## Subsystem: bmpviewer%364BBA5B0050
//	d:\desktop\halftonetest\bmpviewer
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CHtArray2Dib.h

#ifndef CHtArray2Dib_h
#define CHtArray2Dib_h 1

//## begin module%368281740297.additionalIncludes preserve=no
//## end module%368281740297.additionalIncludes

//## begin module%368281740297.includes preserve=yes
//## end module%368281740297.includes

// cdib
#include "cdib.h"
// nttype
#include "nttype.h"
//## begin module%368281740297.declarations preserve=no
//## end module%368281740297.declarations

//## begin module%368281740297.additionalDeclarations preserve=yes
//## end module%368281740297.additionalDeclarations


//## Class: CHtArray2Dib%3681913401AE
//	m_dwLevels:uint32
//	m_dwLimitLevels:uint32
//	m_dwWidth:uint32
//	m_dwHeight: uint32
//
//	 dwScaleX: uint32  dwScaleY: uint32
//## Category: bmpviewer::Core%369F6C81018D
//## Subsystem: bmpviewer%364BBA5B0050
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CHtArray2Dib : public CDIB  //## Inherits: <unnamed>%36819154010A
{
public:
  //## begin CHtArray2Dib%3681913401AE.initialDeclarations preserve=yes
	
	//added by xdh,2004.11.12.
	//To construct a new dib using the color buffer data
	HANDLE CreateDibFromColorBuffer(int nWid, int nHei, BYTE *pBuffer = NULL);
	HANDLE CreateDibFromRawData(int nWid, int nHei, BYTE *pRawData = NULL);
	//////////////////////////////////////////////////////////////////////////
	
  //## end CHtArray2Dib%3681913401AE.initialDeclarations

  public:
    //## Constructors (generated)
      CHtArray2Dib();

    //## Destructor (generated)
      ~CHtArray2Dib();


    //## Other Operations (specified)
      //## Operation: OpenHtArrayDIB%914519251
      HANDLE OpenHtArrayDIB (LPSTR lpFileName);

      //## Operation: HtArray2Dib%914519253
      HANDLE HtArray2Dib (HANDLE hHtArray);

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_dwLevels%36827EFB0279
      //	m_dwLevels:uint32
      //	m_dwLimitLevels:uint32
      //	m_dwWidth:uint32
      //	m_dwHeight: uint32
      const uint32 get_m_dwLevels () const;
      void set_m_dwLevels (uint32 value);

      //## Attribute: m_dwLimitLevels%36827F2003B3
      //	m_dwLimitLevels:uint32
      //	m_dwWidth:uint32
      //	m_dwHeight: uint32
      const uint32 get_m_dwLimitLevels () const;
      void set_m_dwLimitLevels (uint32 value);

      //## Attribute: m_dwWidth%36827F5C0296
      //	m_dwWidth:uint32
      //	m_dwWidth:uint32
      //	m_dwHeight: uint32
      const uint32 get_m_dwWidth () const;
      void set_m_dwWidth (uint32 value);

      //## Attribute: m_dwHeight%36827F65039E
      const uint32 get_m_dwHeight () const;
      void set_m_dwHeight (uint32 value);

      //## Attribute: m_dwScaleX%36828EEB0031
      const uint32 get_m_dwScaleX () const;
      void set_m_dwScaleX (uint32 value);

      //## Attribute: m_dwScaleY%36828F040312
      const uint32 get_m_dwScaleY () const;
      void set_m_dwScaleY (uint32 value);

    // Data Members for Class Attributes

      //## begin CHtArray2Dib::m_dwLevels%36827EFB0279.attr preserve=no  public: uint32 {UA} 0
      uint32 m_dwLevels;
      //## end CHtArray2Dib::m_dwLevels%36827EFB0279.attr

      //## begin CHtArray2Dib::m_dwLimitLevels%36827F2003B3.attr preserve=no  public: uint32 {UA} 0
      uint32 m_dwLimitLevels;
      //## end CHtArray2Dib::m_dwLimitLevels%36827F2003B3.attr

      //## begin CHtArray2Dib::m_dwWidth%36827F5C0296.attr preserve=no  public: uint32 {UA} 0
      uint32 m_dwWidth;
      //## end CHtArray2Dib::m_dwWidth%36827F5C0296.attr

      //## begin CHtArray2Dib::m_dwHeight%36827F65039E.attr preserve=no  public: uint32 {UA} 0
      uint32 m_dwHeight;
      //## end CHtArray2Dib::m_dwHeight%36827F65039E.attr

      //## begin CHtArray2Dib::m_dwScaleX%36828EEB0031.attr preserve=no  public: uint32 {UA} 1
      uint32 m_dwScaleX;
      //## end CHtArray2Dib::m_dwScaleX%36828EEB0031.attr

      //## begin CHtArray2Dib::m_dwScaleY%36828F040312.attr preserve=no  public: uint32 {UA} 1
      uint32 m_dwScaleY;
      //## end CHtArray2Dib::m_dwScaleY%36828F040312.attr

    // Additional Public Declarations
      //## begin CHtArray2Dib%3681913401AE.public preserve=yes
      //## end CHtArray2Dib%3681913401AE.public

  protected:
    // Additional Protected Declarations
      //## begin CHtArray2Dib%3681913401AE.protected preserve=yes
      //## end CHtArray2Dib%3681913401AE.protected

  private:
    // Additional Private Declarations
      //## begin CHtArray2Dib%3681913401AE.private preserve=yes
      //## end CHtArray2Dib%3681913401AE.private

  private:  //## implementation

    //## Other Operations (specified)
      //## Operation: IsValidNetFile%914537967
      //	BOOL CNetTable::IsValidNetFile ( HANDLE hnetfile )
      //	{
      //	    FILETIME MainProgramWriteTime, writetime ;
      //	    if ( !GetMainProgramWriteTime ( &MainProgramWrite
      //	Time ) )
      //	        return FALSE ;
      //
      //	    uint32 readsize ;
      //	    ReadFile ( hnetfile, &writetime, sizeof(FILETIME),
      //	(LPDWORD)&readsize, NULL ) ;
      //	    if ( readsize != sizeof(FILETIME) )
      //	        return FALSE ;
      //
      //	    if ( writetime.dwLowDateTime != MainProgramWrite
      //	Time.dwLowDateTime ||
      //	         writetime.dwHighDateTime != MainProgramWrite
      //	Time.dwHighDateTime )
      //	        return FALSE ;
      //
      //	    return TRUE ;
      //	}
      BOOL IsValidNetFile (HANDLE hnetfile);

      //## Operation: GetHtArrayData%914519252
      HANDLE GetHtArrayData (LPSTR lpFileName);

      //## Operation: DecodeFrmData%914523962
      //	    void DecodeFrmData ( LPUINT16 lpFrm, uint32 dwWidth,
      //	uint32 dwHeight, uint32 dwScaleX, uint32 dwScaleY ) ;
      void DecodeFrmData (LPUINT16 lpFrm, uint32 dwWidth, uint32 dwHeight, uint32 dwScaleX, uint32 dwScaleY);

    // Additional Implementation Declarations
      //## begin CHtArray2Dib%3681913401AE.implementation preserve=yes
      //## end CHtArray2Dib%3681913401AE.implementation

};

//## begin CHtArray2Dib%3681913401AE.postscript preserve=yes
//## end CHtArray2Dib%3681913401AE.postscript

// Class CHtArray2Dib 

//## Get and Set Operations for Class Attributes (inline)

inline const uint32 CHtArray2Dib::get_m_dwLevels () const
{
  //## begin CHtArray2Dib::get_m_dwLevels%36827EFB0279.get preserve=no
  return m_dwLevels;
  //## end CHtArray2Dib::get_m_dwLevels%36827EFB0279.get
}

inline void CHtArray2Dib::set_m_dwLevels (uint32 value)
{
  //## begin CHtArray2Dib::set_m_dwLevels%36827EFB0279.set preserve=no
  m_dwLevels = value;
  //## end CHtArray2Dib::set_m_dwLevels%36827EFB0279.set
}

inline const uint32 CHtArray2Dib::get_m_dwLimitLevels () const
{
  //## begin CHtArray2Dib::get_m_dwLimitLevels%36827F2003B3.get preserve=no
  return m_dwLimitLevels;
  //## end CHtArray2Dib::get_m_dwLimitLevels%36827F2003B3.get
}

inline void CHtArray2Dib::set_m_dwLimitLevels (uint32 value)
{
  //## begin CHtArray2Dib::set_m_dwLimitLevels%36827F2003B3.set preserve=no
  m_dwLimitLevels = value;
  //## end CHtArray2Dib::set_m_dwLimitLevels%36827F2003B3.set
}

inline const uint32 CHtArray2Dib::get_m_dwWidth () const
{
  //## begin CHtArray2Dib::get_m_dwWidth%36827F5C0296.get preserve=no
  return m_dwWidth;
  //## end CHtArray2Dib::get_m_dwWidth%36827F5C0296.get
}

inline void CHtArray2Dib::set_m_dwWidth (uint32 value)
{
  //## begin CHtArray2Dib::set_m_dwWidth%36827F5C0296.set preserve=no
  m_dwWidth = value;
  //## end CHtArray2Dib::set_m_dwWidth%36827F5C0296.set
}

inline const uint32 CHtArray2Dib::get_m_dwHeight () const
{
  //## begin CHtArray2Dib::get_m_dwHeight%36827F65039E.get preserve=no
  return m_dwHeight;
  //## end CHtArray2Dib::get_m_dwHeight%36827F65039E.get
}

inline void CHtArray2Dib::set_m_dwHeight (uint32 value)
{
  //## begin CHtArray2Dib::set_m_dwHeight%36827F65039E.set preserve=no
  m_dwHeight = value;
  //## end CHtArray2Dib::set_m_dwHeight%36827F65039E.set
}

inline const uint32 CHtArray2Dib::get_m_dwScaleX () const
{
  //## begin CHtArray2Dib::get_m_dwScaleX%36828EEB0031.get preserve=no
  return m_dwScaleX;
  //## end CHtArray2Dib::get_m_dwScaleX%36828EEB0031.get
}

inline void CHtArray2Dib::set_m_dwScaleX (uint32 value)
{
  //## begin CHtArray2Dib::set_m_dwScaleX%36828EEB0031.set preserve=no
  m_dwScaleX = value;
  //## end CHtArray2Dib::set_m_dwScaleX%36828EEB0031.set
}

inline const uint32 CHtArray2Dib::get_m_dwScaleY () const
{
  //## begin CHtArray2Dib::get_m_dwScaleY%36828F040312.get preserve=no
  return m_dwScaleY;
  //## end CHtArray2Dib::get_m_dwScaleY%36828F040312.get
}

inline void CHtArray2Dib::set_m_dwScaleY (uint32 value)
{
  //## begin CHtArray2Dib::set_m_dwScaleY%36828F040312.set preserve=no
  m_dwScaleY = value;
  //## end CHtArray2Dib::set_m_dwScaleY%36828F040312.set
}

//## begin module%368281740297.epilog preserve=yes
//## end module%368281740297.epilog


#endif
