//## begin module%4175B5850036.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%4175B5850036.cm

//## begin module%4175B5850036.cp preserve=no
//## end module%4175B5850036.cp

//## Module: CRelxationLabelling%4175B5850036; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CRelxationLabelling.h

#ifndef CRelxationLabelling_h
#define CRelxationLabelling_h 1

//## begin module%4175B5850036.additionalIncludes preserve=no
//## end module%4175B5850036.additionalIncludes

//## begin module%4175B5850036.includes preserve=yes
#include <math.h>

class CMesh;
class CMeshOpti;
class CMeshForGeoImg;

#define  USING_CHECK_RECT
//## end module%4175B5850036.includes

// CMyCGLib
#include "CMyCGLib.h"
// CDiffusionVarCo
#include "CDiffusionVarCo.h"
// CEasyProgressBar
#include "CEasyProgressBar.h"
// CArray2DInt
#include "CArray2DInt.h"
// CDIB
#include "CDIB.h"
//## begin module%4175B5850036.declarations preserve=no
//## end module%4175B5850036.declarations

//## begin module%4175B5850036.additionalDeclarations preserve=yes
//## end module%4175B5850036.additionalDeclarations


//## begin CP%416F97FA0370.preface preserve=yes
//## end CP%416F97FA0370.preface

//## Class: CP%416F97FA0370
//	Probability: P(Ai:Lembd)
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CP : public CArray2DFLT  //## Inherits: <unnamed>%416F9A100378
{
  //## begin CP%416F97FA0370.initialDeclarations preserve=yes
  //## end CP%416F97FA0370.initialDeclarations

  public:
    //## Constructors (generated)
      CP();

    // Data Members for Class Attributes

      //## Attribute: m_bInitialized%417DFACA01F3
      //## begin CP::m_bInitialized%417DFACA01F3.attr preserve=no  public: BOOL {UA} 0
      BOOL m_bInitialized;
      //## end CP::m_bInitialized%417DFACA01F3.attr

    // Additional Public Declarations
      //## begin CP%416F97FA0370.public preserve=yes
///////////////




///////////////
      //## end CP%416F97FA0370.public
  protected:
    // Additional Protected Declarations
      //## begin CP%416F97FA0370.protected preserve=yes
      //## end CP%416F97FA0370.protected

  private:
    // Additional Private Declarations
      //## begin CP%416F97FA0370.private preserve=yes
      //## end CP%416F97FA0370.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CP%416F97FA0370.implementation preserve=yes
      //## end CP%416F97FA0370.implementation

};

//## begin CP%416F97FA0370.postscript preserve=yes
//## end CP%416F97FA0370.postscript

//## begin Cr%416F98B4029B.preface preserve=yes
//## end Cr%416F98B4029B.preface

//## Class: Cr%416F98B4029B
//	Correction: r(Ai:lembd_j)
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class Cr : public CP  //## Inherits: <unnamed>%417DFDB70074
{
  //## begin Cr%416F98B4029B.initialDeclarations preserve=yes
  //## end Cr%416F98B4029B.initialDeclarations

  public:
    //## Destructor (generated)
      ~Cr();

    // Additional Public Declarations
      //## begin Cr%416F98B4029B.public preserve=yes
      //## end Cr%416F98B4029B.public

  protected:
    // Additional Protected Declarations
      //## begin Cr%416F98B4029B.protected preserve=yes
      //## end Cr%416F98B4029B.protected

  private:
    // Additional Private Declarations
      //## begin Cr%416F98B4029B.private preserve=yes
      //## end Cr%416F98B4029B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin Cr%416F98B4029B.implementation preserve=yes
      //## end Cr%416F98B4029B.implementation

};

//## begin Cr%416F98B4029B.postscript preserve=yes
//## end Cr%416F98B4029B.postscript

//## begin CNode2D%416F9AA301D5.preface preserve=yes
//## end CNode2D%416F9AA301D5.preface

