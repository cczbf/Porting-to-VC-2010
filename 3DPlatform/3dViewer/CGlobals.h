//## begin module%40D7F314015F.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40D7F314015F.cm

//## begin module%40D7F314015F.cp preserve=no
//## end module%40D7F314015F.cp

//## Module: CGlobals%40D7F314015F; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CGlobals.h

#ifndef CGlobals_h
#define CGlobals_h 1

//## begin module%40D7F314015F.additionalIncludes preserve=no
//## end module%40D7F314015F.additionalIncludes

//## begin module%40D7F314015F.includes preserve=yes
#include "stdafx.h"
#include "mainfrm.h"
#include "CTransformedImage.h"
#include "COpenGL3DEx.h"

class COpenGL3DEx;
class CMy3dViewerDoc;
class CDlgTab00;
class CDlgTab02;
class CMainFrame;


//## end module%40D7F314015F.includes

// 3dViewer
//#include "3dViewer.h"
//## begin module%40D7F314015F.declarations preserve=no

extern class CGlobals g_CGlobals;

#define WM_MYMESSAGE (WM_USER+100) 
#define WM_MYMESSAGE1 (WM_USER+101) 


//## end module%40D7F314015F.declarations

//## begin module%40D7F314015F.additionalDeclarations preserve=yes
//## end module%40D7F314015F.additionalDeclarations


//## begin CGlobals%40D7F2FA008F.preface preserve=yes
//## end CGlobals%40D7F2FA008F.preface

//## Class: CGlobals%40D7F2FA008F
//## Category: 3dViewer%40C7B40F0349
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CGlobals 
{
  //## begin CGlobals%40D7F2FA008F.initialDeclarations preserve=yes
  //## end CGlobals%40D7F2FA008F.initialDeclarations

  public:
    //## Constructors (generated)
      CGlobals();

    //## Destructor (generated)
      ~CGlobals();


    //## Other Operations (specified)
      //## Operation: set_m_pCBmpviewerDoc%40D8F1560381
      void set_m_pCBmpviewerDoc (CMy3dViewerDoc* pCDoc);
	  
	  void set_m_pCOpenGL3DEx (COpenGL3DEx* pC3DEx);

      //## Operation: get_m_pCBmpviewerDoc%40DA45150244
      CMy3dViewerDoc* get_m_pCBmpviewerDoc ();

	  COpenGL3DEx* get_m_pCOpenGL3DEx ();

      //## Operation: SetMeanGrayBeforeFFT%40DA4C90019D
      void SetMeanGrayBeforeFFT (double dblGray);

      //## Operation: set_m_pCMainFrame%40DA797A0242
      void set_m_pCMainFrame (CMainFrame* pCMainFrame);

	  /*
      //## Operation: SetMesh%418836FA034C
      void SetMesh (int nTriangle, int* nV1, int* nV2, int* nV3, 
					COLORREF* c12, COLORREF* c23, COLORREF* c31, 
					double *ratio12, double *ratio23, double *ratio31,
		            int nVlength, double* x, double* y, double* z, 	
					int nEdgeNodeNum,
					CString strAverage = "",
					CString strConviance = "",
					CString strGeoRatio = "",
					int nType = 1) ;
					*/
     /*
  	  void SetMesh(int nVertexNum, double *x, double *y, double *z,
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
					int nGeoImgMeshDispType=0);
					*/
					

      // Data Members for Class Attributes
      //## Attribute: m_pCMainFrame%40D7F74E0396
      //## begin CGlobals::m_pCMainFrame%40D7F74E0396.attr preserve=no  public: CMainFrame* {UA} NULL
      CMainFrame* m_pCMainFrame;
      //## end CGlobals::m_pCMainFrame%40D7F74E0396.attr

      //## Attribute: m_dblMeanGrayBeforeFFT%40DA4CC10356
      //## begin CGlobals::m_dblMeanGrayBeforeFFT%40DA4CC10356.attr preserve=no  public: double {UA} 0.0
      double m_dblMeanGrayBeforeFFT;
      //## end CGlobals::m_dblMeanGrayBeforeFFT%40DA4CC10356.attr

      //## Attribute: m_CTransformedImage%40DA773F01B5
      //## begin CGlobals::m_CTransformedImage%40DA773F01B5.attr preserve=no  public: CTransformedImage {UA} 
      CTransformedImage m_CTransformedImage;
      //## end CGlobals::m_CTransformedImage%40DA773F01B5.attr

    // Data Members for Associations

      //## Association: 3dViewer::<unnamed>%40D8F11A015E
      //## Role: CGlobals::m_pCMy3dViewerDoc%40D8F11A03C0
      //## begin CGlobals::m_pCMy3dViewerDoc%40D8F11A03C0.role preserve=no  public: CMy3dViewerDoc { -> RHgAN}
      CMy3dViewerDoc *m_pCMy3dViewerDoc;
	  COpenGL3DEx *m_pCOpenGL3DEx;
	  char g_chAbsoluteErr[256];
	  char g_chIncrementErr[256];
	  CDlgTab00 *m_pDlgTab00;
	  CDlgTab01 *m_pDlgTab01;
	  CDlgTab02 *m_pDlgTab02;
	  
      //## end CGlobals::m_pCMy3dViewerDoc%40D8F11A03C0.role

    // Additional Public Declarations
      //## begin CGlobals%40D7F2FA008F.public preserve=yes
	  //added by xiedonghai, 2004,11,2. To set new mesh
//	  void SetMesh( int nTriangle, int* nV1, int* nV2, int* nV3, 
//		            COLORREF* c12, COLORREF* c23, COLORREF* c31, 
//				   int nVlength, double* x = NULL, double* y = NULL, double* z = NULL);
      //## end CGlobals%40D7F2FA008F.public
  protected:
    // Additional Protected Declarations
      //## begin CGlobals%40D7F2FA008F.protected preserve=yes
      //## end CGlobals%40D7F2FA008F.protected

  private:
    // Additional Private Declarations
      //## begin CGlobals%40D7F2FA008F.private preserve=yes
      //## end CGlobals%40D7F2FA008F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CGlobals%40D7F2FA008F.implementation preserve=yes
      //## end CGlobals%40D7F2FA008F.implementation

};

//## begin CGlobals%40D7F2FA008F.postscript preserve=yes
void  HSV_To_RGB(float &r, float &g, float &b, float h, float s, float v);
float ComputeStretchRatio(float d3l, float d2l);
void  ComputeSphereCoordi(float x, float y, float z, float &u, float &v); //from 3D space coordinate to sphere coordinate 
//## end CGlobals%40D7F2FA008F.postscript

// Class CGlobals 

//## begin module%40D7F314015F.epilog preserve=yes
//## end module%40D7F314015F.epilog


#endif
