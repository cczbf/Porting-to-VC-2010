

#include "stdafx.h"

#include "math.h"

#include "2DMesh.h"
#include "CVertex.h"
#include "CTriangle.h"
#include "GraphicsAlgorithms.h"
#include "CGlobals.h"      //using some global functions

#include "MeshForGeoImg.h"

//////////////////////////////////////////////////////////////////////////
//C2DMesh Implementation:

#define STATPARANUM  5


C2DMesh::C2DMesh()
{
	m_p3Dx = NULL;
	m_p3Dy = NULL;
	m_p3Dz = NULL;
	m_lfStat = NULL;
	
	m_n3DPtNum = 0;
	m_nBorderLen = 0;
	m_pPts = NULL;
	m_pFaces = NULL;

	m_pAddPts = NULL;
	m_pAddFaces = NULL;

	m_nFaceSum = 0;
	m_nPtSum = 0;
	m_nAddFaces = 0;
	m_nAddPnt = 0;

	m_lfScale = 1;

	m_nAddPnt = 0;
	m_nAddFaces = 0;

	m_pAddPts= new  POINT3D[NUMALLOC];
	m_pAddFaces = new stFACE[NUMALLOC];
    
	memset(m_pAddPts, 0 ,sizeof(POINT3D)*NUMALLOC);
	memset(m_pAddFaces, 0 ,sizeof(stFACE)*NUMALLOC);

	m_lfStat= new  double[STATPARANUM];

	for(int i=0; i < STATPARANUM; i++)
		m_lfStat[i]=-1;	
}

C2DMesh::C2DMesh(const C2DMesh &mesh)
         :CMesh(mesh)
{
	ASSERT(&mesh != NULL);

	if( &mesh == NULL )
		return;

	if(this == &mesh)
		return;

	//invoke the base copy constructor first
	//this->CMesh::CMesh(mesh);
	//CMesh::CMesh(mesh);

	this->m_vCutPath = mesh.m_vCutPath;	
	this->m_vInMapPnt = mesh.m_vInMapPnt;
	this->m_vBndPnt = mesh.m_vBndPnt;
	this->m_vNewFaces = mesh.m_vNewFaces;
	this->m_vRemeshFaces = mesh.m_vRemeshFaces;

	this->m_vGeoMap = mesh.m_vGeoMap;
	this->m_vBndMapPnt = mesh.m_vBndMapPnt;

	this->m_n3DPtNum = mesh.m_n3DPtNum; 
	this->m_nBorderLen = mesh.m_nBorderLen;

	this->m_p3Dx = new float[m_n3DPtNum];
	this->m_p3Dy = new float[m_n3DPtNum];
	this->m_p3Dz = new float[m_n3DPtNum];
 
	if( (mesh.m_p3Dx != NULL) && (mesh.m_p3Dy != NULL)  && (mesh.m_p3Dy != NULL ) )
	for(int i=0; i<this->m_n3DPtNum; i++)
	{
		this->m_p3Dx[i] = mesh.m_p3Dx[i];
		this->m_p3Dy[i] = mesh.m_p3Dy[i];
		this->m_p3Dz[i] = mesh.m_p3Dz[i];
	}

	this->m_nGeoimgMeshDispType =mesh.m_nGeoimgMeshDispType ; 

	this->m_lfScale = mesh.m_lfScale;
	
	m_pAddPts= new  POINT3D[NUMALLOC];
	m_pAddFaces = new stFACE[NUMALLOC];
	this->m_nAddFaces = mesh.m_nAddFaces;
	this->m_nAddPnt = mesh.m_nAddPnt;

	if( (mesh.m_pAddPts != NULL)  && (mesh.m_pAddFaces != NULL) )
	{		
		for(int i=0; i<NUMALLOC; i++)
		{
			this->m_pAddPts[i].x = mesh.m_pAddPts[i].x;
			this->m_pAddPts[i].y = mesh.m_pAddPts[i].y;
			this->m_pAddPts[i].z = mesh.m_pAddPts[i].z;
			
			this->m_pAddFaces[i].n = mesh.m_pAddFaces[i].n;
			
			for(int j=0; j<m_pAddFaces[i].n; j++)
			{
				m_pAddFaces[i].vI[j] =mesh.m_pAddFaces[i].vI[j];
			}
		}
	}
	
	m_lfStat= new  double[STATPARANUM];
	if(mesh.m_lfStat != NULL)
	{
		
		for(int  i=0; i < STATPARANUM; i++)
			this->m_lfStat[i] = mesh.m_lfStat[i];	
	}
	this->m_nFaceSum = mesh.m_nFaceSum;
	this->m_nPtSum  = mesh.m_nPtSum; 

	/*
	this->m_nFaceSum = mesh.m_nFaceSum;
	this->m_nPtSum  = mesh.m_nPtSum; 
	m_pPts = new POINT3D[m_nPtSum]; 
	for(i=0; i<m_nPtSum; i++)
	{
		m_pPts[i].x = mesh.m_pPts[i].x;
		m_pPts[i].y = mesh.m_pPts[i].y;
		m_pPts[i].z = mesh.m_pPts[i].z;

		m_pPts[i].IsNull = mesh.m_pPts[i].IsNull;		
	}

	m_pFaces = new stFACE[m_nFaceSum];
	for(i=0; i<m_nFaceSum; i++)
	{
		m_pFaces[i].n = mesh.m_pFaces[i].n;

		for(int j=0; j<m_pFaces[i].n; j++)
		{
			m_pFaces[i].vI[j] = mesh.m_pFaces[i].vI[j];
		}
	}
	*/
}

C2DMesh::~C2DMesh()
{
	if(m_p3Dx != NULL)
		delete []m_p3Dx;
	if(m_p3Dy != NULL)
		delete []m_p3Dy;
	if(m_p3Dz != NULL)
		delete []m_p3Dz;

	if(m_pAddPts != NULL)
		delete m_pAddPts;
	if(m_pAddPts != NULL)
		delete m_pAddFaces;
	if(m_lfStat != NULL)
		delete m_lfStat;
}

