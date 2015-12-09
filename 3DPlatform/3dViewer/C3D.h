//## begin module%40E2104C027F.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40E2104C027F.cm

//## begin module%40E2104C027F.cp preserve=no
//## end module%40E2104C027F.cp

//## Module: C3D%40E2104C027F; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\C3D.h

#ifndef C3D_h
#define C3D_h 1

//## begin module%40E2104C027F.additionalIncludes preserve=no
//## end module%40E2104C027F.additionalIncludes

//## begin module%40E2104C027F.includes preserve=yes
//## end module%40E2104C027F.includes

//## begin module%40E2104C027F.declarations preserve=no
//## end module%40E2104C027F.declarations

//## begin module%40E2104C027F.additionalDeclarations preserve=yes
//## end module%40E2104C027F.additionalDeclarations


//## begin C3D%40E2102C020B.preface preserve=yes
//## end C3D%40E2102C020B.preface

//## Class: C3D%40E2102C020B
//## Category: 3dViewer::Core%40DB9BAD036C
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class C3D 
{
  //## begin C3D%40E2102C020B.initialDeclarations preserve=yes
  //## end C3D%40E2102C020B.initialDeclarations

  public:
    //## Destructor (generated)
      ~C3D();


    //## Other Operations (specified)
      //## Operation: display%40E2158C0209
      void display ();

      //## Operation: displayObjects%40E215A503B4
      void displayObjects ();

      //## Operation: init%40E215BB0211
      void init ();

      //## Operation: keyboard%40E215CE01FA
      void keyboard (unsigned char key, int x, int y);

      //## Operation: reshape%40E21601013F
      void __cdecl reshape (int w, int h);

      //## Operation: run%40E21657005C
      //	main()
      int run ();

    // Additional Public Declarations
      //## begin C3D%40E2102C020B.public preserve=yes
      //## end C3D%40E2102C020B.public

  protected:
    // Additional Protected Declarations
      //## begin C3D%40E2102C020B.protected preserve=yes
      //## end C3D%40E2102C020B.protected

  private:

    //## Other Operations (specified)
      //## Operation: accFrustum%40E2115402F7
      //	/* accFrustum()
      //	 * The first 6 arguments are identical to the gl
      //	Frustum() call.
      //	 *
      //	 * pixdx and pixdy are anti-alias jitter in pixels.
      //	 * Set both equal to 0.0 for no anti-alias jitter.
      //	 * eyedx and eyedy are depth-of field jitter in pixels.
      //	 * Set both equal to 0.0 for no depth of field effects.
      //	 *
      //	 * focus is distance from eye to plane in focus.
      //	 * focus must be greater than, but not equal to 0.0.
      //	 *
      //	 * Note that accFrustum() calls glTranslatef().  You
      //	will
      //	 * probably want to insure that your ModelView matrix
      //	has been
      //	 * initialized to identity before calling accFrustum().
      //	 */
      void accFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar, GLdouble pixdx, GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, GLdouble focus);

      //## Operation: accPerspective%40E214D3028F
      //	/* accPerspective()
      //	 *
      //	 * The first 4 arguments are identical to the glu
      //	Perspective() call.
      //	 * pixdx and pixdy are anti-alias jitter in pixels.
      //	 * Set both equal to 0.0 for no anti-alias jitter.
      //	 * eyedx and eyedy are depth-of field jitter in pixels.
      //	 * Set both equal to 0.0 for no depth of field effects.
      //	 *
      //	 * focus is distance from eye to plane in focus.
      //	 * focus must be greater than, but not equal to 0.0.
      //	 *
      //	 * Note that accPerspective() calls accFrustum().
      //	 */
      void accPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar, GLdouble pixdx, GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, GLdouble focus);

    // Additional Private Declarations
      //## begin C3D%40E2102C020B.private preserve=yes
      //## end C3D%40E2102C020B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin C3D%40E2102C020B.implementation preserve=yes
      //## end C3D%40E2102C020B.implementation

};

//## begin C3D%40E2102C020B.postscript preserve=yes
//## end C3D%40E2102C020B.postscript

// Class C3D 

//## begin module%40E2104C027F.epilog preserve=yes
//## end module%40E2104C027F.epilog


#endif
