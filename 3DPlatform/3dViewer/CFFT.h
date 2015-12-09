//## begin module%3886A050000B.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3886A050000B.cm

//## begin module%3886A050000B.cp preserve=no
//## end module%3886A050000B.cp

//## Module: CFFT%3886A050000B; Package specification
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: d:\Desktop\Halftonetest\bmpviewer0\CFFT.h

#ifndef CFFT_h
#define CFFT_h 1

//## begin module%3886A050000B.additionalIncludes preserve=no
//## end module%3886A050000B.additionalIncludes

//## begin module%3886A050000B.includes preserve=yes
//## end module%3886A050000B.includes

// CRawImage16
#include "CRawImage16.h"
//## begin module%3886A050000B.declarations preserve=no
//## end module%3886A050000B.declarations

//## begin module%3886A050000B.additionalDeclarations preserve=yes
//## end module%3886A050000B.additionalDeclarations


//## begin CFFT%388682310134.preface preserve=yes
//## end CFFT%388682310134.preface

//## Class: CFFT%388682310134
//## Category: bmpviewer::Error Diffusion Classes%3875916E0092
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3886CE2202E3;CRawImage16 { -> }

class CFFT 
{
  //## begin CFFT%388682310134.initialDeclarations preserve=yes
  //## end CFFT%388682310134.initialDeclarations

  public:
    //## Constructors (generated)
      CFFT();

    //## Destructor (specified)
      //## Operation: ~CFFT%3886AF6E009C
      ~CFFT ();


    //## Other Operations (specified)
      //## Operation: FFT2D%3886826F021A
      //	      //	2-dimensional FFT Calculation Program
      //	      void FFT2D ();
      void FFT2D ();

      //## Operation: IFFT2D%3886827A02C0
      void IFFT2D ();

      //## Operation: CalcFrequency%388E55540295
      void CalcFrequency (WORD NumSamples, 	//  must be integer power of 2
      WORD FrequencyIndex, 	// { must be in the range 0 .. NumSamples-1 }
      double* lpRealIn, double* lpImagIn, double& RealOut, double& ImagOut);

      //## Operation: SetWorkingImage%3886CC990143
      void SetWorkingImage (CRawImage16* lpCRawFTImage);

      //## Operation: CopyReal%388D136D030E
      void CopyReal ();

      //## Operation: CopyImagnary%388D137B0282
      void CopyImagnary ();

      //## Operation: CopyMagnitude%388D15CA03D6
      void CopyMagnitude ();

      //## Operation: CopyPhaseAngle%388D15F200C7
      void CopyPhaseAngle ();

      //## Operation: MeanGrayOf_image%3B60C90C0034
      //	Calculateing the mean gray of the image contained in the
      //	member variable 'image'.
      double MeanGrayOf_image ();

      //## Operation: CopyPeridogram%3B6111FE03E3
      void CopyPeridogram ();

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_N%3890EA710285
      //	Image size:   m_M x m_n
      //	i.e.: m_M rows and m_N columns
      const int get_m_N () const;
      void set_m_N (int value);

    // Data Members for Class Attributes

      //## Attribute: m_M%3886914C02CD
      //	Image size:   m_M x m_n
      //	i.e.: m_M rows and m_N columns
      //## begin CFFT::m_M%3886914C02CD.attr preserve=no  public: int {UA} 64
      int m_M;
      //## end CFFT::m_M%3886914C02CD.attr

      //## begin CFFT::m_N%3890EA710285.attr preserve=no  public: int {UA} 64
      int m_N;
      //## end CFFT::m_N%3890EA710285.attr

      //## Attribute: image%3886ABFD029E
      //## begin CFFT::image%3886ABFD029E.attr preserve=no  public: unsigned _int16* {UA} NULL
      unsigned _int16* image;
      //## end CFFT::image%3886ABFD029E.attr

      //## Attribute: zr%3886AEE301E6
      //## begin CFFT::zr%3886AEE301E6.attr preserve=no  public: float* {UA} NULL
      float* zr;
      //## end CFFT::zr%3886AEE301E6.attr

      //## Attribute: zi%3886AEF201B6
      //## begin CFFT::zi%3886AEF201B6.attr preserve=no  public: float* {UA} NULL
      float* zi;
      //## end CFFT::zi%3886AEF201B6.attr

    // Additional Public Declarations
      //## begin CFFT%388682310134.public preserve=yes
      //## end CFFT%388682310134.public