bool C2DMesh::Load(CString file)
{
			
	FILE *fp = NULL;
    fp = fopen(file, "r");
	
	if(fp != NULL)
	{		
		int n3DPtsNum = 0;
		int n2DInnerPtsNum = 0;
		int nTriangleNum = 0;
		
		float gX = 0;
		float gY = 0;
		float gZ = 0;
		float x = 0;
		float y = 0;
		int   index = 0;
		int va = 0;
		int vb = 0;
		int vc = 0;		

		//read the 3D points
		fscanf(fp, "%d", &n3DPtsNum);	
		m_n3DPtNum = n3DPtsNum;

		if(m_p3Dx != NULL)
			delete []m_p3Dx;
		if(m_p3Dy != NULL)
			delete []m_p3Dy;
		if(m_p3Dz != NULL)
			delete []m_p3Dz;

		m_p3Dx = new float[m_n3DPtNum];
		m_p3Dy = new float[m_n3DPtNum];
		m_p3Dz = new float[m_n3DPtNum];

		for (int i=0; i<n3DPtsNum; i++)
		{
			fscanf(fp,"%f %f %f", &gX, &gY, &gZ);			
			
			m_p3Dx[i] = gX;
			m_p3Dy[i] = gY;
			m_p3Dz[i] = gZ;			
		}

		int nEdgePtsNum = 0;
		//read the number of edge points, (Actually, it's number of border nodes.-zbf 041029)
		fscanf(fp, "%d", &nEdgePtsNum);
		m_nBorderLen = nEdgePtsNum;
		//read the 2D mesh points (2D internal nodes -zbf 041029)
		fscanf(fp, "%d", &n2DInnerPtsNum);
        
		int n2DPtsNum = nEdgePtsNum + n2DInnerPtsNum;

		m_CVertexArray.SetSize(n2DPtsNum + 1);
		m_nNumOfVertexs = n2DPtsNum + 1;  //the number of triangle vertex		
	
		//m_CVertexArray.SetSize(n2DPtsNum );
		//m_nNumOfVertexs = n2DPtsNum ;  //the number of triangle vertex		
						
		for ( i=0; i<n2DPtsNum; i++)
		{
			fscanf(fp, "%f %f %d", &x, &y, &index);
			
			m_CVertexArray[i].dX = x;
			m_CVertexArray[i].dY = y;
			m_CVertexArray[i].dZ = 0; //default set as 0
			m_CVertexArray[i].index = index;
		}
	 
		m_CVertexArray[n2DPtsNum].dX = 0;
		m_CVertexArray[n2DPtsNum].dY = 0;
    	m_CVertexArray[n2DPtsNum].dZ = 1.0; //default set as 0
				 		
		//read the triangle index	
		fscanf(fp, "%d", &nTriangleNum);
		m_CTriangleArray.SetSize(nTriangleNum);
        m_nNumOfTriangles = nTriangleNum;
		
		//m_pCRelxationLabelling->SetTriangleNum(nTriangleNum);
		
		for (i=0; i<nTriangleNum; i++)
		{
			fscanf(fp, "%d %d %d", &va, &vb, &vc);

			m_CTriangleArray[i].pnV[0] = va;
			m_CTriangleArray[i].pnV[1] = vb;
			m_CTriangleArray[i].pnV[2] = vc;	
		}		
		fclose(fp);
		
		ComputeColor();
	}	
 
	return TRUE;
}

int     C2DMesh::GetBorderLen()
{
	    return m_nBorderLen;
}

int     C2DMesh::Get3DPointNum()
{
	return m_n3DPtNum;
}
void   C2DMesh::Get3DPoint(float &x, float &y, float &z, int index)
{
	ASSERT(index < m_n3DPtNum);
	x = m_p3Dx[index];
	y = m_p3Dy[index];
	z = m_p3Dz[index];
}

