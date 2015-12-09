// DialogControl.h : main header file for the DIALOGCONTROL application
//

#if !defined(AFX_DIALOGCONTROL_H__382D8F45_FEE9_11D1_98D4_444553540000__INCLUDED_)
#define AFX_DIALOGCONTROL_H__382D8F45_FEE9_11D1_98D4_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDialogControlApp:
// See DialogControl.cpp for the implementation of this class
//

class CDialogControlApp : public CWinApp
{
public:
	CDialogControlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogControlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDialogControlApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCONTROL_H__382D8F45_FEE9_11D1_98D4_444553540000__INCLUDED_)
