//## begin module%3886A05700FB.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3886A05700FB.cm

//## begin module%3886A05700FB.cp preserve=no
//## end module%3886A05700FB.cp

//## Module: CFFT%3886A05700FB; Package body
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: d:\Desktop\Halftonetest\bmpviewer0\CFFT.cpp

//## begin module%3886A05700FB.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%3886A05700FB.additionalIncludes

//## begin module%3886A05700FB.includes preserve=yes
#include "math.h"
#include "FLOAT.H"

//## end module%3886A05700FB.includes

// CFFT
#include "CFFT.h"
//## begin module%3886A05700FB.declarations preserve=no
//## end module%3886A05700FB.declarations

//## begin module%3886A05700FB.additionalDeclarations preserve=yes
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
//## end module%3886A05700FB.additionalDeclarations


// Class CFFT 












CFFT::CFFT()
  //## begin CFFT::CFFT%.hasinit preserve=no
      : m_M(64), m_N(64), image(NULL), m_fMaxReal(FLT_MIN), m_fMinReal(FLT_MAX), m_fMaxImagnary(FLT_MIN), m_fMinImagnary(FLT_MAX), zr(NULL), zi(NULL), m_dblMeanGrayOfSourceImage(128.0)
  //## end CFFT::CFFT%.hasinit
  //## begin CFFT::CFFT%.initialization preserve=yes
  //## end CFFT::CFFT%.initialization
{
  //## begin CFFT::CFFT%.body preserve=yes
  //## end CFFT::CFFT%.body
}


CFFT::~CFFT ()
{
  //## begin CFFT::~CFFT%3886AF6E009C.body preserve=yes
	if ( zi != NULL )
		free(zi);
	if ( zr != NULL )
		free(zr);

	if ( m_lpCRawFTImage != NULL )
		if( m_lpCRawFTImage->m_lpPixels != NULL )
		{
			GlobalUnlock(m_lpCRawFTImage->m_hPixels);
			m_lpCRawFTImage->m_lpPixels	= NULL;
		}
  //## end CFFT::~CFFT%3886AF6E009C.body
}



//## Other Operations (implementation)
void CFFT::FFT2D ()
{
  //## begin CFFT::FFT2D%3886826F021A.body preserve=yes
//#define _NOTREADY1
#ifndef _NOTREADY1

	long i, j;
	double *R, *I;
//	float *zr, *zi;
//	unsigned char *image;
	
	/* allocate memory for the working array */

	zr = (float*)malloc(m_M*m_N*sizeof(float));
	zi = (float*)malloc(m_M*m_N*sizeof(float));
	R  = (double*)malloc(max(m_M,m_N)*sizeof(double));
	I  = (double*)malloc(max(m_M,m_N)*sizeof(double));

//	image=(unsigned char*)malloc(M*M);

	ASSERT(!( zi == NULL || zr == NULL || image == NULL ));

	/* read in image data */

	/* copy image to zr, zi array */ 
	for ( i = 0; i < m_M; i++ )//for each row
		for ( j = 0; j < m_N; j++ )//for each element in the row
	{
		zr[i*m_N+j]	= (((i+j)%2)?-(float)image[i*m_N+j]:(float)image[i*m_N+j]);
//		zr[i*M+j]	= (float)image[i*M+j];
		zi[i*m_N+j]	= 0.0f;
	}
//	free(image);
	/* --- 1-d FFT of row --- */
	for ( i = 0; i < m_M; i++)//for each row
	{
		for ( j = 0; j < m_N; j++ )//for each element in the row
		{
			R[j] = zr[i*m_N+j];
			I[j] = zi[i*m_N+j];
		}
		FFT(R, I, m_N);
		for ( j = 0; j < m_N; j++ )
		{
			zr[i*m_N+j] = (float)R[j];
			zi[i*m_N+j] = (float)I[j];
		}
	}

	/* --- 1-d FFT of col --- */
	for ( j = 0; j < m_N; j++)//for each column
	{
		for ( i = 0; i < m_M; i++ )
		{
			R[i] = zr[i*m_N+j];
			I[i] = zi[i*m_N+j];
		}
		FFT(R, I, m_M);
		for ( i = 0; i < m_M; i++ )
		{
			zr[i*m_N+j] = (float)R[i];
			zi[i*m_N+j] = (float)I[i];
		}
	};



	/* save FFT result as float type file */

//	printf("FFT file name:\n");
//	printf("Real part -->");
//	scanf("%s", file_name);
//	printf("\n");
//	if (( fp = fopen(file_name, "wb")) == NULL )
//	{
//		printf("open error");
//		exit(-3);
//	}
//	fwrite(zr, sizeof(float), M*M, fp);
//	fclose(fp);
//	free(zr);

//	printf("Imagine part -->");
//	scanf("%s", file_name);
//	printf("\n");
//	if (( fp = fopen(file_name, "wb")) == NULL )
//	{
//		printf("open error");
//		exit(-4);
//	}
//	fwrite(zi, sizeof(float), M*M, fp);
//	fclose(fp);
//	free(zi);

//	};


free(R);
free(I);
/*
float fMin = FLT_MAX;
float fMax = FLT_MIN;

for ( i = 0; i < M; i++ )
	for ( j = 0; j < M; j++ )
	{
		fMin = min(fMin, zr[i*M+j]);//	= (float)image[i*M+j];
		fMax = max(fMax, zr[i*M+j]);//	= (float)image[i*M+j];
	}

this->m_fMaxReal	= fMax;
this->m_fMinReal	= fMin;

for ( i = 0; i < M; i++ )
	for ( j = 0; j < M; j++ )
	{
		fMin = min(fMin, zi[i*M+j]);//	= (float)image[i*M+j];
		fMax = max(fMax, zi[i*M+j]);//	= (float)image[i*M+j];
	}
this->m_fMaxImagnary	= fMax;
this->m_fMinImagnary	= fMin;
*/
#endif


  //## end CFFT::FFT2D%3886826F021A.body
}

