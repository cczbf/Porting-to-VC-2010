#include <math.h>
#include <float.h>

#define NRANSI

#include "CArray.h"
#include "nrutil.h"
#include "cmeshopti.h"

//{{Added by xdh 20041215
#include "cglobals.h"
#include "CEasyProgressBar.h"
extern CMeshOpti *g_pCMeshOpti;
//}}Added by xdh 20041215


void fdjac(int n, float x[], float fvec[], float **df,
void (*vecfunc)(int, float [], float []));

//void fdjac(int n,float x[],float fvec[],float **df,void (*vecfunc)());

//float **df,fvec[],x[];
//int n;
//void (*vecfunc)();


void vecfunc1(int n,float *x,float *fvec)
{
	int i,j;
	int nN = n / 4;
	float *Xi=vector_(1,nN);
	float *Yi=vector_(1,nN);
	float *Zi=vector_(1,nN);
	float *Ai=vector_(1,nN);// A: alfa
	for ( i = 1; i <= nN; i++ )
	{
		Xi[i] = x[i];
		Yi[i] = x[nN+i];
		Zi[i] = x[2*nN+i];
		Ai[i] = x[3*nN+i];
	}

	if (g_pCMeshOpti != NULL)
	{
		g_pCMeshOpti->m_CL1.Cal(&(g_pCMeshOpti->m_CLw), x);
		g_pCMeshOpti->m_CL.Cal( &(g_pCMeshOpti->m_CL0),&(g_pCMeshOpti->m_CL1));
		float w = (float)g_pCMeshOpti->m_dW;
		
		for ( i = 1; i <= nN; i++ )
		{
			fvec[i]  =	Ai[i]*Xi[i];
			for ( j = 1; j<=nN; j++)
				fvec[i] -= (float)g_pCMeshOpti->m_CLw[i-1][j-1]*Xi[j];
			
			fvec[i]  = (float)(w *fvec[i]*1.0 + (1.0 - w)*g_pCMeshOpti->m_CL[i-1][i-1]); //Our mothod
			
		}
		for ( i = 1; i <= nN; i++ )
		{
			fvec[i+nN]  =	Ai[i]*Yi[i];// + 
			//						g_pCMesh->m_CL[i-1][i-1]; //Our mothod;
			for ( j = 1; j<=nN; j++)
				fvec[i+nN] -= (float)g_pCMeshOpti->m_CLw[i-1][j-1]*Yi[j];
			
			fvec[i+nN]  =	(float)(w*fvec[i+nN]*1.0 + (1.0 - w)*g_pCMeshOpti->m_CL[i-1][i-1]); //Our mothod;
			
		}
		for ( i = 1; i <= nN; i++ )
		{
			fvec[i+2*nN]  =	Ai[i]*Zi[i];// +
			//						g_pCMesh->m_CL[i-1][i-1]; //Our mothod;
			for (  j = 1; j<=nN; j++)
				fvec[i+2*nN] -= (float)g_pCMeshOpti->m_CLw[i-1][j-1]*Zi[j];
			
			fvec[i+2*nN]  =	(float)(w*fvec[i+2*nN]*1.0 + (1.0-w)*g_pCMeshOpti->m_CL[i-1][i-1]); //Our mothod;
			
		}
		
		for ( i = 1; i <= nN; i++ )
		{
			fvec[i+3*nN]  =(float) fabs(Xi[i]*Xi[i]+Yi[i]*Yi[i]+Zi[i]*Zi[i] - 1.0);
		}
	}

	free_vector(Xi,1,nN);
	free_vector(Yi,1,nN);
	free_vector(Zi,1,nN);
	free_vector(Ai,1,nN);

}

