//## begin module%3875951801DC.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3875951801DC.cm

//## begin module%3875951801DC.cp preserve=no
//## end module%3875951801DC.cp

//## Module: CRawImage16%3875951801DC; Pseudo Package specification
//## Source file: d:\desktop\halftonetest\bmpviewer\CRawImage16.h

#ifndef CRawImage16_h
#define CRawImage16_h 1

//## begin module%3875951801DC.additionalIncludes preserve=no
//## end module%3875951801DC.additionalIncludes

//## begin module%3875951801DC.includes preserve=yes
//## end module%3875951801DC.includes

//## begin module%3875951801DC.additionalDeclarations preserve=yes
//## end module%3875951801DC.additionalDeclarations


//## begin CRawImage16%3875951801DC.preface preserve=yes
//## end CRawImage16%3875951801DC.preface

//## Class: CRawImage16%3875951801DC
//## Category: bmpviewer::Error Diffusion Classes%3875916E0092
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//	虽然CRawImage16使用16位的象素存储，但CErrorDiffusion 及其
//	导出类只使用8位格式(包括输入、输出图象)。即16位的象素值只
//	用了低8位。

class CRawImage16 
{
  //## begin CRawImage16%3875951801DC.initialDeclarations preserve=yes
  //## end CRawImage16%3875951801DC.initialDeclarations

  public:
    //## Constructors (generated)
      CRawImage16();

    //## Constructors (specified)
      //## Operation: CRawImage16%3879414A02F5
      CRawImage16 (unsigned _int32 nWidth, unsigned _int32 nHeight);

    //## Destructor (generated)
      ~CRawImage16();


    //## Other Operations (specified)
      //## Operation: Copy%38759640000B
      void Copy (UINT* lpPixels);

      //## Operation: Copy%387BF64A01B3
      void Copy (HANDLE hRaw, unsigned _int32 nW, unsigned _int32 nH);

      //## Operation: Copy%38759C080257
      void Copy (char* szFileName);

      //## Operation: Copy%38759D9901FA
      void Copy (UINT nDensity, unsigned _int32 nW = 0, unsigned _int32 nH = 0);

	  
      //## Operation: ExpandToPowerOf2Dim%3CAAEC53022A
      void ExpandToPowerOf2Dim (unsigned _int32* nW = NULL, unsigned _int32* nH = NULL);


      //## Operation: DestroyPixelBuffer%387A83A50262
      void DestroyPixelBuffer ();
/*
    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_nWidth%38792D72023C
      const unsigned _int32 get_m_nWidth () const;
      void set_m_nWidth (unsigned _int32 value);

      //## Attribute: m_nHeight%38792E0A0032
      const unsigned _int32 get_m_nHeight () const;
      void set_m_nHeight (unsigned _int32 value);

      //## Attribute: m_lpPixels%3879419A024F
      const unsigned _int16* get_m_lpPixels () const;
      void set_m_lpPixels (unsigned _int16* value);

      //## Attribute: m_hPixels%387941EB0224
      const HANDLE get_m_hPixels () const;
      void set_m_hPixels (HANDLE value);
*/
    // Data Members for Class Attributes

      //## begin CRawImage16::m_nWidth%38792D72023C.attr preserve=no  public: unsigned _int32 {UA} 256
      unsigned _int32 m_nWidth;
      //## end CRawImage16::m_nWidth%38792D72023C.attr

      //## begin CRawImage16::m_nHeight%38792E0A0032.attr preserve=no  public: unsigned _int32 {UA} 256
      unsigned _int32 m_nHeight;
      //## end CRawImage16::m_nHeight%38792E0A0032.attr

      //## begin CRawImage16::m_lpPixels%3879419A024F.attr preserve=no  public: unsigned _int16* {UA} NULL
      unsigned _int16* m_lpPixels;
      //## end CRawImage16::m_lpPixels%3879419A024F.attr

      //## begin CRawImage16::m_hPixels%387941EB0224.attr preserve=no  public: HANDLE {UA} NULL
      HANDLE m_hPixels;
      //## end CRawImage16::m_hPixels%387941EB0224.attr

