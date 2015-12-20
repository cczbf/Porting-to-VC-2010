 // 3dViewerDoc.cpp : implementation of the CMy3dViewerDoc class
//

#include "stdafx.h"
#include "3dViewer.h"

#include "3dViewerDoc.h"
#include "CGlobals.h"
#include "GraphicDialog.h"
#include "CFFT.h"
#include "CFWT.h"
#include "COpenGL3DEx.h"
#include "CGlobals.h"

#include "CMesh.h"
#include "Mesh3D.h"
#include "CBmpViewerData.h"
#include "COpenGL.h"
#include "COpenGL3D.h"
#include "CRelxationLabelling.h"

#include "MeshForGeoImg.h"

#include "DigitalGeometry.h"

#include "Triangles.h"
#include "float.h"

#include "MeshGlobal.h"

#include "io.h"

#include "SubdivideMesh.h"

#include "DlgSubdivideSetting.h"

#include "CDIBFlood.h"

struct SurfacePt
{
	double u,v;
	float r;
};



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy3dViewerDoc

CRelxationLabelling *CMy3dViewerDoc::m_pCRelxationLabelling = NULL;

#define PI 3.14159265358979323846


IMPLEMENT_DYNCREATE(CMy3dViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy3dViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CMy3dViewerDoc)
	ON_COMMAND(IDM_EDIT_INVERSE, OnEditInverse)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_EQUALIZE, OnEditEqualize)
	ON_COMMAND(ID_EDIT_FourierTransform, OnEDITFourierTransform)
	ON_COMMAND(ID_EDIT_InverseWaveletTransform, OnEDITInverseWaveletTransform)
	ON_COMMAND(ID_EDIT_WaveletTransform, OnEDITWaveletTransform)
	ON_COMMAND(IDM_EDIT_DAUB12, OnEditDaub12)
	ON_COMMAND(IDM_EDIT_DAUB20, OnEditDaub20)
	ON_COMMAND(IDM_EDIT_DAUB4, OnEditDaub4)
	ON_COMMAND(IDM_EDIT_FFT_PERIDOGRAM, OnEditFftPeridogram)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_DAUB12, OnUpdateEditDaub12)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_DAUB20, OnUpdateEditDaub20)
	ON_UPDATE_COMMAND_UI(IDM_EDIT_DAUB4, OnUpdateEditDaub4)
	ON_COMMAND(IDM_FILE_INFO, OnFileInfo)
	ON_COMMAND(IDC_EDIT_GRAPHIC, OnEditGraphic)
	ON_COMMAND(IDC_EDIT_FILTER, OnEditFilter)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_CHANGE_MESH_COLOR, OnChangeMeshColor)
	ON_COMMAND(ID_DEBUG_RL_SETMESH, OnDebugRlSetmesh)
	ON_COMMAND(IDM_SetMeshSphere, OnSetMeshSphere)
	ON_COMMAND(IDM_SAVE_OBJ_AS_PLY, OnSaveObjAsPly)
	ON_COMMAND(IDM_MESH_SUBDIVISION, OnMeshSubdivision)
	ON_COMMAND(IDM_CONSTRUCT_MODEL_CUBE, OnConstructModelCube)
	ON_COMMAND(IDM_MODEL_HEXAHEDRON, OnModelHexahedron)
	ON_COMMAND(IDM_MODEL_RESOLVE, OnModelResolve)
	ON_COMMAND(IDM_MODEL_EXTRUDE, OnModelExtrude)
	ON_COMMAND(IDM_MESH_AVERAGE, OnMeshAverage)
	ON_COMMAND(IDM_FLOOD_FILL, OnFloodFill)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy3dViewerDoc construction/destruction

CMy3dViewerDoc::CMy3dViewerDoc()
{
	// TODO: add one-time construction code here

	m_CBmpViewerData = new CBmpViewerData;
	g_CGlobals.set_m_pCBmpviewerDoc((CMy3dViewerDoc*)this);
	m_pCOpenGL = (COpenGL*)new COpenGL3DEx;

	m_pMesh[0] = new CMesh;
	m_pMesh[1] = new CMesh;

	m_nCurrentMeshIndex = 0;

	m_strFileName = " ";

	m_bIsNew = FALSE;

	m_bIsShow3DDepth = FALSE;

	m_strTitle[0] = " New !";
	m_strTitle[1] = " New !";

	m_pMeshForGeoImg = NULL;
	m_pCurrent2DMesh = NULL;
	m_pCRelxationLabelling = NULL;
	
	
}

CMy3dViewerDoc::~CMy3dViewerDoc()
{
		if ( m_CBmpViewerData != NULL )
		{
			delete m_CBmpViewerData;
			m_CBmpViewerData = NULL;
		}
//	if(pScreenDatas)
//		delete pScreenDatas;
		if ( m_pCOpenGL != NULL )
		{
			delete m_pCOpenGL;
			this->m_pCOpenGL = NULL;
		}
		if (m_pMesh != NULL)
		{
			delete m_pMesh[0];
			delete m_pMesh[1];
		}
		
		if(m_pMeshForGeoImg != NULL)
			delete m_pMeshForGeoImg;

		if(m_pCRelxationLabelling != NULL)
			delete m_pCRelxationLabelling;
}

BOOL CMy3dViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMy3dViewerDoc serialization

void CMy3dViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMy3dViewerDoc diagnostics

#ifdef _DEBUG
void CMy3dViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy3dViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMy3dViewerDoc commands