  protected:
    // Additional Protected Declarations
      //## begin CFFT%388682310134.protected preserve=yes
      //## end CFFT%388682310134.protected

  private:
    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_dblMeanGrayOfSourceImage%3B6112B0033F
      const double get_m_dblMeanGrayOfSourceImage () const;
      void set_m_dblMeanGrayOfSourceImage (double value);

    // Data Members for Class Attributes

      //## Attribute: m_fMaxReal%388D10060103
      //## begin CFFT::m_fMaxReal%388D10060103.attr preserve=no  private: float {UA} FLT_MIN
      float m_fMaxReal;
      //## end CFFT::m_fMaxReal%388D10060103.attr

      //## Attribute: m_fMinReal%388D16D200DD
      //## begin CFFT::m_fMinReal%388D16D200DD.attr preserve=no  private: float {UA} FLT_MAX
      float m_fMinReal;
      //## end CFFT::m_fMinReal%388D16D200DD.attr

      //## Attribute: m_fMaxImagnary%388D16DD0115
      //## begin CFFT::m_fMaxImagnary%388D16DD0115.attr preserve=no  private: float {UA} FLT_MIN
      float m_fMaxImagnary;
      //## end CFFT::m_fMaxImagnary%388D16DD0115.attr

      //## Attribute: m_fMinImagnary%388D16FE0271
      //## begin CFFT::m_fMinImagnary%388D16FE0271.attr preserve=no  private: float {UA} FLT_MAX
      float m_fMinImagnary;
      //## end CFFT::m_fMinImagnary%388D16FE0271.attr

      //## Attribute: m_lpCRawFTImage%3886CCE20102
      //## begin CFFT::m_lpCRawFTImage%3886CCE20102.attr preserve=no  private: CRawImage16* {UA} 
      CRawImage16* m_lpCRawFTImage;
      //## end CFFT::m_lpCRawFTImage%3886CCE20102.attr

      //## begin CFFT::m_dblMeanGrayOfSourceImage%3B6112B0033F.attr preserve=no  private: double {UA} 128.0
      double m_dblMeanGrayOfSourceImage;
      //## end CFFT::m_dblMeanGrayOfSourceImage%3B6112B0033F.attr

    // Additional Private Declarations
      //## begin CFFT%388682310134.private preserve=yes
      //## end CFFT%388682310134.private

  private: //## implementation

    //## Other Operations (specified)
      //## Operation: FFT%388682BD03AD
      //	Purpose: Performaing 1-dimensional FFT
      //	Arguments:
      //	A	-- FFT origin / result real part
      //	B	-- FFT origin / result imagine part
      void FFT (double* A, double* B, int m);

      //## Operation: DFT%388E5501012D
      void DFT (double* dR, double* dI, int m);

    // Additional Implementation Declarations
      //## begin CFFT%388682310134.implementation preserve=yes
      //## end CFFT%388682310134.implementation

};

//## begin CFFT%388682310134.postscript preserve=yes
//## end CFFT%388682310134.postscript

// Class CFFT 

//## Get and Set Operations for Class Attributes (inline)

inline const int CFFT::get_m_N () const
{
  //## begin CFFT::get_m_N%3890EA710285.get preserve=no
  return m_N;
  //## end CFFT::get_m_N%3890EA710285.get
}

inline void CFFT::set_m_N (int value)
{
  //## begin CFFT::set_m_N%3890EA710285.set preserve=no
  m_N = value;
  //## end CFFT::set_m_N%3890EA710285.set
}

inline const double CFFT::get_m_dblMeanGrayOfSourceImage () const
{
  //## begin CFFT::get_m_dblMeanGrayOfSourceImage%3B6112B0033F.get preserve=no
  return m_dblMeanGrayOfSourceImage;
  //## end CFFT::get_m_dblMeanGrayOfSourceImage%3B6112B0033F.get
}

inline void CFFT::set_m_dblMeanGrayOfSourceImage (double value)
{
  //## begin CFFT::set_m_dblMeanGrayOfSourceImage%3B6112B0033F.set preserve=no
  m_dblMeanGrayOfSourceImage = value;
  //## end CFFT::set_m_dblMeanGrayOfSourceImage%3B6112B0033F.set
}

//## begin module%3886A050000B.epilog preserve=yes
//## end module%3886A050000B.epilog


#endif
