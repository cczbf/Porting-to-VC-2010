//## begin module%40E266F20126.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40E266F20126.cm

//## begin module%40E266F20126.cp preserve=no
//## end module%40E266F20126.cp

//## Module: COpenGL%40E266F20126; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\COpenGL.cpp

//## begin module%40E266F20126.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%40E266F20126.additionalIncludes

//## begin module%40E266F20126.includes preserve=yes
#include <math.h>
#include <float.h>

#include "CGlobals.h"
//## end module%40E266F20126.includes

// COpenGL
#include "COpenGL.h"
//## begin module%40E266F20126.declarations preserve=no
//## end module%40E266F20126.declarations

//## begin module%40E266F20126.additionalDeclarations preserve=yes
static const double m_ZOOMSCALE = 0.008;//Moved here by ZBF, 20041020
GLubyte m_RasterFont[][13] = 
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36}, 
	{0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00}, 
	{0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18}, 
	{0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70}, 
	{0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e}, 
	{0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c}, 
	{0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30}, 
	{0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00}, 
	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03}, 
	{0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c}, 
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18}, 
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c}, 
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06}, 
	{0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60}, 
	{0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e}, 
	{0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18}, 
	{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, 
	{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc}, 
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff}, 
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, 
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e}, 
	{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06}, 
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3}, 
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0}, 
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3}, 
	{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e}, 
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, 
	{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c}, 
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, 
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
	{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3}, 
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3}, 
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff}, 
	{0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c}, 
	{0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60}, 
	{0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18}, 
	{0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70}, 
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0}, 
	{0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03}, 
	{0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e}, 
	{0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0}, 
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00}, 
	{0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00}, 
	{0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0}, 
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78}, 
	{0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00}, 
	{0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00}, 
	{0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00}, 
	{0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
	{0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f}, 
	{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, 
	{0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00} 
};




//## end module%40E266F20126.additionalDeclarations


// Class COpenGL 









































COpenGL::COpenGL ()
  //## begin COpenGL::COpenGL%40E26543034B.hasinit preserve=no
      : m_hDC(NULL), m_hWnd(NULL), m_bLightingOn(!TRUE),m_bAcceleratingOn(!TRUE),m_bMemoryBoosterOn(!TRUE),m_angle(0.0)
  //## end COpenGL::COpenGL%40E26543034B.hasinit
  //## begin COpenGL::COpenGL%40E26543034B.initialization preserve=yes
  //## end COpenGL::COpenGL%40E26543034B.initialization
{
  //## begin COpenGL::COpenGL%40E26543034B.body preserve=yes
	m_Version = "GLDemo1.0";
	m_fC			= 0.5f; // do not change this value !!!
	m_fDelta		= 0.002f * m_fC;
	m_bDrawNothing	= TRUE;
	m_hGLContext	= NULL;

	m_bLButtonDown	= FALSE;
	m_bRButtonDown	= FALSE;

	m_nFontOffset	= 0;
	m_nTimerSpeed	= 200;
	m_fDY			= -3;
	m_fAngleX		= 0;
	m_fAngleY		= 45;
	m_fAngleZ		   = 10;

	m_pDS			= NULL;
	m_fDV           = 3;
	m_nD			= 30; // for legend
	m_bDraw			= true;

	m_xTranslation = 0;
	m_yTranslation = 0;
	m_zTranslation = 0;
	m_angle = 0;
	m_dfZoom = 1;

	m_angle = 0;
	m_axis[0] = 1;
	m_axis[1] = 0;
	m_axis[2] = 0;

	m_nDispType = 0;
	
	m_LDownPos.x = 0;
	m_LDownPos.y = 0;

	m_nElapseTime = 30;

	m_ModeType = SURFACE;
	m_ModeType0 = SURFACE;

  //## end COpenGL::COpenGL%40E26543034B.body
}


COpenGL::~COpenGL ()
{
  //## begin COpenGL::~COpenGL%40E26543034A.body preserve=yes
//  bool COpenGL::CreateViewGLContext()
{
	//if(m_hDC != NULL)
	//	free(m_hDC);
	//KillFont();	
}
  //## end COpenGL::~COpenGL%40E26543034A.body
}



//## Other Operations (implementation)
bool COpenGL::CreateViewGLContext ()
{
  //## begin COpenGL::CreateViewGLContext%40E26543031F.body preserve=yes

//	bool COpenGL::CreateViewGLContext()
{
	m_hGLContext = wglCreateContext(m_hDC); // Create an OpenGL rendering context
	if(!m_hGLContext)
		return false;
	if(!wglMakeCurrent(m_hDC, m_hGLContext)) // Set the current rendering context
		return false;

	return true;
}

  //## end COpenGL::CreateViewGLContext%40E26543031F.body
}

bool COpenGL::SetWindowPixelFormat ()
{
  //## begin COpenGL::SetWindowPixelFormat%40E265430320.body preserve=yes
  
//	bool COpenGL::SetWindowPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd;
	
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Size of this pfd
	pfd.nVersion = 1;							// Version number : must be 1
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |			// Support window
				  PFD_SUPPORT_OPENGL |			// Support OpenGL
				  PFD_DOUBLEBUFFER |			// Double buffered
				  PFD_STEREO_DONTCARE;			// Support either monoscopic or stereoscopic
	pfd.iPixelType = PFD_TYPE_RGBA;				// RGBA type
	pfd.cColorBits = 32;						// Specifies the number of color bitplanes in each color buffer
	pfd.cRedBits = 8;							// Specifies the number of red bitplanes in each RGBA color buffer
	pfd.cRedShift = 16;							// Specifies the shift count for red bitplanes in each RGBA color buffer
	pfd.cGreenBits = 8;							// Specifies the number of green bitplanes in each RGBA color buffer
	pfd.cGreenShift = 8;						// Specifies the shift count for green bitplanes in each RGBA color buffer
	pfd.cBlueBits = 8;							// Specifies the number of blue bitplanes in each RGBA color buffer
	pfd.cBlueShift = 0;							// Specifies the shift count for blue bitplanes in each RGBA color buffer
	pfd.cAlphaBits = 0;							// Specifies the number of alpha bitplanes in each RGBA color buffer. Alpha bitplanes are not supported
	pfd.cAlphaShift = 0;						// Specifies the shift count for alpha bitplanes in each RGBA color buffer. Alpha bitplanes are not supported
	pfd.cAccumBits = 64;						// Specifies the total number of bitplanes in the accumulation buffer
	pfd.cAccumRedBits = 16;						// Specifies the number of red bitplanes in the accumulation buffer
	pfd.cAccumGreenBits = 16;					// Specifies the number of green bitplanes in the accumulation buffer
	pfd.cAccumBlueBits = 16;					// Specifies the number of blue bitplanes in the accumulation buffer
	pfd.cAccumAlphaBits = 0;					// Specifies the number of alpha bitplanes in the accumulation buffer
	pfd.cDepthBits = 32;						// Specifies the depth of the depth (z-axis) buffer
	pfd.cStencilBits = 8;						// Specifies the depth of the stencil buffer
	pfd.cAuxBuffers = 0;						// Specifies the number of auxiliary buffers. Auxiliary buffers are not supported
	pfd.iLayerType = PFD_MAIN_PLANE;			// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used
	pfd.bReserved = 0;							// Specifies the number of overlay and underlay planes
	pfd.dwLayerMask = 0;						// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used
	pfd.dwVisibleMask = 0;						// Specifies the transparent color or index of an underlay plane
	pfd.dwDamageMask = 0;						// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used

	int m_GLPixelIndex = ChoosePixelFormat(m_hDC, &pfd);// Attempts to match an appropriate pixel format supported by a device context to a given pixel format specification
	if(m_GLPixelIndex == 0)								// Choose default
	{
		m_GLPixelIndex = 1;
		if(DescribePixelFormat(m_hDC, m_GLPixelIndex,	// Obtains information about the pixel format identified by iPixelFormat of the device associated with hdc
			sizeof(PIXELFORMATDESCRIPTOR), &pfd)==0)
			return FALSE;
	}
	if(!SetPixelFormat(m_hDC, m_GLPixelIndex, &pfd))	//Sets the pixel format of the specified device context to the format specified by the iPixelFormat index
		return false;
	
	return true;
}

	
	
	
	
	
  //## end COpenGL::SetWindowPixelFormat%40E265430320.body
}

