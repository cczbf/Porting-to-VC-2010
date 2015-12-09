 
//system includes
#include "stdafx.h"
#include "math.h"
#include "float.h"
#include <stdio.h>

//our includes
#include "CMesh.h"         //header file
#include "TriMesh.h"       //a 3d modle lib header
#include "LabelPts.h"
#include "CMyCGlib.h"
#include "CGlobals.h"


#define PI 3.14159265358979323846

CMesh::CMesh()
  : m_nNumOfVertexs(0), m_nNumOfTriangles(0), m_dMeshScale(1.0)
{
    //## begin CMesh::CMesh%.body preserve=yes
	m_nEdgePointsNum = 0;
	m_nType = -1;
	m_CTriangleArray.SetSize(0);
	m_CVertexArray.SetSize(0);
	
	//m_pCRelxationLabelling = NULL;
    /*
	m_MinX = 0;  m_MaxX = 1;
	m_MinY = 0;  m_MaxY = 1;
	m_MinZ = 0;  m_MaxZ = 1;	
	*/

	m_nEdgeNum = 0;
	m_nNumOfVertexs = 0;
	m_nNumOfTriangles = 0;

	//m_pBitAdjacentMatrix = NULL;	

	m_pEdgeNodes = NULL;
	m_pLabelPts = NULL;

	m_nEdgePointsNum = 0;


  //## end CMesh::CMesh%.body
}

CMesh::CMesh(const CMesh &mesh)
{
	 m_nNumOfVertexs  = mesh.m_nNumOfVertexs; 
     m_nNumOfTriangles = mesh.m_nNumOfTriangles ;

	 /*
     m_MinX = mesh.m_MinX;
     m_MinY = mesh.m_MinY;
     m_MinZ = mesh.m_MinZ;
     m_MaxX = mesh.m_MaxX;
     m_MaxY = mesh.m_MaxY ;
     m_MaxZ = mesh.m_MaxZ ;
     m_CenterX = mesh.m_CenterX ;
     m_CenterY = mesh.m_CenterY;
     m_CenterZ = mesh.m_CenterZ ;
	 */

     m_dW = mesh.m_dW;
     m_dMeshScale = mesh.m_dMeshScale;
     
	 int nTriangle = mesh.m_CTriangleArray.GetSize();
	 m_CTriangleArray.SetSize(nTriangle);
	 for(int i=0; i<nTriangle; i++)
	 {
		 m_CTriangleArray[i].pnV[0] = mesh.m_CTriangleArray[i].pnV[0];
		 m_CTriangleArray[i].pnV[1] = mesh.m_CTriangleArray[i].pnV[1];
		 m_CTriangleArray[i].pnV[2] = mesh.m_CTriangleArray[i].pnV[2];
			
		 m_CTriangleArray[i].dC12 = mesh.m_CTriangleArray[i].dC12;
		 m_CTriangleArray[i].dC23 = mesh.m_CTriangleArray[i].dC23;
		 m_CTriangleArray[i].dC31 = mesh.m_CTriangleArray[i].dC31;

		 m_CTriangleArray[i].dColorR = mesh.m_CTriangleArray[i].dColorR ;
		 m_CTriangleArray[i].dColorG = mesh.m_CTriangleArray[i].dColorG ;
		 m_CTriangleArray[i].dColorB = mesh.m_CTriangleArray[i].dColorB ;	
		 		 
	 }
    
	 int nVertex = mesh.m_CVertexArray.GetSize();
	 m_CVertexArray.SetSize(nVertex);
	 for(int i=0; i<nVertex; i++)
	 {
		 m_CVertexArray[i].dX = mesh.m_CVertexArray[i]->dX;
		 m_CVertexArray[i].dY = mesh.m_CVertexArray[i]->dY;
		 m_CVertexArray[i].dZ = mesh.m_CVertexArray[i]->dZ;

		 m_CVertexArray[i].index = mesh.m_CVertexArray[i].index;
	 }

	 int nNormal = mesh.m_NormalArray.GetSize();
     m_NormalArray.SetSize(nNormal);
	 for(i=0; i<nNormal; i++)
	 {
		 m_NormalArray[i].dX = mesh.m_NormalArray[i].dX;
		 m_NormalArray[i].dY = mesh.m_NormalArray[i].dY;
		 m_NormalArray[i].dZ = mesh.m_NormalArray[i].dZ;
	 }

	 m_nEdgePointsNum = mesh.m_nEdgePointsNum;//added by xdh, the number of edge points
	 //m_geometryStretch = mesh.m_geometryStretch;
	
	 m_nType = mesh.m_nType;  // the type of mesh: 0---3D; 1---2D;
	 
	 m_nEdgeNum = mesh.m_nEdgeNum;
	  	  	  
	 //m_strAverageOfStretchRatio = mesh.m_strAverageOfStretchRatio;
	 //m_strCoviarianceOfStretchRatio = mesh.m_strCoviarianceOfStretchRatio ;
	 //m_strGeoRatio = mesh.m_strGeoRatio ;

 	 m_pEdgeNodes = NULL;
	 
     if( &mesh.GetLabel() == NULL )
	 	 m_pLabelPts = NULL;
	 else
		 m_pLabelPts = new CLabelPts( mesh.GetLabel() );
	 
	 //m_pLabelPts = NULL;
	 //???
	 if(m_pEdgeNodes)
	 {
	 	ReleaseEdges();
		m_pEdgeNodes = NULL;
	 }

}

CMesh::~CMesh()
{
	ReleaseEdges();

	if(m_pLabelPts != NULL)
		delete m_pLabelPts;
}