void  C2DMesh::ComputeColor()
{	
	int n = m_CTriangleArray.GetSize();
	for( int i=0; i<n; i++ )
	{
		int ia;
		int ib;
		int ic;
		
		//CNode3D pa, pb, pc;
		float pax,pay,paz;
		float pbx,pby,pbz;
		float pcx,pcy,pcz;
		
		float a[3],b[3],c[3];
		a[0] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[0] ].dX ) ;
		a[1] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[0] ].dY );
		a[2] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[0] ].dZ - 0.5 );
		ia = m_CVertexArray[m_CTriangleArray[i].pnV[0] ].index;
		//pa = m_pCRelxationLabelling->Get3DPoint(ia);
		pax = m_p3Dx[ia];
		pay = m_p3Dy[ia];
		paz = m_p3Dz[ia];

		
		b[0] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[1] ].dX );
		b[1] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[1] ].dY );
		b[2] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[1] ].dZ - 0.5 );
		ib = m_CVertexArray[m_CTriangleArray[i].pnV[1] ].index;
		//pb = m_pCRelxationLabelling->Get3DPoint(ib);
		pbx = m_p3Dx[ib];
		pby = m_p3Dy[ib];
		pbz = m_p3Dz[ib];

		
		c[0] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[2] ].dX );
		c[1] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[2] ].dY );
		c[2] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[2] ].dZ - 0.5 );
		ic = m_CVertexArray[m_CTriangleArray[i].pnV[2] ].index;
		//pc = m_pCRelxationLabelling->Get3DPoint(ic);
		pcx = m_p3Dx[ic];
		pcy = m_p3Dy[ic];
		pcz = m_p3Dz[ic];
	
		float d2_ab = 0;
		float d3_ab = 0;
		
		float d2_ac = 0;
		float d3_ac = 0;
		
		float d2_bc = 0;
		float d3_bc = 0;
		
		d2_ab = (float)sqrt( (a[0]-b[0])*(a[0]-b[0]) + 
			(a[1]-b[1])*(a[1]-b[1]) +
			(a[2]-b[2])*(a[2]-b[2]) );
		d3_ab = (float)sqrt( (pax-pbx)*(pax-pbx) + 
			(pay-pby)*(pay-pby) +
			(paz-pbz)*(paz-pbz) );
		
		d2_ac = (float)sqrt( (a[0]-c[0])*(a[0]-c[0]) + 
			(a[1]-c[1])*(a[1]-c[1]) +
			(a[2]-c[2])*(a[2]-c[2]) );
		d3_ac = (float)sqrt( (pax-pcx)*(pax-pcx) + 
		          (pay-pcy)*(pay-pcy) +
				  (paz-pcz)*(paz-pcz) );
		
		d2_bc = (float)sqrt( (c[0]-b[0])*(c[0]-b[0]) + 
			(c[1]-b[1])*(c[1]-b[1]) +
			(c[2]-b[2])*(c[2]-b[2]) );
		d3_bc = (float)sqrt( (pbx-pcx)*(pbx-pcx) + 
			(pby-pcy)*(pby-pcy) +
			(pbz-pcz)*(pbz-pcz) );
		
		float ratio = 0;
		float cr,cg,cb;
		int   nr,ng,nb;
		
		ratio = ComputeStretchRatio(d3_ab, d2_ab);
		int H = 240-(int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		m_CTriangleArray[i].dC12 = RGB(nr, ng, nb);
		m_CTriangleArray[i].dfRatio12 = d3_ab / d2_ab;
		
		ratio = ComputeStretchRatio(d3_ac, d2_ac);
		H = 240 - (int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		m_CTriangleArray[i].dC31 = RGB(nr, ng, nb);
		m_CTriangleArray[i].dfRatio31 = d3_ac / d2_ac;
		
		ratio = ComputeStretchRatio(d3_bc, d2_bc);
		H = 240 - (int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		m_CTriangleArray[i].dC23 = RGB(nr, ng, nb);
		m_CTriangleArray[i].dfRatio23 = d3_bc / d2_bc;
	}		
}

stFACE  C2DMesh::meshFaces(int n)
{
	stFACE  p;
	
	if(n < m_nFaceSum )
	{
		p.n = m_pFaces[n].n;

		for(int i=0; i<p.n; i++)
			p.vI[i] = m_pFaces[n].vI[i];
	}
	else if(n < m_nFaceSum+ m_nAddFaces)
		p = m_pAddFaces[n - m_nFaceSum];
	else
		AfxMessageBox("Exceed the range of the faces!");
	
	return(p);
}

int C2DMesh::FindMaxStrTri()
{
	int i,j;
	double Ss[3],St[3],L2;
	int t[3];
	double tx[3],ty[3];
	double a,b,c;
	int tempIndex;
	double *A;
	double *geoStretch;

	//double lfSum1,lfSum2;

	geoStretch=(double*)malloc(m_vNewFaces.size()*sizeof(double));
    A = (double*)malloc(m_vNewFaces.size()*sizeof(double));

	for(i=0; i<m_vNewFaces.size(); i++)
	{ 
		for(j=0; j < 3; j++)
		{ 
			if(m_vNewFaces.at(i).flag[j]==0)
			{
				tempIndex = m_vNewFaces.at(i).index[j];
				tx[j]=m_vInMapPnt.at(tempIndex).p.x;
				ty[j]=m_vInMapPnt.at(tempIndex).p.y;
				t[j]=m_vInMapPnt.at(tempIndex).pointIndex;
			}
			else if(m_vNewFaces.at(i).flag[j]==1)
			{ 
				tempIndex = m_vNewFaces.at(i).index[j];
				tx[j]=m_vBndMapPnt.at(tempIndex).p.x;
				ty[j]=m_vBndMapPnt.at(tempIndex).p.y;
				t[j]=m_vBndMapPnt.at(tempIndex).pointIndex;
			}
		}
		A[i]=((tx[1]-tx[0])*(ty[2]-ty[0])-(tx[2]-tx[0])*(ty[1]-ty[0]))/2;

		Ss[0]=((meshPnt(t[0]).x)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).x)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).x)*(ty[0]-ty[1]))/(2*A[i]);

		St[0]=((meshPnt(t[0]).x)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).x)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).x)*(tx[1]-tx[0]))/(2*A[i]);

		Ss[1]=((meshPnt(t[0]).y)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).y)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).y)*(ty[0]-ty[1]))/(2*A[i]);

		St[1]=((meshPnt(t[0]).y)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).y)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).y)*(tx[1]-tx[0]))/(2*A[i]);

		Ss[2]=((meshPnt(t[0]).z)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).z)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).z)*(ty[0]-ty[1]))/(2*A[i]);

		St[2]=((meshPnt(t[0]).z)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).z)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).z)*(tx[1]-tx[0]))/(2*A[i]);
		
		a=Ss[0]*Ss[0]+Ss[1]*Ss[1]+Ss[2]*Ss[2];
		b=Ss[0]*St[0]+Ss[1]*St[1]+Ss[2]*St[2];
		c=St[0]*St[0]+St[1]*St[1]+St[2]*St[2];

		L2=sqrt((a+c)/2);
		geoStretch[i]=L2;
	}

	double geoMax=-1000.0;
	int maxIndex;
	for(i=0; i < m_vNewFaces.size(); i++)
	{ 
		if(geoStretch[i]>geoMax) 
		{
			maxIndex =i;
			geoMax = geoStretch[i];
		}
	}
	free(A);
	A = NULL;

	delete geoStretch;

	return(maxIndex);
}

 

void C2DMesh::computeGeoStretch(double *geoS)
{
	int i,j;
	double Ss[3],St[3],L2;
	int t[3];
	double tx[3],ty[3];
	double a,b,c;
	int tempIndex;
	double *A;
	double* geoStretch;

	double lfSum1,lfSum2;

	geoStretch=(double*)malloc((m_vNewFaces.size())*sizeof(double));
	A = (double*)malloc(m_vNewFaces.size()*sizeof(double));

	for(i=0; i < m_vNewFaces.size(); i++)
	{
		A[i]=0;
		geoStretch[i]=0;
	}

	for(i=0; i<m_vNewFaces.size(); i++)
	{ 
		if(0)
		{
		if(m_vNewFaces.at(i).flag[0]==1 ||
			m_vNewFaces.at(i).flag[1]==1 ||
			m_vNewFaces.at(i).flag[2]==1)
			continue;
		}
		for(j=0; j < 3; j++)
		{ 
			if(m_vNewFaces.at(i).flag[j]==0)
			{
				tempIndex = m_vNewFaces.at(i).index[j];
				tx[j]=m_vInMapPnt.at(tempIndex).p.x;
				ty[j]=m_vInMapPnt.at(tempIndex).p.y;
				t[j]=m_vInMapPnt.at(tempIndex).pointIndex;
			}
			else if(m_vNewFaces.at(i).flag[j]==1)
			{ 
				tempIndex = m_vNewFaces.at(i).index[j];
				tx[j]=m_vBndMapPnt.at(tempIndex).p.x;
				ty[j]=m_vBndMapPnt.at(tempIndex).p.y;
				t[j]=m_vBndMapPnt.at(tempIndex).pointIndex;
			}
		}
		A[i]=((tx[1]-tx[0])*(ty[2]-ty[0])-(tx[2]-tx[0])*(ty[1]-ty[0]))/2;

		if(A[i]==0)
			A[i]=0;
	//	ASSERT(A[i]!=0);

		Ss[0]=((meshPnt(t[0]).x)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).x)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).x)*(ty[0]-ty[1]))/(2*A[i]);

		St[0]=((meshPnt(t[0]).x)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).x)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).x)*(tx[1]-tx[0]))/(2*A[i]);

		Ss[1]=((meshPnt(t[0]).y)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).y)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).y)*(ty[0]-ty[1]))/(2*A[i]);

		St[1]=((meshPnt(t[0]).y)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).y)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).y)*(tx[1]-tx[0]))/(2*A[i]);

		Ss[2]=((meshPnt(t[0]).z)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).z)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).z)*(ty[0]-ty[1]))/(2*A[i]);

		St[2]=((meshPnt(t[0]).z)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).z)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).z)*(tx[1]-tx[0]))/(2*A[i]);
		
		a=Ss[0]*Ss[0]+Ss[1]*Ss[1]+Ss[2]*Ss[2];
		b=Ss[0]*St[0]+Ss[1]*St[1]+Ss[2]*St[2];
		c=St[0]*St[0]+St[1]*St[1]+St[2]*St[2];

		L2=sqrt((a+c)/2);
		geoStretch[i]=L2;
	}

	lfSum1=0;
	lfSum2=0;
	for(i=0; i < m_vNewFaces.size(); i++)
	{ 
		lfSum1+=geoStretch[i]*geoStretch[i]*A[i];
		lfSum2+=A[i];
	}
	*geoS=sqrt(lfSum1/lfSum2);


    free(geoStretch);
	free(A);
}


