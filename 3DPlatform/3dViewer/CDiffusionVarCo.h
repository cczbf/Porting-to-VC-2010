//## begin module%41947171005F.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%41947171005F.cm

//## begin module%41947171005F.cp preserve=no
//## end module%41947171005F.cp

//## Module: CDiffusionVarCo%41947171005F; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CDiffusionVarCo.h

#ifndef CDiffusionVarCo_h
#define CDiffusionVarCo_h 1

//## begin module%41947171005F.additionalIncludes preserve=no
//## end module%41947171005F.additionalIncludes

//## begin module%41947171005F.includes preserve=yes
//## end module%41947171005F.includes

//## begin module%41947171005F.declarations preserve=no
//## end module%41947171005F.declarations

//## begin module%41947171005F.additionalDeclarations preserve=yes
//## end module%41947171005F.additionalDeclarations


//## begin CDiffusionVarCo%41947085029E.preface preserve=yes
//## end CDiffusionVarCo%41947085029E.preface

//## Class: CDiffusionVarCo%41947085029E
//	Implementation of SIGGRAPH 2003 Paper: "Improving
//	Mid-tone Quality of Variable-Coefficient Error Diffusion"
//	By Zhou,BingFeng and Fang,XiFeng
//	Institute of Computer Science and Technology
//	Peking University
//	Beijing
//	P.R.China
//	2003.4.25
//## Category: 3dViewer%40C7B40F0349
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CDiffusionVarCo 
{
  //## begin CDiffusionVarCo%41947085029E.initialDeclarations preserve=yes
  //## end CDiffusionVarCo%41947085029E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CDiffusionVarCo%41947085030F
      CDiffusionVarCo ();

    //## Destructor (specified)
      //## Operation: ~CDiffusionVarCo%41947085030E
      ~CDiffusionVarCo ();


    //## Other Operations (specified)
      //## Operation: ImageDiffusion%41947085030A
      int ImageDiffusion (int* lpImg, int nImgWidth, int nImgHeight);

      //## Operation: ImageDiffusion%419478110180
      int ImageDiffusion (LPBYTE lpImg, int nImgWidth, int nImgHeight);

      //## Operation: ImageDiffusion%419735C401AC
      int ImageDiffusion (double* lpImg, int nImgWidth, int nImgHeight);

    // Additional Public Declarations
      //## begin CDiffusionVarCo%41947085029E.public preserve=yes
      //## end CDiffusionVarCo%41947085029E.public

  protected:
    // Additional Protected Declarations
      //## begin CDiffusionVarCo%41947085029E.protected preserve=yes
      //## end CDiffusionVarCo%41947085029E.protected

  private:

    //## Other Operations (specified)
      //## Operation: ChangeCoefficient%4194708502FB
      int ChangeCoefficient (int nLevel);

      //## Operation: CalcNewThreshold%4194708502FD
      int CalcNewThreshold (int nDensity);

      //## Operation: InitPara%4194708502FF
      void InitPara ();

      //## Operation: offset1%419470850300
      int offset1 (int x, int y, int w, int h);

      //## Operation: offset%419470850305
      int offset (int x, int y, int w, int h);

    // Data Members for Class Attributes

      //## Attribute: m_nSum%4194708502F9
      //## begin CDiffusionVarCo::m_nSum%4194708502F9.attr preserve=no  private: int {UA} 
      int m_nSum;
      //## end CDiffusionVarCo::m_nSum%4194708502F9.attr

      //## Attribute: m_lpnDiffMatrix%4194708502FA
      //## begin CDiffusionVarCo::m_lpnDiffMatrix%4194708502FA.attr preserve=no  private: int[25] {UA} 
      int m_lpnDiffMatrix[25];
      //## end CDiffusionVarCo::m_lpnDiffMatrix%4194708502FA.attr

      //## Attribute: m_lpnCoefficient%419472AC0243
      //## begin CDiffusionVarCo::m_lpnCoefficient%419472AC0243.attr preserve=no  private: _int32[512] {UA} 
      _int32 m_lpnCoefficient[512];
      //## end CDiffusionVarCo::m_lpnCoefficient%419472AC0243.attr

      //## Attribute: m_lpRandomScale%419472F5007B
      //## begin CDiffusionVarCo::m_lpRandomScale%419472F5007B.attr preserve=no  private: _int32[128] {UA} 
      _int32 m_lpRandomScale[128];
      //## end CDiffusionVarCo::m_lpRandomScale%419472F5007B.attr

    // Data Members for Associations



    // Additional Private Declarations
      //## begin CDiffusionVarCo%41947085029E.private preserve=yes
      //## end CDiffusionVarCo%41947085029E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CDiffusionVarCo%41947085029E.implementation preserve=yes
      //## end CDiffusionVarCo%41947085029E.implementation

};

//## begin CDiffusionVarCo%41947085029E.postscript preserve=yes
//## end CDiffusionVarCo%41947085029E.postscript

// Class CDiffusionVarCo 

//## begin module%41947171005F.epilog preserve=yes
//## end module%41947171005F.epilog


#endif
