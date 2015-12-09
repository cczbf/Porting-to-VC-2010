// DlgOptiSettings.cpp : implementation file
//

#include "stdafx.h"
#include "3dviewer.h"
#include "DlgOptiSettings.h"
#include "cglobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptiSettings dialog


CDlgOptiSettings::CDlgOptiSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptiSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptiSettings)
	m_fErrorBound = 0.01f;
	m_fIncrementBound = 0.0001f;
	m_nIterationLimit = 100;
	m_ckNo = TRUE;
	m_ckOptiGlobal = TRUE;
	m_ckOptiLocal = FALSE;
	m_ckOptiMatlab = FALSE;
	m_ckOptiNo = FALSE;
	m_ckTargetFound = TRUE;
	m_ckTargetOurs = FALSE;
	m_ckYes = FALSE;
	m_ckLwModeEqui = FALSE;
	m_ckLwModePreserveLength = TRUE;
	m_fWeight = 1.0;
	m_strFilePath = _T("");
	m_bIsNode2Label = FALSE;
	m_ckGlobalLocal = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgOptiSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptiSettings)
	DDX_Control(pDX, IDC_CHECK_OPTI_GLOBAL_LOCAL, m_ctGlobalLocal);
	DDX_Control(pDX, IDC_CHECK_TARGET_OURS, m_ctTargetOurs);
	DDX_Control(pDX, IDC_CHECK_TARGET_FOUNDAMENTAL, m_ctTargetFound);
	DDX_Control(pDX, IDC_CHECK_OPTI_NO_OPTI, m_ctOptiNo);
	DDX_Control(pDX, IDC_CHECK_OPTI_MATLAB, m_ctOptiMatlab);
	DDX_Control(pDX, IDC_CHECK_OPTI_LOCAL, m_ctOptiLocal);
	DDX_Control(pDX, IDC_CHECK_OPTI_GLOBAL, m_ctOptiGlobal);
	DDX_Control(pDX, IDC_CHECK_LW_MODE_LENGTH_PRESERVE, m_ctLwLengthPreserve);
	DDX_Control(pDX, IDC_CHECK_LW_MODE_EQUI_ANGLE, m_ctLwEqui);
	DDX_Control(pDX, IDC_CHECK_NO, m_ctNo);
	DDX_Control(pDX, IDC_CHECK_YES, m_ctYes);
	DDX_Text(pDX, IDC_EDIT_ERROR_BOUND, m_fErrorBound);
	DDX_Text(pDX, IDC_EDIT_INCREMENT_BOUND, m_fIncrementBound);
	DDX_Text(pDX, IDC_EDIT_ITERATION_LIMIT, m_nIterationLimit);
	DDX_Check(pDX, IDC_CHECK_NO, m_ckNo);
	DDX_Check(pDX, IDC_CHECK_OPTI_GLOBAL, m_ckOptiGlobal);
	DDX_Check(pDX, IDC_CHECK_OPTI_LOCAL, m_ckOptiLocal);
	DDX_Check(pDX, IDC_CHECK_OPTI_MATLAB, m_ckOptiMatlab);
	DDX_Check(pDX, IDC_CHECK_OPTI_NO_OPTI, m_ckOptiNo);
	DDX_Check(pDX, IDC_CHECK_TARGET_FOUNDAMENTAL, m_ckTargetFound);
	DDX_Check(pDX, IDC_CHECK_TARGET_OURS, m_ckTargetOurs);
	DDX_Check(pDX, IDC_CHECK_YES, m_ckYes);
	DDX_Check(pDX, IDC_CHECK_LW_MODE_EQUI_ANGLE, m_ckLwModeEqui);
	DDX_Check(pDX, IDC_CHECK_LW_MODE_LENGTH_PRESERVE, m_ckLwModePreserveLength);
	DDX_Text(pDX, IDC_EDIT_WEIGHT, m_fWeight);
	DDV_MinMaxDouble(pDX, m_fWeight, 0., 1.01);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_strFilePath);
	DDX_Check(pDX, IDC_CHECK_NODE2LABEL, m_bIsNode2Label);
	DDX_Check(pDX, IDC_CHECK_OPTI_GLOBAL_LOCAL, m_ckGlobalLocal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptiSettings, CDialog)
	//{{AFX_MSG_MAP(CDlgOptiSettings)
	ON_BN_CLICKED(IDC_CHECK_LW_MODE_EQUI_ANGLE, OnCheckLwModeEquiAngle)
	ON_BN_CLICKED(IDC_CHECK_LW_MODE_LENGTH_PRESERVE, OnCheckLwModeLengthPreserve)
	ON_BN_CLICKED(IDC_CHECK_YES, OnCheckYes)
	ON_BN_CLICKED(IDC_CHECK_NO, OnCheckNo)
	ON_BN_CLICKED(IDC_CHECK_OPTI_NO_OPTI, OnCheckOptiNoOpti)
	ON_BN_CLICKED(IDC_CHECK_OPTI_GLOBAL, OnCheckOptiGlobal)
	ON_BN_CLICKED(IDC_CHECK_OPTI_LOCAL, OnCheckOptiLocal)
	ON_BN_CLICKED(IDC_CHECK_OPTI_MATLAB, OnCheckOptiMatlab)
	ON_BN_CLICKED(IDC_CHECK_TARGET_FOUNDAMENTAL, OnCheckTargetFoundamental)
	ON_BN_CLICKED(IDC_CHECK_TARGET_OURS, OnCheckTargetOurs)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_FILE, OnButtonInputFile)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_CHECK_NODE2LABEL, OnCheckNode2label)
	ON_BN_CLICKED(IDC_CHECK_OPTI_GLOBAL_LOCAL, OnCheckOptiGlobalLocal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptiSettings message handlers

void CDlgOptiSettings::OnCheckLwModeEquiAngle() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_ckLwModePreserveLength = FALSE;
	m_ctLwLengthPreserve.SetCheck(m_ckLwModePreserveLength);
	m_ctLwEqui.SetCheck(m_ckLwModeEqui);

}

