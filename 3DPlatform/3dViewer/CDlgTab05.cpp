// CDlgTab05.cpp : implementation file
//

#include "stdafx.h"
#include "3dViewer.h"
#include "CDlgTab05.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTab05 dialog


CDlgTab05::CDlgTab05(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab05::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTab05)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTab05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTab05)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTab05, CDialog)
	//{{AFX_MSG_MAP(CDlgTab05)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTab05 message handlers

void CDlgTab05::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void CDlgTab05::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}