void vecfunc0(int n,float *x,float *fvec)
{
//x[1...4n], xi:x[1...n], yi:x[n+1,...2n],zi:x[2n+1,...3n], alfai:x[3n+1,...,4n]
//fvec[1...4n]
int i,j;
int nN = n / 4;
float *Xi=vector_(1,nN);
float *Yi=vector_(1,nN);
float *Zi=vector_(1,nN);
float *Ai=vector_(1,nN);// A: alfa
for ( i = 1; i <= nN; i++ )
{
	Xi[i] = x[i];
	Yi[i] = x[nN+i];
	Zi[i] = x[2*nN+i];
	Ai[i] = x[3*nN+i];
}
for ( i = 1; i <= nN; i++ )
{
	fvec[i]  = Ai[i]*Xi[i];
	for ( j = 1; j<=nN; j++)
		fvec[i] -= (float)g_pCMeshOpti->m_CLw[i-1][j-1]*Xi[j];
}
for ( i = 1; i <= nN; i++ )
{
	fvec[i+nN]  = Ai[i]*Yi[i];
	for ( j = 1; j<=nN; j++)
		fvec[i+nN] -= (float)g_pCMeshOpti->m_CLw[i-1][j-1]*Yi[j];
}
for ( i = 1; i <= nN; i++ )
{
	fvec[i+2*nN]  = Ai[i]*Zi[i];
	for (  j = 1; j<=nN; j++)
		fvec[i+2*nN] -= (float)g_pCMeshOpti->m_CLw[i-1][j-1]*Zi[j];
}
for ( i = 1; i <= nN; i++ )
{
	fvec[i+3*nN]  =(float) fabs(Xi[i]*Xi[i]+Yi[i]*Yi[i]+Zi[i]*Zi[i] - 1.0);
}

free_vector(Xi,1,nN);
free_vector(Yi,1,nN);
free_vector(Zi,1,nN);
free_vector(Ai,1,nN);

}

void vecfunc(int n,float *x,float *fvec)
{

	int nOptTarget = 1;

    if(g_pCMeshOpti != NULL)
		nOptTarget = g_pCMeshOpti->m_nOptTarget;
	

	switch (nOptTarget)
	{
		case 0:
			vecfunc0( n, x,fvec); //Original
			break;
		case 1:
			vecfunc1( n, x,fvec); //Ours
		break;
	}
}



void usrfun(float *x,int n,float *fvec,float **fjac);
void usrfun(float *x,int n,float *fvec,float **fjac)
{
//x[1...4n], xi:x[1...n], yi:x[n+1,...2n],zi:x[2n+1,...3n], alfai:x[3n+1,...,4n]
//fvec[1...4n]

	vecfunc(n,x,fvec);
	fdjac(n,x,fvec,fjac,vecfunc);
};

#define FREERETURN {free_matrix(fjac,1,n,1,n);free_vector(fvec,1,n);\
	free_vector(p,1,n);free_ivector(indx,1,n);return;}

//#define USEORG
#ifdef USEORG
void mnewt(int ntrial, float x[], int n, float tolx, float tolf)
{
	/*
	Given an initial guess x[1..n] for a root in n dimensions, take ntrial Newton-Raphson steps
to improve the root. Stop if the root converges in either summed absolute variable increments
tolx or summed absolute function values tolf.
	*/

	void lubksb(float **a, int n, int *indx, float b[]);
	void ludcmp(float **a, int n, int *indx, float *d);
	int k,i,*indx;
	float errx,errf,d,*fvec,**fjac,*p;

	indx=ivector(1,n);
	p=vector(1,n);
	fvec=vector(1,n);
	fjac=matrix(1,n,1,n);
	
	for (k=1;k<=ntrial;k++)
	{
		usrfun(x,n,fvec,fjac);
		errf=0.0;
		for (i=1;i<=n;i++) errf += (float)fabs(fvec[i]);

		printf("%d\tAbsolute error: %10.6f\t\t", k,errf);

		if (errf <= tolf) FREERETURN
		for (i=1;i<=n;i++) p[i] = -fvec[i];
		ludcmp(fjac,n,indx,&d);
		lubksb(fjac,n,indx,p);
		errx=0.0;
		for (i=1;i<=n;i++) {
			errx += (float)fabs(p[i]);
			x[i] += p[i];
		}

		printf("N-S method Increamental error: %f\n", errx);

		if (errx <= tolx)
			FREERETURN
	}
	FREERETURN
}
#endif //ORG