//## Class: CNode2D%416F9AA301D5
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CNode2D 
{
  //## begin CNode2D%416F9AA301D5.initialDeclarations preserve=yes
  //## end CNode2D%416F9AA301D5.initialDeclarations

  public:
    //## Constructors (generated)
      CNode2D();

    // Data Members for Class Attributes

      //## Attribute: u%416F9AB5023F
      //## begin CNode2D::u%416F9AB5023F.attr preserve=no  public: double {UA} 
      double u;
      //## end CNode2D::u%416F9AB5023F.attr

      //## Attribute: v%416F9ABE0396
      //## begin CNode2D::v%416F9ABE0396.attr preserve=no  public: double {UA} 
      double v;
      //## end CNode2D::v%416F9ABE0396.attr

      //## Attribute: x%41A188690263
      //## begin CNode2D::x%41A188690263.attr preserve=no  public: double {UA} 
      double x;
      //## end CNode2D::x%41A188690263.attr

      //## Attribute: y%41A1887400CE
      //## begin CNode2D::y%41A1887400CE.attr preserve=no  public: double {UA} 
      double y;
      //## end CNode2D::y%41A1887400CE.attr

      //## Attribute: z%41A188780247
      //## begin CNode2D::z%41A188780247.attr preserve=no  public: double {UA} 
      double z;
      //## end CNode2D::z%41A188780247.attr

      //## Attribute: m_nLabelIndex%417CA26B01CA
      //## begin CNode2D::m_nLabelIndex%417CA26B01CA.attr preserve=no  public: int {UA} 
      int m_nLabelIndex;
      //## end CNode2D::m_nLabelIndex%417CA26B01CA.attr

      //## Attribute: m_n3DIndex%4178A85300F5
      //## begin CNode2D::m_n3DIndex%4178A85300F5.attr preserve=no  public: int {UA} 
      int m_n3DIndex;
      //## end CNode2D::m_n3DIndex%4178A85300F5.attr

      //## Attribute: dMin_u%419BED45020D
      //## begin CNode2D::dMin_u%419BED45020D.attr preserve=no  public: double {UA} -FLT_MAX
      double dMin_u;
      //## end CNode2D::dMin_u%419BED45020D.attr

      //## Attribute: dMin_v%419BED7E03DB
      //## begin CNode2D::dMin_v%419BED7E03DB.attr preserve=no  public: double {UA} -FLT_MAX
      double dMin_v;
      //## end CNode2D::dMin_v%419BED7E03DB.attr

      //## Attribute: dMax_u%419BED8F035D
      //## begin CNode2D::dMax_u%419BED8F035D.attr preserve=no  public: double {UA} FLT_MAX
      double dMax_u;
      //## end CNode2D::dMax_u%419BED8F035D.attr

      //## Attribute: dMax_v%419BEDA00037
      //## begin CNode2D::dMax_v%419BEDA00037.attr preserve=no  public: double {UA} FLT_MAX
      double dMax_v;
      //## end CNode2D::dMax_v%419BEDA00037.attr

      //## Attribute: m_bCrossZero%41A013D40384
      //## begin CNode2D::m_bCrossZero%41A013D40384.attr preserve=no  public: BOOL {UA} FALSE
      BOOL m_bCrossZero;
      //## end CNode2D::m_bCrossZero%41A013D40384.attr

      //## Attribute: x0%41A1883A0021
      //## begin CNode2D::x0%41A1883A0021.attr preserve=no  public: double {UA} 
      double x0;
      //## end CNode2D::x0%41A1883A0021.attr

      //## Attribute: x1%41A188440198
      //## begin CNode2D::x1%41A188440198.attr preserve=no  public: double {UA} 
      double x1;
      //## end CNode2D::x1%41A188440198.attr

      //## Attribute: y0%41A1885000F5
      //## begin CNode2D::y0%41A1885000F5.attr preserve=no  public: double {UA} 
      double y0;
      //## end CNode2D::y0%41A1885000F5.attr

      //## Attribute: y1%41A188560099
      //## begin CNode2D::y1%41A188560099.attr preserve=no  public: double {UA} 
      double y1;
      //## end CNode2D::y1%41A188560099.attr

      //## Attribute: z0%41A188590300
      //## begin CNode2D::z0%41A188590300.attr preserve=no  public: double {UA} 
      double z0;
      //## end CNode2D::z0%41A188590300.attr

      //## Attribute: z1%41A1885E0127
      //## begin CNode2D::z1%41A1885E0127.attr preserve=no  public: double {UA} 
      double z1;
      //## end CNode2D::z1%41A1885E0127.attr

    // Additional Public Declarations
      //## begin CNode2D%416F9AA301D5.public preserve=yes
//      int m_n3DIndex;

      //## end CNode2D%416F9AA301D5.public
  protected:
    // Additional Protected Declarations
      //## begin CNode2D%416F9AA301D5.protected preserve=yes
      //## end CNode2D%416F9AA301D5.protected

  private:
    // Additional Private Declarations
      //## begin CNode2D%416F9AA301D5.private preserve=yes
      //## end CNode2D%416F9AA301D5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CNode2D%416F9AA301D5.implementation preserve=yes
      //## end CNode2D%416F9AA301D5.implementation

};

//## begin CNode2D%416F9AA301D5.postscript preserve=yes
//## end CNode2D%416F9AA301D5.postscript

//## begin CLambda%416F9CDD03A1.preface preserve=yes
//## end CLambda%416F9CDD03A1.preface

//## Class: CLambda%416F9CDD03A1
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CLambda 
{
  //## begin CLambda%416F9CDD03A1.initialDeclarations preserve=yes
  //## end CLambda%416F9CDD03A1.initialDeclarations

  public:
    // Data Members for Class Attributes

      //## Attribute: u%416F9CEB031F
      //	or theta in spherical coord.
      //## begin CLambda::u%416F9CEB031F.attr preserve=no  public: double {UA} 
      double u;
      //## end CLambda::u%416F9CEB031F.attr

      //## Attribute: v%416F9CF40083
      //	or fai in spherical coord.
      //## begin CLambda::v%416F9CF40083.attr preserve=no  public: double {UA} 
      double v;
      //## end CLambda::v%416F9CF40083.attr

      //## Attribute: sin_u%4196CFAC01A1
      //## begin CLambda::sin_u%4196CFAC01A1.attr preserve=no  public: double {UA} 
      double sin_u;
      //## end CLambda::sin_u%4196CFAC01A1.attr

      //## Attribute: sin_v%419868DC02C2
      //## begin CLambda::sin_v%419868DC02C2.attr preserve=no  public: double {UA} 
      double sin_v;
      //## end CLambda::sin_v%419868DC02C2.attr

      //## Attribute: cos_u%419868F703A7
      //## begin CLambda::cos_u%419868F703A7.attr preserve=no  public: double {UA} 
      double cos_u;
      //## end CLambda::cos_u%419868F703A7.attr

      //## Attribute: cos_v%419869090245
      //## begin CLambda::cos_v%419869090245.attr preserve=no  public: double {UA} 
      double cos_v;
      //## end CLambda::cos_v%419869090245.attr

      //## Attribute: x%41A188260325
      //## begin CLambda::x%41A188260325.attr preserve=no  public: double {UA} 
      double x;
      //## end CLambda::x%41A188260325.attr

      //## Attribute: y%41A1882D009A
      //## begin CLambda::y%41A1882D009A.attr preserve=no  public: double {UA} 
      double y;
      //## end CLambda::y%41A1882D009A.attr

      //## Attribute: z%41A1883100E6
      //## begin CLambda::z%41A1883100E6.attr preserve=no  public: double {UA} 
      double z;
      //## end CLambda::z%41A1883100E6.attr

    // Additional Public Declarations
      //## begin CLambda%416F9CDD03A1.public preserve=yes
      //## end CLambda%416F9CDD03A1.public

  protected:
    // Additional Protected Declarations
      //## begin CLambda%416F9CDD03A1.protected preserve=yes
      //## end CLambda%416F9CDD03A1.protected

  private:
    // Additional Private Declarations
      //## begin CLambda%416F9CDD03A1.private preserve=yes
      //## end CLambda%416F9CDD03A1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CLambda%416F9CDD03A1.implementation preserve=yes
      //## end CLambda%416F9CDD03A1.implementation

};

//## begin CLambda%416F9CDD03A1.postscript preserve=yes
//## end CLambda%416F9CDD03A1.postscript

//## begin CNode3D%4178A7FD0065.preface preserve=yes
//## end CNode3D%4178A7FD0065.preface

