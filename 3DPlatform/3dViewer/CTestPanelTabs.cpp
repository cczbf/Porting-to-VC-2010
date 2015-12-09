//## begin module%40CFBD4300AE.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40CFBD4300AE.cm

//## begin module%40CFBD4300AE.cp preserve=no
//## end module%40CFBD4300AE.cp

//## Module: CTestPanelTabs%40CFBD4300AE; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CTestPanelTabs.cpp

//## begin module%40CFBD4300AE.additionalIncludes preserve=no
//## end module%40CFBD4300AE.additionalIncludes

//## begin module%40CFBD4300AE.includes preserve=yes
//## end module%40CFBD4300AE.includes

// CTestPanelTabs
#include "CTestPanelTabs.h"
//## begin module%40CFBD4300AE.declarations preserve=no
const char* cszTabName01                   = "旧阈值矩阵测试";
const char* cszOldHalftoneArrayGeneration  = "旧阈值矩阵生成";
const char* cszBalHalftoneArrayTest        = "方正平衡网点测试";
const char* cszBalancedHalftoneArrayGen    = "方正平衡网点生成";
const char* cszErrorDiffusion              = "误差扩散测试";
const char* cszPoissonDistribution         = "最短泊松分布";
const char* cszTemporalTestBed             = "临时测试平台";
const char* cszLocalOptimization            = "局部优化";
const char* cszThAryFromErrorD            = "误差扩散转阈值矩阵";
//## end module%40CFBD4300AE.declarations

//## begin module%40CFBD4300AE.additionalDeclarations preserve=yes
//## end module%40CFBD4300AE.additionalDeclarations


// Class CTabHub 











CTabHub::CTabHub()
  //## begin CTabHub::CTabHub%.hasinit preserve=no
  //## end CTabHub::CTabHub%.hasinit
  //## begin CTabHub::CTabHub%.initialization preserve=yes
  //## end CTabHub::CTabHub%.initialization
{
  //## begin CTabHub::CTabHub%.body preserve=yes
  //## end CTabHub::CTabHub%.body
}


CTabHub::~CTabHub()
{
  //## begin CTabHub::~CTabHub%.body preserve=yes
  //## end CTabHub::~CTabHub%.body
}



//## Other Operations (implementation)
BOOL CTabHub::OnInitDialog (CTabCtrl* pCTabParent)
{
  //## begin CTabHub::OnInitDialog%40D2B3C90092.body preserve=yes

//	m_pCTestTab01		= new CTestTab01;

	m_CTestTab00.CTestPanelTabsInit(pCTabParent);
	m_CTestTab01.CTestPanelTabsInit(pCTabParent);
	m_CTestTab02.CTestPanelTabsInit(pCTabParent);
	m_CTestTab03.CTestPanelTabsInit(pCTabParent);
	m_CTestTab04.CTestPanelTabsInit(pCTabParent);
	m_CTestTab05.CTestPanelTabsInit(pCTabParent);
	m_CTestTab06.CTestPanelTabsInit(pCTabParent);
	m_CTestTab07.CTestPanelTabsInit(pCTabParent);

	return TRUE;
  //## end CTabHub::OnInitDialog%40D2B3C90092.body
}

void CTabHub::OnSelchangeTABTestPanels (NMHDR* pNMHDR, LRESULT* pResult, CTabCtrl* pCTabCtrl)
{
  //## begin CTabHub::OnSelchangeTABTestPanels%40D3A0A8005B.body preserve=yes
	this->m_CTestTab00.TabChange(pCTabCtrl);
	this->m_CTestTab01.TabChange(pCTabCtrl);
	this->m_CTestTab02.TabChange(pCTabCtrl);
	this->m_CTestTab03.TabChange(pCTabCtrl);
	this->m_CTestTab04.TabChange(pCTabCtrl);
	this->m_CTestTab05.TabChange(pCTabCtrl);
	this->m_CTestTab06.TabChange(pCTabCtrl);
	this->m_CTestTab07.TabChange(pCTabCtrl);
  //## end CTabHub::OnSelchangeTABTestPanels%40D3A0A8005B.body
}

