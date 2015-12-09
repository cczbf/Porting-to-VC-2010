// CDlgTab03.cpp : implementation file
//

#include "stdafx.h"
#include "3dViewer.h"
#include "CDlgTab03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTab03 dialog


CDlgTab03::CDlgTab03(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab03::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTab03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTab03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTab03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTab03, CDialog)
	//{{AFX_MSG_MAP(CDlgTab03)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTab03 message handlers

void CDlgTab03::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void CDlgTab03::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}
