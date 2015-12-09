#if !defined(AFX_SCROLLDLG_H__B6747F81_5165_403F_A3AC_81EE41432A26__INCLUDED_)
#define AFX_SCROLLDLG_H__B6747F81_5165_403F_A3AC_81EE41432A26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScrollDlg dialog

class CScrollDlg : public CDialog
{
// Construction
public:
	void ResetScrollbars();
	void SetupScrollbars();
	CScrollDlg(CWnd* pParent = NULL);   // standard constructor
	CScrollDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);


// Dialog Data
	//{{AFX_DATA(CScrollDlg)
	//enum { IDD = IDD_GRAPHICDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	//Define the Variables that will be required for setting 
	//up a Vertical Scroll bar
	int  m_nVertInc,m_nVscrollMax,m_nVscrollPos;
	//Initialized the variable for Client Rectangle
    CRect m_ClientRect;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScrollDlg)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLDLG_H__B6747F81_5165_403F_A3AC_81EE41432A26__INCLUDED_)
