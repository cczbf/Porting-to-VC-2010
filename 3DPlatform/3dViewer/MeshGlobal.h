#ifndef MESH_GLOBAL_
#define MESH_GLOBAL_

class CMesh ;


CMesh* ExtrudeMesh(const double *x, const double *y, 
				   const int nPtNum, const double dfHeight = 1);

CMesh* ExtrudeMesh1(const int nContour, const int *pContNum, 
					double *x, double* y, double dfHeight = 1);


CMesh* RotateMesh(const double *z, const double *x, const int nPtNum, const int nSlices = 10);

CMesh* RotateMesh1(const double *z, const double *x, const int nPtNum, const int nSlices = 10);


void   OrderVertex(const int nContour, const int *pContNum, double *x, double *y);
double ComputePolyArea(const int nVertex, const double* x, const double *y);



#endif