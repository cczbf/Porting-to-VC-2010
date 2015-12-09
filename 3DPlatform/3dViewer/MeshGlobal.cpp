#include "stdafx.h"
#include "MeshGlobal.h"
#include "CMesh.h"

#include "Triangles.h"
#include "float.h"

#include "math.h"

#include "PolyTri.h"


#define  PI  3.1415926

#include <vector>
using std::vector;

struct TriangleIndex
{
	int va;
	int vb;
	int vc;
};


struct SurfacePt
{
	double u,v;
	float r;
};


CMesh* ExtrudeMesh(const double *x, const double *y, 
				   const int nPtNum, const double dfHeight)
{	
	if(nPtNum)
	{	
		//{{ Construct the triangles 
		vector<TriangleIndex> triangleColl;
		TriangleIndex triangle;

		for(int i=0; i<nPtNum; i++)
		{
			
			triangle.vc = i; 
			triangle.vb = (i + 1) % nPtNum; 
			triangle.va = i + nPtNum;
			
			triangleColl.push_back(triangle);
			
			triangle.vc = (i + 1) % nPtNum; 
			triangle.vb = (i + 1) % nPtNum + nPtNum; 
			triangle.va = i + nPtNum;
			
			triangleColl.push_back(triangle);
		}

		/*
		//using Delaynay Triangle to construct the top and bottom
		for(i=0; i<2; i++)
		{			
			CTINClass tin(" pczAenVQ ");
			//tin.MarkHull();
			
			tin.BeginAddPoints();

			for(int j=0; j<nPtNum; j++)
			{
				tin.AddPoint(x[j], y[j], j + i*nPtNum);
			}
			tin.EndAddPoints();
			tin.FastConstruct();
			tin.EnableIntersection( FALSE );
			
			LONG nTriangleNum = 0 ;		
			TRIANGLE **tris = tin.SelectTriangles(&nTriangleNum, 0, 0, 0, 0 );		
			if(i==0)
			{				
				for(int ii=0; ii<nTriangleNum; ii++)
				{
					triangle.vc = int((*tris)->vertex[0]->attr);
					triangle.vb = int((*tris)->vertex[1]->attr);
					triangle.va = int((*tris)->vertex[2]->attr);
					triangleColl.push_back(triangle);
					
					*tris++;
				}
			}
			else
			{				
				for(int ii=0; ii<nTriangleNum; ii++)
				{
					triangle.va = int((*tris)->vertex[0]->attr);
					triangle.vb = int((*tris)->vertex[1]->attr);
					triangle.vc = int((*tris)->vertex[2]->attr);
					triangleColl.push_back(triangle);
					
					*tris++;
				}
				
			}
		}
		//}} Construct the triangles
		*/
		

		//{{construct the new mesh
		double *z[2];
		z[0] = new double[nPtNum];
		z[1] = new double[nPtNum];
		for( i=0; i<nPtNum; i++)
			z[0][i] = 0;
		for(i=0; i<nPtNum; i++)
			z[1][i] = dfHeight;
        		
		double *tx = new double [nPtNum*2];//+ 2];
		double *ty = new double [nPtNum*2];// + 2];
		double *tz = new double [nPtNum*2];// + 2];
         
		for(i=0; i<2; i++)
		{
			for(int j=0; j<nPtNum; j++)
			{
				tx[i*nPtNum + j] = x[j];
				ty[i*nPtNum + j] = y[j];
				tz[i*nPtNum + j] = z[i][j];				 
			}
		}
	    
		
		int *va = new int[triangleColl.size()];
		int *vb = new int[triangleColl.size()];
		int *vc = new int[triangleColl.size()];
		for(i=0; i<triangleColl.size(); i++ )
		{
			va[i] = triangleColl[i].va;
			vb[i] = triangleColl[i].vb;
			vc[i] = triangleColl[i].vc;
		}
		
		
		CMesh *pMesh = new CMesh;
		pMesh->SetMesh(nPtNum*2, tx ,ty, tz,
			triangleColl.size(),
			va, vb, vc);
		//}}construct the new mesh
				
		//release mem
		delete []z[0];
		delete []z[1];
				
		delete tx;
		delete ty;
		delete tz;
		
		delete va;
		delete vb;
		delete vc;
		
		return pMesh;
	}
	else
	{
		return NULL;
	}
}