//## Class: CNode3D%4178A7FD0065
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CNode3D 
{
  //## begin CNode3D%4178A7FD0065.initialDeclarations preserve=yes
  //## end CNode3D%4178A7FD0065.initialDeclarations

  public:
    // Data Members for Class Attributes

      //## Attribute: x%4178A8320396
      //## begin CNode3D::x%4178A8320396.attr preserve=no  public: double {UA} 
      double x;
      //## end CNode3D::x%4178A8320396.attr

      //## Attribute: y%4178A83E03A8
      //## begin CNode3D::y%4178A83E03A8.attr preserve=no  public: double {UA} 
      double y;
      //## end CNode3D::y%4178A83E03A8.attr

      //## Attribute: z%4178A84A016A
      //## begin CNode3D::z%4178A84A016A.attr preserve=no  public: double {UA} 
      double z;
      //## end CNode3D::z%4178A84A016A.attr

    // Additional Public Declarations
      //## begin CNode3D%4178A7FD0065.public preserve=yes
      //## end CNode3D%4178A7FD0065.public

  protected:
    // Additional Protected Declarations
      //## begin CNode3D%4178A7FD0065.protected preserve=yes
      //## end CNode3D%4178A7FD0065.protected

  private:
    // Additional Private Declarations
      //## begin CNode3D%4178A7FD0065.private preserve=yes
      //## end CNode3D%4178A7FD0065.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CNode3D%4178A7FD0065.implementation preserve=yes
      //## end CNode3D%4178A7FD0065.implementation

};

//## begin CNode3D%4178A7FD0065.postscript preserve=yes
//## end CNode3D%4178A7FD0065.postscript

//## begin CAdjancentMtrx%417C618C000D.preface preserve=yes
//## end CAdjancentMtrx%417C618C000D.preface

//## Class: CAdjancentMtrx%417C618C000D
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CAdjancentMtrx : public CArray2DFLT  //## Inherits: <unnamed>%417C61B5032D
{
  //## begin CAdjancentMtrx%417C618C000D.initialDeclarations preserve=yes
  //## end CAdjancentMtrx%417C618C000D.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin CAdjancentMtrx%417C618C000D.public preserve=yes
      //## end CAdjancentMtrx%417C618C000D.public

  protected:
    // Additional Protected Declarations
      //## begin CAdjancentMtrx%417C618C000D.protected preserve=yes
      //## end CAdjancentMtrx%417C618C000D.protected

  private:
    // Additional Private Declarations
      //## begin CAdjancentMtrx%417C618C000D.private preserve=yes
      //## end CAdjancentMtrx%417C618C000D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CAdjancentMtrx%417C618C000D.implementation preserve=yes
      //## end CAdjancentMtrx%417C618C000D.implementation

};

//## begin CAdjancentMtrx%417C618C000D.postscript preserve=yes
//## end CAdjancentMtrx%417C618C000D.postscript

//## begin CEdge4RL%417DF1B00384.preface preserve=yes
//## end CEdge4RL%417DF1B00384.preface

//## Class: CEdge4RL%417DF1B00384
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CEdge4RL 
{
  //## begin CEdge4RL%417DF1B00384.initialDeclarations preserve=yes
  //## end CEdge4RL%417DF1B00384.initialDeclarations

  public:
    // Data Members for Class Attributes

      //## Attribute: v1%417DF1CE00B6
      //## begin CEdge4RL::v1%417DF1CE00B6.attr preserve=no  public: int {UA} 
      int v1;
      //## end CEdge4RL::v1%417DF1CE00B6.attr

      //## Attribute: v2%417DF1D90134
      //## begin CEdge4RL::v2%417DF1D90134.attr preserve=no  public: int {UA} 
      int v2;
      //## end CEdge4RL::v2%417DF1D90134.attr

      //## Attribute: dStretchMatrics%417DF1E60291
      //## begin CEdge4RL::dStretchMatrics%417DF1E60291.attr preserve=no  public: double {UA} 
      double dStretchMatrics;
      //## end CEdge4RL::dStretchMatrics%417DF1E60291.attr

    // Additional Public Declarations
      //## begin CEdge4RL%417DF1B00384.public preserve=yes
      //## end CEdge4RL%417DF1B00384.public

  protected:
    // Additional Protected Declarations
      //## begin CEdge4RL%417DF1B00384.protected preserve=yes
      //## end CEdge4RL%417DF1B00384.protected

  private:
    // Additional Private Declarations
      //## begin CEdge4RL%417DF1B00384.private preserve=yes
      //## end CEdge4RL%417DF1B00384.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CEdge4RL%417DF1B00384.implementation preserve=yes
      //## end CEdge4RL%417DF1B00384.implementation

};

//## begin CEdge4RL%417DF1B00384.postscript preserve=yes
//## end CEdge4RL%417DF1B00384.postscript

//## begin CQ%4185CC9301DB.preface preserve=yes
//## end CQ%4185CC9301DB.preface

//## Class: CQ%4185CC9301DB
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CQ : public CP  //## Inherits: <unnamed>%4185CC9E015F
{
  //## begin CQ%4185CC9301DB.initialDeclarations preserve=yes
  //## end CQ%4185CC9301DB.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin CQ%4185CC9301DB.public preserve=yes
      //## end CQ%4185CC9301DB.public

  protected:
    // Additional Protected Declarations
      //## begin CQ%4185CC9301DB.protected preserve=yes
      //## end CQ%4185CC9301DB.protected

  private:
    // Additional Private Declarations
      //## begin CQ%4185CC9301DB.private preserve=yes
      //## end CQ%4185CC9301DB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CQ%4185CC9301DB.implementation preserve=yes
      //## end CQ%4185CC9301DB.implementation

};

//## begin CQ%4185CC9301DB.postscript preserve=yes
//## end CQ%4185CC9301DB.postscript

//## begin CTrinagle%419AE564015E.preface preserve=yes
//## end CTrinagle%419AE564015E.preface

