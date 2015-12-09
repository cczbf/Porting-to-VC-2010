#if !defined(AFX_CDLGTESTPANEL_H__6E807AA1_2369_46C9_9213_6DE5DB47EE62__INCLUDED_)
#define AFX_CDLGTESTPANEL_H__6E807AA1_2369_46C9_9213_6DE5DB47EE62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cDlgTestPanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTestPanel dialog
#include "CTestPanelTabs.h"

class CDlgTestPanel : public CDialog
{
// Construction
public:
//		CTestTab01 m_pCTestTab01;
	CTabHub m_CTabHub;
	CDlgTestPanel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTestPanel)
	enum { IDD = IDD_TEST_PANEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTestPanel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTestPanel)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTABTestPanels(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTABTestPanels(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTESTPANEL_H__6E807AA1_2369_46C9_9213_6DE5DB47EE62__INCLUDED_)
