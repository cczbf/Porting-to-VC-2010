//## begin module%3693F3C400EF.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3693F3C400EF.cm

//## begin module%3693F3C400EF.cp preserve=no
//## end module%3693F3C400EF.cp

//## Module: CDIBOrg%3693F3C400EF; Pseudo Package body
//## Subsystem: bmpviewer::Core%369F6C81018D
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CDIBOrg.cpp

//## begin module%3693F3C400EF.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%3693F3C400EF.additionalIncludes

//## begin module%3693F3C400EF.includes preserve=yes
//## end module%3693F3C400EF.includes

// CDIBOrg
#include "CDIBOrg.h"
//## begin module%3693F3C400EF.additionalDeclarations preserve=yes
//## end module%3693F3C400EF.additionalDeclarations


// Class CDIBOrg 



CDIBOrg::CDIBOrg()
  //## begin CDIBOrg::CDIBOrg%.hasinit preserve=no
      : m_hDib(NULL)
  //## end CDIBOrg::CDIBOrg%.hasinit
  //## begin CDIBOrg::CDIBOrg%.initialization preserve=yes
  //## end CDIBOrg::CDIBOrg%.initialization
{
  //## begin CDIBOrg::CDIBOrg%.body preserve=yes
  //## end CDIBOrg::CDIBOrg%.body
}


CDIBOrg::~CDIBOrg()
{
  //## begin CDIBOrg::~CDIBOrg%.body preserve=yes
	if ( m_hDib != NULL )
	{
		GlobalFree(m_hDib);
	}
  //## end CDIBOrg::~CDIBOrg%.body
}



//## Other Operations (implementation)
void CDIBOrg::FreeDib ()
{
  //## begin CDIBOrg::FreeDib%916955276.body preserve=yes
	if ( m_hDib != NULL )
	{
		GlobalFree(m_hDib);
		m_hDib	= NULL;
	}
  //## end CDIBOrg::FreeDib%916955276.body
}

// Additional Declarations
  //## begin CDIBOrg%3693F3C400EF.declarations preserve=yes
  //## end CDIBOrg%3693F3C400EF.declarations

//## begin module%3693F3C400EF.epilog preserve=yes
//## end module%3693F3C400EF.epilog
