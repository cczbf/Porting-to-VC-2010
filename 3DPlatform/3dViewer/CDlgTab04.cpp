// CDlgTab04.cpp : implementation file
//

#include "stdafx.h"
#include "3dViewer.h"
#include "CDlgTab04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTab04 dialog


CDlgTab04::CDlgTab04(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab04::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTab04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTab04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTab04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTab04, CDialog)
	//{{AFX_MSG_MAP(CDlgTab04)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTab04 message handlers

void CDlgTab04::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CDlgTab04::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}
