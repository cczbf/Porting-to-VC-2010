//## begin module%40EB6BE101BB.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40EB6BE101BB.cm

//## begin module%40EB6BE101BB.cp preserve=no
//## end module%40EB6BE101BB.cp

//## Module: COpenGL3DEx%40EB6BE101BB; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\COpenGL3DEx.h

#ifndef COpenGL3DEx_h
#define COpenGL3DEx_h 1

//## begin module%40EB6BE101BB.additionalIncludes preserve=no
//## end module%40EB6BE101BB.additionalIncludes

//## begin module%40EB6BE101BB.includes preserve=yes
//## end module%40EB6BE101BB.includes

// CMesh

class CMesh;
// COpenGL3D
#include "COpenGL3D.h"
//## begin module%40EB6BE101BB.declarations preserve=no
//## end module%40EB6BE101BB.declarations

//## begin module%40EB6BE101BB.additionalDeclarations preserve=yes
//## end module%40EB6BE101BB.additionalDeclarations


//## begin COpenGL3DEx%40EB695901F5.preface preserve=yes
//## end COpenGL3DEx%40EB695901F5.preface

//## Class: COpenGL3DEx%40EB695901F5
//		  void DrawEdgeWithStretchRatio();   	  void raw2DMesh
//	Edge(); 	  void DrawStatistics();   	  void SetMesh
//	Flag(bool  bIsNew);
//## Category: 3dViewer::Core%40DB9BAD036C
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: 

class COpenGL3DEx : public COpenGL3D  //## Inherits: <unnamed>%40EB6C2F038A
{
public:
  //## begin COpenGL3DEx%40EB695901F5.initialDeclarations preserve=yes
	  void Draw3DMeshWithStrethRatio();
  	  void DrawArc(float a1, float a2, float a3,
		           float b1, float b2, float b3);
	  void Draw3DMeshWithArc();
	  void DrawGeoimgMesh();
	  void DrawLabelPts();
      
	  void DrawText(CString title, CString txt, float x, float y, float z);
  //## end COpenGL3DEx%40EB695901F5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: COpenGL3DEx%40F32FEC0145
      COpenGL3DEx ();

    //## Destructor (specified)
      //## Operation: ~COpenGL3DEx%40F3A43301F9
      ~COpenGL3DEx ();


    //## Other Operations (specified)
      //## Operation: LoadSurferRAWData%40EB9EA300D9
      void LoadSurferRAWData (HANDLE hRaw, int col, int row);

      //## Operation: Load3DData%40EBAB53021A
      void Load3DData (HANDLE hRaw, int col, int row, float x0 = 0.0, float y0 = 0.0, float x1 = 0.0, float y1 = 0.0);

      //## Operation: InitOpenGL%40EDFAA8027E
      bool InitOpenGL (HWND hWnd);

      //## Operation: LightingOn%40F23A3D0159
      void LightingOn (BOOL bOn = TRUE);
	  
      //## Operation: OpenGLRendering%40F4966E0227
      void OpenGLRendering ();

      //## Operation: DrawMeshSurface%40F4A52C0075
      void DrawMeshSurface ();

      //## Operation: OpenGL3DExDrawMesh%40F4DCC70175
      void OpenGL3DExDrawMesh ();

      //## Operation: DrawAxis%40F72D6A011C
      void DrawAxis ();

      //## Operation: DrawBorder%40F72D730197
      void DrawBorder ();

      //## Operation: DrawMeshGrid%40F747B2024D

      //## Operation: Draw2DMeshWithStretchRatio%418843A90053
      void Draw3DEdgeWithStretchRatio ();

      //## Operation: Draw2DMeshEdge%418843CB01ED
      void Draw2DMeshEdgeVertex ();

      //## Operation: DrawStatistics%418843E602B4

      //## Operation: SetMeshFlag%418843EB031F
      void SetMeshFlag (bool bIsNew);

    // Data Members for Associations

      //## Association: Triangle Mesh::<unnamed>%40F3A25D01F2
      //## Role: COpenGL3DEx::m_pCMesh%40F3A25F00DD
      //## begin COpenGL3DEx::m_pCMesh%40F3A25F00DD.role preserve=no  public: CMesh { -> RHgAN}
      CMesh *m_pCMesh;
	  BOOL  m_bIsShowLabel;
	  BOOL  m_bIsShowLine;
	  BOOL  m_bIsShowArc;
      //## end COpenGL3DEx::m_pCMesh%40F3A25F00DD.role

  protected:
    // Additional Protected Declarations
      //## begin COpenGL3DEx%40EB695901F5.protected preserve=yes
	  bool m_bIsNewMesh;
      //## end COpenGL3DEx%40EB695901F5.protected
  private:
    // Additional Private Declarations
      //## begin COpenGL3DEx%40EB695901F5.private preserve=yes
      //## end COpenGL3DEx%40EB695901F5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin COpenGL3DEx%40EB695901F5.implementation preserve=yes
      //## end COpenGL3DEx%40EB695901F5.implementation

};

//## begin COpenGL3DEx%40EB695901F5.postscript preserve=yes
//## end COpenGL3DEx%40EB695901F5.postscript

// Class COpenGL3DEx 

//## begin module%40EB6BE101BB.epilog preserve=yes
//## end module%40EB6BE101BB.epilog


#endif
