//## begin module%3CAE55E20013.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CAE55E20013.cm

//## begin module%3CAE55E20013.cp preserve=no
//## end module%3CAE55E20013.cp

//## Module: CFWT_FFT_Util%3CAE55E20013; Package body
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: D:\H\desktop\HalfToneTest\bmpviewer--nxn\CFWT_FFT_Util.cpp

//## begin module%3CAE55E20013.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%3CAE55E20013.additionalIncludes

//## begin module%3CAE55E20013.includes preserve=yes
//## end module%3CAE55E20013.includes

// CFWT_FFT_Util
#include "CFWT_FFT_Util.h"
//## begin module%3CAE55E20013.declarations preserve=no
//## end module%3CAE55E20013.declarations

//## begin module%3CAE55E20013.additionalDeclarations preserve=yes
#define NR_END 1
#define FREE_ARG char*

//## end module%3CAE55E20013.additionalDeclarations


// Class CFWT_FFT_Util 


CFWT_FFT_Util::CFWT_FFT_Util()
  //## begin CFWT_FFT_Util::CFWT_FFT_Util%.hasinit preserve=no
  //## end CFWT_FFT_Util::CFWT_FFT_Util%.hasinit
  //## begin CFWT_FFT_Util::CFWT_FFT_Util%.initialization preserve=yes
  //## end CFWT_FFT_Util::CFWT_FFT_Util%.initialization
{
  //## begin CFWT_FFT_Util::CFWT_FFT_Util%.body preserve=yes
  //## end CFWT_FFT_Util::CFWT_FFT_Util%.body
}



//## Other Operations (implementation)
void CFWT_FFT_Util::nrerror (char error_text[])
{
  //## begin CFWT_FFT_Util::nrerror%3CAE51450268.body preserve=yes

	TRACE("Numerical Recipes run-time error...\n");
	TRACE("%s\n",error_text);
	TRACE("...now exiting to system...\n");

	
	exit(1);


  //## end CFWT_FFT_Util::nrerror%3CAE51450268.body
}

float * CFWT_FFT_Util::vector (long nl, long nh)
{
  //## begin CFWT_FFT_Util::vector%3CAE516F01ED.body preserve=yes

/* allocate a float vector with subscript range v[nl..nh] */
	float *v;
	v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
	if (!v) 
		nrerror("allocation failure in vector()");
	return v-nl+NR_END;


  //## end CFWT_FFT_Util::vector%3CAE516F01ED.body
}

void CFWT_FFT_Util::free_vector (float *v, long nl, long nh)
{
  //## begin CFWT_FFT_Util::free_vector%3CAE518601D0.body preserve=yes

	/* free a float vector allocated with vector() */
	free((FREE_ARG) (v+nl-NR_END));
  
  //## end CFWT_FFT_Util::free_vector%3CAE518601D0.body
}

// Additional Declarations
  //## begin CFWT_FFT_Util%3CAE50E701A2.declarations preserve=yes
  //## end CFWT_FFT_Util%3CAE50E701A2.declarations

//## begin module%3CAE55E20013.epilog preserve=yes
//## end module%3CAE55E20013.epilog
