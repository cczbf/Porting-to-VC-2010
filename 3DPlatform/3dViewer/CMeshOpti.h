//## begin module%3FCA8CD003E7.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FCA8CD003E7.cm

//## begin module%3FCA8CD003E7.cp preserve=no
//## end module%3FCA8CD003E7.cp

//## Module: CMeshOpti%3FCA8CD003E7; Pseudo Package specification
//## Source file: C:\zbf\Research\Spherical Para\Foundmentals of SP\fsp\CMeshOpti.h

#ifndef CMeshOpti_h
#define CMeshOpti_h 1

//## begin module%3FCA8CD003E7.additionalIncludes preserve=no
//## end module%3FCA8CD003E7.additionalIncludes

//## begin module%3FCA8CD003E7.includes preserve=yes
//## end module%3FCA8CD003E7.includes

// CLw
#include "CLw_.h"
// CTriangleArray
#include "CTriangleArray_.h"
// CVertexArray
#include "CVertexArray_.h"
// CL
#include "CL_.h"
// CL0
#include "CL0_.h"
//## begin module%3FCA8CD003E7.additionalDeclarations preserve=yes
#pragma once
//## end module%3FCA8CD003E7.additionalDeclarations


//## begin CMeshOpti%3FCA8CD003E7.preface preserve=yes
class CMesh;
//## end CMeshOpti%3FCA8CD003E7.preface

//## Class: CMeshOpti%3FCA8CD003E7
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n


class CMeshOpti 
{
public:
	//## begin CMeshOpti%3FCA8CD003E7.initialDeclarations preserve=yes
	  CTriangle_ GetTriangle(int index);
	  CVertex_ GetVertex(int index);
	  int GetTriangleNum();
	  int GetVertexNum();
	  void SetDataFromMesh(const CMesh& mesh);
  //## end CMeshOpti%3FCA8CD003E7.initialDeclarations

  public:
    //## Constructors (generated)
      CMeshOpti();

    //## Destructor (generated)
      ~CMeshOpti();


    //## Other Operations (specified)
      //## Operation: LoadFromPly%3FCA8D0E0168
      BOOL LoadFromPly (CString strFile);

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
      void Normalize ();

      //## Operation: SetOptimizationIterationLimit%3FCE82D700D8
      void SetOptimizationIterationLimit (int nInterateLimit);

      //## Operation: Distance%3FCF4C6E00B8
      double Distance (double x0, double y0, double z0, double x1, double y1, double z1);

      //## Operation: ToSphere%3FCF46110034
      void ToSphere ();

      //## Operation: RemoveIsolateVertex%3FCF542303E0
      void RemoveIsolateVertex ();

    // Data Members for Class Attributes

      //## Attribute: m_nNumOfVertexs%3FCA8CF503B1
      //## begin CMeshOpti::m_nNumOfVertexs%3FCA8CF503B1.attr preserve=no  public: int {UA} 0
      int m_nNumOfVertexs;
      //## end CMeshOpti::m_nNumOfVertexs%3FCA8CF503B1.attr

      //## Attribute: m_nNumOfTriangles%3FCA8CFE0143
      //## begin CMeshOpti::m_nNumOfTriangles%3FCA8CFE0143.attr preserve=no  public: int {UA} 0
      int m_nNumOfTriangles;
      //## end CMeshOpti::m_nNumOfTriangles%3FCA8CFE0143.attr

      //## Attribute: m_MinX%3FCDAC66012C
      //## begin CMeshOpti::m_MinX%3FCDAC66012C.attr preserve=no  public: double {UA} FLT_MAX
      double m_MinX;
      //## end CMeshOpti::m_MinX%3FCDAC66012C.attr

      //## Attribute: m_MinY%3FCDAC7500C4
      //## begin CMeshOpti::m_MinY%3FCDAC7500C4.attr preserve=no  public: double {UA} FLT_MAX
      double m_MinY;
      //## end CMeshOpti::m_MinY%3FCDAC7500C4.attr

      //## Attribute: m_MinZ%3FCDAD160209
      //## begin CMeshOpti::m_MinZ%3FCDAD160209.attr preserve=no  public: double {UA} FLT_MAX
      double m_MinZ;
      //## end CMeshOpti::m_MinZ%3FCDAD160209.attr

      //## Attribute: m_MaxX%3FCDAD4103D2
      //## begin CMeshOpti::m_MaxX%3FCDAD4103D2.attr preserve=no  public: double {UA} FLT_MIN
      double m_MaxX;
      //## end CMeshOpti::m_MaxX%3FCDAD4103D2.attr

      //## Attribute: m_MaxY%3FCDAD560310
      //## begin CMeshOpti::m_MaxY%3FCDAD560310.attr preserve=no  public: double {UA} FLT_MIN
      double m_MaxY;
      //## end CMeshOpti::m_MaxY%3FCDAD560310.attr

      //## Attribute: m_MaxZ%3FCDAD6001C1
      //## begin CMeshOpti::m_MaxZ%3FCDAD6001C1.attr preserve=no  public: double {UA} FLT_MIN
      double m_MaxZ;
      //## end CMeshOpti::m_MaxZ%3FCDAD6001C1.attr

      //## Attribute: m_CenterX%3FCDADDE02D3
      //## begin CMeshOpti::m_CenterX%3FCDADDE02D3.attr preserve=no  public: double {UA} 0.0
      double m_CenterX;
      //## end CMeshOpti::m_CenterX%3FCDADDE02D3.attr