void CDlgOptiSettings::OnCheckLwModeLengthPreserve() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);

	m_ckLwModeEqui = FALSE;
	m_ctLwLengthPreserve.SetCheck(m_ckLwModePreserveLength);
	m_ctLwEqui.SetCheck(m_ckLwModeEqui);
	
}

void CDlgOptiSettings::OnCheckYes() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_ckNo = FALSE;	
	m_ctNo.SetCheck(m_ckNo);
}

void CDlgOptiSettings::OnCheckNo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_ckYes = FALSE;	
	m_ctYes.SetCheck(m_ckYes);
}

void CDlgOptiSettings::OnCheckOptiNoOpti() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);

	m_ckOptiGlobal = FALSE;
	m_ckOptiLocal = FALSE;
	m_ckOptiMatlab = FALSE;
	m_ckGlobalLocal = FALSE;
  
	m_ctGlobalLocal.SetCheck(m_ckGlobalLocal); 	
	m_ctOptiNo.SetCheck(m_ckOptiNo);
	m_ctOptiMatlab.SetCheck(m_ckOptiMatlab);
	m_ctOptiLocal.SetCheck(m_ckOptiLocal);
	m_ctOptiGlobal.SetCheck(m_ckOptiGlobal);	
}

void CDlgOptiSettings::OnCheckOptiGlobal() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
		
	m_ckOptiLocal = FALSE;
	m_ckOptiMatlab = FALSE;
	m_ckOptiNo = FALSE;
	m_ckGlobalLocal = FALSE;
  
	m_ctGlobalLocal.SetCheck(m_ckGlobalLocal);
	m_ctOptiNo.SetCheck(m_ckOptiNo);
	m_ctOptiMatlab.SetCheck(m_ckOptiMatlab);
	m_ctOptiLocal.SetCheck(m_ckOptiLocal);
	m_ctOptiGlobal.SetCheck(m_ckOptiGlobal);
}

void CDlgOptiSettings::OnCheckOptiLocal() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);

	m_ckOptiGlobal = FALSE;
	m_ckOptiMatlab = FALSE;
    m_ckOptiNo = FALSE;
	m_ckGlobalLocal = FALSE;
  
	m_ctGlobalLocal.SetCheck(m_ckGlobalLocal); 	
	m_ctOptiNo.SetCheck(m_ckOptiNo);
	m_ctOptiMatlab.SetCheck(m_ckOptiMatlab);
	m_ctOptiLocal.SetCheck(m_ckOptiLocal);
	m_ctOptiGlobal.SetCheck(m_ckOptiGlobal);	
}