BOOL CMesh::LoadFromRaw (HANDLE hRaw, int w, int h)
{
	
	this->m_nNumOfTriangles = (w-1)*(h-1)*2;
	this->m_nNumOfVertexs   = w*h;
	
	int i,j,k;
	unsigned char *lpRaw = (unsigned char*)GlobalLock(hRaw);
	
    /* get the description of the first element */
	this->m_CVertexArray.SetSize( this->m_nNumOfVertexs);
	
	/* grab all the vertex elements*/
	k = 0;
	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
			
		{
			/* grab and element from the file */
			this->m_CVertexArray[k].dX = (double)i/(double)(w-1);
			this->m_CVertexArray[k].dY = (double)j/(double)(h-1);
			this->m_CVertexArray[k].dZ = (double)lpRaw[k]/255.0;
			k++;
		}
		
		/* if we're on face elements, read them in */
		/* create a list to hold all the face elements */
		this->m_CTriangleArray.SetSize(this->m_nNumOfTriangles);
		
		/* grab all the face elements */
		k = 0;
		int m = 0;
		for ( i = 0; i < w-1; i++ )
			for ( j = 0; j < h-1; j++ ) 
			{
				/* grab and element from the file */
				k = j*w + i;
				this->m_CTriangleArray[m].pnV[0] = k;
				this->m_CTriangleArray[m].pnV[1] = k+1;
				this->m_CTriangleArray[m].pnV[2] = k+1+w;
				m++;
				
				this->m_CTriangleArray[m].pnV[0] = k;
				this->m_CTriangleArray[m].pnV[2] = k+w;
				this->m_CTriangleArray[m].pnV[1] = k+1+w;
				m++;
			}
			
			ASSERT(m==this->m_nNumOfTriangles);
			//calculate the normals:
			GlobalUnlock(hRaw);
			
			return TRUE;

  //## end CMesh::LoadFromRaw%41344610035B.body
}

BOOL CMesh::LoadFromINI (CString strFile)
{
  //## begin CMesh::LoadFromINI%3FCA8D1C001A.body preserve=yes
  return TRUE;
  //## end CMesh::LoadFromINI%3FCA8D1C001A.body
}

BOOL CMesh::Write (CString strFile)
{
  //## begin CMesh::Write2Ply%3FCA8D24016B.body preserve=yes
	
	FILE *pFPly;
	pFPly = fopen( strFile, "w" );
    if (pFPly == NULL )
		return false;

	int  nNumOfVertexs = m_CVertexArray.GetSize();
	int  nNumOfTriangles = m_CTriangleArray.GetSize();

	fprintf(pFPly, "ply\nformat ascii 1.0\ncomment author: Zhou BingFeng\nobj_info random information\n");
    fprintf(pFPly, "element vertex %d\n", nNumOfVertexs);
	fprintf(pFPly, "property float x\nproperty float y\nproperty float z\n");
	fprintf(pFPly, "element face %d\n",   nNumOfTriangles);
	fprintf(pFPly, "property list uchar int vertex_indices\nelement facecolor 0\nend_header\n");

	for( int i = 0; i < nNumOfVertexs; i++)
	{
		fprintf(pFPly, "%f %f %f\n", this->m_CVertexArray[i].dX, this->m_CVertexArray[i].dY, this->m_CVertexArray[i].dZ );
	}
	
	for( i = 0; i < nNumOfTriangles; i++)
	{
		fprintf(pFPly, "3 %d %d %d\n", 
			this->m_CTriangleArray[i].pnV[0],
			this->m_CTriangleArray[i].pnV[1],
			this->m_CTriangleArray[i].pnV[2] );
	}

    for( i = 0; i < nNumOfTriangles; i++)
	{
		fprintf(pFPly, "%d %d %d\n", 
			(int)this->m_CTriangleArray[i].dColorR,
			(int)this->m_CTriangleArray[i].dColorG,
			(int)this->m_CTriangleArray[i].dColorB);
	}

	_fcloseall();
//	fclose( pFPly); 
  return TRUE;
  //## end CMesh::Write2Ply%3FCA8D24016B.body
}

BOOL CMesh::Write2INI (CString strFile)
{
  //## begin CMesh::Write2INI%3FCA8D2B0273.body preserve=yes
return TRUE;
  //## end CMesh::Write2INI%3FCA8D2B0273.body
}