      //## Attribute: m_CenterY%3FCDADF30042
      //## begin CMeshOpti::m_CenterY%3FCDADF30042.attr preserve=no  public: double {UA} 0.0
      double m_CenterY;
      //## end CMeshOpti::m_CenterY%3FCDADF30042.attr

      //## Attribute: m_CenterZ%3FCDADF50050
      //## begin CMeshOpti::m_CenterZ%3FCDADF50050.attr preserve=no  public: double {UA} 0.0
      double m_CenterZ;
      //## end CMeshOpti::m_CenterZ%3FCDADF50050.attr

      //## Attribute: m_nOptIterationLimit%3FCE826B01C6
      //## begin CMeshOpti::m_nOptIterationLimit%3FCE826B01C6.attr preserve=no  public: int {UA} 100
      int m_nOptIterationLimit;
      //## end CMeshOpti::m_nOptIterationLimit%3FCE826B01C6.attr

      //## Attribute: m_dErrorBound%3FCE8A3A0001
      //## begin CMeshOpti::m_dErrorBound%3FCE8A3A0001.attr preserve=no  public: float {UA} (float)0.01
      float m_dErrorBound;
      //## end CMeshOpti::m_dErrorBound%3FCE8A3A0001.attr

      //## Attribute: m_dIncErrorBound%3FCE8A4F00E1
      //## begin CMeshOpti::m_dIncErrorBound%3FCE8A4F00E1.attr preserve=no  public: float {UA} (float)0.0001
      float m_dIncErrorBound;
      //## end CMeshOpti::m_dIncErrorBound%3FCE8A4F00E1.attr

      //## Attribute: m_nOptTarget%3FD56C940196
      //## begin CMeshOpti::m_nOptTarget%3FD56C940196.attr preserve=no  public: int {UA} 0
      int m_nOptTarget;
      //## end CMeshOpti::m_nOptTarget%3FD56C940196.attr

      //## Attribute: m_dW%3FD6A5490263
      //## begin CMeshOpti::m_dW%3FD6A5490263.attr preserve=no  public: float {UA} 1.0
      float m_dW;
      //## end CMeshOpti::m_dW%3FD6A5490263.attr

    // Data Members for Associations

      //## Association: <unnamed>%3FCA8E180000
      //## Role: CMeshOpti::m_CVertexArray%3FCA8E1902A4
      //## begin CMeshOpti::m_CVertexArray%3FCA8E1902A4.role preserve=no  public: CVertexArray { -> VHgAN}
      CVertexArray_ m_CVertexArray;
      //## end CMeshOpti::m_CVertexArray%3FCA8E1902A4.role

      //## Association: <unnamed>%3FCA8E210300
      //## Role: CMeshOpti::m_CTriangleArray%3FCA8E2203E1
      //## begin CMeshOpti::m_CTriangleArray%3FCA8E2203E1.role preserve=no  public: CTriangleArray { -> VHgAN}
      CTriangleArray_ m_CTriangleArray;
      //## end CMeshOpti::m_CTriangleArray%3FCA8E2203E1.role

      //## Association: <unnamed>%3FCAB10C03AB
      //## Role: CMeshOpti::m_CLw%3FCAB10E0025
      //## begin CMeshOpti::m_CLw%3FCAB10E0025.role preserve=no  public: CLw { -> VHgAN}
      CLw_ m_CLw;
      //## end CMeshOpti::m_CLw%3FCAB10E0025.role

      //## Association: <unnamed>%3FD56FB3036D
      //## Role: CMeshOpti::m_CL1%3FD56FB4035B
      //## begin CMeshOpti::m_CL1%3FD56FB4035B.role preserve=no  public: CL0 { -> VHgAN}
      CL0_ m_CL1;
      //## end CMeshOpti::m_CL1%3FD56FB4035B.role

      //## Association: <unnamed>%3FD56FBB0094
      //## Role: CMeshOpti::m_CL0%3FD56FBC0385
      //## begin CMeshOpti::m_CL0%3FD56FBC0385.role preserve=no  public: CL0 { -> VHgAN}
      CL0_ m_CL0;
      //## end CMeshOpti::m_CL0%3FD56FBC0385.role

      //## Association: <unnamed>%3FD5701302AB
      //## Role: CMeshOpti::m_CL%3FD57014027B
      //## begin CMeshOpti::m_CL%3FD57014027B.role preserve=no  public: CL { -> VHgAN}
      CL_ m_CL;
      //## end CMeshOpti::m_CL%3FD57014027B.role

    // Additional Public Declarations
      //## begin CMeshOpti%3FCA8CD003E7.public preserve=yes
      //## end CMeshOpti%3FCA8CD003E7.public

  protected:
    // Additional Protected Declarations
      //## begin CMeshOpti%3FCA8CD003E7.protected preserve=yes
      //## end CMeshOpti%3FCA8CD003E7.protected

  private:

    //## Other Operations (specified)
      //## Operation: Distance%3FCF2B92029D
      double Distance (int i, int j);

    // Additional Private Declarations
      //## begin CMeshOpti%3FCA8CD003E7.private preserve=yes
      //## end CMeshOpti%3FCA8CD003E7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CMeshOpti%3FCA8CD003E7.implementation preserve=yes
      //## end CMeshOpti%3FCA8CD003E7.implementation

};

//## begin CMeshOpti%3FCA8CD003E7.postscript preserve=yes
//## end CMeshOpti%3FCA8CD003E7.postscript

// Class CMeshOpti 

//## begin module%3FCA8CD003E7.epilog preserve=yes
//## end module%3FCA8CD003E7.epilog


#endif
