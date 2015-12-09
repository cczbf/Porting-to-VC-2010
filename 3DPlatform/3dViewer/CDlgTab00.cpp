 // CDlgTab00.cpp : implementation file
//

#include "stdafx.h"
#include "Triangles.h"

#include "3dViewer.h"
#include "CDlgTab00.h"
#include "CDIB.h"
#include "CLDIFile.h"
#include "3dViewerDoc.h"
#include "CGlobals.h"
#include "COpenGL3DEx.h"
#include "CDiffusionVarCo.h"
#include "Dlg2DMeshInfoDisp.h"
#include "DlgOptiSettings.h"
#include "tetgen.h"



#include "CMeshOpti.h"
#include "CRelxationLabelling.h"

#include "2DMesh.h"

#include <GL/glut.h>


//using std::min;


#define PI 3.14159265358979323846

CMeshOpti *g_pCMeshOpti = NULL;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTab00 dialog

//extern CMeshOpti *g_pCMeshOpti;

CDlgTab00::CDlgTab00(CWnd* pParent /*=NULL*/)
	: CScrollDlg(CDlgTab00::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTab00)
	m_bCHECK_1ST = !FALSE;
	//}}AFX_DATA_INIT
	
	m_nItem = 0;

	//open the log.ini file
	//FILE *fp = fopen("log.ini", "r");
	//m_LogFile << 1 <<endl;
	//m_LogFile << " Hello world! " <<endl;
	//m_LogFile.close();o
	//if( !m_LogFile )
	//	AfxMessageBox(" Can not open the log.ini file! ");
		
	m_nPass = 0;    

	m_colorBck = RGB(0, 0, 0);
}


void CDlgTab00::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTab00)
	DDX_Control(pDX, IDC_BUTTON_SAVE_2DMESH, m_btnSave2DMesh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTab00, CScrollDlg)
	//{{AFX_MSG_MAP(CDlgTab00)
	ON_BN_CLICKED(IDC_BUTTON_OpenPLY, OnBUTTONOpenPLY)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_2DMESH, OnButtonOpen2dmesh)
	ON_BN_CLICKED(IDC_BUTTON_MESH_INFOR, OnButtonMeshInfor)
	ON_EN_CHANGE(IDC_EDIT_Sample_Density, OnChangeEDITSampleDensity)
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_WM_CAPTURECHANGED()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_LOG, OnButtonOpenLog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_2DMESH, OnButtonSave2dmesh)
	ON_BN_CLICKED(IDC_BUTTON_POINTCLOULD, OnButtonPointclould)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYMESSAGE,OnMyMessage) 
	ON_MESSAGE(WM_MYMESSAGE1,OnMyMessage1) 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTab00 message handlers

void CDlgTab00::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CDlgTab00::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

  
void CDlgTab00::Load3DData(FILE* fp, char type)
{
//	if (!ConfirmOverWrite())
//	   return;
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	((COpenGL3D*)pDoc->m_pCOpenGL)->Load3DData(fp, type);
//	Show3DForm();
}


void CDlgTab00::OnBUTTONOpenPLY() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(1);

	CFileDialog  dilg(TRUE, NULL, "*.ply",    //OPENFILENAME
	                  OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                  "PLY Files (*.ply)|*.ply|OBJ Files (*.obj)|*.obj|All Files (*.*)|*.*| |");
	                                               
	dilg.m_ofn.lpstrTitle="Load Model File";
	
	if (dilg.DoModal() == IDOK)
	{	
		m_btnSave2DMesh.EnableWindow(FALSE);

		this->m_bSpherical = !FALSE;
		this->UpdateData(0);

		//for list control
		m_nItem = 0;
		m_nPass = 0;

		CString cstrFName = dilg.GetPathName( );
		m_cstrFName = cstrFName;
		CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
		
		//release the old mesh
		if(  pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] != NULL)
		{
			delete pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] ;
			pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] = NULL;
		}
		
		//produce a new mesh
		pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex]  = new CMesh;
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh = ( pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] ); 
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->m_nType = 0;
				
		//load the new mesh
		bool bIsThrough = TRUE;
		
		bIsThrough = pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex]->Load(cstrFName);


		//if the mesh is too large, display a message
		if(!bIsThrough)