bool COpenGL::LoadMarkerData (const char* FileName)
{
  //## begin COpenGL::LoadMarkerData%40E265430321.body preserve=yes
//bool COpenGL::LoadMarkerData(const char* FileName)
{
	FILE* fp;
	if( ( fp = fopen(FileName, "r" ) ) == NULL)
		return	 false;

	char ch[100];
	int i = 0;
	int n;
	int flag;
	float x;
	float y;

	fgets(ch, 100, fp);
	for(; ;)
	{
		n = fscanf(fp, "%f%f%d%s\n", &x, &y, &flag, ch);
		if (x < m_pDS->m_fXmin || x > m_pDS->m_fXmax ||
			y < m_pDS->m_fYmin || y > m_pDS->m_fYmax)
		{
			MessageBox(NULL, "The values of markers are out of ranges", "Warning ...", MB_OK);
			fclose(fp);
			return false;
		}
		if(n != 4) break;
		i++;
	}

	if(i>0) m_pDS->m_Marker.SetSize(0);
	else
	{
		fclose(fp);
		return false;
	}

	m_pDS->m_Marker.SetSize(i);
	fseek( fp, 0L, SEEK_SET );
	fgets(ch, 100, fp);
	for(int j=0; j<i; j++)
	{
		fscanf(fp, "%f%f%d%s\n", &x, &y, &flag, ch);
		CMarker marker;
		m_pDS->m_Marker[j].x	 = x;
		m_pDS->m_Marker[j].y	 = y;
		m_pDS->m_Marker[j].Hight = 2 * m_fC;
		m_pDS->m_Marker[j].Flag= flag;
		m_pDS->m_Marker[j].Name= ch;
	}
	fclose(fp);

	for(j=0; j<i; j++)
	{
		m_pDS->m_Marker[j].x = (m_pDS->m_Marker[j].x - m_pDS->m_fXmin) / 
							   (m_pDS->m_fXmax - m_pDS->m_fXmin) - m_fC;
		m_pDS->m_Marker[j].y = (m_pDS->m_Marker[j].y - m_pDS->m_fYmin) / 
							   (m_pDS->m_fYmax - m_pDS->m_fYmin) - m_fC;
	}

	m_pDS->m_bMark = 1;

	return true;
}

  //## end COpenGL::LoadMarkerData%40E265430321.body
}

