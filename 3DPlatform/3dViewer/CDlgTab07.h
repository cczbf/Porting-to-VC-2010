#if !defined(AFX_CDLGTAB07_H__7BFA3811_FC8B_48B9_A462_0E672D9DA311__INCLUDED_)
#define AFX_CDLGTAB07_H__7BFA3811_FC8B_48B9_A462_0E672D9DA311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTab07.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTab07 dialog

//##ModelId=40D515590379
class CDlgTab07 : public CDialog
{
// Construction
public:
	//##ModelId=40D515590385
	CDlgTab07(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTab07)
	enum { IDD = IDD_TAB_DIALOG07 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTab07)
	protected:
	//##ModelId=40D515590383
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTab07)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTAB07_H__7BFA3811_FC8B_48B9_A462_0E672D9DA311__INCLUDED_)
