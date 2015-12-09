#if !defined(AFX_CDLGTAB05_H__D94242AE_5905_4E94_9E0A_9CEA2E333246__INCLUDED_)
#define AFX_CDLGTAB05_H__D94242AE_5905_4E94_9E0A_9CEA2E333246__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTab05.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTab05 dialog

//##ModelId=40D5155903D3
class CDlgTab05 : public CDialog
{
// Construction
public:
	//##ModelId=40D5155903E8
	CDlgTab05(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTab05)
	enum { IDD = IDD_TAB_DIALOG05 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTab05)
	protected:
	//##ModelId=40D5155903DE
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTab05)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTAB05_H__D94242AE_5905_4E94_9E0A_9CEA2E333246__INCLUDED_)