void CFFT::IFFT2D ()
{
  //## begin CFFT::IFFT2D%3886827A02C0.body preserve=yes
  //## end CFFT::IFFT2D%3886827A02C0.body
}

void CFFT::FFT (double* A, double* B, int m)
{
  //## begin CFFT::FFT%388682BD03AD.body preserve=yes
#define _NOTREADY
#ifndef  _NOTREADY
/*----------------------------------------------------------
Purpose: Performaing 1-dimensional FFT
Arguments:
A	-- FFT origin / result real part
B	-- FFT origin / result imagine part
------------------------------------------------------------*/
double	darg, arg, sin1, cos1, temp, t1, t2;
int		n, k, l, j1, j2, lm, lmx, lo, lix, li, ii, jj, md2, m1;

#define A(i) A[(i-1)]
#define B(i) B[(i-1)]	/* define macros for convenience */

/* calculate l such that 2**l = size */
n = 1;
for ( k = 1; k <= 100; k++ )
{
	n *=2;
	if ( n == m )
	{
		l = k;
		break;
	}
}

/* --- begin FFT --- */

lmx		= m;
darg	= atan( (double)1.0 )*8.0/(double)m;

for ( lo = 1; lo <= l; lo++ )
{
	lix	= lmx;
	lmx	= lmx / 2;
	arg	= 0.0;

	for ( lm = 1; lm <= lmx; lm++ )
	{
		cos1	= cos(arg);
		sin1	= -sin(arg);
		arg		+= darg;

		for ( li = lix; li <= m; li += lix )
		{
			j1	= li - lix + lm;
			j2	= j1 + lmx;
			t1	= A(j1) - A(j2);
			t2	= B(j1) - B(j2);
			A(j1)	+= A(j2);
			B(j1)	+= B(j2);
			A(j2)	= cos1*t1 + sin1*t2;
			A(j2)	= cos1*t2 - sin1*t1;
		}

	}
	darg = 2.0 * darg;
}

/* code reverse */

jj	= 1;
md2	= m / 2;
m1	= m - 1;
for ( ii = 1; ii<=m1; ii++ )
{
	if( ( ii - jj )<0 )
	{
		temp=A(ii); A(ii)=A(jj); A(jj)=temp;/* swap data */
		temp=B(ii); B(ii)=B(jj); B(jj)=temp;
	}
	k	= md2;
	while( (k-jj)<0 )
	{
		jj -= k;
		k  /= 2;
	}
	jj	+= k;
}

/* normalization */
for( ii = 1; ii<=m; ii++ )
{
	A(ii)	/= (double)m;
	B(ii)	/= (double)m;
}
#else
this->DFT(A,B,m);
#endif

  //## end CFFT::FFT%388682BD03AD.body
}

void CFFT::DFT (double* dR, double* dI, int m)
{
  //## begin CFFT::DFT%388E5501012D.body preserve=yes
	double* dR_in; 
	double* dI_in;

	dR_in	= (double*)malloc(m*sizeof(double));
	dI_in	= (double*)malloc(m*sizeof(double));

	for ( WORD i = 0; i < m; i++ )
	{
		dR_in[i]	= dR[i];
		dI_in[i]	= dI[i];
	}
	
	for ( i = 0; i < m; i++ )
		this->CalcFrequency (m, i, dR_in, dI_in, dR[i], dI[i]);
	free(dR_in);
	free(dI_in);

  //## end CFFT::DFT%388E5501012D.body
}