void CMesh::Normalize (float fScaleFactor)
{
  //## begin CMesh::Normalize%3FCDAC5D0145.body preserve=yes
	int i;
	int nSize = m_CVertexArray.GetSize();

	if(m_nNumOfVertexs)
	{
		double dfMinX,dfMinY,dfMinZ;
		dfMinX = FLT_MAX;
		dfMinY = FLT_MAX;
		dfMinZ = FLT_MAX;
		double dfMaxX,dfMaxY,dfMaxZ;
		dfMaxX = FLT_MIN;
		dfMaxY = FLT_MIN;
		dfMaxZ = FLT_MIN;    
		
		double dfCenterX = 0;
		double dfCenterY = 0;
		double dfCenterZ = 0;

		/*
		//{{ added by zbf 2004.11.11
		this->m_MinX = 
		this->m_MinY = 
		this->m_MinZ = FLT_MAX;
			
		this->m_MaxX = 
		this->m_MaxY = 
		this->m_MaxZ = -FLT_MIN;
		//}} added by zbf 2004.11.11
		*/

		for ( i = 0; i < this->m_nNumOfVertexs; i++)
		{
			dfMinX = min( dfMinX, this->m_CVertexArray[i].dX);
			dfMinY = min( dfMinY, this->m_CVertexArray[i].dY);
			dfMinZ = min( dfMinZ, this->m_CVertexArray[i].dZ);
			
			dfMaxX = max( dfMaxX, this->m_CVertexArray[i].dX);
			dfMaxY = max( dfMaxY, this->m_CVertexArray[i].dY);
			dfMaxZ = max( dfMaxZ, this->m_CVertexArray[i].dZ);
		}

		dfCenterX	= (dfMinX + dfMaxX) / 2.0;
		dfCenterY	= (dfMinY + dfMaxY) / 2.0;
		dfCenterZ	= (dfMinZ + dfMaxZ) / 2.0;

		for ( i = 0; i < this->m_nNumOfVertexs; i++)
		{
			this->m_CVertexArray[i].dX -= dfCenterX;
			this->m_CVertexArray[i].dY -= dfCenterY;
			this->m_CVertexArray[i].dZ -= dfCenterZ;
		}
		
		dfMinX -= dfCenterX;
		dfMinY -= dfCenterY;
		dfMinZ -= dfCenterZ;
		
		dfMaxX -= dfCenterX;
		dfMaxY -= dfCenterY;
		dfMaxZ -= dfCenterZ;
		
		double dScaleX = max(-dfMinX, dfMaxX);
		double dScaleY = max(-dfMinY, dfMaxY);
		double dScaleZ = max(-dfMinZ, dfMaxZ);
		
		double dScale = max(dScaleX,max(dScaleY, dScaleZ));

		dScale /= fScaleFactor;
		if ( dScale != 0.0)
			for ( i = 0; i < this->m_nNumOfVertexs; i++)
			{
				//		this->m_CVertexArray[i].dX = (1.0-0.618)*this->m_CVertexArray[i].dX / dScale;
				//		this->m_CVertexArray[i].dY = (1.0-0.618)*this->m_CVertexArray[i].dY / dScale;
				//		this->m_CVertexArray[i].dZ = (1.0-0.618)*this->m_CVertexArray[i].dZ / dScale;
				
				this->m_CVertexArray[i].dX = this->m_CVertexArray[i].dX / dScale;
				this->m_CVertexArray[i].dY = this->m_CVertexArray[i].dY / dScale;
				this->m_CVertexArray[i].dZ = this->m_CVertexArray[i].dZ / dScale;				
			}
			
		/*
		for ( i = 0; i < this->m_nNumOfVertexs; i++)
		{
			this->m_MinX = min( this->m_MinX, this->m_CVertexArray[i].dX);
			this->m_MinY = min( this->m_MinY, this->m_CVertexArray[i].dY);
			this->m_MinZ = min( this->m_MinZ, this->m_CVertexArray[i].dZ);
			
			this->m_MaxX = max( this->m_MaxX, this->m_CVertexArray[i].dX);
			this->m_MaxY = max( this->m_MaxY, this->m_CVertexArray[i].dY);
			this->m_MaxZ = max( this->m_MaxZ, this->m_CVertexArray[i].dZ);
		}
			//reset 
		
			m_MinX = 0;  m_MaxX = 1;
			m_MinY = 0;  m_MaxY = 1;
			m_MinZ = 0;  m_MaxZ = 1;	
			//Removed by zbf 2004.11.11
			*/
	}
  //## end CMesh::Normalize%3FCDAC5D0145.body
} 
 
void CMesh::SetMesh (int nTriangle, int* nV1, int* nV2, int* nV3, DWORD* c12, DWORD* c23, DWORD* c31,
					 double* ratio12, double* ratio23, double *ratio31, 
					 int nVlength, double* x, double* y, double* z, 
					 int nEdgeNodeNum, 
					 CString strAverage, 
					 CString strConviance, 
					 CString strGeoRatio)
{
  //## begin CMesh::SetMesh%4181ECA4023A.body preserve=yes
  
	//before set a new mesh, we must release the memory for edges
	ReleaseEdges();

  //if the data is changed, refresh the point data
	if ( x != NULL )   
	{
		m_CVertexArray.SetSize(nVlength );// + 1);
		for (int i=0; i<nVlength; i++ )
		{
     		m_CVertexArray[i].dX = x[i];
			m_CVertexArray[i].dY = y[i];
			m_CVertexArray[i].dZ = z[i]; 
		}
        
		//to make the MIN != MAX
		//m_CVertexArray[nVlength].dX = 0;
		//m_CVertexArray[nVlength].dY = 0;
	    //m_CVertexArray[nVlength].dZ = 1; 		
	}
	
	if(nVlength)
	{		
		m_nNumOfVertexs = nVlength;// + 1; //add a point to make the 2D mesh Zmin != Zmax
		
		//refresh the triangle 
		m_CTriangleArray.SetSize(nTriangle);
		for (int i=0; i<nTriangle; i++)
		{
			m_CTriangleArray[i].pnV[0] = nV1[i];
			m_CTriangleArray[i].pnV[1] = nV2[i];
			m_CTriangleArray[i].pnV[2] = nV3[i];
		    
			if( c12!=NULL && c23!=NULL && c31!=NULL )
			{
				m_CTriangleArray[i].dC12 = c12[i]; 
			    m_CTriangleArray[i].dC23 = c23[i];
			    m_CTriangleArray[i].dC31 = c31[i];
			}

			if( ratio12!=NULL && ratio23!=NULL && ratio31!=NULL )
			{
				m_CTriangleArray[i].dfRatio12 = ratio12[i]; 
			    m_CTriangleArray[i].dfRatio23 = ratio23[i];
			    m_CTriangleArray[i].dfRatio31 = ratio31[i];			
			}
		}
		
		m_nNumOfTriangles = nTriangle;
		
		Normalize(0.5);		
	}

	/*
	if (strAverage != "")
	{
		m_strAverageOfStretchRatio = strAverage;
		m_strCoviarianceOfStretchRatio = strConviance;		
		m_strGeoRatio = strGeoRatio;
	}*/
	
	
	if (nEdgeNodeNum > 0)
		m_nEdgePointsNum = nEdgeNodeNum;
  
	//## end CMesh::SetMesh%4181ECA4023A.body
}

