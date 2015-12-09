//## begin module%40E4BB9E018E.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40E4BB9E018E.cm

//## begin module%40E4BB9E018E.cp preserve=no
//## end module%40E4BB9E018E.cp

//## Module: CTool3D%40E4BB9E018E; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CTool3D.cpp

//## begin module%40E4BB9E018E.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%40E4BB9E018E.additionalIncludes

//## begin module%40E4BB9E018E.includes preserve=yes
//## end module%40E4BB9E018E.includes

// CTool3D
#include "CTool3D.h"
//## begin module%40E4BB9E018E.declarations preserve=no
//## end module%40E4BB9E018E.declarations

//## begin module%40E4BB9E018E.additionalDeclarations preserve=yes
//## end module%40E4BB9E018E.additionalDeclarations


// Class CTool3D 


CTool3D::CTool3D ()
  //## begin CTool3D::CTool3D%40E4CE3E02F3.hasinit preserve=no
  //## end CTool3D::CTool3D%40E4CE3E02F3.hasinit
  //## begin CTool3D::CTool3D%40E4CE3E02F3.initialization preserve=yes
  //## end CTool3D::CTool3D%40E4CE3E02F3.initialization
{
  //## begin CTool3D::CTool3D%40E4CE3E02F3.body preserve=yes
	this->set_m_nToolID(3);
	set_m_pCBmpViewerData(NULL);
//	CToolBase *pTBS	= (CToolBase*)this;
//	m_pCursorBase	= new CCursorZoomOut(pTBS);

  //## end CTool3D::CTool3D%40E4CE3E02F3.body
}



//## Other Operations (implementation)
BOOL CTool3D::OnLButtonUp (UINT nFlags, CPoint point)
{
  //## begin CTool3D::OnLButtonUp%40E4BB41034D.body preserve=yes
	this->m_pCOpenGL->LButtonUp(point);
		return 1;
//	this->m_pCOpenGL->MButtonUp(point);
//	return 1;
//	this->m_pCOpenGL->RButtonUp(point);
//	return 1;
  //## end CTool3D::OnLButtonUp%40E4BB41034D.body
}

BOOL CTool3D::OnLButtonDown (UINT nFlags, CPoint point)
{
  //## begin CTool3D::OnLButtonDown%40E4BB49038A.body preserve=yes
	this->m_pCOpenGL->LButtonDown(point);
		return 0;
	this->m_pCOpenGL->MButtonDown(point);
		return 1;
	this->m_pCOpenGL->RButtonDown(point);
		return 1;
  //## end CTool3D::OnLButtonDown%40E4BB49038A.body
}

BOOL CTool3D::OnMouseMove (UINT nFlags, CPoint point)
{
  //## begin CTool3D::OnMouseMove%40E4BB50024A.body preserve=yes
	return this->m_pCOpenGL->MouseMove(nFlags, point);
  //## end CTool3D::OnMouseMove%40E4BB50024A.body
}

BOOL CTool3D::OnLBDblClick (UINT nFlags, CPoint point)
{
  //## begin CTool3D::OnLBDblClick%40E4CEA00177.body preserve=yes
	return 1;
  //## end CTool3D::OnLBDblClick%40E4CEA00177.body
}

void CTool3D::set_m_pCOpenGL (COpenGL* pCOpenGL)
{
  //## begin CTool3D::set_m_pCOpenGL%40E4CBFF03A0.body preserve=yes
	m_pCOpenGL = pCOpenGL;
  //## end CTool3D::set_m_pCOpenGL%40E4CBFF03A0.body
}

BOOL CTool3D::OnMButtonUp (UINT nFlags, CPoint point)
{
  //## begin CTool3D::OnMButtonUp%4178DB320123.body preserve=yes
	this->m_pCOpenGL->MButtonUp(point);
	return 1;
  //## end CTool3D::OnMButtonUp%4178DB320123.body
}

BOOL CTool3D::OnMButtonDown (UINT nFlags, CPoint point)
{
  //## begin CTool3D::OnMButtonDown%4178DBA20020.body preserve=yes
	this->m_pCOpenGL->MButtonDown(point);
	return 1;

  //## end CTool3D::OnMButtonDown%4178DBA20020.body
}

BOOL CTool3D::OnRButtonUp (UINT nFlags, CPoint point)
{
  //## begin CTool3D::OnRButtonUp%4178DF230246.body preserve=yes
	this->m_pCOpenGL->RButtonUp(point);
	return 1;
  //## end CTool3D::OnRButtonUp%4178DF230246.body
}

BOOL CTool3D::OnRButtonDown (UINT nFlags, CPoint point)
{
  //## begin CTool3D::OnRButtonDown%4178DF52021C.body preserve=yes
	this->m_pCOpenGL->RButtonDown(point);
	return 1;
  //## end CTool3D::OnRButtonDown%4178DF52021C.body
}

// Additional Declarations
  //## begin CTool3D%40E4B5D1016B.declarations preserve=yes
  //## end CTool3D%40E4B5D1016B.declarations

//## begin module%40E4BB9E018E.epilog preserve=yes
//## end module%40E4BB9E018E.epilog