//			AfxMessageBox(" CRelaxationLabelling Initialization Failed!");
			AfxMessageBox("Worning: This message is shown because possiblly because the mesh is too large. (more then 5000 vertex?)");

		//set the doc title
		pDoc->m_strFileName = cstrFName;
		pDoc->m_strTitle[pDoc->m_nCurrentMeshIndex] = pDoc->m_strFileName ;

		//ready for new mesh
		pDoc->m_nCurrentMeshIndex ++;
		pDoc->m_nCurrentMeshIndex = pDoc->m_nCurrentMeshIndex % 2;
		
		// Borrowing the following functions to initize the COpenGL
		HANDLE hRaw = GlobalAlloc(GHND, 4*sizeof(BYTE));
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->Load3DData(hRaw, 2, 2, 0.0, 0.0, 0.0, 0.0);
		GlobalFree ( hRaw );
		
		//the the size of mesh is not too large, set the RL 
		if ( this->m_bSpherical && bIsThrough)
		{
			CMesh *pCMesh = ((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh; 
			pDoc->SetEmptyRL(); //produce a new CRL object
		   	
			CRelxationLabelling *pCRelxationLabelling = pDoc->m_pCRelxationLabelling;
			pCRelxationLabelling->SetDataFromMesh(*pCMesh);			
		}
		
		//normalize the mesh
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->Normalize(0.5);		
		
		//switch to the 3D display mode
		CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd( );
		pMainFrame->Set3DView();
		pDoc->UpdateView();
		pDoc->SetPathName((LPCTSTR)cstrFName);	
		
		//write into log edit box
		CTime time = CTime::GetCurrentTime();   // Construct Time object
		CString m_strTime =time.Format("%Y-%m-%d %H:%M:%S");
		m_HistoryEdit.AppendString(" ");  
		m_HistoryEdit.AppendString( m_strTime);			
		CString strCurrentFilePath ="Open Ply file: " + cstrFName ;
		m_HistoryEdit.AppendString(strCurrentFilePath);
		//////////////////////////////////////////////////////////////////////////
		
		//write log 
		m_currentSection = m_strTime + "  " + cstrFName;
		WriteResultSettings(m_currentSection);			
		//////////////////////////////////////////////////////////////////////////

		//refresh the geoimage processing
		if(g_CGlobals.m_pDlgTab02 != NULL)
			g_CGlobals.m_pDlgTab02->Invalidate();		
	}	
}

void CDlgTab00::OnButtonOpen2dmesh() 
{
	// TODO: Add your control notification handler code here
	
	CFileDialog  dilg(TRUE, NULL, "*.txt",    //OPENFILENAME
	                  OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                  "2D Mesh Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	                                               
	dilg.m_ofn.lpstrTitle="Load 2D Mesh Flile";
	
	if (dilg.DoModal() == IDOK)
	{	
		m_btnSave2DMesh.EnableWindow(TRUE);

		this->m_bSpherical = FALSE;
		this->UpdateData(0);
		CString cstrFName = dilg.GetPathName( );		
		CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
		
		//release the old mesh
		if(  pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] != NULL)
		{
			delete pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] ;
			pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] = NULL;
		}
		//produce a new mesh
		pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex]  = new C2DMesh;
	   
		//load the mesh
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh = ( pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex] ); 
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->m_nType = 1;
		pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex]->Load(cstrFName);

        //set the RL using the mesh
		CMesh *pCMesh = ((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh; 
		pDoc->SetEmptyRL();
		CRelxationLabelling *pCRelxationLabelling = pDoc->m_pCRelxationLabelling;
		pCRelxationLabelling->SetDataFromMesh(*pCMesh);				
		//////////////////////////////////////////////////////////////////////////
						
		pDoc->m_pMesh[pDoc->m_nCurrentMeshIndex]->Normalize(0.5f);

		//set the doc title
		pDoc->m_strFileName = cstrFName;
		pDoc->m_strTitle[pDoc->m_nCurrentMeshIndex] = pDoc->m_strFileName ;
	
		//ready for new mesh
        pDoc->m_nCurrentMeshIndex ++;
		pDoc->m_nCurrentMeshIndex = pDoc->m_nCurrentMeshIndex % 2;


		HANDLE hRaw = GlobalAlloc(GHND, 4*sizeof(BYTE));
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->Load3DData(hRaw, 2, 2, 0.0, 0.0, 0.0, 0.0);
		GlobalFree ( hRaw );
		
		pDoc->m_pCOpenGL->m_pDS->m_MapType = MESH2D;
		((COpenGL3DEx*)(pDoc->m_pCOpenGL))->Reset();			
		
		//switch to 3D display mode
		CMainFrame* pMainFrame	= (CMainFrame*)AfxGetMainWnd( );
		pMainFrame->Set3DView();
		pDoc->UpdateView();
	}	
}