void CMesh::GetMesh (int* nTriangle, int** nV1, int** nV2, int** nV3, int** c12, int** c23, int** c31, int* nVlength, double** x, double** y, double** z, int* nEdgeNodeNum)
{
  //## begin CMesh::GetMesh%41882FD101F8.body preserve=yes
  if(nTriangle != NULL)
  {
		*nTriangle = m_CTriangleArray.GetSize();
		
		*nV1 = new int[*nTriangle];
		*nV2 = new int[*nTriangle];
		*nV3 = new int[*nTriangle];
		
		for (int i=0; i<*nTriangle; i++)
		{
			(*nV1)[i] = m_CTriangleArray[i].pnV[0];
			(*nV2)[i] = m_CTriangleArray[i].pnV[1];
			(*nV3)[i] = m_CTriangleArray[i].pnV[2];
		}
	}	

  if(nEdgeNodeNum != NULL)
	  *nEdgeNodeNum = GetBorderLen();

  //## end CMesh::GetMesh%41882FD101F8.body
}

//DEL bool CMesh::ComputeBitAdjacentMatrix ()
//DEL {
//DEL   //## begin CMesh::ComputeBitAdjacentMatrix%418EC7740391.body preserve=yes
//DEL 
//DEL //void  CMesh::ComputeBitAdjacentMatrix()
//DEL {
//DEL 	if (m_pBitAdjacentMatrix != NULL )
//DEL 		delete m_pBitAdjacentMatrix;
//DEL 
//DEL 	m_pBitAdjacentMatrix = new BitAdjacentMatrix;
//DEL 
//DEL 	int  nPointNum = m_CVertexArray.GetSize();
//DEL 	if ( !m_pBitAdjacentMatrix->Init(nPointNum, nPointNum) ) 
//DEL 	{
//DEL 		delete m_pBitAdjacentMatrix;
//DEL 		m_pBitAdjacentMatrix = NULL;
//DEL 		return false;
//DEL 	}
//DEL 
//DEL 	int nTriangleNum = m_CTriangleArray.GetSize();
//DEL 
//DEL 	int va = 0;
//DEL 	int vb = 0;
//DEL 	int vc = 0;
//DEL 
//DEL 	for(int i=0; i<nTriangleNum; i++)
//DEL 	{
//DEL 		va = m_CTriangleArray[i].pnV[0];
//DEL 		vb = m_CTriangleArray[i].pnV[1];
//DEL 		vc = m_CTriangleArray[i].pnV[2];	
//DEL 				
//DEL 		m_pBitAdjacentMatrix->SetPos(va, vb);
//DEL 		m_pBitAdjacentMatrix->SetPos(vb, va);
//DEL 		m_pBitAdjacentMatrix->SetPos(va, vc);
//DEL 		m_pBitAdjacentMatrix->SetPos(vc, va);
//DEL 		m_pBitAdjacentMatrix->SetPos(vb, vc);
//DEL 		m_pBitAdjacentMatrix->SetPos(vc, vb);		
//DEL 	}	
//DEL 	
//DEL 	return true;	
//DEL }
//DEL 
//DEL 
//DEL   //## end CMesh::ComputeBitAdjacentMatrix%418EC7740391.body
//DEL }

int CMesh::GetEdgeNum () 
{
//## begin CMesh::GetEdgeNum%418EC7980054.body preserve=yes
//	int  CMesh::GetEdgeNum()
{
	if( m_pEdgeNodes != NULL )
		return m_nEdgeNum;
	else
	{
		ExtractEdges();
		return m_nEdgeNum;
	}
}
 //## end CMesh::GetEdgeNum%418EC7980054.body
}

void  CMesh::GetEdgeIndices(int &i, int &j, int index) const
{
	if(m_pEdgeNodes != NULL)
	{
		int nEdgeNodes = m_CVertexArray.GetSize();

		ASSERT(index < m_nEdgeNum );

        int nSum = 0;
		int nSumLast = 0;

		for(int k=0; k<nEdgeNodes; k++)
		{
			nSumLast = nSum;
			nSum += m_pEdgeNodes[k].index;
			
			if(index < nSum)
			{
				int indexOfNode = index - nSumLast;
				EdgeNode *pHead = m_pEdgeNodes[k].pNext;
				while(pHead != NULL)
				{
					if( indexOfNode == 0 )
					{
                        i = k;
						j = pHead->index;
						break;
					}
					indexOfNode--;
				}
				break;
			}
		}
	}
}

int CMesh::GetGenus () const
{
  //## begin CMesh::GetGenus%418EC7BA0161.body preserve=yes
//	int  CMesh::GetGenus()
{
	int fF = m_CTriangleArray.GetSize();
	int fV = m_CVertexArray.GetSize();
	int fE = m_nEdgeNum;

	int g = (2 - ( fF - fE + fV ))/2;
	
	return g;
}
  //## end CMesh::GetGenus%418EC7BA0161.body
}

