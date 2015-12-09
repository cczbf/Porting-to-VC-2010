// DlgLightDispMode.cpp : implementation file
//

#include "stdafx.h"
#include "DlgLightDispMode.h"
#include "3dViewerDoc.h"

#include "CGlobals.h"
#include "COpenGL3DEx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLightDispMode dialog


CDlgLightDispMode::CDlgLightDispMode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLightDispMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLightDispMode)
	//}}AFX_DATA_INIT
	m_bIsLighting = FALSE;

}


void CDlgLightDispMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLightDispMode)
	DDX_Control(pDX, IDC_CHECK_DISABLE_LIGHT, m_BtnLight);
	DDX_Control(pDX, IDC_STATIC_DIFFUSE, m_ControlColorLightDiffuse);
	DDX_Control(pDX, IDC_STATIC_SPECULAR, m_ControlColorLightSpecular);
	DDX_Control(pDX, IDC_STATIC_AMBIENT, m_ControlColorLightAmbient);
	DDX_Control(pDX, IDC_STATIC_GROUND, m_ControlBackColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLightDispMode, CDialog)
	//{{AFX_MSG_MAP(CDlgLightDispMode)
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_PAINT()
	ON_WM_CAPTURECHANGED()
	ON_BN_CLICKED(IDC_RADIO_VERTEX, OnRadioVertex)
	ON_BN_CLICKED(IDC_RADIO_WIREFRAME, OnRadioWireframe)
	ON_BN_CLICKED(IDC_RADIO_FACE, OnRadioFace)
	ON_BN_CLICKED(IDC_CHECK_DISABLE_LIGHT, OnCheckDisableLight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLightDispMode message handlers

void CDlgLightDispMode::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();

	CRect rect;

	// back color
	m_ControlBackColor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(BackColor);

		if(dlg.DoModal()==IDOK)
		{
			BackColor = dlg.GetColor();	
			if (pDoc)
				pDoc->m_pCOpenGL->SetBckGroundColor(BackColor);
		}
	}

	// ambient light color
	m_ControlColorLightAmbient.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(AmbientColor);

		if(dlg.DoModal()==IDOK)
		{
			AmbientColor = dlg.GetColor();	
			if (pDoc)
				pDoc->m_pCOpenGL->SetAmbient(AmbientColor);
		}
	}		
	
	// specular light color
	m_ControlColorLightSpecular.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(SpecularColor);
		if(dlg.DoModal()==IDOK)
		{
			SpecularColor = dlg.GetColor();	
			if (pDoc)
				pDoc->m_pCOpenGL->SetSpecular(SpecularColor);
		}
	}		

	// diffuse light color
	m_ControlColorLightDiffuse.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(DiffuseColor);
		if(dlg.DoModal()==IDOK)
		{
			DiffuseColor = dlg.GetColor();	
			if (pDoc)
				pDoc->m_pCOpenGL->SetDiffuse(DiffuseColor);
		}
	}		

	pDoc->UpdateView();

	Invalidate();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgLightDispMode::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
}

void CDlgLightDispMode::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CRect rect;
	// Color back
	m_ControlBackColor.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CBrush BrushBack(BackColor);
	dc.FillRect(&rect,&BrushBack);
	
	// Color light ambient
	m_ControlColorLightAmbient.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightAmbient(AmbientColor);
	dc.FillRect(&rect,&BrushLightAmbient);

	// Color light diffuse
	m_ControlColorLightDiffuse.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightDiffuse(DiffuseColor);
	dc.FillRect(&rect,&BrushLightDiffuse);

	// Color light specular
	m_ControlColorLightSpecular.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightSpecular(SpecularColor);
	dc.FillRect(&rect,&BrushLightSpecular);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgLightDispMode::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CDialog::OnCaptureChanged(pWnd);
}

void CDlgLightDispMode::OnRadioVertex() 
{
	// TODO: Add your control notification handler code here
	
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc)
		((COpenGL3DEx*)(pDoc->m_pCOpenGL))->SetVertexMode();
		
	pDoc->UpdateView();

}

void CDlgLightDispMode::OnRadioWireframe() 
{
	// TODO: Add your control notification handler code here
	
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc)
		((COpenGL3DEx*)(pDoc->m_pCOpenGL))->SetWireFrameMode();
	pDoc->UpdateView();		
}

void CDlgLightDispMode::OnRadioFace() 
{
	// TODO: Add your control notification handler code here
	
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if (pDoc)
		((COpenGL3DEx*)(pDoc->m_pCOpenGL))->SetFaceMode();
	pDoc->UpdateView();
}

BOOL CDlgLightDispMode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	BackColor=RGB(0,0,0);
	AmbientColor=RGB(255,0,0);
	DiffuseColor=RGB(255,255,255);	
	SpecularColor=RGB(0,0,0);

	m_BtnLight.SetCheck(m_bIsLighting);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLightDispMode::OnCheckDisableLight() 
{
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	
	// TODO: Add your control notification handler code here
    m_bIsLighting = !m_bIsLighting;
	m_BtnLight.SetCheck(m_bIsLighting);
	
	if (pDoc)
		((COpenGL3DEx*)(pDoc->m_pCOpenGL))->LightingOn(m_bIsLighting);

	pDoc->UpdateView();
}
