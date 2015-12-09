#if !defined(AFX_DLGTAB01_H__56068F1D_ACEC_4DC3_9A48_DD5C8F447C39__INCLUDED_)
#define AFX_DLGTAB01_H__56068F1D_ACEC_4DC3_9A48_DD5C8F447C39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTab01.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTab01 dialo
#include "resource.h"
#include "COpenGL.h"

//##ModelId=40D000EB021D
class CDlgTab01 : public CDialog
{
// Construction
public:
	//##ModelId=40D000EB0229
	CDlgTab01(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTab01)
	enum { IDD = IDD_TAB_DIALOG01 };
	CButton	m_ctrlMemoryBoosterCheckBox;
	CSliderCtrl	m_AM;
	CButton	m_ctrlAccelerateCheckBox;
	CStatic	m_ControlColorLightAmbient;
	CStatic	m_ControlColorLightSpecular;
	CStatic	m_ControlColorLightDiffuse;
	CStatic	m_ControlBackColor;
	CButton	m_ctrlLightCheckBox;
	CButton	m_nRa2D;
	CButton	m_nRaGrid;
	CButton	m_nRaSurface;
	CComboBox	m_CRType;
	CButton	m_RCENTER;
	CButton	m_RRANDOM;
	CButton	m_RMOVE;
	CButton	m_RYZ;
	CButton	m_RXY;
	CButton	m_RXYZ;
	CButton	m_RXZ;
	CButton	m_RNO;
	CSliderCtrl	m_SSpeed;
	CSliderCtrl	m_SZ;
	CSliderCtrl	m_SY;
	CSliderCtrl	m_SX;
	//}}AFX_DATA
	
	//added by xie dong hai,2004,10,20
	static BOOL	m_bLighting; 
	static BOOL m_bAccelerating;
	static BOOL m_bMemoryBoostering;
	static int  m_nAccMul;
	static int  m_nXScale;   //record the scale scroll bar postion of X
	static int  m_nYScale;   //record the scale scroll bar postion of Y
	static int  m_nZScale;   //record the scale scroll bar postion of Z
	static int  m_nSpeedValue; //record the speed scroll bar postion 
	static int  m_nRotationType; //record the selection of Combox of Rotation type 
	static CLinkType m_LinkType;
	static CMapType  m_ModeType;
	//////////////////////////////////////////////////////////////////////////
	
	static COLORREF BackColor;
	static COLORREF AmbientColor;
	static COLORREF DiffuseColor;
	static COLORREF SpecularColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTab01)
	protected:
	//##ModelId=40D000EB0227
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTab01)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRno();
	afx_msg void OnRxy();
	afx_msg void OnRxz();
	afx_msg void OnRyz();
	afx_msg void OnRxyz();
	afx_msg void OnRmove();
	afx_msg void OnRrandom();
	afx_msg void OnRcenter();
	afx_msg void OnSelchangeCrtype();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRasurface();
	afx_msg void OnRagrid();
	afx_msg void OnRa2d();
	afx_msg void OnCheckLighting();
	afx_msg void OnPaint();
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnButtonReset();
	afx_msg void OnCheckAccelerating();
	afx_msg void OnOutofmemorySz(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawAccmul(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void MemoryBooster();
	afx_msg void OnMemorybooster();
	afx_msg void OnCheckMemoryBooster();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTAB01_H__56068F1D_ACEC_4DC3_9A48_DD5C8F447C39__INCLUDED_)