void COpenGL::SaveMaps ()
{
  //## begin COpenGL::SaveMaps%40E265430323.body preserve=yes
  //## end COpenGL::SaveMaps%40E265430323.body
}

void COpenGL::OpenMaps ()
{
  //## begin COpenGL::OpenMaps%40E265430325.body preserve=yes
  //## end COpenGL::OpenMaps%40E265430325.body
}

void COpenGL::Legend (RECT& rt)
{
  //## begin COpenGL::Legend%40E265430327.body preserve=yes
  //## end COpenGL::Legend%40E265430327.body
}

void COpenGL::DrawBox ()
{
  //## begin COpenGL::DrawBox%40E265430329.body preserve=yes
  //## end COpenGL::DrawBox%40E265430329.body
}

void COpenGL::ColorLevel (float level)
{
  //## begin COpenGL::ColorLevel%40E26543032A.body preserve=yes
//	void COpenGL::ColorLevel(float level)
{
	if (m_pDS->m_fCrThreshold > level)
		glColor3f(m_pDS->m_BackCr.r, m_pDS->m_BackCr.g, m_pDS->m_BackCr.b);
	else if(m_pDS->m_bAutoCr)
	{
		for( int i=0; i<m_pDS->m_nCrLevel; i++ )
		{
			if(level >= (-m_fC + i * 2 * m_fC / m_pDS->m_nCrLevel) &&
			   level <= (-m_fC + (i + 1) * 2 * m_fC / m_pDS->m_nCrLevel))
			{
				glColor3f(m_pDS->m_MinCr.r+i*(m_pDS->m_MaxCr.r-m_pDS->m_MinCr.r) / (m_pDS->m_nCrLevel - 1),
						  m_pDS->m_MinCr.g+i*(m_pDS->m_MaxCr.g-m_pDS->m_MinCr.g) / (m_pDS->m_nCrLevel - 1),
						  m_pDS->m_MinCr.b+i*(m_pDS->m_MaxCr.b-m_pDS->m_MinCr.b) / (m_pDS->m_nCrLevel - 1));
				return;
			}
		}
	}
	else
	{
		for(int i=0; i<m_pDS->m_nUserCrNum; i++)
		{
			if( level >= (-m_fC + i * 2 * m_fC / m_pDS->m_nUserCrNum) &&
				level <= (-m_fC + (i + 1) * 2 * m_fC / m_pDS->m_nUserCrNum))
			{
				glColor3f(GetRValue(m_pDS->m_UserCr.GetAt(i)) / 255.f, 
						  GetGValue(m_pDS->m_UserCr.GetAt(i)) / 255.f,
						  GetBValue(m_pDS->m_UserCr.GetAt(i)) / 255.f);
				return;
			}
		}
	}
}

  //## end COpenGL::ColorLevel%40E26543032A.body
}

void COpenGL::GDIDrawBanner ()
{
  //## begin COpenGL::GDIDrawBanner%40E26543032C.body preserve=yes
  //## end COpenGL::GDIDrawBanner%40E26543032C.body
}

void COpenGL::OpenGLDrawBanner ()
{
  //## begin COpenGL::OpenGLDrawBanner%40E26543032D.body preserve=yes
  //## end COpenGL::OpenGLDrawBanner%40E26543032D.body
}

void COpenGL::GDIDrawMap ()
{
  //## begin COpenGL::GDIDrawMap%40E26543032E.body preserve=yes
  //## end COpenGL::GDIDrawMap%40E26543032E.body
}

void COpenGL::DrawLegend ()
{
  //## begin COpenGL::DrawLegend%40E265430330.body preserve=yes
//void COpenGL::DrawLegend()
{
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.0, 0.0, m_pDS->m_zTrans);

	RECT rt;
	::GetClientRect(m_hWnd, &rt);
	gluOrtho2D ((float)(rt.left + (rt.right - rt.left) / 20.), 
				(float)(rt.right + (rt.right - rt.left) / 20.),
				(float)rt.top, (float)rt.bottom);

	Legend(rt);
}
  //## end COpenGL::DrawLegend%40E265430330.body
}

