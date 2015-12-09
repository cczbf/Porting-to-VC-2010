//## begin module%41E3789E01B3.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%41E3789E01B3.cm

//## begin module%41E3789E01B3.cp preserve=no
//## end module%41E3789E01B3.cp

//## Module: CMyCGLib%41E3789E01B3; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CMyCGLib.cpp

//## begin module%41E3789E01B3.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%41E3789E01B3.additionalIncludes

//## begin module%41E3789E01B3.includes preserve=yes
#include <math.h>
#include <float.h>

//## end module%41E3789E01B3.includes

// CMyCGLib
#include "CMyCGLib.h"
//## begin module%41E3789E01B3.declarations preserve=no
//## end module%41E3789E01B3.declarations

//## begin module%41E3789E01B3.additionalDeclarations preserve=yes
//## end module%41E3789E01B3.additionalDeclarations


// Class CMyCGLib 


//## Other Operations (implementation)
double CMyCGLib::pp (double* a, double* b, int n)
{
  //## begin CMyCGLib::pp%41E378620058.body preserve=yes

  double c;
  register int i;
  c=0.0;
  for(i=1;i<=n;i=i+1) c=c+a[i-1]*b[i-1];
  return(c);
 
  //## end CMyCGLib::pp%41E378620058.body
}

bool CMyCGLib::normalize (double* v, int n)
{
  //## begin CMyCGLib::normalize%41E3787601F1.body preserve=yes
    double u;
    register int i;
    u=sqrt(pp(v,v,n));
    
	if( u != 0.0)
//	if( u > 0.0000001/*FLT_MIN*100.0*/)
     {
       for(i=0; i<n; i=i+1)
		   v[i]=v[i]/u;
       return(1);
     }
    else 
		return(0);

  //## end CMyCGLib::normalize%41E3787601F1.body
}

void CMyCGLib::cross_product (double p[3], double q[3], double* r)
{
  //## begin CMyCGLib::cross_product%41E378840197.body preserve=yes
	r[1-1]=p[2-1]*q[3-1]-p[3-1]*q[2-1];
	r[2-1]=p[3-1]*q[1-1]-p[1-1]*q[3-1];
	r[3-1]=p[1-1]*q[2-1]-p[2-1]*q[1-1];

  //## end CMyCGLib::cross_product%41E378840197.body
}

void CMyCGLib::sub (double* a, double* b, double* c, int n)
{
  //## begin CMyCGLib::sub%41E3949102F2.body preserve=yes
	for( register int i = 0; i < n; i++)
	{
		c[i] = a[i] - b[i];
	}
  //## end CMyCGLib::sub%41E3949102F2.body
}

void CMyCGLib::scale (double* v0, double* v1, double s, int n)
{
  //## begin CMyCGLib::scale%41E395930158.body preserve=yes
	for( register int i = 0; i < n; i++)
	{
		v1[i] = v0[i]*s;
	}
  //## end CMyCGLib::scale%41E395930158.body
}

bool CMyCGLib::is_ccw (float* lpx, float* lpy, int n)
{
  //## begin CMyCGLib::is_ccw%41E5CFFC016B.body preserve=yes
	double dfArea = 0;
	for(int i=0; i<n; i++)
	{
		dfArea += ( lpx[i]*lpy[(i+1)%n] - lpx[(i+1)%n]*lpy[i]);
	}
	if(dfArea > 0)
		return true;
	else
		return false;	
  //## end CMyCGLib::is_ccw%41E5CFFC016B.body
}

// Additional Declarations
  //## begin CMyCGLib%41E3783A0386.declarations preserve=yes

double CMyCGLib::ComputeTriangleArea(double *v1, double *v2, double *v3)
{
	//compute the vector representing two edges of the triangle
	double e1[3];
	double e2[3];
	int j = 0;
	for ( j=0; j<3; j++)
		e1[j] = v1[j] - v3[j];
	for ( j=0; j<3; j++)
		e2[j] = v2[j] - v3[j];
	
	//compute  the cross product of e1 and e2
	double cp[3];
	cp[0] =  e1[1]*e2[2] - e1[2]*e2[1];
	cp[1] = -(e1[0]*e2[2] - e1[2]*e2[0]);
	cp[2] =  e1[0]*e2[1] - e1[1]*e2[0];
	
	double m = sqrt( cp[0]*cp[0] + cp[1]*cp[1] + cp[2]*cp[2] );
	
	return m/2;
}

double CMyCGLib::DistancePt2Line(double x, double y, double A, double B, double C)
{
	return fabs(x*A + y*B + C) / sqrt(A*A + B*B) ;
}
//## end CMyCGLib%41E3783A0386.declarations

//## begin module%41E3789E01B3.epilog preserve=yes
//## end module%41E3789E01B3.epilog
