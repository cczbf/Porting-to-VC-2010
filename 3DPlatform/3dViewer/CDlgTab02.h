#if !defined(AFX_CDLGTAB02_H__8DB50534_1751_4241_8694_2CB3838F8697__INCLUDED_)
#define AFX_CDLGTAB02_H__8DB50534_1751_4241_8694_2CB3838F8697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTab02.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTab02 dialog 

class C2DMesh;

//##ModelId=40D2E34D034D
class CDlgTab02 : public CDialog
{
// Construction
public:
	//##ModelId=40D2E34D0358
	CDlgTab02(CWnd* pParent = NULL);   // standard constructor
	~CDlgTab02();

// Dialog Data
	//{{AFX_DATA(CDlgTab02)
	enum { IDD = IDD_TAB_DIALOG02 };
	CButton	m_btnOpen2dMesh;
	CButton	m_btnSave2DMesh;
	CButton	m_btnBat;
	CButton	m_btnTagentOpti;
	CButton	m_btnReconstruction;
	CButton	m_btnGenerateGeoimage;
	CButton	m_btnFloaterTutte;
	CButton	m_btnFloaterSameAngle;
	CButton	m_btnFloaterMeanValue;
	CButton	m_btnFloaterKeepShape;
	CButton	m_btnFindInitTagent;
	CButton	m_btnEdgeParam;
	CButton	m_btnDetectBorder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTab02)
	protected:
	//##ModelId=40D2E34D0356
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	bool bIsDetectBorder;
	bool bIsFindInitTagent;
	bool bIsTagentOpti;
	bool bIsBorderParam;
	bool bIsFloater;
	bool bIsGenerateGeoimage;
	bool bIReconstruction;

	//new mesh:
	C2DMesh *m_pInitCut;
	C2DMesh *m_pCutOpti;
	C2DMesh *m_pEdgeParam;	
	
	// Generated message map functions
	//{{AFX_MSG(CDlgTab02)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTAB02_H__8DB50534_1751_4241_8694_2CB3838F8697__INCLUDED_)
