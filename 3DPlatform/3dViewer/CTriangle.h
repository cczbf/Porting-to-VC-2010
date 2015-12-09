//## begin module%3FCA8D6E00B9.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FCA8D6E00B9.cm

//## begin module%3FCA8D6E00B9.cp preserve=no
//## end module%3FCA8D6E00B9.cp

//## Module: CTriangle%3FCA8D6E00B9; Pseudo Package specification
//## Source file: C:\zbf\Research\Spherical Para\Foundmentals of SP\fsp\CTriangle.h

#ifndef CTriangle_h
#define CTriangle_h 1

//## begin module%3FCA8D6E00B9.additionalIncludes preserve=no
//## end module%3FCA8D6E00B9.additionalIncludes

//## begin module%3FCA8D6E00B9.includes preserve=yes
//## end module%3FCA8D6E00B9.includes

//## begin module%3FCA8D6E00B9.additionalDeclarations preserve=yes
//## end module%3FCA8D6E00B9.additionalDeclarations


//## begin CTriangle%3FCA8D6E00B9.preface preserve=yes
//## end CTriangle%3FCA8D6E00B9.preface

//## Class: CTriangle%3FCA8D6E00B9
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CTriangle 
{
  //## begin CTriangle%3FCA8D6E00B9.initialDeclarations preserve=yes
  //## end CTriangle%3FCA8D6E00B9.initialDeclarations

  public:
    //## Constructors (generated)
      CTriangle();

    //## Destructor (generated)
      ~CTriangle();

    // Data Members for Class Attributes

      //## Attribute: pnV%3FCA8D72032F
      //## begin CTriangle::pnV%3FCA8D72032F.attr preserve=no  public: int[3] {UA} 
      int pnV[3];
      //## end CTriangle::pnV%3FCA8D72032F.attr

      //## Attribute: dColorR%3FCAA4FB0108
      //## begin CTriangle::dColorR%3FCAA4FB0108.attr preserve=no  public: double {UA} 0.0
      double dColorR;
      //## end CTriangle::dColorR%3FCAA4FB0108.attr

      //## Attribute: dColorG%3FCAA50D0291
      //## begin CTriangle::dColorG%3FCAA50D0291.attr preserve=no  public: double {UA} 0.0
      double dColorG;
      //## end CTriangle::dColorG%3FCAA50D0291.attr

      //## Attribute: dColorB%3FCAA512000C
      //## begin CTriangle::dColorB%3FCAA512000C.attr preserve=no  public: double {UA} 0.0
      double dColorB;
      //## end CTriangle::dColorB%3FCAA512000C.attr

      // Additional Public Declarations
      //## begin CTriangle%3FCA8D6E00B9.public preserve=yes
	  //added by xdh,to record the streth ratio of each edge
	  COLORREF dC12;      //the color of edge(p1-p2)
	  COLORREF dC23;      //the color of edge(p2-p3) 
	  COLORREF dC31;      //the color of edge(p3-p1) 
      double   dfRatio12; //the streth ratio of edge(p1-p2)
	  double   dfRatio23; //the streth ratio of edge(p2-p3) 
	  double   dfRatio31; //the streth ratio of edge(p3-p1) 
      //## end CTriangle% 3FCA8D6E00B9.public

  protected:
    // Additional Protected Declarations
      //## begin CTriangle%3FCA8D6E00B9.protected preserve=yes
      //## end CTriangle%3FCA8D6E00B9.protected

  private:
    // Additional Private Declarations
      //## begin CTriangle%3FCA8D6E00B9.private preserve=yes
      //## end CTriangle%3FCA8D6E00B9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CTriangle%3FCA8D6E00B9.implementation preserve=yes
      //## end CTriangle%3FCA8D6E00B9.implementation

};

//## begin CTriangle%3FCA8D6E00B9.postscript preserve=yes
//## end CTriangle%3FCA8D6E00B9.postscript

// Class CTriangle 

//## begin module%3FCA8D6E00B9.epilog preserve=yes
//## end module%3FCA8D6E00B9.epilog


#endif