void CDlgOptiSettings::OnCheckOptiMatlab() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);

	m_ckOptiGlobal = FALSE;
	m_ckOptiLocal = FALSE;
	m_ckOptiNo = FALSE;

	m_ctOptiNo.SetCheck(m_ckOptiNo);
	m_ctOptiMatlab.SetCheck(m_ckOptiMatlab);
	m_ctOptiLocal.SetCheck(m_ckOptiLocal);
	m_ctOptiGlobal.SetCheck(m_ckOptiGlobal);	
}

void CDlgOptiSettings::OnCheckTargetFoundamental() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_ckTargetOurs = FALSE;
	
	m_ctTargetFound.SetCheck(m_ckTargetFound);
	m_ctTargetOurs.SetCheck(m_ckTargetOurs);		
}

void CDlgOptiSettings::OnCheckTargetOurs() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_ckTargetFound = FALSE;

	m_ctTargetFound.SetCheck(m_ckTargetFound);
	m_ctTargetOurs.SetCheck(m_ckTargetOurs);	
}

void CDlgOptiSettings::OnButtonInputFile() 
{
	// TODO: Add your control notification handler code here
	
	CFileDialog  dilg(TRUE, NULL, "*.ply",    //OPENFILENAME
	                  OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                  "PLY Files (*.ply)|*.ply|All Files (*.*)|*.*||");
	
	if( dilg.DoModal() == IDOK )
	{
		m_strFilePath = dilg.GetPathName();
		UpdateData(0);		
	}
}

BOOL CDlgOptiSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//g_CGlobals.m_pDlgOptiSettings = this;

	// TODO: Add extra initialization here
/*
	CString    strItem1 = _T("Step");
	CString    strItem2 = _T("Absolute Error");
	CString    strItem3 = _T("incremental Error"); 
	
	CRect rect;
	m_listMsg.GetWindowRect(&rect);	
	
	m_listMsg.InsertColumn(0, strItem1, LVCFMT_LEFT,
		rect.Width() * 1/5, 0);
	m_listMsg.InsertColumn(1, strItem2, LVCFMT_LEFT,
		rect.Width() * 2/5, 1);
	m_listMsg.InsertColumn(1, strItem3, LVCFMT_LEFT,
		rect.Width() * 2/5, 2);
		*/


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOptiSettings::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
}

void CDlgOptiSettings::InsertMsg(char *absoluteErr, char *incrementalErr, int item)
{
	/*
	CString step;
	step.Format("%d", item);
	m_listMsg.InsertItem(item, step); 

	for(int subitem=1; subitem<3; subitem++)
	{			
		LV_ITEM         lvitem;
		
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = item;
		lvitem.iSubItem = subitem;
		
		if(subitem == 1)
			lvitem.pszText = absoluteErr;
		else
			lvitem.pszText = incrementalErr;

		m_listMsg.SetItem(&lvitem);
	}
	*/
}

LONG CDlgOptiSettings::OnMyMessage(WPARAM w,LPARAM l) 
{
	LONG t(0);
	return t;
}



void CDlgOptiSettings::OnCheckNode2label() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);	
}

void CDlgOptiSettings::OnCheckOptiGlobalLocal() 
{
	// TODO: Add your control notification handler code here
	UpdateData(1);

	m_ckOptiGlobal = FALSE;
	m_ckOptiMatlab = FALSE;
	m_ckOptiLocal = FALSE; 
    m_ckOptiNo = FALSE;

	m_ctOptiNo.SetCheck(m_ckOptiNo);
	m_ctOptiMatlab.SetCheck(m_ckOptiMatlab);
	m_ctOptiLocal.SetCheck(m_ckOptiLocal);
	m_ctOptiGlobal.SetCheck(m_ckOptiGlobal);
	m_ctGlobalLocal.SetCheck(m_ckGlobalLocal); 		
}
