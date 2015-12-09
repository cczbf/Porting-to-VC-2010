//## begin module%3CA958560349.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CA958560349.cm

//## begin module%3CA958560349.cp preserve=no
//## end module%3CA958560349.cp

//## Module: CFWT%3CA958560349; Package body
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: D:\H\desktop\HalfToneTest\bmpviewer--nxn\CFWT.cpp

//## begin module%3CA958560349.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%3CA958560349.additionalIncludes

//## begin module%3CA958560349.includes preserve=yes
#include <math.h>
#include <FLOAT.H>

//#include <stdio.h>
//## end module%3CA958560349.includes

// CFWT
#include "CFWT.h"
//## begin module%3CA958560349.declarations preserve=no
//## end module%3CA958560349.declarations

//## begin module%3CA958560349.additionalDeclarations preserve=yes
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define NR_END 1
#define FREE_ARG char*
//## end module%3CA958560349.additionalDeclarations


// Class CFWT 




CFWT::CFWT (int n)
  //## begin CFWT::CFWT%529D795BFEED.hasinit preserve=no
      : m_pCWaveletFilterBase(NULL)
  //## end CFWT::CFWT%529D795BFEED.hasinit
  //## begin CFWT::CFWT%529D795BFEED.initialization preserve=yes
  //## end CFWT::CFWT%529D795BFEED.initialization
{
  //## begin CFWT::CFWT%529D795BFEED.body preserve=yes
	SetWaveletFiletr(n);
  //## end CFWT::CFWT%529D795BFEED.body
}


CFWT::~CFWT ()
{
  //## begin CFWT::~CFWT%438A7BA1FEED.body preserve=yes
	if (m_pCWaveletFilterBase !=NULL)
	{
		delete m_pCWaveletFilterBase;
		m_pCWaveletFilterBase	= NULL;
	}
  //## end CFWT::~CFWT%438A7BA1FEED.body
}



//## Other Operations (implementation)
void CFWT::wtn (float* a, unsigned long* nn, int ndim, int isign)
{
  //## begin CFWT::wtn%9F8A71D8FEED.body preserve=yes
/*
Replaces a by its ndim-dimensional discrete wavelet transform, if isign is input as 1. Here
nn[1..ndim] is an integer array containing the lengths of each dimension (number of real
values), which MUST all be powers of 2. a is a real array of length equal to the product of
these lengths, in which the data are stored as in a multidimensional real array. If isign is input
as -1, a is replaced by its inverse wavelet transform. The routine wtstep, whose actual name
must be supplied in calling this routine, is the underlying wavelet filter. Examples of wtstep
are daub4 and (preceded by pwtset) pwt.*/

	unsigned long i1,i2,i3,k,n,nnew,nprev=1,nt,ntot=1;
	int idim;
	float *wksp;

	for (idim=1;idim<=ndim;idim++) ntot *= nn[idim];
	wksp=vector(1,ntot);
	for (idim=1;idim<=ndim;idim++) {
		n=nn[idim];
		nnew=n*nprev;
		if (n > 4) {
			for (i2=0;i2<ntot;i2+=nnew) {
				for (i1=1;i1<=nprev;i1++) {
					for (i3=i1+i2,k=1;k<=n;k++,i3+=nprev) wksp[k]=a[i3];
					if (isign >= 0) {
						for(nt=n;nt>=4;nt >>= 1)
//							(*wtstep)(wksp,nt,isign);
//							daub4(wksp,nt,isign);
							m_pCWaveletFilterBase->wtstep(wksp,nt,isign);
					} else {
						for(nt=4;nt<=n;nt <<= 1)
//							(*wtstep)(wksp,nt,isign);
//							daub4(wksp,nt,isign);
							m_pCWaveletFilterBase->wtstep(wksp,nt,isign);
					}

					for (i3=i1+i2,k=1;k<=n;k++,i3+=nprev) a[i3]=wksp[k];
				}
			}
		}
		nprev=nnew;
	}
	free_vector(wksp,1,ntot);
//    return 1	;
  //## end CFWT::wtn%9F8A71D8FEED.body
}

