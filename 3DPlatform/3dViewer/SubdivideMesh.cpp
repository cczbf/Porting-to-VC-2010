// SubdivideMesh.cpp: implementation of the CSubdivideMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SubdivideMesh.h"
#include "CMesh.h"

#include "CMyCGLib.h"
#include "math.h"
#include "matrix.h"
#include "Triangles.h"

#include "FLOAT.h"
#include "progressbar.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubdivideMesh::CSubdivideMesh()
{
	m_pMesh = NULL;
	m_subPtColl.clear();
	m_subTriangleColl.clear();	

	memset(m_T, 0 ,sizeof(double)*3);
	memset(m_R, 0 ,sizeof(double)*9);
	memset(m_iR, 0 ,sizeof(double)*9);
}

CSubdivideMesh::~CSubdivideMesh()
{
	if(m_pMesh != NULL)
		delete m_pMesh;

	m_subPtColl.clear();
	m_subTriangleColl.clear();	

}

void CSubdivideMesh::Init(const char *filename)
{
	if(filename != NULL)
	{
		m_pMesh = new CMesh;
		m_pMesh->Load(filename);
	}
}

void CSubdivideMesh::Init(const CMesh &mesh)
{
	if(&mesh != NULL)
	{
		m_pMesh = new CMesh(mesh);
	}
}

void CSubdivideMesh::ComputeTransformation(double *vecA, double *vecB, double *vecC)
{
	if(vecA !=NULL && vecB != NULL && vecC != NULL)
	{		
		m_T[0] = vecA[0];
		m_T[1] = vecA[1];
		m_T[2] = vecA[2];
		
		double normal[3];
		double vecAC[3], vecAB[3], vecBC[3];
		double tv[3];
		
		CMyCGLib cglib;
		
		cglib.sub(vecB, vecA, tv, 3);
		vecAB[0] = tv[0];
		vecAB[1] = tv[1];
		vecAB[2] = tv[2];
		
		cglib.sub(vecC, vecA, tv, 3);
		vecAC[0] = tv[0];
		vecAC[1] = tv[1];
		vecAC[2] = tv[2];
		
		cglib.sub(vecC, vecB, tv, 3);
		vecBC[0] = tv[0];
		vecBC[1] = tv[1];
		vecBC[2] = tv[2];
		
		//compute the normal
		cglib.cross_product(vecAB, vecAC, normal);
		cglib.normalize(normal, 3);
		
		double tA[3][3];  //transformed coordinate
		double A[3][3];   //original  coordinate
		
		//the original data:
		A[0][0] = vecB[0] - vecA[0];  A[0][1] = vecC[0] - vecA[0];  A[0][2] = normal[0];
		A[1][0] = vecB[1] - vecA[1];  A[1][1] = vecC[1] - vecA[1];  A[1][2] = normal[1];
		A[2][0] = vecB[2] - vecA[2];  A[2][1] = vecC[2] - vecA[2];  A[2][2] = normal[2];
		
		//compute the transformed coordinate:
		double Lab = sqrt( cglib.pp(vecAB, vecAB, 3) ); 
		double Lac = sqrt( cglib.pp(vecAC, vecAC, 3) ); 
		double Lbc = sqrt( cglib.pp(vecBC, vecBC, 3) ); 
		
		double angleBAC = acos( (Lab*Lab + Lac*Lac - Lbc*Lbc)/ (2*Lab*Lac) );
		
		tA[0][0] = Lab;  tA[0][1] = Lac*cos(angleBAC);  tA[0][2] = 0;
		tA[1][0] = 0;    tA[1][1] = Lac*sin(angleBAC);  tA[1][2] = 0;
		tA[2][0] = 0;    tA[2][1] = 0;                  tA[2][2] = 1;
		
		//compute transform matrix
		invers_matrix(*A, 3);
		mult(*tA, *A, *m_R, 3, 3 ,3);	
		//
		for(int i=0; i<3; i++)
			for(int j=0; j<3; j++)
			{
				m_iR[j][i] = m_R[j][i];
			}
			invers_matrix(*m_iR, 3);
			
		//////////////////////////////////////////////////////////////////////////
		//return the corresponding 2D coordinats of 3D points
		vecA[0] = 0;   vecA[1] = 0; vecA[2] = 0;
		vecB[0] = Lab; vecB[1] = 0; vecB[2] = 0;
		vecC[0] = Lac*cos(angleBAC); vecC[1] = Lac*sin(angleBAC);  vecC[2] = 0;
		//
	}
}


