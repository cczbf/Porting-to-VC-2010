#if !defined(AFX_DLG2DMESHINFODISP_H__341AD27C_C10D_45BD_BB52_BF1F100EB709__INCLUDED_)
#define AFX_DLG2DMESHINFODISP_H__341AD27C_C10D_45BD_BB52_BF1F100EB709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg2DMeshInfoDisp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg2DMeshInfoDisp dialog

class CDlg2DMeshInfoDisp : public CDialog
{
// Construction
public:
	CDlg2DMeshInfoDisp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg2DMeshInfoDisp)
	enum { IDD = IDD_DIALOG_2D_MESH_INFO };
	CString	m_str2DNum;
	CString	m_str3DNum;
	CString	m_strEdgeNum;
	CString	m_strSampleGap;
	CString	m_strFaceNumber;
	CString	m_strBorderNodeNum;
	CString	m_strGenus;
	CString	m_str2DEdgeNum;
	CString	m_str2DNodeNum;
	CString	m_str2DFaceNum;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg2DMeshInfoDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg2DMeshInfoDisp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG2DMESHINFODISP_H__341AD27C_C10D_45BD_BB52_BF1F100EB709__INCLUDED_)
