// cDlgTestPanel.cpp : implementation file
//

#include "stdafx.h"
#include "3dViewer.h"
#include "cDlgTestPanel.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTestPanel dialog


CDlgTestPanel::CDlgTestPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTestPanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTestPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTestPanel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTestPanel, CDialog)
	//{{AFX_MSG_MAP(CDlgTestPanel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TestPanels, OnSelchangeTABTestPanels)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_TestPanels, OnSelchangingTABTestPanels)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTestPanel message handlers

BOOL CDlgTestPanel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd( );

	
	CTabCtrl *pCTabCtrl = (CTabCtrl *)this->GetDlgItem(IDC_TAB_TestPanels);

//	m_pCTestTab01		= new CTestTab01(pCTabCtrl);
//	m_pCTestTab01		= new CTestTab01;
	m_CTabHub.OnInitDialog(pCTabCtrl);
/*
	m_pCHtArrayTest1Tab				= new CHtArrayTest1Tab(pCTabCtrl);
	m_pCHtArrayGenTab				= new CHtArrayGenTab(pCTabCtrl);	
	m_pCHtArrayTestTab				= new CHtArrayTestTab(pCTabCtrl);
	m_pCErrDifTestTab				= new CErrDifTestTab(pCTabCtrl);
	m_CPoissonDistribTestTab		= new CPoissonDistribTestTab(pCTabCtrl);
	m_pCTemporalTestTab				= new CTemporalTestTab(pCTabCtrl);
	m_pCLocalOptimizeTestTab		= new CLocalOptimizeTestTab(pCTabCtrl);
//	m_pCErrorD2ThAryTestTab			= new CErrorD2ThAryTestTab(pCTabCtrl);

*/
    
	pCTabCtrl->SetCurSel(0);

	NMHDR	NMHDR;
	LRESULT Result;
 
	this->OnSelchangingTABTestPanels(&NMHDR, &Result);
	this->OnSelchangeTABTestPanels(&NMHDR, &Result);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	
}

void CDlgTestPanel::OnSelchangeTABTestPanels(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTabCtrl *pCTabCtrl = (CTabCtrl *)this->GetDlgItem(IDC_TAB_TestPanels);
//	int nIndex	= pCTabCtrl->GetCurSel()+1;
	this->m_CTabHub.OnSelchangeTABTestPanels(pNMHDR, pResult,pCTabCtrl);
	*pResult = 0;
}

void CDlgTestPanel::OnSelchangingTABTestPanels(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTabCtrl *pCTabCtrl = (CTabCtrl *)this->GetDlgItem(IDC_TAB_TestPanels);
//	int nIndex	= pCTabCtrl->GetCurSel()+1;

	this->m_CTabHub.OnSelchangingTABTestPanels(pNMHDR, pResult,pCTabCtrl); 
	*pResult = 0;
}