void CSubdivideMesh::Compute3DPt(double x, double y, double *grd)
{
	//compute the corresponding 3D point	
	if(grd != NULL)
    {		
		double planePt[3];				
		planePt[0] = x;
		planePt[1] = y;
		planePt[2] = 0;
		
		mult(*m_iR, planePt, grd, 3, 3, 1 );
		for(int ii=0; ii<3; ii++)
		{
			grd[ii] += m_T[ii]; 
		}
	}
}


int CSubdivideMesh::InsertSubPt(SubPt spt)
{	    
	vector<SubPt>::iterator ip;
	ip = find(m_subPtColl.begin(), m_subPtColl.end(), spt); 

	if( ip == m_subPtColl.end() )
	{		
		//compute the 3D coordinate	
		Compute3DPt(spt.px[0], spt.px[1], spt.x);
		//insert
		m_subPtColl.push_back(spt);	
		return m_subPtColl.size()-1;
	}
	else
	{
		return distance(m_subPtColl.begin(), ip);
	}
}


/*
 *   Subdivide a triangle
 */

void CSubdivideMesh::SubdivideTriangle(	double xa, double ya, 
						double xb, double yb,
						double xc, double yc,
						int    vA,
						int    vB,
						int    vC,
						double dfLen)
{

	//float tx[3],ty[3];
	//tx[0] = xa; tx[1] = xb; tx[2] = xc;
	//ty[0] = ya; ty[1] = yb; ty[2] = yc;

	vector<double>   xColl;
	vector<double>   yColl;
	vector<int>      indexColl; //corresponding 3D point index
	//vector<SimpleTriangle> eachTriColl;

    double dMinLen = dfLen;
		
	xColl.clear();
	yColl.clear();
	indexColl.clear();
	//eachTriColl.clear();
	   
	SubPt spt;
	//add the edge points
	xColl.push_back(xa);	yColl.push_back(ya);
	spt.v1 = vA ;   spt.v2 = vA ;  spt.subIndex = 0;
	spt.px[0] = xa; spt.px[1] = ya;
	int tI = InsertSubPt(spt);
	indexColl.push_back(tI);

	xColl.push_back(xb);	yColl.push_back(yb);
	spt.v1 = vB ;   spt.v2 = vB ;  spt.subIndex = 0;
	spt.px[0] = xb; spt.px[1] = yb;
	tI = InsertSubPt(spt);
	indexColl.push_back(tI);
	
	xColl.push_back(xc);	yColl.push_back(yc);
	spt.v1 = vC ;   spt.v2 = vC ;  spt.subIndex = 0;
	spt.px[0] = xc; spt.px[1] = yc;
	tI = InsertSubPt(spt);
	indexColl.push_back(tI);
	
	//{{edge AB
	int nSubNum = xb/dMinLen - 0.5;
	for(int i=1; i<=nSubNum; i++)
	{
		if(vA < vB)
		{
			spt.v1 = vA;
			spt.v2 = vB;
			
			spt.subIndex = i;
		}	
		else
		{
			spt.v1 = vB;
			spt.v2 = vA;

			spt.subIndex = nSubNum - i + 1;
		}

		spt.px[0] = xa + i*dMinLen;
		spt.px[1] = ya;

		tI = InsertSubPt(spt);
		indexColl.push_back(tI);
		
		xColl.push_back(xa + i*dMinLen );
		yColl.push_back(ya);
	}
	//}}edge AB

	//{{edge AC
	//compute the direction
	CMyCGLib cgLib;
	double dAC[2];
	dAC[0] = xc - xa;
	dAC[1] = yc - ya;
	cgLib.normalize(dAC, 2);
	double Lab = sqrt( (xa-xc)*(xa-xc) + (ya-yc)*(ya-yc) );
	nSubNum = Lab/dMinLen - 0.5;
	for(i=1; i<=nSubNum; i++)
	{	
		if(vA < vC)
		{
			spt.v1 = vA;
			spt.v2 = vC;

			spt.subIndex = i;
		}
		else
		{
			spt.v1 = vC;
			spt.v2 = vA;

			spt.subIndex = nSubNum - i + 1;
		}
		
		spt.px[0] = dAC[0]*dMinLen*i;
		spt.px[1] = dAC[1]*dMinLen*i;	


		tI = InsertSubPt(spt);
		indexColl.push_back(tI);
		
		xColl.push_back(dAC[0]*dMinLen*i);
		yColl.push_back(dAC[1]*dMinLen*i);
	}    
	//}}edge AC
	
	//{{edge BC
	double dBC[2];
	dBC[0] = xc - xb;
	dBC[1] = yc - yb;
	cgLib.normalize(dBC, 2);
	double Lbc = sqrt( (xb-xc)*(xb-xc) + (yb-yc)*(yb-yc) );
	nSubNum = Lbc/dMinLen - 0.5;

    for(i=1; i<=nSubNum; i++)
	{
		if(vB < vC)
		{
			spt.v1 = vB;
			spt.v2 = vC;

     		spt.subIndex = i;	
		}
		else
		{
			spt.v1 = vC;
			spt.v2 = vB;

			spt.subIndex = nSubNum - i + 1;
		}

		spt.px[0] = dBC[0]*dMinLen*i + xb;
		spt.px[1] = dBC[1]*dMinLen*i + yb;

		tI = InsertSubPt(spt);
		indexColl.push_back(tI);
		
		xColl.push_back(dBC[0]*dMinLen*i + xb);
		yColl.push_back(dBC[1]*dMinLen*i + yb);
	}
	//}}edge BC
	
	//{{Added grid point inside the triangle
    int nInsideIndex = 0;
	float fScale = 1000000;//because the rgn only accept integer, so multiply 10000

	CRgn rgn;
	POINT pts[3];
	pts[0].x = xa * fScale;  
	pts[0].y = ya * fScale;
	
	pts[1].x = xb * fScale;
	pts[1].y = yb * fScale;

	pts[2].x = xc * fScale;
	pts[2].y = yc * fScale;
 
	rgn.CreatePolygonRgn(pts, 3, ALTERNATE );
	
    //must consider xc???
	int nWidNum  = 0;
	float xInit = 0;
    if(xc<0)
	{
		nWidNum = (xb - xc) / dMinLen - 0.5;
		xInit = xc;
	}
	else if(xc < xb)
	{
		nWidNum = xb / dMinLen - 0.5;
		xInit = 0;
	}
	else
	{
		nWidNum = xc / dMinLen - 0.5;
		xInit = 0;
	}	
	
	int nHeiNum = yc / dMinLen - 0.5;

	CMyCGLib cglib;

	double bcA = yb - yc;
	double bcB = xc - xb;
	double bcC = (xb-xc)*yc - (yb-yc)*xc ;

	for(int j=1; j<=nHeiNum; j++)
	{
		for(int i=1; i<=nWidNum; i++)
		{

			double tx = xInit + i*dMinLen ;//- (dMinLen/2);
			double ty = j*dMinLen ;//- (dMinLen/2);

			POINT pt;
			pt.y =  ty * fScale;
			pt.x =  tx * fScale;
        		
			if( rgn.PtInRegion(pt))
			{
				//compute the ditance from point to the three edge:
				double dAB = ty;
				double dAC = cglib.DistancePt2Line( tx, ty, yc, -xc, 0 );
				double dBC = cglib.DistancePt2Line( tx, ty, bcA, bcB, bcC );
				
				double dfMin = min(dAB, min(dAC,dBC));

				if( (dfMin - dMinLen/2) >= 0 ) 
				{					
					spt.v1 = -1;
					spt.v2 = -1;
					spt.subIndex = 0;
					
					//compute the corresponding 3D point
					Compute3DPt(tx, ty, spt.x);

					TRACE("tx:%lf  ty:%lf\n",tx, ty);
					
					int nCurrentSize = m_subPtColl.size();
					indexColl.push_back( m_subPtColl.size() );  //inside points are all new subdivide point               
					m_subPtColl.push_back(spt);	
					
					xColl.push_back(tx);
					yColl.push_back(ty);		
				}
			}
		}
	}
	//}}Added grid point inside the triangle
		
	
	//{{generate the 2D Delaunay Triangles
	//eachTriColl.clear();

	ASSERT(xColl.size() == indexColl.size());

	CTINClass tin(" pczAenVQ ");
	tin.BeginAddPoints();
    for(i=0; i<xColl.size(); i++)
	{		
		tin.AddPoint(xColl[i], yColl[i], indexColl[i]);
	}

	tin.EndAddPoints();
	tin.FastConstruct();
	tin.EnableIntersection( false );
	
	long nTriangleNum = 0 ;
	TRIANGLE **tris = tin.SelectTriangles(&nTriangleNum, 0, 0, 0, 0 );
		
	for (i=0; i<nTriangleNum; i++)
	{	
		double v1[3];
		double v2[3];
		double v3[3];
		int    index1, index2, index3;
		
		v1[0] =  (*tris)->vertex[0]->x;
		v1[1] =  (*tris)->vertex[0]->y;
		v1[2] = 0;
		index1 = int((*tris)->vertex[0]->attr);
		
		v2[0] =  (*tris)->vertex[1]->x;
		v2[1] =  (*tris)->vertex[1]->y;
		v2[2] = 0;
		index2 = int((*tris)->vertex[1]->attr);
		
		v3[0] =  (*tris)->vertex[2]->x;
		v3[1] =  (*tris)->vertex[2]->y;
		v3[2] = 0;
		index3 = int((*tris)->vertex[2]->attr);

		double s = cgLib.ComputeTriangleArea(v1, v2, v3);
		
		if(s > 1e-10)
		{			
			SimpleTriangle tri;
			
			tri.va = index1;
			tri.vb = index2;
			tri.vc = index3;        
			
			//eachTriColl.push_back(tri);   //for each triangle
			m_subTriangleColl.push_back(tri); //for all the new triangles

			TRACE("%d %d %d \n", index1, index2, index3);
		}
		*tris++;		 
	}
	//}}generate the 2D Delaunay Triangles
}

