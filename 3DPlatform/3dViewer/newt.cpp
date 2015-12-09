#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//{{Added by xdh 20041215
#include "cglobals.h"
#include "CEasyProgressBar.h"
//}}Added by xdh 20041215

#define NRANSI

#include "nrutil.h"
//#define MAXITS 200
int MAXITS = 200;
//#define TOLF 1.0e-4
float TOLF = (float)1.0e-4;


#define TOLMIN 1.0e-6
//#define TOLMIN 0.0e-6
//#define TOLX 1.0e-7
double TOLX = 1.0e-7;
#define STPMX 100.0

/*
Here :
MAXITS is the maximum number of iterations; 
TOLF sets the convergence criterion on function values; 
TOLMIN sets the criterion for deciding whether spurious convergence to a minimum of fmin has occurred; 
TOLX is the convergence criterion on x; 
STPMX is the scaled maximum step length allowed in line searches.
*/


int nn;//Global variables to communicate with fmin.
float *fvec;
void (*nrfuncv)(int n, float v[], float f[]);
#define FREERETURN {free_vector(fvec,1,n);free_vector(xold,1,n);\
	free_vector(p,1,n);free_vector(g,1,n);free_matrix(fjac,1,n,1,n);\
	free_ivector(indx,1,n);return;}



/*
Given an initial guess x[1..n] for a root in n dimensions, nd the root by a globally convergent
Newton's method. The vector of functions to be zeroed, called fvec[1..n] in the routine
below, is returned by the user-supplied routine vecfunc(n,x,fvec). The output quantity
check is false (0) on a normal return and true (1) if the routine has converged to a local
minimum of the function fmin dened below. In this case try restarting from a dierent initial
guess.
*/
void newt(float x[], int n, int *check,
	void (*vecfunc)(int, float [], float []))
{
	void fdjac(int n, float x[], float fvec[], float **df,
		void (*vecfunc)(int, float [], float []));
	float fmin(float x[]);
	void lnsrch(int n, float xold[], float fold, float g[], float p[], float x[],
		 float *f, float stpmax, int *check, float (*func)(float []));
	void lubksb(float **a, int n, int *indx, float b[]);
	void ludcmp(float **a, int n, int *indx, float *d);
	int i,its,j,*indx;
	float d,den,f,fold,stpmax,sum,temp,test,**fjac,*g,*p,*xold;

	indx=ivector(1,n);
	fjac=matrix(1,n,1,n);
	g=vector_(1,n);
	p=vector_(1,n);
	xold=vector_(1,n);
	fvec=vector_(1,n);
	nn=n;
	nrfuncv=vecfunc;
	f=fmin(x);
	test=0.0;
	
	for (i=1;i<=n;i++)
		if (fabs(fvec[i]) > test) test=(float)fabs(fvec[i]);
	
	if (test<0.01*TOLF) 
		FREERETURN
	
	for (sum=0.0,i=1;i<=n;i++)
		sum += (float)SQR(x[i]);
	stpmax=(float)(STPMX*FMAX((float)sqrt(sum),(float)n));

	CEasyProgressBar bar( _T("Optimization Using Newton Method"), MAXITS );

	for (its=1; its<=MAXITS; its++) 
	{
		bar.UpdateProgress();

		fdjac(n,x,fvec,fjac,vecfunc);
		for (i=1;i<=n;i++)
		{
			for (sum=0.0,j=1;j<=n;j++) sum += fjac[j][i]*fvec[j];
			g[i]=sum;
		}
		for (i=1;i<=n;i++) xold[i]=x[i];
		fold=f;
		for (i=1;i<=n;i++) p[i] = -fvec[i];
		ludcmp(fjac,n,indx,&d);
		lubksb(fjac,n,indx,p);
		lnsrch(n,xold,fold,g,p,x,&f,stpmax,check,fmin);
		test=0.0;
		for (i=1;i<=n;i++)
			if (fabs(fvec[i]) > test) test=(float)fabs(fvec[i]);

		printf("%d : Absolute Error:%f ", its, test);
		
		//{{Added by xdh 20041215
		_gcvt(test, 10, g_CGlobals.g_chAbsoluteErr);
		//{{Added by xdh 20041215

		if (test < TOLF) 
		{
			*check=0;
			FREERETURN
		}
		if (*check)
		{
			test=0.0;
			den=(float)FMAX((float)f,(float)(0.5*n));
			for (i=1;i<=n;i++) 
			{
				temp=(float)fabs(g[i])*FMAX((float)fabs(x[i]),1.0)/den;
				if (temp > test) test=temp;
			}
			*check=(test < TOLMIN ? 1 : 0);
			printf ("\nLocal minimium.\n");
			FREERETURN
		}
		test=0.0;
		for (i=1;i<=n;i++) 
		{
			temp=((float)fabs(x[i]-xold[i]))/FMAX((float)fabs(x[i]),1.0);
			if (temp > test) test=temp;
		}

		printf("\tIncremental Error:%f \n", test);

		//{{Added by xdh 20041215
		_gcvt(test, 10, g_CGlobals.g_chIncrementErr);
		g_CGlobals.m_pDlgTab00->SendMyMsg();
		MSG msg;
		HACCEL hAccelTable=NULL;
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

		if (test < TOLX) 
			FREERETURN
	}
//	nrerror("MAXITS exceeded in newt");
	printf ("MAXITS exceeded in newt\n");
}


void vecfunc(int n,float *x,float *fvec);
void mnewt_global(int ntrial, float x[], int n, float tolx, float tolf)
{

	int check;

	MAXITS = ntrial;
	TOLF = tolf;
	TOLX = tolx;
	/*
Given an initial guess x[1..n] for a root in n dimensions, nd the root by a globally convergent
Newton's method. The vector of functions to be zeroed, called fvec[1..n] in the routine
below, is returned by the user-supplied routine vecfunc(n,x,fvec). The output quantity
check is false (0) on a normal return and true (1) if the routine has converged to a local
minimum of the function fmin dened below. In this case try restarting from a dierent initial
guess.
*/	
	newt(
			x,		//float x[], 
			n,		//int n,
			&check,	//int *check,
			vecfunc	//void (*vecfunc)(int, float [], float [])
			);
if( check )
{
	printf("Not necessarily converged!!!\n");
}
else
	printf("Considered to have succesfully converged!!!\n");

}





#undef MAXITS
#undef TOLF
#undef TOLMIN
#undef TOLX
#undef STPMX
#undef FREERETURN
#undef NRANSI
