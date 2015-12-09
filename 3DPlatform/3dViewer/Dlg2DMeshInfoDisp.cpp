// Dlg2DMeshInfoDisp.cpp : implementation file
//

#include "stdafx.h"
#include "3dviewer.h"
#include "Dlg2DMeshInfoDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg2DMeshInfoDisp dialog


CDlg2DMeshInfoDisp::CDlg2DMeshInfoDisp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg2DMeshInfoDisp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg2DMeshInfoDisp)
	m_str2DNum = _T("");
	m_str3DNum = _T("");
	m_strEdgeNum = _T("");
	m_strSampleGap = _T("");
	m_strFaceNumber = _T("");
	m_strBorderNodeNum = _T("");
	m_strGenus = _T("");
	m_str2DEdgeNum = _T("");
	m_str2DNodeNum = _T("");
	m_str2DFaceNum = _T("");
	//}}AFX_DATA_INIT
}


void CDlg2DMeshInfoDisp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg2DMeshInfoDisp)
	DDX_Text(pDX, IDC_STATIC_2D_NUM, m_str2DNum);
	DDX_Text(pDX, IDC_STATIC_3D_NUM, m_str3DNum);
	DDX_Text(pDX, IDC_STATIC_EDGE_NUM, m_strEdgeNum);
	DDX_Text(pDX, IDC_STATIC_SAMPLE_GAP, m_strSampleGap);
	DDX_Text(pDX, IDC_STATIC_FACE_NUMBER, m_strFaceNumber);
	DDX_Text(pDX, IDC_STATIC_BORDER_NODE_NUM, m_strBorderNodeNum);
	DDX_Text(pDX, IDC_STATIC_GENUS_NUM, m_strGenus);
	DDX_Text(pDX, IDC_STATIC_2D_EDGE_NUM, m_str2DEdgeNum);
	DDX_Text(pDX, IDC_STATIC_2D_NODE_NUM, m_str2DNodeNum);
	DDX_Text(pDX, IDC_STATIC_2D_FACE_NUMBER, m_str2DFaceNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg2DMeshInfoDisp, CDialog)
	//{{AFX_MSG_MAP(CDlg2DMeshInfoDisp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg2DMeshInfoDisp message handlers
