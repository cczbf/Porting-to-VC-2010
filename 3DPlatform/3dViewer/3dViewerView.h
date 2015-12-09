// 3dViewerView.h : interface of the CMy3dViewerView class
//
/////////////////////////////////////////////////////////////////////////////
#include "CBmpDblBuffering.h"
#if !defined(AFX_3DVIEWERVIEW_H__24FE5A65_7436_4994_8DE3_1F32ED508F55__INCLUDED_)
#define AFX_3DVIEWERVIEW_H__24FE5A65_7436_4994_8DE3_1F32ED508F55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//##ModelId=40C7B40F03DD
class CMy3dViewerView : public CView
{
private:
    CBmpViewerData *GetBmpViewerData();

protected: // create from serialization only
	//##ModelId=40C7B40F03F9
	CMy3dViewerView();
	DECLARE_DYNCREATE(CMy3dViewerView)

// Attributes
public:
    CBmpDblBuffering* m_pCBmpDblBuf;
public:
	//##ModelId=40C7B40F03F8
	CMy3dViewerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy3dViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawStastic();
	void Render();
	//##ModelId=40C7B40F03EB
	virtual ~CMy3dViewerView();
#ifdef _DEBUG
	//##ModelId=40C7B40F03EA
	virtual void AssertValid() const;
	//##ModelId=40C7B40F03E8
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMy3dViewerView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMenuitem32792();
	afx_msg void OnUpdateMenuitem32792(CCmdUI* pCmdUI);
	afx_msg void OnMenuitem32791();
	afx_msg void OnUpdateMenuitem32791(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in 3dViewerView.cpp
inline CMy3dViewerDoc* CMy3dViewerView::GetDocument()
   { return (CMy3dViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DVIEWERVIEW_H__24FE5A65_7436_4994_8DE3_1F32ED508F55__INCLUDED_)
