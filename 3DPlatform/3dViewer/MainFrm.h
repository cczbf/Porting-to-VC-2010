// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__DD021B34_E92C_49AA_A336_9712DDE67828__INCLUDED_)
#define AFX_MAINFRM_H__DD021B34_E92C_49AA_A336_9712DDE67828__INCLUDED_

#include "ctoolbase.h"
#include "ctoolpan.h"
#include "CoolDialogBar.h"	// Added by ClassView
#include "cDlgTestPanel.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cooldialogbar.h"
#include "DlgLightDispMode.h"


//##ModelId=40C7B40F03B2
class CMainFrame : public CFrameWnd
{
	
	private:
	int DeleteOldTool();
	CBmpViewerData * GetBmpViewerData();
protected: // create from serialization only
	//##ModelId=40C7B40F03CB
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//##ModelId=40C7B40F03C9
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=40CFEE3E0163
	CDlgTestPanel m_dlgCTestPanel;
	//##ModelId=40C7B40F03C0
	CCoolDialogBar m_CDlgBarPanel;

	//##ModelId=40C7B40F03C8
	virtual ~CMainFrame();
#ifdef _DEBUG
	//##ModelId=40C7B40F03C7
	virtual void AssertValid() const;
	//##ModelId=40C7B40F03C5
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void Set2DView();
	void Set3DView();
	void ShowLightCtrlPanel(bool bIsShow);
	CToolBase  *m_pCToolBase;
protected:  // control bar embedded members
	//##ModelId=40C7B40F03BB
	CStatusBar  m_wndStatusBar;
	//##ModelId=40C7B40F03B6
	CToolBar    m_wndToolBar;

	//for lighting control bar
	CCoolDialogBar     m_colorCtrl;
	CDlgLightDispMode  m_dlgcolor;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewPan();
	afx_msg void OnUpdateViewPan(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomIn();
	afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomOut();
	afx_msg void OnUpdateViewZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnEditEqualize();
	afx_msg void OnUpdateEditEqualize(CCmdUI* pCmdUI);
	afx_msg void OnView3dBrowser();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__DD021B34_E92C_49AA_A336_9712DDE67828__INCLUDED_)
