//## begin module%3CAE4A130234.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CAE4A130234.cm

//## begin module%3CAE4A130234.cp preserve=no
//## end module%3CAE4A130234.cp

//## Module: CWaveletFilterBase%3CAE4A130234; Package specification
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: D:\H\desktop\HalfToneTest\bmpviewer--nxn\CWaveletFilterBase.h

#ifndef CWaveletFilterBase_h
#define CWaveletFilterBase_h 1

//## begin module%3CAE4A130234.additionalIncludes preserve=no
//## end module%3CAE4A130234.additionalIncludes

//## begin module%3CAE4A130234.includes preserve=yes
//## end module%3CAE4A130234.includes

// CFWT_FFT_Util
#include "CFWT_FFT_Util.h"
//## begin module%3CAE4A130234.declarations preserve=no
//## end module%3CAE4A130234.declarations

//## begin module%3CAE4A130234.additionalDeclarations preserve=yes
//## end module%3CAE4A130234.additionalDeclarations


//## begin wavefilt%3CB0E6CF03C3.preface preserve=yes
//## end wavefilt%3CB0E6CF03C3.preface

//## Class: wavefilt%3CB0E6CF03C3
//	extern wavefilt wfilt;
//	typedef struct {
//		int ncof,ioff,joff;
//		float *cc,*cr;
//	} wavefilt;
//## Category: bmpviewer::Error Diffusion Classes::FWT%3CA94D4700BB
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct wavefilt 
{
  //## begin wavefilt%3CB0E6CF03C3.initialDeclarations preserve=yes
  //## end wavefilt%3CB0E6CF03C3.initialDeclarations

    //## Constructors (generated)
      wavefilt();

    // Data Members for Class Attributes

      //## Attribute: ncof%3CB0EDF2036E
      //## begin wavefilt::ncof%3CB0EDF2036E.attr preserve=no  public: int {UA} 
      int ncof;
      //## end wavefilt::ncof%3CB0EDF2036E.attr

      //## Attribute: ioff%3CB0EE0F0269
      //## begin wavefilt::ioff%3CB0EE0F0269.attr preserve=no  public: int {UA} 
      int ioff;
      //## end wavefilt::ioff%3CB0EE0F0269.attr

      //## Attribute: joff%3CB0EE2200DE
      //## begin wavefilt::joff%3CB0EE2200DE.attr preserve=no  public: int {UA} 
      int joff;
      //## end wavefilt::joff%3CB0EE2200DE.attr

      //## Attribute: cc%3CB0EE30035E
      //## begin wavefilt::cc%3CB0EE30035E.attr preserve=no  public: float* {UA} 
      float* cc;
      //## end wavefilt::cc%3CB0EE30035E.attr

      //## Attribute: cr%3CB0EE420100
      //## begin wavefilt::cr%3CB0EE420100.attr preserve=no  public: float* {UA} 
      float* cr;
      //## end wavefilt::cr%3CB0EE420100.attr

  public:
    // Additional Public Declarations
      //## begin wavefilt%3CB0E6CF03C3.public preserve=yes
      //## end wavefilt%3CB0E6CF03C3.public

  protected:
    // Additional Protected Declarations
      //## begin wavefilt%3CB0E6CF03C3.protected preserve=yes
      //## end wavefilt%3CB0E6CF03C3.protected

  private:
    // Additional Private Declarations
      //## begin wavefilt%3CB0E6CF03C3.private preserve=yes
      //## end wavefilt%3CB0E6CF03C3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin wavefilt%3CB0E6CF03C3.implementation preserve=yes
      //## end wavefilt%3CB0E6CF03C3.implementation

};

//## begin wavefilt%3CB0E6CF03C3.postscript preserve=yes
//## end wavefilt%3CB0E6CF03C3.postscript

//## begin CWaveletFilterBase%3CAE46D60292.preface preserve=yes
//## end CWaveletFilterBase%3CAE46D60292.preface

//## Class: CWaveletFilterBase%3CAE46D60292; Abstract
//## Category: bmpviewer::Error Diffusion Classes::FWT%3CA94D4700BB
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3CAE5779008E;CFWT_FFT_Util { -> }

class CWaveletFilterBase 
{
  //## begin CWaveletFilterBase%3CAE46D60292.initialDeclarations preserve=yes
  //## end CWaveletFilterBase%3CAE46D60292.initialDeclarations

  public:
    //## Constructors (generated)
      CWaveletFilterBase();


    //## Other Operations (specified)
      //## Operation: wtstep%3CAE470801AA
      virtual void wtstep (float* a, unsigned long n, int isign);

    // Data Members for Associations

      //## Association: bmpviewer::Error Diffusion Classes::FWT::<unnamed>%3CAE57B90027
      //## Role: CWaveletFilterBase::m_CFWT_FFT_Util%3CAE57B902BB
      //## begin CWaveletFilterBase::m_CFWT_FFT_Util%3CAE57B902BB.role preserve=no  public: CFWT_FFT_Util { -> VHgAN}
      CFWT_FFT_Util m_CFWT_FFT_Util;
      //## end CWaveletFilterBase::m_CFWT_FFT_Util%3CAE57B902BB.role

      //## Association: bmpviewer::Error Diffusion Classes::FWT::<unnamed>%3CB0E74002C2
      //## Role: CWaveletFilterBase::wfilt%3CB0E7410128
      //## begin CWaveletFilterBase::wfilt%3CB0E7410128.role preserve=no  public: wavefilt { -> VHgAN}
      wavefilt wfilt;
      //## end CWaveletFilterBase::wfilt%3CB0E7410128.role