BOOL CMy3dViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;
	
	// TODO: Add your specialized creation code here
	HANDLE hDib;
	hDib = m_CDIB.OpenDIB((LPSTR)lpszPathName);

	int w,h;
	HANDLE hDib0 = m_CDIB.CopyDib(hDib);
	HANDLE hRaw = m_CDIB.Dib2BWRaw8(hDib0,&w,&h);//hDib0 is freed in this call

	//m_bIsShow3DDepth = true;
	if (m_bIsShow3DDepth)
	{	
		if(m_pMesh[m_nCurrentMeshIndex] != NULL)
			delete m_pMesh[m_nCurrentMeshIndex];
		
		m_pMesh[m_nCurrentMeshIndex] = new CMesh;

		((COpenGL3DEx *)(m_pCOpenGL))->m_pCMesh = m_pMesh[m_nCurrentMeshIndex]; 
		((COpenGL3DEx *)(m_pCOpenGL))->m_pCMesh->m_nType = 0;
		
		((COpenGL3DEx *)(this->m_pCOpenGL))->m_pCMesh->LoadFromRaw(hRaw, w, h);
		((COpenGL3DEx *)(this->m_pCOpenGL))->m_pCMesh->Normalize(0.5f);
		GlobalFree ( hRaw );
		
		m_nCurrentMeshIndex = (m_nCurrentMeshIndex + 1) % 2;

		// Borrowing the following functions to initize the COpenGL
		hRaw = GlobalAlloc(GHND, 4*sizeof(BYTE));
		((COpenGL3DEx *)(this->m_pCOpenGL))->Load3DData(hRaw, 2, 2, 0.0, 0.0, 0.0, 0.0);
		GlobalFree ( hRaw );		
	}
	if ( hDib != NULL )
	{
		m_CBmpViewerData->SetNewPosition();
		m_CBmpViewerData->SetDIBHandle(hDib);
		//m_CDIBOrg.FreeDib();
		this->UpdateView();
		this->SetPathName(lpszPathName);
	}
	
	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd( );
	pMainFrame->Set2DView();

	return TRUE;

}

BOOL CMy3dViewerDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
		// TODO: Add your specialized code here and/or call the base class
	HANDLE hDib	= m_CBmpViewerData->GetDIBHandle ();
	if ( hDib != NULL )
	{
		if (strstr(lpszPathName, ".bmp"))
		{
		  m_CDIB.WriteDIB((LPSTR)lpszPathName, hDib);
		  return TRUE;
		}
		else
			if (strstr(lpszPathName, ".ply"))
			{
				((COpenGL3DEx *)(this->m_pCOpenGL))->m_pCMesh->Write(lpszPathName);
			}
			else
		{
			MessageBeep(MB_ICONASTERISK);
			return FALSE;
		}
	}
	else
	{
		((COpenGL3DEx *)(this->m_pCOpenGL))->m_pCMesh->Write(lpszPathName);
	}

	
	//	return CDocument::OnSaveDocument(lpszPathName);
	return FALSE;	
}

BOOL CMy3dViewerDoc::UpdateView ()
{
// This example uses CDocument::GetFirstViewPosition
// and GetNextView to repaint each view.void CMyDoc::OnRepaintAllViews()
   POSITION pos = GetFirstViewPosition();
   BOOL bRet	= FALSE;
   while (pos != NULL)
   {
      CView* pView = GetNextView(pos);
	  pView->Invalidate(FALSE);
	  pView->UpdateWindow();
	  bRet	= TRUE;
   }
//   g_CGlobals.m_pCMainFrame->SetFocus();

   return bRet;
}


void CMy3dViewerDoc::TransferByBuffer(HANDLE hHtArray,
									 DWORD dwWidth, 
									 DWORD dwHeight,
									 DWORD dwScaleX,
									 DWORD dwScaleY,
									 DWORD dwLevels,
									 DWORD dwLimitLevels
									 )
{
	
	HANDLE hDib;
	m_CDIB.set_m_dwWidth(dwWidth); 
    m_CDIB.set_m_dwHeight(dwHeight);
	m_CDIB.set_m_dwScaleX(dwScaleX);
	m_CDIB.set_m_dwScaleY(dwScaleY);
	m_CDIB.set_m_dwLevels(dwLevels);
	m_CDIB.set_m_dwLimitLevels(dwLimitLevels);

	hDib = m_CDIB.HtArray2Dib(hHtArray);
	if ( hDib != NULL )
	{
		m_CBmpViewerData->SetDIBHandle(hDib);
		//m_CDIBOrg.FreeDib();
		this->UpdateView();
	}
	
	GenerateMeshFromImg();
}


void CMy3dViewerDoc::OnEditInverse() 
{
	// TODO: Add your command handler code here
	    HANDLE	hDib0	= m_CBmpViewerData->GetDIBHandle();

		BYTE	pbMap[256];
		for ( int i = 0; i < 256; i++ )
			pbMap[i]	= 255- i;

		HANDLE	hDib1	= m_CDIB.CopyDib(hDib0);

		m_CDIB.DIBCurveMapping(hDib1, hDib0, pbMap);

		m_CBmpViewerData->SetDIBHandle(NULL);// call this function with NULL to delete hDib0
		hDib0 = NULL;
		
		m_CBmpViewerData->SetDIBHandle(hDib1);

		GenerateMeshFromImg();

		this->UpdateView();	
}

void CMy3dViewerDoc::OnEditUndo() 
{
	// TODO: Add your command handler code here

	int nToolID = 0;

	if (g_CGlobals.m_pCMainFrame->m_pCToolBase != NULL)
		nToolID = g_CGlobals.m_pCMainFrame->m_pCToolBase->get_m_nToolID();
	
	if( nToolID != 3)
	{
		m_CBmpViewerData->Undo();// call this function with NULL to delete hDib0
	}
	else
	{	
		((COpenGL3DEx *)(m_pCOpenGL))->m_pCMesh = (m_pMesh[m_nCurrentMeshIndex] ); 
		m_nCurrentMeshIndex = (m_nCurrentMeshIndex + 1)%2;
        
		if(g_CGlobals.m_pDlgTab02 != NULL)
			g_CGlobals.m_pDlgTab02->Invalidate();		
	}

	this->UpdateView();		
}

