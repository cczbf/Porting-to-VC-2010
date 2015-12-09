//## begin module%40F3459802E7.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40F3459802E7.cm

//## begin module%40F3459802E7.cp preserve=no
//## end module%40F3459802E7.cp

//## Module: CMesh%40F3459802E7; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CMesh.h

#ifndef CMesh_h
#define CMesh_h 1

//## begin module%40F3459802E7.additionalIncludes preserve=no
//## end module%40F3459802E7.additionalIncludes

//## begin module%40F3459802E7.includes preserve=yes
//## end module%40F3459802E7.includes

// CArray2DInt
#include "CArray2DInt.h"
// CRelxationLabelling
#include "CRelxationLabelling.h"
// CIntArray
#include "CIntArray.h"
// CL0
#include "CL0.h"
// CL
#include "CL.h"
// CVertexArray
#include "CVertexArray.h"
// CTriangleArray
#include "CTriangleArray.h"
// CLw
#include "CLw.h"
//## begin module%40F3459802E7.declarations preserve=no
//## end module%40F3459802E7.declarations

//## begin module%40F3459802E7.additionalDeclarations preserve=yes
//## end module%40F3459802E7.additionalDeclarations


//## begin CAdjacentMatrixEx%413442F9005A.preface preserve=yes
//## end CAdjacentMatrixEx%413442F9005A.preface

//## Class: CAdjacentMatrixEx%413442F9005A
//## Category: Triangle Mesh%40F38ABC0112
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CAdjacentMatrixEx : public CArray2DFLT  //## Inherits: <unnamed>%4135455A00A5
{
  //## begin CAdjacentMatrixEx%413442F9005A.initialDeclarations preserve=yes
  //## end CAdjacentMatrixEx%413442F9005A.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: FindBorder%41357E49017E
      int FindBorder ();

    // Data Members for Associations

      //## Association: Triangle Mesh::<unnamed>%41357D3C01D1
      //## Role: CAdjacentMatrixEx::m_CIntArrayBorder%41357D3D00CF
      //## begin CAdjacentMatrixEx::m_CIntArrayBorder%41357D3D00CF.role preserve=no  public: CIntArray { -> VHgAN}
      CIntArray m_CIntArrayBorder;
      //## end CAdjacentMatrixEx::m_CIntArrayBorder%41357D3D00CF.role

    // Additional Public Declarations
      //## begin CAdjacentMatrixEx%413442F9005A.public preserve=yes
      //## end CAdjacentMatrixEx%413442F9005A.public

  protected:
    // Additional Protected Declarations
      //## begin CAdjacentMatrixEx%413442F9005A.protected preserve=yes
      //## end CAdjacentMatrixEx%413442F9005A.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_nBorderLength%41357E39030C
      //## begin CAdjacentMatrixEx::m_nBorderLength%41357E39030C.attr preserve=no  private: int {UA} 0
      int m_nBorderLength;
      //## end CAdjacentMatrixEx::m_nBorderLength%41357E39030C.attr

    // Additional Private Declarations
      //## begin CAdjacentMatrixEx%413442F9005A.private preserve=yes
      //## end CAdjacentMatrixEx%413442F9005A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CAdjacentMatrixEx%413442F9005A.implementation preserve=yes
      //## end CAdjacentMatrixEx%413442F9005A.implementation

};

//## begin CAdjacentMatrixEx%413442F9005A.postscript preserve=yes
//## end CAdjacentMatrixEx%413442F9005A.postscript

//## begin CMesh%3FCA8CD003E7.preface preserve=yes
//## end CMesh%3FCA8CD003E7.preface

