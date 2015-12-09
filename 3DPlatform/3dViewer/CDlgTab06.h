#if !defined(AFX_CDLGTAB06_H__1745C936_D31A_467D_B9C2_711729CD541B__INCLUDED_)
#define AFX_CDLGTAB06_H__1745C936_D31A_467D_B9C2_711729CD541B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTab06.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTab06 dialog

//##ModelId=40D5155903A1
class CDlgTab06 : public CDialog
{
// Construction
public:
	//##ModelId=40D5155903B6
	CDlgTab06(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTab06)
	enum { IDD = IDD_TAB_DIALOG06 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTab06)
	protected:
	//##ModelId=40D5155903AC
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTab06)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTAB06_H__1745C936_D31A_467D_B9C2_711729CD541B__INCLUDED_)