void COpenGL::DrawAxis ()
{
  //## begin COpenGL::DrawAxis%40E265430331.body preserve=yes
//void COpenGL::DrawAxis()
{

	if ( m_pDS !=NULL )
		glColor3f(m_pDS->m_BorderCr.r, m_pDS->m_BorderCr.g, m_pDS->m_BorderCr.b);
	else
		glColor3f(0, 0, 0);
	
	if ( m_pDS !=NULL )
	
	if(m_pDS->m_MapType != FLAT)
		Draw3DLine(-m_fC, -m_fC, m_fC, -m_fC, -m_fC, 6.8f * m_fC / 5);		// Z axis

	Draw3DLine(-m_fC, m_fC, -m_fC, -m_fC, 6.5f * m_fC / 5, -m_fC );			// Y axis 
	Draw3DLine(m_fC, -m_fC, -m_fC, 6.2f * m_fC / 5, -m_fC, -m_fC );			// X axis
	
	if ( m_pDS !=NULL )
	if(m_pDS->m_MapType != FLAT)
	{
		glRasterPos3f(-m_fC, -m_fC, 7 * m_fC / 5);
		PrintString("Z");
	}
	glRasterPos3f(-m_fC, 6.6f * m_fC / 5, -m_fC);
	PrintString("Y");
	glRasterPos3f(6.3f * m_fC / 5, -m_fC, -m_fC);
	PrintString("X");
}
  //## end COpenGL::DrawAxis%40E265430331.body
}

void COpenGL::DrawBorder ()
{
  //## begin COpenGL::DrawBorder%40E265430332.body preserve=yes
//void COpenGL::DrawBorder()
{
	glColor3f(m_pDS->m_BorderCr.r, m_pDS->m_BorderCr.g, m_pDS->m_BorderCr.b);
	DrawBox();
	glBegin(GL_LINE_STRIP);
		glVertex3f(-m_fC, -m_fC, -m_fC);
		glVertex3f(-m_fC, m_fC,  -m_fC);
		glVertex3f(m_fC, m_fC,   -m_fC);
		glVertex3f(m_fC, -m_fC,  -m_fC);
		glVertex3f(-m_fC, -m_fC, -m_fC);
	glEnd();
}
  //## end COpenGL::DrawBorder%40E265430332.body
}

void COpenGL::OpenGLRendering ()
{
  //## begin COpenGL::OpenGLRendering%40E265430333.body preserve=yes
//void COpenGL::OpenGLRendering()
{
	if (!m_bDraw) 
		return;

	m_bDraw = false;

	// begin OpenGL call
	wglMakeCurrent(m_hDC, m_hGLContext);

	//if (m_pDS == NULL)
	//	glClearColor(140.0f/255, 195.0f/255, 245.0f/155, 1.0f);
	//else
	//	glClearColor(m_pDS->m_BackCr.r, m_pDS->m_BackCr.g, m_pDS->m_BackCr.b, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	if (m_pDS == NULL)
		OpenGLDrawBanner();
	else
		OpenGLDrawMap();

//#define TESTINGGLOBE
#ifdef TESTINGGLOBE
	///////Test, test, test//////////////{{
//this->DrawAxis ();

GLUquadricObj* pQ = gluNewQuadric(  );
gluSphere(
  pQ,//GLUquadricObj *qobj,
  0.5,//GLdouble radius,
  32,//GLint slices,
  32 //GLint stacks
);
gluDeleteQuadric(
  pQ//GLUquadricObj *state
);
#endif

	/////////////////////////////////////}}
		
	glPopMatrix();


	glFinish();
	SwapBuffers(wglGetCurrentDC());

	// begin GDI call
	if (m_pDS == NULL)
		GDIDrawBanner();
	else
		GDIDrawMap();

	m_bDraw = true;
}
  //## end COpenGL::OpenGLRendering%40E265430333.body
}

void COpenGL::PrintString (const char* str)
{
  //## begin COpenGL::PrintString%40E265430334.body preserve=yes
//void COpenGL::PrintString(const char* str)
{
    glPushAttrib(GL_LIST_BIT);
    glListBase(m_nFontOffset);
    glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte*)str);
    glPopAttrib();
}

	
  //## end COpenGL::PrintString%40E265430334.body
}

void COpenGL::RasterFont ()
{
  //## begin COpenGL::RasterFont%40E265430336.body preserve=yes
//	void COpenGL::RasterFont()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    m_nFontOffset = glGenLists(128);
    for (GLuint i = 32; i < 127; i++)
	{
		glNewList(i + m_nFontOffset, GL_COMPILE);
		glBitmap(8, 13, 0.0f, 2.0f, 10.0f, 0.0f, m_RasterFont[i-32]);
		glEndList();
    }
}

  //## end COpenGL::RasterFont%40E265430336.body
}

void COpenGL::LButtonUp (POINT point)
{
  //## begin COpenGL::LButtonUp%40E265430337.body preserve=yes
//	void LButtonUp(POINT point)
	if(g_CGlobals.m_pDlgTab00->m_bAccelerated)
	{
		g_CGlobals.m_pCOpenGL3DEx->m_bAcceleratingOn=FALSE;
	    switch (m_ModeType0)
		{
    	case SURFACE :
            SetFaceMode ();
	        break;
     	case GRID :
            SetWireFrameMode ();
		    break;
	    case PTS :
            SetVertexMode();
	     	break;
	    default:
		  break;
		}
	}
	
	{ m_bLButtonDown = false; }

  //## end COpenGL::LButtonUp%40E265430337.body
}