void CMy3dViewerDoc::OnEditEqualize() 
{
	// TODO: Add your command handler code here
		HANDLE	hDib0	= m_CBmpViewerData->GetDIBHandle();

		DWORD	pdwHistoR[256],	pdwHistoG[256],	pdwHistoB[256];
		BYTE	pbHEqMapR[256],	pbHEqMapG[256],	pbHEqMapB[256];

		WORD nW = m_CDIB.DIBWidth(hDib0);
		WORD nH = m_CDIB.DIBHeight(hDib0);

		m_CDIB.Histogram( hDib0, pdwHistoR, pdwHistoG, pdwHistoB);
		m_CDIB.HistogramEqualization(pdwHistoR, pbHEqMapR, nW, nH);
		m_CDIB.HistogramEqualization(pdwHistoG, pbHEqMapG, nW, nH);
		m_CDIB.HistogramEqualization(pdwHistoB, pbHEqMapB, nW, nH);

		HANDLE	hDib1	= m_CDIB.CopyDib(hDib0);

		m_CDIB.DIBCurveMapping(hDib1, hDib0, pbHEqMapR, pbHEqMapG, pbHEqMapB);

		m_CBmpViewerData->SetDIBHandle(NULL);// call this function with NULL to delete hDib0
		hDib0 = NULL;

		m_CBmpViewerData->SetDIBHandle(hDib1);

		GenerateMeshFromImg();

		this->UpdateView();	
}

void CMy3dViewerDoc::OnEDITFourierTransform() 
{
	// TODO: Add your command handler code here
	CFFT CFFT;
	HANDLE hDib = this->m_CBmpViewerData->GetDIBHandle();

//	_int32 nW, nH;
	int nW, nH;

	HANDLE hRaw = this->m_CDIB.Dib2BWRaw16(hDib, &nW, &nH);
	
	CRawImage16 CRawImage16;

	CRawImage16.Copy (hRaw, nW, nH);

	CFFT.SetWorkingImage(&CRawImage16);
	g_CGlobals.SetMeanGrayBeforeFFT (CFFT.MeanGrayOf_image());
	CFFT.FFT2D();
	CFFT.CopyMagnitude();
//	CFFT.CopyPeridogram();

	this->TransferByBuffer(
				CRawImage16.m_hPixels,
				CRawImage16.m_nWidth,
				CRawImage16.m_nHeight,
				0,0,0,0
		);

	CFFT.SetWorkingImage(NULL);	
    
}

void CMy3dViewerDoc::OnEDITInverseWaveletTransform() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here

	CFWT CFWT(g_CGlobals.m_CTransformedImage.m_nTransformFilterID);
	HANDLE hDib = this->m_CBmpViewerData->GetDIBHandle();

	int nW, nH;

	HANDLE hRaw = this->m_CDIB.Dib2BWRaw16(hDib, &nW, &nH);
	
	CRawImage16 CRawImage16;

	CRawImage16.Copy (hRaw, nW, nH);

	CFWT.SetWorkingImage(&CRawImage16);
	
	_int32 nW1, nH1;

	float *lpData =
	g_CGlobals.m_CTransformedImage.GetData(&nW1, &nH1);

	CFWT.SetReal4Transform(lpData, nW1, nH1);
	CFWT.IFWT2D();
	CFWT.CopyWT(FALSE);//Not visualize the result in decibel(分贝) Format the result and copy to CRawImage buffer
//	CFFT.CopyPeridogram();

	this->TransferByBuffer(
				CRawImage16.m_hPixels,
				CRawImage16.m_nWidth,
				CRawImage16.m_nHeight,
				0,0,0,0
		);

	CFWT.SetWorkingImage(NULL);
	
	
}

void CMy3dViewerDoc::OnEDITWaveletTransform() 
{
	// TODO: Add your command handler code here
	
	CFWT CFWT(g_CGlobals.m_CTransformedImage.m_nTransformFilterID);
	HANDLE hDib = this->m_CBmpViewerData->GetDIBHandle();

	int nW, nH;

	HANDLE hRaw = this->m_CDIB.Dib2BWRaw16(hDib, &nW, &nH);
	
	CRawImage16 CRawImage16;

	CRawImage16.Copy (hRaw, nW, nH);
	CRawImage16.ExpandToPowerOf2Dim((unsigned __int32*)&nW, (unsigned __int32*)&nH);

	CFWT.SetWorkingImage(&CRawImage16);

	CFWT.FWT2D();
	CFWT.CopyWT();//Format the result and copy to CRawImage buffer

	long nM,nN;

	float *zr = CFWT.GetWTReal(&nM,&nN);
	g_CGlobals.m_CTransformedImage.SetData(zr,(__int32)nW, (__int32)nH);//Store the transform for reconstruct

	this->TransferByBuffer(
				CRawImage16.m_hPixels,
				CRawImage16.m_nWidth,
				CRawImage16.m_nHeight,
				0,0,0,0
		);

	CFWT.SetWorkingImage(NULL);

}

void CMy3dViewerDoc::OnEditDaub12() 
{
	// TODO: Add your command handler code here
g_CGlobals.m_CTransformedImage.m_nTransformFilterID	= 2;	
}

void CMy3dViewerDoc::OnEditDaub20() 
{
	// TODO: Add your command handler code here
	g_CGlobals.m_CTransformedImage.m_nTransformFilterID	= 3;
}

