#if !defined(AFX_DLGLIGHTDISPMODE_H__AA96AAAC_D4CB_497E_B735_1589D815D205__INCLUDED_)
#define AFX_DLGLIGHTDISPMODE_H__AA96AAAC_D4CB_497E_B735_1589D815D205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLightDispMode.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgLightDispMode dialog

class CDlgLightDispMode : public CDialog
{
// Construction
public:
	CDlgLightDispMode(CWnd* pParent = NULL);   // standard constructor

	COLORREF BackColor;
	COLORREF AmbientColor;
	COLORREF DiffuseColor;
	COLORREF SpecularColor;

	bool     m_bIsLighting;

// Dialog Data
	//{{AFX_DATA(CDlgLightDispMode)
	enum { IDD = IDD_DIALOG_COLOR };
	CButton	m_BtnLight;
	CStatic	m_ControlColorLightDiffuse;
	CStatic	m_ControlColorLightSpecular;
	CStatic	m_ControlColorLightAmbient;
	CStatic	m_ControlBackColor;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLightDispMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLightDispMode)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnPaint();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnRadioVertex();
	afx_msg void OnRadioWireframe();
	afx_msg void OnRadioFace();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDisableLight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLIGHTDISPMODE_H__AA96AAAC_D4CB_497E_B735_1589D815D205__INCLUDED_)
