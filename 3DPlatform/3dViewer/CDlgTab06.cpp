// CDlgTab06.cpp : implementation file
//

#include "stdafx.h"
#include "3dViewer.h"
#include "CDlgTab06.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTab06 dialog


CDlgTab06::CDlgTab06(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab06::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTab06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTab06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTab06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTab06, CDialog)
	//{{AFX_MSG_MAP(CDlgTab06)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTab06 message handlers

void CDlgTab06::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CDlgTab06::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}