//## Class: CTrinagle%419AE564015E
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct CTrinagle 
{
  //## begin CTrinagle%419AE564015E.initialDeclarations preserve=yes
  //## end CTrinagle%419AE564015E.initialDeclarations

    // Data Members for Class Attributes

      //## Attribute: A%419AE57B0003
      //## begin CTrinagle::A%419AE57B0003.attr preserve=no  public: int {UA} 
      int A;
      //## end CTrinagle::A%419AE57B0003.attr

      //## Attribute: B%419AE5820388
      //## begin CTrinagle::B%419AE5820388.attr preserve=no  public: int {UA} 
      int B;
      //## end CTrinagle::B%419AE5820388.attr

      //## Attribute: C%419AE5850011
      //## begin CTrinagle::C%419AE5850011.attr preserve=no  public: int {UA} 
      int C;
      //## end CTrinagle::C%419AE5850011.attr

  public:
    // Additional Public Declarations
      //## begin CTrinagle%419AE564015E.public preserve=yes
      //## end CTrinagle%419AE564015E.public

  protected:
    // Additional Protected Declarations
      //## begin CTrinagle%419AE564015E.protected preserve=yes
      //## end CTrinagle%419AE564015E.protected

  private:
    // Additional Private Declarations
      //## begin CTrinagle%419AE564015E.private preserve=yes
      //## end CTrinagle%419AE564015E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CTrinagle%419AE564015E.implementation preserve=yes
      //## end CTrinagle%419AE564015E.implementation

};

//## begin CTrinagle%419AE564015E.postscript preserve=yes
//## end CTrinagle%419AE564015E.postscript

//## begin CTriangleRL%419AEA210162.preface preserve=yes
//## end CTriangleRL%419AEA210162.preface

//## Class: CTriangleRL%419AEA210162
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct CTriangleRL 
{
  //## begin CTriangleRL%419AEA210162.initialDeclarations preserve=yes
  //## end CTriangleRL%419AEA210162.initialDeclarations

    // Data Members for Class Attributes

      //## Attribute: A%419AEB420082
      //## begin CTriangleRL::A%419AEB420082.attr preserve=no  public: int {UA} 
      int A;
      //## end CTriangleRL::A%419AEB420082.attr

      //## Attribute: B%419AEB5600A8
      //## begin CTriangleRL::B%419AEB5600A8.attr preserve=no  public: int {UA} 
      int B;
      //## end CTriangleRL::B%419AEB5600A8.attr

      //## Attribute: C%419AEB5B020E
      //## begin CTriangleRL::C%419AEB5B020E.attr preserve=no  public: int {UA} 
      int C;
      //## end CTriangleRL::C%419AEB5B020E.attr

  public:
    // Additional Public Declarations
      //## begin CTriangleRL%419AEA210162.public preserve=yes
      //## end CTriangleRL%419AEA210162.public

  protected:
    // Additional Protected Declarations
      //## begin CTriangleRL%419AEA210162.protected preserve=yes
      //## end CTriangleRL%419AEA210162.protected

  private:
    // Additional Private Declarations
      //## begin CTriangleRL%419AEA210162.private preserve=yes
      //## end CTriangleRL%419AEA210162.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CTriangleRL%419AEA210162.implementation preserve=yes
      //## end CTriangleRL%419AEA210162.implementation

};

//## begin CTriangleRL%419AEA210162.postscript preserve=yes
//## end CTriangleRL%419AEA210162.postscript

//## begin CRelxationLabelling%416F95E400E7.preface preserve=yes
//## end CRelxationLabelling%416F95E400E7.preface

//## Class: CRelxationLabelling%416F95E400E7
//## Category: Relaxation Labelling%416F952701CC
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4185A31D00BC;CEasyProgressBar { -> }
//## Uses: <unnamed>%4199736601AB;CDiffusionVarCo { -> }
//## Uses: <unnamed>%419973EF0068;CDib { -> }

class CRelxationLabelling 
{
  //## begin CRelxationLabelling%416F95E400E7.initialDeclarations preserve=yes
//  void SetMesh();
//	void SetBorderLen(int nBorderLen);
public:
	float  ComputeGeoStretch();
    double GetGeometryStretch() const;
	void   Compute3DStatisticOfRation();

	//set data from mesh
    void  SetDataFromGeoimg( CMeshForGeoImg &geoMesh );
	void  SetDataFromMesh( CMesh &mesh);
    void  SetDataFromOpti( int nSum, float *x = NULL , float *y = NULL, float *z = NULL );
    void  SetDataFromOpti( CMeshOpti &optimesh);
	void  SetEdge();	 

	//save the current result
	void  Save2DMesh(CString filename);

	//initialize the 2D Mesh Node box
    //find the Check Rectangle for 2D Mesh
	void  SetNode2DLabelBox1(int ai);
	
  //## end CRelxationLabelling%416F95E400E7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CRelxationLabelling%418197CC0270
      CRelxationLabelling ();

    //## Destructor (specified)
      //## Operation: ~CRelxationLabelling%418197F003C6
      ~CRelxationLabelling ();


    //## Other Operations (specified)
      //## Operation: f%418197E001BA
      virtual double f (int i, int li, int j, int lj);

      //## Operation: f0%41986FC1003F
      double f0 (int i, int li, int j, int lj);

      //## Operation: f1%41986FD9027F
      virtual double f1 (int i, int li, int j, int lj);

      //## Operation: P%417CA4C0007E
      double P (int ai, int li);

      //## Operation: r%4174C70902E0
      //	Corrections: r(ai:lambdaj)
      double r (int ai, int li);

      //## Operation: r%4174CD35011F
      //	Comapbility of two labelings:r(ai:Lj,ak:Ll)
      virtual double r (int ai, int Li, int ak, int Lk);

      //## Operation: q%417CB73002C0
      double q (int aj, int ai, int Li);

      //## Operation: Q%417CBBE601B5
      double Q (int ai, int Li);

      //## Operation: Fai%417CC46D004C
      double Fai ();

      //## Operation: C%4189D6220031
      double C (int i, int j);

      //## Operation: Iterating%417DEA7B020E
      void Iterating (int nLoop);

      //## Operation: Get3DPoint%417DEABC028A
      CNode3D Get3DPoint (int index);

      //## Operation: Stretch%417DF214008E
      double Stretch (int nV1Index, int nV2Index);

      //## Operation: Stretch%417DF299034D
      double Stretch (double e2D, double e3D);

      //## Operation: SetEdgeList%4180B98E016D
      void SetEdgeList ();