/*
//double Distance (double x0, double y0, double z0, double x1, double y1, double z1);
//double Distance (int i, int j);
double CMesh::Distance (int i, int j)
{
  //## begin CMesh::Distance%3FCF2B92029D.body preserve=yes
	double d =	(this->m_CVertexArray[i].dX - this->m_CVertexArray[j].dX)*(this->m_CVertexArray[i].dX - this->m_CVertexArray[j].dX) + 
				(this->m_CVertexArray[i].dY - this->m_CVertexArray[j].dY)*(this->m_CVertexArray[i].dY - this->m_CVertexArray[j].dY) +
				(this->m_CVertexArray[i].dZ - this->m_CVertexArray[j].dZ)*(this->m_CVertexArray[i].dZ - this->m_CVertexArray[j].dZ);
	d = sqrt (d);
	return (d);
  //## end CMesh::Distance%3FCF2B92029D.body
}
double CMesh::Distance (double x0, double y0, double z0, double x1, double y1, double z1)
{
  //## begin CMesh::Distance%3FCF4C6E00B8.body preserve=yes
	double d =	(x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) + (z0 - z1)*(z0 - z1);
	d = sqrt (d);
	return d;
  //## end CMesh::Distance%3FCF4C6E00B8.body
}
*/

//DEL void CMesh::Load2DMesh (CString fileName)
//DEL {
//DEL //## begin CMesh::Load2DMesh%417C5C4D02DD.body preserve=yes
//DEL //	void CMesh::Load2DMesh(CString fileName)
//DEL 
//DEL 	/*{
//DEL 	FILE *fp = NULL;
//DEL     fp = fopen(fileName, "r");
//DEL 	
//DEL 	if(fp != NULL)
//DEL 	{		
//DEL 		int n3DPtsNum = 0;
//DEL 		int n2DInnerPtsNum = 0;
//DEL 		int nTriangleNum = 0;
//DEL 		
//DEL 		float gX = 0;
//DEL 		float gY = 0;
//DEL 		float gZ = 0;
//DEL 		float x = 0;
//DEL 		float y = 0;
//DEL 		int   index = 0;
//DEL 		int va = 0;
//DEL 		int vb = 0;
//DEL 		int vc = 0;		
//DEL 
//DEL 		//ASSERT(m_pCRelxationLabelling != NULL);
//DEL 
//DEL 		//read the 3D points
//DEL 		fscanf(fp, "%d", &n3DPtsNum);		
//DEL 		for (int i=0; i<n3DPtsNum; i++)
//DEL 		{
//DEL 			fscanf(fp,"%f %f %f", &gX, &gY, &gZ);			
//DEL 			CNode3D p;
//DEL 			p.x = gX; p.y = gY; p.z = gZ;
//DEL 		}
//DEL 
//DEL 		int nEdgePtsNum = 0;
//DEL 		//read the number of edge points, (Actually, it's number of border nodes.-zbf 041029)
//DEL 		fscanf(fp, "%d", &nEdgePtsNum);
//DEL 		m_nEdgePointsNum = nEdgePtsNum;
//DEL 		//read the 2D mesh points (2D internal nodes -zbf 041029)
//DEL 		fscanf(fp, "%d", &n2DInnerPtsNum);
//DEL         
//DEL 		int n2DPtsNum = nEdgePtsNum + n2DInnerPtsNum;
//DEL 
//DEL 		m_CVertexArray.SetSize(n2DPtsNum + 1);
//DEL 		m_nNumOfVertexs = n2DPtsNum + 1;  //the number of triangle vertex		
//DEL 				
//DEL 		for ( i=0; i<n2DPtsNum; i++)
//DEL 		{
//DEL 			fscanf(fp, "%f %f %d", &x, &y, &index);
//DEL 			
//DEL 			m_CVertexArray[i].dX = x;
//DEL 			m_CVertexArray[i].dY = y;
//DEL 			m_CVertexArray[i].dZ = 0; //default set as 0
//DEL 			m_CVertexArray[i].index = index;
//DEL 
//DEL 			CNode2D p;
//DEL 			p.u = x;  p.v = y;  p.m_n3DIndex = index;
//DEL 		}
//DEL 	 
//DEL 		m_CVertexArray[n2DPtsNum].dX = 0;
//DEL 		m_CVertexArray[n2DPtsNum].dY = 0;
//DEL     	m_CVertexArray[n2DPtsNum].dZ = 1.0; //default set as 0
//DEL 				 		
//DEL 		//read the triangle index	
//DEL 		fscanf(fp, "%d", &nTriangleNum);
//DEL 		m_CTriangleArray.SetSize(nTriangleNum);
//DEL         m_nNumOfTriangles = nTriangleNum;
//DEL 		
//DEL 		//m_pCRelxationLabelling->SetTriangleNum(nTriangleNum);
//DEL 		
//DEL 		for (i=0; i<nTriangleNum; i++)
//DEL 		{
//DEL 			fscanf(fp, "%d %d %d", &va, &vb, &vc);
//DEL 
//DEL 			m_CTriangleArray[i].pnV[0] = va;
//DEL 			m_CTriangleArray[i].pnV[1] = vb;
//DEL 			m_CTriangleArray[i].pnV[2] = vc;	
//DEL 		}		
//DEL 		fclose(fp);
//DEL 		
//DEL 		//compute the bit adjacent matrix , added by XDH, 2004.11.07
//DEL 		ComputeBitAdjacentMatrix();
//DEL 		ComputeEdgeNum();
//DEL 
//DEL 		//
//DEL 		//ComputeTriangleArea();
//DEL 		//ComputeColor();
//DEL 	}	
//DEL }
//DEL */
//DEL   //## end CMesh::Load2DMesh%417C5C4D02DD.body
//DEL }

