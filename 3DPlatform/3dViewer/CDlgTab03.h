#if !defined(AFX_CDLGTAB03_H__31CFA943_A97B_426B_9914_B7C92594DA4F__INCLUDED_)
#define AFX_CDLGTAB03_H__31CFA943_A97B_426B_9914_B7C92594DA4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTab03.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTab03 dialog

//##ModelId=40D5155A004F
class CDlgTab03 : public CDialog
{
// Construction
public:
	//##ModelId=40D5155A005C
	CDlgTab03(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTab03)
	enum { IDD = IDD_TAB_DIALOG03 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTab03)
	protected:
	//##ModelId=40D5155A005A
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTab03)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTAB03_H__31CFA943_A97B_426B_9914_B7C92594DA4F__INCLUDED_)
