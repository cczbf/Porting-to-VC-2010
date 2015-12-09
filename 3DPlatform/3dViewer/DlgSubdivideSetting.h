#if !defined(AFX_DLGSUBDIVIDESETTING_H__012E4DBF_A310_4EB4_995C_474FBE547257__INCLUDED_)
#define AFX_DLGSUBDIVIDESETTING_H__012E4DBF_A310_4EB4_995C_474FBE547257__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubdivideSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSubdivideSetting dialog

class CDlgSubdivideSetting : public CDialog
{
// Construction
public:
	CDlgSubdivideSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSubdivideSetting)
	enum { IDD = IDD_DIALOG_SUBDIVIDE_SETTING };
	CString	m_strLongestEdge;
	CString	m_strShortestEdge;
	CString	m_strInputLen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubdivideSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSubdivideSetting)
	afx_msg void OnChangeEditInputLen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBDIVIDESETTING_H__012E4DBF_A310_4EB4_995C_474FBE547257__INCLUDED_)
