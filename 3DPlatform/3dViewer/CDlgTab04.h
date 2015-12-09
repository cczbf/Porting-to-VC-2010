#if !defined(AFX_CDLGTAB04_H__7E14FC05_4CF7_44CF_8B76_7B0CF4584A2C__INCLUDED_)
#define AFX_CDLGTAB04_H__7E14FC05_4CF7_44CF_8B76_7B0CF4584A2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTab04.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTab04 dialog

//##ModelId=40D5155A001D
class CDlgTab04 : public CDialog
{
// Construction
public:
	//##ModelId=40D5155A002A
	CDlgTab04(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTab04)
	enum { IDD = IDD_TAB_DIALOG04 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTab04)
	protected:
	//##ModelId=40D5155A0028
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTab04)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTAB04_H__7E14FC05_4CF7_44CF_8B76_7B0CF4584A2C__INCLUDED_)