void CDlgTab00::OnButtonMeshInfor() 
{
	// TODO: Add your control notification handler code here

	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	if( (pDoc->m_pCOpenGL != NULL) && (pDoc->m_pCOpenGL->m_pDS != NULL) )
		pDoc->m_pCOpenGL->KillOpenGLTimer(pDoc->m_pCOpenGL->m_pDS->m_Rotate);


	if (((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh !=NULL )
	{
		int nType = ((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->m_nType;
		
		CDlg2DMeshInfoDisp dlg;	

		switch(nType) {
		case 0:
		case 2:
			{
				int nNodeNum = ( (COpenGL3DEx *)(pDoc->m_pCOpenGL) )->m_pCMesh->m_CVertexArray.GetSize();
				int nFaceNum = ( (COpenGL3DEx *)(pDoc->m_pCOpenGL) )->m_pCMesh->m_CTriangleArray.GetSize();
				int nEdgeNum = ( (COpenGL3DEx *)(pDoc->m_pCOpenGL) )->m_pCMesh->GetEdgeNum();
				int nGenusNum = ( (COpenGL3DEx *)(pDoc->m_pCOpenGL) )->m_pCMesh->GetGenus(); 			
				
				dlg.m_str2DNum.Format("%d", nNodeNum);
				dlg.m_strFaceNumber.Format("%d", nFaceNum);		
				dlg.m_strEdgeNum.Format("%d", nEdgeNum);
				dlg.m_strGenus.Format("%d", nGenusNum);
			}
			break;

		case 1:
			{

				int nNodeNum = ( (COpenGL3DEx *)(pDoc->m_pCOpenGL) )->m_pCMesh->m_CVertexArray.GetSize();
				int nFaceNum = ( (COpenGL3DEx *)(pDoc->m_pCOpenGL) )->m_pCMesh->m_CTriangleArray.GetSize();
				int nEdgeNum = ( (COpenGL3DEx *)(pDoc->m_pCOpenGL) )->m_pCMesh->GetEdgeNum();
								
				dlg.m_str2DNodeNum.Format("%d", nNodeNum);
				dlg.m_str2DFaceNum.Format("%d", nFaceNum);		
				dlg.m_str2DEdgeNum.Format("%d", nEdgeNum);
				
				if ( pDoc->m_pCRelxationLabelling != NULL )
				{
					int nBorderNodeNum = ( (COpenGL3DEx *)(pDoc->m_pCOpenGL) )->m_pCMesh->GetBorderLen();
					
					dlg.m_str3DNum.Format("%d",
						pDoc->m_pCRelxationLabelling->m_n3DNode_length);
					
					dlg.m_strBorderNodeNum.Format("%d", nBorderNodeNum);
					
					dlg.m_strSampleGap.Format("%lf", 
						pDoc->m_pCRelxationLabelling->m_dSampleGap );
				}
				
			}
			break;
		default:
			break;
		}
		
		//dlg.m_strEdgeNum.Format("%d", 
		//	( (COpenGL3DEx *)(pDoc->m_pCOpenGL) )->m_pCMesh->m_pCRelxationLabelling->m_nEdgeList_length);
		//for 2d mesh 		
		dlg.DoModal();		
		//pDoc->m_pCOpenGL->SetOpenGLTimer(pDoc->m_pCOpenGL->m_pDS->m_Rotate);
	}
	else
	{
		AfxMessageBox(" 3D Mesh not Loaded!");
		
	}
		
}

 
 
  	
//}

void CDlgTab00::OnChangeEDITSampleDensity() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	this->UpdateData();
	// TODO: Add your control notification handler code here
	
}

CDlgTab00::~CDlgTab00()
{
	int i = 0;

	WriteSettings("Recent Settings");
}

BOOL CDlgTab00::OnInitDialog() 
{
	CDialog::OnInitDialog();

	g_CGlobals.m_pDlgTab00 = this;

	// TODO: Add extra initialization here
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	CMesh* pCMesh = ((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh;
	
	/*
	pDoc->SetEmptyRL();
	this->m_nParamSpaceWidth = 	pDoc->m_pCRelxationLabelling->m_nLabelListW;
	this->m_nParamSpaceHeight = pDoc->m_pCRelxationLabelling->m_nLabelListH;
	this->m_dSampleDensity = pDoc->m_pCRelxationLabelling->m_dSampleingDensity;
	*/

	this->UpdateData(0);

	//disable 2dmesh saving button
	m_btnSave2DMesh.EnableWindow(FALSE);

	//Edit box control
	m_HistoryEdit.SubclassDlgItem (EDIT_HISTORY, this);
	//////////////////////////////////////////////////////////////////////////

	// TODO: Add extra initialization here
	// TODO: Add extra initialization here
	m_nVscrollPos = 0;
    // Get the initial dimensions of the dialog
    GetClientRect(&m_ClientRect);
	
///////////////HERE IS THE TRICK////////////////////////	
	//Increase the height of Dialog at Runtime	
	m_ClientRect.bottom	*=	3;     
	//Initialize the VerticalScrollbar
    ResetScrollbars();
	//Setup The Vertical Scrollbar
	SetupScrollbars();
	

	GetCurrentDirectory(256, m_strCurrentDir);
	m_strPath = strcat(m_strCurrentDir, "\\Log.ini");
	
	//if the log file exist,read the file
	if( m_myini.SetPath(m_strPath) )
	{
		CString strSpherical = m_myini.GetKeyValue("Recent Settings", "Spherical");
		m_bSpherical = atoi(strSpherical);		

		CString strWidth = m_myini.GetKeyValue("Recent Settings", "Param Space Width");
		m_nParamSpaceWidth = atoi(strWidth);
		
		CString strHeight = m_myini.GetKeyValue("Recent Settings", "Param Space Height");
		m_nParamSpaceHeight = atoi(strHeight);
		
		CString strInteratioNumber = m_myini.GetKeyValue("Recent Settings", "Iteration Number");
		m_nNumOfIterations = atoi(strInteratioNumber);		

		CString strDisplayLabel = m_myini.GetKeyValue("Recent Settings", "Display Label");
		m_bIsShowLabel = atoi(strDisplayLabel);	
		((COpenGL3DEx *)(pDoc->m_pCOpenGL)) ->m_bIsShowLabel = m_bIsShowLabel;
		

		CString strDispGreatCircle = m_myini.GetKeyValue("Recent Settings", "Display Great Circle");
		m_bIsShowArc  = atoi(strDispGreatCircle);	
		((COpenGL3DEx *)(pDoc->m_pCOpenGL)) ->m_bIsShowArc = m_bIsShowArc;

		
     	CString strDispLine = m_myini.GetKeyValue("Recent Settings", "Display Linear Edge");
		m_bIsShowLine = atoi(strDispLine);
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_bIsShowLine = m_bIsShowLine;
		

		CString strErrorDiffusionSample = m_myini.GetKeyValue("Recent Settings", "Error Diffusion Sample");
		m_bErrorDSphere = atoi(strErrorDiffusionSample);
		
		CString strShow2DSample = m_myini.GetKeyValue("Recent Settings", "Spherical Sampling Show 2D Sampling");
		m_bShow2DSampling = atoi(strShow2DSample);
		
		CString strShow3DSample = m_myini.GetKeyValue("Recent Settings", "Spherical Sampling Show 3D Sampling");
		m_bShow3DSampling = atoi(strShow3DSample);
	
		CString strEvenDistributionSample = m_myini.GetKeyValue("Recent Settings", "Even Distribution Sample");
		m_bEvenSphere  = atoi(strEvenDistributionSample);
		
		CString strSampleDensity = m_myini.GetKeyValue("Recent Settings", "Sample Density");
		m_dSampleDensity = atof(strSampleDensity);

		CString strQuick = m_myini.GetKeyValue("Recent Settings", "Is Quick");
		m_bQuick = atoi(strQuick);
		//read color
		CString red = m_myini.GetKeyValue("Recent Settings","Background Red");
		CString green = m_myini.GetKeyValue("Recent Settings","Background Green");
		CString blue = m_myini.GetKeyValue("Recent Settings","Background Blue");
		m_colorBck = RGB( atoi(red), atoi(green), atoi(blue) );

		CString strAccelerated = m_myini.GetKeyValue("Recent Settings", "Is Accelerated");
		m_bAccelerated = atoi(strAccelerated);

		CString strAcceleratedSpeed = m_myini.GetKeyValue("Recent Settings", "Accelerated Speed");
		m_nNumOfAccSpeed = atoi(strAcceleratedSpeed);

	 	CString strMemoryBooster = m_myini.GetKeyValue("Recent Settings", "Is MemoryBooster");
		m_bMemoryBoostered = atoi(strMemoryBooster);

		CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
		pDoc->m_pCOpenGL->SetBckGroundColor(m_colorBck);

		UpdateData(0);
	}

	//write the panel settings
	WriteSettings("Recent Settings");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

 
void CDlgTab00::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDialog::OnPaint();
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgTab00::OnCancelMode() 
{
	CDialog::OnCancelMode();	
	// TODO: Add your message handler code here	
}

 
LONG CDlgTab00::OnMyMessage(WPARAM w,LPARAM l) 
{
	LONG t(0);

	InsertMsg(g_CGlobals.g_chAbsoluteErr, g_CGlobals.g_chIncrementErr, m_nItem);	
	m_nItem++;

	UpdateData(1);

	return t;
}

LONG CDlgTab00::OnMyMessage1(WPARAM w,LPARAM l) 
{
	LONG t(0);

	InsertMsg1(g_CGlobals.g_chAbsoluteErr, g_CGlobals.g_chIncrementErr, m_nItem);	
	m_nItem++;

	UpdateData(1);

	return t;
}

void CDlgTab00::SendMyMsg()
{
	SendMessage(WM_MYMESSAGE);
}

void CDlgTab00::SendMyMsg1()
{
	SendMessage(WM_MYMESSAGE1);
}

void CDlgTab00::InsertMsg(char *absoluteErr, char *incrementalErr, int item)
{
	/*
	CString step;
	step.Format("%d", item+1);
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
    
    CString strEditRow;
	strEditRow.Format(" Step:%d     Absolute Error: %s     Incremental Error: %s",
		                  item+1, absoluteErr, incrementalErr);
	
	//m_HistoryEdit.SetWindowText("");
	m_HistoryEdit.AppendString(strEditRow);

	//save as the result into the ini file
	//CString strItem;
	//strItem.Format("%d", item);
	//m_myini.AddKeyValue(m_strTime, "Step"+strItem, strItem);
	//m_myini.AddKeyValue(m_strTime, "Absoulte Error"+strItem, absoluteErr);
	//m_myini.AddKeyValue(m_strTime, "Incremental Error"+strItem, incrementalErr);	
	WriteResultInit(m_currentSection, absoluteErr, incrementalErr, item+1);	
}

void CDlgTab00::InsertMsg1(char *absoluteErr, char *incrementalErr, int item)
{
    CString strEditRow;
	strEditRow.Format(" Step:%d     Absolute Error Using N-S : %s     Incremental Error Using N-S: %s",
		                  item+1, absoluteErr, incrementalErr);
	
	//m_HistoryEdit.SetWindowText("");
	m_HistoryEdit.AppendString(strEditRow);
	WriteResultInit1(m_currentSection, absoluteErr, incrementalErr, item+1);	
}

void CDlgTab00::WriteLog()
{
	/*
	m_LogFile.open("log.ini", ios_base::app);

	CString strEditContent;
	m_HistoryEdit.GetWindowText(strEditContent);
	int nSize = strEditContent.GetLength();
	for (int i=0; i<)
	TCHAE c = strEditContent.GetAt()
	m_LogFile << str << endl;
	//m_LogFile << str << endl;
	//m_LogFile << "aaa" << endl;
	//m_LogFile << 1 << endl;
	m_LogFile.close();
	*/

	//using MFC CFile to write log 
	CString strEditContent;
	m_HistoryEdit.GetWindowText(strEditContent);
	strEditContent += "\n";
	
	//set the working path 
    SetCurrentDirectory(m_strCurrentDir);
    
	BOOL bIsOpen = m_LogFile.Open("log.ini", CFile::modeWrite);

	if(!bIsOpen)
	{
		bIsOpen = m_LogFile.Open("log.ini", CFile::modeCreate|CFile::modeWrite);

		CString strItem;
		strItem.Format("Param Space Width: %d ", m_nParamSpaceWidth);
		m_LogFile.WriteString(strItem + "\n"); 

		strItem.Format("Param Space Height: %d ", m_nParamSpaceHeight);
		m_LogFile.WriteString(strItem + "\n"); 

		strItem.Format("Sample Density: %lf ", m_dSampleDensity);
		m_LogFile.WriteString(strItem + "\n"); 

		//m_LogFile.Close();
	}
	//CStdioFile file("log.ini", CFile::modeWrite);
	if(bIsOpen)
	{	
		m_LogFile.SeekToEnd();
		m_LogFile.WriteString(strEditContent); 
		m_LogFile.Close();
	}
	//////////////////////////////////////////////////////////////////////////

}

//DEL void CDlgTab00::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
//DEL }

void CDlgTab00::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CDialog::OnCaptureChanged(pWnd);
}

void CDlgTab00::OnButtonOpenLog() 
{
	// TODO: Add your control notification handler code here
    //SetCurrentDirectory(m_strCurrentDir);

	//CString file( strcat(m_strCurrentDir, "\\log.int"));
	ShellExecute(NULL,"open","notepad.exe", m_strPath ,"",SW_SHOW );
}

//DEL void CDlgTab00::ResetScrollbars()
//DEL {
//DEL 
//DEL }

//DEL void CDlgTab00::SetupScrollbars()
//DEL {
//DEL 
//DEL }

void CDlgTab00::WriteIniParams()
{

	//if( myini.SetPath(m_strPath) )
	{
		CString strValue;
		strValue.Format(" %d", m_nParamSpaceWidth);		
		m_myini.AddKeyValue("Params", "Param Space Width", strValue);	
		
		strValue.Format(" %d", m_nParamSpaceHeight);		
		m_myini.AddKeyValue("Params", "Param Space Height", strValue);	

		strValue.Format(" %lf", m_dSampleDensity);		
		m_myini.AddKeyValue("Params", "Sample Density", strValue);		
	}
}

void CDlgTab00::WriteIniResult()
{	

	//CString settings = section + "  See"
		
}

void CDlgTab00::WriteSettings(CString section)
{		
	CString strValue;
	
	if(m_bSpherical)
		m_myini.AddKeyValue(section, "Spherical", "1");	
	else
		m_myini.AddKeyValue(section, "Spherical", "0");	

	
	strValue.Format(" %d", m_nParamSpaceWidth);		
	m_myini.AddKeyValue(section, "Param Space Width", strValue);	
	
	strValue.Format(" %d", m_nParamSpaceHeight);		
	m_myini.AddKeyValue(section, "Param Space Height", strValue);	

	strValue.Format("%d", m_nNumOfIterations);
	m_myini.AddKeyValue(section, "Iteration Number", strValue);	
    
	
	if(m_bIsShowLabel)
		m_myini.AddKeyValue(section, "Display Label", "1");	
	else
		m_myini.AddKeyValue(section, "Display Label", "0");	

	if(m_bIsShowArc)
		m_myini.AddKeyValue(section, "Display Great Circle", "1");	
	else
		m_myini.AddKeyValue(section, "Display Great Circle", "0");	

	if(m_bIsShowLine)
		m_myini.AddKeyValue(section, "Display Linear Edge", "1");	
	else
		m_myini.AddKeyValue(section, "Display Linear Edge", "0");	

	if(m_bErrorDSphere)
		m_myini.AddKeyValue("Recent Settings", "Error Diffusion Sample", "1");	
	else
		m_myini.AddKeyValue("Recent Settings", "Error Diffusion Sample", "0");	

	if(m_bShow2DSampling)
		m_myini.AddKeyValue(section, "Spherical Sampling Show 2D Sampling", "1");	
	else
		m_myini.AddKeyValue(section, "Spherical Sampling Show 2D Sampling", "0");	
	
	if(m_bShow3DSampling)
		m_myini.AddKeyValue(section, "Spherical Sampling Show 3D Sampling", "1");	
	else
		m_myini.AddKeyValue(section, "Spherical Sampling Show 3D Sampling", "0");	

	if(m_bEvenSphere)
		m_myini.AddKeyValue(section, "Even Distribution Sample", "1");	
	else
		m_myini.AddKeyValue(section, "Even Distribution Sample", "0");	
	
	if(m_bQuick)
		m_myini.AddKeyValue(section, "Is Quick", "1");	
	else
		m_myini.AddKeyValue(section, "Is Quick", "0");	

	if(m_bAccelerated)
		m_myini.AddKeyValue(section, "Is Accelerated", "1");	
	else
		m_myini.AddKeyValue(section, "Is Accelerated", "0");

	if(m_bMemoryBoostered)
		m_myini.AddKeyValue(section, "Is MemoryBooster", "1");	
	else
		m_myini.AddKeyValue(section, "Is MemoryBooster", "0");

	strValue.Format("%d", m_nNumOfAccSpeed);
    m_myini.AddKeyValue(section, "Accelerated Speed", strValue);	

	strValue.Format(" %lf", m_dSampleDensity);		
	m_myini.AddKeyValue(section, "Sample Density", strValue);	


	//write the Background Color
	int r,g,b;
	r = GetRValue(m_colorBck);
	g = GetGValue(m_colorBck);
	b = GetBValue(m_colorBck);
	strValue.Format("%d", r);
	m_myini.AddKeyValue(section, "Background Red", strValue);	
	strValue.Format("%d", g);
	m_myini.AddKeyValue(section, "Background Green", strValue);	
	strValue.Format("%d", b);
	m_myini.AddKeyValue(section, "Background Blue", strValue);	

}

void CDlgTab00::WriteResultSettings(CString section)
{
   CString settingSection = section + "  Settings";
   WriteSettings(settingSection);
}

void CDlgTab00::WriteResultInit(CString section, CString absoluteErr,
								CString incrementErr, int step)
{
	CString initSection = section + " Inital Optimization ";

	CString aerr;
	aerr.Format("Step%d Absolute Error", step);
	m_myini.AddKeyValue(initSection, aerr, absoluteErr );
	
	CString increErr;
	increErr.Format("Step%d Incremental Error", step);
	m_myini.AddKeyValue(initSection, increErr, incrementErr );

}

void CDlgTab00::WriteResultInit1(CString section, CString absoluteErr,
								CString incrementErr, int step)
{
	CString initSection = section + " Inital Optimization ";

	CString aerr;
	aerr.Format("Step%d Absolute Error Using N-S", step);
	m_myini.AddKeyValue(initSection, aerr, absoluteErr );
	
	CString increErr;
	increErr.Format("Step%d Incremental Error Using N-S", step);
	m_myini.AddKeyValue(initSection, increErr, incrementErr );

}


void CDlgTab00::WriteResultPass(CString strAve, 
								CString strCov,
								CString strGeo
								/*int pass*/)
{
	CString passSection = m_currentSection+"  < Pass:  Average Of Stretch Ratio   Covariance Of Stretch Ratio > ";

	CString strAveStep;
	strAveStep.Format("Pass%d ", m_nPass);
	m_myini.AddKeyValue(passSection, strAveStep, "\t"+strAve+"\t"+strCov);

	//CString strCovStep;
	//strCovStep.Format("Pass%d ", m_nPass);
	//m_myini.AddKeyValue(passSection, strCovStep, strCov);

	//CString strGeoStep;
	//strGeoStep.Format("Pass%d Geometry Stretch Ratio", m_nPass);
	//m_myini.AddKeyValue(passSection, strGeoStep, strGeo);

	m_nPass++;
}

void CDlgTab00::OpenLog()
{
	OnButtonOpenLog();
}

  
 
void CDlgTab00::WriteOptiParams(OptiParams optiParams)
{
	CString section = "Recent Settings";
	CString value;
	value.Format("%d", optiParams.iterationLimit);
	m_myini.AddKeyValue(section, "Init.Iteration Limit", value );

	value.Format("%d", optiParams.nNode2Label);
	m_myini.AddKeyValue(section, "Init.Node2Label", value );
	
	value.Format("%d", optiParams.nLwMode);	
	m_myini.AddKeyValue(section, "Init.Lw Mode", value );

	value.Format("%lf", optiParams.dfErrorBound);	
	m_myini.AddKeyValue(section, "Init.Error Bound", value );

	value.Format("%lf", optiParams.dfIncrementBound);	
	m_myini.AddKeyValue(section, "Init.Incremental Bound", value );

	value.Format("%d", optiParams.nMaptoSphereFirst);	
	m_myini.AddKeyValue(section, "Init.Map to Sphere First", value );

	value.Format("%d", optiParams.nOptiMode);
	m_myini.AddKeyValue(section, "Init.Optimization Mode", value );
	
	value.Format("%d", optiParams.nOptiTarget);	
	m_myini.AddKeyValue(section, "Init.Optimization Target", value );

	value.Format("%f", optiParams.fWeight);	
	m_myini.AddKeyValue(section, "Init.Weight ", value );
	
}

  
void CDlgTab00::ResetOptiIterator()
{
	m_nItem = 0;
}

void CDlgTab00::OnButtonSave2dmesh() 
{
	// TODO: Add your control notification handler code here
   	char strFilter[]="2D Mesh files(*.txt)|*.txt|All files(*.*)|*.*|";
	CFileDialog fDlg(FALSE,"txt","*.txt",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);
    
	if(fDlg.DoModal() == IDOK)
	{
		CString filePath = fDlg.GetPathName();
		CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();

		if(pDoc->m_pCRelxationLabelling != NULL)
			pDoc->m_pCRelxationLabelling->Save2DMesh(filePath);


		//if( (COpenGL3DEx *)(pDoc->m_pCOpenGL) != NULL )
		//	((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->Write(filePath);
         
	}

}

void  CDlgTab00::SetBckColor(COLORREF bckColor)
{
	m_colorBck = bckColor;
}

void CDlgTab00::WriteCRLResult()
{ 
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();
	CRelxationLabelling *pCRelxationLabelling = 
	pDoc->m_pCRelxationLabelling;

	double ave = pCRelxationLabelling->GetAvarageOfRatio();
	double cov = pCRelxationLabelling->GetCovOfRatio();
	CString strAve;
	strAve.Format("%lf", ave);
	CString strCov;
	strCov.Format("%lf", cov);

	CString strIndex;
	strIndex.Format("%d", m_nPass);
	m_HistoryEdit.AppendString("pass"+strIndex+": "+"\t"+strAve+"  ,  "+strCov);
	//m_HistoryEdit.AppendString(" Covariance of Stretch Ratio " + strIndex +" : " + strCov);

	WriteResultPass(strAve, strCov, "0");
}

  
 

void CDlgTab00::OnButtonPointclould() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(1);
	CFileDialog  dilg(TRUE, NULL, "*.node",    //OPENFILENAME
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                  "NODE Files (*.node)|*.node|OBJ Files (*.obj)|*.obj|All Files (*.*)|*.*| |");
	dilg.m_ofn.lpstrTitle="Load Model File";
	
	if (dilg.DoModal() == IDOK)
	{
		m_btnSave2DMesh.EnableWindow(FALSE);
		
		this->m_bSpherical = !FALSE;
		this->UpdateData(0);
		m_nItem = 0;
		m_nPass = 0;
		
		CString cstrFName = dilg.GetFileTitle( );
		tetgenio in,out;
		char *filename=cstrFName.GetBuffer(0);
		//AfxMessageBox(cstrFName);
		in.load_node(filename);


		/*for(int i = 0; i< (in.numberofpoints+1)/3; i++){
		
			double x,y,z;
			x=in.pointlist[i];
			y=in.pointlist[i+1];
			z=in.pointlist[i+2];
			glBegin( GL_POINTS);
			glvertex3f(x,y,z);
			glEnd();
		}*/
		tetrahedralize("",&in, &out);
		out.save_elements(filename);
	}
	
}