      //## Operation: GetEdge%4180B9FF02EC
      CEdge4RL GetEdge (int index);

      //## Operation: ComputeSampleGap%4180BA710372
      void ComputeSampleGap ();

      //## Operation: ComputeStatisticOfRatio%4180BAA10212
      void ComputeStatisticOfRatio ();

      //## Operation: GetAvarageOfRatio%4180BAEE034A
      double GetAvarageOfRatio () const;

      //## Operation: GetCovOfRatio%4180BB27032E
      double GetCovOfRatio () const;

      //## Operation: Load2DMesh%4180BDBB03A5
      void Load2DMesh (CString fileName);

      //## Operation: SetLabelList%4181B5E40065
      void SetLabelList (int w, int h, BOOL bNodeAtLabel = TRUE);

      //## Operation: SetLabelList0%41996A320013
      void SetLabelList0 (int w, int h);

      //## Operation: SetLabelList1%41996F2200A6
      void SetLabelList1 (int w, int h, double dDensity);

      //## Operation: GetLabel%4181B62003C8
      CLambda GetLabel (int nIndex);

      //## Operation: SetTriangleVertexIndex%419AE6960046
      //	//set the index of every triangle
      //	//Author: XDH
      void SetTriangleVertexIndex (int a, int b, int c, int index);

      //## Operation: UpdateCR%4181FA2303B5
      void UpdateCR ();

      //## Operation: UpDate_P%418208CE01D3
      void UpDate_P (double dblFai);

      //## Operation: MaxPossibleLable%4181FD750050
      int MaxPossibleLable (int ai, double* dblProbability = NULL);

      //## Operation: SetNode2DLabelings%4182011A0278
      int SetNode2DLabelings ();

      //## Operation: GetMesh%41873CAF0277
      void GetMesh (int* nTriangle, int** nV1, int** nV2, int** nV3, int** c12, int** c23, int** c31, int* nVlength, double** x, double** y, double** z);

      //## Operation: SetMesh%41884AB1011C
      void SetMesh ();

      //## Operation: SetMesh0%4199A33A0321
      void SetMesh0 ();

      //## Operation: SetMesh1%4199A34503D1
      void SetMesh1 ();

      //## Operation: IsBorderNode%4189EAC90123
      bool IsBorderNode (int ai);

      //## Operation: SetBorderLen%418ACDAB033D
      void SetBorderLen (int nBorderLen);

      //## Operation: SphereJacobianMap%41945FA403CE
      HANDLE SphereJacobianMap (int* pW, int* pH, double dScale, int* nBorderW, int* nBorderH);

      //## Operation: SetLabelFromSphericalRaw%4196C08E025C
      void SetLabelFromSphericalRaw (HANDLE hRaw, int w, int h, int* nPtCount = NULL, double** px = NULL, double** py = NULL, double** pz = NULL);

      //## Operation: SetLabelFromEvenDistribution%41A2AF700119
      void SetLabelFromEvenDistribution (int w, int h, double dSampleDensity = 1.0, int* nPtCount = NULL, double** px = NULL, double** py = NULL, double** pz = NULL);

      //## Operation: SetTriangleNum%419AEC1C0193
      void SetTriangleNum (int nNum);

      //## Operation: InitNode2D4Sphere%4199C7F802F4
      void InitNode2D4Sphere ();

      //## Operation: GeodesicDistance%419AAE050104
      //	//set the number of triangles
      //	//Author: XDH
      double GeodesicDistance (double th1, double f1, double th2, double f2);

      //## Operation: SetNode2DLabelBox%419BED080327
      void SetNode2DLabelBox (int ai);

      //## Operation: IsLabelInNodeLabelBox%419BF90700B6
      BOOL IsLabelInNodeLabelBox (int ai, int li);

      //## Operation: SetAdjacentMatrixEdge%4180BDDC00C7
      void SetAdjacentMatrixEdge (int v1, int v2, int v3);

      //## Operation: SetAdjacentMatrixDim%4180BE0C0289
      void SetAdjacentMatrixDim (int row, int col);

      //## Operation: Set2DPoint%4180BE2C00EA
      void Set2DPoint (CNode2D p, int index);

      //## Operation: Set3DPoint%4180BE4A031E
      void Set3DPoint (CNode3D p, int index);

      //## Operation: Set2DPointNum%4180BE77014C
      void Set2DPointNum (int num);

      //## Operation: Set3DPointNum%4180BE8D0252
      void Set3DPointNum (int num);

    // Data Members for Class Attributes

      //## Attribute: m_nBorderlength%4178A9E30295
      //## begin CRelxationLabelling::m_nBorderlength%4178A9E30295.attr preserve=no  public: int {UA} 0
      int m_nBorderlength;
      //## end CRelxationLabelling::m_nBorderlength%4178A9E30295.attr

      //## Attribute: m_n2DNode_length%4178AA020249
      //## begin CRelxationLabelling::m_n2DNode_length%4178AA020249.attr preserve=no  public: int {UA} 0
      int m_n2DNode_length;
      //## end CRelxationLabelling::m_n2DNode_length%4178AA020249.attr

      //## Attribute: m_n3DNode_length%4178AA1503B9
      //## begin CRelxationLabelling::m_n3DNode_length%4178AA1503B9.attr preserve=no  public: int {UA} 0
      int m_n3DNode_length;
      //## end CRelxationLabelling::m_n3DNode_length%4178AA1503B9.attr

      //## Attribute: m_nLabelList_length%417CA57A032E
      //## begin CRelxationLabelling::m_nLabelList_length%417CA57A032E.attr preserve=no  public: int {UA} 0
      int m_nLabelList_length;
      //## end CRelxationLabelling::m_nLabelList_length%417CA57A032E.attr

      //## Attribute: m_nLabelListW%4181967F0158
      //## begin CRelxationLabelling::m_nLabelListW%4181967F0158.attr preserve=no  public: int {UA} 32
      int m_nLabelListW;
      //## end CRelxationLabelling::m_nLabelListW%4181967F0158.attr

      //## Attribute: m_nLabelListH%41819692026E
      //## begin CRelxationLabelling::m_nLabelListH%41819692026E.attr preserve=no  public: int {UA} 32
      int m_nLabelListH;
      //## end CRelxationLabelling::m_nLabelListH%41819692026E.attr

