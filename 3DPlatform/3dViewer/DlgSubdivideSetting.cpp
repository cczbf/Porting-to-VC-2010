// DlgSubdivideSetting.cpp : implementation file
//

#include "stdafx.h"
#include "3dviewer.h"
#include "DlgSubdivideSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubdivideSetting dialog


CDlgSubdivideSetting::CDlgSubdivideSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubdivideSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubdivideSetting)
	m_strLongestEdge = _T("");
	m_strShortestEdge = _T("");
	m_strInputLen = _T("");
	//}}AFX_DATA_INIT
}


void CDlgSubdivideSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubdivideSetting)
	DDX_Text(pDX, IDC_STATIC_LONGEST_EDGE, m_strLongestEdge);
	DDX_Text(pDX, IDC_STATIC_SHORTEST_EDGE, m_strShortestEdge);
	DDX_Text(pDX, IDC_EDIT_INPUT_LEN, m_strInputLen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSubdivideSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSubdivideSetting)
	ON_EN_CHANGE(IDC_EDIT_INPUT_LEN, OnChangeEditInputLen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubdivideSetting message handlers

void CDlgSubdivideSetting::OnChangeEditInputLen() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData(1);

	// TODO: Add your control notification handler code here
	
}
