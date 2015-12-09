//## begin module%40E266EE029D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40E266EE029D.cm

//## begin module%40E266EE029D.cp preserve=no
//## end module%40E266EE029D.cp

//## Module: COpenGL%40E266EE029D; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\COpenGL.h

#ifndef COpenGL_h
#define COpenGL_h 1

//## begin module%40E266EE029D.additionalIncludes preserve=no
//## end module%40E266EE029D.additionalIncludes

//## begin module%40E266EE029D.includes preserve=yes
#include "stdafx.h"
//## end module%40E266EE029D.includes

// CMyCGLib
#include "CMyCGLib.h"
//## begin module%40E266EE029D.declarations preserve=no
//## end module%40E266EE029D.declarations

//## begin module%40E266EE029D.additionalDeclarations preserve=yes
//{{

#include "gl\gl.h"
#include "gl\glu.h"
#include "ClassArr.h"
//#include "Str.h"
#include <afx.h>
#include "TrackBall.h"

#define Draw3DLine(x1,y1,z1,x2,y2,z2) \
		glBegin(GL_LINES); \
		glVertex3f((x1),(y1),(z1)); \
		glVertex3f((x2),(y2),(z2)); \
		glEnd();

struct CXYZW		{ float* x; float* y; float* z; float* w; };
struct CFRGB		{ float r; float g; float b; };
enum   CDataGroup	{ G3D, GTRUE3D, G4D, G4DNOPROP, G4DHASPROP};
enum   CMapType		{ FLAT, GRID, PTS, SURFACE, BLOCK, NOPROPERTY4D, PROPERTY4D, MESH2D};
enum   CRotateType	{ RX, RY, RZ, RXY, RXZ, RYZ, RXYZ, NONE};
enum   CLinkType	{ NoLink, XYLink, XZLink, YZLink, XYZLink }; 
enum   CMoveType	{ MOVE, RANDROTAT, CENTERROTAT }; 

struct CMarker
{
	float	x;
	float	y;
	float	Hight;
	int		Flag;
//	CStr	Name;

	CString Name; //??
};

typedef CClassArray<CMarker, CMarker&>	CMarkArray;

struct CDataSet		
{ 
//	CStr		m_MapName;
	CString		m_MapName;

	CLinkType	m_Link;
	CMapType	m_MapType;
	CRotateType	m_Rotate;
	CMoveType	m_Move;

	CFRGB m_BackCr; 
	CFRGB m_MinCr; 
	CFRGB m_MaxCr; 
	CFRGB m_BorderCr;
	bool  m_bAutoCr;
	int	  m_nCrLevel;
	int	  m_nUserCrNum;
	float m_fCrThreshold;
	CClassArray<COLORREF, COLORREF&> m_UserCr;

	int   m_bAxes;
	int   m_bLegend;
	int   m_bBorder;
	int   m_bSide;
	int   m_bBase;

	int   nDataGroup;
	int	  nX;
	int	  nY;
	int	  nZ;
	float m_fXmin;
	float m_fXmax;
	float m_fYmin;
	float m_fYmax;
	float m_fZmin;
	float m_fZmax;
	float m_fWmin;
	float m_fWmax;

	float m_fDx;
	float m_fDy;
	int	  m_xLgdOs;  // legend offset
	int   m_yLgdOs;

	// rotation, transformation, scaling
	float m_xRotat;
	float m_yRotat;
	float m_zRotat;
	float m_xTrans;
	float m_yTrans;
	float m_zTrans;
	float m_xScale;
	float m_yScale;
	float m_zScale;

	CMarkArray m_Marker;
	CFRGB m_MarkerCr;
	int   m_bMark;
	int   m_bLable;
	int   m_bMarkLine;
	int   m_nMarkIndex;

	//for rotation matrix
	float objectXform[16];
};


struct C3DSet : public CDataSet
{
	CXYZW	m_XYZ;
	// add more properties here for 3D mapping

	C3DSet()
	{
		m_XYZ.x = NULL;
		m_XYZ.y = NULL;
		m_XYZ.z = NULL;
		m_XYZ.w = NULL;
	};

	~C3DSet()
	{
		if(m_XYZ.x)	delete []m_XYZ.x;
		if(m_XYZ.y)	delete []m_XYZ.y;
		if(m_XYZ.z)	delete []m_XYZ.z;
		if(m_XYZ.w)	delete []m_XYZ.w;
	};
};

