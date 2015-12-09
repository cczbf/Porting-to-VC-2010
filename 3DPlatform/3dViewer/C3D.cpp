//## begin module%40E2104F02B5.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40E2104F02B5.cm

//## begin module%40E2104F02B5.cp preserve=no
//## end module%40E2104F02B5.cp

//## Module: C3D%40E2104F02B5; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\C3D.cpp

//## begin module%40E2104F02B5.additionalIncludes preserve=no
//## end module%40E2104F02B5.additionalIncludes

//## begin module%40E2104F02B5.includes preserve=yes

#include "glut.h"
#include <stdlib.h>
#include <math.h>
#include "jitter.h"

//## end module%40E2104F02B5.includes

// C3D
#include "C3D.h"
//## begin module%40E2104F02B5.declarations preserve=no
//## end module%40E2104F02B5.declarations

//## begin module%40E2104F02B5.additionalDeclarations preserve=yes
#ifdef WIN32
//#define near zNear
//#define far zFar
#endif
#define PI_ 3.14159265358979323846

#define ACSIZE	8
//## end module%40E2104F02B5.additionalDeclarations


// Class C3D 

C3D::~C3D()
{
  //## begin C3D::~C3D%.body preserve=yes
  //## end C3D::~C3D%.body
}



//## Other Operations (implementation)
void C3D::accFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar, GLdouble pixdx, GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
  //## begin C3D::accFrustum%40E2115402F7.body preserve=yes
	{
   GLdouble xwsize, ywsize; 
   GLdouble dx, dy;
   GLint viewport[4];

   glGetIntegerv (GL_VIEWPORT, viewport);
	
   xwsize = right - left;
   ywsize = top - bottom;
	
   dx = -(pixdx*xwsize/(GLdouble) viewport[2] + eyedx*zNear/focus);
   dy = -(pixdy*ywsize/(GLdouble) viewport[3] + eyedy*zNear/focus);
	
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum (left + dx, right + dx, bottom + dy, top + dy, zNear, zFar);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef ((GLfloat)(-eyedx), (GLfloat)(-eyedy), (GLfloat)(0.0) );
}
  //## end C3D::accFrustum%40E2115402F7.body
}

void C3D::accPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar, GLdouble pixdx, GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
  //## begin C3D::accPerspective%40E214D3028F.body preserve=yes

	   GLdouble fov2,left,right,bottom,top;

   fov2 = ((fovy*PI_) / 180.0) / 2.0;

   top = zNear / (cos(fov2) / sin(fov2));
   bottom = -top;

   right = top * aspect;
   left = -right;

   accFrustum (left, right, bottom, top, zNear, zFar,
               pixdx, pixdy, eyedx, eyedy, focus);

  //## end C3D::accPerspective%40E214D3028F.body
}

void C3D::display ()
{
  //## begin C3D::display%40E2158C0209.body preserve=yes

    GLint viewport[4];
   int jitter;
//   int i;

   glGetIntegerv (GL_VIEWPORT, viewport);

   glClear(GL_ACCUM_BUFFER_BIT);
//   for ( i = 0; i < 10; i++ )
   for (jitter = 0; jitter < ACSIZE; jitter++) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      accPerspective (50.0, 
         (GLdouble) viewport[2]/(GLdouble) viewport[3], 
         1.0, 15.0, j8[jitter].x, j8[jitter].y, 0.0, 0.0, 1.0);
      displayObjects ();
      glAccum(GL_ACCUM, 1.0/ACSIZE);
   }
   glAccum (GL_RETURN, 1.0);
   glFlush();
  //## end C3D::display%40E2158C0209.body
}

void C3D::displayObjects ()
{
  //## begin C3D::displayObjects%40E215A503B4.body preserve=yes
 
   GLfloat torus_diffuse[] = { 0.7f, 0.7f, 0.0f, 1.0f };
   GLfloat cube_diffuse[] = { 0.0f, 0.7f, 0.7f, 1.0f };
   GLfloat sphere_diffuse[] = { 0.7f, 0.0f, 0.7f, 1.0f };
   GLfloat octa_diffuse[] = { 0.7f, 0.4f, 0.4f, 1.0f };
    
   glPushMatrix ();
   glTranslatef (0.0, 0.0, -5.0); 
   glRotatef (30.0, 1.0, 0.0, 0.0);

   glPushMatrix ();
   glTranslatef (-0.80f, 0.35f, 0.0f); 
   glRotatef (100.0f, 1.0f, 0.0f, 0.0f);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, torus_diffuse);
   glutSolidTorus (0.275f, 0.85f, 16, 16);
   glPopMatrix ();

   glPushMatrix ();
   glTranslatef (-0.75, -0.50, 0.0); 
   glRotatef (45.0, 0.0, 0.0, 1.0);
   glRotatef (45.0, 1.0, 0.0, 0.0);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_diffuse);
   glutSolidCube (1.5);
   glPopMatrix ();

   glPushMatrix ();
   glTranslatef (0.75f, 0.60f, 0.0f); 
   glRotatef (30.0, 1.0, 0.0, 0.0);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse);
   glutSolidSphere (1.0, 16, 16);
   glPopMatrix ();

   glPushMatrix ();
   glTranslatef (0.70f, -0.90f, 0.25f); 
   glMaterialfv(GL_FRONT, GL_DIFFUSE, octa_diffuse);
   glutSolidOctahedron ();
   glPopMatrix ();

   glPopMatrix ();

  //## end C3D::displayObjects%40E215A503B4.body
}

void C3D::init ()
{
  //## begin C3D::init%40E215BB0211.body preserve=yes

   GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
   GLfloat lm_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
    
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel (GL_FLAT);

   glClearColor(0.0, 0.25, 0.5, 0.0);
   //glClearColor(140.0f/255, 195.0f/255, 245.0f/155, 1.0f);
   glClearAccum(0.0, 0.0, 0.0, 0.0);

  //## end C3D::init%40E215BB0211.body
}

void C3D::keyboard (unsigned char key, int x, int y)
{
  //## begin C3D::keyboard%40E215CE01FA.body preserve=yes
  //## end C3D::keyboard%40E215CE01FA.body
}

void    __cdecl C3D::reshape (int w, int h)
{
  //## begin C3D::reshape%40E21601013F.body preserve=yes

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  //## end C3D::reshape%40E21601013F.body
}

int C3D::run ()
{
  //## begin C3D::run%40E21657005C.body preserve=yes
	int argc;
	char **argv = NULL;
	argv[0] = "ZBF";

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB
                        | GLUT_ACCUM | GLUT_DEPTH);
   glutInitWindowSize (250, 250);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();
   /*
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   */
   glutMainLoop();
   
	return 1;
  //## end C3D::run%40E21657005C.body
}

// Additional Declarations
  //## begin C3D%40E2102C020B.declarations preserve=yes
  //## end C3D%40E2102C020B.declarations

//## begin module%40E2104F02B5.epilog preserve=yes
//## end module%40E2104F02B5.epilog
