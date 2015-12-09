// CDlgTab02.cpp : implementation file
//

#include "stdafx.h"
#include "3dViewer.h"
#include "CDlgTab02.h"

#include "3dViewerDoc.h"
#include "CGlobals.h"

#include "MeshForGeoImg.h"
#include "CMesh.h"
#include "2DMesh.h"
#include "CRelxationLabelling.h"

#include "COpenGL3DEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTab02 dialog


CDlgTab02::CDlgTab02(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab02::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTab02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	bIsDetectBorder = FALSE;
	bIsFindInitTagent = FALSE;
	bIsTagentOpti = FALSE;
	bIsBorderParam = FALSE;
	bIsFloater = FALSE;
	bIsGenerateGeoimage = FALSE;
	bIReconstruction = FALSE;
		
	m_pInitCut = NULL;	
	m_pCutOpti = NULL;
	m_pEdgeParam = NULL;	
}

CDlgTab02::~CDlgTab02()
{

	if(m_pInitCut != NULL)
		delete m_pInitCut;

	if(m_pCutOpti != NULL)
		delete m_pCutOpti;

	if(m_pEdgeParam != NULL)
		delete m_pEdgeParam;
}

void CDlgTab02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTab02)
 	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTab02, CDialog)
	//{{AFX_MSG_MAP(CDlgTab02)
	ON_WM_CANCELMODE()
	ON_WM_SETCURSOR()
	ON_WM_CAPTURECHANGED()
	ON_WM_PAINT()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_CHARTOITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTab02 message handlers

void CDlgTab02::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CDlgTab02::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}


 
 
 
BOOL CDlgTab02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTab02::OnCancelMode() 
{
	CDialog::OnCancelMode();
	// TODO: Add your message handler code here
}

BOOL CDlgTab02::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CDlgTab02::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CDialog::OnCaptureChanged(pWnd);
}

void CDlgTab02::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
 	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgTab02::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

int CDlgTab02::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	g_CGlobals.m_pDlgTab02 = this;

	return 0;
}

int CDlgTab02::OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnCharToItem(nChar, pListBox, nIndex);
}

//DEL void CDlgTab02::SetMesh()
//DEL {
//DEL 	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
//DEL 	pDoc->m_pMeshForGeoImg->SetMesh();
//DEL 	
//DEL }
 
//DEL void CDlgTab02::SetRL()
//DEL {
//DEL 	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
//DEL 
//DEL 	if( pDoc->m_pMeshForGeoImg != NULL )
//DEL 	{
//DEL 		pDoc->SetEmptyRL();
//DEL 		CRelxationLabelling *pCRelxationLabelling = pDoc->m_pCRelxationLabelling;
//DEL 		
//DEL 		//set RL using the result
//DEL 		pCRelxationLabelling->SetDataFromGeoimg( *pDoc->m_pMeshForGeoImg );
//DEL 
//DEL         //enable save button
//DEL 		g_CGlobals.m_pDlgTab00->m_btnSave2DMesh.EnableWindow(TRUE);
//DEL 
//DEL 		//disable sphere check box
//DEL 		g_CGlobals.m_pDlgTab00->m_bSpherical = FALSE;
//DEL 		g_CGlobals.m_pDlgTab00->UpdateData(0);
//DEL 
//DEL 	}
//DEL 
//DEL }