      //## Attribute: m_nEdgeList_length%417DF3E70022
      //## begin CRelxationLabelling::m_nEdgeList_length%417DF3E70022.attr preserve=no  public: int {UA} 0
      int m_nEdgeList_length;
      //## end CRelxationLabelling::m_nEdgeList_length%417DF3E70022.attr

      //## Attribute: m_dSampleGap%417E16A801C0
      //## begin CRelxationLabelling::m_dSampleGap%417E16A801C0.attr preserve=no  public: double {UA} 0.0
      double m_dSampleGap;
      //## end CRelxationLabelling::m_dSampleGap%417E16A801C0.attr

      //## Attribute: m_bFirst%418DE0210219
      //## begin CRelxationLabelling::m_bFirst%418DE0210219.attr preserve=no  public: bool {UA} TRUE
      bool m_bFirst;
      //## end CRelxationLabelling::m_bFirst%418DE0210219.attr

      //## Attribute: m_bIsForSpherical%41986FE5034E
      //## begin CRelxationLabelling::m_bIsForSpherical%41986FE5034E.attr preserve=no  public: BOOL {UA} FALSE
      BOOL m_bIsForSpherical;
      //## end CRelxationLabelling::m_bIsForSpherical%41986FE5034E.attr

      //## Attribute: m_dSampleingDensity%4199727F0054
      //## begin CRelxationLabelling::m_dSampleingDensity%4199727F0054.attr preserve=no  public: double {UA} 0.399
      double m_dSampleingDensity;
      //## end CRelxationLabelling::m_dSampleingDensity%4199727F0054.attr

      //## Attribute: m_nSphericalSampling%41A2BED10029
      //	1-- Error diffusion spherical
      //	2-- Even Distriubtion spherical
      //## begin CRelxationLabelling::m_nSphericalSampling%41A2BED10029.attr preserve=no  public: int {UA} 1
      int m_nSphericalSampling;
      //## end CRelxationLabelling::m_nSphericalSampling%41A2BED10029.attr

      //## Attribute: m_bQuickIterating%41E762FC016D
      //## begin CRelxationLabelling::m_bQuickIterating%41E762FC016D.attr preserve=no  public: bool {UA} FALSE
      bool m_bQuickIterating;
      //## end CRelxationLabelling::m_bQuickIterating%41E762FC016D.attr

    // Data Members for Associations

      //## Association: Relaxation Labelling::<unnamed>%416F98F7039C
      //## Role: CRelxationLabelling::m_pCProbability%416F98F801A9
      //## begin CRelxationLabelling::m_pCProbability%416F98F801A9.role preserve=no  public: CP { -> VHgAN}
      CP m_pCProbability;
      //## end CRelxationLabelling::m_pCProbability%416F98F801A9.role

      //## Association: Relaxation Labelling::<unnamed>%416F98FA01DE
      //## Role: CRelxationLabelling::m_pCR%416F98FB0009
      //## begin CRelxationLabelling::m_pCR%416F98FB0009.role preserve=no  public: Cr { -> VHgAN}
      Cr m_pCR;
      //## end CRelxationLabelling::m_pCR%416F98FB0009.role

      //## Association: Relaxation Labelling::<unnamed>%416F9ACD030C
      //## Role: CRelxationLabelling::m_pCNode2D%416F9ACE026D
      //## begin CRelxationLabelling::m_pCNode2D%416F9ACE026D.role preserve=no  public: CNode2D { -> RHgAN}
      CNode2D *m_pCNode2D;
      //## end CRelxationLabelling::m_pCNode2D%416F9ACE026D.role

      //## Association: Relaxation Labelling::<unnamed>%416F9CFF01B5
      //## Role: CRelxationLabelling::m_pCLabelList%416F9D0000F9
      //## begin CRelxationLabelling::m_pCLabelList%416F9D0000F9.role preserve=no  public: CLambda { -> RHgAN}
      CLambda *m_pCLabelList;
      //## end CRelxationLabelling::m_pCLabelList%416F9D0000F9.role

      //## Association: Relaxation Labelling::<unnamed>%4178A80B001F
      //## Role: CRelxationLabelling::m_pCNode3D%4178A80B0336
      //## begin CRelxationLabelling::m_pCNode3D%4178A80B0336.role preserve=no  public: CNode3D { -> RHgAN}
      CNode3D *m_pCNode3D;
      //## end CRelxationLabelling::m_pCNode3D%4178A80B0336.role

      //## Association: Relaxation Labelling::<unnamed>%417C5EF000A2
      //## Role: CRelxationLabelling::m_pCArray2DFLTAdjancentMtrx%417C5EF002E7
      //## begin CRelxationLabelling::m_pCArray2DFLTAdjancentMtrx%417C5EF002E7.role preserve=no  public: CArray2DFLT { -> RHgAN}
      CArray2DFLT *m_pCArray2DFLTAdjancentMtrx;
      //## end CRelxationLabelling::m_pCArray2DFLTAdjancentMtrx%417C5EF002E7.role

      //## Association: Relaxation Labelling::<unnamed>%417C61B8007E
      //## Role: CRelxationLabelling::m_pCAdjancentMtrx0%417C61B8007F
      //## begin CRelxationLabelling::m_pCAdjancentMtrx0%417C61B8007F.role preserve=no  public: CAdjancentMtrx { -> VHgAN}
      CAdjancentMtrx m_pCAdjancentMtrx0;
      //## end CRelxationLabelling::m_pCAdjancentMtrx0%417C61B8007F.role

      //## Association: Relaxation Labelling::<unnamed>%417DF3C1024E
      //## Role: CRelxationLabelling::m_pEdgeList%417DF3C2019B
      //## begin CRelxationLabelling::m_pEdgeList%417DF3C2019B.role preserve=no  public: CEdge4RL { -> RHgAN}
      CEdge4RL *m_pEdgeList;
      //## end CRelxationLabelling::m_pEdgeList%417DF3C2019B.role

      //## Association: Relaxation Labelling::<unnamed>%4185CCA20100
      //## Role: CRelxationLabelling::m_CQ%4185CCA301F2
      //## begin CRelxationLabelling::m_CQ%4185CCA301F2.role preserve=no  public: CQ { -> VHgAN}
      CQ m_CQ;
      //## end CRelxationLabelling::m_CQ%4185CCA301F2.role