CMesh* ExtrudeMesh1(const int nContour, const int *pContNum, 
					double *x, double* y, const double dfHeight )
{	
	if(nContour)
	{	
		OrderVertex(nContour, pContNum, x, y);

		vector<double> xColl;
		vector<double> yColl;
		vector<double> zColl;
		vector<TriangleIndex> triColl;
		
		//{{construct the side face
		for(int i=0; i<nContour; i++)
		{
			double *tx = new double[pContNum[i]];
			double *ty = new double[pContNum[i]];
			
			int nOff = 0;
			for(int ii=0; ii<i; ii++)
				nOff += pContNum[ii];
			
			for(int j=0; j<pContNum[i]; j++)
			{
				tx[j] = x[nOff + j];
				ty[j] = y[nOff + j];
			}
			
			CMesh *pMesh = ExtrudeMesh(tx, ty, pContNum[i], dfHeight);
			//add the triangle    
			for(j=0; j<pMesh->GetTriangleNum(); j++)
			{
				TriangleIndex tri;
				pMesh->GetTriangleIndices(tri.vc, tri.vb, tri.va, j);
				tri.va += xColl.size();
				tri.vb += xColl.size();
				tri.vc += xColl.size();
				triColl.push_back(tri);
			}
			//add the point
			for(j=0; j<pMesh->GetVertexNum(); j++)
			{
				float fx,fy,fz;
				pMesh->GetVertex(fx, fy, fz, j);
				
				xColl.push_back(fx);
				yColl.push_back(fy);
				zColl.push_back(fz);
			}

			delete pMesh;
			delete tx;
			delete ty;
		}
		//}}construct the side face
		
		//{{construct the top and bottom
		//bottom	
		CPolyTri plyTri;
		plyTri.Init(nContour, pContNum, x, y);
		int *v1 = new int[1000];
		int *v2 = new int[1000];
		int *v3 = new int[1000];
		int nTriangle = plyTri.Triangulate(v1, v2 ,v3);
         
		//because the index in Polygon Triangulation is starting  from 1
		//so substract 1
		for(i=0; i<nTriangle; i++)
		{
			v1[i] = v1[i] - 1;
			v2[i] = v2[i] - 1;
			v3[i] = v3[i] - 1;
		}

		for(i=0; i<nTriangle; i++)
		{
			TriangleIndex tri;	
			tri.vc = (v1[i]); // + xColl.size());
			tri.vb = (v2[i]); // + xColl.size());
			tri.va = (v3[i]); // + xColl.size());
			triColl.push_back(tri);
		}
		/*
		int nContourPts = 0 ;
		for(i=0; i<nContour; i++)
			nContourPts += pContNum[i];
		for(i=0; i<nContourPts; i++)
		{
			xColl.push_back(x[i]);
			yColl.push_back(y[i]);
			zColl.push_back(0);
		} */ 
		//top
		for(i=0; i<nTriangle; i++)
		{
			TriangleIndex tri;	
			tri.va = v1[i] + xColl.size()/2;
			tri.vb = v2[i] + xColl.size()/2;
			tri.vc = v3[i] + xColl.size()/2;
			triColl.push_back(tri);
		}
		/*
		for(i=0; i<nContourPts; i++)
		{
			xColl.push_back(x[i]);
			yColl.push_back(y[i]);
			zColl.push_back(dfHeight);
		}    
		*/

		delete v1;
		delete v2;
		delete v3;
		//}}construct the top and bottom
		
		double *dx,*dy,*dz;
		ASSERT(xColl.size() == yColl.size());
		ASSERT(xColl.size() == zColl.size());
		ASSERT(zColl.size() == yColl.size());
		dx = new double[xColl.size()];
		dy = new double[yColl.size()];
		dz = new double[zColl.size()];
		for(i=0; i<xColl.size();i++)
		{
			dx[i] = xColl[i];
			dy[i] = yColl[i];
			dz[i] = zColl[i];
		}
		int *va = new int[triColl.size()];
		int *vb = new int[triColl.size()];
		int *vc = new int[triColl.size()];
		for(i=0; i<triColl.size(); i++)
		{
			va[i] = triColl[i].va;
			vb[i] = triColl[i].vb;
			vc[i] = triColl[i].vc;
		}
		
		CMesh *pMesh = new CMesh;
		pMesh->SetMesh(xColl.size(), dx , dy, dz,
			triColl.size(),
			va, vb, vc);
		
		delete dx;
		delete dy;
		delete dz;
		
		delete va;
		delete vb;
		delete vc;
		
		return pMesh;
	}
	else
		return NULL;
}