typedef CClassArray<C3DSet, C3DSet&> C3DSetArray;


//}}
//## end module%40E266EE029D.additionalDeclarations


//## begin COpenGL%40E2654302E9.preface preserve=yes
//## end COpenGL%40E2654302E9.preface

//## Class: COpenGL%40E2654302E9
//## Category: 3dViewer%40C7B40F0349
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class COpenGL : public CMyCGLib  //## Inherits: <unnamed>%41E37A190081
{
  //## begin COpenGL%40E2654302E9.initialDeclarations preserve=yes
	//added by xdh,2002,10,26
    //void HSV_To_RGB(float &r, float &g, float &b, float h, float s, float v);
public:
	void DrawColorBar();	
	int  m_nElapseTime;  //the elapsing time (milliseconds ) of rendering a frame    

	CMapType  m_ModeType;// the displaying mode: Surfac,Grid,Line
	CMapType  m_ModeType0;
	HGLRC GetContext() const; //return the context device handle	

	//added by xdh,2004,10,26.
	//for text printing
	GLYPHMETRICSFLOAT gmf[256];	// Storage For Information About Our Outline Font Characters
	GLuint	base;				// Base Display List For The Font Set
//  void Reset();
  //## end COpenGL%40E2654302E9.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: COpenGL%40E26543034B
      COpenGL ();

    //## Destructor (specified)
      //## Operation: ~COpenGL%40E26543034A
      ~COpenGL ();


    //## Other Operations (specified)
      //## Operation: LoadMarkerData%40E265430321
      bool LoadMarkerData (const char* FileName);

      //## Operation: SaveMaps%40E265430323
      virtual void SaveMaps ();

      //## Operation: OpenMaps%40E265430325
      virtual void OpenMaps ();

      //## Operation: Legend%40E265430327
      virtual void Legend (RECT& rt);

      //## Operation: DrawBox%40E265430329
      virtual void DrawBox ();

      //## Operation: ColorLevel%40E26543032A
      virtual void ColorLevel (float level);

      //## Operation: GDIDrawBanner%40E26543032C
      virtual void GDIDrawBanner ();

      //## Operation: OpenGLDrawBanner%40E26543032D
      virtual void OpenGLDrawBanner ();

      //## Operation: GDIDrawMap%40E26543032E
      virtual void GDIDrawMap () = 0;

      //## Operation: DrawLegend%40E265430330
      void DrawLegend ();

      //## Operation: DrawAxis%40E265430331
      virtual void DrawAxis ();

      //## Operation: DrawBorder%40E265430332
      void DrawBorder ();

      //## Operation: OpenGLRendering%40E265430333
      virtual void OpenGLRendering ();

      //## Operation: PrintString%40E265430334
      void PrintString (const char* str);

      //## Operation: RasterFont%40E265430336
      //	 raster font support
      void RasterFont ();

      //## Operation: LButtonUp%40E265430337
      void LButtonUp (POINT point);

      //## Operation: LButtonDown%40E265430339
      void LButtonDown (POINT point);

      //## Operation: MouseMove%40E26543033B
      BOOL MouseMove (UINT nFlags, POINT point);

      //## Operation: KillOpenGLTimer%40E26543033E
      void KillOpenGLTimer (int n);

      //## Operation: SetOpenGLTimer%40E265430340
      void SetOpenGLTimer (int n);

      //## Operation: OpenGLTimer%40E265430342
      void OpenGLTimer (CRotateType type);

      //## Operation: OpenGLSize%40E265430344
      void OpenGLSize (int cx, int cy);

      //## Operation: DestroyOpenGL%40E265430347
      void DestroyOpenGL ();

      //## Operation: InitOpenGL%40E265430348
      //	 OpenGL initialization
      virtual bool InitOpenGL (HWND hWnd);

      //## Operation: OpenGLDrawMap%40E9F97303DC
      virtual void OpenGLDrawMap () = 0;

      //## Operation: RButtonUp%4175DD45024E
      void RButtonUp (POINT point);

      //## Operation: SetFaceMode%4175DDD400EC
      void SetFaceMode ();

      //## Operation: SetWireFrameMode%4175DE29035B
      void SetWireFrameMode ();

      //## Operation: SetVertexMode%4175DEA500C4
      void SetVertexMode ();

      //## Operation: SetSpecular%4175DF360343
      void SetSpecular (COLORREF specularColor);

      //## Operation: SetDiffuse%4175DF9601D9
      void SetDiffuse (COLORREF diffuseColor);

      //## Operation: SetAmbient%4175DFE40235
      void SetAmbient (COLORREF ambientColor);

      //## Operation: SetBckGroundColor%4175E0250379
      void SetBckGroundColor (COLORREF bckGrdColor);

      //## Operation: SetLight%4175E06703A6
      void SetLight (bool bIsOn);

      //## Operation: MButtonDown%4175E095037A
      void MButtonDown (POINT point);

      //## Operation: MButtonUp%4175E1380266
      void MButtonUp (POINT point);

      //## Operation: RButtonDown%4175E16902C1
      void RButtonDown (POINT point);

      //## Operation: Reset%4178D229031C
      void Reset ();

    // Data Members for Class Attributes

      //## Attribute: m_bDraw%40E2654302ED
      //## begin COpenGL::m_bDraw%40E2654302ED.attr preserve=no  public: bool {UA} 
      bool m_bDraw;
      //## end COpenGL::m_bDraw%40E2654302ED.attr

      //## Attribute: m_nD%40E2654302EE
      //## begin COpenGL::m_nD%40E2654302EE.attr preserve=no  public: int {UA} 
      int m_nD;
      //## end COpenGL::m_nD%40E2654302EE.attr

      //## Attribute: m_fDelta%40E2654302EF
      //## begin COpenGL::m_fDelta%40E2654302EF.attr preserve=no  public: float {UA} 
      float m_fDelta;
      //## end COpenGL::m_fDelta%40E2654302EF.attr

      //## Attribute: m_fAngleZ%40E2654302F0
      //## begin COpenGL::m_fAngleZ%40E2654302F0.attr preserve=no  public: float {UA} 
      float m_fAngleZ;
      //## end COpenGL::m_fAngleZ%40E2654302F0.attr

      //## Attribute: m_fAngleY%40E2654302F1
      //## begin COpenGL::m_fAngleY%40E2654302F1.attr preserve=no  public: float {UA} 
      float m_fAngleY;
      //## end COpenGL::m_fAngleY%40E2654302F1.attr

      //## Attribute: m_fAngleX%40E2654302F2
      //## begin COpenGL::m_fAngleX%40E2654302F2.attr preserve=no  public: float {UA} 
      float m_fAngleX;
      //## end COpenGL::m_fAngleX%40E2654302F2.attr

      //## Attribute: m_fDY%40E2654302F3
      //	 Position, rotation ,scaling
      //## begin COpenGL::m_fDY%40E2654302F3.attr preserve=no  public: float {UA} 
      float m_fDY;
      //## end COpenGL::m_fDY%40E2654302F3.attr

      //## Attribute: m_fC%40E2654302F4
      //## begin COpenGL::m_fC%40E2654302F4.attr preserve=no  public: float {UA} 
      float m_fC;
      //## end COpenGL::m_fC%40E2654302F4.attr

      //## Attribute: m_CursorRotation%40E2654302F5
      //## begin COpenGL::m_CursorRotation%40E2654302F5.attr preserve=no  public: HCURSOR {UA} 
      HCURSOR m_CursorRotation;
      //## end COpenGL::m_CursorRotation%40E2654302F5.attr

      //## Attribute: m_bDrawNothing%40E265430300
      //## begin COpenGL::m_bDrawNothing%40E265430300.attr preserve=no  public: bool {UA} 
      bool m_bDrawNothing;
      //## end COpenGL::m_bDrawNothing%40E265430300.attr

      //## Attribute: m_bRButtonDown%40E265430301
      //## begin COpenGL::m_bRButtonDown%40E265430301.attr preserve=no  public: bool {UA} 
      bool m_bRButtonDown;
      //## end COpenGL::m_bRButtonDown%40E265430301.attr

      //## Attribute: m_bLButtonDown%40E265430302
      //	 Mouse
      //## begin COpenGL::m_bLButtonDown%40E265430302.attr preserve=no  public: bool {UA} 
      bool m_bLButtonDown;
      //## end COpenGL::m_bLButtonDown%40E265430302.attr

      //## Attribute: m_nTimerSpeed%40E265430303
      //## begin COpenGL::m_nTimerSpeed%40E265430303.attr preserve=no  public: int {UA} 
      int m_nTimerSpeed;
      //## end COpenGL::m_nTimerSpeed%40E265430303.attr

      //## Attribute: m_hDC%40E26543030E
      //## begin COpenGL::m_hDC%40E26543030E.attr preserve=no  public: HDC {UA} NULL
      HDC m_hDC;
      //## end COpenGL::m_hDC%40E26543030E.attr

      //## Attribute: m_hWnd%40E26543030F
      //## begin COpenGL::m_hWnd%40E26543030F.attr preserve=no  public: HWND {UA} NULL
      HWND m_hWnd;
      //## end COpenGL::m_hWnd%40E26543030F.attr

      //## Attribute: m_Version%40E36A240289
      //## begin COpenGL::m_Version%40E36A240289.attr preserve=no  public: CString {UA} 
      CString m_Version;
      //## end COpenGL::m_Version%40E36A240289.attr

      //## Attribute: m_hGLContext%40E36B78006B
      //## begin COpenGL::m_hGLContext%40E36B78006B.attr preserve=no  public: HGLRC {UA} 
      HGLRC m_hGLContext;
      //## end COpenGL::m_hGLContext%40E36B78006B.attr

      //## Attribute: m_nFontOffset%40E36BB501EF
      //## begin COpenGL::m_nFontOffset%40E36BB501EF.attr preserve=no  public: GLuint {UA} 
      GLuint m_nFontOffset;
      //## end COpenGL::m_nFontOffset%40E36BB501EF.attr

      //## Attribute: m_pDS%40E36BE5032E
      //## begin COpenGL::m_pDS%40E36BE5032E.attr preserve=no  public: CDataSet* {UA} 
      CDataSet* m_pDS;
      //## end COpenGL::m_pDS%40E36BE5032E.attr

      //## Attribute: m_LDownPos%40E36ED600CE
      //## begin COpenGL::m_LDownPos%40E36ED600CE.attr preserve=no  public: POINT {UA} 
      POINT m_LDownPos;
      //## end COpenGL::m_LDownPos%40E36ED600CE.attr

      //## Attribute: m_DataGroup%40E3BAE4033E
      //## begin COpenGL::m_DataGroup%40E3BAE4033E.attr preserve=no  public: CDataGroup {UA} 
      CDataGroup m_DataGroup;
      //## end COpenGL::m_DataGroup%40E3BAE4033E.attr

      //## Attribute: m_bLightingOn%40F332EC00DD
      //## begin COpenGL::m_bLightingOn%40F332EC00DD.attr preserve=no  public: BOOL {UA} !TRUE
      BOOL m_bLightingOn;
      //## end COpenGL::m_bLightingOn%40F332EC00DD.attr
      BOOL m_bAcceleratingOn;
      //## Attribute: m_trackBall%4175E2D00227
	  BOOL m_bMemoryBoosterOn;
      //## begin COpenGL::m_trackBall%4175E2D00227.attr preserve=no  public: CTrackBall {UA} 
      CTrackBall m_trackBall;
      //## end COpenGL::m_trackBall%4175E2D00227.attr

      //## Attribute: m_axis%4175E3F4028B
      //## begin COpenGL::m_axis%4175E3F4028B.attr preserve=no  public: float[3] {UA} 
      float m_axis[3];
      //## end COpenGL::m_axis%4175E3F4028B.attr

      //## Attribute: m_angle%4175E5B402CB
      //## begin COpenGL::m_angle%4175E5B402CB.attr preserve=no  public: float {UA} 0.0
      float m_angle;
      //## end COpenGL::m_angle%4175E5B402CB.attr

      //## Attribute: m_xTranslation%4175E6540393
      //## begin COpenGL::m_xTranslation%4175E6540393.attr preserve=no  public: float {UA} 
      float m_xTranslation;
      //## end COpenGL::m_xTranslation%4175E6540393.attr

      //## Attribute: m_yTranslation%4175E66501AD
      //## begin COpenGL::m_yTranslation%4175E66501AD.attr preserve=no  public: float {UA} 
      float m_yTranslation;
      //## end COpenGL::m_yTranslation%4175E66501AD.attr

      //## Attribute: m_zTranslation%4175E6740303
      //## begin COpenGL::m_zTranslation%4175E6740303.attr preserve=no  public: float {UA} 
      float m_zTranslation;
      //## end COpenGL::m_zTranslation%4175E6740303.attr

      //## Attribute: m_dfZoom%4175E67C0228
      //## begin COpenGL::m_dfZoom%4175E67C0228.attr preserve=no  public: double {UA} 
      double m_dfZoom;
      //## end COpenGL::m_dfZoom%4175E67C0228.attr

      //## Attribute: m_rMouseDownPoint%4175E6840035
      //## begin COpenGL::m_rMouseDownPoint%4175E6840035.attr preserve=no  public: CPoint {UA} 
      CPoint m_rMouseDownPoint;
      //## end COpenGL::m_rMouseDownPoint%4175E6840035.attr

      //## Attribute: m_mMouseDownPoint%4175E6B30246
      //## begin COpenGL::m_mMouseDownPoint%4175E6B30246.attr preserve=no  public: CPoint {UA} 
      CPoint m_mMouseDownPoint;
      //## end COpenGL::m_mMouseDownPoint%4175E6B30246.attr

      //## Attribute: m_DS%4175E6BB0305
      //## begin COpenGL::m_DS%4175E6BB0305.attr preserve=no  public: C3DSetArray {UA} 
      C3DSetArray m_DS;
      //## end COpenGL::m_DS%4175E6BB0305.attr

      //## Attribute: m_p3D%4175E6C303A7
      //## begin COpenGL::m_p3D%4175E6C303A7.attr preserve=no  public: C3DSet* {UA} 
      C3DSet* m_p3D;
      //## end COpenGL::m_p3D%4175E6C303A7.attr

    // Additional Public Declarations
      //## begin COpenGL%40E2654302E9.public preserve=yes
      //## end COpenGL%40E2654302E9.public

  protected:
    // Additional Protected Declarations
      //## begin COpenGL%40E2654302E9.protected preserve=yes
      //## end COpenGL%40E2654302E9.protected

  private:

    //## Other Operations (specified)
      //## Operation: CreateViewGLContext%40E26543031F
      bool CreateViewGLContext ();

      //## Operation: SetWindowPixelFormat%40E265430320
      bool SetWindowPixelFormat ();

    // Data Members for Class Attributes

      //## Attribute: m_fDV%40E2654302EA
      //## begin COpenGL::m_fDV%40E2654302EA.attr preserve=no  private: float {UA} 
      float m_fDV;
      //## end COpenGL::m_fDV%40E2654302EA.attr

      //## Attribute: m_fWinCx%4178D39E030E
      //## begin COpenGL::m_fWinCx%4178D39E030E.attr preserve=no  private: float {UA} 
      float m_fWinCx;
      //## end COpenGL::m_fWinCx%4178D39E030E.attr

      //## Attribute: m_fWinCy%4178D3AD0067
      //## begin COpenGL::m_fWinCy%4178D3AD0067.attr preserve=no  private: float {UA} 
      float m_fWinCy;
      //## end COpenGL::m_fWinCy%4178D3AD0067.attr

      //## Attribute: m_nHeight%40E2654302EB
      //## begin COpenGL::m_nHeight%40E2654302EB.attr preserve=no  private: int {UA} 
      int m_nHeight;
      //## end COpenGL::m_nHeight%40E2654302EB.attr

      //## Attribute: m_nWidth%40E2654302EC
      //## begin COpenGL::m_nWidth%40E2654302EC.attr preserve=no  private: int {UA} 
      int m_nWidth;
      //## end COpenGL::m_nWidth%40E2654302EC.attr

      //## Attribute: name%4178D2CA01FB
      //## begin COpenGL::name%4178D2CA01FB.attr preserve=no  private: int {UA} 
      int name;
      //## end COpenGL::name%4178D2CA01FB.attr

    // Additional Private Declarations
      //## begin COpenGL%40E2654302E9.private preserve=yes
	  int m_nDispType; //display mode: 0---face  1---line  2---vertex
      //## end COpenGL%40E2654302E9.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin COpenGL%40E2654302E9.implementation preserve=yes
  public:

	  /*
	  float      m_angle;

	  float      m_fWinCx;
	  float      m_fWinCy;
	  
	  float  m_xTranslation;
	  float  m_yTranslation;
	  float  m_zTranslation;
	  
      double m_dfZoom;

	  CPoint m_rMouseDownPoint;	
	  CPoint m_mMouseDownPoint;

	  C3DSetArray	m_DS;
	  C3DSet*		m_p3D;
	  */
      //## end COpenGL%40E2654302E9.implementation
};

//## begin COpenGL%40E2654302E9.postscript preserve=yes
//## end COpenGL%40E2654302E9.postscript

// Class COpenGL 

//## begin module%40E266EE029D.epilog preserve=yes
//## end module%40E266EE029D.epilog


#endif