void COpenGL::LButtonDown (POINT point)
{
  //## begin COpenGL::LButtonDown%40E265430339.body preserve=yes

//	void LButtonDown(POINT point)	
	{
	m_bLButtonDown = TRUE; 
	m_LDownPos = point;
	if(g_CGlobals.m_pDlgTab00->m_bAccelerated)
	{
    	m_ModeType0=g_CGlobals.m_pDlgTab01->m_ModeType;
		g_CGlobals.m_pCOpenGL3DEx->m_bAcceleratingOn=TRUE;
        SetVertexMode();
	}
	//add by xdh:
	m_trackBall.StartMotion(point);

	}
  //## end COpenGL::LButtonDown%40E265430339.body
}

BOOL COpenGL::MouseMove (UINT nFlags, POINT point)
{
  //## begin COpenGL::MouseMove%40E26543033B.body preserve=yes
//void COpenGL::MouseMove(UINT nFlags, POINT point) 

	if (!m_pDS) 
		return FALSE;
	
	float fM = 0;
	if (m_fWinCx > m_fWinCy)
		fM = m_fWinCx;
	else
		fM = m_fWinCy;

	float pixel_diff = 0;

	//for rotation
	if (nFlags == MK_LBUTTON)
	{
		m_trackBall.Track(point, m_axis, m_angle);	

		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)m_p3D->objectXform);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
        
		//if (m_p3D->m_MapType == MESH2D )
		//	glLoadIdentity();
		//else

		glRotatef(m_angle, m_axis[0], m_axis[1], m_axis[2]);
		
		glMultMatrixf((GLfloat*)m_p3D->objectXform);
	
		return TRUE;
	}

	//for moving
	if (nFlags == MK_MBUTTON)
	{		
		//glTranslated(m_p3D->m_xTrans, m_p3D->m_yTrans, m_p3D->m_zTrans);
		m_xTranslation +=  (point.x-m_mMouseDownPoint.x) / fM;
		m_yTranslation += -(point.y-m_mMouseDownPoint.y) / fM;

		float cx = m_fWinCx;
		float cy = m_fWinCy;
		glViewport(0, 0, (int)cx, (int)cy);

		glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        		
    	gluPerspective(45, (cy == 0) ? 1.0 : cx/(double)cy, 0.001, 15.0);
		glTranslatef(m_xTranslation, 0, 0);
		glTranslatef(0, m_yTranslation, 0);
		glTranslatef(0, 0, -3);
		
		m_mMouseDownPoint = point;

		glMatrixMode(GL_MODELVIEW);

		return TRUE;

	}
    //for scaling
	if (nFlags == MK_RBUTTON)
	{
		glMatrixMode(GL_MODELVIEW);
		pixel_diff = (float)(point.x - m_rMouseDownPoint.x); 
		m_dfZoom = 1.0 + pixel_diff * m_ZOOMSCALE;

		glScaled(m_dfZoom, m_dfZoom, m_dfZoom);

		m_rMouseDownPoint = point;

		return TRUE;
	}

	return FALSE;
	
  //## end COpenGL::MouseMove%40E26543033B.body
}

void COpenGL::KillOpenGLTimer (int n)
{
  //## begin COpenGL::KillOpenGLTimer%40E26543033E.body preserve=yes
//  void COpenGL::KillOpenGLTimer(int n)
{
	if(n >= RX && n <= RXYZ)
		::KillTimer(m_hWnd, n);	
}
  //## end COpenGL::KillOpenGLTimer%40E26543033E.body
}

void COpenGL::SetOpenGLTimer (int n)
{
  //## begin COpenGL::SetOpenGLTimer%40E265430340.body preserve=yes
// void COpenGL::SetOpenGLTimer(int n)
{
	if(n >= RX && n <= RXYZ)
	{
		//if (!::SetTimer(m_hWnd, n, m_nTimerSpeed, NULL))
		if (!::SetTimer(m_hWnd, n, 40, NULL))
		{
			::MessageBox(m_hWnd, "Not enough timers available for this window.",
					"OpenGL ...", MB_ICONEXCLAMATION | MB_OK);
			return;
		}
	}
}
  //## end COpenGL::SetOpenGLTimer%40E265430340.body
}

