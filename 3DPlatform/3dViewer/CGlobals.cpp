//## begin module%40D7F3180092.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40D7F3180092.cm

//## begin module%40D7F3180092.cp preserve=no
//## end module%40D7F3180092.cp

//## Module: CGlobals%40D7F3180092; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CGlobals.cpp

//## begin module%40D7F3180092.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%40D7F3180092.additionalIncludes
//## begin module%40D7F3180092.includes preserve=yes
#include "math.h"
//## end module%40D7F3180092.includes

// CGlobals
#include "CGlobals.h"
#include "COpenGl3DEx.h"
#include "CDlgTab00.h"
#include "CDlgTab01.h"
#include "3dViewerDoc.h"
#include "COpenGL3DEx.h"
#include "2DMesh.h"

//## begin module%40D7F3180092.declarations preserve=no
//## end module%40D7F3180092.declarations

//## begin module%40D7F3180092.additionalDeclarations preserve=yes
class CGlobals g_CGlobals;
//## end module%40D7F3180092.additionalDeclarations

// Class CGlobals 


CGlobals::CGlobals()
  //## begin CGlobals::CGlobals%.hasinit preserve=no
      : m_pCMainFrame(NULL), m_dblMeanGrayBeforeFFT(0.0)
  //## end CGlobals::CGlobals%.hasinit
  //## begin CGlobals::CGlobals%.initialization preserve=yes
  //## end CGlobals::CGlobals%.initialization
{
  //## begin CGlobals::CGlobals%.body preserve=yes
//	m_pDlgOptiSettings = NULL;
	m_pCMy3dViewerDoc = NULL;
	m_pCOpenGL3DEx = NULL;
	m_pCMainFrame = NULL;
	m_pDlgTab00 = NULL;
	m_pDlgTab01 = NULL;
	m_pDlgTab02 = NULL; 
  //## end CGlobals::CGlobals%.body
}


CGlobals::~CGlobals()
{
  //## begin CGlobals::~CGlobals%.body preserve=yes
  //## end CGlobals::~CGlobals%.body
}



//## Other Operations (implementation)
void CGlobals::set_m_pCBmpviewerDoc (CMy3dViewerDoc* pCDoc)
{
  //## begin CGlobals::set_m_pCBmpviewerDoc%40D8F1560381.body preserve=yes
	this->m_pCMy3dViewerDoc = pCDoc;
  //## end CGlobals::set_m_pCBmpviewerDoc%40D8F1560381.body
}

void CGlobals::set_m_pCOpenGL3DEx (COpenGL3DEx* pC3DEx)
{
    this->m_pCOpenGL3DEx = pC3DEx;
}

CMy3dViewerDoc* CGlobals::get_m_pCBmpviewerDoc ()
{
  //## begin CGlobals::get_m_pCBmpviewerDoc%40DA45150244.body preserve=yes
	return m_pCMy3dViewerDoc;
  //## end CGlobals::get_m_pCBmpviewerDoc%40DA45150244.body
}

COpenGL3DEx* CGlobals::get_m_pCOpenGL3DEx()
{
    return m_pCOpenGL3DEx;
}

void CGlobals::SetMeanGrayBeforeFFT (double dblGray)
{
  //## begin CGlobals::SetMeanGrayBeforeFFT%40DA4C90019D.body preserve=yes
	this->m_dblMeanGrayBeforeFFT	= dblGray;
  //## end CGlobals::SetMeanGrayBeforeFFT%40DA4C90019D.body
}

void CGlobals::set_m_pCMainFrame (CMainFrame* pCMainFrame)
{
  //## begin CGlobals::set_m_pCMainFrame%40DA797A0242.body preserve=yes
	this->m_pCMainFrame = pCMainFrame;
  //## end CGlobals::set_m_pCMainFrame%40DA797A0242.body
}