void CMesh::SetMesh (int nVlength, 
					 double* x, double* y, double* z,
					 int nTriangle,
					 int* nV1, int* nV2, int* nV3)
{
  //## begin CMesh::SetMesh%41971B340276.body preserve=yes
	int j;
    this->m_nNumOfTriangles = 0;
    this->m_nNumOfVertexs   = 0;
    
  this->m_CVertexArray.SetSize(nVlength);
  this->m_nNumOfVertexs = nVlength;
       
  for (j = 0; j < nVlength; j++)
	  {
  		this->m_CVertexArray[j].dX = x[j];
		this->m_CVertexArray[j].dY = y[j];
		this->m_CVertexArray[j].dZ = z[j];
      }

  if (nTriangle != 0)
	{
   	  this->m_CTriangleArray.SetSize(nTriangle);
 	  this->m_nNumOfTriangles = nTriangle;
      for (j = 0; j < nTriangle; j++) 
	  {
  			this->m_CTriangleArray[j].pnV[0] = nV1[j];
 			this->m_CTriangleArray[j].pnV[1] = nV2[j];
 			this->m_CTriangleArray[j].pnV[2] = nV3[j];
	  }
      //this->ComputeBitAdjacentMatrix();  // Added again by XDH, 2004.11.07, for ply info.
	  //ComputeEdgeNum();
    }
  else
	{//input is point cloud
		nTriangle = nVlength;
		this->m_CTriangleArray.SetSize(nTriangle);
 		this->m_nNumOfTriangles = nTriangle;
		for (j = 0; j < nTriangle; j++) 
		{
  			this->m_CTriangleArray[j].pnV[0] = j;
 			this->m_CTriangleArray[j].pnV[1] = j;
 			this->m_CTriangleArray[j].pnV[2] = j;
		}
	} 
  //## end CMesh::SetMesh%41971B340276.body
}

/*
CString CMesh::GetStrAverageRatio()
{
	return m_strAverageOfStretchRatio;
}
CString CMesh::GetStrConvarianceRatio()
{
	return m_strCoviarianceOfStretchRatio;
}
CString CMesh::GetStrGeoRation()
{
	return m_strGeoRatio;
}
*/

/*
float CMesh::GetGeometryStretch()
{
	return (float)m_geometryStretch;
}*/

//DEL int CMesh::GetBorderNodeNum()
//DEL {
//DEL 	return m_nEdgePointsNum;
//DEL }

//DEL void CMesh::ComputeEdgeNum()
//DEL {
//DEL 
//DEL 	ExtractEdges();
//DEL     
//DEL 	/*
//DEL 	if( m_pBitAdjacentMatrix->IsInit())
//DEL 	{
//DEL 		int nEdgeNum = 0;
//DEL 
//DEL 		int row = m_pBitAdjacentMatrix->get_m_nHei();
//DEL 		int col = m_pBitAdjacentMatrix->get_m_nWid();
//DEL 		
//DEL 		for (int j=0; j<row; j++ )
//DEL 			for (int i=0; i<j; i++)
//DEL 			{
//DEL 				
//DEL 				if (m_pBitAdjacentMatrix->IsPosAdjacent(i,j))
//DEL 				{
//DEL 					nEdgeNum++;
//DEL 				}
//DEL 			}
//DEL 			m_nEdgeNum = nEdgeNum;		
//DEL 			delete m_pBitAdjacentMatrix;
//DEL 			m_pBitAdjacentMatrix = NULL;
//DEL 	}
//DEL 	*/	
//DEL }

double  CMesh::ComputeScale(double inits)
{	
	int nTriangleSum = m_CTriangleArray.GetSize();

	double sumOfArea = 0;

	for (int i=0; i<nTriangleSum; i++)
	{
		 double v1[3];
		 double v2[3];
		 double v3[3];

		 int index1 = 0;
		 int index2 = 0;
		 int index3 = 0;
		 
		 //retrieve the index of each vertex
		 index1 = m_CTriangleArray[i].pnV[0];
		 index2 = m_CTriangleArray[i].pnV[1];
		 index3 = m_CTriangleArray[i].pnV[2];
		 
		 //retrieve the coordinate of each vertex
		 int i1,i2,i3;
		 //CNode3D p;
		 float px,py,pz;

		 i1 = m_CVertexArray[index1].index;
		 {
			 px = m_CVertexArray[index1].dX;
			 py = m_CVertexArray[index1].dY;
			 pz = m_CVertexArray[index1].dZ;
		 }

		 v1[0] = px;
		 v1[1] = py;
		 v1[2] = pz;

		 i2 = m_CVertexArray[index2].index;
		 {
			 px = m_CVertexArray[index2].dX;
			 py = m_CVertexArray[index2].dY;
			 pz = m_CVertexArray[index2].dZ;
		 }

 		 v2[0] = px;
		 v2[1] = py;
		 v2[2] = pz;

		 i3 = m_CVertexArray[index3].index;
		 {
			 px = m_CVertexArray[index3].dX;
			 py = m_CVertexArray[index3].dY;
			 pz = m_CVertexArray[index3].dZ;
		 }
 		 v3[0] = px;
		 v3[1] = py;
		 v3[2] = pz;

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
		 double s = m/2;

		 sumOfArea += s;
	}

	//scale each vertex
	double lamda = sqrt( sumOfArea / inits);
	return 	lamda;
}