void CMy3dViewerDoc::OnEditDaub4() 
{
	// TODO: Add your command handler code here
g_CGlobals.m_CTransformedImage.m_nTransformFilterID	= 0;	
}

void CMy3dViewerDoc::OnEditFftPeridogram() 
{
	// TODO: Add your command handler code here
	CFFT CFFT;
	HANDLE hDib = this->m_CBmpViewerData->GetDIBHandle();

//	_int32 nW, nH;
	int nW, nH;

	HANDLE hRaw = this->m_CDIB.Dib2BWRaw16(hDib, &nW, &nH);
	
	CRawImage16 CRawImage16;

	CRawImage16.Copy (hRaw, nW, nH);

	CFFT.SetWorkingImage(&CRawImage16);
	g_CGlobals.SetMeanGrayBeforeFFT (CFFT.MeanGrayOf_image());
	CFFT.FFT2D();
//	CFFT.CopyMagnitude();
	CFFT.CopyPeridogram();
	this->TransferByBuffer(
				CRawImage16.m_hPixels,
				CRawImage16.m_nWidth,
				CRawImage16.m_nHeight,
				0,0,0,0
		);

	CFFT.SetWorkingImage(NULL);
		
}

void CMy3dViewerDoc::OnUpdateEditDaub12(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
pCmdUI->Enable( TRUE );
	if ( g_CGlobals.m_CTransformedImage.m_nTransformFilterID	== 2 )
		pCmdUI->SetCheck( 1 );
	else
		pCmdUI->SetCheck( 0 );
		
}

void CMy3dViewerDoc::OnUpdateEditDaub20(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable( TRUE );
	if ( g_CGlobals.m_CTransformedImage.m_nTransformFilterID	== 3 )
		pCmdUI->SetCheck( 1 );
	else
		pCmdUI->SetCheck( 0 );
	
}

void CMy3dViewerDoc::OnUpdateEditDaub4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
pCmdUI->Enable( TRUE );
	if ( g_CGlobals.m_CTransformedImage.m_nTransformFilterID	== 0 )
		pCmdUI->SetCheck( 1 );
	else
		pCmdUI->SetCheck( 0 );
	
}

void CMy3dViewerDoc::OnFileInfo() 
{
	// TODO: Add your command handler code here

	    HANDLE	hDib0	= m_CBmpViewerData->GetDIBHandle();

		WORD nW = m_CDIB.DIBWidth(hDib0);
		WORD nH = m_CDIB.DIBHeight(hDib0);
		CString	CStr;
		
		CStr.Format("Width:%d\r\nHeight:%d", nW,nH );
		AfxGetMainWnd()->MessageBox(CStr, "图象信息", MB_OK);
	
}

void CMy3dViewerDoc::OnEditGraphic() 
{
	// TODO: Add your command handler code here
	CGraphicDialog dlg ;
	dlg.DoModal() ;
}

void CMy3dViewerDoc::OnEditFilter() 
{
	// TODO: Add your command handler code here

    HANDLE	hSrcDib	= m_CBmpViewerData->GetDIBHandle();
	HANDLE	hDstDib	= m_CDIB.CopyDib(hSrcDib);

	double * hMap = (double *) new BYTE[5 * 5 * sizeof(double)] ;

	double dF[5], dFt[5] ;
	dF[0] = dFt[0] = 1.0 / 24.0 ;
	dF[1] = dFt[1] = 5.0 / 24.0 ;
	dF[2] = dFt[2] = 12.0 / 24.0 ;
	dF[3] = dFt[3] = 5.0 / 24.0 ;
	dF[4] = dFt[4] = 1.0 / 24.0 ;

	for(int i = 0 ; i < 5 ; i ++)
	{
		for(int j = 0 ; j < 5 ; j ++)
			hMap[i * 5 + j] = dF[i] * dFt[j] ;
	}
	m_CDIB.LowFilterDib(hDstDib, hSrcDib,hMap) ;

	m_CBmpViewerData->SetDIBHandle(NULL);// call this function with NULL to delete hSrcDib
	hSrcDib = NULL;
		
	m_CBmpViewerData->SetDIBHandle(hDstDib);

	GenerateMeshFromImg();

	this->UpdateView();
	
}

void CMy3dViewerDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here

	char strFilter[]="Bitmap files(*.bmp)|*.bmp|Ply file(*.ply)|*.ply|All files(*.*)|*.*|";

	CFileDialog fDlg(FALSE,"bmp","*.bmp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);

	CString    fName;

	if(fDlg.DoModal()==IDOK)
	{
		fName=fDlg.GetPathName();
		OnSaveDocument((LPCTSTR)fName);
	}	
}