void CFWT::daub4 (float* a, unsigned long n, int isign)
{
  //## begin CFWT::daub4%4D4443B6FEED.body preserve=yes
/*
Applies the Daubechies 4-coefficient wavelet filter to data vector a[1..n] (for isign=1) or
applies its transpose (for isign=-1). Used hierarchically by routines wt1 and wtn.
*/

#define C0 0.4829629131445341
#define C1 0.8365163037378079
#define C2 0.2241438680420134
#define C3 -0.1294095225512604

	float *wksp;
	unsigned long nh,nh1,i,j;

	if (n < 4) return;
	wksp=vector(1,n);
	nh1=(nh=n >> 1)+1;
	if (isign >= 0) {
		for (i=1,j=1;j<=n-3;j+=2,i++) {
			wksp[i]    = (float)(C0*a[j]+C1*a[j+1]+C2*a[j+2]+C3*a[j+3]);
			wksp[i+nh] = (float)(C3*a[j]-C2*a[j+1]+C1*a[j+2]-C0*a[j+3]);
		}
		wksp[i]		= (float)(C0*a[n-1]+C1*a[n]+C2*a[1]+C3*a[2]);
		wksp[i+nh]	= (float)(C3*a[n-1]-C2*a[n]+C1*a[1]-C0*a[2]);
	} else {
		wksp[1]	= (float)(C2*a[nh]+C1*a[n]+C0*a[1]+C3*a[nh1]);
		wksp[2] = (float)(C3*a[nh]-C0*a[n]+C1*a[1]-C2*a[nh1]);
		for (i=1,j=3;i<nh;i++) {
			wksp[j++]	= (float)(C2*a[i]+C1*a[i+nh]+C0*a[i+1]+C3*a[i+nh1]);
			wksp[j++]	= (float)(C3*a[i]-C0*a[i+nh]+C1*a[i+1]-C2*a[i+nh1]);
		}
	}
	for (i=1;i<=n;i++) a[i]=wksp[i];
	free_vector(wksp,1,n);

#undef C0
#undef C1
#undef C2
#undef C3

  //## end CFWT::daub4%4D4443B6FEED.body
}

void CFWT::nrerror (char error_text[])
{
  //## begin CFWT::nrerror%3CA957790292.body preserve=yes
/* Numerical Recipes standard error handler */

//	fprintf(stderr,"Numerical Recipes run-time error...\n");
//	fprintf(stderr,"%s\n",error_text);
//	fprintf(stderr,"...now exiting to system...\n");

	TRACE("Numerical Recipes run-time error...\n");
	TRACE("%s\n",error_text);
	TRACE("...now exiting to system...\n");

	
	exit(1);


  //## end CFWT::nrerror%3CA957790292.body
}

float * CFWT::vector (long nl, long nh)
{
  //## begin CFWT::vector%3CA957B70297.body preserve=yes
/* allocate a float vector with subscript range v[nl..nh] */
	float *v;
	v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
	if (!v) 
		nrerror("allocation failure in vector()");
	return v-nl+NR_END;

  //## end CFWT::vector%3CA957B70297.body
}

void CFWT::free_vector (float *v, long nl, long nh)
{
  //## begin CFWT::free_vector%3CA95A080393.body preserve=yes
/* free a float vector allocated with vector() */
	free((FREE_ARG) (v+nl-NR_END));
  //## end CFWT::free_vector%3CA95A080393.body
}

void CFWT::FWT2D ()
{
  //## begin CFWT::FWT2D%3CA97210009A.body preserve=yes

//	long m,n;
	m = (long)(log((double)m_M)/log(2.0));
	if (((1<<m)-m_M)!=0)
		m++;
	n = (long)(log((double)m_N)/log(2.0));
	if (((1<<n)-m_N)!=0)
		n++;
	m = (1<<m);
	n = (1<<n);
	this->zr = (float*)malloc((m*n+1)*sizeof(float));

	for ( int i = 0; i <= m*n; i++)
		zr[i] = (float)0.0;
	/* copy image to zr array */ 
	for ( i = 0; i < m_M; i++ )//for each row (m_M, m  are Heights)
		for ( int j = 0; j < m_N; j++ )//for each element in the row (m_N, n are Widths)
	{
		zr[i*n+j+1]	= image[i*m_N+j];
	}

	//  this->wtn(a : float*, nn : unsigned long*, ndim : int, isign : int, wtstep : void (*) (float*, DWORD, int)) : void
	unsigned long nn[3];
//	nn[1] = m;
//	nn[2] = n;
	nn[1] = n;
	nn[2] = m;


//	this->wtn(zr, nn, 2, 1, this->daub4);
	this->wtn(zr, nn, 2, 1);
  //## end CFWT::FWT2D%3CA97210009A.body
}

void CFWT::IFWT2D ()
{
  //## begin CFWT::IFWT2D%3CAAD2660297.body preserve=yes

	/* zr mut be set prior to this call */ 
	
	//  this->wtn(a : float*, nn : unsigned long*, ndim : int, isign : int, wtstep : void (*) (float*, DWORD, int)) : void
	unsigned long nn[3];
//	nn[1] = m;
//	nn[2] = n;
	nn[1] = n;
	nn[2] = m;

	this->wtn(zr, nn, 2, -1);
  //## end CFWT::IFWT2D%3CAAD2660297.body
}