bool CMesh::Load(CString file)
{
	TriMesh *mesh = NULL;
	mesh = TriMesh::read(file);
 
	if(mesh != NULL)
	{
		mesh->need_faces();
		mesh->need_normals();
		
		int nFace = mesh->faces.size();
		int nVertex = mesh->vertices.size(); 
		int nNormal = mesh->normals.size();
        int multiple = 1;
	
		if(g_CGlobals.m_pDlgTab00->m_bMemoryBoostered==1&&nVertex>32767)
		{
			multiple=nVertex/32767; 

            //initialize the Vertexs and Faces information
		    int nFace0 = nFace/multiple+1;
		    int nVertex0 = nFace0*3;
		    int nNormal0 = nVertex0;
		    int i,j;

	    	m_CTriangleArray.SetSize(nFace0);
		    m_nNumOfTriangles = nFace0;
		
	     	m_CVertexArray.SetSize(nVertex0);
	    	m_nNumOfVertexs = nVertex0;
				
		    m_NormalArray.SetSize(nNormal0);

	    	for(i=j=0; i<nFace; i+=multiple,j++)
			{
				int a, b, c;
				float x,y,z;
				float nx,ny,nz;
			
				mesh->GetFaceIndex(a, b, c, i);   //三角形的3个顶点

				mesh->GetVertex(x, y, z, a);      //第一个顶点坐标
				mesh->GetNormal(nx, ny, nz, a);   //第一个顶点法线坐标
		    	this->m_CVertexArray[3*j].dX = x;
				this->m_CVertexArray[3*j].dY = y;
				this->m_CVertexArray[3*j].dZ = z;
				m_NormalArray[3*j].dX = nx;
				m_NormalArray[3*j].dY = ny;
				m_NormalArray[3*j].dZ = nz;

				mesh->GetVertex(x, y, z, b);      //第二个顶点坐标
				mesh->GetNormal(nx, ny, nz, b);   //第二个顶点法线坐标
				this->m_CVertexArray[3*j+1].dX = x;
				this->m_CVertexArray[3*j+1].dY = y;
				this->m_CVertexArray[3*j+1].dZ = z;
				m_NormalArray[3*j+1].dX = nx;
				m_NormalArray[3*j+1].dY = ny;
				m_NormalArray[3*j+1].dZ = nz;

				mesh->GetVertex(x, y, z, c);      //第三个顶点坐标
				mesh->GetNormal(nx, ny, nz, c);   //第三个顶点法线坐标
				this->m_CVertexArray[3*j+2].dX = x;
				this->m_CVertexArray[3*j+2].dY = y;
				this->m_CVertexArray[3*j+2].dZ = z;
				m_NormalArray[3*j+2].dX = nx;
				m_NormalArray[3*j+2].dY = ny;
				m_NormalArray[3*j+2].dZ = nz;

				m_CTriangleArray[j].pnV[0] = 3*j;
				m_CTriangleArray[j].pnV[1] = 3*j+1;
				m_CTriangleArray[j].pnV[2] = 3*j+2;
			}

		}
		else
		{
			//initialize the Vertexs and Faces information
			m_CVertexArray.SetSize(nVertex);
			m_nNumOfVertexs = nVertex;
		
			for(int i=0; i<nVertex; i++)
			{
				float x,y,z;
				mesh->GetVertex(x, y, z, i);
			
				this->m_CVertexArray[i].dX = x;
				this->m_CVertexArray[i].dY = y;
				this->m_CVertexArray[i].dZ = z;
			}
		
			m_CTriangleArray.SetSize(nFace);
			m_nNumOfTriangles = nFace;
		
			for( i=0; i<nFace; i++)
			{
				int a, b, c;
				mesh->GetFaceIndex(a, b, c, i);
			
				m_CTriangleArray[i].pnV[0] = a;
				m_CTriangleArray[i].pnV[1] = b;
				m_CTriangleArray[i].pnV[2] = c;
			}
		
			m_NormalArray.SetSize(nNormal);
			for(i=0; i<nNormal; i++)
			{
				float nx,ny,nz;
				mesh->GetNormal(nx, ny, nz, i);
			
				m_NormalArray[i].dX = nx;
				m_NormalArray[i].dY = ny;
				m_NormalArray[i].dZ = nz;
			}
        
        }

		if(mesh != NULL)
			delete mesh;
		
		//scale the mesh,let the area be same as the area of sphere(radius = 1)
		Normalize();
		double scale = ComputeScale(4*PI);   
		Normalize(float(1.0)/(float)scale);
		
		if(nVertex/multiple> 5000 ) 
			return FALSE;

		return TRUE;
	}
	else
	{
		AfxMessageBox("File is invalid!");
		return FALSE;    
	}
}

int     CMesh::GetVertexNum() const
{
	    return m_CVertexArray.GetSize();
}
void    CMesh::GetVertex(float &x, float &y, float &z, int index) const
{
	    ASSERT(index < m_CVertexArray.GetSize() );

	    x = m_CVertexArray[index].dX;
		y = m_CVertexArray[index].dY;
		z = m_CVertexArray[index].dZ;
}

int     CMesh::GetTriangleNum() const
{
	    return m_CTriangleArray.GetSize();
}

void    CMesh::GetTriangleIndices(int &i, int &j, int &k, int index) const
{
	    ASSERT(index < m_CTriangleArray.GetSize() );

		i = m_CTriangleArray[index].pnV[0];
		j = m_CTriangleArray[index].pnV[1];
		k = m_CTriangleArray[index].pnV[2];
}

void    CMesh::GetVertexNormal(float &nx, float &ny, float &nz, int index) const
{
	
	ASSERT(index < m_CVertexArray.GetSize() );

	nx = m_NormalArray[index].dX;
	ny = m_NormalArray[index].dY;
	nz = m_NormalArray[index].dZ;
}

int     CMesh::GetBorderLen() 
{
	return m_nEdgePointsNum;
}

