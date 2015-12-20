
#include "stdafx.h"
#include "CMesh.h"
#include "DigitalGeometry.h"

#include <vector>
#include <algorithm>
using namespace std;


bool CMPt::operator == (const CMPt& rhs) const
{
	if(  ( (m_a==rhs.GetA()) && (m_b==rhs.GetB()) ) || 
		 ( (m_b==rhs.GetA()) && (m_a==rhs.GetB()) ) )
		return true;
	else 
		return false;
}

CMesh* SubdivisionMesh(const CMesh &mesh)
{

	if(&mesh == NULL)
		return NULL;
       
    int nVertexNum = mesh.GetVertexNum();
	if(nVertexNum <= 0)
		return NULL;
	
	vector<CMPt> mptColl;
	vector<CMPt>::iterator it;
	vector<TiangleIndices> trianglesColl;

	int nIndex = 0;

	for(int i=0; i<mesh.GetTriangleNum(); i++)
	{
		int v1,v2,v3;
		mesh.GetTriangleIndices(v1, v2, v3, i);
		CMPt mpt12(v1, v2);
		CMPt mpt13(v1, v3);
		CMPt mpt23(v2, v3);		
        
		int index12, index13, index23;

		//find the index of new mean point
		it = find(mptColl.begin(), mptColl.end(), mpt12);
		if( it == mptColl.end() )
		{
			mptColl.push_back(mpt12);
			index12 = nIndex;
			nIndex++;
		}
		else
		{
			index12 = distance(mptColl.begin(), it);
		}

		it = find(mptColl.begin(), mptColl.end(), mpt13);
		if( it == mptColl.end() )
		{
			mptColl.push_back(mpt13);
			index13 = nIndex;
			nIndex++;
		}
		else
		{
			index13 = distance(mptColl.begin(), it);
		}

		it = find(mptColl.begin(), mptColl.end(), mpt23);
		if( it == mptColl.end() )
		{
			mptColl.push_back(mpt23);
			index23 = nIndex;
			nIndex++;
		}
		else
		{
			index23 = distance(mptColl.begin(), it);
		}

		index12 += nVertexNum;
		index13 += nVertexNum;
		index23 += nVertexNum;

		//construct new triangles;
		TiangleIndices newTriangles[4];
		newTriangles[0].v1 = v1;
		newTriangles[0].v2 = index12;
		newTriangles[0].v3 = index13;
		
		newTriangles[1].v1 = index12;
		newTriangles[1].v2 = v2;
		newTriangles[1].v3 = index23;

		newTriangles[2].v1 = index13;
		newTriangles[2].v2 = index23;
		newTriangles[2].v3 = v3;

		newTriangles[3].v1 = index12;
		newTriangles[3].v2 = index23;
		newTriangles[3].v3 = index13;

		for(int j=0; j<4; j++)
			trianglesColl.push_back(newTriangles[j]);
	}
   

	CMesh *pMesh = new CMesh();
	//input the vertex and triangle infor into new mesh
	int   nVertexSize = mesh.GetVertexNum() + mptColl.size();
	double *x = new double[nVertexSize];
	double *y = new double[nVertexSize];
	double *z = new double[nVertexSize];
	for(int i=0; i<mesh.GetVertexNum(); i++)
	{
		float fx, fy, fz;
		mesh.GetVertex(fx, fy, fz, i);
		x[i] = (double)fx * 10;
		y[i] = (double)fy * 10;
		z[i] = (double)fz * 10;
	}
    
	for(int i=0; i<mptColl.size(); i++)
	{
		int indexA = mptColl[i].GetA();
		int indexB = mptColl[i].GetB();
		float xa,ya,za;
		float xb,yb,zb;
		mesh.GetVertex(xa, ya, za, indexA);
		mesh.GetVertex(xb, yb, zb, indexB);

		x[i + mesh.GetVertexNum()] = ( (xa + xb) / 2 ) * 10;
		y[i + mesh.GetVertexNum()] = ( (ya + yb) / 2 ) * 10;
		z[i + mesh.GetVertexNum()] = ( (za + zb) / 2 ) * 10;		
	}

	int *va = new int[trianglesColl.size()];
	int *vb = new int[trianglesColl.size()];
	int *vc = new int[trianglesColl.size()];
	for(int i=0; i<trianglesColl.size(); i++)
	{
	   int v1,v2,v3;
	   v1 = trianglesColl[i].v1;
	   v2 = trianglesColl[i].v2;
	   v3 = trianglesColl[i].v3;

	   va[i] = v1;
	   vb[i] = v2;
	   vc[i] = v3;
	}

	pMesh->SetMesh(trianglesColl.size(), va, vb, vc,
		NULL,NULL,NULL,NULL,NULL,NULL,
		nVertexSize, x, y, z,
		0, "", "", "");

	delete x;
	delete y;
	delete z;
	delete va;
	delete vb;
	delete vc;

	return pMesh;
}