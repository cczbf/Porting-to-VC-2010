//## begin module%4175B5880383.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%4175B5880383.cm

//## begin module%4175B5880383.cp preserve=no
//## end module%4175B5880383.cp

//## Module: CRelxationLabelling%4175B5880383; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CRelxationLabelling.cpp

//## begin module%4175B5880383.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%4175B5880383.additionalIncludes

//## begin module%4175B5880383.includes preserve=yes
#include "math.h"
#include <FLOAT.H>
//## end module%4175B5880383.includes

// CRelxationLabelling
#include "CRelxationLabelling.h"
//## begin module%4175B5880383.declarations preserve=no
//## end module%4175B5880383.declarations

//## begin module%4175B5880383.additionalDeclarations preserve=yes
#include "progressbar.h"
#include "3dviewerdoc.h"
#include "copengl3dex.h"

#include "cglobals.h"

#include "CMesh.h"
#include "LabelPts.h"
#include "2DMesh.h"
#include "CMeshOpti.h"
#include "MeshForGeoImg.h"

// CEasyProgressBar
#include "CEasyProgressBar.h"



#define PI 3.14159265358979323846
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
//## end module%4175B5880383.additionalDeclarations


// Class CRelxationLabelling 




























CRelxationLabelling::CRelxationLabelling ()
  //## begin CRelxationLabelling::CRelxationLabelling%418197CC0270.hasinit preserve=no
      : m_nBorderlength(0), m_n2DNode_length(0), m_n3DNode_length(0), m_nLabelList_length(0), m_nLabelListW(32), m_nLabelListH(32), m_nEdgeList_length(0), m_dSampleGap(0.0), m_dfAvarageOfRatio(0.0), m_dfConvOfRatio(0.0), m_bFirst(TRUE), m_bIsForSpherical(FALSE), m_dSampleingDensity(0.399), m_nSphericalSampling(1), m_dTensionFactor(0.2), m_bQuickIterating(FALSE), m_pCNode2D(NULL)
  //## end CRelxationLabelling::CRelxationLabelling%418197CC0270.hasinit
  //## begin CRelxationLabelling::CRelxationLabelling%418197CC0270.initialization preserve=yes
  //## end CRelxationLabelling::CRelxationLabelling%418197CC0270.initialization
{
  //## begin CRelxationLabelling::CRelxationLabelling%418197CC0270.body preserve=yes
	m_pCNode2D		= NULL;
	m_pCNode3D		= NULL;
    m_pEdgeList		= NULL;
	m_pCLabelList	= NULL;
	m_dfAvarageOfRatio = 0;
	m_dfConvOfRatio = 0;

	m_nTriangleNum = 0;
	/*
	m_pTriangleVertexA = NULL;
	m_pTriangleVertexB = NULL;
	m_pTriangleVertexC = NULL;
	*/
	this->m_pCTriangle    = NULL;
	m_geometryStretch = 0;

	m_nModeType = 0;
  //## end CRelxationLabelling::CRelxationLabelling%418197CC0270.body
}


CRelxationLabelling::~CRelxationLabelling ()
{
  //## begin CRelxationLabelling::~CRelxationLabelling%418197F003C6.body preserve=yes
	if (m_pCNode2D != NULL)
		delete m_pCNode2D;
	if (m_pCNode3D != NULL)
		delete m_pCNode3D;
	if (m_pEdgeList != NULL)
		delete m_pEdgeList;

	if (m_pCLabelList != NULL)
		delete m_pCLabelList;
/*
	if ( m_pTriangleVertexA )
		delete m_pTriangleVertexA;
	if ( m_pTriangleVertexB )
		delete m_pTriangleVertexB;
	if ( m_pTriangleVertexC )
		delete m_pTriangleVertexC;
*/
	if ( m_pCTriangle != NULL )
		delete  m_pCTriangle;
  //## end CRelxationLabelling::~CRelxationLabelling%418197F003C6.body
}



//## Other Operations (implementation)
double CRelxationLabelling::f1 (int i, int li, int j, int lj)
{
  //## begin CRelxationLabelling::f1%41986FD9027F.body preserve=yes
	
	double l, p;
	double p3d_i[3], p3d_j[3];
//	double p2d_i[2], p2d_j[2];
	int    i3d, j3d;

	if ( ((this->m_pCAdjancentMtrx0))[i][j] != 0.0 )
	{
	i3d			= this->m_pCNode2D[i].m_n3DIndex;
	j3d			= this->m_pCNode2D[j].m_n3DIndex;

	p3d_i[0]	= this->m_pCNode3D[i3d].x;
	p3d_i[1]	= this->m_pCNode3D[i3d].y;
	p3d_i[2]	= this->m_pCNode3D[i3d].z;

	p3d_j[0]	= this->m_pCNode3D[j3d].x;
	p3d_j[1]	= this->m_pCNode3D[j3d].y;
	p3d_j[2]	= this->m_pCNode3D[j3d].z;

	l			= (p3d_i[0] - p3d_j[0])*(p3d_i[0] - p3d_j[0]) +
				  (p3d_i[1] - p3d_j[1])*(p3d_i[1] - p3d_j[1]) +
				  (p3d_i[2] - p3d_j[2])*(p3d_i[2] - p3d_j[2]);
//		l			= sqrt (l);
	l			= sqrt (l)*this->m_dTensionFactor;

	double u1 	= this->m_pCLabelList[li].u;
	double u2 	= this->m_pCLabelList[lj].u;
	double		sin_v1,sin_v2,cos_v1,cos_v2;
	sin_v1 = this->m_pCLabelList[li].sin_v;
	sin_v2 = this->m_pCLabelList[lj].sin_v;
	cos_v1 = this->m_pCLabelList[li].cos_v;
	cos_v2 = this->m_pCLabelList[lj].cos_v;
	double dp = sin_v1*sin_v2*cos((u1-u2)*2.0*PI)+cos_v1*cos_v2;
	dp = BOUND(dp,-1.0,1.0);
	ASSERT(fabs(dp)<=1.0);
	p			= acos(dp);
	
	p			= fabs(fabs(p)-l);

	return p;
	}
	else
	 return 0.0;


  //## end CRelxationLabelling::f1%41986FD9027F.body
}

double CRelxationLabelling::P0 (int ai, int li)
{
  //## begin CRelxationLabelling::P0%4174C67A0226.body preserve=yes
	if(this->IsLabelInNodeLabelBox(ai,li))
	{	
		register double fsum = 0.0;
		register int ak;
		int n = this ->m_n2DNode_length;
		for (  ak = 0; ak < n; ak++ )
		{
			if ( ((this->m_pCAdjancentMtrx0))[ai][ak] != 0.0 )
//		if ( this->C(ai,k) != 0.0 )
			{
//			fsum = fsum + f(ai,li , k, this->m_pCNode2D[k].m_n3DIndex);
				fsum = fsum + f(ai, li , ak, this->m_pCNode2D[ak].m_nLabelIndex);
			}
		}

		double p0 = 1.0/(1.0+fsum);

		return p0;
	}
	else
		return 0.0;
  //## end CRelxationLabelling::P0%4174C67A0226.body
}

double CRelxationLabelling::P (int ai, int li)
{
  //## begin CRelxationLabelling::P%417CA4C0007E.body preserve=yes
	double pp;
	if (this->m_pCProbability.m_bInitialized)
		return this->m_pCProbability[ai][li];
	else
	{

	double Psum=0.0;

	int k, m;
	m = m_nLabelList_length;
	for ( k =0; k < m; k++)
	{
 		pp	  = P0(ai,k);
		Psum += pp;
	}

	pp	  = P0(ai,li);

//	Psum = P0(ai,li)/Psum;
	Psum = pp/Psum;
	return Psum;
	 
	}
  //## end CRelxationLabelling::P%417CA4C0007E.body
}

double CRelxationLabelling::r (int ai, int li)
{
  //## begin CRelxationLabelling::r%4174C70902E0.body preserve=yes

	if (this->m_pCR.m_bInitialized == TRUE)
		return this->m_pCR[ai][li];
	else
	{
	double X = 0.0;
	int m = m_nLabelList_length;
	double P1, Q1;

	for (int Lk= 0; Lk < m; Lk++)
	{
//		X += P(ai,Lk)*Q(ai,Lk);

		P1 = P(ai,Lk);
		Q1 = Q(ai,Lk);	
		X += P1*Q1;

	}

	double RaiLi;// = P(ai,li)*(Q(ai,li)-X)/X;
//	RaiLi = P(ai,li)*(Q(ai,li)-X)/X;
	P1 = P(ai,li);
	Q1 = Q(ai,li);
	ASSERT ( X!=0.0);
	RaiLi = P1*(Q1-X)/X;
//	Shittt, X==0!!!
	return RaiLi;
	}
  //## end CRelxationLabelling::r%4174C70902E0.body
}

double CRelxationLabelling::r (int ai, int Li, int ak, int Lk)
{
  //## begin CRelxationLabelling::r%4174CD35011F.body preserve=yes
	double R;
	if ( this->IsBorderNode(ai))
	{
	 if( ai == ak )
		 return 1.0;
	 else
		 return 0.0;
	}
	else
	{
	if ( (ai != ak) && (Li == Lk) )
		R = 0.0;
	else
		if ( ((this->m_pCAdjancentMtrx0))[ai][ak] != 0.0 )
//		if ( this->C(ai,ak) != 0.0 )
		{
			R = 1.0/(1.0+fabs(this->f(ai, Li, ak, Lk)));
		}
		else R = /*1.0*/ 0.0;
	return R;
	}
  //## end CRelxationLabelling::r%4174CD35011F.body
}

double CRelxationLabelling::q (int aj, int ai, int Li)
{
  //## begin CRelxationLabelling::q%417CB73002C0.body preserve=yes


//double CRelxationLabelling::q (int aj, int ai, int Li)
{
	double qsum = 0.0/*,r0*/;
	int m = this->m_nLabelList_length;
	for (int Lj = 0; Lj < m; Lj++)
	{
		if ( this->IsLabelInNodeLabelBox(aj,Lj) )
			qsum += r(ai,Li,aj,Lj)*P(aj,Lj);
	}
	return qsum;
}


  //## end CRelxationLabelling::q%417CB73002C0.body
}

double CRelxationLabelling::Fai ()
{
  //## begin CRelxationLabelling::Fai%417CC46D004C.body preserve=yes


//double CRelxationLabelling::Fai ()
{
  	register double A = 0.0, Cij;
	register int ai,li, aj, lj, m,n;

	double r1,r2,r3;
	m = this->m_nLabelList_length;
	n = this->m_n2DNode_length;
	{
	CEasyProgressBar oCEasyProgressBar("Calculating A ...",n);
		
	for ( ai = 0; ai < n; ai++ )
	{
		for (aj = 0; aj < n; aj++)
		{
			Cij = this->C(ai,aj);
			if (Cij != 0.0 )
			for ( li = 0; li < m; li++)
			{
				r2 = r(ai,li);
				if ( r2 != 0.0 )
				for ( lj = 0; lj < m; lj++)
				{

//					 Cij =((this->m_pCAdjancentMtrx))[ai][aj];
//					 Cij = this->C(ai,aj);
//					 if ( Cij != 0.0 )
//					 {
//						 double r1,r2,r3;
//						A += Cij*r(ai,li,aj,lj)*r(ai,li)*r(aj,lj);
						 r1 = r(ai,li,aj,lj);
//						 r2 = r(ai,li);
						 r3 = r(aj,lj);
						 A += Cij*r1*r2*r3;
//					 }
				}
			}
		}
	oCEasyProgressBar.UpdateProgress();
	}
	}
	A = -A / 2.0;
	
	double M = FLT_MAX;
	{
	CEasyProgressBar oCEasyProgressBar("Calculating M ...",n);
	for ( ai = 0; ai < n; ai++ )
	{
		for ( li = 0; li < m; li++)
		{
			r1 = r(ai,li);
			if (r1 < 0.0)
			{
			  M = min(M, -P(ai,li)/r1);
			}
		}
	oCEasyProgressBar.UpdateProgress();
	}
	}

	if ( A <= 0.0 )
	{
		ASSERT ( M!= FLT_MAX);
		return M;
	}
	else
	{
		CEasyProgressBar oCEasyProgressBar("Calculating B ...",n);
		register double B = 0.0,p1;
		for ( ai = 0; ai < n; ai++ )
		{
			for (aj = 0; aj < n; aj++)
				{
					Cij = this->C(ai,aj);
					if (Cij!=0.0)
					for ( li = 0; li < m; li++)
					{
						p1 = r(ai,li);
						if ( p1 != 0.0 )
						for ( lj = 0; lj < m; lj++)
						{
//						Cij =((this->m_pCAdjancentMtrx))[ai][aj];
//						Cij = this->C(ai,aj);
//						if ( Cij != 0.0 )
//							B += Cij*P(ai,li)*r(ai,li,aj,lj)*r(aj,lj);
							B += /* Cij* */p1*r(ai,li,aj,lj)*P(aj,lj);
						}
					}
				}
			oCEasyProgressBar.UpdateProgress();
		}
		B = -B;
		return min(M,-B/(2.0*A));

	}

}







  //## end CRelxationLabelling::Fai%417CC46D004C.body
}

double CRelxationLabelling::C (int i, int j)
{
  //## begin CRelxationLabelling::C%4189D6220031.body preserve=yes
//	if ( i < m_nBorderlength || j < m_nBorderlength )
//		return 0.0;
//	else
		return this->m_pCAdjancentMtrx0[i][j];
  //## end CRelxationLabelling::C%4189D6220031.body
}