void CMy3dViewerDoc::OnChangeMeshColor() 
{
	// TODO: Add your command handler code here
	
	//get the new mesh
	int nVertexNum = 0;
	int nTriangleNum = 0;
   	nTriangleNum = ((COpenGL3DEx *)m_pCOpenGL)->m_pCMesh->m_CTriangleArray.GetSize();
	nVertexNum = ((COpenGL3DEx *)m_pCOpenGL)->m_pCMesh->m_CVertexArray.GetSize();

	int *v1 = new int[nTriangleNum];
	int *v2 = new int[nTriangleNum];
	int *v3 = new int[nTriangleNum];
	COLORREF *c12 = new COLORREF[nTriangleNum];
	COLORREF *c23 = new COLORREF[nTriangleNum];
	COLORREF *c31 = new COLORREF[nTriangleNum];

	for (int i=0; i<nTriangleNum; i++ )
	{
		v1[i] =  ((COpenGL3DEx *)m_pCOpenGL)->m_pCMesh->m_CTriangleArray[i].pnV[0];
		v2[i] =  ((COpenGL3DEx *)m_pCOpenGL)->m_pCMesh->m_CTriangleArray[i].pnV[1];
		v3[i] =  ((COpenGL3DEx *)m_pCOpenGL)->m_pCMesh->m_CTriangleArray[i].pnV[2];

		c12[i] = RGB(255,0,0);
		c23[i] = RGB(0,0,255);
		c31[i] = RGB(0,255,0);	
	}

	double *x = new double[nVertexNum];
	double *y = new double[nVertexNum];
	double *z = new double[nVertexNum];

	for (int i=0; i<nVertexNum; i++)
	{
		x[i] =  ((COpenGL3DEx *)m_pCOpenGL)->m_pCMesh->m_CVertexArray[i].dX;
		y[i] =  ((COpenGL3DEx *)m_pCOpenGL)->m_pCMesh->m_CVertexArray[i].dY;
		z[i] =  ((COpenGL3DEx *)m_pCOpenGL)->m_pCMesh->m_CVertexArray[i].dZ;
	}

//	if ( ((COpenGL3DEx *)m_pCOpenGL)->m_pNewMesh == NULL)
//		((COpenGL3DEx *)m_pCOpenGL)->m_pNewMesh = new CMesh;
//	else
//		delete ((COpenGL3DEx *)m_pCOpenGL)->m_pNewMesh;
//	
//	((COpenGL3DEx *)m_pCOpenGL)->m_pNewMesh->SetMesh(nVertexNum, v1, v2, v3,
//		                                             nTriangleNum, c12, c23, c31,
//													 x,y,z); 

	//g_CGlobals.SetMesh(nTriangleNum,v1, v2, v3,
     //          c12, c23, c31, nVertexNum, x,y,z);

	delete []v1;
	delete []v2;
	delete []v3;

	delete []c12;
	delete []c31;
	delete []c23;

	delete x;
	delete y;
	delete z;

	m_bIsNew = TRUE;
	((COpenGL3DEx *)m_pCOpenGL)->SetMeshFlag(TRUE);

	UpdateView();
}

void CMy3dViewerDoc::DoAddMesh(CMesh *pMesh)
{
	//m_MeshCollection.Add(pMesh);
	UpdateView();	
}

void CMy3dViewerDoc::DoRemoveMesh(CMesh *pMesh)
{
	//m_MeshCollection.Remove(pMesh);
	UpdateView();
}

void CMy3dViewerDoc::OnDebugRlSetmesh() 
{
	// TODO: Add your command handler code here
	//m_RelaxtionLabel.SetMesh();	
}

void CMy3dViewerDoc::OnSetMeshSphere() 
{
	// TODO: Add your command handler code here
	
	m_pCRelxationLabelling->SetMesh();
	//((COpenGL3DEx *)m_pCOpenGL)->m_pCMesh->m_pCRelxationLabelling->SetMesh();		
}

void CMy3dViewerDoc::OnSaveObjAsPly() 
{
	// TODO: Add your command handler code here
	
	if( ( (COpenGL3DEx *)(m_pCOpenGL) )->m_pCMesh != NULL )
	{		
		char strFilter[]="Ply file(*.ply)|*.ply||";
		CFileDialog fDlg(0,"ply","*.ply",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);
		fDlg.m_ofn.lpstrTitle="Save As...";
		
		if(fDlg.DoModal()==IDOK)
		{
			CString strFileName = fDlg.GetPathName();
			//if( ! ((COpenGL3DEx *)(m_pCOpenGL))->m_pCMesh->m_ObjModel.SaveAsPLY(strFileName) )
			//	AfxMessageBox(" This is not OBJ model type! ");
		}
	}
}

void CMy3dViewerDoc::SetEmptyRL()
{
	if (m_pCRelxationLabelling != NULL)
		delete m_pCRelxationLabelling;

//	m_pCRelxationLabelling = new CRelxationLabelling; 
 	m_pCRelxationLabelling = new CRelxationLabelling ; 
//	m_pCRelxationLabelling = new CRelxationLabelling2; 

}

