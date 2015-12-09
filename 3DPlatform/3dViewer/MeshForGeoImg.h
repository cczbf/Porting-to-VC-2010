// MeshForGeoImg.h: interface for the CMeshForGeoImg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHFORGEOIMG_H__392DDB69_C916_4806_A396_3A8CA5300021__INCLUDED_)
#define AFX_MESHFORGEOIMG_H__392DDB69_C916_4806_A396_3A8CA5300021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mesh3D.h"

class CMesh;
class C2DMesh;

enum GeoimgMeshDispType
{
	eBoundary,
	eInitCut,
	eAugCut,
	eBoundParam,
	eInterParam,
	eCreateGeoImage,
	eReconstruct,
	eFinalCut,
} ;


class CMeshForGeoImg  : public Mesh3D
{
public:
	CMeshForGeoImg();
	virtual ~CMeshForGeoImg();
		
	void SetData(CMesh &mesh);  
	void SetMesh();
	void ReadyForCreateGeoimage(C2DMesh &mesh);

	void    SetDispType(int nType);
    GeoimgMeshDispType GetDispType() const;

	void    OutPut2DMesh(CString filename);   //output 2dmesh  
	void    OutPutGeoimage(CString filename); //output the geoimage into bmp file

	void    OutputGeoimagePixel(int i, int j, float &r, float &g, float &b);
	int     GetGeoimageWid();
	int     GetGeoimageHei();

	int     GetTriangleNum() const;
	void    GetTriangle(int &va, int &vb, int &vc, int index) const;
	int     Get3DPtNum() const;
	void    GetVertex(double &x, double &y, double &z,int index) const;

	int     GetAddPtNum() const;
	void    GetAddPt(double &x, double &y, double &z, bool &bIsNull, int index ) const;
	int     GetAddFaceNum() const;
	void    GetAddFace(int &va, int &vb, int &vc, int &n, int index) const;

	void    GetMeshPt(double &x, double &y, double &z, int index) const;
	void    GetMeshFace(int &va, int &vb, int &vc, int index) const;

	vector<GEOIMG_EDGE_>  GetCutPath() const;  
	vector<MapPoint>      GetBndMapPnt() const;   
	vector<MapPoint>      GetInMapPnt() const;  
	vector<BndPoint>      GetBndPnt() const;   
	vector<newFACE>       GetNewFaces() const; 
	vector<TriAngle>      GetRemeshFaces() const;
	vector<MapPoint>      GetGeoMap() const;

	int    GetReconstructPtNum() const;
	void   GetReconstructPt(double &x, double &y, double &z, int index) const;
	int    GetReconstructTriangleNum() const;
	void   GetReconstructTriangle(int &va, int &vb, int &vc, int index) const;

	double  GetScale() const;

	const vector<MapPoint>& GetBoundMapPointVec();
	const vector<MapPoint>& GetInnerMapPointVec();
	const vector<newFACE>&  GetNewFaceVec();

	//transform the reconstruction model format to simple format
	void    FormatTransform();
	
private:
	GeoimgMeshDispType m_enumType;	
	
	vector<POINT3D>    m_vecPts; //saving the 3D points
    vector<newFACE>    m_vecTriangleIndices; //save the triangle index	
    	
};

#endif // !defined(AFX_MESHFORGEOIMG_H__392DDB69_C916_4806_A396_3A8CA5300021__INCLUDED_)