      //## Association: Relaxation Labelling::<unnamed>%419AEA42039B
      //## Role: CRelxationLabelling::m_pCTriangle%419AEA430248
      //## begin CRelxationLabelling::m_pCTriangle%419AEA430248.role preserve=no  public: CTriangleRL { -> RHgAN}
      CTriangleRL *m_pCTriangle;
      //## end CRelxationLabelling::m_pCTriangle%419AEA430248.role

    // Additional Public Declarations
      //## begin CRelxationLabelling%416F95E400E7.public preserve=yes
      //## end CRelxationLabelling%416F95E400E7.public

  protected:

    //## Other Operations (specified)
      //## Operation: P0%4174C67A0226
      //	Calculating initial probability P0
      virtual double P0 (int ai, int li);

    // Data Members for Class Attributes

      //## Attribute: m_dTensionFactor%41DCA385010E
      //	Used only in function  f1()
      //## begin CRelxationLabelling::m_dTensionFactor%41DCA385010E.attr preserve=no  protected: double {UA} 0.2
      double m_dTensionFactor;
      //## end CRelxationLabelling::m_dTensionFactor%41DCA385010E.attr

    // Additional Protected Declarations
      //## begin CRelxationLabelling%416F95E400E7.protected preserve=yes
      //## end CRelxationLabelling%416F95E400E7.protected

  private:

    //## Other Operations (specified)
      //## Operation: Initial_P%4181E8FA024F
      void Initial_P ();

    // Data Members for Class Attributes

      //## Attribute: m_dfAvarageOfRatio%4180B8460284
      //## begin CRelxationLabelling::m_dfAvarageOfRatio%4180B8460284.attr preserve=no  private: double {UA} 0.0
      double m_dfAvarageOfRatio;
      //## end CRelxationLabelling::m_dfAvarageOfRatio%4180B8460284.attr

      //## Attribute: m_dfConvOfRatio%4180B873027E
      //## begin CRelxationLabelling::m_dfConvOfRatio%4180B873027E.attr preserve=no  private: double {UA} 0.0
      double m_dfConvOfRatio;
      //## end CRelxationLabelling::m_dfConvOfRatio%4180B873027E.attr

    // Additional Private Declarations
      //## begin CRelxationLabelling%416F95E400E7.private preserve=yes
	  int    m_nTriangleNum;
	  double m_geometryStretch;
	  int    m_nModeType; //0---2D Mesh; 1---3D Mesh

      //## end CRelxationLabelling%416F95E400E7.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin CRelxationLabelling%416F95E400E7.implementation preserve=yes
      //## end CRelxationLabelling%416F95E400E7.implementation

};

//## begin CRelxationLabelling%416F95E400E7.postscript preserve=yes
//## end CRelxationLabelling%416F95E400E7.postscript

//## begin CRelxationLabelling1%41E36A620177.preface preserve=yes
//## end CRelxationLabelling1%41E36A620177.preface

//## Class: CRelxationLabelling1%41E36A620177
//## Category: Relaxation Labelling::RL1%41E36949039A
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%41E37BED0215;CMyCGLib { -> }

class CRelxationLabelling1 : public CRelxationLabelling  //## Inherits: <unnamed>%41E36A6A02EB
{
  //## begin CRelxationLabelling1%41E36A620177.initialDeclarations preserve=yes
  //## end CRelxationLabelling1%41E36A620177.initialDeclarations

  public:
    //## Constructors (generated)
      CRelxationLabelling1();

    //## Destructor (generated)
      ~CRelxationLabelling1();


    //## Other Operations (specified)
      //## Operation: f1%41E36D71010F
      void f1 (int i, int li, int j, int lj, double* f1_x, double* f1_y, double* f1_z);

    // Additional Public Declarations
      //## begin CRelxationLabelling1%41E36A620177.public preserve=yes
      //## end CRelxationLabelling1%41E36A620177.public

  protected:

    //## Other Operations (specified)
      //## Operation: P0%41E36ECC012C
      double P0 (int ai, int li);

    // Additional Protected Declarations
      //## begin CRelxationLabelling1%41E36A620177.protected preserve=yes
      //## end CRelxationLabelling1%41E36A620177.protected

  private:
    // Additional Private Declarations
      //## begin CRelxationLabelling1%41E36A620177.private preserve=yes
      //## end CRelxationLabelling1%41E36A620177.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CRelxationLabelling1%41E36A620177.implementation preserve=yes
      //## end CRelxationLabelling1%41E36A620177.implementation

};

//## begin CRelxationLabelling1%41E36A620177.postscript preserve=yes
//## end CRelxationLabelling1%41E36A620177.postscript

//## begin CRelxationLabelling2%41E4A0E2004C.preface preserve=yes
//## end CRelxationLabelling2%41E4A0E2004C.preface

//## Class: CRelxationLabelling2%41E4A0E2004C
//## Category: Relaxation Labelling::RL1%41E36949039A
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CRelxationLabelling2 : public CRelxationLabelling1  //## Inherits: <unnamed>%41E4A0F60069
{
  //## begin CRelxationLabelling2%41E4A0E2004C.initialDeclarations preserve=yes
  //## end CRelxationLabelling2%41E4A0E2004C.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: r%41E4A11E027A
      double r (int ai, int Li, int ak, int Lk);

      //## Operation: f_act%41E4A12702D7
      double f_act (int ai, int li, int aj, int lj);

      //## Operation: f%41E4A13202F0
      double f (int i, int li, int j, int lj);

    // Additional Public Declarations
      //## begin CRelxationLabelling2%41E4A0E2004C.public preserve=yes
      //## end CRelxationLabelling2%41E4A0E2004C.public

  protected:
    // Additional Protected Declarations
      //## begin CRelxationLabelling2%41E4A0E2004C.protected preserve=yes
      //## end CRelxationLabelling2%41E4A0E2004C.protected

  private:
    // Additional Private Declarations
      //## begin CRelxationLabelling2%41E4A0E2004C.private preserve=yes
      //## end CRelxationLabelling2%41E4A0E2004C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CRelxationLabelling2%41E4A0E2004C.implementation preserve=yes
      //## end CRelxationLabelling2%41E4A0E2004C.implementation

};

