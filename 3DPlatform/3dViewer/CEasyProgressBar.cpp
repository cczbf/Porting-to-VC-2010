//## begin module%41859D7F030D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%41859D7F030D.cm

//## begin module%41859D7F030D.cp preserve=no
//## end module%41859D7F030D.cp

//## Module: CEasyProgressBar%41859D7F030D; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CEasyProgressBar.cpp

//## begin module%41859D7F030D.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%41859D7F030D.additionalIncludes

//## begin module%41859D7F030D.includes preserve=yes
//## end module%41859D7F030D.includes

// CEasyProgressBar
#include "CEasyProgressBar.h"
//## begin module%41859D7F030D.declarations preserve=no
//## end module%41859D7F030D.declarations

//## begin module%41859D7F030D.additionalDeclarations preserve=yes
//## end module%41859D7F030D.additionalDeclarations


// Class CEasyProgressBar 




CEasyProgressBar::CEasyProgressBar (char* strTitle, int nScale)
  //## begin CEasyProgressBar::CEasyProgressBar%41859F730185.hasinit preserve=no
      : step(0.0), prog(0.0)
  //## end CEasyProgressBar::CEasyProgressBar%41859F730185.hasinit
  //## begin CEasyProgressBar::CEasyProgressBar%41859F730185.initialization preserve=yes
  //## end CEasyProgressBar::CEasyProgressBar%41859F730185.initialization
{
  //## begin CEasyProgressBar::CEasyProgressBar%41859F730185.body preserve=yes

	this->m_pCProgressBar = new CProgressBar(_T(strTitle), 100, 100, TRUE);
    this->m_pCProgressBar->SetText(strTitle);

	this->prog = 0.0;
	this->step = (float)100.0/(float)(nScale);

  //## end CEasyProgressBar::CEasyProgressBar%41859F730185.body
}


CEasyProgressBar::~CEasyProgressBar ()
{
  //## begin CEasyProgressBar::~CEasyProgressBar%4185B118027B.body preserve=yes
	delete this->m_pCProgressBar;
	this->m_pCProgressBar = NULL;
  //## end CEasyProgressBar::~CEasyProgressBar%4185B118027B.body
}


// Additional Declarations
  //## begin CEasyProgressBar%41859D0302DD.declarations preserve=yes
  //## end CEasyProgressBar%41859D0302DD.declarations

//## begin module%41859D7F030D.epilog preserve=yes
//## end module%41859D7F030D.epilog