void COpenGL::OpenGLTimer (CRotateType type)
{
  //## begin COpenGL::OpenGLTimer%40E265430342.body preserve=yes

	if (!m_bDraw) 
		return;

	float fDx = 0;
	float fDy = 0;
	float fDz = 0;

	m_fDV = (float)m_nTimerSpeed / (float)30.0 ;

	switch(type)
	{
		case RX:
			m_pDS->m_xRotat += m_fDV;			
			
			fDx = m_fDV;
			break;
		case RY:
			m_pDS->m_yRotat += m_fDV;
			
			fDy = m_fDV;
			break;
		case RZ:
			m_pDS->m_zRotat += m_fDV;
			
			fDz = m_fDV;
			break;
		case RXY:
			m_pDS->m_yRotat += m_fDV;
			m_pDS->m_xRotat += (m_fDV - 1);
		
			fDx = m_fDV - 1;
			fDy = m_fDV;
			break;
		case RXZ:
			m_pDS->m_zRotat += m_fDV;
			m_pDS->m_xRotat += (m_fDV - 1);
			
			fDz = m_fDV;
			fDx = m_fDV - 1;
			
			break;
		case RYZ:
			m_pDS->m_zRotat += m_fDV;
			m_pDS->m_yRotat += (m_fDV - 1);
			
			fDz = m_fDV;
			fDy = m_fDV - 1;
			
			break;
		case RXYZ:
			m_pDS->m_zRotat += m_fDV;
			m_pDS->m_yRotat += (m_fDV - 1);
			m_pDS->m_xRotat += (m_fDV - 2);
			
			fDz = m_fDV;
			fDy = m_fDV - 1;
			fDx = m_fDV - 2;
			
			break;
	}	
	
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)m_p3D->objectXform);
	
	glLoadIdentity();
	glRotatef(fDx, 1, 0, 0);
	glRotatef(fDy, 0, 1, 0);
	glRotatef(fDz, 0, 0, 1);

	glMultMatrixf((GLfloat*)m_p3D->objectXform);	

	::InvalidateRect(m_hWnd, NULL, FALSE);

  //## end COpenGL::OpenGLTimer%40E265430342.body
}

void COpenGL::OpenGLSize (int cx, int cy)
{
  //## begin COpenGL::OpenGLSize%40E265430344.body preserve=yes
//	void COpenGL::OpenGLSize(int cx, int cy)
{
	m_trackBall.SetSize(cx,cy);

	m_fWinCx = (float)(cx);
	m_fWinCy = (float)(cy);

	// Set OpenGL perspective, viewport and mode
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (cy == 0) ? 1.0 : cx / (double)cy, 0.001, 15.0);

	glTranslatef(m_xTranslation, 0, 0);
	glTranslatef(0, m_yTranslation, 0);
	glTranslatef(0, 0, -3);
    
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

  //## end COpenGL::OpenGLSize%40E265430344.body
}

void COpenGL::DestroyOpenGL ()
{
  //## begin COpenGL::DestroyOpenGL%40E265430347.body preserve=yes
//void COpenGL::DestroyOpenGL()
{
	if(wglGetCurrentContext())
		wglMakeCurrent(NULL, NULL);
	
	if(m_hGLContext)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

	::ReleaseDC(m_hWnd, m_hDC);
}
  //## end COpenGL::DestroyOpenGL%40E265430347.body
}

bool COpenGL::InitOpenGL (HWND hWnd)
{
  //## begin COpenGL::InitOpenGL%40E265430348.body preserve=yes
//	bool COpenGL::InitOpenGL(HWND hWnd)
{
	if (!::IsWindow(hWnd))
		return false;

	m_hWnd = hWnd;
	m_hDC = ::GetDC(m_hWnd);
	
	if(!SetWindowPixelFormat())
		return false;
	if(!CreateViewGLContext())
		return false;
	
    //glClearColor(140.0f/255, 195.0f/255, 245.0f/155, 1.0f);
    //glClearColor(0.0, 0.25, 0.5, 0.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	RasterFont();
	//BulidFont();
	return true;
}

  //## end COpenGL::InitOpenGL%40E265430348.body
}

void COpenGL::OpenGLDrawMap ()
{
  //## begin COpenGL::OpenGLDrawMap%40E9F97303DC.body preserve=yes
  //## end COpenGL::OpenGLDrawMap%40E9F97303DC.body
}

void COpenGL::RButtonUp (POINT point)
{
  //## begin COpenGL::RButtonUp%4175DD45024E.body preserve=yes
	if(g_CGlobals.m_pDlgTab00->m_bAccelerated)
	{
		g_CGlobals.m_pCOpenGL3DEx->m_bAcceleratingOn=FALSE;
	    switch (m_ModeType0)
		{
    	case SURFACE :
            SetFaceMode ();
	        break;
     	case GRID :
            SetWireFrameMode ();
		    break;
	    case PTS :
            SetVertexMode();
	     	break;
	    default:
		  break;
		}
	}
	m_rMouseDownPoint = CPoint(0,0);

  //## end COpenGL::RButtonUp%4175DD45024E.body
}

void COpenGL::SetFaceMode ()
{
  //## begin COpenGL::SetFaceMode%4175DDD400EC.body preserve=yes
	m_ModeType = SURFACE; 
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	m_nDispType = 0;

  //## end COpenGL::SetFaceMode%4175DDD400EC.body
}

void COpenGL::SetWireFrameMode ()
{
  //## begin COpenGL::SetWireFrameMode%4175DE29035B.body preserve=yes
//	void COpenGL::SetWireFrameMode()
{
	m_ModeType = GRID;
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	m_nDispType = 1; 
}
  //## end COpenGL::SetWireFrameMode%4175DE29035B.body
}