void CFFT::CalcFrequency (WORD NumSamples, WORD FrequencyIndex, double* lpRealIn, double* lpImagIn, double& RealOut, double& ImagOut)
{
  //## begin CFFT::CalcFrequency%388E55540295.body preserve=yes
#define PI 3.141592654
	
    WORD k;
    double cos1, cos2, cos3, theta, beta;
    double sin1, sin2, sin3;

    RealOut	= 0.0;
    ImagOut	= 0.0;
    theta	= 2.0*PI * FrequencyIndex / NumSamples;
    sin1	= sin ( -2.0 * theta );
    sin2	= sin ( -theta );
    cos1	= cos ( -2.0 * theta );
    cos2	= cos ( -theta );
    beta	= 2.0 * cos2;
    for (k = 0; k <= NumSamples-1; k++)
	{
        // Update trig values 
        sin3	= beta*sin2 - sin1;
        sin1	= sin2;
        sin2	= sin3;

        cos3	= beta*cos2 - cos1;
        cos1	= cos2;
        cos2	= cos3;

        RealOut	= RealOut + lpRealIn[k]*cos3 - lpImagIn[k]*sin3;
        ImagOut	= ImagOut + lpImagIn[k]*cos3 + lpRealIn[k]*sin3;
    }

  //## end CFFT::CalcFrequency%388E55540295.body
}

void CFFT::SetWorkingImage (CRawImage16* lpCRawFTImage)
{
  //## begin CFFT::SetWorkingImage%3886CC990143.body preserve=yes
	if ( lpCRawFTImage != NULL )
	{
		m_lpCRawFTImage				= lpCRawFTImage;
		m_lpCRawFTImage->m_lpPixels	= (unsigned _int16*)
			GlobalLock(m_lpCRawFTImage->m_hPixels);
		this->image					= m_lpCRawFTImage->m_lpPixels;
		this->m_M					= (int)m_lpCRawFTImage->m_nHeight;
		this->m_N					= (int)m_lpCRawFTImage->m_nWidth;

	}
	else
	{//reset(clear) the working inage
		if ( m_lpCRawFTImage->m_lpPixels != NULL )
			GlobalUnlock(m_lpCRawFTImage->m_hPixels);
		m_lpCRawFTImage->m_lpPixels = NULL;
		m_lpCRawFTImage				= NULL;
		image						= NULL;

	}
  //## end CFFT::SetWorkingImage%3886CC990143.body
}

void CFFT::CopyReal ()
{
  //## begin CFFT::CopyReal%388D136D030E.body preserve=yes
/*
	for ( int i = 0; i < M; i++ )
		for ( int j = 0; j < M; j++ )
	{
		image[i*M+j]	= (unsigned __int16 )zr[i*M+j];
	}

*/
  //## end CFFT::CopyReal%388D136D030E.body
}

void CFFT::CopyImagnary ()
{
  //## begin CFFT::CopyImagnary%388D137B0282.body preserve=yes
  //## end CFFT::CopyImagnary%388D137B0282.body
}

void CFFT::CopyMagnitude ()
{
  //## begin CFFT::CopyMagnitude%388D15CA03D6.body preserve=yes
	double fR, fI, fM;
	double fMax = -FLT_MAX;
	double fMin = FLT_MAX;
	double fMax2nd = -FLT_MAX;

	double  *lpfImage;

//	lpfImage	= (double *)malloc(M*M*sizeof(double));
	lpfImage	= (double *)malloc(m_M*m_N*sizeof(double));
	fR		= zr[0];
	fI		= zi[0];
	fM		= sqrt(fR*fR + fI*fI);
	fMax	=
	fMax2nd	= fM;

	for ( int i = 0; i < m_M; i++ )//for each row
		for ( int j = 0; j < m_N; j++ )//for each elements in the row
	{
		fR	= zr[i*m_N+j];
		fI	= zi[i*m_N+j];
		fM	= sqrt(fR*fR + fI*fI);

		if ( fM > fMax )
		{
			fMax2nd	= fMax;
			fMax	= fM;
		}
		fMin	= min( fM, fMin );
		lpfImage[i*m_N+j]	= fM;
	}

	for ( i = 0; i < m_M; i++ )//for each row
		for ( int j = 0; j < m_N; j++ )//for each element in the row
	{
		image[i*m_N+j]	= (unsigned __int16 )
			BOUND((lpfImage[i*m_N+j]-fMin)*255.0 / fMax2nd, 0.0, 255.);
	}
	free (lpfImage);
  //## end CFFT::CopyMagnitude%388D15CA03D6.body
}