//## begin CRelxationLabelling2%41E4A0E2004C.postscript preserve=yes
//## end CRelxationLabelling2%41E4A0E2004C.postscript

// Class CP 

// Class Cr 

// Class CNode2D 

// Class CLambda 

// Class CNode3D 

// Class CAdjancentMtrx 

// Class CEdge4RL 

// Class CQ 

// Class CTrinagle 

// Class CTriangleRL 

// Class CRelxationLabelling 


//## Other Operations (inline)
inline double CRelxationLabelling::f (int i, int li, int j, int lj)
{
  //## begin CRelxationLabelling::f%418197E001BA.body preserve=yes

	if ( this->m_bIsForSpherical )
		return this->f1(i, li, j, lj);
	else
		return this->f0(i, li, j, lj);

 


  //## end CRelxationLabelling::f%418197E001BA.body
}

inline double CRelxationLabelling::f0 (int i, int li, int j, int lj)
{
  //## begin CRelxationLabelling::f0%41986FC1003F.body preserve=yes


 
	double l, p;
	double p3d_i[3], p3d_j[3];
	double p2d_i[2], p2d_j[2];
	int    i3d, j3d;


	if ( ((this->m_pCAdjancentMtrx0))[i][j] != 0.0 )
//	if ( this->C(i,j) != 0.0 )
	{
	i3d			= this->m_pCNode2D[i].m_n3DIndex;
	j3d			= this->m_pCNode2D[j].m_n3DIndex;

	p3d_i[0]	= this->m_pCNode3D[i3d].x;
	p3d_i[1]	= this->m_pCNode3D[i3d].y;
	p3d_i[2]	= this->m_pCNode3D[i3d].z;

	p3d_j[0]	= this->m_pCNode3D[j3d].x;
	p3d_j[1]	= this->m_pCNode3D[j3d].y;
	p3d_j[2]	= this->m_pCNode3D[j3d].z;

	l			= (p3d_i[0] - p3d_j[0])*(p3d_i[0] - p3d_j[0]) +
				  (p3d_i[1] - p3d_j[1])*(p3d_i[1] - p3d_j[1]) +
				  (p3d_i[2] - p3d_j[2])*(p3d_i[2] - p3d_j[2]);
	l			= sqrt (l);

	p2d_i[0]	= this->m_pCLabelList[li].u;
	p2d_i[1]	= this->m_pCLabelList[li].v;
	p2d_j[0]	= this->m_pCLabelList[lj].u;
	p2d_j[1]	= this->m_pCLabelList[lj].v;

	p			= (p2d_i[0] - p2d_j[0])*(p2d_i[0] - p2d_j[0]) +
				  (p2d_i[1] - p2d_j[1])*(p2d_i[1] - p2d_j[1]);

	p			= sqrt(p);

//	p			= fabs((p-l)/p);
	p			= fabs(p-l);

	return p;
	}
	else
	 return 0.0;





  //## end CRelxationLabelling::f0%41986FC1003F.body
}

inline double CRelxationLabelling::Q (int ai, int Li)
{
  //## begin CRelxationLabelling::Q%417CBBE601B5.body preserve=yes
		
  	if(m_CQ.m_bInitialized)
		return m_CQ[ai][Li];
	else
		if ( this->IsLabelInNodeLabelBox (ai,Li) )
		{
			double Cij,Qsum=0.0;
			int n = this->m_n2DNode_length;

			for ( int aj = 0; aj < n; aj++)
			{
//	     Cij =((this->m_pCAdjancentMtrx))[ai][aj];
		     Cij =this->C(ai,aj);
			if ( Cij == 0.0 ) 
			 continue;
			Qsum +=Cij*q(aj,ai,Li);
			}
			return Qsum;
		}
		else
			return 0.0;

  //## end CRelxationLabelling::Q%417CBBE601B5.body
}

inline BOOL CRelxationLabelling::IsLabelInNodeLabelBox (int ai, int li)
{
  //## begin CRelxationLabelling::IsLabelInNodeLabelBox%419BF90700B6.body preserve=yes

  #ifndef  USING_CHECK_RECT  
  //added by xdh,2004,12,13
  if (!m_bIsForSpherical)
    	return 1;
  #endif
  
  
#define USEXYZBOX 
#ifndef USEXYZBOX 
//USEXYZBOX not defined{{
	register double u = this->m_pCLabelList[li].u;
	register double v = this->m_pCLabelList[li].v;
	BOOL bRet;
	if (!this->m_pCNode2D[ai].m_bCrossZero)
		bRet = (	u>=this->m_pCNode2D[ai].dMin_u&&
					v>=this->m_pCNode2D[ai].dMin_v&&
					u<=this->m_pCNode2D[ai].dMax_u&&
					v<=this->m_pCNode2D[ai].dMax_v);
	else
		bRet = (   (u>=this->m_pCNode2D[ai].dMin_u ||
					u<=this->m_pCNode2D[ai].dMax_u)&&
					v>=this->m_pCNode2D[ai].dMin_v &&
					v<=this->m_pCNode2D[ai].dMax_v);
	return bRet;
//USEXYZBOX not defined}}
#else
//USEXYZBOX defined {{



	register double x = this->m_pCLabelList[li].x;
	register double y = this->m_pCLabelList[li].y;
	register double z = this->m_pCLabelList[li].z;

	BOOL bRet;
	bRet = (	
				x >= this->m_pCNode2D[ai].x0 &&
				y >= this->m_pCNode2D[ai].y0 &&
				z >= this->m_pCNode2D[ai].z0 &&
				x <= this->m_pCNode2D[ai].x1 &&
				y <= this->m_pCNode2D[ai].y1 &&
				z <= this->m_pCNode2D[ai].z1					
			);

	if (bRet)
		return 1;
	else
		return 0;

//USEXYZBOX defined }}
#endif
  //## end CRelxationLabelling::IsLabelInNodeLabelBox%419BF90700B6.body
}

// Class CRelxationLabelling1 

// Class CRelxationLabelling2 

//## begin module%4175B5850036.epilog preserve=yes
//## end module%4175B5850036.epilog


#endif