void COpenGL::SetVertexMode ()
{
  //## begin COpenGL::SetVertexMode%4175DEA500C4.body preserve=yes
//void COpenGL::SetVertexMode()
{
	m_ModeType = PTS;
	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	m_nDispType = 2; 
}

  //## end COpenGL::SetVertexMode%4175DEA500C4.body
}

void COpenGL::SetSpecular (COLORREF specularColor)
{
  //## begin COpenGL::SetSpecular%4175DF360343.body preserve=yes
//	void COpenGL::SetSpecular(COLORREF specularColor)
{

		float r,g,b;
		r = (float)GetRValue(specularColor) / 255.0f;
		g = (float)GetGValue(specularColor) / 255.0f;
		b = (float)GetBValue(specularColor) / 255.0f;
		float	specularProperties[]  = {r,g,b,1.0f};
		
		glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);

}
  //## end COpenGL::SetSpecular%4175DF360343.body
}

void COpenGL::SetDiffuse (COLORREF diffuseColor)
{
  //## begin COpenGL::SetDiffuse%4175DF9601D9.body preserve=yes
//	void COpenGL::SetDiffuse(COLORREF diffuseColor)
{

		float r,g,b;
		
		r = (float)GetRValue(diffuseColor) / 255.0f;
		g = (float)GetGValue(diffuseColor) / 255.0f;
		b = (float)GetBValue(diffuseColor) / 255.0f;
		float	diffuseProperties[]  = {r,g,b,1.0f};

		glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
   	    //glEnable(GL_LIGHTING);
	   // glEnable(GL_LIGHT0);


}
  //## end COpenGL::SetDiffuse%4175DF9601D9.body
}

void COpenGL::SetAmbient (COLORREF ambientColor)
{
  //## begin COpenGL::SetAmbient%4175DFE40235.body preserve=yes
	
//void COpenGL::SetAmbient(COLORREF ambientColor)
{
		float r,g,b;
		r = (float)GetRValue(ambientColor) / 255.0f;
		g = (float)GetGValue(ambientColor) / 255.0f;
		b = (float)GetBValue(ambientColor) / 255.0f;
		float	ambientProperties[]  = {r,g,b,1.0f};
		
		glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
}
  //## end COpenGL::SetAmbient%4175DFE40235.body
}

void COpenGL::SetBckGroundColor (COLORREF bckGrdColor)
{
  //## begin COpenGL::SetBckGroundColor%4175E0250379.body preserve=yes
//void COpenGL::SetBckGroundColor(COLORREF bckGrdColor)
{
		float r,g,b;
		r = (float)GetRValue(bckGrdColor) / 255.0f;
		g = (float)GetGValue(bckGrdColor) / 255.0f;
		b = (float)GetBValue(bckGrdColor) / 255.0f;
		glClearColor(r,g,b,1.0f);

}


  //## end COpenGL::SetBckGroundColor%4175E0250379.body
}

void COpenGL::SetLight (bool bIsOn)
{
//## begin COpenGL::SetLight%4175E06703A6.body preserve=yes
//	void COpenGL::SetLight(bool bIsOn)
{
	if (bIsOn)
	{		

	   GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	   
	   GLfloat  light_position[] = { 0.0, 0.0, 5.0, 1.0 };
	   GLfloat  lm_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	   GLfloat	diffuseProperties[]  = {1.0f, 1.0f, 1.0f, 1.0f};
       GLfloat	specularProperties[] = {0.5f, 0.5f, 0.5f, 1.0f};

	   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	   glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	  
	   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	   glLightfv(GL_LIGHT0, GL_AMBIENT, lm_ambient);
	   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
       glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
    
	   glEnable(GL_LIGHTING);
	   glEnable(GL_LIGHT0);
	   glEnable(GL_DEPTH_TEST);

	   glShadeModel (GL_SMOOTH);

	   //glClearColor(0.0, 0.0, 0.0, 0.0);
	   glClearAccum(0.0, 0.0, 0.0, 0.0);
	}
	else
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);		
	}

}

  //## end COpenGL::SetLight%4175E06703A6.body
}

void COpenGL::MButtonDown (POINT point)
{
  //## begin COpenGL::MButtonDown%4175E095037A.body preserve=yes
//	void COpenGL::MButtonDown(POINT point)
	if(g_CGlobals.m_pDlgTab00->m_bAccelerated)
	{
    	m_ModeType0=g_CGlobals.m_pDlgTab01->m_ModeType;
	    g_CGlobals.m_pCOpenGL3DEx->m_bAcceleratingOn=TRUE;
        SetVertexMode();
	}
	{
	    m_mMouseDownPoint = point;
	}

  //## end COpenGL::MButtonDown%4175E095037A.body
}