CMesh* CSubdivideMesh::OutputSubMesh(double dfLen)
{	
	//ASSERT(m_pMesh != NULL);

	if(m_pMesh != NULL )
	{
		if(m_pMesh->GetVertexNum())
		{				
			//double dfMinEdge = m_pMesh->GetLenOfShortestEdge();						
			
			if( dfLen < 1e-6 )
			{
				AfxMessageBox(" the length of min edge is too small! ");
				return NULL ; 
			}

			//{{clear the container for each triangle
			m_subTriangleColl.clear();
			m_subPtColl.clear();	
			//}}clear the container for each triangle
			
			CProgressBar *pbar = new CProgressBar(_T("Subdividing Mesh..."), 100, 100, TRUE);
			pbar->SetText("Subdividing Mesh...");
			double step = 100.0/(double)(m_pMesh->GetTriangleNum());
			double prog = 0;
			
			//{{subdivide each triangle
			for(int i=0; i<m_pMesh->GetTriangleNum(); i++)
			{
				prog += step;
				pbar->SetPos((int)prog);				
				
				int v[3];
				m_pMesh->GetTriangleIndices(v[0], v[1], v[2], i);
	
				float vecA[3],vecB[3],vecC[3];
				m_pMesh->GetVertex(vecA[0], vecA[1], vecA[2], v[0]);
				m_pMesh->GetVertex(vecB[0], vecB[1], vecB[2], v[1]);
				m_pMesh->GetVertex(vecC[0], vecC[1], vecC[2], v[2]);
				double dVecA[3], dVecB[3], dVecC[3];
				for(int ii=0; ii<3; ii++)
				{
					dVecA[ii] = vecA[ii];
					dVecB[ii] = vecB[ii];
					dVecC[ii] = vecC[ii];
				}
				
				ComputeTransformation(dVecA, dVecB, dVecC);				
				SubdivideTriangle(dVecA[0], dVecA[1],
					dVecB[0], dVecB[1],
					dVecC[0], dVecC[1],
					v[0],
					v[1],
					v[2],
					dfLen);		
			} 
			//{{subdivide each triangle			
			delete pbar;
			
			//{{construct a new mesh 
			CMesh *pMesh = new CMesh;			
			double *x = new double[m_subPtColl.size()];
			double *y = new double[m_subPtColl.size()];
			double *z = new double[m_subPtColl.size()];       
			for(i=0; i<m_subPtColl.size(); i++)
			{
				x[i] = m_subPtColl[i].x[0];
				y[i] = m_subPtColl[i].x[1];
				z[i] = m_subPtColl[i].x[2];
			}
			
			int *nV1 = new int[m_subTriangleColl.size()];
			int *nV2 = new int[m_subTriangleColl.size()];
			int *nV3 = new int[m_subTriangleColl.size()];
			for(i=0; i<m_subTriangleColl.size(); i++)
			{
				nV1[i] = m_subTriangleColl[i].va;
				nV2[i] = m_subTriangleColl[i].vb;
				nV3[i] = m_subTriangleColl[i].vc;
			}
			
			pMesh->SetMesh(m_subPtColl.size(),
				x, y, z,
				m_subTriangleColl.size(),
				nV1, nV2, nV3);
			
			delete x;
			delete y;
			delete z;
			delete nV1;
			delete nV2;
			delete nV3;
			//}}construct a new mesh 
			
			return pMesh;		
	}
	else
		return  NULL;
	}
	else
		return  NULL;
}