//------------------------------------------------------------------------
//  Name: ComputeTriangleArea 
//  Desc: Compute the sum of areas of triangles of model
//        normalize the sum of areas to 1 by scaling the vertex coordinate
//------------------------------------------------------------------------
void   C2DMesh::ComputeTriangleArea()
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
		 i1 = m_CVertexArray[index1].index;
		 //p = m_pCRelxationLabelling->Get3DPoint(i1);
 	 	 float px,py,pz;
     	 px = m_p3Dx[i1];
		 py = m_p3Dy[i1];
		 pz = m_p3Dz[i1];

		 v1[0] = px;
		 v1[1] = py;
		 v1[2] = pz;

		 i2 = m_CVertexArray[index2].index;
		 //p = m_pCRelxationLabelling->Get3DPoint(i2);
		 px = m_p3Dx[i2];
		 py = m_p3Dy[i2];
		 pz = m_p3Dz[i2];

 		 v2[0] = px;
		 v2[1] = py;
		 v2[2] = pz;

		 i3 = m_CVertexArray[index3].index;
		 //p = m_pCRelxationLabelling->Get3DPoint(i3);
		 px = m_p3Dx[i3];
		 py = m_p3Dy[i3];
		 pz = m_p3Dz[i3];
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
	double lamda = sqrt( sumOfArea );
	lamda *= this->m_dMeshScale;

	//int nVertexSize = m_pCRelxationLabelling->m_n3DNode_length;

	for ( i=0; i<m_n3DPtNum; i++)
	{
		//CNode3D p = m_pCRelxationLabelling->Get3DPoint(i);
		float px,py,pz;
		px = m_p3Dx[i];
		py = m_p3Dy[i];
		pz = m_p3Dz[i];

		px= px / lamda;
		py= py / lamda;
		pz= pz / lamda;
		
		//m_pCRelxationLabelling->Set3DPoint(p,i);
	}	
}

void    C2DMesh::Get2DPtIndexTo3D(int &indexTo3D, int index)
{
         indexTo3D = m_CVertexArray[index].index;
}

int     C2DMesh::GetDispType()
{
	return m_nGeoimgMeshDispType;
}

/*
//set data
void    C2DMesh::SetMesh(int nVertexNum, double *x, double *y, double *z,
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
						 int nGeoImgMeshDispType)
{
		//before set a new mesh, we must release the memory for edges
     	ReleaseEdges();
	    
		m_lfScale = dfScale;

		m_nGeoimgMeshDispType = nGeoImgMeshDispType;
        
		//
		m_CVertexArray.SetSize(nVertexNum);
		for (int i=0; i<nVertexNum; i++ )
		{
     		m_CVertexArray[i].dX = x[i];
			m_CVertexArray[i].dY = y[i];
			m_CVertexArray[i].dZ = z[i]; 
        }
		
		//refresh the triangle 
		m_CTriangleArray.SetSize(nTriangle);
		for ( i=0; i<nTriangle; i++)
		{
			m_CTriangleArray[i].pnV[0] = nV1[i];
			m_CTriangleArray[i].pnV[1] = nV2[i];
			m_CTriangleArray[i].pnV[2] = nV3[i];
		}

		m_nFaceSum = nTriangle;
		m_nPtSum = nVertexNum;

		//
		m_nAddPnt = nAddPts;
		m_nAddFaces = nAddFaces;
				
		for(i=0; i< nAddPts; i++)
		{
			m_pAddPts[i].x = pAddPts[i].x;
			m_pAddPts[i].y = pAddPts[i].y;
			m_pAddPts[i].z = pAddPts[i].z;
			m_pAddPts[i].IsNull = pAddPts[i].IsNull;
		}
		for(i=0; i<nAddFaces; i++ )
		{
			m_pAddFaces[i].n = pAddFaces[i].n;		
			
			for(int j=0; j<pAddFaces[i].n; j++)
			{
				m_pAddFaces[i].vI[j] = m_pAddFaces[i].vI[j];
			}
		}		

		m_vCutPath = cutPath;
		m_vBndMapPnt = vBndMapPnt;
		m_vInMapPnt = vInMapPnt;
		m_vBndPnt = vBndPnt;
		m_vNewFaces = vNewFaces;
		m_vRemeshFaces = vRemeshFaces;
		m_vGeoMap = vGeoMap;

		Normalize(0.5);			
}
*/

void C2DMesh::SetMesh (int nTriangle, int* nV1, int* nV2, int* nV3, DWORD* c12, DWORD* c23, DWORD* c31,
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
		m_CVertexArray.SetSize(nVlength + 1);
		for (int i=0; i<nVlength; i++ )
		{
     		m_CVertexArray[i].dX = x[i];
			m_CVertexArray[i].dY = y[i];
			m_CVertexArray[i].dZ = z[i]; 
		}
        
		//to make the MIN != MAX
		m_CVertexArray[nVlength].dX = 0;
		m_CVertexArray[nVlength].dY = 0;
	    m_CVertexArray[nVlength].dZ = 1; 		
	}
	
	if(nVlength)
	{		
		m_nNumOfVertexs = nVlength + 1;
		
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
		m_nBorderLen = nEdgeNodeNum;
  
	//## end CMesh::SetMesh%4181ECA4023A.body
}