    // Additional Public Declarations
      //## begin CRawImage16%3875951801DC.public preserve=yes
      //## end CRawImage16%3875951801DC.public

  protected:
    // Additional Protected Declarations
      //## begin CRawImage16%3875951801DC.protected preserve=yes
      //## end CRawImage16%3875951801DC.protected

  private:
    // Additional Private Declarations
      //## begin CRawImage16%3875951801DC.private preserve=yes
      //## end CRawImage16%3875951801DC.private
	    // Data Members for Class Attributes

      //## Attribute: m_nMaxPixelValue%38AA052100FA
      //## begin CRawImage16::m_nMaxPixelValue%38AA052100FA.attr preserve=no  private: unsigned _int16 {UA} 255
      unsigned _int16 m_nMaxPixelValue;
      //## end CRawImage16::m_nMaxPixelValue%38AA052100FA.attr

      //## Attribute: m_nMinPixelValue%38AA05360014
      //## begin CRawImage16::m_nMinPixelValue%38AA05360014.attr preserve=no  private: unsigned _int16 {UA} 0
      unsigned _int16 m_nMinPixelValue;
      //## end CRawImage16::m_nMinPixelValue%38AA05360014.attr


  private: //## implementation
    // Additional Implementation Declarations
      //## begin CRawImage16%3875951801DC.implementation preserve=yes
      //## end CRawImage16%3875951801DC.implementation

};

//## begin CRawImage16%3875951801DC.postscript preserve=yes
//## end CRawImage16%3875951801DC.postscript

// Class CRawImage16 

//## Get and Set Operations for Class Attributes (inline)
/*
inline const unsigned _int32 CRawImage16::get_m_nWidth () const
{
  //## begin CRawImage16::get_m_nWidth%38792D72023C.get preserve=no
  return m_nWidth;
  //## end CRawImage16::get_m_nWidth%38792D72023C.get
}

inline void CRawImage16::set_m_nWidth (unsigned _int32 value)
{
  //## begin CRawImage16::set_m_nWidth%38792D72023C.set preserve=no
  m_nWidth = value;
  //## end CRawImage16::set_m_nWidth%38792D72023C.set
}

inline const unsigned _int32 CRawImage16::get_m_nHeight () const
{
  //## begin CRawImage16::get_m_nHeight%38792E0A0032.get preserve=no
  return m_nHeight;
  //## end CRawImage16::get_m_nHeight%38792E0A0032.get
}

inline void CRawImage16::set_m_nHeight (unsigned _int32 value)
{
  //## begin CRawImage16::set_m_nHeight%38792E0A0032.set preserve=no
  m_nHeight = value;
  //## end CRawImage16::set_m_nHeight%38792E0A0032.set
}

inline const unsigned _int16* CRawImage16::get_m_lpPixels () const
{
  //## begin CRawImage16::get_m_lpPixels%3879419A024F.get preserve=no
  return m_lpPixels;
  //## end CRawImage16::get_m_lpPixels%3879419A024F.get
}

inline void CRawImage16::set_m_lpPixels (unsigned _int16* value)
{
  //## begin CRawImage16::set_m_lpPixels%3879419A024F.set preserve=no
  m_lpPixels = value;
  //## end CRawImage16::set_m_lpPixels%3879419A024F.set
}

inline const HANDLE CRawImage16::get_m_hPixels () const
{
  //## begin CRawImage16::get_m_hPixels%387941EB0224.get preserve=no
  return m_hPixels;
  //## end CRawImage16::get_m_hPixels%387941EB0224.get
}

inline void CRawImage16::set_m_hPixels (HANDLE value)
{
  //## begin CRawImage16::set_m_hPixels%387941EB0224.set preserve=no
  m_hPixels = value;
  //## end CRawImage16::set_m_hPixels%387941EB0224.set
}

//## begin module%3875951801DC.epilog preserve=yes
//## end module%3875951801DC.epilog
*/

#endif
