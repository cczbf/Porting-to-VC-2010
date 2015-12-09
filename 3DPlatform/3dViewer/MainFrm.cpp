// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "3dViewer.h"
#include "MainFrm.h"
#include "CGlobals.h"
#include "CToolZoomIn.h"
#include "CToolZoomOut.h"
#include "CToolBase.h"
#include "CTool3D.h"
#include "3dViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_PAN, OnViewPan)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PAN, OnUpdateViewPan)
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, OnUpdateViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT, OnUpdateViewZoomOut)
	ON_COMMAND(ID_EDIT_EQUALIZE, OnEditEqualize)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EQUALIZE, OnUpdateEditEqualize)
	ON_COMMAND(ID_VIEW_3D_BROWSER, OnView3dBrowser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pCToolBase = NULL;	
	g_CGlobals.set_m_pCMainFrame(this);	
	//m_bIsShowLightCtrlPanel = FALSE;
}

CMainFrame::~CMainFrame()
{
	if ( m_pCToolBase != NULL )
	{
		this->DeleteOldTool();
		m_pCToolBase	= NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

		
	if (!m_colorCtrl.Create_(this, &m_dlgcolor, CString("Lighting & Disp Model"), IDD_DIALOG_COLOR ) ) 
	{
		TRACE0("Failed to create dialogbar\n");
		return FALSE;      // fail to create
	}
	
	m_colorCtrl.SetBarStyle(CBRS_ALIGN_RIGHT);
	
	m_colorCtrl.EnableDocking(CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_colorCtrl);
	ShowControlBar(&m_colorCtrl, 0, 0);
	
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	
//Add by ZBF {{
	CString CStr("Test Panel");
//	CString CStr("");
	if (!m_CDlgBarPanel.Create_(this,
								&m_dlgCTestPanel,
								CStr,
								IDD_TEST_PANEL))
	{						//	IDD_SCENE_AND_SPRITE
		TRACE("Failed to create Secne List\n");
		return -1;
	}
	else
	{
		CControlBar* pCDlgBar = (CControlBar*)&m_CDlgBarPanel;
		pCDlgBar->SetBarStyle(pCDlgBar->GetBarStyle()
							  |CBRS_TOOLTIPS
							  |CBRS_FLYBY
							  |CBRS_SIZE_DYNAMIC);
	}
	m_CDlgBarPanel.EnableDocking(CBRS_ALIGN_ANY);
	this->DockControlBar(&m_CDlgBarPanel);
//Add by ZBF }}



	

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



void CMainFrame::OnViewPan() 
{
	// TODO: Add your command handler code here
	this->DeleteOldTool();
	ASSERT(m_pCToolBase==NULL);
	m_pCToolBase = new CToolPan;
	CBmpViewerData *pData = GetBmpViewerData();
	m_pCToolBase->set_m_pCBmpViewerData(pData);	

	CMy3dViewerDoc* pDoc = (CMy3dViewerDoc*)GetActiveDocument();
	pDoc->UpdateView();
    pDoc->SetPathName(" 2D Image... ");


}

void CMainFrame::OnUpdateViewPan(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
}

void CMainFrame::OnViewZoomIn() 
{
	// TODO: Add your command handler code here
	this->DeleteOldTool();
	ASSERT(m_pCToolBase==NULL);
	m_pCToolBase = (CToolBase*)new CToolZoomIn;
	CBmpViewerData *pData = GetBmpViewerData();
	m_pCToolBase->set_m_pCBmpViewerData(pData);
		
}

void CMainFrame::OnUpdateViewZoomIn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnViewZoomOut() 
{
	// TODO: Add your command handler code here
	this->DeleteOldTool();
	ASSERT(m_pCToolBase==NULL);
	m_pCToolBase = (CToolBase*)new CToolZoomOut;
	CBmpViewerData *pData = GetBmpViewerData();
	m_pCToolBase->set_m_pCBmpViewerData(pData);	
}

void CMainFrame::OnUpdateViewZoomOut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
}


CBmpViewerData * CMainFrame::GetBmpViewerData()
{
	CMy3dViewerDoc* pDoc = (CMy3dViewerDoc*)GetActiveDocument();
	ASSERT_VALID(pDoc);
	return ((CBmpViewerData*)(pDoc->m_CBmpViewerData));

}
int CMainFrame::DeleteOldTool()
{
	if ( this->m_pCToolBase == NULL )
		return -1;
	int iToolID	= this->m_pCToolBase->get_m_nToolID();
	switch ( iToolID )
	{
	case 0:
		delete ((CToolPan*)m_pCToolBase);
		break;
	case 1:
		delete ((CToolZoomIn*)m_pCToolBase);
		break;
	case 2:
		delete ((CToolZoomOut*)m_pCToolBase);
		break;
	case 3:
		delete ((CTool3D*)m_pCToolBase);
		break;
	}
	m_pCToolBase	= NULL;
return iToolID;
}



void CMainFrame::OnEditEqualize() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnUpdateEditEqualize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnView3dBrowser() 
{
	// TODO: Add your command handler code here
	this->DeleteOldTool();
	ASSERT(m_pCToolBase==NULL);
	
	CTool3D* pCTool3D = NULL; 
    pCTool3D = new CTool3D;
	m_pCToolBase = (CToolBase*)pCTool3D;

//	CBmpViewerData *pData = GetBmpViewerData();
//	m_pCToolBase->set_m_pCBmpViewerData(pData);
	
	CMy3dViewerDoc* pDoc = (CMy3dViewerDoc*)GetActiveDocument();
	pCTool3D->set_m_pCOpenGL(pDoc->m_pCOpenGL);

	pDoc->UpdateView();
    pDoc->SetPathName(" 3D Mesh... ");
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	ShowWindow(SW_SHOWMAXIMIZED);
}

void CMainFrame::ShowLightCtrlPanel(bool bIsShow)
{
	//m_bIsShowLightCtrlPanel = !m_bIsShowLightCtrlPanel;

	if (bIsShow)
		ShowControlBar(&m_colorCtrl, 1, 0);
	else
		ShowControlBar(&m_colorCtrl, 0, 0);
}

void CMainFrame::Set3DView()
{
	// TODO: Add your command handler code here
	this->DeleteOldTool();
	ASSERT(m_pCToolBase==NULL);
	
	CTool3D* pCTool3D = NULL; 
    pCTool3D = new CTool3D;
	m_pCToolBase = (CToolBase*)pCTool3D;

//	CBmpViewerData *pData = GetBmpViewerData();
//	m_pCToolBase->set_m_pCBmpViewerData(pData);
	
	CMy3dViewerDoc* pDoc = (CMy3dViewerDoc*)GetActiveDocument();
	pCTool3D->set_m_pCOpenGL(pDoc->m_pCOpenGL);
}

void CMainFrame::Set2DView()
{
	OnViewPan();
}
