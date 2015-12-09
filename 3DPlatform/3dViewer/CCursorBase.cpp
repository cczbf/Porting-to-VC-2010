//## begin module%36510D3F01F4.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36510D3F01F4.cm

//## begin module%36510D3F01F4.cp preserve=no
//## end module%36510D3F01F4.cp

//## Module: CCursorBase%36510D3F01F4; Pseudo Package body
//## Subsystem: bmpviewer::Tools%369E44800341
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CCursorBase.cpp

//## begin module%36510D3F01F4.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%36510D3F01F4.additionalIncludes

//## begin module%36510D3F01F4.includes preserve=yes
//## end module%36510D3F01F4.includes

// CCursorBase
#include "CCursorBase.h"
//## begin module%36510D3F01F4.additionalDeclarations preserve=yes
//## end module%36510D3F01F4.additionalDeclarations


// Class CCursorBase 



CCursorBase::CCursorBase (CToolBase* pCToolBase)
  //## begin CCursorBase::CCursorBase%911272806.hasinit preserve=no
      : m_pCToolBase(NULL)
  //## end CCursorBase::CCursorBase%911272806.hasinit
  //## begin CCursorBase::CCursorBase%911272806.initialization preserve=yes
  //## end CCursorBase::CCursorBase%911272806.initialization
{
  //## begin CCursorBase::CCursorBase%911272806.body preserve=yes
	this->m_pCToolBase	= pCToolBase;

  //## end CCursorBase::CCursorBase%911272806.body
}

CCursorBase::CCursorBase ()
  //## begin CCursorBase::CCursorBase%911272809.hasinit preserve=no
      : m_pCToolBase(NULL)
  //## end CCursorBase::CCursorBase%911272809.hasinit
  //## begin CCursorBase::CCursorBase%911272809.initialization preserve=yes
  //## end CCursorBase::CCursorBase%911272809.initialization
{
  //## begin CCursorBase::CCursorBase%911272809.body preserve=yes
  //## end CCursorBase::CCursorBase%911272809.body
}


// Additional Declarations
  //## begin CCursorBase%36510D3F01F4.declarations preserve=yes
  //## end CCursorBase%36510D3F01F4.declarations

//## begin module%36510D3F01F4.epilog preserve=yes
//## end module%36510D3F01F4.epilog