void CRelxationLabelling::Iterating (int nLoop)
{
  //## begin CRelxationLabelling::Iterating%417DEA7B020E.body preserve=yes
	{
		//Step 1 Initial probability
		int m,n;
		int ai,li, c = 0, aj;
		double dFai, dEpsilon = 0.00001, r0;
		int ai1,li1;
		int nChangedNode;
		
		n = this->m_n2DNode_length;  //the number of vertex
		m = this->m_nLabelList_length; // the number of Label
		
		if ( this->m_bFirst )
		{
			this->SetLabelList(	this->m_nLabelListW,
								this->m_nLabelListH,
								TRUE /* Map node to label(sphere)*/);//Prepare label set.
			 
			this->Initial_P();
						
			n = this->m_n2DNode_length;
			m = this->m_nLabelList_length;
			
			for (  ai = 0;  ai < n;  ai++)
				this->m_pCAdjancentMtrx0[ ai][ai] = 1.0;
/*			
			TRACE ( "Cij:\r\n");
			for (  ai = 0 ;  ai < n;  ai++)
			{
				for (  aj = 0 ;  aj < n;  aj++)
				{
					double df = this->m_pCAdjancentMtrx0[ ai][ aj];
					TRACE( "%f ", df);
				}
				TRACE ( "\r\n");
			}
*/			
			if ( !this->m_bQuickIterating)
			{
				this->m_pCR.SetDim(n,m);
				this->m_pCR.m_bInitialized = !TRUE;
				this->m_CQ.SetDim(n,m);
				this->m_CQ.m_bInitialized = !TRUE;
			}
			this->m_pCProbability.m_bInitialized = TRUE;
			this->m_bFirst = FALSE;

			if (nChangedNode = this->SetNode2DLabelings())
				this->SetMesh ();
		}
		else
		{
			double	Rmax = -FLT_MAX;
			do
			{
				if(!this->m_bQuickIterating)
				{
				//Step 2 Calculating incremental
				{
					CEasyProgressBar oCEasyProgressBar("Updating Q[ai][li] ...",n);
					
					this->m_CQ.m_bInitialized = FALSE;
					for ( register int ai1 = 0; ai1 < n; ai1++ )
					{
						for ( register int li1 = 0; li1 < m; li1++ )
							this->m_CQ[ai1][li1] = this->Q(ai1,li1);

						oCEasyProgressBar.UpdateProgress();
					}

					oCEasyProgressBar.~CEasyProgressBar();
				}
				this->m_CQ.m_bInitialized = TRUE;
				{
					CEasyProgressBar oCEasyProgressBar("Updating Labelling and r ...",n);
					this->m_pCR.m_bInitialized = FALSE;
					for ( ai1 = 0; ai1 < n; ai1++ )
					{
						for ( li1 = 0; li1 < m; li1++ )
						{
							//Rmax = max(Rmax, (fabs(this->m_pCR[ai1][li1] = float(this->r(ai1, li1)))));
							r0 = this->r(ai1, li1);
							this->m_pCR[ai1][li1] = r0;
							r0 = this->m_pCR[ai1][li1];
							Rmax = max(Rmax, fabs(r0));
						}
						oCEasyProgressBar.UpdateProgress();
					}
					this->m_pCR.m_bInitialized = TRUE;
					oCEasyProgressBar.~CEasyProgressBar();
				}
				
				//Step 3 Calculating incremental amount
//				dFai = this->Fai();
				dFai = 1.0;
				//Step 4 Updating probabilities
				for ( ai = 0; ai < n; ai++ )
					for ( li = 0; li < m; li++ )
						this->m_pCProbability[ai][li] += float(this->m_pCR[ai][li]*dFai);
				}
			else
				this->Initial_P();
			
			nChangedNode = this->SetNode2DLabelings();
					
			this->SetMesh ();
			c++;
			}
			//	while (((dFai >= dEpsilon)||(Rmax >= dEpsilon))&&(c<nLoop));
			while ((nChangedNode!=0)&&(c<nLoop));
		}
 }
  //## end CRelxationLabelling::Iterating%417DEA7B020E.body
}

CNode3D CRelxationLabelling::Get3DPoint (int index)
{
  //## begin CRelxationLabelling::Get3DPoint%417DEABC028A.body preserve=yes
//	CNode3D CRelxationLabelling::Get3DPoint(int index)
{
	CNode3D p;
	p.x = 0;
	p.y = 0;
	p.z = 0;
	
	ASSERT(index >= 0);

	if(m_pCNode3D !=NULL )
		return m_pCNode3D[index];	
	else
		return p;

}

  //## end CRelxationLabelling::Get3DPoint%417DEABC028A.body
}

double CRelxationLabelling::Stretch (int nV1Index, int nV2Index)
{
  //## begin CRelxationLabelling::Stretch%417DF214008E.body preserve=yes
 
//double CRelxationLabelling::Stretch (int nV1Index, int nV2Index)
{
  

	int j = nV1Index;
	int i = nV2Index;
	
	//compute the stretch ratio of each edge             				
	CNode3D pa, pb;					
	double  a[3], b[3];
	int     ia = 0;
	int     ib = 0;
	
	a[0] = m_pCNode2D[j].u - 0.5 ;
	a[1] = m_pCNode2D[j].v - 0.5;
	a[2] = -0.5;
	ia = m_pCNode2D[j].m_n3DIndex;
	pa = m_pCNode3D[ia];
	
	b[0] = m_pCNode2D[i].u - 0.5;
	b[1] = m_pCNode2D[i].v - 0.5;
	b[2] = -0.5;
	ib = m_pCNode2D[i].m_n3DIndex;
	pb = m_pCNode3D[ib];
	
	float d2_ab = 0;
	float d3_ab = 0;
	
	d2_ab = (float)sqrt((a[0]-b[0])*(a[0]-b[0]) + 
						(a[1]-b[1])*(a[1]-b[1]) +
						(a[2]-b[2])*(a[2]-b[2]) );
	d3_ab = (float)sqrt((pa.x-pb.x)*(pa.x-pb.x) + 
						(pa.y-pb.y)*(pa.y-pb.y) +
						(pa.z-pb.z)*(pa.z-pb.z) );
	
	float ratio = 0;
	
	ratio = (float)Stretch(d2_ab,d3_ab);

	return ratio;
	
}

  //## end CRelxationLabelling::Stretch%417DF214008E.body
}

double CRelxationLabelling::Stretch (double e2D, double e3D)
{
  //## begin CRelxationLabelling::Stretch%417DF299034D.body preserve=yes
	float ratio = 0;
		
	float fE   = float(exp(1));
	float lfx = float(fabs(e3D - e2D));
	ratio = float(( fE - pow(fE, (1/(lfx+1))) )/(fE-1));

	return ratio;
  //## end CRelxationLabelling::Stretch%417DF299034D.body
}

void CRelxationLabelling::SetEdgeList ()
{
  //## begin CRelxationLabelling::SetEdgeList%4180B98E016D.body preserve=yes
//	void CRelxationLabelling::SetEdgeList()
{
	//compute the number of edges according to the adjancent matrix
	int nSizeOfAdjacentMatrix = m_pCAdjancentMtrx0.GetSize();
    
	int i = 0;
	int j = 0;	
	
	if(nSizeOfAdjacentMatrix)
	{
		int nEdgeNum = 0;

		for (j=0; j<nSizeOfAdjacentMatrix; j++)
			for(i=0; i<j; i++)
			{
				if ( m_pCAdjancentMtrx0[j][i] )
					nEdgeNum++;
			}	
			
		m_nEdgeList_length = nEdgeNum;	
		
        //{{Added by xdh,04.12.30
		if(m_pEdgeList  != NULL)
		{
			delete m_pEdgeList ;
			m_pEdgeList = NULL;
		}
		//}}Added by xdh,04.12.30
		
		m_pEdgeList = new CEdge4RL[nEdgeNum];
        
		nEdgeNum = 0;
		for (j=0; j<nSizeOfAdjacentMatrix; j++)
			for(i=0; i<j; i++)
			{
				if ( m_pCAdjancentMtrx0[j][i] )
				{
					m_pEdgeList[nEdgeNum].v1 = j;
					m_pEdgeList[nEdgeNum].v2 = i;

					//compute the stretch ratio of each edge             				
					//float ratio = (float)Stretch(j, i);				
					//m_pEdgeList[nEdgeNum].dStretchMatrics = ratio;

					nEdgeNum++;
				}
			}		
			//
	}
	else
	{
		AfxMessageBox(" No adjancent matrix, constructing Edge list failed! ");
	}

}

  //## end CRelxationLabelling::SetEdgeList%4180B98E016D.body
}

CEdge4RL CRelxationLabelling::GetEdge (int index)
{
  //## begin CRelxationLabelling::GetEdge%4180B9FF02EC.body preserve=yes
//	CEdge4RL CRelxationLabelling::GetEdge(int index)
{
	CEdge4RL e;
	e = m_pEdgeList[index];
	return e;
}
  //## end CRelxationLabelling::GetEdge%4180B9FF02EC.body
}

void CRelxationLabelling::ComputeSampleGap ()
{
  //## begin CRelxationLabelling::ComputeSampleGap%4180BA710372.body preserve=yes
//	void  CRelxationLabelling::ComputeSampleGap()
{	
	
	if (m_nEdgeList_length)
	{
		float minD = 10000000;

		for (int i=0; i<m_nEdgeList_length; i++)
		{
			int va,vb;
			
			va = m_pEdgeList[i].v1;
			vb = m_pEdgeList[i].v2;

			float xa,ya;
			float xb,yb;
			
			xa = float(m_pCNode2D[va].u);
			ya = float(m_pCNode2D[va].v);

			xb = float(m_pCNode2D[vb].u);
			yb = float(m_pCNode2D[vb].v);

			float dx,dy;
			dx = (float)fabs(xa - xb);
			dy = (float)fabs(ya - yb);
			
			float maxD = max(dx, dy);

			if (maxD < minD)
				minD = maxD;
		}
		m_dSampleGap = minD;
	}
	else
	{

	}	
    

}

  //## end CRelxationLabelling::ComputeSampleGap%4180BA710372.body
}

void CRelxationLabelling::ComputeStatisticOfRatio ()
{
  //## begin CRelxationLabelling::ComputeStatisticOfRatio%4180BAA10212.body preserve=yes
//	void CRelxationLabelling::ComputeStatisticOfRatio()
{	
	double lfMean;
	double lfCov;
	
	double lfSum=0.0;
	
	//计算平均值
	for(int i=0; i < m_nEdgeList_length; i++)
	{ 
		int index1 = 0;
		int index2 = 0;
		index1 = m_pEdgeList[i].v1;
		index2 = m_pEdgeList[i].v2;		
		//
		float ratio = (float)Stretch(index1, index2);	
		m_pEdgeList[i].dStretchMatrics = ratio;

		lfSum += m_pEdgeList[i].dStretchMatrics;
	}
	
	lfMean = (lfSum)/(double)m_nEdgeList_length;
	
	lfSum = 0;
	for(i=0; i < m_nEdgeList_length; i++)
	{ 
		lfSum += (m_pEdgeList[i].dStretchMatrics - lfMean)*
			     (m_pEdgeList[i].dStretchMatrics - lfMean);
	}
	
	lfCov = sqrt(lfSum/(m_nEdgeList_length-1));

	m_dfAvarageOfRatio = lfMean;
	m_dfConvOfRatio = lfCov;	
}

	CString averageOfSR;
	averageOfSR.Format("%lf", m_dfAvarageOfRatio);
	CString coviarianceOfSR;
	coviarianceOfSR.Format("%lf", m_dfConvOfRatio);
	CString geoRatio;
	geoRatio.Format("%lf", m_geometryStretch);
    
	/*
    g_CGlobals.SetMesh(0, 
					   NULL, NULL, NULL,
                       NULL, NULL, NULL, 
					   NULL, NULL, NULL,
					   0, NULL, NULL, NULL,
					   0,
					   averageOfSR,
					   coviarianceOfSR,
					   geoRatio);
	*/
	
	//////////////////////////////////////////////////////////////////////////
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	int nCurMeshIndex = pDoc->m_nCurrentMeshIndex;
	pDoc->m_pMesh[nCurMeshIndex]->SetMesh( 0, 
									       NULL, NULL, NULL,
										   NULL, NULL, NULL, 
										   NULL, NULL, NULL,
										   0, NULL, NULL, NULL,
					                       0,
										   averageOfSR,
										   coviarianceOfSR,
										   geoRatio);
	
	//pDoc->m_strTitle[nCurMeshIndex] = pDoc->m_strFileName + " " ;
  //## end CRelxationLabelling::ComputeStatisticOfRatio%4180BAA10212.body
}

double CRelxationLabelling::GetAvarageOfRatio () const
{
  //## begin CRelxationLabelling::GetAvarageOfRatio%4180BAEE034A.body preserve=yes
		return m_dfAvarageOfRatio;
  //## end CRelxationLabelling::GetAvarageOfRatio%4180BAEE034A.body
}

double CRelxationLabelling::GetCovOfRatio () const
{
  //## begin CRelxationLabelling::GetCovOfRatio%4180BB27032E.body preserve=yes
	return m_dfConvOfRatio;
  //## end CRelxationLabelling::GetCovOfRatio%4180BB27032E.body
}

void CRelxationLabelling::Load2DMesh (CString fileName)
{
  //## begin CRelxationLabelling::Load2DMesh%4180BDBB03A5.body preserve=yes
//	void CRelxationLabelling::Load2DMesh(CString fileName)
{

	FILE *fp = NULL;
    fp = fopen(fileName, "r");
	
	if(fp != NULL)
	{		
		int n3DPtsNum = 0;
		int n2DPtsNum = 0;
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
		m_n3DNode_length = n3DPtsNum;
		m_pCNode3D = new CNode3D[n3DPtsNum];
		for (int i=0; i<n3DPtsNum; i++)
		{
			fscanf(fp,"%f %f %f", &gX, &gY, &gZ);
			m_pCNode3D[i].x = gX;
			m_pCNode3D[i].y = gY;
			m_pCNode3D[i].z = gZ;
		}
		//read the 2D mesh points
		fscanf(fp, "%d", &n2DPtsNum);
		m_n2DNode_length = n2DPtsNum;
		m_pCNode2D = new CNode2D[n2DPtsNum];
		for (i=0; i<n2DPtsNum; i++)
		{
			fscanf(fp, "%f %f %d", &x, &y, &index);
			m_pCNode2D[i].u = x;
			m_pCNode2D[i].v = y;
			m_pCNode2D[i].m_n3DIndex = index;			
		}
		//read the triangle index
		fscanf(fp, "%d", &nTriangleNum);
		for (i=0; i<nTriangleNum; i++)
		{
			fscanf(fp, "%d %d %d", &va, &vb, &vc);
		}		
		fclose(fp);
	}	
}


  //## end CRelxationLabelling::Load2DMesh%4180BDBB03A5.body
}

