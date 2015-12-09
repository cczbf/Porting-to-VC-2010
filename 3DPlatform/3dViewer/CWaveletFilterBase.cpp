//## begin module%3CAE4A1803D5.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CAE4A1803D5.cm

//## begin module%3CAE4A1803D5.cp preserve=no
//## end module%3CAE4A1803D5.cp

//## Module: CWaveletFilterBase%3CAE4A1803D5; Package body
//## Subsystem: bmpviewer::Error Diffusion Packages%3882B16203CC
//	Code is generated to
//	$SOURCECPP
//	See 'Directory' in  C++ Tab
//## Source file: D:\H\desktop\HalfToneTest\bmpviewer--nxn\CWaveletFilterBase.cpp

//## begin module%3CAE4A1803D5.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%3CAE4A1803D5.additionalIncludes

//## begin module%3CAE4A1803D5.includes preserve=yes
//## end module%3CAE4A1803D5.includes

// CWaveletFilterBase
#include "CWaveletFilterBase.h"
//## begin module%3CAE4A1803D5.declarations preserve=no
//## end module%3CAE4A1803D5.declarations

//## begin module%3CAE4A1803D5.additionalDeclarations preserve=yes
//## end module%3CAE4A1803D5.additionalDeclarations


// Class CWaveletFilterBase 





CWaveletFilterBase::CWaveletFilterBase()
  //## begin CWaveletFilterBase::CWaveletFilterBase%.hasinit preserve=no
      : m_nFilterID(0)
  //## end CWaveletFilterBase::CWaveletFilterBase%.hasinit
  //## begin CWaveletFilterBase::CWaveletFilterBase%.initialization preserve=yes
  //## end CWaveletFilterBase::CWaveletFilterBase%.initialization
{
  //## begin CWaveletFilterBase::CWaveletFilterBase%.body preserve=yes
  //## end CWaveletFilterBase::CWaveletFilterBase%.body
}



//## Other Operations (implementation)
void CWaveletFilterBase::wtstep (float* a, unsigned long n, int isign)
{
  //## begin CWaveletFilterBase::wtstep%3CAE470801AA.body preserve=yes
  //## end CWaveletFilterBase::wtstep%3CAE470801AA.body
}

// Additional Declarations
  //## begin CWaveletFilterBase%3CAE46D60292.declarations preserve=yes
  //## end CWaveletFilterBase%3CAE46D60292.declarations

// Class Cdaub4 

//## begin Cdaub4::nFilterID%3CAE4E95014B.attr preserve=no  private: static int {UAC} 0
const int Cdaub4::nFilterID = 0;
//## end Cdaub4::nFilterID%3CAE4E95014B.attr

Cdaub4::Cdaub4()
  //## begin Cdaub4::Cdaub4%.hasinit preserve=no
  //## end Cdaub4::Cdaub4%.hasinit
  //## begin Cdaub4::Cdaub4%.initialization preserve=yes
  //## end Cdaub4::Cdaub4%.initialization
{
  //## begin Cdaub4::Cdaub4%.body preserve=yes
  //## end Cdaub4::Cdaub4%.body
}



//## Other Operations (implementation)
void Cdaub4::wtstep (float* a, unsigned long n, int isign)
{
  //## begin Cdaub4::wtstep%3CAE489601AA.body preserve=yes
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
	wksp = m_CFWT_FFT_Util.vector(1,n);
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
	m_CFWT_FFT_Util.free_vector(wksp,1,n);

#undef C0
#undef C1
#undef C2
#undef C3

  
  //## end Cdaub4::wtstep%3CAE489601AA.body
}

// Additional Declarations
  //## begin Cdaub4%3CAE478501BE.declarations preserve=yes
  //## end Cdaub4%3CAE478501BE.declarations

// Class Cpwt 

//## begin Cpwt::nFilterID%3CAE5022028B.attr preserve=no  private: static int {UA} 1
int Cpwt::nFilterID = 1;
//## end Cpwt::nFilterID%3CAE5022028B.attr

Cpwt::Cpwt()
  //## begin Cpwt::Cpwt%.hasinit preserve=no
  //## end Cpwt::Cpwt%.hasinit
  //## begin Cpwt::Cpwt%.initialization preserve=yes
  //## end Cpwt::Cpwt%.initialization
{
  //## begin Cpwt::Cpwt%.body preserve=yes
  //## end Cpwt::Cpwt%.body
}

Cpwt::Cpwt (int n)
  //## begin Cpwt::Cpwt%3CB0F021027D.hasinit preserve=no
  //## end Cpwt::Cpwt%3CB0F021027D.hasinit
  //## begin Cpwt::Cpwt%3CB0F021027D.initialization preserve=yes
  //## end Cpwt::Cpwt%3CB0F021027D.initialization
{
  //## begin Cpwt::Cpwt%3CB0F021027D.body preserve=yes
	if ( n == 4 || n == 20 )
		pwtset (n);
	else
		pwtset (12);


  //## end Cpwt::Cpwt%3CB0F021027D.body
}



