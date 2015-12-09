//## begin module%3CA9584C039F.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CA9584C039F.cm

//## begin module%3CA9584C039F.cp preserve=no
//## end module%3CA9584C039F.cp

//## Module: CFWT%3CA9584C039F; Package specification
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: D:\H\desktop\HalfToneTest\bmpviewer--nxn\CFWT.h

#ifndef CFWT_h
#define CFWT_h 1

//## begin module%3CA9584C039F.additionalIncludes preserve=no
//## end module%3CA9584C039F.additionalIncludes

//## begin module%3CA9584C039F.includes preserve=yes
//## end module%3CA9584C039F.includes

// CFFT
#include "CFFT.h"
// CWaveletFilterBase
#include "CWaveletFilterBase.h"
//## begin module%3CA9584C039F.declarations preserve=no
//## end module%3CA9584C039F.declarations

//## begin module%3CA9584C039F.additionalDeclarations preserve=yes
#define void__cdecl void __cdecl 
//## end module%3CA9584C039F.additionalDeclarations


//## begin CFWT%3CA951C80236.preface preserve=yes
//## end CFWT%3CA951C80236.preface

//## Class: CFWT%3CA951C80236
//	Class for FWT(Fast Wavelet Ttransform)
//	Author: ZhouBingFeng
//	Date: 2002.4.2
//	Reference: Neumerical Receipes in C,pp,591-717
//## Category: bmpviewer::Error Diffusion Classes::FWT%3CA94D4700BB
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CFWT : public CFFT  //## Inherits: <unnamed>%3CA983C30229
{
  //## begin CFWT%3CA951C80236.initialDeclarations preserve=yes
  //## end CFWT%3CA951C80236.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CFWT%529D795BFEED; C++
      //	/////////////////////////////////////////////////////////
      //	///////////
      //	Construction/Destruction
      //	/////////////////////////////////////////////////////////
      //	///////////
      CFWT (int n);

    //## Destructor (specified)
      //## Operation: ~CFWT%438A7BA1FEED; C++
      virtual ~CFWT ();


    //## Other Operations (specified)
      //## Operation: wtn%9F8A71D8FEED; C++
      //	Replaces a by its ndim-dimensional discrete wavelet
      //	transform, if isign is input as 1. Here
      //	nn[1..ndim] is an integer array containing the lengths
      //	of each dimension (number of real
      //	values), which MUST all be powers of 2. a is a real
      //	array of length equal to the product of
      //	these lengths, in which the data are stored as in a
      //	multidimensional real array. If isign is input
      //	as -1, a is replaced by its inverse wavelet transform.
      //	The routine wtstep, whose actual name
      //	must be supplied in calling this routine, is the
      //	underlying wavelet filter. Examples of wtstep
      //	are daub4 and (preceded by pwtset) pwt.*/
      void wtn (float* a, unsigned long* nn, int ndim, int isign);

      //## Operation: daub4%4D4443B6FEED; C++
      void daub4 (float* a, unsigned long n, int isign);

      //## Operation: FWT2D%3CA97210009A
      void FWT2D ();

      //## Operation: IFWT2D%3CAAD2660297
      void IFWT2D ();

      //## Operation: CopyWT%3CA9841D00CE
      //	CopyWT():
      //
      //	Format the result and copy to internal CRawImage buffer
      void CopyWT (BOOL bInDecibel = TRUE);

      //## Operation: CopyWTReal%3CAA6FC50315
      //	CopyWTReal(lpData : float*, nM : long*, nN : long*)
      //	:
      //	Copy the result in zr ( a member buffer, defined in its
      //	parent CFFT) out to lpData
      void CopyWTReal (float* lpData, long* nW, long* nH);

      //## Operation: GetWTReal%3CAACEF5013D
      float* GetWTReal (long* nW = NULL, long* nH = NULL);

      //## Operation: SetReal4Transform%3CAA767A0300
      void SetReal4Transform (float* lpData, long nW, long nH);

      //## Operation: SetWaveletFiletr%3CAE4E0D0006
      //	n = 0:
      //	daub 4
      //
      //	n = 1:
      //	daub 4 (slow)
      //	n = 2:
      //	daub 12
      //	n = 3
      //	daub 20
      void SetWaveletFiletr (int nFilterID = 2);

    // Data Members for Associations

      //## Association: bmpviewer::Error Diffusion Classes::FWT::<unnamed>%3CAE4C0802FD
      //## Role: CFWT::m_pCWaveletFilterBase%3CAE4C0901BE
      //## begin CFWT::m_pCWaveletFilterBase%3CAE4C0901BE.role preserve=no  public: CWaveletFilterBase { -> RHgAN}
      CWaveletFilterBase *m_pCWaveletFilterBase;
      //## end CFWT::m_pCWaveletFilterBase%3CAE4C0901BE.role

    // Additional Public Declarations
      //## begin CFWT%3CA951C80236.public preserve=yes
      //## end CFWT%3CA951C80236.public

  protected:
    // Additional Protected Declarations
      //## begin CFWT%3CA951C80236.protected preserve=yes
      //## end CFWT%3CA951C80236.protected

  private:

    //## Other Operations (specified)
      //## Operation: nrerror%3CA957790292
      //	void nrerror(char error_text[])
      void nrerror (char error_text[]);

      //## Operation: vector%3CA957B70297
      //	float *vector(long nl, long nh)
      float* vector (long nl, long nh);

      //## Operation: free_vector%3CA95A080393
      //	void free_vector(float *v, long nl, long nh)
      void free_vector (float* v, long nl, long nh);

    // Data Members for Class Attributes

      //## Attribute: m%3CA988AA02D1
      //	m,n is at the boundry of power of 2. they are the size
      //	of float working buffer.
      //	m is HEIGHT (i.e. number of rows)
      //	n is WIDTH (i.e. number of columns)!
      //## begin CFWT::m%3CA988AA02D1.attr preserve=no  private: long {UA} 
      long m;
      //## end CFWT::m%3CA988AA02D1.attr

      //## Attribute: n%3CA988B202C8
      //	m,n is at the boundry of power of 2.
      //## begin CFWT::n%3CA988B202C8.attr preserve=no  private: long {UA} 
      long n;
      //## end CFWT::n%3CA988B202C8.attr

    // Additional Private Declarations
      //## begin CFWT%3CA951C80236.private preserve=yes
      //## end CFWT%3CA951C80236.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CFWT%3CA951C80236.implementation preserve=yes
      //## end CFWT%3CA951C80236.implementation

};

//## begin CFWT%3CA951C80236.postscript preserve=yes
//## end CFWT%3CA951C80236.postscript

// Class CFWT 

//## begin module%3CA9584C039F.epilog preserve=yes
//## end module%3CA9584C039F.epilog


#endif