CMesh* RotateMesh(const double *z, const double *x, const int nPtNum, const int nSlices)
{		
	    //////////////////////////////////////////////////////////////////////////
	  	vector<SurfacePt> sptColl;  //surface points
		if(nPtNum)
		{			
			CTINClass tin(" pczAenVQ ");
			tin.BeginAddPoints();
			
			int nSamplePtNum = nSlices;
			int nIndex = 0;
			
			float fMaxZ = FLT_MIN;
			float fMinZ = FLT_MAX;
			
			SurfacePt spt;
			
			for(int i=0; i<nPtNum; i++)
			{
				double u,v;    // u ---longtitude ; v---latitude
				double ur,vr;
				//double ul,vl;
				
				int nSamplePt = nSamplePtNum ;//* x[i]; //compute the sample points corresponding to radius x[i]
				float fStep = 1.0 / nSamplePt;
								
				if(fMaxZ < z[i])
					fMaxZ = z[i];
				if(fMinZ > z[i])
					fMinZ = z[i];
				
				for(int j=0; j<nSamplePt; j++)
				{
					v = z[i] ; 
					u = fStep * j;	
					
					ur = u + 1.01;
					vr = v;					
					
					spt.r = x[i];
					spt.u = u;
					spt.v = v;	
					
					sptColl.push_back(spt);					
					tin.AddPoint(u,  v,  nIndex, nIndex);
					tin.AddPoint(ur, vr, nIndex, nIndex);		
					
					nIndex++;
				}				
			}
			
            //add two points
            spt.v = fMaxZ + 0.01;
			spt.u = 0;
			spt.r = 0;
			tin.AddPoint( spt.u, spt.v, nIndex, nIndex);
			tin.AddPoint( spt.u + 1.01, spt.v, nIndex, nIndex);
			sptColl.push_back(spt);
			
			spt.v = fMinZ - 0.01;
			spt.u = 0;
			spt.r = 0;
			tin.AddPoint(spt.u, spt.v, nIndex + 1, nIndex + 1);
			tin.AddPoint(spt.u + 1.01, spt.v, nIndex + 1, nIndex + 1);
			sptColl.push_back(spt);					
			//////////////////////////////////////////////////////////////////////////
			
			tin.EndAddPoints();
			tin.FastConstruct();
			tin.EnableIntersection( FALSE );
						
			//////////////////////////////////////////////////////////////////////////        
			//save the vetex points
			int nVertexSize = sptColl.size();
			double *tx = new double[nVertexSize];
			double *ty = new double[nVertexSize];
			double *tz = new double[nVertexSize];			
			for(i=0; i<nVertexSize; i++)
			{
				double u,v,r;
				u = sptColl[i].u * 2 * PI;
				v = sptColl[i].v * PI;
				r = sptColl[i].r;
				
				if(r == 0)
				{
					tx[i] = 0;
					ty[i] = 0;
					
					if( v/PI > fMaxZ )
						tz[i] = fMaxZ;
					else if (v/PI > fMinZ)
						tz[i] = v/PI;
					else
						tz[i] = fMinZ;
					
				} 
				else
				{				
					tx[i] = r*cos(u); //*sin(v);
					ty[i] = r*sin(u); //*sin(v);
					tz[i] = v/PI;
				}
			}
			
			//construct the triangles
			LONG nTriangleNum = 0 ;		
			TRIANGLE **tris = tin.SelectTriangles(&nTriangleNum, 0, 0, 0, 0 );		
			int *nV1 = new int[nTriangleNum];
			int *nV2 = new int[nTriangleNum];
			int *nV3 = new int[nTriangleNum];		
			int nRealNum = 0;		
			for (i=0; i<nTriangleNum; i++)
			{	
				double vx1,vy1;
				double vx2,vy2;
				double vx3,vy3;
				int    index1, index2, index3;
				
				vx1 =  (*tris)->vertex[0]->x;
				vy1 =  (*tris)->vertex[0]->y;
				index1 = int((*tris)->vertex[0]->attr);
				
				vx2 =  (*tris)->vertex[1]->x;
				vy2 =  (*tris)->vertex[1]->y;
				index2 = int((*tris)->vertex[1]->attr);
				
				vx3 =  (*tris)->vertex[2]->x;
				vy3 =  (*tris)->vertex[2]->y;
				index3 = int((*tris)->vertex[2]->attr);
				
				if ( ((vx1 <= 1 && vy1 <= 1 && vx1 >= 0  &&  vy1 >= 0) ||
					(vx2 <= 1 && vy2 <= 1 && vx2 >= 0  &&  vy2 >= 0) ||
					(vx3 <= 1 && vy3 <= 1 && vx3 >= 0  &&  vy3 >= 0) ) 
					&& ( (index1 != index2) && (index2 != index3) && (index1 !=index3) )
					&& ( vx1 >= 0 && vx2 >= 0 && vx3 >= 0 ) )
				{
					nV1[nRealNum] = int((*tris)->vertex[0]->attr);
					nV2[nRealNum] = int((*tris)->vertex[1]->attr);
					nV3[nRealNum] = int((*tris)->vertex[2]->attr);
					
					nRealNum++;
				}			
				*tris++;
			}
			//////////////////////////////////////////////////////////////////////////	
			
			CMesh *pMesh = new CMesh;
			pMesh->SetMesh(nVertexSize, tx , ty , tz, nRealNum, nV1, nV2, nV3);
			
			delete tx;
			delete ty;
			delete tz;
			delete nV1;
			delete nV2;
			delete nV3;

			return pMesh;
		}
		else
			return NULL;
}