void COpenGL::MButtonUp (POINT point)
{
  //## begin COpenGL::MButtonUp%4175E1380266.body preserve=yes
//	void COpenGL::MButtonUp(POINT point)
	if(g_CGlobals.m_pDlgTab00->m_bAccelerated)
	{
		g_CGlobals.m_pCOpenGL3DEx->m_bAcceleratingOn=FALSE;
	    switch (m_ModeType0)
		{
    	case SURFACE :
            SetFaceMode ();
	        break;
     	case GRID :
            SetWireFrameMode ();
		    break;
	    case PTS :
            SetVertexMode();
	     	break;
	    default:
		  break;
		}
	}
	{
	    m_mMouseDownPoint = CPoint(0,0);
	// release mouse capture	
	}



 
  //## end COpenGL::MButtonUp%4175E1380266.body
}

void COpenGL::RButtonDown (POINT point)
{
  //## begin COpenGL::RButtonDown%4175E16902C1.body preserve=yes
//	void COpenGL::RButtonDown(POINT point)
	if(g_CGlobals.m_pDlgTab00->m_bAccelerated)
	{
    	m_ModeType0=g_CGlobals.m_pDlgTab01->m_ModeType;
		g_CGlobals.m_pCOpenGL3DEx->m_bAcceleratingOn=TRUE;
        SetVertexMode();
	}
	{
	    m_rMouseDownPoint = point;
	}

  //## end COpenGL::RButtonDown%4175E16902C1.body
}

void COpenGL::Reset ()
{
  //## begin COpenGL::Reset%4178D229031C.body preserve=yes
//void COpenGL::Reset()
{

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
	if (m_p3D != NULL)
	{
		memset(m_p3D->objectXform, 0, 16*sizeof(float));
		m_p3D->m_xScale = 1;
		m_p3D->m_yScale = 1;
		m_p3D->m_zScale = 1;

		KillOpenGLTimer(m_pDS->m_Rotate);	
	}

	m_xTranslation = 0;
	m_yTranslation = 0;

	int cx =(int) m_fWinCx;
	int cy =(int) m_fWinCy;

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   	gluPerspective(45, (cy == 0) ? 1.0 : cx/(double)cy, 0.001, 15.0);
	glTranslatef(0, 0, -3);

	glMatrixMode(GL_MODELVIEW);

	m_dfZoom = 1;

	//
	//m_trackBall.StartMotion(m_LDownPos);
	m_trackBall.StopMotion(0, 0);
	m_angle = 0;
	m_axis[0] = 1;
	m_axis[1] = 0;
	m_axis[2] = 0;

}

  //## end COpenGL::Reset%4178D229031C.body
}

// Additional Declarations
  //## begin COpenGL%40E2654302E9.declarations preserve=yes
  //## end COpenGL%40E2654302E9.declarations

//## begin module%40E266F20126.epilog preserve=yes
void COpenGL::DrawColorBar()
{

	//glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
	//gluOrtho2D(-1, 1, -1, 1);
    
	glLoadIdentity();
	
	glPushMatrix();	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	//glNormal3f(0,0,1);

	int index = 0;

	float sum = 120;
	float fInterval = (float)(1.6) / sum;		

	for (int h=0; h<240; h+=2)
	{			
		//glColor3f(1, 0, 0);
		float r = 0;
		float g = 0;
		float b = 0;
		HSV_To_RGB(r, g, b, (float)(h), (float)(1.0), (float)(1.0));
		glColor3f(r, g, b);
		
		glBegin(GL_POLYGON);
		glVertex3f((float)(-1.2), (float)(0.9) - index*fInterval,  (float)(-0.5));
		glVertex3f((float)(-1.1), (float)(0.9) - index*fInterval,  (float)(-0.5));
		glVertex3f((float)(-1.1), (float)(0.9) - (index+1)*fInterval, (float)-0.5);
		glVertex3f((float)(-1.2), (float)(0.9) - (index+1)*fInterval, (float)-0.5);
		glEnd();
		
		index++;
	}
	glPopMatrix();
	
	
	switch(m_nDispType )
	{
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
		break;
	default:
		break;
	}
	
}

HGLRC COpenGL::GetContext() const
{
	return m_hGLContext;
}


//## end module%40E266F20126.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin COpenGL::cross_product%40EF1F820281.body preserve=no
	r[1-1]=p[2-1]*q[3-1]-p[3-1]*q[2-1];
	r[2-1]=p[3-1]*q[1-1]-p[1-1]*q[3-1];
	r[3-1]=p[1-1]*q[2-1]-p[2-1]*q[1-1];

//## end COpenGL::cross_product%40EF1F820281.body

//## begin COpenGL::normalize%40EF292C031E.body preserve=no

    double u;
    register int i;
    u=sqrt(pp(v,v,n));
    
	if( u != 0.0)
//	if( u > 0.0000001/*FLT_MIN*100.0*/)
     {
       for(i=0; i<n; i=i+1)
		   v[i]=v[i]/u;
       return(1);
     }
    else 
		return(0);
   
//## end COpenGL::normalize%40EF292C031E.body

//## begin COpenGL::pp1%40EF2A7D008E.body preserve=no

  double c;
  register int i;
  c=0.0;
  for(i=1;i<=n;i=i+1) c=c+a[i-1]*b[i-1];
  return(c);

//## end COpenGL::pp1%40EF2A7D008E.body

#endif
