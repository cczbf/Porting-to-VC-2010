// 3dViewerDoc.h : interface of the CMy3dViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DVIEWERDOC_H__E3B666E2_8868_49AF_B286_6AE07FAEF210__INCLUDED_)
#define AFX_3DVIEWERDOC_H__E3B666E2_8868_49AF_B286_6AE07FAEF210__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "cdib.h"
#include "cdiborg.h"
#include "CHtArray2Dib.h"



class CMesh;
class C2DMesh;
class CMeshForGeoImg;
class CRelxationLabelling;
class CBmpViewerData;
class COpenGL;

struct STATISTIC
{
	CString strAverageOfStretchRatio ;
	CString strCoviarianceOfStretchRatio;		
	CString strGeoRatio ;	
};


//##ModelId=40C7B4100027
class CMy3dViewerDoc : public CDocument
{
protected: // create from serialization only
	//##ModelId=40C7B4100030
	CMy3dViewerDoc();
	DECLARE_DYNCREATE(CMy3dViewerDoc)

// Attributes
public:
	 CBmpViewerData  *m_CBmpViewerData;
     CHtArray2Dib    m_CDIB;
	 CDIBOrg m_CDIBOrg;

     CMesh        *m_pMesh[2];
	 int          m_nCurrentMeshIndex;
	 STATISTIC    m_struStatistic[2];
	 bool         m_bIsNew;

 	 static CRelxationLabelling *m_pCRelxationLabelling;
     CMeshForGeoImg *m_pMeshForGeoImg;
   	 C2DMesh *m_pCurrent2DMesh;
	 	 
	 //bool         m_bIsOpen3DModel;

	 //
	 CString          m_strTitle[2]; //save the two buffer title
	 CString          m_strFileName; //save the current file name
	 //CString          m_strLabelName;

	 //added by xdh,
	 //to control the switch between 2D image and 3d mesh
	 bool m_bIsShow3DDepth;
	 //bool m_bIsCapture;
	 //

// Operations
public:
	  void SetNewMesh(CMesh *pMesh = NULL);
   	  void SetEmptyRL();
      BOOL UpdateView ();
	  void TransferByBuffer(HANDLE hHtArray,
						 DWORD dwWidth, 
						 DWORD dwHeight,
						 DWORD dwScaleX,
						 DWORD dwScaleY,
						 DWORD dwLevels,
						 DWORD dwLimitLevels
						);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy3dViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GenerateMeshFromImg();
	void OutputGeoimageIntoImgBuffer();
	void DoRemoveMesh(CMesh *pMesh);
	void DoAddMesh(CMesh *pMesh);
	COpenGL *m_pCOpenGL;
	
	//##ModelId=40C7B410002C
	virtual ~CMy3dViewerDoc();
#ifdef _DEBUG
	//##ModelId=40C7B410002B
	virtual void AssertValid() const;
	//##ModelId=40C7B4100029
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMy3dViewerDoc)
	afx_msg void OnEditInverse();
	afx_msg void OnEditUndo();
	afx_msg void OnEditEqualize();
	afx_msg void OnEDITFourierTransform();
	afx_msg void OnEDITInverseWaveletTransform();
	afx_msg void OnEDITWaveletTransform();
	afx_msg void OnEditDaub12();
	afx_msg void OnEditDaub20();
	afx_msg void OnEditDaub4();
	afx_msg void OnEditFftPeridogram();
	afx_msg void OnUpdateEditDaub12(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDaub20(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDaub4(CCmdUI* pCmdUI);
	afx_msg void OnFileInfo();
	afx_msg void OnEditGraphic();
	afx_msg void OnEditFilter();
	afx_msg void OnFileSaveAs();
	afx_msg void OnChangeMeshColor();
	afx_msg void OnDebugRlSetmesh();
	afx_msg void OnSetMeshSphere();
	afx_msg void OnSaveObjAsPly();
	afx_msg void OnMeshSubdivision();
	afx_msg void OnConstructCube();
	afx_msg void OnConstructModelCube();
	afx_msg void OnModelHexahedron();
	afx_msg void OnModelResolve();
	afx_msg void OnModelExtrude();
	afx_msg void OnMeshAverage();
	afx_msg void OnFloodFill();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DVIEWERDOC_H__E3B666E2_8868_49AF_B286_6AE07FAEF210__INCLUDED_)
