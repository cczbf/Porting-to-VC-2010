#if !defined(AFX_DLGOPTISETTINGS_H__A152A680_7F3C_4B5C_8077_59AAA50E393D__INCLUDED_)
#define AFX_DLGOPTISETTINGS_H__A152A680_7F3C_4B5C_8077_59AAA50E393D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptiSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptiSettings dialog



class CDlgOptiSettings : public CDialog
{
// Construction
public:
	void InsertMsg(char *absoluteErr, char *incrementalErr, int item);
	CDlgOptiSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOptiSettings)
	enum { IDD = IDD_DIALOG_COMPUTE_INITIAL_VALUE };
	CButton	m_ctGlobalLocal;
	CButton	m_ctTargetOurs;
	CButton	m_ctTargetFound;
	CButton	m_ctOptiNo;
	CButton	m_ctOptiMatlab;
	CButton	m_ctOptiLocal;
	CButton	m_ctOptiGlobal;
	CButton	m_ctLwLengthPreserve;
	CButton	m_ctLwEqui;
	CButton	m_ctNo;
	CButton	m_ctYes;
	float	m_fErrorBound;
	float	m_fIncrementBound;
	int		m_nIterationLimit;
	BOOL	m_ckNo;
	BOOL	m_ckOptiGlobal;
	BOOL	m_ckOptiLocal;
	BOOL	m_ckOptiMatlab;
	BOOL	m_ckOptiNo;
	BOOL	m_ckTargetFound;
	BOOL	m_ckTargetOurs;
	BOOL	m_ckYes;
	BOOL	m_ckLwModeEqui;
	BOOL	m_ckLwModePreserveLength;
	double	m_fWeight;
	CString	m_strFilePath;
	BOOL	m_bIsNode2Label;
	BOOL	m_ckGlobalLocal;
	//}}AFX_DATA

	//
	//int m_nIterationLimit; //default 100
	int m_nLwMode;         //0 --- Equi-angle; 1------Length Preserve<default>
	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptiSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptiSettings)
	afx_msg void OnCheckLwModeEquiAngle();
	afx_msg void OnCheckLwModeLengthPreserve();
	afx_msg void OnCheckYes();
	afx_msg void OnCheckNo();
	afx_msg void OnCheckOptiNoOpti();
	afx_msg void OnCheckOptiGlobal();
	afx_msg void OnCheckOptiLocal();
	afx_msg void OnCheckOptiMatlab();
	afx_msg void OnCheckTargetFoundamental();
	afx_msg void OnCheckTargetOurs();
	afx_msg void OnButtonInputFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg void OnCheckNode2label();
	afx_msg void OnCheckOptiGlobalLocal();
	//}}AFX_MSG
	LONG OnMyMessage(WPARAM w,LPARAM l); 
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTISETTINGS_H__A152A680_7F3C_4B5C_8077_59AAA50E393D__INCLUDED_)
