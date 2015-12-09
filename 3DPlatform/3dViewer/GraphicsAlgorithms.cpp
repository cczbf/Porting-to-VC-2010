
#include "stdafx.h"
#include "GraphicsAlgorithms.h"
#include "math.h"


POINT3D CrossValue(POINT3D p1,POINT3D p2)
{

	POINT3D p;
	p.x=p1.y*p2.z-p1.z*p2.y;
	p.y=-(p1.x*p2.z-p1.z*p2.x);
    p.z=p1.x*p2.y-p1.y*p2.x;

	double norm = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);

	p.x = p.x /norm;
	p.y = p.y /norm;
	p.z = p.z /norm;

	return p;

}