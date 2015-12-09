
#ifndef	TWODMESH_H
#define TWODMESH_H	

#include "CMesh.h"
#include "typedef.h"

#include <vector>
using namespace std;

class CMeshForGeoImg;
  

/* C2DMesh: 
 * For Geometry Image .
 * By xiedonghai,2004.12.13.
 */
class C2DMesh  : public CMesh
{
public:
	C2DMesh();
	C2DMesh(const C2DMesh &mesh);
	
	virtual ~C2DMesh();
	
	//override the Load()
	bool    Load(CString file);
    void    ComputeColor();
    void    ComputeTriangleArea();
    
	void    Get2DPtIndexTo3D(int &indexTo3D, int index);

	int     Get3DPointNum();
	void    Get3DPoint(float &x, float &y, float &z, int index);
	int     GetBorderLen();

	//Save the data into 2D mesh format
    virtual BOOL Write (CString strFile);
		
	//Functions for Geometry Displaying  
	void    DisplayCutpath();
	void    DisplayBoundaryMap();
	void    DisplayInterior2();
	void    DisplayGeoImage();
	void    DisplayReconstruct();
	
	
	POINT3D meshPnt(int n);
	stFACE  meshFaces(int n);
	int     FindMaxStrTri();
	void    computeGeoStretch(double*);
	void    Reconstruct();
	void    CreateGeoImage();

	void    CreateStretchRatio();

	/*
	//set data
	void    SetMesh(int nVertexNum, double *x, double *y, double *z,
		            int nTriangle,  int *nV1, int *nV2, int *nV3,
					POINT3D *pAddPts,
					int     nAddPts,
					stFACE  *pAddFaces,
					int     nAddFaces, 
					double  dfScale,
					vector<GEOIMG_EDGE_> &cutPath,
					vector<MapPoint> &vBndMapPnt,
					vector<MapPoint> &vInMapPnt,
					vector<BndPoint> &vBndPnt,
					vector<newFACE>  &vNewFaces,
					vector<TriAngle> &vRemeshFaces,	
					vector<MapPoint> &vGeoMap,
					int nGeoImgMeshDispType = 0);
					*/

	void    SetMesh(const CMeshForGeoImg &meshForGeoimg);

    //set the data
    void    SetMesh( int nTriangle, int* nV1, int* nV2, int* nV3, 
		             DWORD* c12, DWORD* c23, DWORD* c31, 
					 double* ratio12, double* ratio23, double* ratio31, 
					 int nVlength, double* x, double* y, double* z, 
					 int nEdgeNodeNum, 
					 CString strAverage, CString strConviance, CString strGeoRatio);
	  
	int     GetDispType();

private:
	
	//saving the corresponding 3D points
	int   m_n3DPtNum;
	int   m_nBorderLen;

	float *m_p3Dx; 
	float *m_p3Dy;
	float *m_p3Dz;
  		
	int   m_nGeoimgMeshDispType ;

	//Data for Geometry Displaying  
   	vector<GEOIMG_EDGE_> m_vCutPath;	
	vector<MapPoint> m_vInMapPnt;
	vector<BndPoint> m_vBndPnt;
	vector<newFACE>  m_vNewFaces;
	vector<TriAngle> m_vRemeshFaces;
	
	
	double m_lfScale;
	double* m_lfStat;

	//
	POINT3D *m_pPts;	
	stFACE  *m_pFaces;
	int m_nFaceSum;
	int m_nPtSum;
	
	//
	int m_nAddFaces;
	int m_nAddPnt;
	POINT3D *m_pAddPts;
	stFACE  *m_pAddFaces;

	//geometry image
	//MapPoint **m_pGeoMap;

	vector<MapPoint> m_vGeoMap;
	vector<MapPoint> m_vBndMapPnt;

};



#endif