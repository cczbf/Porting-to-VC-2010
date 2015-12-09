#if !defined(AFX_CDLGTAB00_H__325263A9_CFAC_46A0_AC45_D5EBB1073A67__INCLUDED_)
#define AFX_CDLGTAB00_H__325263A9_CFAC_46A0_AC45_D5EBB1073A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTab00.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTab00 dialog
#include "resource.h"
#include "HistoryEdit.h"
#include "ScrollDlg.h"
#include "IniFile.h"

//#include <fstream>
//using namespace std;
//##ModelId=40D515590340

//added by xdh,2004,12,6
struct OptiParams
{
	int  iterationLimit;
	int  nNode2Label;
	int  nLwMode;
	double dfErrorBound;
	double dfIncrementBound;
	int    nMaptoSphereFirst;
	int    nOptiMode;
	int    nOptiTarget;
	float  fWeight;
};
//////////////////////////////////////////////////////////////////////////


class CDlgTab00 : public CScrollDlg
{
// Construction
public:
	bool m_bEvenSphere;
	bool m_bIsShowLine;
	bool m_bIsShowLabel;
	bool m_bShow2DSampling;
	bool m_bShow3DSampling;
	int m_nNumOfIterations;
	int m_nNumOfAccSpeed;
	bool m_bQuick;
	bool m_bAccelerated;
	bool m_bMemoryBoostered;
	bool m_bErrorDSphere;
	bool m_bSpherical;
	bool m_bIsShowArc;
	unsigned int m_nParamSpaceHeight;
	unsigned int m_nParamSpaceWidth;
	double m_dSampleDensity;
	void WriteCRLResult();
	void ResetOptiIterator();
	void WriteOptiParams(OptiParams optiParams);
	void OpenLog();
	void WriteResultPass(CString strAve, CString strCov, CString strGeo);
	
	void WriteResultInit(CString section, CString absoluteErr, CString incrementErr, int step);
	void WriteResultInit1(CString section, CString absoluteErr, CString incrementErr, int step);
	void WriteResultSettings(CString section);
    void WriteSettings(CString section);
	void WriteIniResult();
	void WriteIniParams();
	void WriteLog();
	void InsertMsg(char *absoluteErr, char *incrementalErr, int item);
	void InsertMsg1(char *absoluteErr, char *incrementalErr, int item);
	void SendMyMsg();
	void SendMyMsg1();

	COLORREF GetBckColor() { return m_colorBck; }
	void     SetBckColor(COLORREF bckColor);

	~CDlgTab00();
	void Load3DData(FILE* fp, char type);
	//##ModelId=40D51559034A
	CDlgTab00(CWnd* pParent = NULL);   // standard constructor

	int   m_nItem;

	//for display the messages
	CHistoryEdit m_HistoryEdit;
	//
	//ofstream m_LogFile;
	CStdioFile m_LogFile;
	char m_strCurrentDir[256];
	CString m_cstrFName;
	CString m_strPath; 
	CIniFile m_myini;
	CString m_strTime;
	CString m_currentSection;
	int     m_nPass;


// Dialog Data
	//{{AFX_DATA(CDlgTab00)
	enum { IDD = IDD_TAB_DIALOG00 };
	CButton	m_btnSave2DMesh;
	BOOL	m_bCHECK_1ST;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTab00)
	protected:
	//##ModelId=40D515590348
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	//the color for setting background of OpenGL
	COLORREF m_colorBck;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTab00)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBUTTONOpenPLY();
	afx_msg void OnButtonOpen2dmesh();
	afx_msg void OnButtonMeshInfor();
	afx_msg void OnChangeEDITSampleDensity();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnButtonOpenLog();
	afx_msg void OnButtonSave2dmesh();
	afx_msg void OnButtonPointclould();
	//}}AFX_MSG
	LONG OnMyMessage(WPARAM w,LPARAM l); 
	LONG OnMyMessage1(WPARAM w,LPARAM l); 
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTAB00_H__325263A9_CFAC_46A0_AC45_D5EBB1073A67__INCLUDED_)