void CMesh::ExtractEdges()
{
	
	int nTriangleNum = m_CTriangleArray.GetSize();
	int nVertexNum = m_CVertexArray.GetSize();
	
	if(nTriangleNum<0 || nVertexNum<0)
		return;
	
	if(m_pEdgeNodes != NULL)
	{
		m_pEdgeNodes = NULL;
		delete m_pEdgeNodes;
	}

	m_pEdgeNodes = new EdgeNode[nVertexNum];
    for(int i=0; i<nVertexNum; i++)
	{
		m_pEdgeNodes[i].index = i;
		m_pEdgeNodes[i].pNext = NULL;
	}

	for(i=0; i<nTriangleNum; i++)
	{
		int a,b,c;
		a = m_CTriangleArray[i].pnV[0];
		b = m_CTriangleArray[i].pnV[1];
		c = m_CTriangleArray[i].pnV[2];

		int index;
		EdgeNode e;
		e.index = 0;
		e.pNext = NULL;

		if(a<b)
		{
			index = a;
			e.index = b;
		}
		else
		{
			index = b;
			e.index =a;
		}

        InsertNode( &m_pEdgeNodes[index], e );

		if(a<c)
		{
			index = a;
			e.index = c;
		}
		else
		{
			index = c;
			e.index =a;
		}
        InsertNode( &m_pEdgeNodes[index], e );

		if(b<c)
		{
			index = b;
			e.index = c;
		}
		else
		{
			index = c;
			e.index = b;
		}

        InsertNode( &m_pEdgeNodes[index], e );
	}

	//summarize the number of edge of each node
     int nEdgeSum = 0;
	 for( i=0; i<nVertexNum; i++)
	 {
		 EdgeNode *pHead = m_pEdgeNodes[i].pNext;
		 int nSum = 0;
	     while(pHead != NULL)
		 {
			 nSum ++;
			 pHead = pHead->pNext;
		 }
         m_pEdgeNodes[i].index = nSum;
		 nEdgeSum += nSum; 
	 }	
	 m_nEdgeNum = nEdgeSum;	 
}

void   CMesh::ReleaseEdges()
{
	if( m_pEdgeNodes!=NULL )
	{
    	int nVertexNum = m_CVertexArray.GetSize();

		for(int i=0; i<nVertexNum; i++)
		{
			int nEdgeOfEachNode = m_pEdgeNodes[i].index;
			EdgeNode **nodes = NULL;
			nodes = new EdgeNode*[nEdgeOfEachNode];

			EdgeNode *pHead = m_pEdgeNodes[i].pNext;
			int index = 0;
			while (pHead != NULL)
			{
				nodes[index] = pHead;
				index ++;

				pHead = pHead->pNext;
			}
			
			for(int j=0; j<index; j++)
				delete nodes[j];
			delete nodes;
		}

		m_pEdgeNodes = NULL;
    }
}

void InsertNode(EdgeNode *pHead, EdgeNode node)
{	
	bool bIsHave = FALSE;
	EdgeNode *pLast;
	while (pHead!=NULL)
	{
		if( pHead->index == node.index )
			bIsHave = TRUE;
		pLast = pHead;
		pHead = pHead->pNext;
	}
	
	if(!bIsHave)
	{
		EdgeNode *pNew = new EdgeNode;
		pNew->index = node.index;
		pNew->pNext = NULL;
		pLast->pNext = pNew;
	}
}

void CMesh::SetLabelPts(const CLabelPts &labelPts)
{	
	if(m_pLabelPts != NULL)
	{
		delete m_pLabelPts;
		m_pLabelPts = NULL;
	}
	m_pLabelPts = new CLabelPts(labelPts);	
}

const CLabelPts& CMesh::GetLabel() const
{
	return *m_pLabelPts;
}

/*
int  CMesh::Get3DPointNum()
{
	return 0;
}

void   CMesh::Get3DPoint(float &x, float &y, float &z, int index)
{
   x = 0;
   y = 0;
   z = 0;
}
*/
//## end module%40F3459A01F0.epilog


double CMesh::GetLenOfShortestEdge()
{
	//{{find the shortest edge:
	double dfMinEdge = FLT_MAX;
	for(int i=0; i<GetEdgeNum(); i++)
	{			
		int v1,v2;
		GetEdgeIndices(v1, v2, i);
		
		float p1[3], p2[3];
		GetVertex(p1[0], p1[1], p1[2], v1);
		GetVertex(p2[0], p2[1], p2[2], v2);
		double dv[3];
		for(int ii=0; ii<3; ii++)
		{
			dv[ii] = p1[ii] - p2[ii];				
		}
		
		CMyCGLib cglib;			
		double dfLen = sqrt( cglib.pp(dv, dv, 3) );
		
		if(dfMinEdge > dfLen)
			dfMinEdge = dfLen;
	}
	return dfMinEdge;
	//}}find the shortest edge:
}

double  CMesh::GetLenOfLongestEdge()
{
	double dfMaxEdge = FLT_MIN;
	for(int i=0; i<GetEdgeNum(); i++)
	{			
		int v1,v2;
		GetEdgeIndices(v1, v2, i);
		
		float p1[3], p2[3];
		GetVertex(p1[0], p1[1], p1[2], v1);
		GetVertex(p2[0], p2[1], p2[2], v2);
		double dv[3];
		for(int ii=0; ii<3; ii++)
		{
			dv[ii] = p1[ii] - p2[ii];				
		}
		
		CMyCGLib cglib;			
		double dfLen = sqrt( cglib.pp(dv, dv, 3) );
		
		if(dfMaxEdge < dfLen)
			dfMaxEdge = dfLen;
	}
	return dfMaxEdge;
	//}}find the shortest edge:
}