//## Class: CMesh%3FCA8CD003E7
//## Category: Triangle Mesh%40F38ABC0112
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CMesh 
{
  //## begin CMesh%3FCA8CD003E7.initialDeclarations preserve=yes
public:
   float ComputeStretchRatio(float  d3l, float d2l);
   float ComputeGeoStretch();
   float GetGeometryStretch();
//   void  SerRL(CRelxationLabelling *pRL);
   void  SetEmptyRL();
   void  ComputeColor();
   int   GetBorderNodeNum();
   void  ComputeEdgeNum();
   //added by xdh,2004,11,2. for Undo/Redo
   void  AddRef();
   void  Release();
  //## end CMesh%3FCA8CD003E7.initialDeclarations

  public:
    //## Constructors (generated)
      CMesh();

    //## Destructor (generated)
      ~CMesh();


    //## Other Operations (specified)
      //## Operation: LoadFromPly%3FCA8D0E0168
      BOOL LoadFromPly (CString strFile);

      //## Operation: LoadFromRaw%41344610035B
      BOOL LoadFromRaw (HANDLE hRaw, int w, int h);

      //## Operation: LoadFromINI%3FCA8D1C001A
      BOOL LoadFromINI (CString strFile);

      //## Operation: Write2Ply%3FCA8D24016B
      BOOL Write2Ply (CString strFile);

      //## Operation: Write2INI%3FCA8D2B0273
      BOOL Write2INI (CString strFile);

      //## Operation: CalculateLw%3FCAB28C010D
      void CalculateLw ();

      //## Operation: CalculateLwInverseEdge%3FCF2A86032E
      void CalculateLwInverseEdge ();

      //## Operation: CalculateLw1%3FCD29D6006A
      void CalculateLw1 ();

      //## Operation: Purify%3FCC578E003C
      void Purify ();

      //## Operation: Optimize%3FCC56B203DE
      void Optimize (int nMode = 1);

      //## Operation: Normalize%3FCDAC5D0145
      void Normalize (float fScaleFactor = 1.0f);

      //## Operation: SetAdjacentMatrixEx%413448380278
      void SetAdjacentMatrixEx ();

      //## Operation: SetMesh%4181ECA4023A
      void SetMesh (int nTriangle, int* nV1, int* nV2, int* nV3, DWORD* c12, DWORD* c23, DWORD* c31, int nVlength, double* x, double* y, double* z);

      //## Operation: GetMesh%41882FD101F8
      void GetMesh (int* nTriangle, int** nV1, int** nV2, int** nV3, int** c12, int** c23, int** c31, int* nVlength, double** x, double** y, double** z);

      //## Operation: SetOptimizationIterationLimit%3FCE82D700D8
      void SetOptimizationIterationLimit (int nInterateLimit);

      //## Operation: Distance%3FCF4C6E00B8
      double Distance (double x0, double y0, double z0, double x1, double y1, double z1);

      //## Operation: ToSphere%3FCF46110034
      void ToSphere ();

      //## Operation: RemoveIsolateVertex%3FCF542303E0
      void RemoveIsolateVertex ();

      //## Operation: Load2DMesh%417C5C4D02DD
      void Load2DMesh (CString fileName);

    // Data Members for Class Attributes

      //## Attribute: m_nNumOfVertexs%3FCA8CF503B1
      //## begin CMesh::m_nNumOfVertexs%3FCA8CF503B1.attr preserve=no  public: int {UA} 0
      int m_nNumOfVertexs;
      //## end CMesh::m_nNumOfVertexs%3FCA8CF503B1.attr

      //## Attribute: m_nNumOfTriangles%3FCA8CFE0143
      //## begin CMesh::m_nNumOfTriangles%3FCA8CFE0143.attr preserve=no  public: int {UA} 0
      int m_nNumOfTriangles;
      //## end CMesh::m_nNumOfTriangles%3FCA8CFE0143.attr

      //## Attribute: m_MinX%3FCDAC66012C
      //## begin CMesh::m_MinX%3FCDAC66012C.attr preserve=no  public: double {UA} FLT_MAX
      double m_MinX;
      //## end CMesh::m_MinX%3FCDAC66012C.attr

      //## Attribute: m_MinY%3FCDAC7500C4
      //## begin CMesh::m_MinY%3FCDAC7500C4.attr preserve=no  public: double {UA} FLT_MAX
      double m_MinY;
      //## end CMesh::m_MinY%3FCDAC7500C4.attr

      //## Attribute: m_MinZ%3FCDAD160209
      //## begin CMesh::m_MinZ%3FCDAD160209.attr preserve=no  public: double {UA} FLT_MAX
      double m_MinZ;
      //## end CMesh::m_MinZ%3FCDAD160209.attr

      //## Attribute: m_MaxX%3FCDAD4103D2
      //## begin CMesh::m_MaxX%3FCDAD4103D2.attr preserve=no  public: double {UA} FLT_MIN
      double m_MaxX;
      //## end CMesh::m_MaxX%3FCDAD4103D2.attr

      //## Attribute: m_MaxY%3FCDAD560310
      //## begin CMesh::m_MaxY%3FCDAD560310.attr preserve=no  public: double {UA} FLT_MIN
      double m_MaxY;
      //## end CMesh::m_MaxY%3FCDAD560310.attr

      //## Attribute: m_MaxZ%3FCDAD6001C1
      //## begin CMesh::m_MaxZ%3FCDAD6001C1.attr preserve=no  public: double {UA} FLT_MIN
      double m_MaxZ;
      //## end CMesh::m_MaxZ%3FCDAD6001C1.attr

      //## Attribute: m_CenterX%3FCDADDE02D3
      //## begin CMesh::m_CenterX%3FCDADDE02D3.attr preserve=no  public: double {UA} 0.0
      double m_CenterX;
      //## end CMesh::m_CenterX%3FCDADDE02D3.attr

      //## Attribute: m_CenterY%3FCDADF30042
      //## begin CMesh::m_CenterY%3FCDADF30042.attr preserve=no  public: double {UA} 0.0
      double m_CenterY;
      //## end CMesh::m_CenterY%3FCDADF30042.attr

      //## Attribute: m_CenterZ%3FCDADF50050
      //## begin CMesh::m_CenterZ%3FCDADF50050.attr preserve=no  public: double {UA} 0.0
      double m_CenterZ;
      //## end CMesh::m_CenterZ%3FCDADF50050.attr

      //## Attribute: m_nOptIterationLimit%3FCE826B01C6
      //## begin CMesh::m_nOptIterationLimit%3FCE826B01C6.attr preserve=no  public: int {UA} 100
      int m_nOptIterationLimit;
      //## end CMesh::m_nOptIterationLimit%3FCE826B01C6.attr

      //## Attribute: m_dErrorBound%3FCE8A3A0001
      //## begin CMesh::m_dErrorBound%3FCE8A3A0001.attr preserve=no  public: float {UA} (float)0.01
      float m_dErrorBound;
      //## end CMesh::m_dErrorBound%3FCE8A3A0001.attr

      //## Attribute: m_dIncErrorBound%3FCE8A4F00E1
      //## begin CMesh::m_dIncErrorBound%3FCE8A4F00E1.attr preserve=no  public: float {UA} (float)0.0001
      float m_dIncErrorBound;
      //## end CMesh::m_dIncErrorBound%3FCE8A4F00E1.attr

      //## Attribute: m_nOptTarget%3FD56C940196
      //## begin CMesh::m_nOptTarget%3FD56C940196.attr preserve=no  public: int {UA} 0
      int m_nOptTarget;
      //## end CMesh::m_nOptTarget%3FD56C940196.attr

      //## Attribute: m_dW%3FD6A5490263
      //## begin CMesh::m_dW%3FD6A5490263.attr preserve=no  public: float {UA} 1.0
      float m_dW;
      //## end CMesh::m_dW%3FD6A5490263.attr

    // Data Members for Associations

      //## Association: Triangle Mesh::<unnamed>%413443460001
      //## Role: CMesh::m_CAdjacentMatrixEx%41344346032C
      //## begin CMesh::m_CAdjacentMatrixEx%41344346032C.role preserve=no  public: CAdjacentMatrixEx { -> VHgAN}
      CAdjacentMatrixEx m_CAdjacentMatrixEx;
      //## end CMesh::m_CAdjacentMatrixEx%41344346032C.role

      //## Association: <unnamed>%3FCA8E210300
      //## Role: CMesh::m_CTriangleArray%3FCA8E2203E1
      //## begin CMesh::m_CTriangleArray%3FCA8E2203E1.role preserve=no  public: CTriangleArray { -> VHgAN}
      CTriangleArray m_CTriangleArray;
      //## end CMesh::m_CTriangleArray%3FCA8E2203E1.role

      //## Association: <unnamed>%3FD5701302AB
      //## Role: CMesh::m_CL%3FD57014027B
      //## begin CMesh::m_CL%3FD57014027B.role preserve=no  public: CL { -> VHgAN}
      CL m_CL;
      //## end CMesh::m_CL%3FD57014027B.role

      //## Association: <unnamed>%3FCA8E180000
      //## Role: CMesh::m_CVertexArray%3FCA8E1902A4
      //## begin CMesh::m_CVertexArray%3FCA8E1902A4.role preserve=no  public: CVertexArray { -> VHgAN}
      CVertexArray m_CVertexArray;
      //## end CMesh::m_CVertexArray%3FCA8E1902A4.role

      //## Association: <unnamed>%3FCAB10C03AB
      //## Role: CMesh::m_CLw%3FCAB10E0025
      //## begin CMesh::m_CLw%3FCAB10E0025.role preserve=no  public: CLw { -> VHgAN}
      CLw m_CLw;
      //## end CMesh::m_CLw%3FCAB10E0025.role

      //## Association: <unnamed>%3FD56FBB0094
      //## Role: CMesh::m_CL0%3FD56FBC0385
      //## begin CMesh::m_CL0%3FD56FBC0385.role preserve=no  public: CL0 { -> VHgAN}
      CL0 m_CL0;
      //## end CMesh::m_CL0%3FD56FBC0385.role

      //## Association: <unnamed>%3FD56FB3036D
      //## Role: CMesh::m_CL1%3FD56FB4035B
      //## begin CMesh::m_CL1%3FD56FB4035B.role preserve=no  public: CL0 { -> VHgAN}
      CL0 m_CL1;
      //## end CMesh::m_CL1%3FD56FB4035B.role

      //## Association: Relaxation Labelling::<unnamed>%417C59480064
      //## Role: CMesh::m_pCRelxationLabelling%417C59490142
      //## begin CMesh::m_pCRelxationLabelling%417C59490142.role preserve=no  public: static CRelxationLabelling { -> RHgAN}
      static CRelxationLabelling *m_pCRelxationLabelling;
      //## end CMesh::m_pCRelxationLabelling%417C59490142.role

    // Additional Public Declarations
      //## begin CMesh%3FCA8CD003E7.public preserve=yes
	  //added by xdh, the number of edge points
	  int      m_nEdgePointsNum;
	  double   m_geometryStretch;
	  DWORD    m_dwRef;  // the counter for undo   
	  int      m_nType;  // the type of mesh: 0---3D; 1---2D;
	  int      m_nEdgeNum;
      //## end CMesh%3FCA8CD003E7.public
  protected:
    // Additional Protected Declarations
      //## begin CMesh%3FCA8CD003E7.protected preserve=yes
      //## end CMesh%3FCA8CD003E7.protected

  private:

    //## Other Operations (specified)
      //## Operation: Distance%3FCF2B92029D
      double Distance (int i, int j);

    // Additional Private Declarations
      //## begin CMesh%3FCA8CD003E7.private preserve=yes
      //## end CMesh%3FCA8CD003E7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CMesh%3FCA8CD003E7.implementation preserve=yes
      //## end CMesh%3FCA8CD003E7.implementation

};

//## begin CMesh%3FCA8CD003E7.postscript preserve=yes
//## end CMesh%3FCA8CD003E7.postscript

// Class CAdjacentMatrixEx 

// Class CMesh 

//## begin module%40F3459802E7.epilog preserve=yes
//## end module%40F3459802E7.epilog


#endif
