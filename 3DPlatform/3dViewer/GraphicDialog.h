// GraphicDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphicDialog dialog

class CGraphicDialog : public CDialog
{
// Construction
public:
	int m_nShowAry1[256];
	int m_nShowAry2[256];
	CGraphicDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGraphicDialog)
	enum { IDD = IDD_GRAPHICDLG };
	CStatic	m_CStaticD;
	CStatic	m_CStaticM;
	CStatic	m_CStaticY;
	CStatic	m_CStaticX;
	BOOL	m_bOverLay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DisplayPoint1(CPoint point);
	void DisplayPoint2(CPoint point);
	// Generated message map functions
	//{{AFX_MSG(CGraphicDialog)
	afx_msg void OnMap1();
	afx_msg void OnMap2();
	virtual void OnOK();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnOverlayed();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
//	BOOL m_bPaint1;
//	BOOL m_bPaint2;
	BOOL m_bMap2;
	BOOL m_bMap1;
};
