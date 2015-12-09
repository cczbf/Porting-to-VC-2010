//## begin module%41E3789B0190.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%41E3789B0190.cm

//## begin module%41E3789B0190.cp preserve=no
//## end module%41E3789B0190.cp

//## Module: CMyCGLib%41E3789B0190; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CMyCGLib.h

#ifndef CMyCGLib_h
#define CMyCGLib_h 1

//## begin module%41E3789B0190.additionalIncludes preserve=no
//## end module%41E3789B0190.additionalIncludes

//## begin module%41E3789B0190.includes preserve=yes
//## end module%41E3789B0190.includes

//## begin module%41E3789B0190.declarations preserve=no
//## end module%41E3789B0190.declarations

//## begin module%41E3789B0190.additionalDeclarations preserve=yes
//## end module%41E3789B0190.additionalDeclarations


//## begin CMyCGLib%41E3783A0386.preface preserve=yes
//## end CMyCGLib%41E3783A0386.preface

//## Class: CMyCGLib%41E3783A0386
//	这是一个算法库，不能有全局变量。
//## Category: 3dViewer::OpenGL%41E3776B0220
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CMyCGLib 
{
  //## begin CMyCGLib%41E3783A0386.initialDeclarations preserve=yes
  //## end CMyCGLib%41E3783A0386.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: pp%41E378620058
      double pp (double* a, double* b, int n);

      //## Operation: normalize%41E3787601F1
      bool normalize (double* v, int n);

      //## Operation: cross_product%41E378840197
      void cross_product (double p[3], double q[3], double* r);

      //## Operation: sub%41E3949102F2
      //	c = a - b
      //	a,b,c are vectors of n dimentoin.
      void sub (double* a, double* b, double* c, int n);

      //## Operation: scale%41E395930158
      void scale (double* v0, double* v1, double s, int n);

      //## Operation: is_ccw%41E5CFFC016B
      //	see: http://mathworld.wolfram.com/PolygonArea.html
      bool is_ccw (float* lpx, float* lpy, int n);

    // Additional Public Declarations
      //## begin CMyCGLib%41E3783A0386.public preserve=yes
      double ComputeTriangleArea(double *v1, double *v2, double *v3);
	  double DistancePt2Line(double x, double y, double A, double B, double C);
      //## end CMyCGLib%41E3783A0386.public

  protected:
    // Additional Protected Declarations
      //## begin CMyCGLib%41E3783A0386.protected preserve=yes
      //## end CMyCGLib%41E3783A0386.protected

  private:
    // Additional Private Declarations
      //## begin CMyCGLib%41E3783A0386.private preserve=yes
      //## end CMyCGLib%41E3783A0386.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CMyCGLib%41E3783A0386.implementation preserve=yes
      //## end CMyCGLib%41E3783A0386.implementation

};

//## begin CMyCGLib%41E3783A0386.postscript preserve=yes
//## end CMyCGLib%41E3783A0386.postscript

// Class CMyCGLib 

//## begin module%41E3789B0190.epilog preserve=yes
//## end module%41E3789B0190.epilog


#endif