    // Additional Public Declarations
      //## begin CWaveletFilterBase%3CAE46D60292.public preserve=yes
      //## end CWaveletFilterBase%3CAE46D60292.public

  protected:
    // Additional Protected Declarations
      //## begin CWaveletFilterBase%3CAE46D60292.protected preserve=yes
      //## end CWaveletFilterBase%3CAE46D60292.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_nFilterID%3CAE4E5E01A0
      //## begin CWaveletFilterBase::m_nFilterID%3CAE4E5E01A0.attr preserve=no  private: int {UA} 0
      int m_nFilterID;
      //## end CWaveletFilterBase::m_nFilterID%3CAE4E5E01A0.attr

    // Additional Private Declarations
      //## begin CWaveletFilterBase%3CAE46D60292.private preserve=yes
      //## end CWaveletFilterBase%3CAE46D60292.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CWaveletFilterBase%3CAE46D60292.implementation preserve=yes
      //## end CWaveletFilterBase%3CAE46D60292.implementation

};

//## begin CWaveletFilterBase%3CAE46D60292.postscript preserve=yes
//## end CWaveletFilterBase%3CAE46D60292.postscript

//## begin Cdaub4%3CAE478501BE.preface preserve=yes
//## end Cdaub4%3CAE478501BE.preface

//## Class: Cdaub4%3CAE478501BE
//## Category: bmpviewer::Error Diffusion Classes::FWT%3CA94D4700BB
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class Cdaub4 : public CWaveletFilterBase  //## Inherits: <unnamed>%3CAE478F0095
{
  //## begin Cdaub4%3CAE478501BE.initialDeclarations preserve=yes
  //## end Cdaub4%3CAE478501BE.initialDeclarations

  public:
    //## Constructors (generated)
      Cdaub4();


    //## Other Operations (specified)
      //## Operation: wtstep%3CAE489601AA
      void wtstep (float* a, unsigned long n, int isign);

    // Additional Public Declarations
      //## begin Cdaub4%3CAE478501BE.public preserve=yes
      //## end Cdaub4%3CAE478501BE.public

  protected:
    // Additional Protected Declarations
      //## begin Cdaub4%3CAE478501BE.protected preserve=yes
      //## end Cdaub4%3CAE478501BE.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: nFilterID%3CAE4E95014B
      //## begin Cdaub4::nFilterID%3CAE4E95014B.attr preserve=no  private: static int {UAC} 0
      static const int nFilterID;
      //## end Cdaub4::nFilterID%3CAE4E95014B.attr

    // Additional Private Declarations
      //## begin Cdaub4%3CAE478501BE.private preserve=yes
      //## end Cdaub4%3CAE478501BE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin Cdaub4%3CAE478501BE.implementation preserve=yes
      //## end Cdaub4%3CAE478501BE.implementation

};

//## begin Cdaub4%3CAE478501BE.postscript preserve=yes
//## end Cdaub4%3CAE478501BE.postscript

//## begin Cpwt%3CAE48670233.preface preserve=yes
//## end Cpwt%3CAE48670233.preface

//## Class: Cpwt%3CAE48670233
//## Category: bmpviewer::Error Diffusion Classes::FWT%3CA94D4700BB
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class Cpwt : public CWaveletFilterBase  //## Inherits: <unnamed>%3CAE488103BE
{
  //## begin Cpwt%3CAE48670233.initialDeclarations preserve=yes
  //## end Cpwt%3CAE48670233.initialDeclarations

  public:
    //## Constructors (generated)
      Cpwt();

    //## Constructors (specified)
      //## Operation: Cpwt%3CB0F021027D
      Cpwt (int n);


    //## Other Operations (specified)
      //## Operation: wtstep%3CAE4B430331
      void wtstep (float* a, unsigned long n, int isign);

      //## Operation: pwtset%3CB0E3E803E1
      //	pwtset(n : int) : void
      //
      //		Initializing routine for pwt, here implementing the
      //	Daubechies wavelet filters with 4, 12, and
      //	20 coe.cients, as selected by the input value n. Further
      //	wavelet filters can be included in the
      //	obvious manner. This routine must be called (once)
      //	before the first use of pwt. (Forthecase
      //	n=4, the specific routine daub4 is considerably faster
      //	than pwt.)
      void pwtset (int n);

    // Additional Public Declarations
      //## begin Cpwt%3CAE48670233.public preserve=yes
      //## end Cpwt%3CAE48670233.public

  protected:
    // Additional Protected Declarations
      //## begin Cpwt%3CAE48670233.protected preserve=yes
      //## end Cpwt%3CAE48670233.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: nFilterID%3CAE5022028B
      //## begin Cpwt::nFilterID%3CAE5022028B.attr preserve=no  private: static int {UA} 1
      static int nFilterID;
      //## end Cpwt::nFilterID%3CAE5022028B.attr

    // Additional Private Declarations
      //## begin Cpwt%3CAE48670233.private preserve=yes
      //## end Cpwt%3CAE48670233.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin Cpwt%3CAE48670233.implementation preserve=yes
      //## end Cpwt%3CAE48670233.implementation

};

//## begin Cpwt%3CAE48670233.postscript preserve=yes
//## end Cpwt%3CAE48670233.postscript

// Class wavefilt 

// Class CWaveletFilterBase 

// Class Cdaub4 

// Class Cpwt 

//## begin module%3CAE4A130234.epilog preserve=yes
//## end module%3CAE4A130234.epilog


#endif