void CTabHub::OnSelchangingTABTestPanels (NMHDR* pNMHDR, LRESULT* pResult, CTabCtrl* pCTabCtrl)
{
  //## begin CTabHub::OnSelchangingTABTestPanels%40D3A0B902E0.body preserve=yes
	this->m_CTestTab00.TabChanging(pCTabCtrl);
	this->m_CTestTab01.TabChanging(pCTabCtrl);
	this->m_CTestTab02.TabChanging(pCTabCtrl);
	this->m_CTestTab03.TabChanging(pCTabCtrl);	
	this->m_CTestTab04.TabChanging(pCTabCtrl);
	this->m_CTestTab05.TabChanging(pCTabCtrl);
	this->m_CTestTab06.TabChanging(pCTabCtrl);
	this->m_CTestTab07.TabChanging(pCTabCtrl);
  //## end CTabHub::OnSelchangingTABTestPanels%40D3A0B902E0.body
}

// Additional Declarations
  //## begin CTabHub%40D2B35302F5.declarations preserve=yes
  //## end CTabHub%40D2B35302F5.declarations

// Parameterized Class CTestPanelTabs 







template <int nIndex, int nDialogID, class CDlgTabContent>
CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::CTestPanelTabs()
  //## begin CTestPanelTabs::CTestPanelTabs%.hasinit preserve=no
      : m_nIndex(0), m_pCTabCtrlParent(NULL), m_pCTabCtrl0(NULL), m_pCDlgTabContent(NULL),
	    m_szLableString("\\Open\\Dispay\\Tab2\\Tab3\\Tab4\\Tab5\\Tab6\\Tab7\\")
  //## end CTestPanelTabs::CTestPanelTabs%.hasinit 
  //## begin CTestPanelTabs::CTestPanelTabs%.initialization preserve=yes
  //## end CTestPanelTabs::CTestPanelTabs%.initialization
{
  //## begin CTestPanelTabs::CTestPanelTabs%.body preserve=yes
  //## end CTestPanelTabs::CTestPanelTabs%.body
}


template <int nIndex, int nDialogID, class CDlgTabContent>
CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::~CTestPanelTabs ()
{
  //## begin CTestPanelTabs::~CTestPanelTabs%3691091D0129.body preserve=yes

  if ( m_pCDlgTabContent != NULL )
  {
	delete m_pCDlgTabContent;
	m_pCDlgTabContent	= NULL;
  }


  //## end CTestPanelTabs::~CTestPanelTabs%3691091D0129.body
}



//## Other Operations (implementation)
template <int nIndex, int nDialogID, class CDlgTabContent>
void CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::CTestPanelTabsInit (CTabCtrl* pCTabCtrlParent)
{
  //## begin CTestPanelTabs::CTestPanelTabsInit%369103FD033F.body preserve=yes
	m_nIndex	= nIndex;
	
//	ASSERT(strlen(cszTabLable)<127);
	char pcBuf[1024];
	char *pcMyLable = pcBuf;
	strcpy (pcMyLable, m_szLableString);
	for ( int i = 0; i < nIndex+1; i++)
	{
		pcMyLable = strchr(pcMyLable, '\\')+1;
	}
	char *pcNextLabel =	strchr(pcMyLable,'\\');
	if (pcNextLabel !=NULL)
		*pcNextLabel = '\0';

	strcpy(m_szTabLable, pcMyLable); 

//	m_pCTabCtrl	= pCTabCtrlParent;

	this->StartTabSession(pCTabCtrlParent);	
	this->SetLable();
	this->EndTabSession();	

  //## end CTestPanelTabs::CTestPanelTabsInit%369103FD033F.body
}

template <int nIndex, int nDialogID, class CDlgTabContent>
void CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::SetLable ()
{
  //## begin CTestPanelTabs::SetLable%36910C5A006C.body preserve=yes
	TC_ITEM TabCtrlItem;

	TabCtrlItem.mask = TCIF_TEXT;
	TabCtrlItem.pszText = m_szTabLable;
	m_pCTabCtrlParent->InsertItem(m_nIndex,&TabCtrlItem);


  //## end CTestPanelTabs::SetLable%36910C5A006C.body
}

