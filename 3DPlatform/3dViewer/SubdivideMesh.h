// SubdivideMesh.h: interface for the CSubdivideMesh class.
// 
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBDIVIDEMESH_H__EAA829F1_BEF0_4EAC_B938_EBD46BC07A4B__INCLUDED_)
#define AFX_SUBDIVIDEMESH_H__EAA829F1_BEF0_4EAC_B938_EBD46BC07A4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

class CMesh;

struct SimpleTriangle
{
	int va,vb,vc;
};

//describe the subdividing point 
struct SubPt
{
	int v1,v2;      //the end point 
	int subIndex;   //the index (from v1 to v2)

	double px[2];  //the plane points 
	double x[3];   //the corresponding 3D point
	
	bool operator==(const SubPt spt)
	{
		if( v1 == spt.v1 && v2 == spt.v2 && subIndex == spt.subIndex )
			return true;
		else
			return false;
	}
};


class CSubdivideMesh   
{
public:
	CSubdivideMesh();
	virtual ~CSubdivideMesh();
    
	void Init(const char *filename);     //initialization from file
    void Init(const CMesh &mesh);  //initialization from CMesh 
    CMesh* OutputSubMesh(double dfLen);

#ifdef _DEBUG
public:
#else
private:
#endif
	void ComputeTransformation(double *vecA, double *vecB, double *vecC);
	void Compute3DPt(double x, double y, double *grd);
	int  InsertSubPt(SubPt spt);
	void SubdivideTriangle(	double xa, double ya, 
						double xb, double yb,
						double xc, double yc,
						int    vA,
						int    vB,
						int    vC,
						double dfLen = 0.01);


    CMesh *m_pMesh;  //the raw mesh data
    
	//{{data for algorithm
    //for new subdivided points and triangles
	vector<SubPt>    m_subPtColl;             //coding the new subdivided point
	vector<SimpleTriangle> m_subTriangleColl;
	double m_T[3];   //the translation 
	double m_R[3][3], m_iR[3][3];
	//}}data for algorithm
};

#endif // !defined(AFX_SUBDIVIDEMESH_H__EAA829F1_BEF0_4EAC_B938_EBD46BC07A4B__INCLUDED_)