void CMy3dViewerDoc::OutputGeoimageIntoImgBuffer()
{
	if(m_pMeshForGeoImg != NULL)
	{
		int nWid = m_pMeshForGeoImg->GetGeoimageWid();
		int nHei = m_pMeshForGeoImg->GetGeoimageHei();

		float *pGeoimageR = new float[nHei*nWid];
		float *pGeoimageG = new float[nHei*nWid];
		float *pGeoimageB = new float[nHei*nWid];

		float fMin = 1000;
		float fMax = -1000;
		
		float r,g,b;

		for(int i=0; i<nHei; i++)
			for(int j=0; j<nWid; j++)
			{
				m_pMeshForGeoImg->OutputGeoimagePixel(i,j,r,g,b);
                
				if( r < fMin )
					fMin = r;
				if( g < fMin )
					fMin = g;
				if( b < fMin )
					fMin = b;

				if( r > fMax )
					fMax = r;
				if( g > fMax )
					fMax = g;
				if( b > fMax )
					fMax = b;
			}	
			
			for(int i=0; i<nHei; i++)
				for(int j=0; j<nWid; j++)
				{
					float r,g,b;
					m_pMeshForGeoImg->OutputGeoimagePixel(i,j,r,g,b);

					//TRACE("r: %f  g: %f b: %f \n", r, g, b);
					
					//convert to RGB
					pGeoimageR[i*nWid + j] = ((r-fMin)/(fMax-fMin));
					pGeoimageG[i*nWid + j] = ((g-fMin)/(fMax-fMin));
					pGeoimageB[i*nWid + j] = ((b-fMin)/(fMax-fMin));
				}

			//interpolation of Geoimage
			int nScale = 5;
			BYTE *pInterpolateGeoimage = new BYTE[nWid*nHei*nScale*nScale*3];
            
			for(int i=0; i<(nHei)*nScale  ; i++)
				for(int j=0; j<(nWid)*nScale ; j++)
				{
		
					int l,r,t,b;
					l = j/nScale;
					t = i/nScale;
					
					if( l>=(nWid-1) )  
						l = nWid-2;
					if( t>=(nHei-1) )
						t = nHei-2;

					r = l+1;
					b = t+1;
                    
					float tlr,tlg,tlb;
					float trr,trg,trb;
					float blr,blg,blb;
					float brr,brg,brb;
					
					tlr = pGeoimageR[t*nWid + l];
					tlg = pGeoimageG[t*nWid + l];
					tlb = pGeoimageB[t*nWid + l];

					trr = pGeoimageR[t*nWid + r];
					trg = pGeoimageG[t*nWid + r];
			    	trb = pGeoimageB[t*nWid + r];

					blr = pGeoimageR[b*nWid + l];
					blg = pGeoimageG[b*nWid + l];
					blb = pGeoimageB[b*nWid + l];

					brr = pGeoimageR[b*nWid + r];
					brg = pGeoimageG[b*nWid + r];
					brb = pGeoimageB[b*nWid + r];

					float xOff = (float)(j%nScale)/(float)(nScale);
					float yOff = (float)(i%nScale)/(float)(nScale);
					
					float ir,ig,ib;
					
					//linear interpolation
					ir = ( tlr*(1-xOff)*(1-yOff) + trr*xOff*(1-yOff) + blr*(1-xOff)*yOff + brr*xOff*yOff )*255;
					ig = ( tlg*(1-xOff)*(1-yOff) + trg*xOff*(1-yOff) + blg*(1-xOff)*yOff + brg*xOff*yOff )*255;
					ib = ( tlb*(1-xOff)*(1-yOff) + trb*xOff*(1-yOff) + blb*(1-xOff)*yOff + brb*xOff*yOff )*255;

					//TRACE("ir: %f  ig: %f ib: %f \n", ir, ig, ib);

					pInterpolateGeoimage[i*nWid*nScale*3 + j*3] = ib;
					pInterpolateGeoimage[i*nWid*nScale*3 + j*3 + 1] = ig;
					pInterpolateGeoimage[i*nWid*nScale*3 + j*3 + 2] = ir;					
				}
				
				HANDLE hDib = m_CDIB.CreateDibFromRawData(nWid*nScale, nHei*nScale, pInterpolateGeoimage);
				
				delete pGeoimageR;
				delete pGeoimageG;
				delete pGeoimageB;
				delete pInterpolateGeoimage;

				if ( hDib != NULL )
				{
					m_CBmpViewerData->SetNewPosition();
					m_CBmpViewerData->SetDIBHandle(hDib);
					UpdateView();										
				}				
	}
}

void CMy3dViewerDoc::OnMeshSubdivision() 
{
	// TODO: Add your command handler code here
    CWaitCursor wt;

	int nCurrentIndex = (m_nCurrentMeshIndex + 1) % 2;    	
	CMesh *pSubdivisionMesh = SubdivisionMesh( *(m_pMesh[nCurrentIndex] ) );

	if(pSubdivisionMesh != NULL)
	{
		pSubdivisionMesh->m_nType = 0;	
		SetNewMesh(pSubdivisionMesh);		
		UpdateView();
	}
}

void CMy3dViewerDoc::GenerateMeshFromImg()
{

	/*
	//{{Added by xdh,05.1.4. Generate the mesh from image
	if (m_bIsShow3DDepth)
	{	
		if(m_pMesh[m_nCurrentMeshIndex] != NULL)
			delete m_pMesh[m_nCurrentMeshIndex];
		m_pMesh[m_nCurrentMeshIndex] = new CMesh;
   		((COpenGL3DEx *)(m_pCOpenGL))->m_pCMesh = m_pMesh[m_nCurrentMeshIndex]; 
		
		((COpenGL3DEx *)(m_pCOpenGL))->m_pCMesh->m_nType = 0;
		
        HANDLE hDib = m_CBmpViewerData->get_m_hDib();
		int w = m_CBmpViewerData->get_m_w0();
		int h = m_CBmpViewerData->get_m_h0();
		HANDLE hDib0 = m_CDIB.CopyDib(hDib);
     	HANDLE hRaw = m_CDIB.Dib2BWRaw8(hDib0,&w,&h);//hDib0 is freed in this call
		((COpenGL3DEx *)(this->m_pCOpenGL))->m_pCMesh->LoadFromRaw(hRaw, w, h);
		((COpenGL3DEx *)(this->m_pCOpenGL))->m_pCMesh->Normalize(0.5f);
		GlobalFree ( hRaw );	
		
		m_nCurrentMeshIndex = (m_nCurrentMeshIndex + 1) % 2;

		// Borrowing the following functions to initize the COpenGL
		HANDLE hRaw1 = GlobalAlloc(GHND, 4*sizeof(BYTE));
		((COpenGL3DEx *)(this->m_pCOpenGL))->Load3DData(hRaw1, 2, 2, 0.0, 0.0, 0.0, 0.0);
		GlobalFree ( hRaw1 );		
	}
	//}}Added by xdh,05.1.4.
	*/
}

void CMy3dViewerDoc::OnConstructCube() 
{
	// TODO: Add your command handler code here

}