void CFWT::CopyWT (BOOL bInDecibel)
{
  //## begin CFWT::CopyWT%3CA9841D00CE.body preserve=yes

	double fR;
	double fMax    = -FLT_MAX;
	double fMin    = FLT_MAX;
	double fMax2nd = -FLT_MAX;
	double fMax3rd;
	double fMax4th;
	double fMax5th;
	double fMax6th;

	double  *lpfImage; 

	lpfImage	= (double *)malloc(m*n*sizeof(double));
	fR		= zr[1];
//	fI		= zi[0];
//	fM		= sqrt(fR*fR + fI*fI);
	fMax6th = 
	fMax5th = 
	fMax4th = 
	fMax3rd = 
	fMax2nd	=
	fMax	= fR;

	for ( int i = 0; i < m; i++ )//for each row
		for ( int j = 0; j < n; j++ )//for each elements in the row
	{
//		fR	= zr[i*m+j+1];
		fR	= zr[i*n+j+1]; //n is width!

		if ( fR > fMax )
		{
			fMax6th = fMax5th;
			fMax5th = fMax4th;
			fMax4th = fMax3rd;
			fMax3rd = fMax2nd;
			fMax2nd	= fMax;
			fMax	= fR;
		}
		fMin	= min( fR, fMin );
		lpfImage[i*n+j]	= fR; //n is width!
	}

	if (!bInDecibel)
		for ( i = 0; i < m_M; i++ )//for each row
			for ( int j = 0; j < m_N; j++ )//for each element in the row
		{
			image[i*m_N+j]	= (unsigned __int16 )
				BOUND((lpfImage[i*n+j]-fMin)*255.0 / fMax2nd, 0.0, 255.);//n is WIDTH!
		}
	else
	{
		double a;
		for ( i = 0; i < m_M; i++ )//for each row
			for ( int j = 0; j < m_N; j++ )//for each element in the row
		{

			a	= lpfImage[i*n+j];
			if ( a ==  0.0 )
				a = -127.0;
			else
				a = log10(a);
			image[i*m_N+j]	= (unsigned __int16 )BOUND(a*45.0 + 127.0, 0.0, 255.);//
		}
	}

	free (lpfImage);

  //## end CFWT::CopyWT%3CA9841D00CE.body
}

void CFWT::CopyWTReal (float* lpData, long* nW, long* nH)
{
  //## begin CFWT::CopyWTReal%3CAA6FC50315.body preserve=yes

	for ( int i = 0; i < m; i++ )//for each row
		for ( int j = 0; j < n; j++ )//for each element in the row
	{
//		lpData[j*m+i]	= zr[j*m+i+1];
		lpData[i*n+j]	= zr[i*n+j+1];////n is WIDTH!
	}
	
	*nH = m;
	*nW = n;
	
  //## end CFWT::CopyWTReal%3CAA6FC50315.body
}

float* CFWT::GetWTReal (long* nW, long* nH)
{
  //## begin CFWT::GetWTReal%3CAACEF5013D.body preserve=yes

	*nH = m;
	*nW = n;
	return zr+1;

  //## end CFWT::GetWTReal%3CAACEF5013D.body
}

void CFWT::SetReal4Transform (float* lpData, long nW, long nH)
{
  //## begin CFWT::SetReal4Transform%3CAA767A0300.body preserve=yes
//	ASSERT(nM==m&&nN==n);
	
	m = (long)(log((double)m_M)/log(2.0));
	if (((1<<m)-m_M)!=0)
	m++;
	n = (long)(log((double)m_N)/log(2.0));
	if (((1<<n)-m_N)!=0)
		n++;
	m = (1<<m);
	n = (1<<n);
   
	if ( this->zr!=NULL)
		free(this->zr);

	this->zr = (float*)malloc((m*n+1)*sizeof(float));

	ASSERT(zr!=NULL);

	for ( int i = 0; i < m_M; i++ )//for each row
		for ( int j = 0; j < m_N; j++ )//for each element in the row
	{
		zr[i*n+j+1] = lpData[i*m_N+j];
	}
	
  //## end CFWT::SetReal4Transform%3CAA767A0300.body
}

void CFWT::SetWaveletFiletr (int nFilterID)
{
  //## begin CFWT::SetWaveletFiletr%3CAE4E0D0006.body preserve=yes
	if (m_pCWaveletFilterBase !=NULL)
	{
		delete m_pCWaveletFilterBase;
		m_pCWaveletFilterBase	= NULL;
	}
	switch(nFilterID)
	{
	case 0:
		m_pCWaveletFilterBase = new Cdaub4;
		break;
	case 1:
		m_pCWaveletFilterBase = new Cpwt (4);
		break;
	case 2:
		m_pCWaveletFilterBase = new Cpwt (12);
		break;
	case 3:
		m_pCWaveletFilterBase = new Cpwt (20);
		break;
	default:
		m_pCWaveletFilterBase = new Cdaub4;
		break;

	}
  //## end CFWT::SetWaveletFiletr%3CAE4E0D0006.body
}

// Additional Declarations
  //## begin CFWT%3CA951C80236.declarations preserve=yes
  //## end CFWT%3CA951C80236.declarations

//## begin module%3CA958560349.epilog preserve=yes
//## end module%3CA958560349.epilog