CMesh* RotateMesh1(const double *z, const double *x, const int nPtNum, const int nSlices)
{
	if(nPtNum)
	{
		vector<TriangleIndex> triangleColl;
		TriangleIndex triangle;
		
		for(int i=0; i<nPtNum-1; i++)
		{		
			for(int j=0; j<nSlices; j++)
			{
				int a,b,c,d;
				a = i*nSlices + j;
				b = (i+1)*nSlices + j;
				c = (i+1)*nSlices + (j+1)%nSlices;
				d = i*nSlices + (j+1)%nSlices;
			
				if(z[nPtNum-1] > z[0] )
				{					
					triangle.vc = a;
					triangle.vb = b;
					triangle.va = c;
					triangleColl.push_back(triangle);
					
					triangle.vc = a;
					triangle.vb = c;
					triangle.va = d;
					triangleColl.push_back(triangle);
				}
				else
				{
					triangle.vc = c;
					triangle.vb = b;
					triangle.va = a;
					triangleColl.push_back(triangle);
					
					triangle.vc = d;
					triangle.vb = c;
					triangle.va = a;
					triangleColl.push_back(triangle);
				}
			}		
		}
		
		//construct the top and bottom 
		//the bottom
		if(z[0] < z[nPtNum-1])
        {			
			for(i=0; i<nSlices; i++)
			{
				triangle.vc = nPtNum*nSlices;
				triangle.vb = i ;
				triangle.va = (i + 1) % nSlices;
				
				triangleColl.push_back(triangle);
			}
			//the top
			for(i=0; i<nSlices; i++)
			{
				triangle.va = (nPtNum*nSlices) + 1;
				triangle.vb = (nPtNum-1)*nSlices + i;
				triangle.vc = (nPtNum-1)*nSlices + (i + 1)%nSlices;
				triangleColl.push_back(triangle);
			}
		}
		else
		{						
			for(i=0; i<nSlices; i++)
			{
				triangle.va = nPtNum*nSlices;
				triangle.vb = i ;
				triangle.vc = (i + 1) % nSlices;
				
				triangleColl.push_back(triangle);
			}
			//the top
			for(i=0; i<nSlices; i++)
			{
				triangle.vc = (nPtNum*nSlices) + 1;
				triangle.vb = (nPtNum-1)*nSlices + i;
				triangle.va = (nPtNum-1)*nSlices + (i + 1)%nSlices;
				triangleColl.push_back(triangle);
			}
			
		}

		int *va = new int[triangleColl.size()];
		int *vb = new int[triangleColl.size()];
		int *vc = new int[triangleColl.size()];
		for(i=0; i<triangleColl.size(); i++)
		{
			va[i]= triangleColl[i].va;
			vb[i]= triangleColl[i].vb;
			vc[i]= triangleColl[i].vc;
		}
		
		//points	
		double *tx = new double[nPtNum*nSlices + 2];
		double *ty = new double[nPtNum*nSlices + 2];
		double *tz = new double[nPtNum*nSlices + 2];
		float fStep = 360.0/nSlices;
		for(i=0; i<nPtNum; i++)
		{
			for(int j=0; j<nSlices; j++)
			{			
				tx[i*nSlices + j] = x[i] * cos( (fStep*j)/180.0 * PI );
				ty[i*nSlices + j] = x[i] * sin( (fStep*j)/180.0 * PI );
				tz[i*nSlices + j] = z[i];
			}
		}
		 

        
		tx[nPtNum*nSlices] = 0;
		ty[nPtNum*nSlices] = 0;
		tz[nPtNum*nSlices] = z[0];

		tx[nPtNum*nSlices + 1] = 0;
		ty[nPtNum*nSlices + 1] = 0;
		tz[nPtNum*nSlices + 1] = z[nPtNum-1];
		
		CMesh *pMesh = new CMesh;
		pMesh->SetMesh(nPtNum*nSlices + 2, tx , ty , tz, triangleColl.size(), va, vb, vc);
		
		delete tx;
		delete ty;
		delete tz;
		delete va;
		delete vb;
		delete vc;
		
		return pMesh;
	}
	else
		return NULL;
}
/*  Order the vertex
 *	Make sure that the ounter contour is Anti-Clockwise
 *  the inner contour is Clockwise
 */ 
