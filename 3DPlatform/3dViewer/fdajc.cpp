#include <math.h>
#define NRANSI
#include "nrutil.h"
#define EPS 1.0e-4

//void fdjac(n,x,fvec,df,vecfunc)
//float **df,fvec[],x[];
//int n;
//void (*vecfunc)();
void fdjac(
int n,
float x[],
float fvec[],
float **df,
void (*vecfunc)(int, float [], float []))
{
	int i,j;
	float h,temp,*f;

	f=vector_(1,n);
	for (j=1;j<=n;j++) {
		temp=x[j];
		h=(float)EPS*(float)fabs(temp);
		if (h == 0.0) h=(float)EPS;
		x[j]=temp+h;
		h=x[j]-temp;
		(*vecfunc)(n,x,f);
		x[j]=temp;
		for (i=1;i<=n;i++) df[i][j]=(f[i]-fvec[i])/h;
	}
	free_vector(f,1,n);
}
#undef EPS