void CMy3dViewerDoc::OnConstructModelCube() 
{
	// TODO: Add your command handler code here
	
	//////////////////////////////////////////////////////////////////////////
	float Length = 1; //the width of cube
    float radius = sqrt( Length*Length*2) / 2;

	double tx[8];
	double ty[8];
	double tz[8];
	
	for(int i=0; i<4; i++)
	{
		float angle = (i*90.0)/(180.0) * PI;
		tx[i] = radius * cos(angle);
		ty[i] = radius * sin(angle); 
		tz[i] = Length/2;

		tx[i+4] = tx[i];
		ty[i+4] = ty[i];
		tz[i+4] = -Length/2;		
	}

	int va[12],vb[12],vc[12];
	va[0] = 0; vb[0] = 4; vc[0] = 5;
	va[1] = 1; vb[1] = 0; vc[1] = 5;
	
	va[2] = 1; vb[2] = 5; vc[2] = 6;
	va[3] = 2; vb[3] = 1; vc[3] = 6;

	va[4] = 2; vb[4] = 6; vc[4] = 7;
	va[5] = 3; vb[5] = 2; vc[5] = 7;

	va[6] = 3; vb[6] = 7; vc[6] = 4;
	va[7] = 0; vb[7] = 3; vc[7] = 4;

	va[8] = 3; vb[8] = 0; vc[8] = 1;
	va[9] = 2; vb[9] = 3; vc[9] = 1;

	va[10] = 4; vb[10] = 7; vc[10] = 6;
	va[11] = 5; vb[11] = 4; vc[11] = 6;
    //////////////////////////////////////////////////////////////////////////
    
	CMesh *pMesh = new CMesh;
	if(pMesh == NULL)
		return;

	pMesh->SetMesh(8, tx, ty, tz, 12, va, vb, vc);
	pMesh->m_nType = 0;
	
	m_strFileName = "Cube";

	SetNewMesh(pMesh);
   
	UpdateView();
}


void CMy3dViewerDoc::OnModelHexahedron() 
{
	// TODO: Add your command handler code here
	float  radius = 1;

	double tx[5];
	double ty[5];
	double tz[5];

	int va[6], vb[6], vc[6];

	tx[0] = 0; ty[0] = 0; tz[0] = radius;
	tx[1] = 0; ty[1] = 0; tz[1] = -radius;
	tx[2] = 0; ty[2] = 1; tz[2] = 0;
	
	tx[3] = radius * sin( (120.0/180.0)*PI); 
	ty[3] = radius * cos( (120.0/180.0)*PI);
	tz[3] = 0;
	
	tx[4] = radius * sin( (240.0/180.0)*PI);  
	ty[4] = radius * cos( (240.0/180.0)*PI);
	tz[4] = 0;	
	

	va[0] = 0; vb[0] = 3; vc[0] = 2;
	va[1] = 0; vb[1] = 2; vc[1] = 4;
	va[2] = 0; vb[2] = 4; vc[2] = 3;
	va[3] = 4; vb[3] = 1; vc[3] = 3;
	va[4] = 3; vb[4] = 1; vc[4] = 2;
	va[5] = 2; vb[5] = 1; vc[5] = 4;
	  
	CMesh *pMesh = new CMesh;
	if(pMesh == NULL)
		return;

	pMesh->SetMesh(5, tx, ty, tz, 6, va, vb, vc);
	pMesh->m_nType = 0;
	m_strFileName = "Hexahedron";
	SetNewMesh(pMesh);
   
	UpdateView();
}


void CMy3dViewerDoc::SetNewMesh(CMesh *pMesh)
{	
	if(pMesh == NULL)
		return;

	//set the result to the our Back 3D Buffer
	if(m_pMesh[m_nCurrentMeshIndex] != NULL)
		delete m_pMesh[m_nCurrentMeshIndex];
	
	m_pMesh[m_nCurrentMeshIndex] = pMesh;
	((COpenGL3DEx *)(m_pCOpenGL))->m_pCMesh = pMesh; 

	m_strTitle[m_nCurrentMeshIndex] = m_strFileName;
	
	////set data to CRL
	g_CGlobals.m_pDlgTab00->m_nPass = 0;	
	pMesh->Normalize();
	double scale = pMesh->ComputeScale(4*PI);   
	pMesh->Normalize(float(1.0)/(float)scale);
    if ( pMesh->GetVertexNum() < 5000)
	{
		SetEmptyRL(); //produce a new CRL object	  	
		CRelxationLabelling *pCRelxationLabelling = m_pCRelxationLabelling;
		pCRelxationLabelling->SetDataFromMesh(*pMesh);				
	}
	else
		AfxMessageBox("Mesh is too large,the initialization of CRL failed! ");
	pMesh->Normalize(0.5);
	////////////////////////////////////////////////////////
	
	m_nCurrentMeshIndex = (m_nCurrentMeshIndex + 1) % 2;
	m_strTitle[m_nCurrentMeshIndex] = m_strFileName + "*";	
	
	// Borrowing the following functions to initize the COpenGL
	HANDLE hRaw = GlobalAlloc(GHND, 4*sizeof(BYTE));
	((COpenGL3DEx *)(m_pCOpenGL))->Load3DData(hRaw, 2, 2, 0.0, 0.0, 0.0, 0.0);
	GlobalFree ( hRaw );
	
	//switch to the 3D display mode
	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd( );
	pMainFrame->Set3DView();    	
}