void CRelxationLabelling::SetLabelList (int w, int h, BOOL bNodeAtLabel)
{
  //## begin CRelxationLabelling::SetLabelList%4181B5E40065.body preserve=yes
	if (!m_bIsForSpherical)
	{
		this->SetLabelList0 (w, h);
       
        #ifdef  USING_CHECK_RECT  
        //{{Added by xdh 04,12,29. Construct Rectangle for 2D Mesh 
		int n = this->m_n2DNode_length ;
		for (int i = 0; i <n; i++ )
			this->SetNode2DLabelBox1(i);
		//}}Added by xdh 04,12,29. 
		#endif
	}
	else
	{
		this->SetLabelList1(w, h, this->m_dSampleingDensity);
		
		if (bNodeAtLabel)
			this->InitNode2D4Sphere();
	}
  //## end CRelxationLabelling::SetLabelList%4181B5E40065.body
}

void CRelxationLabelling::SetLabelList0 (int w, int h)
{
  //## begin CRelxationLabelling::SetLabelList0%41996A320013.body preserve=yes
	
	if ( m_pCLabelList != NULL )
		delete m_pCLabelList;

	this->m_nLabelList_length	= w*h; 
	
	this->m_nLabelListW			= w;
	this->m_nLabelListH			= h;
	this->m_pCLabelList			= new CLambda[this->m_nLabelList_length];
	if (((w>1)&&(h>1)))//other wise, only allocate memory, no initialization.
	{
		ASSERT(m_pCLabelList!=NULL);
		
		int k=0;
		for ( int j = 0; j < h; j++ )
			for ( int i = 0; i < w; i ++ )
			{
				this->m_pCLabelList[k].u = (double)i/double(w-1);
				this->m_pCLabelList[k].v = (double)j/double(h-1);

			    #ifdef  USING_CHECK_RECT  
				//{{Added by xdh 04,12,29. Initialization for 2D Mesh
				this->m_pCLabelList[k].x = this->m_pCLabelList[k].u;
				this->m_pCLabelList[k].y = this->m_pCLabelList[k].v;
				this->m_pCLabelList[k].z = 0;
				//}}Added by xdh 04,12,29.
				#endif

				k++;
			}

			int ai, li,n,m, li1;
			double d, dm;
			
			n = this->m_n2DNode_length;
			m = this->m_nLabelList_length;

			for ( ai = 0; ai < n; ai++)
			{
				dm = FLT_MAX;

				for ( li = 0; li < m; li++)
				{
					d = (this->m_pCLabelList[li].u -this->m_pCNode2D[ai].u)*(this->m_pCLabelList[li].u -this->m_pCNode2D[ai].u)
						+
						(this->m_pCLabelList[li].v -this->m_pCNode2D[ai].v)*(this->m_pCLabelList[li].v -this->m_pCNode2D[ai].v);

					d = sqrt (d);
					
					if ( d < dm )
					{
						dm = d;
						li1 = li;
					}
				}

				this->m_pCNode2D[ai].m_nLabelIndex = li1;
			}
	}
  //## end CRelxationLabelling::SetLabelList0%41996A320013.body
}

void CRelxationLabelling::SetLabelList1 (int w, int h, double dDensity)
{
  //## begin CRelxationLabelling::SetLabelList1%41996F2200A6.body preserve=yes
	if (m_nSphericalSampling == 2)
	{
		this->SetLabelFromEvenDistribution(w,h,dDensity);
	}
	else
	{
		int nBw, nBh;
		HANDLE hRawDbl = this->SphereJacobianMap(&w,  &h, dDensity, &nBw, &nBh);

		ASSERT(hRawDbl != NULL);
		
		if (hRawDbl != NULL)
		{
			double* lpRawDbl = (double*)GlobalLock(hRawDbl);
			CDiffusionVarCo CDiffusionVarCo;
			
			CDiffusionVarCo.ImageDiffusion(lpRawDbl,w+2*nBw,h+2*nBh);
			
			HANDLE hRaw = GlobalAlloc(GHND,(w+2*nBw)*(h+2*nBh)*sizeof(BYTE));
			LPBYTE lpRaw = (LPBYTE)GlobalLock(hRaw);
			for (  int i = 0; i < (w+2*nBw)*(h+2*nBh); i++)
				if (i < (w+2*nBw)*(h+2*nBh)/2)
					lpRaw[i] = int(lpRawDbl[(w+2*nBw)*(h+2*nBh) -1- i] + 0.5);
				else
					lpRaw[i] = int(lpRawDbl[i] + 0.5);
				GlobalUnlock(hRaw);
				GlobalUnlock(hRawDbl);GlobalFree(hRawDbl); hRawDbl = NULL;
				
				CDIB CDIB;
				HANDLE hDib = CDIB.Raw8Bw2Dib ( hRaw,  w+2*nBw,h+2*nBh);
				GlobalFree (hRaw); hRaw = NULL;
				
				HANDLE hDib1 = CDIB.CropDIB(hDib,nBw,nBh,nBw+w,nBh+h);
				GlobalFree (hDib); hDib = NULL;
				
				int w1, h1;
				hRaw = CDIB.Dib2BWRaw8(hDib1,&w1,&h1); ASSERT((w1==w)&&(h1==h));
				GlobalFree (hDib1);
				
				lpRaw = (LPBYTE)GlobalLock(hRaw);
				for ( i = 0; i < w; i ++)
					lpRaw[i] = lpRaw[w*h-1 - i] = 0;
				lpRaw[w/2] = lpRaw[w*h-w/2] = 255;
				GlobalUnlock(hRaw);

				this->SetLabelFromSphericalRaw(hRaw,w,h);
		}
	}
  //## end CRelxationLabelling::SetLabelList1%41996F2200A6.body
}

CLambda CRelxationLabelling::GetLabel (int nIndex)
{
  //## begin CRelxationLabelling::GetLabel%4181B62003C8.body preserve=yes
	return this->m_pCLabelList[nIndex];
  //## end CRelxationLabelling::GetLabel%4181B62003C8.body
}

void CRelxationLabelling::SetTriangleVertexIndex (int a, int b, int c, int index)
{
  //## begin CRelxationLabelling::SetTriangleVertexIndex%419AE6960046.body preserve=yes
//void CRelxationLabelling::SetTriangleVertexIndex(int a, int b, int c, int index)
{

    ASSERT( index < m_nTriangleNum );
/*	
	m_pTriangleVertexA[index] = a;
	m_pTriangleVertexB[index] = b;
	m_pTriangleVertexC[index] = c;
*/
	m_pCTriangle[index].A = a;
	m_pCTriangle[index].B = b;	
	m_pCTriangle[index].C = c;


}
  //## end CRelxationLabelling::SetTriangleVertexIndex%419AE6960046.body
}

void CRelxationLabelling::Initial_P ()
{
  //## begin CRelxationLabelling::Initial_P%4181E8FA024F.body preserve=yes
	int n,m;
	double dp0;
	
	n = this->m_n2DNode_length ;
	m = this->m_nLabelList_length;

	this->m_pCProbability.SetDim(n, m);
	
	CEasyProgressBar oCEasyProgressBar("Initiating P...",n);

	for ( int ai = 0; ai < n; ai++)
	{ 
		if ( this->IsBorderNode(ai) )
		{
			for ( int li = 0; li < m; li++)
			{
				if ( li == this->m_pCNode2D[ai].m_nLabelIndex)
					this->m_pCProbability[ai][li] = 1.0;
				else
					this->m_pCProbability[ai][li] = 0.0;
			}
		}
		else
		{
#define ACCP			
#ifndef ACCP			
				for ( int li = 0; li < m; li++)
				{
					dp0 = this->P(ai,li);
					
					ASSERT( dp0>=0.0 && dp0<=1.0 );
					
					this->m_pCProbability[ai][li] = dp0;
					dp0 = this->m_pCProbability[ai][li];
				}
#else
				double dPSum = 0.0;
				for ( int li = 0; li < m; li++)
				{
					dp0 = this->P0(ai,li);
					dPSum += dp0;
//					ASSERT( dp0>=0.0 && dp0<=1.0 );
					this->m_pCProbability[ai][li] = dp0;

				}

				ASSERT( dPSum != 0.0 );

				for ( li = 0; li < m; li++)
				{
					dp0 = this->m_pCProbability[ai][li] / dPSum;
					ASSERT( dp0>=0.0 && dp0<=1.0 );
				}


#endif

		}
				
		oCEasyProgressBar.UpdateProgress();
	}	

  //## end CRelxationLabelling::Initial_P%4181E8FA024F.body
}

void CRelxationLabelling::UpdateCR ()
{
  //## begin CRelxationLabelling::UpdateCR%4181FA2303B5.body preserve=yes
   int n,m;
	n = this->m_n2DNode_length;
	m = this->m_nLabelList_length;
	this->m_pCR.SetDim(n, m);
	for ( int ai = 0; ai < n; ai++)
		for ( int li = 0; li < m; li++)
			this->m_pCR[ai][li] = this->r(ai,li);

  //## end CRelxationLabelling::UpdateCR%4181FA2303B5.body
}

void CRelxationLabelling::UpDate_P (double dblFai)
{
  //## begin CRelxationLabelling::UpDate_P%418208CE01D3.body preserve=yes
	int n,m;
	n = this->m_n2DNode_length;
	m = this->m_nLabelList_length;

	for ( int ai = 0; ai < n; ai++)
		for ( int li = 0; li < m; li++)
			this->m_pCProbability[ai][li] += dblFai*this->r(ai,li);

  //## end CRelxationLabelling::UpDate_P%418208CE01D3.body
}

int CRelxationLabelling::MaxPossibleLable (int ai, double* dblProbability)
{
  //## begin CRelxationLabelling::MaxPossibleLable%4181FD750050.body preserve=yes
	int n,m, nLambda = 0;
	n = this->m_n2DNode_length;
	m = this->m_nLabelList_length;
//	double dP = this->m_pCProbability[ai][0];
	double dP = -FLT_MAX;
	double dP1, pSum = 0.0;
	for ( int li = 0; li < m; li++)
	{
		dP1 = this->m_pCProbability[ai][li];
		if( dP1 > dP)
		{
			 dP = this->m_pCProbability[ai][li];
			 nLambda = li;
		}
		pSum += dP1;
// 		ASSERT(dP1>=0.0&& dP1<=1.0);
//		ASSERT(dP1>=-FLT_MIN&& dP1<=1.0);
		ASSERT(dP1>=-1.0&& dP1<=1.0);
	}
//	ASSERT(pSum==1.0);
	*dblProbability		= dP;
	return nLambda;
  //## end CRelxationLabelling::MaxPossibleLable%4181FD750050.body
}

int CRelxationLabelling::SetNode2DLabelings ()
{
  //## begin CRelxationLabelling::SetNode2DLabelings%4182011A0278.body preserve=yes
 
   	int n,m, li,c;
	double pp;
	n = this->m_n2DNode_length;
	m = this->m_nLabelList_length;

	c = 0;
	for ( int ai = 0; ai < n; ai++)
	{
		li = this->MaxPossibleLable(ai,&pp);
		
		if ( this->m_pCNode2D[ai].m_nLabelIndex != li)
		{
			this->m_pCNode2D[ai].m_nLabelIndex = li;
			
			this->m_pCNode2D[ai].u = this->m_pCLabelList[li].u;
			this->m_pCNode2D[ai].v = this->m_pCLabelList[li].v;

			this->m_pCNode2D[ai].x = this->m_pCLabelList[li].x;
			this->m_pCNode2D[ai].y = this->m_pCLabelList[li].y;
			this->m_pCNode2D[ai].z = this->m_pCLabelList[li].z;
			
			c++;
		}		
	}
		
	if(m_nModeType == 1)
	{		
		for ( ai = 0; ai < n; ai++)
		{
			this->SetNode2DLabelBox(ai);
		}
	}
    #ifdef  USING_CHECK_RECT  
	//{{Added by xdh,04,12,29. Refresh 2D Mesh 
	else
	{		
		for ( ai = 0; ai < n; ai++)
		{
			this->SetNode2DLabelBox1(ai);
		}		
	}//}}Added by xdh,04,12,29. 
    #endif

	return c;
  //## end CRelxationLabelling::SetNode2DLabelings%4182011A0278.body
}

void CRelxationLabelling::GetMesh (int* nTriangle, int** nV1, int** nV2, int** nV3, int** c12, int** c23, int** c31, int* nVlength, double** x, double** y, double** z)
{
  //## begin CRelxationLabelling::GetMesh%41873CAF0277.body preserve=yes
//nTriangle

  //## end CRelxationLabelling::GetMesh%41873CAF0277.body
}

void CRelxationLabelling::SetMesh ()
{
  //## begin CRelxationLabelling::SetMesh%41884AB1011C.body preserve=yes
//void CRelxationLabelling::SetMesh()
{
	
	if(m_nModeType == 1)
	{
		SetMesh1(); //set 3D Mesh
	}
	else
	{
		SetMesh0(); //set 2D Mesh
	}	
}

  //## end CRelxationLabelling::SetMesh%41884AB1011C.body
}