/*
void CGlobals::SetMesh (int nTriangle, int* nV1, int* nV2, int* nV3, 
						COLORREF* c12, COLORREF* c23, COLORREF* c31, 
						double *ratio12, double *ratio23, double *ratio31,
						int nVlength, double* x, double* y, double* z,
						int nEdgeNodeNum,
						CString strAverage,
						CString strConviance,
						CString strGeoRatio,
						int nType)
{
  //## begin CGlobals::SetMesh%418836FA034C.body preserve=yes

	int nCurMeshIndex = m_pCMy3dViewerDoc->m_nCurrentMeshIndex;
	
	m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex]->SetMesh( nTriangle, nV1, nV2, nV3,
													   c12, c23, c31, 
													   ratio12, ratio23, ratio31,
													   nVlength, x, y, z,	
													   nEdgeNodeNum,
													   strAverage,
													   strConviance,
													   strGeoRatio
													   );

	m_pCMy3dViewerDoc->m_strTitle[nCurMeshIndex] 
		= m_pCMy3dViewerDoc->m_strFileName + " " ;
    
	if(nTriangle)
	{
		((COpenGL3DEx *)(m_pCMy3dViewerDoc->m_pCOpenGL))->m_pCMesh = ( m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex] ); 
		((COpenGL3DEx *)(m_pCMy3dViewerDoc->m_pCOpenGL))->m_pCMesh->m_nType = nType; 
		
		m_pCMy3dViewerDoc->m_nCurrentMeshIndex =  (m_pCMy3dViewerDoc->m_nCurrentMeshIndex + 1) % 2;
		m_pCMy3dViewerDoc->m_strTitle[m_pCMy3dViewerDoc->m_nCurrentMeshIndex] 
			= m_pCMy3dViewerDoc->m_strFileName + "*";
		
		m_pCMy3dViewerDoc->UpdateView();
	}
  //## end CGlobals::SetMesh%418836FA034C.body
}
*/
/*
void    CGlobals::SetMesh(int nVertexNum, double *x, double *y, double *z,
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
	
	//if(nTriangle)
	{
		int nCurMeshIndex = m_pCMy3dViewerDoc->m_nCurrentMeshIndex;
		
		if(m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex] != NULL)
			delete m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex];

		
        if(nGeoImgMeshDispType != 6)
		{
			C2DMesh *pMesh = new C2DMesh;
			m_pCMy3dViewerDoc->m_pCurrent2DMesh = pMesh;
     		m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex] = pMesh;

			((COpenGL3DEx *)(m_pCMy3dViewerDoc->m_pCOpenGL))->m_pCMesh = ( m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex] ); 
			((COpenGL3DEx *)(m_pCMy3dViewerDoc->m_pCOpenGL))->m_pCMesh->m_nType = 3; 

			
			( (C2DMesh *)m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex] )->SetMesh( nVertexNum, x, y, z,
			nTriangle, nV1, nV2, nV3,
			pAddPts,nAddPts,pAddFaces,nAddFaces,
			dfScale,			
			cutPath,
			vBndMapPnt,
			vInMapPnt,
			vBndPnt,
			vNewFaces,
			vRemeshFaces,
			vGeoMap,
			nGeoImgMeshDispType);

		}
		else
		{
			CMesh  *pMesh = new CMesh;
			m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex] = pMesh;
			m_pCMy3dViewerDoc->m_pCurrent2DMesh = NULL;

			((COpenGL3DEx *)(m_pCMy3dViewerDoc->m_pCOpenGL))->m_pCMesh = ( m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex] ); 
			((COpenGL3DEx *)(m_pCMy3dViewerDoc->m_pCOpenGL))->m_pCMesh->m_nType = 4; 

			m_pCMy3dViewerDoc->m_pMesh[nCurMeshIndex]->SetMesh( nTriangle, nV1, nV2, nV3,
													   NULL, NULL, NULL, 
													   NULL, NULL, NULL,
													   nVertexNum, x, y, z,
													   0,
													   "",
													   "",
													   "");
		}
		
		m_pCMy3dViewerDoc->m_strTitle[nCurMeshIndex] 
			= m_pCMy3dViewerDoc->m_strFileName + " " ;
						
		m_pCMy3dViewerDoc->m_nCurrentMeshIndex =  (m_pCMy3dViewerDoc->m_nCurrentMeshIndex + 1) % 2;
		m_pCMy3dViewerDoc->m_strTitle[m_pCMy3dViewerDoc->m_nCurrentMeshIndex] 
			= m_pCMy3dViewerDoc->m_strFileName + "*";
		
		m_pCMy3dViewerDoc->UpdateView();
	}
}
*/


