#if !defined(AFX_COOLDIALOGBAR_H__6EB5FA61_FFAD_11D1_98D4_444553540000__INCLUDED_)
#define AFX_COOLDIALOGBAR_H__6EB5FA61_FFAD_11D1_98D4_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CoolDialogBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolDialogBar window

//##ModelId=40C7B40F034B
class CCoolDialogBar : public CControlBar
{
	//##ModelId=40C7B40F0376
	const COLORREF	m_clrBtnHilight;
	//##ModelId=40C7B40F0375
	const COLORREF	m_clrBtnShadow;

	// Construction / destruction
public:
	//##ModelId=40C7B40F03B1
    CCoolDialogBar();

// Attributes
public:
	//##ModelId=40C7B40F03B0
    BOOL IsHorz() const;

// Operations
public:

// Overridables
	//##ModelId=40C7B40F03AD
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCoolDialogBar)
    public:
	//##ModelId=40C7B40F03A7
    virtual BOOL  Create_(CWnd* pParentWnd, CDialog *pDialog, CString &pTitle, UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_LEFT);
	//##ModelId=40C7B40F03A4
    virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );
	//##ModelId=40C7B40F03A1
    virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
    //}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=40C7B40F03A0
    virtual ~CCoolDialogBar();
	//##ModelId=40C7B40F039F
    void StartTracking();
	//##ModelId=40C7B40F039D
    void StopTracking(BOOL bAccept);
	//##ModelId=40C7B40F039B
    void OnInvertTracker(const CRect& rect);
    
    // implementation helpers
	//##ModelId=40C7B40F0399
    CPoint& ClientToWnd(CPoint& point);

protected:
	//##ModelId=40C7B40F0397
	void		DrawGripper(CDC &dc);

	//##ModelId=40C7B40F0374
    CSize       m_sizeMin;
	//##ModelId=40C7B40F0373
    CSize       m_sizeHorz;
	//##ModelId=40C7B40F0372
    CSize       m_sizeVert;
	//##ModelId=40C7B40F0371
    CSize       m_sizeFloat;
	//##ModelId=40C7B40F0370
    CRect       m_rectBorder;
	//##ModelId=40C7B40F036F
    CRect       m_rectTracker;
	//##ModelId=40C7B40F036E
    UINT        m_nDockBarID;
	//##ModelId=40C7B40F036D
    CPoint      m_ptOld;
	//##ModelId=40C7B40F036C
    BOOL        m_bTracking;
	//##ModelId=40C7B40F036B
    BOOL        m_bInRecalcNC;
	//##ModelId=40C7B40F036A
    int         m_cxEdge;
	//##ModelId=40C7B40F0369
	CRect		m_rectUndock;
	//##ModelId=40C7B40F0368
	CRect		m_rectClose;
	//##ModelId=40C7B40F0367
	CRect		m_rectGripper;
	//##ModelId=40C7B40F0366
	int			m_cxGripper;
	//##ModelId=40C7B40F0365
	int			m_cxBorder;
	//##ModelId=40C7B40F035B
	CDialog*	m_cDialog;
 
// Generated message map functions
protected:
   //{{AFX_MSG(CCoolDialogBar)
	//##ModelId=40C7B40F0394
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//##ModelId=40C7B40F0391
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//##ModelId=40C7B40F038D
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//##ModelId=40C7B40F038B
    afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//##ModelId=40C7B40F038A
    afx_msg void OnNcPaint();
	//##ModelId=40C7B40F0387
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	//##ModelId=40C7B40F0385
    afx_msg UINT OnNcHitTest(CPoint point);
	//##ModelId=40C7B40F0382
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//##ModelId=40C7B40F037F
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//##ModelId=40C7B40F037D
    afx_msg void OnCaptureChanged(CWnd *pWnd);
	//##ModelId=40C7B40F037A
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//##ModelId=40C7B40F0377
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLDIALOGBAR_H__6EB5FA61_FFAD_11D1_98D4_444553540000__INCLUDED_)