void CFFT::CopyPhaseAngle ()
{
  //## begin CFFT::CopyPhaseAngle%388D15F200C7.body preserve=yes
  //## end CFFT::CopyPhaseAngle%388D15F200C7.body
}

double CFFT::MeanGrayOf_image ()
{
  //## begin CFFT::MeanGrayOf_image%3B60C90C0034.body preserve=yes
	double dblMeanGrey = 0.0;
	for ( int i = 0; i < m_M; i++ )//for each row
		for ( int j = 0; j < m_N; j++ )//for each element in the row
	{
		dblMeanGrey += (double)image[i*m_N+j];
	}
	DWORD dwSize = m_M*m_N;
	if ( dwSize != 0 )
		dblMeanGrey = dblMeanGrey / (double)dwSize;
	else
		dblMeanGrey = 0.5;

	this->m_dblMeanGrayOfSourceImage = dblMeanGrey;
	return dblMeanGrey;

  //## end CFFT::MeanGrayOf_image%3B60C90C0034.body
}

void CFFT::CopyPeridogram ()
{
  //## begin CFFT::CopyPeridogram%3B6111FE03E3.body preserve=yes


	double fR, fI, fM;
	double fMax = -FLT_MAX;
	double fMin = FLT_MAX;
	double fMax2nd = -FLT_MAX;

	double  *lpfImage;
	int		nMax_x, nMax_y;

//	lpfImage	= (double *)malloc(M*M*sizeof(double));
	lpfImage	= (double *)malloc(m_M*m_N*sizeof(double));
	fR		= zr[0];
	fI		= zi[0];
	fM		= sqrt(fR*fR + fI*fI);
	fMax	=
	fMax2nd	= fM;

	for ( int i = 0; i < m_M; i++ )//for each row
		for ( int j = 0; j < m_N; j++ )//for each elements in the row
	{
		fR	= zr[i*m_N+j];
		fI	= zi[i*m_N+j];
		fM	= sqrt(fR*fR + fI*fI);

		if ( fM > fMax )
		{
			fMax2nd	= fMax;
			fMax	= fM;
			nMax_x	= i;
			nMax_y	= j;

		}
		fMin	= min( fM, fMin );
		lpfImage[i*m_N+j]	= fM;
	}

	double sigma = m_dblMeanGrayOfSourceImage*(255.0 - m_dblMeanGrayOfSourceImage);
//	double sigma = m_dblMeanGrayOfSourceImage*m_dblMeanGrayOfSourceImage;
	double d, s = m_M*m_N, sigma1=0.0;
	DWORD lSigma =0;

	fMax = -FLT_MAX;
	fMin = FLT_MAX;

	for ( i = 0; i < m_M; i++ )//for each row
		for ( int j = 0; j < m_N; j++ )//for each element in the row
	{
		if (i==nMax_x&&j==nMax_y)
		{
			image[i*m_N+j]	= 0;
		}
		else
		{
		d = lpfImage[i*m_N+j];
		d = (d*d)/(s);//Periodogram
		d = d / sigma;//Normalized
		sigma1 += d;
		fMin	= min(fMin,d);
		fMax	= max(fMax,d);
		lSigma			+=
//		(image[i*m_N+j]	= (unsigned __int16 )BOUND(d*128.0*0.4, 0.0, 255.));//至此，d的均值应是1.0，（d的和(sigma1)是s=m_nN*m_nM）
		(image[i*m_N+j]	= (unsigned __int16 )BOUND(d*128.0, 0.0, 255.));//至此，d的均值应是1.0，（d的和(sigma1)是s=m_nN*m_nM）
//		(image[i*m_N+j]	= (unsigned __int16 )BOUND(d*128.0, 0.0, 255.));
//		(image[i*m_N+j]	= (unsigned __int16 )BOUND(d*s*128.0*.04, 0.0, 255.));
		}
	}
	double r = sigma1/sigma;

	free (lpfImage);





  //## end CFFT::CopyPeridogram%3B6111FE03E3.body
}

// Additional Declarations
  //## begin CFFT%388682310134.declarations preserve=yes
  //## end CFFT%388682310134.declarations

//## begin module%3886A05700FB.epilog preserve=yes
//## end module%3886A05700FB.epilog
