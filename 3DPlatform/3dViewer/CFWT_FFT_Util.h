//## begin module%3CAE55DB0334.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CAE55DB0334.cm

//## begin module%3CAE55DB0334.cp preserve=no
//## end module%3CAE55DB0334.cp

//## Module: CFWT_FFT_Util%3CAE55DB0334; Package specification
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: D:\H\desktop\HalfToneTest\bmpviewer--nxn\CFWT_FFT_Util.h

#ifndef CFWT_FFT_Util_h
#define CFWT_FFT_Util_h 1

//## begin module%3CAE55DB0334.additionalIncludes preserve=no
//## end module%3CAE55DB0334.additionalIncludes

//## begin module%3CAE55DB0334.includes preserve=yes
//## end module%3CAE55DB0334.includes

//## begin module%3CAE55DB0334.declarations preserve=no
//## end module%3CAE55DB0334.declarations

//## begin module%3CAE55DB0334.additionalDeclarations preserve=yes
//## end module%3CAE55DB0334.additionalDeclarations


//## begin CFWT_FFT_Util%3CAE50E701A2.preface preserve=yes
//## end CFWT_FFT_Util%3CAE50E701A2.preface

//## Class: CFWT_FFT_Util%3CAE50E701A2
//## Category: bmpviewer::Error Diffusion Classes::FWT%3CA94D4700BB
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//## Persistence: Transient
//## Cardinality/Multiplicity: 1..1

class CFWT_FFT_Util 
{
  //## begin CFWT_FFT_Util%3CAE50E701A2.initialDeclarations preserve=yes
  //## end CFWT_FFT_Util%3CAE50E701A2.initialDeclarations

  public:
    //## Constructors (generated)
      CFWT_FFT_Util();


    //## Other Operations (specified)
      //## Operation: nrerror%3CAE51450268
      static void nrerror (char error_text[]);

      //## Operation: vector%3CAE516F01ED
      static float* vector (long nl, long nh);

      //## Operation: free_vector%3CAE518601D0
      static void free_vector (float* v, long nl, long nh);

    // Additional Public Declarations
      //## begin CFWT_FFT_Util%3CAE50E701A2.public preserve=yes
      //## end CFWT_FFT_Util%3CAE50E701A2.public

  protected:
    // Additional Protected Declarations
      //## begin CFWT_FFT_Util%3CAE50E701A2.protected preserve=yes
      //## end CFWT_FFT_Util%3CAE50E701A2.protected

  private:
    // Additional Private Declarations
      //## begin CFWT_FFT_Util%3CAE50E701A2.private preserve=yes
      //## end CFWT_FFT_Util%3CAE50E701A2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CFWT_FFT_Util%3CAE50E701A2.implementation preserve=yes
      //## end CFWT_FFT_Util%3CAE50E701A2.implementation

};

//## begin CFWT_FFT_Util%3CAE50E701A2.postscript preserve=yes
//## end CFWT_FFT_Util%3CAE50E701A2.postscript

// Class CFWT_FFT_Util 

//## begin module%3CAE55DB0334.epilog preserve=yes
//## end module%3CAE55DB0334.epilog


#endif
