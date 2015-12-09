// 3dViewer.h : main header file for the 3DVIEWER application
//

#if !defined(AFX_3DVIEWER_H__98C1AC1B_867B_47DA_BE61_56E2272B62EE__INCLUDED_)
#define AFX_3DVIEWER_H__98C1AC1B_867B_47DA_BE61_56E2272B62EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

//#include"CMeshOpti.h"

/////////////////////////////////////////////////////////////////////////////
// CMy3dViewerApp:
// See 3dViewer.cpp for the implementation of this class
//
//CMeshOpti * g_pCMeshOpti = NULL;


//##ModelId=40C7B410003B
class CMy3dViewerApp : public CWinApp
{
public:
	//##ModelId=40C7B410003F
	CMy3dViewerApp();

	CSingleDocTemplate* pDocTemplate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy3dViewerApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMy3dViewerApp)
	afx_msg void OnAppAbout();
	afx_msg void OnButtonOpenLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DVIEWER_H__98C1AC1B_867B_47DA_BE61_56E2272B62EE__INCLUDED_)