template <int nIndex, int nDialogID, class CDlgTabContent>
BOOL CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::TabSelecting (int nTabIndex)
{
  //## begin CTestPanelTabs::TabSelecting%36913C5D0200.body preserve=yes

	if ( nTabIndex != m_nIndex )
		return FALSE;
	if (m_pCDlgTabContent!=NULL)
	{
//		m_pCDlgTabContent->DestroyWindow();		//Removed by Frank Zhou 2000.3.17
//		delete m_pCDlgTabContent;				//Removed by Frank Zhou 2000.3.17
//		m_pCDlgTabContent	= NULL;				//Removed by Frank Zhou 2000.3.17
		m_pCDlgTabContent->ShowWindow(SW_HIDE);	//This Line is added by Frank ZHou 2000.0.17

	}
	return TRUE;



  //## end CTestPanelTabs::TabSelecting%36913C5D0200.body
}

template <int nIndex, int nDialogID, class CDlgTabContent>
BOOL CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::TabSelected (int nTabIndex)
{
  //## begin CTestPanelTabs::TabSelected%36913F1202CF.body preserve=yes

	if ( nTabIndex != m_nIndex )
		return FALSE;

	if (m_pCDlgTabContent==NULL)
	{//This Line is added by Frank ZHou 2000.0.17
		m_pCDlgTabContent	= new CDlgTabContent;

	ASSERT(m_pCDlgTabContent!=NULL);
	ASSERT(m_pCTabCtrlParent!=NULL);//m_pCTabCtrlParent can be set using StartTabSession(..)
	BOOL bRet = m_pCDlgTabContent->Create( (UINT)nDialogID, m_pCTabCtrlParent );

	if(!bRet)
		TRACE("Tab Content Creation Failed!");
	}//This Line is added by Frank ZHou 2000.0.17

	m_pCDlgTabContent->ShowWindow(SW_SHOW);	//This Line is added by Frank ZHou 2000.0.17

	return TRUE;



  //## end CTestPanelTabs::TabSelected%36913F1202CF.body
}

template <int nIndex, int nDialogID, class CDlgTabContent>
void CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::StartTabSession (CTabCtrl* pCTabParent)
{
  //## begin CTestPanelTabs::StartTabSession%36914DEE01C1.body preserve=yes


	m_pCTabCtrlParent	= pCTabParent;


  //## end CTestPanelTabs::StartTabSession%36914DEE01C1.body
}

template <int nIndex, int nDialogID, class CDlgTabContent>
void CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::EndTabSession ()
{
  //## begin CTestPanelTabs::EndTabSession%36914E700326.body preserve=yes

	m_pCTabCtrlParent	= NULL;
  //## end CTestPanelTabs::EndTabSession%36914E700326.body
}

template <int nIndex, int nDialogID, class CDlgTabContent>
void CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::TabChange (CTabCtrl* pCTabCtrl)
{
  //## begin CTestPanelTabs::TabChange%40D3A2F0037C.body preserve=yes
//	if ( this->m_pCTabCtrlParent !=NULL )
	{
//		int nIndex	= this->m_pCTabCtrl->GetCurSel()+1;
		int nIndex	= pCTabCtrl->GetCurSel();
//		this->StartTabSession(this->m_pCTabCtrl);
		this->StartTabSession(pCTabCtrl);

		this->TabSelected(nIndex);
		this->EndTabSession();
	}
  //## end CTestPanelTabs::TabChange%40D3A2F0037C.body
}

template <int nIndex, int nDialogID, class CDlgTabContent>
void CTestPanelTabs<nIndex,nDialogID,CDlgTabContent>::TabChanging (CTabCtrl* pCTabCtrl)
{
  //## begin CTestPanelTabs::TabChanging%40D3A4A003B9.body preserve=yes
//	if ( this->m_pCTabCtrlParent !=NULL )
	{
//		int nIndex	= this->m_pCTabCtrl->GetCurSel()+1;
		int nIndex	= pCTabCtrl->GetCurSel();

//		this->StartTabSession(this->m_pCTabCtrl);
		this->StartTabSession(pCTabCtrl);
		this->TabSelecting(nIndex);
		this->EndTabSession();
	}
  //## end CTestPanelTabs::TabChanging%40D3A4A003B9.body
}

// Additional Declarations
  //## begin CTestPanelTabs%3690FEB30111.declarations preserve=yes
  //## end CTestPanelTabs%3690FEB30111.declarations

//## begin module%40CFBD4300AE.epilog preserve=yes
//## end module%40CFBD4300AE.epilog