//## Other Operations (implementation)
void Cpwt::wtstep (float* a, unsigned long n, int isign)
{
  //## begin Cpwt::wtstep%3CAE4B430331.body preserve=yes

/*
void pwt(a,n,isign)
float a[];
int isign;
unsigned long n;
*/
/*
Partial wavelet transform: applies an arbitrary wavelet filter to data vector a[1..n] (for isign =
1) or applies its transpose (for isign =  1). Used hierarchically by routines wt1 and wtn.
The actual filter is determined by a preceding (and required) call to pwtset, which initializes
the structure wfilt.
*/
	
		float ai,ai1,*wksp;
		unsigned long i,ii,j,jf,jr,k,n1,ni,nj,nh,nmod;
		if (n < 4) return;
		wksp= m_CFWT_FFT_Util.vector(1,n);
		nmod=wfilt.ncof*n; //A positive constant equal to zero mod n.
		n1=n-1; //Mask of all bits, since n a power of 2.
		nh=n >> 1;
		for (j=1;j<=n;j++) wksp[j]=(float)0.0;
		if (isign >= 0) { //Apply filter.
			for (ii=1,i=1;i<=n;i+=2,ii++) {
				ni=i+nmod+wfilt.ioff; //Pointer to be incremented and wrapped-around.
				nj=i+nmod+wfilt.joff;
				for (k=1;k<=(unsigned long)wfilt.ncof;k++) {
					jf=n1 & (ni+k); //We use bitwise and to wrap-around the point-
					jr=n1 & (nj+k); //ers. 
					wksp[ii] += wfilt.cc[k]*a[jf+1];
					wksp[ii+nh] += wfilt.cr[k]*a[jr+1];
				}
			}
		} else {// Apply transpose filter.
			for (ii=1,i=1;i<=n;i+=2,ii++) {
				ai=a[ii];
				ai1=a[ii+nh];
				ni=i+nmod+wfilt.ioff; //See comments above.
				nj=i+nmod+wfilt.joff;
				for (k=1;k<=(unsigned long)wfilt.ncof;k++) {
					jf=(n1 & (ni+k))+1;
					jr=(n1 & (nj+k))+1;
					wksp[jf] += wfilt.cc[k]*ai;
					wksp[jr] += wfilt.cr[k]*ai1;
				}
			}
		}
		for (j=1;j<=n;j++) a[j]=wksp[j]; //Copy the results back from workspace.
		m_CFWT_FFT_Util.free_vector(wksp,1,n);


  //## end Cpwt::wtstep%3CAE4B430331.body
}

void Cpwt::pwtset (int n)
{
  //## begin Cpwt::pwtset%3CB0E3E803E1.body preserve=yes
	/*
	Initializing routine for pwt, here implementing the Daubechies wavelet filters with 4, 12, and
20 coe.cients, as selected by the input value n. Further wavelet filters can be included in the
obvious manner. This routine must be called (once) before the first use of pwt. (Forthecase
n=4, the specific routine daub4 is considerably faster than pwt.)
	*/

//	void nrerror(char error_text[]);
	int k;
	float sig = (float)-1.0;
	static float c4[5]={
							(float)0.0,
							(float)0.4829629131445341,
							(float)0.8365163037378079,
							(float)0.2241438680420134,
							(float)-0.1294095225512604};
//	static float c4[5]={0.0,
//						0.25,
//						0.25,
//						0.25,
//						0.25};
	static float c12[13]={
							(float)0.0,
							(float)0.111540743350, 
							(float)0.494623890398, 
							(float)0.751133908021,
							(float)0.315250351709,
							(float)-0.226264693965,
							(float)-0.129766867567,
							(float)0.097501605587, 
							(float)0.027522865530,
							(float)-0.031582039318,
							(float)0.000553842201, 
							(float)0.004777257511,
							(float)-0.001077301085};
	static float c20[21]={
							(float)0.0,
							(float)0.026670057901, 
							(float)0.188176800078, 
							(float)0.527201188932,
							(float)0.688459039454, 
							(float)0.281172343661,
							(float)-0.249846424327,
							(float)-0.195946274377, 
							(float)0.127369340336, 
							(float)0.093057364604,
							(float)-0.071394147166,
							(float)-0.029457536822, 
							(float)0.033212674059,
							(float)0.003606553567,
							(float)-0.010733175483, 
							(float)0.001395351747,
							(float)0.001992405295,
							(float)-0.000685856695,
							(float)-0.000116466855,
							(float)0.000093588670,
							(float)-0.000013264203};
	static float c4r[5],c12r[13],c20r[21];

	wfilt.ncof=n;
	if (n == 4) {
		wfilt.cc=c4;
		wfilt.cr=c4r;
	}
	else if (n == 12) {
		wfilt.cc=c12;
		wfilt.cr=c12r;
	}
	else if (n == 20) {
		wfilt.cc=c20;
		wfilt.cr=c20r;
	}
	else m_CFWT_FFT_Util.nrerror("unimplemented value n in pwtset");
	for (k=1;k<=n;k++) {
		wfilt.cr[wfilt.ncof+1-k]=sig*wfilt.cc[k];
		sig = -sig;
	}
	wfilt.ioff = wfilt.joff = -(n >> 1);

	/*
	These values center the \support" of the wavelets at each level. Alternatively, the \peaks"
	of the wavelets can be approximately centered by the choices ioff=-2 and joff=-n+2.
	*/


  //## end Cpwt::pwtset%3CB0E3E803E1.body
}

// Additional Declarations
  //## begin Cpwt%3CAE48670233.declarations preserve=yes
  //## end Cpwt%3CAE48670233.declarations

// Class wavefilt 







wavefilt::wavefilt()
  //## begin wavefilt::wavefilt%.hasinit preserve=no
  //## end wavefilt::wavefilt%.hasinit
  //## begin wavefilt::wavefilt%.initialization preserve=yes
  //## end wavefilt::wavefilt%.initialization
{
  //## begin wavefilt::wavefilt%.body preserve=yes
  //## end wavefilt::wavefilt%.body
}


// Additional Declarations
  //## begin wavefilt%3CB0E6CF03C3.declarations preserve=yes
  //## end wavefilt%3CB0E6CF03C3.declarations

//## begin module%3CAE4A1803D5.epilog preserve=yes
//## end module%3CAE4A1803D5.epilog