#define UseZBF
#ifdef  UseZBF
void mnewt(int ntrial, float x[], int n, float tolx, float tolf)
{
	/*
	Given an initial guess x[1..n] for a root in n dimensions, take ntrial Newton-Raphson steps
to improve the root. Stop if the root converges in either summed absolute variable increments
tolx or summed absolute function values tolf.
	*/

	void lubksb(float **a, int n, int *indx, float b[]);
	void ludcmp(float **a, int n, int *indx, float *d);
	int k,i,*indx;
	float errx,errf,d,*fvec,**fjac,*p;
	errf = FLT_MAX; //zbf

//	bool bUpdateP=1;

	float *x0;
	float errf0,shrink = 1.0; //zbf

	x0		= vector_(1,n);
    for( i = 1; i <= n; i++ ) 
		x0[i] = x[i];

	indx	= ivector(1,n);
	p		= vector_(1,n);
	fvec	= vector_(1,n);
	fjac	= matrix(1,n,1,n);

//	float p0;
//	p0	= vector(1,n);

	CEasyProgressBar bar( _T("Optimization Using Newton-Raphson Method"), ntrial );

	for (k=1;k<=ntrial;k++) 
	{
		bar.UpdateProgress();

//		usrfun(x,n,fvec,fjac);
		usrfun(x0,n,fvec,fjac);
		errf0=0.0;
		for (i=1;i<=n;i++) errf0 += (float)fabs(fvec[i]);

		printf("%d\tN-S method Absolute error: %10.6f\t\t", k, errf0);

		//{{Added by xdh 20041215
		_gcvt(errf0, 30, g_CGlobals.g_chAbsoluteErr);
		//}}Added by xdh 20041215

		if ( errf0 <= errf )
		{
			
			for( i = 1; i <= n; i++ ) 
			{
				x[i] = x0[i];
//				p[i] = p0[i]
			}
//             bUpdateP=1;
			if ( errf0 < errf )
			 shrink = 1.0;
			errf = errf0;
		}
		else
		{
			 //roll back
			for( i = 1; i <= n; i++ ) 
			{
				x0[i] = x[i];
	//			p0[i] = p[i]
			}
  //          bUpdateP=0;
			shrink = shrink*(float)(1.0-0.618);
			printf("\r\n");

			//{{Added by xdh 20041215
			g_CGlobals.m_pDlgTab00->SendMyMsg1();
			//}}Added by xdh 20041215

			continue;
		}

		if (errf <= tolf) FREERETURN
		for (i=1;i<=n;i++) p[i] = -fvec[i];
//		if (bUpdateP)
		{
		ludcmp(fjac,n,indx,&d);
		lubksb(fjac,n,indx,p);
		}
		errx=0.0;
		for (i=1;i<=n;i++) {
			float e;
			x0[i] += (e = p[i]*shrink);
			errx += (float)fabs(e);		 
		}

		printf("N-S method Increamental error: %f\n", errx);
		
		//{{Added by xdh 20041215
		_gcvt(errx, 30, g_CGlobals.g_chIncrementErr);
		MSG msg;
		HACCEL hAccelTable=NULL;
		g_CGlobals.m_pDlgTab00->SendMyMsg1();
		if ( PeekMessage(
			&msg,         // message information
			NULL,           // handle to window
			0,  // first message
			0,  // last message
			PM_REMOVE      // removal options
			)) 
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		//}}Added by xdh 20041215

		if (errx <= tolx) 
			FREERETURN
	}
	FREERETURN
}
#endif //UseZBF
#undef FREERETURN
#undef NRANSI