void   OrderVertex(const int nContour, const int *pContNum, double *x, double *y)
{
	
	//{{the outer contour
	int nPtNum = pContNum[0];
	double *tx = new double[nPtNum + 1];
	double *ty = new double[nPtNum + 1];
	for(int j=0; j<nPtNum; j++)
	{
		tx[j] = x[j];
		ty[j] = y[j];
	}
	tx[nPtNum] = tx[0];
	ty[nPtNum] = ty[0];	
    
	if( ComputePolyArea(nPtNum, tx, ty) < 0) //if the outer contour is clockwise
	{
		for(int i=0; i<nPtNum; i++ )
		{
			x[i] = tx[nPtNum - i -1];		
			y[i] = ty[nPtNum - i -1];		
		}
	}
	delete tx;
	delete ty;
	//}}the outer contour

    //{{the inner contour
	for(int i=1; i<nContour; i++)
	{
		int nPtNum = pContNum[i];
		double *tx = new double[nPtNum + 1];
		double *ty = new double[nPtNum + 1];

		int nOff = 0;
		for(int ii=0; ii<i; ii++)
			nOff += pContNum[ii];

		for(int j=0; j<nPtNum; j++);
		{
			tx[j] = x[j + nOff];
			ty[j] = y[j + nOff];
		}
		tx[nPtNum] = tx[0];
		ty[nPtNum] = ty[0];	
		
		if(ComputePolyArea(nPtNum, tx, ty) > 0) //if the inner contour is Anti-clockwise
		{
			for(int i=0; i<nPtNum; i++ )
			{
				x[i + nOff] = tx[nPtNum - i -1];		
				y[i + nOff] = ty[nPtNum - i -1];		
			}
		}
		delete tx;
		delete ty;	
	}
	//}}the inner contour

}


double ComputePolyArea(const int nVertex, const double* x, const double *y)
{
	double dfArea = 0;
	for(int i=0; i<nVertex; i++)
	{
		dfArea += ( x[i]*y[i+1] - x[i+1]*y[i]);
	}
	return dfArea;
}