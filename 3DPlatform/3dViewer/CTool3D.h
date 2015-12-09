//## begin module%40E4BB9B0266.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40E4BB9B0266.cm

//## begin module%40E4BB9B0266.cp preserve=no
//## end module%40E4BB9B0266.cp

//## Module: CTool3D%40E4BB9B0266; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CTool3D.h

#ifndef CTool3D_h
#define CTool3D_h 1

//## begin module%40E4BB9B0266.additionalIncludes preserve=no
//## end module%40E4BB9B0266.additionalIncludes

//## begin module%40E4BB9B0266.includes preserve=yes
//## end module%40E4BB9B0266.includes

// COpenGL
#include "COpenGL.h"
// CToolBase
#include "CToolBase.h"
//## begin module%40E4BB9B0266.declarations preserve=no
//## end module%40E4BB9B0266.declarations

//## begin module%40E4BB9B0266.additionalDeclarations preserve=yes
//## end module%40E4BB9B0266.additionalDeclarations


//## begin CTool3D%40E4B5D1016B.preface preserve=yes
//## end CTool3D%40E4B5D1016B.preface

//## Class: CTool3D%40E4B5D1016B
//## Category: 3dViewer::Tool for 3D%40E4B37A01FF
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CTool3D : public CToolBase  //## Inherits: <unnamed>%40E4BB3000B3
{
  //## begin CTool3D%40E4B5D1016B.initialDeclarations preserve=yes
  //## end CTool3D%40E4B5D1016B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CTool3D%40E4CE3E02F3
      CTool3D ();


    //## Other Operations (specified)
      //## Operation: OnLButtonUp%40E4BB41034D
      BOOL OnLButtonUp (UINT nFlags, CPoint point);

      //## Operation: OnLButtonDown%40E4BB49038A
      BOOL OnLButtonDown (UINT nFlags, CPoint point);

      //## Operation: OnMouseMove%40E4BB50024A
      BOOL OnMouseMove (UINT nFlags, CPoint point);

      //## Operation: OnLBDblClick%40E4CEA00177
      BOOL OnLBDblClick (UINT nFlags, CPoint point);

      //## Operation: set_m_pCOpenGL%40E4CBFF03A0
      void set_m_pCOpenGL (COpenGL* pCOpenGL);

      //## Operation: OnMButtonUp%4178DB320123
      BOOL OnMButtonUp (UINT nFlags, CPoint point);

      //## Operation: OnMButtonDown%4178DBA20020
      BOOL OnMButtonDown (UINT nFlags, CPoint point);

      //## Operation: OnRButtonUp%4178DF230246
      BOOL OnRButtonUp (UINT nFlags, CPoint point);

      //## Operation: OnRButtonDown%4178DF52021C
      BOOL OnRButtonDown (UINT nFlags, CPoint point);

    // Data Members for Associations

      //## Association: 3dViewer::Tool for 3D::<unnamed>%40E4BEBE021B
      //## Role: CTool3D::m_pCOpenGL%40E4BEBF004F
      //## begin CTool3D::m_pCOpenGL%40E4BEBF004F.role preserve=no  public: COpenGL { -> RHgAN}
      COpenGL *m_pCOpenGL;
      //## end CTool3D::m_pCOpenGL%40E4BEBF004F.role

    // Additional Public Declarations
      //## begin CTool3D%40E4B5D1016B.public preserve=yes
      //## end CTool3D%40E4B5D1016B.public

  protected:
    // Additional Protected Declarations
      //## begin CTool3D%40E4B5D1016B.protected preserve=yes
      //## end CTool3D%40E4B5D1016B.protected

  private:
    // Additional Private Declarations
      //## begin CTool3D%40E4B5D1016B.private preserve=yes
      //## end CTool3D%40E4B5D1016B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CTool3D%40E4B5D1016B.implementation preserve=yes
      //## end CTool3D%40E4B5D1016B.implementation

};

//## begin CTool3D%40E4B5D1016B.postscript preserve=yes
//## end CTool3D%40E4B5D1016B.postscript

// Class CTool3D 

//## begin module%40E4BB9B0266.epilog preserve=yes
//## end module%40E4BB9B0266.epilog


#endif