void CRelxationLabelling::SetMesh0 ()
{
  //## begin CRelxationLabelling::SetMesh0%4199A33A0321.body preserve=yes
	
	//first ,retrieve the original mesh infor about triangle	
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	//int    nTriangleNum; 
	//int    *v1,*v2,*v3;
	int    nEdgeNodeNum;

	((COpenGL3DEx *)pDoc->m_pCOpenGL)->m_pCMesh->GetMesh(
		//&nTriangleNum, 
		//&v1, &v2, &v3,
		NULL,NULL,NULL,NULL,
		NULL,NULL,NULL,
		NULL,NULL,NULL,NULL,
		&nEdgeNodeNum);

	//add the lable points
	int nLableNum = m_nLabelList_length;
	CLabelPts labelPts;	    
	for(int i=0 ; i<nLableNum; i++)
	{
		double u = m_pCLabelList[i].u ;// * 2 * PI;
		double v = m_pCLabelList[i].v ;//* PI;

		POINT3D p;
		p.x = (u - 0.5)*2;
		p.y = (v - 0.5)*2;
		p.z = -1;

		labelPts.AddPt(p);			
	}

	//the new 2d mesh points coordinates
    double *x = new double[m_n2DNode_length ];//+ nLableNum];
	double *y = new double[m_n2DNode_length ];//+ nLableNum];
	double *z = new double[m_n2DNode_length ];//+ nLableNum];

	for (i=0; i<m_n2DNode_length; i++)
	{
		x[i] = (m_pCNode2D[i].u );
		y[i] = (m_pCNode2D[i].v );
		z[i] = 0;
	}

   	//compute the new color:	
    COLORREF *c12 = new COLORREF[m_nTriangleNum ];//+ nLableNum];
	COLORREF *c31 = new COLORREF[m_nTriangleNum ];//+ nLableNum];
	COLORREF *c23 = new COLORREF[m_nTriangleNum ];//+ nLableNum];

	double *ratio12 = new double[m_nTriangleNum ];//+ nLableNum];
	double *ratio23 = new double[m_nTriangleNum ];//+ nLableNum];
	double *ratio31 = new double[m_nTriangleNum ];//+ nLableNum];

	for ( i=0; i<m_nTriangleNum; i++ )
	{		
		int ia;
		int ib;
		int ic;
		
		int v1,v2,v3;
		v1 = m_pCTriangle[i].A;
		v2 = m_pCTriangle[i].B;
		v3 = m_pCTriangle[i].C;

		CNode3D pa, pb, pc;
		
		float a[3],b[3],c[3];
		a[0] = (float)( m_pCNode2D[ v1 ].u ) ;
		a[1] = (float)( m_pCNode2D[ v1 ].v );
		a[2] = 0;
		ia = m_pCNode2D[ v1 ].m_n3DIndex;
		pa = Get3DPoint(ia);
		
		b[0] = (float)( m_pCNode2D[ v2 ].u );
		b[1] = (float)( m_pCNode2D[ v2 ].v );
		b[2] = 0;
		ib = m_pCNode2D[ v2 ].m_n3DIndex;
		pb = Get3DPoint(ib);
		
		c[0] = (float)( m_pCNode2D[ v3 ].u );
		c[1] = (float)( m_pCNode2D[ v3 ].v );
		c[2] = 0 ;
		ic = m_pCNode2D[ v3 ].m_n3DIndex;
		pc = Get3DPoint(ic);
		
		float d2_ab = 0;
		float d3_ab = 0;
		
		float d2_ac = 0;
		float d3_ac = 0;
		
		float d2_bc = 0;
		float d3_bc = 0;
		
		d2_ab = (float)sqrt( (a[0]-b[0])*(a[0]-b[0]) + 
			(a[1]-b[1])*(a[1]-b[1]) +
			(a[2]-b[2])*(a[2]-b[2]) );
		d3_ab = (float)sqrt( (pa.x-pb.x)*(pa.x-pb.x) + 
			(pa.y-pb.y)*(pa.y-pb.y) +
			(pa.z-pb.z)*(pa.z-pb.z) );
		
		d2_ac = (float)sqrt( (a[0]-c[0])*(a[0]-c[0]) + 
			(a[1]-c[1])*(a[1]-c[1]) +
			(a[2]-c[2])*(a[2]-c[2]) );
		d3_ac = (float)sqrt( (pa.x-pc.x)*(pa.x-pc.x) + 
		          (pa.y-pc.y)*(pa.y-pc.y) +
				  (pa.z-pc.z)*(pa.z-pc.z) );
		
		d2_bc = (float)sqrt( (c[0]-b[0])*(c[0]-b[0]) + 
			(c[1]-b[1])*(c[1]-b[1]) +
			(c[2]-b[2])*(c[2]-b[2]) );
		d3_bc = (float)sqrt( (pb.x-pc.x)*(pb.x-pc.x) + 
			(pb.y-pc.y)*(pb.y-pc.y) +
			(pb.z-pc.z)*(pb.z-pc.z) );
		
		float ratio = 0;
		float cr,cg,cb;
		int   nr,ng,nb;
		
		ratio = ComputeStretchRatio(d3_ab, d2_ab);
		int H = 240-(int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		c12[i] = RGB(nr, ng, nb);
		ratio12[i] = d3_ab / d2_ab;
		
		ratio = ComputeStretchRatio(d3_ac, d2_ac);
		H = 240 - (int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		c31[i] = RGB(nr, ng, nb);
		ratio31[i] = d3_ac / d2_ac;
		
		ratio = ComputeStretchRatio(d3_bc, d2_bc);
		H = 240 - (int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		c23[i] = RGB(nr, ng, nb);
		ratio23[i] = d3_bc / d2_bc;
	}

	int *nv1 = new int[m_nTriangleNum ];//+ nLableNum];
	int *nv2 = new int[m_nTriangleNum ];//+ nLableNum];
	int *nv3 = new int[m_nTriangleNum ];//+ nLableNum];
	for (i=0; i<m_nTriangleNum; i++)
	{
		nv1[i] = m_pCTriangle[i].A;
		nv2[i] = m_pCTriangle[i].B;
		nv3[i] = m_pCTriangle[i].C;
	}
	
	//recompute the static data
	ComputeSampleGap();
	ComputeStatisticOfRatio();
	ComputeGeoStretch();

	CString averageOfSR;
	averageOfSR.Format("%lf", m_dfAvarageOfRatio);
	CString coviarianceOfSR;
	coviarianceOfSR.Format("%lf", m_dfConvOfRatio);
    CString geoRatio;
	geoRatio.Format("%lf", m_geometryStretch);

	/*
	//////////////////////////////////////////////////////////////////////////
	g_CGlobals.SetMesh(m_nTriangleNum + nLableNum , 
					   nv1, nv2, nv3,
                       c12, c23, c31, 
					   ratio12, ratio23, ratio31,
					   m_n2DNode_length + nLableNum, x, y, z,
					   nEdgeNodeNum,
					   averageOfSR,
					   coviarianceOfSR,
					   geoRatio);   

	//////////////////////////////////////////////////////////////////////////
	*/

	//////////////////////////////////////////////////////////////////////////
	int nCurMeshIndex = pDoc->m_nCurrentMeshIndex;

	if(pDoc->m_pMesh[nCurMeshIndex] != NULL)
		delete pDoc->m_pMesh[nCurMeshIndex];
	pDoc->m_pMesh[nCurMeshIndex] = new C2DMesh;

	pDoc->m_pMesh[nCurMeshIndex]->SetMesh( m_nTriangleNum , 
										   nv1, nv2, nv3,
										   c12, c23, c31, 
										   ratio12, ratio23, ratio31,
										   m_n2DNode_length, 
										   x, y, z,	
										   nEdgeNodeNum,
										   averageOfSR,
										   coviarianceOfSR,
										   geoRatio);
    //set the label object of CMesh
	pDoc->m_pMesh[nCurMeshIndex]->SetLabelPts(labelPts);
	
	//set statistics
	pDoc->m_struStatistic[nCurMeshIndex].strAverageOfStretchRatio = averageOfSR;
	pDoc->m_struStatistic[nCurMeshIndex].strCoviarianceOfStretchRatio = coviarianceOfSR;
	pDoc->m_struStatistic[nCurMeshIndex].strGeoRatio = geoRatio;

	pDoc->m_strTitle[nCurMeshIndex] = pDoc->m_strFileName + " " ;
	
   	if(m_nTriangleNum)
	{
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh = ( pDoc->m_pMesh[nCurMeshIndex] ); 
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->m_nType = 1; 
		
		pDoc->m_nCurrentMeshIndex =  (pDoc->m_nCurrentMeshIndex + 1) % 2;
		pDoc->m_strTitle[pDoc->m_nCurrentMeshIndex] = pDoc->m_strFileName + "*";
		
		pDoc->UpdateView();
	}
	//////////////////////////////////////////////////////////////////////////
	

	//release the mem
	delete []x;
	delete []y;
	delete []z;

	delete []c12;
	delete []c31;
	delete []c23;

	delete []ratio12;
	delete []ratio23;
	delete []ratio31;

	delete []nv1;
	delete []nv2;
	delete []nv3;

  //## end CRelxationLabelling::SetMesh0%4199A33A0321.body
}

void CRelxationLabelling::SetMesh1 ()
{
  //## begin CRelxationLabelling::SetMesh1%4199A34503D1.body preserve=yes
//void CRelxationLabelling::SetMesh1 ()
{
	//first ,retrieve the original mesh infor about triangle	
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	int    nEdgeNodeNum = 0;
	
	//add label points
	CLabelPts labelPts;	    
	int nLableNum = m_nLabelList_length;
	for(int i=0 ; i<nLableNum; i++)
	{
		double u = m_pCLabelList[i].u * 2 * PI;
		double v = m_pCLabelList[i].v * PI;
        			
		//insert point into label object
		POINT3D p;
		if(v == 0)
		{
			p.x = 0;
			p.y = 0;
			p.z = 1;
		}
		else
		{
			p.x = cos(u)*sin(v);
			p.y = sin(u)*sin(v);
			p.z = cos(v);
		}

		labelPts.AddPt(p);			
	}
    
	//the new 2d mesh points coordinates
    double *x = new double[m_n2DNode_length];// + nLableNum];
	double *y = new double[m_n2DNode_length];// + nLableNum];
	double *z = new double[m_n2DNode_length];// + nLableNum];
	for ( i=0; i<m_n2DNode_length; i++)
	{		
		double u = m_pCNode2D[i].u * 2 * PI;
		double v = m_pCNode2D[i].v * PI;

		//convert the 2D Mesh to sphere coordinate
		if(v == 0)
		{
			x[i] = 0;
			y[i] = 0;
			z[i] = 1;
		}
		else if( fabs(v-PI) < 0.0000001 )
		{		
			x[i] = 0;
			y[i] = 0;
			z[i] = -1;
		}
		else
		{
			x[i] = cos(u)*sin(v);
			y[i] = sin(u)*sin(v);
			z[i] = cos(v);
		}
	}
   	//compute the new color:
    COLORREF *c12 = new COLORREF[m_nTriangleNum ];//+ nLableNum];
	COLORREF *c31 = new COLORREF[m_nTriangleNum ];//+ nLableNum];
	COLORREF *c23 = new COLORREF[m_nTriangleNum ];//+ nLableNum];

	double *ratio12 = new double[m_nTriangleNum ];//+ nLableNum];
	double *ratio23 = new double[m_nTriangleNum ];//+ nLableNum];
	double *ratio31 = new double[m_nTriangleNum ];//+ nLableNum];

	for ( i=0; i<m_nTriangleNum; i++ )
	{		
		int ia;
		int ib;
		int ic;
		
		CNode3D pa, pb, pc;
		
     	//for geodesic distance
		double a[2],b[2],c[2];
		a[0] = ( m_pCNode2D[ m_pCTriangle[i].A ].u ) * 2 * PI ;
		a[1] = ( m_pCNode2D[ m_pCTriangle[i].A ].v ) * PI;
		ia = m_pCNode2D[ m_pCTriangle[i].A ].m_n3DIndex;
		pa = Get3DPoint(ia);
		
		b[0] = (float)( m_pCNode2D[ m_pCTriangle[i].B ].u ) * 2 * PI;
		b[1] = (float)( m_pCNode2D[ m_pCTriangle[i].B ].v ) * PI;
		ib = m_pCNode2D[ m_pCTriangle[i].B ].m_n3DIndex;
		pb = Get3DPoint(ib);
		
		c[0] = (float)( m_pCNode2D[ m_pCTriangle[i].C ].u ) * 2 * PI ;
		c[1] = (float)( m_pCNode2D[ m_pCTriangle[i].C ].v ) * PI;
		ic = m_pCNode2D[ m_pCTriangle[i].C ].m_n3DIndex;
		pc = Get3DPoint(ic);
		
		
		float d2_ab = 0;
		float d3_ab = 0;
		
		float d2_ac = 0;
		float d3_ac = 0;
		
		float d2_bc = 0;
		float d3_bc = 0;
		
		d2_ab = (float)GeodesicDistance(a[0], a[1], b[0], b[1]);
		d3_ab = (float)sqrt( (pa.x-pb.x)*(pa.x-pb.x) + 
			(pa.y-pb.y)*(pa.y-pb.y) +
			(pa.z-pb.z)*(pa.z-pb.z) );
		
		d2_ac = (float)GeodesicDistance(a[0], a[1], c[0], c[1]);         
		d3_ac = (float)sqrt( (pa.x-pc.x)*(pa.x-pc.x) + 
		          (pa.y-pc.y)*(pa.y-pc.y) +
				  (pa.z-pc.z)*(pa.z-pc.z) );
		
		d2_bc = (float)GeodesicDistance(b[0], b[1], c[0], c[1]);
		d3_bc = (float)sqrt( (pb.x-pc.x)*(pb.x-pc.x) + 
			(pb.y-pc.y)*(pb.y-pc.y) +
			(pb.z-pc.z)*(pb.z-pc.z) );
		
		float ratio = 0;
		float cr,cg,cb;
		int   nr,ng,nb;
		
		ratio = ComputeStretchRatio(d3_ab, d2_ab);
		int H = 240-(int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		c12[i] = RGB(nr, ng, nb);
		ratio12[i] = d3_ab / d2_ab;
		
		ratio = ComputeStretchRatio(d3_ac, d2_ac);
		H = 240 - (int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		c31[i] = RGB(nr, ng, nb);
		ratio31[i] = d3_ac / d2_ac;
		
		ratio = ComputeStretchRatio(d3_bc, d2_bc);
		H = 240 - (int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		c23[i] = RGB(nr, ng, nb);
		ratio23[i] = d3_bc / d2_bc;
	}
	
	int *nv1 = new int[m_nTriangleNum ]; //+ nLableNum];
	int *nv2 = new int[m_nTriangleNum ]; //+ nLableNum];
	int *nv3 = new int[m_nTriangleNum ]; //+ nLableNum];

	for (i=0; i<m_nTriangleNum; i++)
	{
		nv1[i] = m_pCTriangle[i].A;
		nv2[i] = m_pCTriangle[i].B;
		nv3[i] = m_pCTriangle[i].C;
	}
   
	Compute3DStatisticOfRation();

	CString averageOfSR;
	averageOfSR.Format("%lf", m_dfAvarageOfRatio);
	CString coviarianceOfSR;
	coviarianceOfSR.Format("%lf", m_dfConvOfRatio);
    CString geoRatio;
	geoRatio.Format("%lf", 0.0);

	//////////////////////////////////////////////////////////////////////////
	int nCurMeshIndex = pDoc->m_nCurrentMeshIndex;
	pDoc->m_pMesh[nCurMeshIndex]->SetMesh( m_nTriangleNum , 
										   nv1, nv2, nv3,
										   c12, c23, c31, 
										   ratio12, ratio23, ratio31,
										   m_n2DNode_length , 
										   x, y, z,	
										   nEdgeNodeNum,
										   averageOfSR,
										   coviarianceOfSR,
										   geoRatio);
    //set the label object of CMesh
	pDoc->m_pMesh[nCurMeshIndex]->SetLabelPts(labelPts);
    
	//set statistics
	pDoc->m_struStatistic[nCurMeshIndex].strAverageOfStretchRatio = averageOfSR;
	pDoc->m_struStatistic[nCurMeshIndex].strCoviarianceOfStretchRatio = coviarianceOfSR;
	pDoc->m_struStatistic[nCurMeshIndex].strGeoRatio = geoRatio;

	
	pDoc->m_strTitle[nCurMeshIndex] = pDoc->m_strFileName + " " ;
	
   	if(m_nTriangleNum)
	{
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh = ( pDoc->m_pMesh[nCurMeshIndex] ); 
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->m_nType = 2; 
		
		pDoc->m_nCurrentMeshIndex =  (pDoc->m_nCurrentMeshIndex + 1) % 2;
		pDoc->m_strTitle[pDoc->m_nCurrentMeshIndex] = pDoc->m_strFileName + "*";
		
		pDoc->UpdateView();
	}
	//////////////////////////////////////////////////////////////////////////
	
	//release the mem
	delete []x;
	delete []y;
	delete []z;

	delete []c12;
	delete []c31;
	delete []c23;

	delete []ratio12;
	delete []ratio23;
	delete []ratio31;

	delete []nv1;
	delete []nv2;
	delete []nv3;	

}
  //## end CRelxationLabelling::SetMesh1%4199A34503D1.body
}

bool CRelxationLabelling::IsBorderNode (int ai)
{
  //## begin CRelxationLabelling::IsBorderNode%4189EAC90123.body preserve=yes
	return ai < this->m_nBorderlength;
  //## end CRelxationLabelling::IsBorderNode%4189EAC90123.body
}

void CRelxationLabelling::SetBorderLen (int nBorderLen)
{
  //## begin CRelxationLabelling::SetBorderLen%418ACDAB033D.body preserve=yes
//	void CRelxationLabelling::SetBorderLen(int nBorderLen)
{
	m_nBorderlength = nBorderLen;	
}

  //## end CRelxationLabelling::SetBorderLen%418ACDAB033D.body
}

HANDLE CRelxationLabelling::SphereJacobianMap (int* pW, int* pH, double dScale, int* nBorderW, int* nBorderH)
{
  //## begin CRelxationLabelling::SphereJacobianMap%41945FA403CE.body preserve=yes
	int w,h;
	int nBw=0, nBh=0;
//	double dx,dy,dz;

	w = this->m_nLabelListW;
	h = this->m_nLabelListH;
	if (nBorderW!=NULL)
	{
		nBw= 64;
		nBh= 64;
	}

	HANDLE hRawDbl;
	int i,j,k;

	hRawDbl = GlobalAlloc(GHND,(w+nBw*2)*(h+nBh*2)*sizeof(double));

//	LPBYTE lpRaw;
	double* lpRawDbl;

	lpRawDbl = (double*)GlobalLock(hRawDbl);
	k = 0;
	for ( int j1 = 0-nBh; j1 < h+nBh; j1++ )
		for ( int i1 = 0-nBw; i1 < w+nBw; i1++)
		{
			j = j1;
			i = i1;
//			lpRaw[k] = BYTE(dScale*fabs(sin ( double(j)/double(h-1)*3.1415926))*255.0);
//			lpRawDbl[k] = (dScale*pow(fabs(sin ( double(j)/double(h-1)*3.1415926)),0.382)*255.0);
			lpRawDbl[k] = (dScale*(fabs(sin ( double(j)/double(h-1)*3.1415926)))*255.0);

			k++;
		}
	GlobalUnlock(hRawDbl);
	*pH = h; *pW = w;
	if (nBorderW!=NULL)
	{
		*nBorderW = nBw;
		*nBorderH = nBh;
	}

	return hRawDbl;
  //## end CRelxationLabelling::SphereJacobianMap%41945FA403CE.body
}

void CRelxationLabelling::SetLabelFromSphericalRaw (HANDLE hRaw, int w, int h, int* nPtCount, double** px, double** py, double** pz)
{
  //## begin CRelxationLabelling::SetLabelFromSphericalRaw%4196C08E025C.body preserve=yes
	if ( hRaw != NULL )
	{
	LPBYTE lpRaw = (LPBYTE)GlobalLock(hRaw);
	int k,l;
	double u,v,*x,*y,*z;
	ASSERT (lpRaw!=NULL);

	this->m_nLabelList_length = 0;

	for ( k = 0; k < w*h; k++ )
		if (lpRaw[k]>128)
			this->m_nLabelList_length++;

	this->SetLabelList0(this->m_nLabelList_length, 1);

	if ( pz != NULL )
	{
		x = new double[this->m_nLabelList_length];
		y = new double[this->m_nLabelList_length];
		z = new double[this->m_nLabelList_length];
	}
	
	k = 0;
	l = 0;
	for ( int j = 0; j < h; j++ )
		for ( int i = 0; i < w; i++ )
		{
//			if ( i < w/2 )
			{
			k = j*w+i;
			if (lpRaw[k]>128)
			{
				u = (double)i/double(w/*-1*/);
				v = (double)j/double(h/*-1*/);		

//#define LieDown
#ifdef LieDown
				u = u*2.0*PI;
				v = v*PI;
				double x0,y0,z0,x1,y1,z1, r;
				x0 = cos(u)*sin(v);
				y0 = sin(u)*sin(v);
				z0 = cos(v);
				y1 = z0; z1 = y0; x1 = x0;//SWAP(y1,z1);
//				y1 = y0; z1 = z0; x1 = x0;//SWAP(y1,z1);
				r = sqrt (x1*x1+y1*y1+z1*z1);
				u = atan2(y1,x1)/(2.0*PI);
				v = acos(z1/r)/PI;
#endif
				this->m_pCLabelList[l].u = u;
				this->m_pCLabelList[l].v = v;
				
				u = u*2.0*PI;
				v = v*PI;
				
				this->m_pCLabelList[l].sin_u = sin(u);
				this->m_pCLabelList[l].sin_v = sin(v);
				this->m_pCLabelList[l].cos_u = cos(u);
				this->m_pCLabelList[l].cos_v = cos(v);

				this->m_pCLabelList[l].x	 = cos(u)*sin(v);
				this->m_pCLabelList[l].y	 = sin(u)*sin(v);
				this->m_pCLabelList[l].z	 = cos(v);
				
				if ( pz != NULL )
				{
//					u = u*2.0*PI;
//					v = v*PI;
					x[l] = this->m_pCLabelList[l].x;
					y[l] = this->m_pCLabelList[l].y;
					z[l] = this->m_pCLabelList[l].z;
				}
				l++;
			}
			}
		}
	m_nLabelList_length = l;

	if ( pz != NULL )
	{
		*nPtCount = m_nLabelList_length;
		*px = x;
		*py = y;
		*pz = z;
	}
	}
	else
		SetLabelFromEvenDistribution(w,h,this->m_dSampleingDensity,nPtCount,px,py,pz);
  //## end CRelxationLabelling::SetLabelFromSphericalRaw%4196C08E025C.body
}

void CRelxationLabelling::SetLabelFromEvenDistribution (int w, int h, double dSampleDensity, int* nPtCount, double** px, double** py, double** pz)
{
  //## begin CRelxationLabelling::SetLabelFromEvenDistribution%41A2AF700119.body preserve=yes
 	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	///Createded by fuyan, 2004.11.22{{

	int nSample;
	int i;
	double t;
	double theta,phi,lastTheta;

//	w = m_nParamSpaceWidth;
//	h = m_nParamSpaceHeight;
	

//	nSample = w * h * this->m_dSampleDensity;
	nSample = int(w * h * dSampleDensity);
	if ( nPtCount != NULL)
		*nPtCount = nSample;

	this->m_nLabelList_length = nSample;

	if (this->m_pCLabelList!=NULL)
		delete this->m_pCLabelList;
	this->m_pCLabelList = new CLambda[this->m_nLabelList_length];
		
	
 	for(i = 0; i < nSample; i++)
	{ 
		t = -1 + (2*i)/double(nSample-1);
		phi = acos(t);
		if(i==0||i==nSample-1)
			theta =0;
		else
		{
			theta = lastTheta+3.6/sqrt(nSample*(1-t*t));
			theta = theta - int(theta/(2*PI)) * 2*PI;
		}
		lastTheta = theta;
		this->m_pCLabelList[i].u = theta/(2*PI);
		this->m_pCLabelList[i].v = phi/PI;
	}

	int nVlength;
	double *x,*y,*z;
	nVlength = this->m_nLabelList_length;

	if ( pz != NULL )
	{
		x = new double[nVlength];
		y = new double[nVlength];
		z = new double[nVlength];
	}
	
	double tempsin_u,tempcos_u,tempsin_v,tempcos_v;
	for(i=0; i < nVlength; i++)
	{ 
		tempsin_u = sin(this->m_pCLabelList[i].u * 2 *PI);
		tempsin_v = sin(this->m_pCLabelList[i].v * PI);
		tempcos_u = cos(this->m_pCLabelList[i].u * 2 * PI);
		tempcos_v = cos(this->m_pCLabelList[i].v * PI);
		this->m_pCLabelList[i].sin_u = tempsin_u;
		this->m_pCLabelList[i].cos_u = tempcos_u;
		this->m_pCLabelList[i].sin_v = tempsin_v;
		this->m_pCLabelList[i].cos_v = tempcos_v;
		this->m_pCLabelList[i].x	 = tempcos_u * tempsin_v;
		this->m_pCLabelList[i].y	 = tempsin_u * tempsin_v;
		this->m_pCLabelList[i].z	 = tempcos_v;

		if ( pz != NULL )
		{
			x[i] = this->m_pCLabelList[i].x;
			y[i] = this->m_pCLabelList[i].y;
			z[i] = this->m_pCLabelList[i].z;
		}
	}

	if ( pz != NULL )
	{
		*px = x;
		*py = y;
		*pz = z;
	}

	//end}}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	


  //## end CRelxationLabelling::SetLabelFromEvenDistribution%41A2AF700119.body
}

void CRelxationLabelling::SetTriangleNum (int nNum)
{
  //## begin CRelxationLabelling::SetTriangleNum%419AEC1C0193.body preserve=yes

//void CRelxationLabelling::SetTriangleNum(int nNum)
{

/*	if ( m_pTriangleVertexA != NULL )
		delete m_pTriangleVertexA;
	if ( m_pTriangleVertexB != NULL)
		delete m_pTriangleVertexB;
	if ( m_pTriangleVertexC != NULL)
		delete m_pTriangleVertexC;
		*/
	/*
	m_pTriangleVertexA = new int[nNum];
	m_pTriangleVertexB = new int[nNum];
	m_pTriangleVertexC = new int[nNum];	
*/
	//{{Added by xdh,04.12.30. 
	if(m_pCTriangle != NULL)
	{ 
		delete m_pCTriangle;
		m_pCTriangle = NULL;
	}
	//}}Added by xdh,04.12.30. 

	m_pCTriangle  = new CTriangleRL[nNum];
	m_nTriangleNum = nNum;

}



  //## end CRelxationLabelling::SetTriangleNum%419AEC1C0193.body
}

void CRelxationLabelling::InitNode2D4Sphere ()
{
  //## begin CRelxationLabelling::InitNode2D4Sphere%4199C7F802F4.body preserve=yes
	double th, f, r,x,y,z;
	
	this->Set2DPointNum (this->m_n3DNode_length);
	
	int m = this->m_nLabelList_length;
	int n = this->m_n3DNode_length, l;
	double dMin = FLT_MAX, g, th1,f1,th2,f2;

	for (int i = 0; i <n; i++ )
	{
		x = this->m_pCNode3D[i].x;
		y = this->m_pCNode3D[i].y;
		z = this->m_pCNode3D[i].z;
		
		r = sqrt(x*x+y*y+z*z);
		f = acos(BOUND(z/r,-1.0,1.0));
		th = atan2(y,x);
		
		if ( th < 0.0 )
			th += 2.0*PI;
		
		this->m_pCNode2D[i].v = f/PI;
		this->m_pCNode2D[i].u  = th/(2.0*PI);
		this->m_pCNode2D[i].m_n3DIndex = i;  

		//
		dMin = FLT_MAX;
		for ( int j = 0; j < m; j++ )
		{
			th1 = this->m_pCLabelList[j].u*2.0*PI;
			f1  = this->m_pCLabelList[j].v*PI;

			g = this->GeodesicDistance(th,f,th1,f1);
			
			if ( g < dMin )
			{
				l = j;
				dMin = g;
				th2 = th1;
				f2 = f1;
			}
		}

		this->m_pCNode2D[i].v = this->m_pCLabelList[l].v;
		this->m_pCNode2D[i].u  = this->m_pCLabelList[l].u;
		this->m_pCNode2D[i].m_nLabelIndex = l;  
		
		this->m_pCNode2D[i].x  = this->m_pCLabelList[l].x;
		this->m_pCNode2D[i].y  = this->m_pCLabelList[l].y;
		this->m_pCNode2D[i].z  = this->m_pCLabelList[l].z;
	}

	for (i = 0; i <n; i++ )
		this->SetNode2DLabelBox(i);
	
  //## end CRelxationLabelling::InitNode2D4Sphere%4199C7F802F4.body
}

double CRelxationLabelling::GeodesicDistance (double th1, double f1, double th2, double f2)
{
  //## begin CRelxationLabelling::GeodesicDistance%419AAE050104.body preserve=yes
	double dp = sin(f1)*sin(f2)*cos(th1-th2)+cos(f1)*cos(f2);
	dp = BOUND(dp,-1.0,1.0);
	ASSERT(fabs(dp)<=1.0);
	double g = acos(dp); 
	return g;
  //## end CRelxationLabelling::GeodesicDistance%419AAE050104.body
}

void CRelxationLabelling::SetNode2DLabelBox (int ai)
{
  //## begin CRelxationLabelling::SetNode2DLabelBox%419BED080327.body preserve=yes
//#define USEXYZBOX define or undefine this symbol must be done in the header file, not here!!
#ifndef USEXYZBOX
//USEXYZBOX{{
#define USEBOX

#ifdef USEBOX
	double nPI = 0.5;
	double Min_u=FLT_MAX,Min_v=FLT_MAX,Max_u=-FLT_MAX,Max_v=-FLT_MAX;
	double u0 =	this->m_pCNode2D[ai].u;
 	
	int n = this->m_n2DNode_length, lj;
	this->m_pCNode2D[ai].m_bCrossZero = FALSE;

	for ( register int aj = 0; aj < n; aj++)
	{
//		lj = this->m_pCNode2D[aj].m_nLabelIndex;
		if ( this->m_pCAdjancentMtrx0[ai][aj] != 0.0 )
		{
// 			if ((fabs(this->m_pCNode2D[aj].u - u0) > nPI))
//				this->m_pCNode2D[ai].m_bCrossZero = TRUE;
			Min_u = min(Min_u, this->m_pCNode2D[aj].u);
			Min_v = min(Min_v, this->m_pCNode2D[aj].v);
			Max_u = max(Max_u, this->m_pCNode2D[aj].u);
			Max_v = max(Max_v, this->m_pCNode2D[aj].v);
		}
	}
	double u;
	
	this->m_pCNode2D[ai].m_bCrossZero = ((Max_u - Min_u) > nPI);
//	if ((Max_u - Min_u) > nPI)
//		ASSERT(this->m_pCNode2D[ai].m_bCrossZero);
	 
	if (this->m_pCNode2D[ai].m_bCrossZero)
	{
		Min_u=FLT_MAX, Max_u=-FLT_MAX;
		for ( aj = 0; aj < n; aj++)
		{
			lj = this->m_pCNode2D[aj].m_nLabelIndex;
			if ( this->m_pCAdjancentMtrx0[ai][aj] != 0.0 )
			{
				u = this->m_pCNode2D[aj].u;
	 			
				if ( u < nPI )
					u = u + 2*nPI;

				Min_u = min(Min_u, u);
 				Max_u = max(Max_u, u);
			}
		}
		Max_u = Max_u - 2.0*nPI;

	}

#define BOXUONLY
#ifdef BOXUONLY
	this->m_pCNode2D[ai].dMin_v = -FLT_MAX;
	this->m_pCNode2D[ai].dMax_v =  FLT_MAX;
#else
	this->m_pCNode2D[ai].dMin_v = Min_v;
	this->m_pCNode2D[ai].dMax_v = Max_v;
#endif

	this->m_pCNode2D[ai].dMin_u = Min_u;
	this->m_pCNode2D[ai].dMax_u = Max_u;

#else
	this->m_pCNode2D[ai].dMin_u = -FLT_MAX;
	this->m_pCNode2D[ai].dMin_v = -FLT_MAX;
	this->m_pCNode2D[ai].dMax_u =  FLT_MAX;
	this->m_pCNode2D[ai].dMax_v =  FLT_MAX;

#endif
//USEXYZBOX}}
#else
//USEXYZBOX defined {{

	double Min_x= FLT_MAX,Min_y= FLT_MAX,Min_z = FLT_MAX;
	double Max_x=-FLT_MAX,Max_y=-FLT_MAX,Max_z =-FLT_MAX;
	
	Min_x = Max_x = this->m_pCNode2D[ai].x;
	Min_y =	Max_y = this->m_pCNode2D[ai].y;
	Min_z = Max_z = this->m_pCNode2D[ai].z;
 	
 	int n = this->m_n2DNode_length ;
	int c = 0;
	 
	for ( register int aj = 0; aj < n; aj++)
	{
 		if ( this->m_pCAdjancentMtrx0[ai][aj] != 0.0 )
		{
 			Min_x = min(Min_x, this->m_pCNode2D[aj].x);
			Min_y = min(Min_y, this->m_pCNode2D[aj].y);
			Min_z = min(Min_z, this->m_pCNode2D[aj].z);
			c++;
			Max_x = max(Max_x, this->m_pCNode2D[aj].x);
			Max_y = max(Max_y, this->m_pCNode2D[aj].y);
			Max_z = max(Max_z, this->m_pCNode2D[aj].z);
		}
	}

	double d,x,y,z;
	d = max(Max_x-Min_x,max(Max_y-Min_y,Max_z-Min_z));
	d = d / 2.0;
	x = (Max_x+Min_x)/2.0;
	y = (Max_y+Min_y)/2.0;
	z = (Max_z+Min_z)/2.0;

	ASSERT (d != 0.0);

	this->m_pCNode2D[ai].x0 = x - d;
	this->m_pCNode2D[ai].y0 = y - d;
	this->m_pCNode2D[ai].z0 = z - d; 
	this->m_pCNode2D[ai].x1 = x + d;
	this->m_pCNode2D[ai].y1 = y + d;
	this->m_pCNode2D[ai].z1 = z + d;


//USEXYZBOX defined }}
#endif 
  //## end CRelxationLabelling::SetNode2DLabelBox%419BED080327.body
}

void CRelxationLabelling::SetAdjacentMatrixEdge (int v1, int v2, int v3)
{
  //## begin CRelxationLabelling::SetAdjacentMatrixEdge%4180BDDC00C7.body preserve=yes
//	void CRelxationLabelling::SetAdjacentMatrixEdge(int v1, int v2, int v3)
{
	/*
	m_pCAdjancentMtrx[v1][v2] += 1;
	m_pCAdjancentMtrx[v2][v1] += 1;

	m_pCAdjancentMtrx[v2][v3] += 1;
	m_pCAdjancentMtrx[v3][v2] += 1;

	m_pCAdjancentMtrx[v1][v3] += 1;
	m_pCAdjancentMtrx[v3][v1] += 1;
	*/
	m_pCAdjancentMtrx0[v1][v2] = 1.0;
	m_pCAdjancentMtrx0[v2][v1] = 1.0;

	m_pCAdjancentMtrx0[v2][v3] = 1.0;
	m_pCAdjancentMtrx0[v3][v2] = 1.0;

	m_pCAdjancentMtrx0[v1][v3] = 1.0;
	m_pCAdjancentMtrx0[v3][v1] = 1.0;

}

  //## end CRelxationLabelling::SetAdjacentMatrixEdge%4180BDDC00C7.body
}

void CRelxationLabelling::SetAdjacentMatrixDim (int row, int col)
{
  //## begin CRelxationLabelling::SetAdjacentMatrixDim%4180BE0C0289.body preserve=yes
//	void CRelxationLabelling::SetAdjacentMatrixDim(int row, int col)
{
//	m_pCAdjancentMtrx.SetDim(row, col);	
	m_pCAdjancentMtrx0.SetDim(row, col);	

	CProgressBar * pbar = NULL;
    pbar = new CProgressBar(_T("Setting Adjacent Matrix"), 100, 100, TRUE);
    pbar->SetText("Setting Adjacent Matrix...");
    float step = 0;
	float prog = 0;
	step = (float)100.0/(float)(row);

	for ( int i=0; i <row; i++)
	{
		prog += step;
		pbar->SetPos((int)prog);
		
		for (int  j=0; j<col; j++)
		{			
//			m_pCAdjancentMtrx[i][j] = 0;
			m_pCAdjancentMtrx0[i][j] = 0;
		}
	}
	delete pbar;	
}

  //## end CRelxationLabelling::SetAdjacentMatrixDim%4180BE0C0289.body
}

void CRelxationLabelling::Set2DPoint (CNode2D p, int index)
{
  //## begin CRelxationLabelling::Set2DPoint%4180BE2C00EA.body preserve=yes
//	void CRelxationLabelling::Set2DPoint(CNode2D p, int index)
{
	ASSERT(index >= 0);
	m_pCNode2D[index] = p;
}

  //## end CRelxationLabelling::Set2DPoint%4180BE2C00EA.body
}

void CRelxationLabelling::Set3DPoint (CNode3D p, int index)
{
  //## begin CRelxationLabelling::Set3DPoint%4180BE4A031E.body preserve=yes
//void CRelxationLabelling::Set3DPoint(CNode3D p, int index)
{
	ASSERT(index >= 0);
	m_pCNode3D[index] = p;
}

  //## end CRelxationLabelling::Set3DPoint%4180BE4A031E.body
}

void CRelxationLabelling::Set2DPointNum (int num)
{
  //## begin CRelxationLabelling::Set2DPointNum%4180BE77014C.body preserve=yes
//void CRelxationLabelling::Set2DPointNum(int num)
{
	ASSERT( num > 0 );

	if ( m_pCNode2D != NULL )
		delete m_pCNode2D;
	m_n2DNode_length = num;
	m_pCNode2D = new CNode2D[num];
}

  //## end CRelxationLabelling::Set2DPointNum%4180BE77014C.body
}

void CRelxationLabelling::Set3DPointNum (int num)
{
  //## begin CRelxationLabelling::Set3DPointNum%4180BE8D0252.body preserve=yes
//	void CRelxationLabelling::Set3DPointNum(int num)
{
	ASSERT( num > 0 );
	
	m_n3DNode_length = num;
	if ( m_pCNode3D != NULL )
 		delete m_pCNode3D;
	m_pCNode3D = NULL;
	ASSERT(m_pCNode3D==NULL);
	
	m_pCNode3D = new CNode3D[num];	
}

  //## end CRelxationLabelling::Set3DPointNum%4180BE8D0252.body
}

// Additional Declarations
  //## begin CRelxationLabelling%416F95E400E7.declarations preserve=yes

float CRelxationLabelling::ComputeGeoStretch()
{	
	int i,j;
	double Ss[3],St[3],L2;
	int t[3];
	double tx[3],ty[3];
	double a,b,c;
	double *A = NULL;
	double *geoStretch = NULL;

	double lfSum1, lfSum2;
       
	int nFaceSize = m_nTriangleNum;
	//int nFaceSize = m_n

	geoStretch = new double[nFaceSize];
	A = new double[nFaceSize];

	for(i=0; i<nFaceSize; i++)
	{ 
		int index[3];
		index[0] = m_pCTriangle[i].A;
		index[1] = m_pCTriangle[i].B;
		index[2] = m_pCTriangle[i].C;		
		
		for (j=0; j<3; j++)
		{
			int tempIndex = index[j];

			tx[j] = m_pCNode2D[tempIndex].u;
			ty[j] = m_pCNode2D[tempIndex].v;
			
			t[j] =  m_pCNode2D[tempIndex].m_n3DIndex;
		}			
		
		A[i]=((tx[1]-tx[0])*(ty[2]-ty[0]) - (tx[2]-tx[0])*(ty[1]-ty[0]))/2;

		CNode3D p1,p2,p3;
		p1 = m_pCNode3D[ t[0] ];
		p2 = m_pCNode3D[ t[1] ];
		p3 = m_pCNode3D[ t[2] ];

		Ss[0]=( p1.x*(ty[1]-ty[2])
			  + p2.x*(ty[2]-ty[0])
			  + p3.x*(ty[0]-ty[1]) )/(2*A[i]);

		Ss[1]=( p1.y*(ty[1]-ty[2])
			  + p2.y*(ty[2]-ty[0])
			  + p3.y*(ty[0]-ty[1]))/(2*A[i]);
		
		Ss[2]=( p1.z*(ty[1]-ty[2])
			  + p2.z*(ty[2]-ty[0])
			  + p3.z*(ty[0]-ty[1]))/(2*A[i]);

			
		St[0]=( p1.x*(tx[2]-tx[1])
			  + p2.x*(tx[0]-tx[2])
			  + p3.x*(tx[1]-tx[0]) )/(2*A[i]);

		St[1]=( p1.y*(tx[2]-tx[1])
			  + p2.y*(tx[0]-tx[2])
			  + p3.y*(tx[1]-tx[0]))/(2*A[i]);

		St[2]=( p1.z*(tx[2]-tx[1])
			  + p2.z*(tx[0]-tx[2])
			  + p3.z*(tx[1]-tx[0]))/(2*A[i]);
		
		a=Ss[0]*Ss[0]+Ss[1]*Ss[1]+Ss[2]*Ss[2];
		b=Ss[0]*St[0]+Ss[1]*St[1]+Ss[2]*St[2];
		c=St[0]*St[0]+St[1]*St[1]+St[2]*St[2];

		L2=sqrt((a+c)/2);
		geoStretch[i]=L2;
	}

	lfSum1=0;
	lfSum2=0;
	for(i=0; i < nFaceSize; i++)
	{ 
		lfSum1+=geoStretch[i]*geoStretch[i]*A[i];
		lfSum2+=A[i];
	}
	
	delete []A;
	delete []geoStretch;

	double geoS = sqrt(lfSum1/lfSum2);
	m_geometryStretch = geoS;

	//set the disp text
	//m_DispText.strGeometryStretchRatio.Format("%lf", geoS);

	return float(geoS);
}

double CRelxationLabelling::GetGeometryStretch() const
{	
	return (float)m_geometryStretch;
}

void  CRelxationLabelling::Compute3DStatisticOfRation()
{
	double lfMean;
	double lfCov;
	
	double lfSum=0.0;
	
	for(int i=0; i < m_nEdgeList_length; i++)
	{ 
		int index1 = 0;
		int index2 = 0;
		index1 = m_pEdgeList[i].v1;
		index2 = m_pEdgeList[i].v2;		
		
		//compute the stretch ratio of each edge             				
		CNode3D pa, pb;					
		double  a[2], b[2];
		int     ia = 0;
		int     ib = 0;
		
		a[0] = m_pCNode2D[index1].u * 2 * PI;
		a[1] = m_pCNode2D[index1].v * PI;
		ia = m_pCNode2D[index1].m_n3DIndex;
		pa = m_pCNode3D[ia];
		
		b[0] = m_pCNode2D[index2].u * 2 * PI;
		b[1] = m_pCNode2D[index2].v * PI;
		ib = m_pCNode2D[index2].m_n3DIndex;
		pb = m_pCNode3D[ib];
		
		float d2_ab = 0;
		float d3_ab = 0;
		
		d2_ab = (float)GeodesicDistance(a[0], a[1], b[0], b[1]);
		d3_ab = (float)sqrt((pa.x-pb.x)*(pa.x-pb.x) + 
			(pa.y-pb.y)*(pa.y-pb.y) +
			(pa.z-pb.z)*(pa.z-pb.z) );
		
		float ratio = 0;
		
		ratio = (float)Stretch(d2_ab,d3_ab);
		
		m_pEdgeList[i].dStretchMatrics = ratio;
		
		lfSum += m_pEdgeList[i].dStretchMatrics;
	}
	
	lfMean = (lfSum) / (double)m_nEdgeList_length;
	
	lfSum = 0;
	for(i=0; i < m_nEdgeList_length; i++)
	{ 
		lfSum += (m_pEdgeList[i].dStretchMatrics - lfMean)*
			     (m_pEdgeList[i].dStretchMatrics - lfMean);
	}
	
	lfCov = sqrt(lfSum/(m_nEdgeList_length-1));

	m_dfAvarageOfRatio = lfMean;
	m_dfConvOfRatio = lfCov;	

	CString averageOfSR;
	averageOfSR.Format("%lf", m_dfAvarageOfRatio);
	CString coviarianceOfSR;
	coviarianceOfSR.Format("%lf", m_dfConvOfRatio);
	CString geoRatio;
	geoRatio.Format("%lf", m_geometryStretch);

	/*
    g_CGlobals.SetMesh(0, 
					   NULL, NULL, NULL,
                       NULL, NULL, NULL, 
					   NULL, NULL, NULL,
					   0, NULL, NULL, NULL,
					   0,
					   averageOfSR,
					   coviarianceOfSR,
					   geoRatio);  
					   */


	//////////////////////////////////////////////////////////////////////////
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	int nCurMeshIndex = pDoc->m_nCurrentMeshIndex;
	pDoc->m_pMesh[nCurMeshIndex]->SetMesh( 0, 
									       NULL, NULL, NULL,
										   NULL, NULL, NULL, 
										   NULL, NULL, NULL,
										   0, NULL, NULL, NULL,
					                       0,
										   averageOfSR,
										   coviarianceOfSR,
										   geoRatio);
	//pDoc->m_strTitle[nCurMeshIndex] = pDoc->m_strFileName + " " ;
	
}  

void CRelxationLabelling::SetDataFromOpti(int nSum, float *x, float *y, float *z)
{
	
	m_n2DNode_length = nSum;
	m_n3DNode_length = nSum;
	m_pCNode2D = new CNode2D[m_n2DNode_length];
	m_pCNode3D = new CNode3D[m_n3DNode_length];

	for(int i=0; i<nSum; i++)
	{
		float u,v;

		//set the 3D point	
		m_pCNode3D[i].x = x[i];
		m_pCNode3D[i].y = y[i];
		m_pCNode3D[i].z = z[i];
		
		//set the 2d 
		ComputeSphereCoordi(x[i], y[i], z[i], u, v);
		m_pCNode2D[i].v = u / PI;
		m_pCNode2D[i].u = v /(2*PI);
		m_pCNode2D[i].m_n3DIndex = i;
	}	 
 
}

void  CRelxationLabelling::SetDataFromGeoimg( CMeshForGeoImg &geoMesh )
{
	//set 3d points
	int n3DPtSum = geoMesh.Get3DPtNum() + geoMesh.GetAddPtNum();
	Set3DPointNum(n3DPtSum);
	for(int i=0; i<n3DPtSum; i++)
	{		
		m_pCNode3D[i].x = geoMesh.meshPnt(i).x;
		m_pCNode3D[i].y = geoMesh.meshPnt(i).y;
		m_pCNode3D[i].z = geoMesh.meshPnt(i).z;
	}	

	//set 2d points
	int nBorderLen = geoMesh.GetBoundMapPointVec().size();
	int n2DPtsNum = geoMesh.GetBoundMapPointVec().size() 
		           + geoMesh.GetInnerMapPointVec().size();

	Set2DPointNum(n2DPtsNum + 1);		
	
	int m = 0;
	for(m=0; m < geoMesh.GetBoundMapPointVec().size(); m++)
	{
		CNode2D p;
		p.u = geoMesh.GetBoundMapPointVec().at(m).p.x;
		p.v = geoMesh.GetBoundMapPointVec().at(m).p.y; 
		p.m_n3DIndex = geoMesh.GetBoundMapPointVec().at(m).pointIndex;
		
		//insert 2D point into RelxationLabelling
		Set2DPoint(p, m);
	}
	for(m=0; m < geoMesh.GetInnerMapPointVec().size(); m++)
	{ 
		CNode2D p;
		p.u = geoMesh.GetInnerMapPointVec().at(m).p.x;
		p.v = geoMesh.GetInnerMapPointVec().at(m).p.y; 
		p.m_n3DIndex = geoMesh.GetInnerMapPointVec().at(m).pointIndex;
		
		//insert 2D point into RelxationLabelling
		Set2DPoint(p, m+nBorderLen);
	}

	//adding a point!!
    CNode2D p;
	p.u = 0; p.v = 0; p.m_n3DIndex = 0;
	Set2DPoint(p, n2DPtsNum);
	//////////////////////////////////////////////////////////////////////////
	
	//set triangles
	SetAdjacentMatrixDim(n2DPtsNum+1, n2DPtsNum+1);				
	
	int j = 0,k = 0;
	int nTriangleNum = geoMesh.GetNewFaceVec().size();
	SetTriangleNum(nTriangleNum);	
	
	for ( k = 0; k < nTriangleNum; k++)
	{
		//get the indices of a triangle
		int va,vb,vc;
		
		int pntIndex[3];
		for(int j=0; j<3 ; j++)
		{ 
			if(geoMesh.GetNewFaceVec().at(k).flag[j]==0)
				pntIndex[j] = geoMesh.GetNewFaceVec().at(k).index[j] + geoMesh.GetBoundMapPointVec().size();
			else 
				pntIndex[j] = geoMesh.GetNewFaceVec().at(k).index[j];
		}
		
		va = pntIndex[0];
		vb = pntIndex[1];
		vc = pntIndex[2];
		SetTriangleVertexIndex(va, vb, vc, k);
		
		m_pCAdjancentMtrx0[va][vb] =
			m_pCAdjancentMtrx0[vb][va] = 1;
		
		m_pCAdjancentMtrx0[vb][vc] =
			m_pCAdjancentMtrx0[vc][vb] = 1;
		
		m_pCAdjancentMtrx0[va][vc] =
			m_pCAdjancentMtrx0[vc][va] = 1;				
	}		

	SetEdgeList();		
	SetBorderLen(nBorderLen);
	
	//compute the sample gap after retrieve the edge information
	ComputeSampleGap(); //	
	ComputeGeoStretch();
	ComputeStatisticOfRatio();
}

void CRelxationLabelling::SetDataFromMesh(CMesh &mesh)
{
	//mesh.ComputeScale(1);
	int nBorderLen =  mesh.GetBorderLen();
	
	//if the mesh is 2D mesh, then invoke special functions
	if( nBorderLen )
	{
		m_nModeType = 0;

		int n2DPtsNum = mesh.GetVertexNum() - 1 ;

		Set2DPointNum(n2DPtsNum);		

		//double dfMinX = FLT_MAX;
		//double dfMaxX = FLT_MIN;
		//double dfMinY = FLT_MAX;
		//double dfMaxY = FLT_MIN;

		for (int i=0; i<n2DPtsNum; i++)
		{
			float x,y,z;
			int index;
			((C2DMesh*)(&mesh))->Get2DPtIndexTo3D(index, i);
			mesh.GetVertex(x,y,z,i);

			CNode2D p;
			p.u = x;
			p.v = y; 
			p.m_n3DIndex = index;
			
			/*
			//find the min and max
			if(dfMinX > x )
				dfMinX = x;
			if(dfMaxX < x)
				dfMaxX = x;

			if(dfMinY > y )
				dfMinY = y;
			if(dfMaxY < y)
				dfMaxY = y;
			*/
			
			//insert 2D point into RelxationLabelling
			Set2DPoint(p, i);
		}

		int n3DPtsNum = ((C2DMesh*)(&mesh))->Get3DPointNum();
		Set3DPointNum(n3DPtsNum);

		for ( i = 0 ; i < n3DPtsNum; i++)
		{
			float x,y,z;
			((C2DMesh*)(&mesh))->Get3DPoint(x, y, z, i);
			
			m_pCNode3D[i].x = x;
			m_pCNode3D[i].y = y;
			m_pCNode3D[i].z = z;
		}				

		SetAdjacentMatrixDim(n2DPtsNum, n2DPtsNum);				

		int j = 0,k = 0;
		int nTriangleNum = mesh.GetTriangleNum();
		SetTriangleNum(nTriangleNum);

		for ( k = 0; k < nTriangleNum; k++)
		{
			//get the indices of a triangle
			int va,vb,vc;

			mesh.GetTriangleIndices(va, vb, vc, k);
			SetTriangleVertexIndex(va, vb, vc, k);

			m_pCAdjancentMtrx0[va][vb] =
				m_pCAdjancentMtrx0[vb][va] = 1;
			
			m_pCAdjancentMtrx0[vb][vc] =
				m_pCAdjancentMtrx0[vc][vb] = 1;
			
			m_pCAdjancentMtrx0[va][vc] =
				m_pCAdjancentMtrx0[vc][va] = 1;				
		}		
		SetEdgeList();	

		SetBorderLen(nBorderLen);

		//compute the sample gap after retrieve the edge information
		ComputeSampleGap(); //	
		ComputeGeoStretch();
		ComputeStatisticOfRatio();
	}//end if
	else
	{
		m_nModeType = 1;

		Set3DPointNum(mesh.m_nNumOfVertexs);
		int nVertexNum = mesh.GetVertexNum();
		for ( int i = 0 ; i < nVertexNum; i++)
		{
			float x,y,z;
			
			mesh.GetVertex(x, y, z, i);
			//TRACE("%f %f %f \n", x, y, z);
			
			m_pCNode3D[i].x = x;
			m_pCNode3D[i].y = y;
			m_pCNode3D[i].z = z;
		}				
		
		SetAdjacentMatrixDim(nVertexNum, nVertexNum);				
		int j = 0,k = 0;
		
		int nTriangleNum = mesh.GetTriangleNum();
		SetTriangleNum(nTriangleNum);

		for ( k = 0; k < nTriangleNum; k++)
		{
			//get the indices of a triangle
			int va,vb,vc;
			mesh.GetTriangleIndices(va, vb, vc, k);
			SetTriangleVertexIndex(va, vb, vc, k);
			m_pCAdjancentMtrx0[va][vb] =
				m_pCAdjancentMtrx0[vb][va] = 1;
			
			m_pCAdjancentMtrx0[vb][vc] =
				m_pCAdjancentMtrx0[vc][vb] = 1;
			
			m_pCAdjancentMtrx0[va][vc] =
				m_pCAdjancentMtrx0[vc][va] = 1;				
		}		
		SetEdgeList();	
	}//end else   

}

void  CRelxationLabelling::SetDataFromOpti( CMeshOpti &optimesh)
{

   //{{Added by xdh,04,12,29. for 3D Mesh
   m_nModeType = 1;
   //}}Added by xdh,04,12,29.

   int nVertexNum = optimesh.GetVertexNum();

   m_n2DNode_length = nVertexNum;
   m_n3DNode_length = nVertexNum;

   if(m_pCNode2D != NULL)
	   delete m_pCNode2D;
   
   //if(m_pCNode3D != NULL)
   //   delete m_pCNode3D;

   m_pCNode2D = new CNode2D[m_n2DNode_length];
   //m_pCNode3D = new CNode3D[m_n3DNode_length];
   
   for (int i=0; i<nVertexNum; i++)
   {
	   CVertex_ v = optimesh.GetVertex(i);
	   
	   float fu,fv;
	   
	   //set the 3D point	
	   //m_pCNode3D[i].x = v.dX;
	   //m_pCNode3D[i].y = v.dY;
	   //m_pCNode3D[i].z = v.dZ;
	   
	   //set the 2d 
	   ComputeSphereCoordi(v.dX, v.dY, v.dZ, fu, fv);
	   
	   m_pCNode2D[i].v = fu / PI;
	   m_pCNode2D[i].u = fv /(2*PI);

	   m_pCNode2D[i].m_n3DIndex = i;	   
   }
   
   int nTriangleNum = optimesh.GetTriangleNum();
   SetTriangleNum(nTriangleNum);
   SetAdjacentMatrixDim(nVertexNum, nVertexNum);
   for(i=0; i<nTriangleNum; i++)
   {
	   CTriangle_ t = optimesh.GetTriangle(i);
	   SetTriangleVertexIndex(t.pnV[0], t.pnV[1], t.pnV[2], i);
	   SetAdjacentMatrixEdge(t.pnV[0], t.pnV[1], t.pnV[2]);
   }
   SetEdgeList();
}

void  CRelxationLabelling::Save2DMesh(CString filename)
{
	
	if(m_nBorderlength)
	{
		int nStrLen = filename.GetLength() - 4;
		CString fileTile  = filename.Left(nStrLen);
		CString file2dmesh = fileTile + "_2DMeshResult.txt";
		
		FILE *fp = fopen(file2dmesh, "w");
		if(fp == NULL)
			return ;
		
		//save the 3D points:
		fprintf(fp, "%d\n", m_n3DNode_length);
		for(int i=0; i<m_n3DNode_length; i++)
		{
			fprintf(fp, "%f %f %f \n", m_pCNode3D[i].x, m_pCNode3D[i].y, m_pCNode3D[i].z );
		}
		
		//save the 2d points
		int nBorderPtNum = m_nBorderlength;
		int nVertexNum = m_n2DNode_length ;

		fprintf(fp, "%d %d \n", nBorderPtNum, nVertexNum-nBorderPtNum);
		
		for(i=0; i<nVertexNum; i++)
		{
			float x,y,z = 0;

			x = m_pCNode2D[i].u;
			y = m_pCNode2D[i].v;
			
			int index = m_pCNode2D[i].m_n3DIndex;
						
			fprintf(fp, "%f %f %d \n", x, y, index);
		}
		
		//save the triangle point list
		int nTriangleNum = m_nTriangleNum;
		fprintf(fp, "%d \n", nTriangleNum);
		for(i=0; i<nTriangleNum; i++)
		{
			int va,vb,vc;
			va = m_pCTriangle[i].A;
			vb = m_pCTriangle[i].B;
			vc = m_pCTriangle[i].C;

			fprintf(fp, "%d %d %d \n", va, vb, vc);
		}
		fclose(fp);
		
	}
}


void  CRelxationLabelling::SetNode2DLabelBox1(int ai)
{		
	double Min_x= FLT_MAX, Min_y= FLT_MAX ;
	double Max_x=-FLT_MAX, Max_y=-FLT_MAX ;
	
	Min_x = Max_x = this->m_pCNode2D[ai].u;
	Min_y =	Max_y = this->m_pCNode2D[ai].v;
	
	int n = this->m_n2DNode_length ;
	
	for ( register int aj = 0; aj < n; aj++)
	{
		if ( this->m_pCAdjancentMtrx0[ai][aj] != 0.0 )
		{
			Min_x = min(Min_x, this->m_pCNode2D[aj].u);
			Min_y = min(Min_y, this->m_pCNode2D[aj].v);
									
			Max_x = max(Max_x, this->m_pCNode2D[aj].u);
			Max_y = max(Max_y, this->m_pCNode2D[aj].v);
		}
	}
	
	double d,x,y;
	d = max( Max_x-Min_x, Max_y-Min_y );
	d = d / 2.0;
	
	x = (Max_x + Min_x)/2.0;
	y = (Max_y + Min_y)/2.0;
		
	ASSERT (d != 0.0);
	
	this->m_pCNode2D[ai].x0 = x - d;
	this->m_pCNode2D[ai].y0 = y - d;
	this->m_pCNode2D[ai].z0 = -0.5; 
	this->m_pCNode2D[ai].x1 = x + d;
	this->m_pCNode2D[ai].y1 = y + d;
	this->m_pCNode2D[ai].z1 = 0.5;
}


void CRelxationLabelling::SetEdge()
{
	//retrieve edges info from the triangles
	/*  
	for(int i=0; i<m_nTriangleNum; i++)
	{
		int v[3] ;
		v[0] =  m_pCTriangle[i].A;
		v[1] =  m_pCTriangle[i].B;
		v[2] =  m_pCTriangle[i].C;
        
		//sort from small to big:
		int t ;
		for(int ii=0; ii<3; ii++)
			for(int jj=ii+1; jj<3; jj++)
			{
				if(v[ii] > v[jj])
				{
					t = v[ii];
					v[ii] = v[jj];
					v[jj] = t;					
				}				
			}
        Edge e;
		e.v1 = v[0]; 		e.v2 = v[1];
		m_Edges.insert(e);
		e.v1 = v[1]; 		e.v2 = v[2];
		m_Edges.insert(e);
		e.v1 = v[0]; 		e.v2 = v[2];
		m_Edges.insert(e);
	}
	*/	
}
/*
bool operator==(const Edge e1,const Edge e2)
{
	if( (e1.v1==e2.v1) && (e1.v2==e2.v2))
		return TRUE;
	else
		return FALSE;         
};*/


  //## end CRelxationLabelling%416F95E400E7.declarations
// Class CP 



CP::CP()
  //## begin CP::CP%.hasinit preserve=no
      : m_bInitialized(0)
  //## end CP::CP%.hasinit
  //## begin CP::CP%.initialization preserve=yes
  //## end CP::CP%.initialization
{
  //## begin CP::CP%.body preserve=yes
  //## end CP::CP%.body
}


// Additional Declarations
  //## begin CP%416F97FA0370.declarations preserve=yes
  //## end CP%416F97FA0370.declarations

// Class Cr 


Cr::~Cr()
{
  //## begin Cr::~Cr%.body preserve=yes
  //## end Cr::~Cr%.body
}


// Additional Declarations
  //## begin Cr%416F98B4029B.declarations preserve=yes
  //## end Cr%416F98B4029B.declarations

// Class CNode2D 




















CNode2D::CNode2D()
  //## begin CNode2D::CNode2D%.hasinit preserve=no
      : dMin_u(-FLT_MAX), dMin_v(-FLT_MAX), dMax_u(FLT_MAX), dMax_v(FLT_MAX), m_bCrossZero(FALSE)
  //## end CNode2D::CNode2D%.hasinit
  //## begin CNode2D::CNode2D%.initialization preserve=yes
  //## end CNode2D::CNode2D%.initialization
{
  //## begin CNode2D::CNode2D%.body preserve=yes
  //## end CNode2D::CNode2D%.body
}


// Additional Declarations
  //## begin CNode2D%416F9AA301D5.declarations preserve=yes
  //## end CNode2D%416F9AA301D5.declarations

// Class CLambda 











// Additional Declarations
  //## begin CLambda%416F9CDD03A1.declarations preserve=yes
  //## end CLambda%416F9CDD03A1.declarations

// Class CNode3D 





// Additional Declarations
  //## begin CNode3D%4178A7FD0065.declarations preserve=yes
  //## end CNode3D%4178A7FD0065.declarations

// Class CAdjancentMtrx 


// Additional Declarations
  //## begin CAdjancentMtrx%417C618C000D.declarations preserve=yes
  //## end CAdjancentMtrx%417C618C000D.declarations

// Class CEdge4RL 





// Additional Declarations
  //## begin CEdge4RL%417DF1B00384.declarations preserve=yes
  //## end CEdge4RL%417DF1B00384.declarations

// Class CQ 


// Additional Declarations
  //## begin CQ%4185CC9301DB.declarations preserve=yes
  //## end CQ%4185CC9301DB.declarations

// Class CTrinagle 




// Additional Declarations
  //## begin CTrinagle%419AE564015E.declarations preserve=yes
  //## end CTrinagle%419AE564015E.declarations

// Class CTriangleRL 





// Additional Declarations
  //## begin CTriangleRL%419AEA210162.declarations preserve=yes
  //## end CTriangleRL%419AEA210162.declarations

// Class CRelxationLabelling1 

CRelxationLabelling1::CRelxationLabelling1()
  //## begin CRelxationLabelling1::CRelxationLabelling1%.hasinit preserve=no
  //## end CRelxationLabelling1::CRelxationLabelling1%.hasinit
  //## begin CRelxationLabelling1::CRelxationLabelling1%.initialization preserve=yes
  //## end CRelxationLabelling1::CRelxationLabelling1%.initialization
{
  //## begin CRelxationLabelling1::CRelxationLabelling1%.body preserve=yes
  //## end CRelxationLabelling1::CRelxationLabelling1%.body
}


CRelxationLabelling1::~CRelxationLabelling1()
{
  //## begin CRelxationLabelling1::~CRelxationLabelling1%.body preserve=yes
  //## end CRelxationLabelling1::~CRelxationLabelling1%.body
}



//## Other Operations (implementation)
void CRelxationLabelling1::f1 (int i, int li, int j, int lj, double* f1_x, double* f1_y, double* f1_z)
{
  //## begin CRelxationLabelling1::f1%41E36D71010F.body preserve=yes
  	double l, p;
	double p3d_i[3], p3d_j[3];
//	double p2d_i[2], p2d_j[2];
	int    i3d, j3d;

	if ( ((this->m_pCAdjancentMtrx0))[i][j] != 0.0 )
	{
	i3d			= this->m_pCNode2D[i].m_n3DIndex;
	j3d			= this->m_pCNode2D[j].m_n3DIndex;

	p3d_i[0]	= this->m_pCNode3D[i3d].x;
	p3d_i[1]	= this->m_pCNode3D[i3d].y;
	p3d_i[2]	= this->m_pCNode3D[i3d].z;

	p3d_j[0]	= this->m_pCNode3D[j3d].x;
	p3d_j[1]	= this->m_pCNode3D[j3d].y;
	p3d_j[2]	= this->m_pCNode3D[j3d].z;

	l			= (p3d_i[0] - p3d_j[0])*(p3d_i[0] - p3d_j[0]) +
				  (p3d_i[1] - p3d_j[1])*(p3d_i[1] - p3d_j[1]) +
				  (p3d_i[2] - p3d_j[2])*(p3d_i[2] - p3d_j[2]);
//		l			= sqrt (l);
	l			= sqrt (l)*this->m_dTensionFactor;

	double u1 	= this->m_pCLabelList[li].u;
	double u2 	= this->m_pCLabelList[lj].u;
	double		sin_v1,sin_v2,cos_v1,cos_v2;
	sin_v1 = this->m_pCLabelList[li].sin_v;
	sin_v2 = this->m_pCLabelList[lj].sin_v;
	cos_v1 = this->m_pCLabelList[li].cos_v;
	cos_v2 = this->m_pCLabelList[lj].cos_v;
	double dp = sin_v1*sin_v2*cos((u1-u2)*2.0*PI)+cos_v1*cos_v2;
	dp = BOUND(dp,-1.0,1.0);
	ASSERT(fabs(dp)<=1.0);
	dp			= acos(dp);
//	p			= fabs(fabs(dp)-l);
	p			=  (fabs(dp)-l);

	*f1_x = this->m_pCLabelList[lj].x -  this->m_pCLabelList[li].x;
	*f1_y = this->m_pCLabelList[lj].y -  this->m_pCLabelList[li].y;
	*f1_z = this->m_pCLabelList[lj].z -  this->m_pCLabelList[li].z;
	double v[3] = {*f1_x, *f1_y, *f1_z};
	CMyCGLib CMyCGLib;
	/*
	CMyCGLib.normalize(v , 3);
	*f1_x = v[0]*p;
	*f1_y = v[1]*p;
	*f1_z = v[2]*p;
	*/
	double normal[3] = {-this->m_pCLabelList[li].x,
						-this->m_pCLabelList[li].y,
						-this->m_pCLabelList[li].z};
	CMyCGLib.normalize(normal, 3);
	dp = CMyCGLib.pp(normal,v,3);
	CMyCGLib.scale(normal,normal,dp,3);
	CMyCGLib.sub(v,normal,v,3);
	CMyCGLib.normalize(v, 3);
	CMyCGLib.scale(v, v, p, 3);
	}
	else
	{
//	 return 0.0;
	}
  //## end CRelxationLabelling1::f1%41E36D71010F.body
}

double CRelxationLabelling1::P0 (int ai, int li)
{
  //## begin CRelxationLabelling1::P0%41E36ECC012C.body preserve=yes

	if(this->IsLabelInNodeLabelBox(ai,li))
	{
		if ( !this->m_bIsForSpherical )
		{	
			register double fsum=0.0;
			register int ak;
			int n = this ->m_n2DNode_length;
			for (  ak = 0; ak < n; ak++ )
			{
				if ( ((this->m_pCAdjancentMtrx0))[ai][ak] != 0.0 )
				{
					fsum = fsum + f0(ai, li , ak, this->m_pCNode2D[ak].m_nLabelIndex);
				}
			}
			double p0 = 1/(1+fsum);
			return p0;
		}
		else
		{
			register double fsum_x, fsum_y, fsum_z,x,y,z;
			register int ak;
			int n = this ->m_n2DNode_length;
			fsum_x = fsum_y = fsum_z = 0.0;

			for (  ak = 0; ak < n; ak++ )
			{
				if ( ((this->m_pCAdjancentMtrx0))[ai][ak] != 0.0 )
				{
					this->f1(ai, li , ak, this->m_pCNode2D[ak].m_nLabelIndex,&x, &y, &z);
					fsum_x += x; 
					fsum_y += y; 
					fsum_z += z; 
				}
			}
			double normal[3] = {-this->m_pCLabelList[ai].x, -this->m_pCLabelList[ai].y, -this->m_pCLabelList[ai].z };	
			double v[3] = {fsum_x,fsum_y,fsum_z};
			
			CMyCGLib CMyCGLib;
			/*
			CMyCGLib.normalize(normal,3);
			double p0 = CMyCGLib.pp(normal,v,3);
			normal[0] *= p0;
			normal[1] *= p0;
			normal[2] *= p0;
			for ( int i = 0; i < 3; i++)
			{
				v[i] = v[i] - normal[i];
			}*/
			double p0 = sqrt(CMyCGLib.pp(v,v,3));
			p0 = 1.0/(1.0+p0);
			return p0;

		}
	}
	else
		return 0.0;



  //## end CRelxationLabelling1::P0%41E36ECC012C.body
}

// Additional Declarations
  //## begin CRelxationLabelling1%41E36A620177.declarations preserve=yes
  //## end CRelxationLabelling1%41E36A620177.declarations

// Class CRelxationLabelling2 


//## Other Operations (implementation)
double CRelxationLabelling2::r (int ai, int Li, int ak, int Lk)
{
  //## begin CRelxationLabelling2::r%41E4A11E027A.body preserve=yes
	

		double R;
	if ( this->IsBorderNode(ai))
	{
	 if( ai == ak )
		 return 1.0;
	 else
		 return 0.0;
	}
	else
	{
	if ( (ai != ak) && (Li == Lk) )
		R = 0.0;
	else
		if ( ((this->m_pCAdjancentMtrx0))[ai][ak] != 0.0 )
//		if ( this->C(ai,ak) != 0.0 )
		{
			R = 1.0/(1.0+fabs(this->f(ai, Li, ak, Lk)));
		}
		else R = /*1.0*/ 0.0;
	return R;
	}

  //## end CRelxationLabelling2::r%41E4A11E027A.body
}

double CRelxationLabelling2::f_act (int ai, int li, int aj, int lj)
{
  //## begin CRelxationLabelling2::f_act%41E4A12702D7.body preserve=yes

	
{
			register double fsum_x, fsum_y, fsum_z,x,y,z;
			register int ak;
			int n = this ->m_n2DNode_length;
			fsum_x = fsum_y = fsum_z = 0.0;

			for (  ak = 0; ak < n; ak++ )
			{
				if ( ((this->m_pCAdjancentMtrx0))[ai][ak] != 0.0 )
				{
				
					if ( ak != aj )
						this->f1(ai, li , ak, this->m_pCNode2D[ak].m_nLabelIndex,&x, &y, &z);
					else
						this->f1(ai, li , aj, lj, &x, &y, &z);
					fsum_x += x; 
					fsum_y += y; 
					fsum_z += z; 
				}
			}
			double normal[3] = {-this->m_pCLabelList[ai].x, -this->m_pCLabelList[ai].y, -this->m_pCLabelList[ai].z };	
			double v[3] = {fsum_x,fsum_y,fsum_z};
			
			CMyCGLib CMyCGLib;
			/*
			CMyCGLib.normalize(normal,3);
			double p0 = CMyCGLib.pp(normal,v,3);
			normal[0] *= p0;
			normal[1] *= p0;
			normal[2] *= p0;
			for ( int i = 0; i < 3; i++)
			{
				v[i] = v[i] - normal[i];
			}*/
			double p0 = sqrt(CMyCGLib.pp(v,v,3));
			p0 = 1.0/(1.0+p0);
			return p0;

		}


  //## end CRelxationLabelling2::f_act%41E4A12702D7.body
}

double CRelxationLabelling2::f (int i, int li, int j, int lj)
{
  //## begin CRelxationLabelling2::f%41E4A13202F0.body preserve=yes
		if ( this->m_bIsForSpherical )
//		return this->f1(i, li, j, lj);
		return this->f_act(i, li, j, lj);
	else
		return this->f0(i, li, j, lj);

  //## end CRelxationLabelling2::f%41E4A13202F0.body
}

// Additional Declarations
  //## begin CRelxationLabelling2%41E4A0E2004C.declarations preserve=yes
  //## end CRelxationLabelling2%41E4A0E2004C.declarations

//## begin module%4175B5880383.epilog preserve=yes
//## end module%4175B5880383.epilog