void CMy3dViewerDoc::OnModelResolve() 
{
	// TODO: Add your command handler code here	
	
	char strFilter[]="Bitmap files(*.txt)|*.txt|All files(*.*)|*.*|";
	CFileDialog fDlg(TRUE,"txt","*.txt",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);
	
	if(fDlg.DoModal()==IDOK)
	{
		CString fName = fDlg.GetPathName();
		
		FILE *fp = fopen(fName, "r");
		if(fp == NULL)
			return;
		
		int nPtNum = 0;
		fscanf(fp,"%d",&nPtNum);
        double *z = new double[nPtNum];
		double *x = new double[nPtNum];
		for(int i=0; i<nPtNum; i++)
		{
			fscanf(fp,"%lf %lf", &z[i], &x[i]);
		}
		
		int nSlices = 10;
		if( !feof(fp) )
		{
			fscanf(fp, "%d", &nSlices);
		}
		fclose(fp);
		
		/*
		//order the vertex, make sure from bottom to top:
		for(i=0; i<nPtNum; i++)
		{  
			for(int j=i+1; j<nPtNum; j++)
			{		
				double tz,tx;
				if(z[i] > z[j])
				{
					tz = z[i];
					z[i] = z[j];
					z[j] = tz;

					tx = x[i];
					x[i] = x[j];
					x[j] = tx;
				}
			}
		}
		*/
		//////////////////////////////////////////////////////////////////////////
		       

		CMesh *pMesh = RotateMesh1(z, x ,nPtNum, nSlices);
		
		pMesh->m_nType = 0;
		m_strFileName = " Rotate Model ";
		SetNewMesh(pMesh);

		delete z;
		delete x;

		UpdateView();			
		}	
}

void CMy3dViewerDoc::OnModelExtrude() 
{
	// TODO: Add your command handler code here

	char strFilter[]="Txt files(*.txt)|*.txt|All files(*.*)|*.*|";
	CFileDialog fDlg(TRUE,"txt","*.txt",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);
	
	if(fDlg.DoModal()==IDOK)
	{
		CString fName = fDlg.GetPathName();

		FILE *fp = fopen(fName, "r");
		if(fp == NULL)
			return;
		
		int nContour = 1;
		//fscanf(fp, "%d", &nContour);
        int *pContNum = new int[nContour];

		vector<double> xColl;
		vector<double> yColl;        
		for(int i=0; i<nContour; i++)
		{			
			fscanf(fp, "%d", &pContNum[i]);
			for(int ii=0; ii<pContNum[i]; ii++)
			{
				double tx,ty;
				fscanf(fp,"%lf %lf", &tx, &ty);
				xColl.push_back(tx);
				yColl.push_back(ty);
			}			
		}
        double *x = new double[xColl.size()];
		double *y = new double[xColl.size()];
		for(int i=0; i<xColl.size(); i++)
		{
			x[i] = xColl[i];
			y[i] = yColl[i];
		}

        //read the height of model
		double fHeight = 1;
        if( !feof(fp))
		{		
			fscanf(fp,"%lf", &fHeight);
		}
        fclose(fp);

		//CMesh *pMesh = ExtrudeMesh(x, y, nPtNum, fHeight);
		CMesh *pMesh = ExtrudeMesh1(nContour, pContNum, x, y, fHeight);

		pMesh->m_nType = 0;
		m_strFileName = " Extrude Mesh ";
		SetNewMesh(pMesh);

		UpdateView();

		delete x;
		delete y;
		delete pContNum;
	}
	
}

void CMy3dViewerDoc::OnMeshAverage() 
{
	// TODO: Add your command handler code here

    CWaitCursor wt;

	CSubdivideMesh subMesh;
	int nCurrentIndex = (m_nCurrentMeshIndex + 1) % 2;   
	
	if(m_pMesh[nCurrentIndex]->GetVertexNum())
	{
		//set the edge length for subdividing
		double dfMin = m_pMesh[nCurrentIndex]->GetLenOfShortestEdge();
		double dfMax = m_pMesh[nCurrentIndex]->GetLenOfLongestEdge();

		CDlgSubdivideSetting dlgSetting;
		dlgSetting.m_strLongestEdge.Format("%lf", dfMax);
		dlgSetting.m_strShortestEdge.Format("%lf", dfMin);
        dlgSetting.m_strInputLen = dlgSetting.m_strShortestEdge;

		if(dlgSetting.DoModal() == IDOK)
		{	
			double dfLen = atof( dlgSetting.m_strInputLen );
            
			if(dfLen < 0)
			{
				AfxMessageBox("Input must be positive!");
				return;
			}
			
			subMesh.Init( *(m_pMesh[nCurrentIndex] ) );
			CMesh *pSubdivisionMesh = subMesh.OutputSubMesh(dfLen);
			if(pSubdivisionMesh != NULL)
			{
				pSubdivisionMesh->m_nType = 0;	
				SetNewMesh(pSubdivisionMesh);		
				UpdateView();
			}	
		}
	}
	else
	{
		AfxMessageBox("No Mesh Opened!");
	}
}


void CMy3dViewerDoc::OnFloodFill() 
{
	// TODO: Add your command handler code here
		    HANDLE	hDib0	= m_CBmpViewerData->GetDIBHandle();
		if(hDib0 == NULL )
			return;
		CDIBFlood l_CDIB;

 		HANDLE	hDib1	= m_CDIB.CopyDib(hDib0);
		int h = l_CDIB.DIBHeight( hDib1 );
		int w = l_CDIB.DIBWidth(hDib1);

//		l_CDIB.BoundryFill4 ( hDib1, w/2, h/2, 255, 0, 0);
		l_CDIB.BoundryFill4_Stack(hDib1, w/2, h/2, 255, 0, 0);
		m_CBmpViewerData->SetDIBHandle(NULL);// call this function with NULL to delete hDib0
		hDib0 = NULL;
		
		m_CBmpViewerData->SetDIBHandle(hDib1);

		GenerateMeshFromImg();

		this->UpdateView();		
}