void    C2DMesh::SetMesh(const CMeshForGeoImg &meshForGeoimg)
{
		//before set a new mesh, we must release the memory for edges
     	ReleaseEdges();
	    
		m_lfScale = meshForGeoimg.GetScale();
		m_nGeoimgMeshDispType = meshForGeoimg.GetDispType();

		int i = 0;
		int nVertexNum = 0;
		int nTriangle = 0;

	    switch(m_nGeoimgMeshDispType)
		{
		case 0:
			
			m_nPtSum = meshForGeoimg.Get3DPtNum();
			m_CVertexArray.SetSize(m_nPtSum);
			for (i=0; i<m_nPtSum; i++ )
			{
				double x,y,z;
				meshForGeoimg.GetVertex(x, y, z, i);
				m_CVertexArray[i].dX = x;
				m_CVertexArray[i].dY = y;
				m_CVertexArray[i].dZ = z; 
			}

			m_nFaceSum = meshForGeoimg.GetTriangleNum();
			m_CTriangleArray.SetSize(m_nFaceSum);
			for ( i=0; i<m_nFaceSum; i++)
			{
				int va,vb,vc;
				meshForGeoimg.GetTriangle(va, vb, vc, i);
				m_CTriangleArray[i].pnV[0] = va;
				m_CTriangleArray[i].pnV[1] = vb;
				m_CTriangleArray[i].pnV[2] = vc;
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			m_nPtSum = meshForGeoimg.Get3DPtNum() + meshForGeoimg.GetAddPtNum();
			m_CVertexArray.SetSize(m_nPtSum);
			
			for(i=0; i < m_nPtSum; i++)
			{ 
				double x,y,z;
				meshForGeoimg.GetMeshPt(x, y, z, i);
				m_CVertexArray[i].dX = x;
				m_CVertexArray[i].dY = y;
				m_CVertexArray[i].dZ = z;
			}
			
			m_nFaceSum = meshForGeoimg.GetTriangleNum() + meshForGeoimg.GetAddFaceNum();
			m_CTriangleArray.SetSize(m_nFaceSum);		

			for(i=0; i<m_nFaceSum; i++)
			{			
				int va,vb,vc;
				meshForGeoimg.GetMeshFace(va, vb, vc, i);				
				m_CTriangleArray[i].pnV[0] = va;
				m_CTriangleArray[i].pnV[1] = vb;
				m_CTriangleArray[i].pnV[2] = vc;
			}
			break;

		case 5:
			break;		
		}

		//
		m_nAddPnt = meshForGeoimg.GetAddPtNum();
		m_nAddFaces = meshForGeoimg.GetAddFaceNum();	
		for(i=0; i< meshForGeoimg.GetAddPtNum(); i++)
		{
			double x,y,z;
			bool bIsNull;
			meshForGeoimg.GetAddPt(x, y, z, bIsNull, i);
			m_pAddPts[i].x = x;
			m_pAddPts[i].y = y;
			m_pAddPts[i].z = z;
			m_pAddPts[i].IsNull = bIsNull;
		}
		for(i=0; i<meshForGeoimg.GetAddFaceNum(); i++ )
		{
			int va,vb,vc,n;
			meshForGeoimg.GetAddFace(va, vb, vc, n, i);
			m_pAddFaces[i].n = n;		
			m_pAddFaces[i].vI[0] = va;
			m_pAddFaces[i].vI[1] = vb;
			m_pAddFaces[i].vI[2] = vc;
		}			

		//
		m_vCutPath = meshForGeoimg.GetCutPath();
		m_vBndMapPnt = meshForGeoimg.GetBndMapPnt(); 
		m_vInMapPnt = meshForGeoimg.GetInMapPnt();
		m_vBndPnt = meshForGeoimg.GetBndPnt();
		m_vNewFaces = meshForGeoimg.GetNewFaces();
		m_vRemeshFaces = meshForGeoimg.GetRemeshFaces();
		m_vGeoMap = meshForGeoimg.GetGeoMap();
		
		Normalize(0.5);	
}

void    C2DMesh::DisplayCutpath()
{
 	int i;
	float t1,t2,t3,t4,t5,t6;
	
 	glColor3f(1.0f, 1.0f, 0.0f);
 	glLineWidth(3.0);
	
  	for(i=0; i< m_vCutPath.size(); i++)
 	 	{
 	 		t1=m_vCutPath.at(i).P[0].x;
 	 		t2=m_vCutPath.at(i).P[0].y;
 	 		t3=m_vCutPath.at(i).P[0].z;
 	 		t4=m_vCutPath.at(i).P[1].x;
 	 		t5=m_vCutPath.at(i).P[1].y;
 	 		t6=m_vCutPath.at(i).P[1].z;
 	 
 	 		glBegin(GL_LINES);
 	 			glVertex3f(t1,t2,t3);
 	 			glVertex3f(t4,t5,t6);
 	 		glEnd();
 	 	} 
	glColor3f(1.0f, 1.0f, 1.0f);
 	glLineWidth(1.0f);
}


//显示边界参数化后的结果
void C2DMesh::DisplayBoundaryMap()
{
	int i;
	vector<MapPoint>::iterator  index;
	index = m_vBndMapPnt.begin();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(-0.5f, 0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(0.5f, -0.5f);
	glEnd();

	glColor3f(1.0, 0.0, 1.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	for(i=0; i < m_vBndMapPnt.size(); i++)
			glVertex2f(m_vBndMapPnt.at(i).p.x-0.5, m_vBndMapPnt.at(i).p.y-0.5);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	for(i=0; i < m_vBndMapPnt.size(); i++)
			glVertex2f(m_vBndMapPnt.at(i).p.x-0.5, m_vBndMapPnt.at(i).p.y - 0.5);
	glVertex2f(-0.5, -0.5);
 	glEnd();
 
	glLineWidth(1.0);
	glPointSize(1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
void C2DMesh::CreateStretchRatio()
{
	
	int i = 0,j = 0;
 	int tstart = 0,tend = 0; 
	
	double t1 = 0.0, t2 = 0.0, t3 = 0.0, t4 = 0.0;
 	double lf3dlen = 0.0, lf2dlen = 0.0;
	
	double R = 0.0, G = 0.0, B = 0.0;
	double H = 0.0, S = 0.0, I = 0.0;

	double lfRatio = 0.0;
	double lfx = 0.0;
	double lfRatioMax = 0.0,lfRatioMin = 0.0;
	double min = 0.0 , max = 0.0; 	
	int    tempIndex = 0;
	double T = 0.0, P = 0.0, Q = 0.0,F = 0.0;
	int    TEMP = 0;
	
	
	double *lfStat = NULL;
	int nCnt = 0;
		
	//geoStretch=(double*)malloc((m_vNewFaces.size())*sizeof(double));
	lfStat = (double*)malloc((3*m_vNewFaces.size())*sizeof(double));
	nCnt = 0;
	
	glColor3f(0.0f, 1.0f, 1.0f);
	glLineWidth(1.0);
	
	I = 1.0;
	S = 1.0;
	
	max = I + S - I * S;
	min = 2*I - max;
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	lfRatioMin = 1000.0;
	lfRatioMax = -1000.0;

	int nSize = m_vNewFaces.size();

	for(i = 0; i < nSize; i++)
	{ 
		for(j=0; j < 3; j++)
		{ 
			int index = m_vNewFaces.at(i).flag[j];

			if(index == 0)
			{
				tempIndex = m_vNewFaces.at(i).index[j];
				t1 = m_vInMapPnt.at(tempIndex).p.x;
				t2 = m_vInMapPnt.at(tempIndex).p.y;
				tstart = m_vInMapPnt.at(tempIndex).pointIndex;
			}
			else if(index == 1)
			{ 
				tempIndex = m_vNewFaces.at(i).index[j];
				t1 = m_vBndMapPnt.at(tempIndex).p.x;
				t2 = m_vBndMapPnt.at(tempIndex).p.y;
				tstart = m_vBndMapPnt.at(tempIndex).pointIndex;
			}

			index = m_vNewFaces.at(i).flag[(j+1)%3];
			
			if(index == 0)
			{
				tempIndex = m_vNewFaces.at(i).index[(j+1)%3];
				t3=m_vInMapPnt.at(tempIndex).p.x;
				t4=m_vInMapPnt.at(tempIndex).p.y;
				tend=m_vInMapPnt.at(tempIndex).pointIndex;
			}
			else if(index == 1)
			{ 
				tempIndex = m_vNewFaces.at(i).index[(j+1)%3];
				t3=m_vBndMapPnt.at(tempIndex).p.x;
				t4=m_vBndMapPnt.at(tempIndex).p.y;
				tend=m_vBndMapPnt.at(tempIndex).pointIndex;
			}
			
			lf3dlen = sqrt((meshPnt(tstart).x - meshPnt(tend).x)*(meshPnt(tstart).x - meshPnt(tend).x)+
				           (meshPnt(tstart).y - meshPnt(tend).y)*(meshPnt(tstart).y - meshPnt(tend).y)+
				           (meshPnt(tstart).z - meshPnt(tend).z)*(meshPnt(tstart).z - meshPnt(tend).z));
			
			lf2dlen = sqrt((t1-t3)*(t1-t3)+(t2-t4)*(t2-t4)); 
			
			lf2dlen = lf2dlen/m_lfScale; 
			
			lfx = fabs(lf3dlen - lf2dlen);
			
			lfRatio = (2.71828 - pow(2.71828, (1/(lfx+1)))) / (2.71828 - 1);
			
			if(lfRatio > lfRatioMax)  
				lfRatioMax = lfRatio;

			if(lfRatio < lfRatioMin) 
				lfRatioMin = lfRatio;
			
			lfStat[nCnt++] = lfRatio;
			
			H = 240 - (int)(lfRatio * 240) % 240;
			H /= 60.0;
			TEMP = floor(H);
			
			F = H - TEMP;
			P = I *(1.0 -S);
			Q = I * (1.0 - (S*F));
			T = I *(1.0 - (S *(1.0 -F)));
			
			switch(TEMP) 
			{
			case 0: R = I;  G = T;  B = P;   break;
			case 1: R = Q;  G = I;  B = P;   break;
			case 2: R = P;  G = I;  B = T;   break;
			case 3: R = P;  G = Q;  B = I;   break; 
			case 4: R = T;  G = P;  B = I;   break;
			case 5: R = I;  G = P;  B = Q;   break;
			}
			
			glColor3f(R, G, B);
			if(lf2dlen > lf3dlen)
			{
				
			}
			
		}	
	}
}

void C2DMesh::DisplayInterior2()
{

	int i = 0,j = 0;
 	int tstart = 0,tend = 0; 
	
	double t1 = 0.0, t2 = 0.0, t3 = 0.0, t4 = 0.0;
 	double lf3dlen = 0.0, lf2dlen = 0.0;
	
	double R = 0.0, G = 0.0, B = 0.0;
	double H = 0.0, S = 0.0, I = 0.0;

	double lfRatio = 0.0;
	double lfx = 0.0;
	double lfRatioMax = 0.0,lfRatioMin = 0.0;
	double min = 0.0 , max = 0.0; 	
	int    tempIndex = 0;
	double T = 0.0, P = 0.0, Q = 0.0,F = 0.0;
	int    TEMP = 0;
		
	double *lfStat = NULL;
	int nCnt = 0;
		
	//geoStretch=(double*)malloc((m_vNewFaces.size())*sizeof(double));
	lfStat = (double*)malloc((3*m_vNewFaces.size())*sizeof(double));
	nCnt = 0;
	
	glColor3f(0.0f, 1.0f, 1.0f);
	glLineWidth(1.0);
	
	I = 1.0;
	S = 1.0;
	
	max = I + S - I * S;
	min = 2*I - max;
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	lfRatioMin = 1000.0;
	lfRatioMax = -1000.0;

	int nSize = m_vNewFaces.size();
    
	//FILE *fp = NULL;
    
	//fp = fopen("F:\\debug-record.txt","w");

	for(i = 0; i < nSize; i++)
	{ 
		for(j=0; j < 3; j++)
		{ 

			int index = m_vNewFaces.at(i).flag[j];

			if(index == 0)
			{
				tempIndex = m_vNewFaces.at(i).index[j];
				t1 = m_vInMapPnt.at(tempIndex).p.x;
				t2 = m_vInMapPnt.at(tempIndex).p.y;
				tstart = m_vInMapPnt.at(tempIndex).pointIndex;
			}
			else if(index == 1)
			{ 
				tempIndex = m_vNewFaces.at(i).index[j];
				t1 = m_vBndMapPnt.at(tempIndex).p.x;
				t2 = m_vBndMapPnt.at(tempIndex).p.y;
				tstart = m_vBndMapPnt.at(tempIndex).pointIndex;
			}

			index = m_vNewFaces.at(i).flag[(j+1)%3];
			
			if(index == 0)
			{
				tempIndex = m_vNewFaces.at(i).index[(j+1)%3];
				t3=m_vInMapPnt.at(tempIndex).p.x;
				t4=m_vInMapPnt.at(tempIndex).p.y;
				tend=m_vInMapPnt.at(tempIndex).pointIndex;
			}
			else if(index == 1)
			{ 
				tempIndex = m_vNewFaces.at(i).index[(j+1)%3];
				t3=m_vBndMapPnt.at(tempIndex).p.x;
				t4=m_vBndMapPnt.at(tempIndex).p.y;
				tend=m_vBndMapPnt.at(tempIndex).pointIndex;
			}
			
			lf3dlen = sqrt((meshPnt(tstart).x - meshPnt(tend).x)*(meshPnt(tstart).x - meshPnt(tend).x)+
				           (meshPnt(tstart).y - meshPnt(tend).y)*(meshPnt(tstart).y - meshPnt(tend).y)+
				           (meshPnt(tstart).z - meshPnt(tend).z)*(meshPnt(tstart).z - meshPnt(tend).z));

			lf2dlen = sqrt((t1-t3)*(t1-t3)+(t2-t4)*(t2-t4)); 
			lf2dlen = lf2dlen/m_lfScale; 
			lfx = fabs(lf3dlen - lf2dlen);

			lfRatio = (2.71828 - pow(2.71828, (1/(lfx+1)))) / (2.71828 - 1);
			            
			//fprintf(fp, "m_lfScale: %lf  lf3dlen: %lf  lf2dlen : %lf  lfx : %lf \n", m_lfScale, lf3dlen, lf2dlen, lfx );
			//fprintf(fp," lfRatioMax: %lf lfRationMin: %lf  lfRatio: %lf \n" , lfRatioMax, lfRatioMin, lfRatio);

			if(lfRatio > lfRatioMax)  
				lfRatioMax = lfRatio;

			if(lfRatio < lfRatioMin) 
				lfRatioMin = lfRatio;
			
			lfStat[nCnt++] = lfRatio;
			
			H = 240 - (int)(lfRatio * 240) % 240;
			H /= 60.0;
			TEMP = floor(H);
			
			F = H - TEMP;
			P = I *(1.0 -S);
			Q = I * (1.0 - (S*F));
			T = I *(1.0 - (S *(1.0 -F)));
			
			switch(TEMP) 
			{
			case 0: R = I;  G = T;  B = P;   break;
			case 1: R = Q;  G = I;  B = P;   break;
			case 2: R = P;  G = I;  B = T;   break;
			case 3: R = P;  G = Q;  B = I;   break; 
			case 4: R = T;  G = P;  B = I;   break;
			case 5: R = I;  G = P;  B = Q;   break;
			}
			
			glColor3f(R, G, B);
			//glColor3f(1.0, 1.0, 0.0);
			if(lf2dlen > lf3dlen)
			{
				glLineStipple(3,0xAAAA);
				glEnable(GL_LINE_STIPPLE);
			}
            
			//fprintf(fp,"t1: %lf t2: %lf t3: %lf t4: %lf\n", t1, t2, t3, t4);  
			
			glBegin(GL_LINES);
			glVertex2f(t1-0.5, t2-0.5);
			glVertex2f(t3-0.5, t4-0.5);
			glEnd();

			glDisable(GL_LINE_STIPPLE);
		}		
	}

	//fclose(fp);
	
	glDisable(GL_LINE_STIPPLE);	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	int t;
	double x[3],y[3];
	t=FindMaxStrTri();
	
	for(j=0; j < 3; j++)
	{ 
		if(m_vNewFaces.at(t).flag[j]==0)
		{
			tempIndex = m_vNewFaces.at(t).index[j];
			x[j]=m_vInMapPnt.at(tempIndex).p.x;
			y[j]=m_vInMapPnt.at(tempIndex).p.y;
		}
		else if(m_vNewFaces.at(t).flag[j]==1)
		{ 
			tempIndex = m_vNewFaces.at(t).index[j];
			x[j]=m_vBndMapPnt.at(tempIndex).p.x;
			y[j]=m_vBndMapPnt.at(tempIndex).p.y;
		}		
	}
	
	glColor3f(1.0f,0.0f,0.0f);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_TRIANGLES);
	glVertex2f(x[0] - 0.5, y[0] - 0.5);
	glVertex2f(x[1] - 0.5, y[1] - 0.5);
	glVertex2f(x[2] - 0.5, y[2] - 0.5);
	glEnd();
	
	
	double lfMean;
	double lfCov;
	double lfSum=0.0;
	//计算平均值
	for(i=0; i < nCnt; i++)
	{ 
		lfSum += lfStat[i];
	}
	lfMean = (lfSum)/(double)nCnt;
	
	lfSum=0;
	for(i=0; i < nCnt; i++)
	{ 
		lfSum += (lfStat[i]-lfMean)*(lfStat[i]-lfMean);
	}
	
	lfCov = sqrt(lfSum/(nCnt-1));
	
	//geometric stretch
	double geoS;
	computeGeoStretch(&geoS);
	
	m_lfStat[0]=lfRatioMax;
	m_lfStat[1]=lfRatioMin;
	m_lfStat[2]=lfMean;
	m_lfStat[3]=lfCov;
	m_lfStat[4]=geoS;	 

	delete lfStat;

}

void C2DMesh::DisplayGeoImage()
{
	int i,j;
	double lfmax,lfmin;
	double t1,t2,t3;
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(-0.5f, 0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(0.5f, -0.5f);
	glEnd();

 	glPointSize(5.0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable(GL_SMOOTH);
	
	lfmax = -100;
	lfmin = 100;

	float ptMinX = 1000, ptMaxX = -1000;
	float ptMinY = 1000,ptMaxY = -1000;

	for(i=0; i <=NGRID; i++)
		for(j = 0; j<=NGRID; j++)
		{ 
			if(m_vGeoMap[i*(NGRID+1) + j].fx > lfmax)
				lfmax = m_vGeoMap[i*(NGRID+1) + j].fx;
			if(m_vGeoMap[i*(NGRID+1) + j].fy > lfmax)
				lfmax = m_vGeoMap[i*(NGRID+1) + j].fy;
			if(m_vGeoMap[i*(NGRID+1) + j].fz > lfmax)
				lfmax = m_vGeoMap[i*(NGRID+1) + j].fz;
		
			if(m_vGeoMap[i*(NGRID+1) + j].fx < lfmin)
				lfmin = m_vGeoMap[i*(NGRID+1) + j].fx;
			if(m_vGeoMap[i*(NGRID+1) + j].fy < lfmin)
				lfmin = m_vGeoMap[i*(NGRID+1) + j].fy;
			if(m_vGeoMap[i*(NGRID+1) + j].fz < lfmin)
				lfmin = m_vGeoMap[i*(NGRID+1) + j].fz;
            
			if( ptMinX > m_vGeoMap[(i)*(NGRID+1)+j].p.x )
				ptMinX = m_vGeoMap[(i)*(NGRID+1)+j].p.x;
			if( ptMinY > m_vGeoMap[(i)*(NGRID+1)+j].p.y )
				ptMinY = m_vGeoMap[(i)*(NGRID+1)+j].p.y;

			if( ptMaxX < m_vGeoMap[(i)*(NGRID+1)+j].p.x )
				ptMaxX = m_vGeoMap[(i)*(NGRID+1)+j].p.x;
			if( ptMaxY < m_vGeoMap[(i)*(NGRID+1)+j].p.y )
				ptMaxY = m_vGeoMap[(i)*(NGRID+1)+j].p.y;
		}
		
	float fMeanX = (ptMinX+ptMaxX)/2;
	float fMeanY = (ptMinY+ptMaxY)/2;    

	for(i = 0; i<NGRID ; i++)
		for(j = 0; j <NGRID ; j++)
		{ 
		   glBegin(GL_POLYGON);
		   t1=(m_vGeoMap[i*(NGRID+1)+j].fx-lfmin)/(lfmax-lfmin);
		   t2=(m_vGeoMap[i*(NGRID+1)+j].fy-lfmin)/(lfmax-lfmin);
		   t3=(m_vGeoMap[i*(NGRID+1)+j].fz-lfmin)/(lfmax-lfmin);
		   glColor3f(t1,t2,t3);
		   glVertex2f(m_vGeoMap[i*(NGRID+1)+j].p.x - fMeanX,
			          m_vGeoMap[i*(NGRID+1)+j].p.y - fMeanY);
		   
		   t1=(m_vGeoMap[(i+1)*(NGRID+1)+j].fx-lfmin)/(lfmax-lfmin);
		   t2=(m_vGeoMap[(i+1)*(NGRID+1)+j].fy-lfmin)/(lfmax-lfmin);
		   t3=(m_vGeoMap[(i+1)*(NGRID+1)+j].fz-lfmin)/(lfmax-lfmin);
		   
		   glColor3f(t1,t2,t3);
		   glVertex2f(m_vGeoMap[(i+1)*(NGRID+1)+j].p.x - fMeanX, 
			          m_vGeoMap[(i+1)*(NGRID+1)+j].p.y - fMeanY);

		   t1=(m_vGeoMap[(i+1)*(NGRID+1)+j+1].fx-lfmin)/(lfmax-lfmin);
		   t2=(m_vGeoMap[(i+1)*(NGRID+1)+j+1].fy-lfmin)/(lfmax-lfmin);
		   t3=(m_vGeoMap[(i+1)*(NGRID+1)+j+1].fz-lfmin)/(lfmax-lfmin);
		   glColor3f(t1,t2,t3);
		   glVertex2f(m_vGeoMap[(i+1)*(NGRID+1)+j+1].p.x - fMeanX,
			          m_vGeoMap[(i+1)*(NGRID+1)+j+1].p.y - fMeanY);

		   t1=(m_vGeoMap[i*(NGRID+1)+j+1].fx-lfmin)/(lfmax-lfmin);
		   t2=(m_vGeoMap[i*(NGRID+1)+j+1].fy-lfmin)/(lfmax-lfmin);
		   t3=(m_vGeoMap[i*(NGRID+1)+j+1].fz-lfmin)/(lfmax-lfmin);
		   
		   glColor3f(t1,t2,t3);
		   glVertex2f(m_vGeoMap[i*(NGRID+1)+j+1].p.x - fMeanX, 
			          m_vGeoMap[i*(NGRID+1)+j+1].p.y - fMeanY);
		   glEnd();	
		}

	glPointSize(1.0);

		
}

void C2DMesh::DisplayReconstruct()
{
	int i;

	glColor3f(1.0f, 1.0f, 0.0f);
	
	glLineWidth(1.0);
	
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	for(i = 0; i < m_vRemeshFaces.size(); i++)
	{ 
		POINT3D p1,p2,p3;
		p1.x = m_vRemeshFaces.at(i).v1.x;
		p1.y = m_vRemeshFaces.at(i).v1.y;
		p1.z = m_vRemeshFaces.at(i).v1.z;

		p2.x = m_vRemeshFaces.at(i).v2.x;
		p2.y = m_vRemeshFaces.at(i).v2.y;
		p2.z = m_vRemeshFaces.at(i).v2.z;

		p3.x = m_vRemeshFaces.at(i).v3.x;
		p3.y = m_vRemeshFaces.at(i).v3.y;
		p3.z = m_vRemeshFaces.at(i).v3.z;
		
		POINT3D v1,v2;
		v1.x = p1.x - p2.x;
		v1.y = p1.y - p2.y;
		v1.y = p1.z - p2.z;

		v2.x = p3.x - p2.x;
		v2.y = p3.y - p2.y;
		v2.y = p3.z - p2.z;

		POINT3D nml = CrossValue( v1, v2 ); 
        glNormal3f(nml.x, nml.y, nml.z);
		glEnable(GL_NORMALIZE );

		glBegin(GL_TRIANGLES);
			glVertex3f(m_vRemeshFaces.at(i).v1.x, m_vRemeshFaces.at(i).v1.y, m_vRemeshFaces.at(i).v1.z);
			glVertex3f(m_vRemeshFaces.at(i).v2.x, m_vRemeshFaces.at(i).v2.y, m_vRemeshFaces.at(i).v2.z);
			glVertex3f(m_vRemeshFaces.at(i).v3.x, m_vRemeshFaces.at(i).v3.y, m_vRemeshFaces.at(i).v3.z);
		glEnd();
	}
}

POINT3D  C2DMesh::meshPnt(int n)
{
	POINT3D  p;
	
	if(n < m_nPtSum)
	{
		p.x = m_CVertexArray[n].dX;
		p.y = m_CVertexArray[n].dY;
		p.z = m_CVertexArray[n].dZ;
	}
	else if( n < m_nPtSum+m_nAddPnt)
	{
		p = m_pAddPts[n-m_nPtSum];
	}
	else
	{ 
		AfxMessageBox("Exceed the range of the points!");
	}

	return(p);
}

BOOL  C2DMesh::Write(CString strFile)
{

	int nStrLen = strFile.GetLength() - 4;
	CString fileTile  = strFile.Left(nStrLen);
	CString file2dmesh = fileTile + "_2DMeshResult.txt";

	FILE *fp = fopen(strFile, "w");
	if(fp == NULL)
		return FALSE;
	
	//save the 3D points:
	fprintf(fp, "%d", m_n3DPtNum);
	for(int i=0; i<m_n3DPtNum; i++)
	{
		fprintf(fp, "%f %f %f \n", m_p3Dx[i], m_p3Dy[i], m_p3Dz[i] );
	}
   
	//save the 2d points
	int nBorderPtNum = GetBorderLen();
	int nVertexNum = GetVertexNum();
	fprintf(fp, "%d %d", nBorderPtNum, nVertexNum-nBorderPtNum);

	for(i=0; i<nVertexNum; i++)
	{
		float x,y,z;
		
		GetVertex(x, y, z, i);
		fprintf(fp, "%f %f %f\n", x, y, z);
	}
	
	//save the triangle point list
	int nTriangleNum = GetTriangleNum();
	for(i=0; i<nTriangleNum; i++)
	{
		int va,vb,vc;
		GetTriangleIndices(va, vb, vc, i);
		fprintf(fp, "%d %d %d", va, vb, vc);
	}
	fclose(fp);

	return TRUE;
}
