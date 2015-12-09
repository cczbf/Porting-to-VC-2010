// CDlgTab07.cpp : implementation file
//

#include "stdafx.h"
#include "3dViewer.h"
#include "CDlgTab07.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTab07 dialog


CDlgTab07::CDlgTab07(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab07::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTab07)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTab07::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTab07)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTab07, CDialog)
	//{{AFX_MSG_MAP(CDlgTab07)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTab07 message handlers

void CDlgTab07::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void CDlgTab07::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}
