//## begin module%3CAA59280135.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CAA59280135.cm

//## begin module%3CAA59280135.cp preserve=no
//## end module%3CAA59280135.cp

//## Module: CTransformedImage%3CAA59280135; Package specification
//## Subsystem: bmpviewer::Globals%38869EB800D6
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: D:\H\desktop\HalfToneTest\bmpviewer--nxn\CTransformedImage.h

#ifndef CTransformedImage_h
#define CTransformedImage_h 1

//## begin module%3CAA59280135.additionalIncludes preserve=no
//## end module%3CAA59280135.additionalIncludes

//## begin module%3CAA59280135.includes preserve=yes
//## end module%3CAA59280135.includes

//## begin module%3CAA59280135.declarations preserve=no
//## end module%3CAA59280135.declarations

//## begin module%3CAA59280135.additionalDeclarations preserve=yes
//## end module%3CAA59280135.additionalDeclarations


//## begin CTransformedImage%3CAA5889035E.preface preserve=yes
//## end CTransformedImage%3CAA5889035E.preface

//## Class: CTransformedImage%3CAA5889035E
//## Category: bmpviewer::Globals%3693E06F035F
//## Subsystem: bmpviewer::Globals%38869EB800D6
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CTransformedImage 
{
  //## begin CTransformedImage%3CAA5889035E.initialDeclarations preserve=yes
  //## end CTransformedImage%3CAA5889035E.initialDeclarations

  public:
    //## Constructors (generated)
      CTransformedImage();

    //## Destructor (specified)
      //## Operation: ~CTransformedImage%3CAA6A99021A
      ~CTransformedImage ();


    //## Other Operations (specified)
      //## Operation: SetData%3CAA66570291
      //	<<>> SetData(lpData : float*, nW : __int32, nH : __
      //	int32) : void
      //
      //	COPY the data into the class and expand the memory
      //	buffer accordingly.
      void SetData (float* lpData, __int32 nW, __int32 nH);

      //## Operation: GetData%3CAA69EB0334
      void GetData (float* lpData, __int32* nW, __int32* nH);

      //## Operation: GetData%3CAACC3F03BF
      float* GetData (__int32* nW, __int32* nH);

    // Data Members for Class Attributes

      //## Attribute: m_nTransformFilterID%3CB0FC63029D
      //	m_nTransformFilterID = 0
      //	FWT daub 4
      //	m_nTransformFilterID = 1
      //	FWT daub 4 (SLOW)
      //	m_nTransformFilterID = 2
      //	FWT daub 12
      //	m_nTransformFilterID = 3
      //	FWT daub 20
      //## begin CTransformedImage::m_nTransformFilterID%3CB0FC63029D.attr preserve=no  public: __int16 {UA} 0
      __int16 m_nTransformFilterID;
      //## end CTransformedImage::m_nTransformFilterID%3CB0FC63029D.attr

    // Additional Public Declarations
      //## begin CTransformedImage%3CAA5889035E.public preserve=yes
      //## end CTransformedImage%3CAA5889035E.public

  protected:
    // Additional Protected Declarations
      //## begin CTransformedImage%3CAA5889035E.protected preserve=yes
      //## end CTransformedImage%3CAA5889035E.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_lpData%3CAA589302E9
      //## begin CTransformedImage::m_lpData%3CAA589302E9.attr preserve=no  private: float* {UA} NULL
      float* m_lpData;
      //## end CTransformedImage::m_lpData%3CAA589302E9.attr

      //## Attribute: m_nW%3CAA58CC0034
      //## begin CTransformedImage::m_nW%3CAA58CC0034.attr preserve=no  private: __int32 {UA} 
      __int32 m_nW;
      //## end CTransformedImage::m_nW%3CAA58CC0034.attr

      //## Attribute: m_nH%3CAA58DA011A
      //## begin CTransformedImage::m_nH%3CAA58DA011A.attr preserve=no  private: __int32 {UA} 
      __int32 m_nH;
      //## end CTransformedImage::m_nH%3CAA58DA011A.attr

    // Additional Private Declarations
      //## begin CTransformedImage%3CAA5889035E.private preserve=yes
      //## end CTransformedImage%3CAA5889035E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CTransformedImage%3CAA5889035E.implementation preserve=yes
      //## end CTransformedImage%3CAA5889035E.implementation

};

//## begin CTransformedImage%3CAA5889035E.postscript preserve=yes
//## end CTransformedImage%3CAA5889035E.postscript

// Class CTransformedImage 

//## begin module%3CAA59280135.epilog preserve=yes
//## end module%3CAA59280135.epilog


#endif