// Additional Declarations
  //## begin CGlobals%40D7F2FA008F.declarations preserve=yes

  //## end CGlobals%40D7F2FA008F.declarations
//## begin module%40D7F3180092.epilog preserve=yes

/*  Convert HSV to RGB 
 *	Input:  h--hue; s---saturation; v---value
 *  Output: r--red; g---green ; b---blue
 *  Return: void
 *  added by xiedonghai,2004,10.26
 */ 

void HSV_To_RGB(float &r, float &g, float &b, float h, float s, float v)
{
	if (s == 0) //the color is on the black-and-white center line
	{
		if (h<0 || h>360)
		{
			r = v;
			g = v;
			b = v;
		}
		else
			AfxMessageBox("Hue must be given a value!");
	}
	else
	{
		float f = 0;
		float p = 0;
		float q = 0;
		float t = 0;
		int i = 0;
		
		if ( fabs(h-360.0) < 1e-6 )
			h = 0.0;
		h /= 60.0;
		i = (int)floor(h);
		f = h - i;
		p = v * ((float)(1.0) - s);
		q = v * ((float)(1.0) - s*f);
		t = v * ((float)(1.0) - s*((float)1.0 - f) );

		switch ( i )
		{
		case 0: 
			r = v;  g = t; b = p;			
			break;
		case 1:
			r = q;  g = v; b = p;
			break;
		case 2:
			r = p;  g = v; b = t;
			break;
		case 3:
			r = p;  g = q; b = v;
			break;
		case 4:
			r = t;  g = p; b = v;
			break;
		case 5:
			r = v;  g = p; b = q;
			break;
		default:
			break;
		}
	}	
}

float ComputeStretchRatio(float d3l, float d2l)
{
	float ratio = 0;
		
	float fE = (float)exp(double(1));
	float lfx = (float)fabs(d3l - d2l);

	ratio = float(( fE - pow(fE, (1/(lfx+1))) )/(fE-1));
	
	/*
	if(lfRatio > lfRatioMax)  
		lfRatioMax = lfRatio;
	if(lfRatio < lfRatioMin)  
		lfRatioMin = lfRatio;
	
	lfStat[nCnt++]= lfRatio;
	
	H = 240-(int)(lfRatio * 240)%240;
	H/=60.0;
	TEMP=floor(H);
	F = H -TEMP;
	P = I *(1.0 -S);
	Q = I * (1.0 - (S*F));
	T = I *(1.0 - (S *(1.0 -F)));
	switch (TEMP)
	{
	case 0 :
	    R =I; G =T; B =P; 
		break;
	case 1 :
		R =Q; G =I; B = P ;
		break;
	case 2 : 
		R= P; G = I; B = T; 
		break;
	case 3 :
		R = P; G =Q; B= I;
		break; 
	case 4 : 
		R = T; G = P; B= I;
		break;
	case 5 :
		R = I; G = P; B = Q;
		break;
	}
	*/

	return ratio;
}


void  ComputeSphereCoordi(float x, float y, float z, float &u, float &v)
{
	u = 0;
	v = 0;

	#define PI 3.14159265358979323846

	double r = sqrt( x*x + y*y + z*z );
	u = (float)acos(z / r);
		
	if(x == 0)
	{	
		if(y > 0)
			v = (float)(PI / 2);
		else 
			v = (float)(PI * 1.5);
		
		return;
	}
	
	float tv = (float)atan(y / x);

	if( y > 0)
	{
		if (x > 0)  // 1 
			v = tv;
		else
			v = (float)(tv + PI); // 2
	}
	else
	{
		if(x > 0)
			v = (float)(tv + 2*PI); // 4
		else
			v = (float)(tv + PI);   // 3 
	}
}
//## end module%40D7F3180092.epilog
