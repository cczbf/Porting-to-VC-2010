//## begin module%40EB6BE5029D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40EB6BE5029D.cm

//## begin module%40EB6BE5029D.cp preserve=no
//## end module%40EB6BE5029D.cp

//## Module: COpenGL3DEx%40EB6BE5029D; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\COpenGL3DEx.cpp

//## begin module%40EB6BE5029D.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%40EB6BE5029D.additionalIncludes

//## begin module%40EB6BE5029D.includes preserve=yes
#include "stdafx.h"
//#include "CDIB.h"
//#include <math.h>
//#include <errno.h>
#include <FLOAT.H>

#include "ProgressBar.h" //Added by XDH, 2004.10.22, --comment by zbf
#include "CGlobals.h"

#include "math.h"
#include "matrix.h"
//## end module%40EB6BE5029D.includes

// COpenGL3DEx
#include "COpenGL3DEx.h"
#include "2DMesh.h"
#include "LabelPts.h"

//## begin module%40EB6BE5029D.declarations preserve=no
//## end module%40EB6BE5029D.declarations
//## begin module%40EB6BE5029D.additionalDeclarations preserve=yes

#pragma optimize( "", off )

//## end module%40EB6BE5029D.additionalDeclarations


// Class COpenGL3DEx 


COpenGL3DEx::COpenGL3DEx ()
  //## begin COpenGL3DEx::COpenGL3DEx%40F32FEC0145.hasinit preserve=no
  //## end COpenGL3DEx::COpenGL3DEx%40F32FEC0145.hasinit
  //## begin COpenGL3DEx::COpenGL3DEx%40F32FEC0145.initialization preserve=yes
  //## end COpenGL3DEx::COpenGL3DEx%40F32FEC0145.initialization
{
  //## begin COpenGL3DEx::COpenGL3DEx%40F32FEC0145.body preserve=yes
			this->m_pCMesh = NULL;
			//this->m_pNewMesh = NULL;
			m_bIsNewMesh = FALSE;
			m_bIsShowLabel = TRUE;
			m_bIsShowLine = TRUE;
			m_bIsShowArc = TRUE;
			g_CGlobals.set_m_pCOpenGL3DEx((COpenGL3DEx*)this);
  //## end COpenGL3DEx::COpenGL3DEx%40F32FEC0145.body
}


COpenGL3DEx::~COpenGL3DEx ()
{
  //## begin COpenGL3DEx::~COpenGL3DEx%40F3A43301F9.body preserve=yes
	if (this->m_pCMesh != NULL )
		{
			delete this->m_pCMesh;
			this->m_pCMesh = NULL;
		}
  //## end COpenGL3DEx::~COpenGL3DEx%40F3A43301F9.body
}



//## Other Operations (implementation)
void COpenGL3DEx::LoadSurferRAWData (HANDLE hRaw, int col, int row)
{
  //## begin COpenGL3DEx::LoadSurferRAWData%40EB9EA300D9.body preserve=yes

//void COpenGL3D::LoadSurferBinaryData(FILE* fp)
{
	int n;
//	short col, row;
//	double x1, x2, y1, y2, z1, z2;
/*
	fseek(fp, 4, SEEK_SET);
	fread(&col, 2, 1, fp);
	fread(&row, 2, 1, fp);
	fread(&x1, 8, 1, fp);
	fread(&x2, 8, 1, fp);
	fread(&y1, 8, 1, fp);
	fread(&y2, 8, 1, fp);
	fread(&z1, 8, 1, fp);
	fread(&z2, 8, 1, fp);
*/	
	m_DS.RemoveAll(); 
	m_p3D = NULL;
	m_pDS = NULL;
	n = 1;
	m_DS.SetSize(n);

	Init3DDataSet(col, row, G3D, n);
/*
	m_DS[0].m_fXmin = (float)x1;
	m_DS[0].m_fXmax = (float)x2;
	m_DS[0].m_fYmin = (float)y1;
	m_DS[0].m_fYmax = (float)y2;
	m_DS[0].m_fZmin = (float)z1;
	m_DS[0].m_fZmax = (float)z2;
	m_DS[0].m_MapName = "Surfer Binary File";
*/
	m_DS[0].m_fXmin = 0.0f;
	m_DS[0].m_fXmax = 10.0f;
	m_DS[0].m_fYmin = 0.0f;
	m_DS[0].m_fYmax = 10.0f;
	m_DS[0].m_fZmin = FLT_MAX;
	m_DS[0].m_fZmax = -FLT_MAX;
	m_DS[0].m_MapName = "Surfer Binary File";
	
	if(hRaw != NULL)
	{	
		CProgressBar *pBar = NULL;
		pBar = new CProgressBar(_T("Progress"), 100, 100, TRUE);
		pBar->SetText("Processing...");
		float step = 0.0;
		float prog = 0.0;
		step = (float)(100.0)/(float)(row*col);
		
		//	float tValue;
		BYTE *lpBYTE;
		lpBYTE = (BYTE*)GlobalLock(hRaw);
		for(int k=0; k<row; k++)
		{
			for(int j=0; j<col; j++)
			{
				prog += step;
				pBar->SetPos(int(prog));
				
				int i = k * col + j;
				//			fread(&tValue, 4, 1, fp);
				//			m_DS[0].m_XYZ.z[i] = tValue;
				m_DS[0].m_XYZ.z[i] = lpBYTE[i];
				m_DS[0].m_fZmin		= min(lpBYTE[i],m_DS[0].m_fZmin);
				m_DS[0].m_fZmax		= max(lpBYTE[i],m_DS[0].m_fZmax);
			}
		}
		delete pBar;
		
		GlobalUnlock(hRaw);
	}

	for(int k=0; k<col; k++)
		m_DS[0].m_XYZ.x[k] = 1.0f * k / (col - 1) - m_fC;

	for(int k=0; k<row; k++)
		m_DS[0].m_XYZ.y[k] = 1.0f * k / (row - 1) - m_fC;


	n = row * col;
	for(int k=0; k<n; k++)
			m_DS[0].m_XYZ.z[k] = (m_DS[0].m_XYZ.z[k] - m_DS[0].m_fZmin) / 
								 (m_DS[0].m_fZmax - m_DS[0].m_fZmin) - m_fC;
}

  //## end COpenGL3DEx::LoadSurferRAWData%40EB9EA300D9.body
}

void COpenGL3DEx::Load3DData (HANDLE hRaw, int col, int row, float x0, float y0, float x1, float y1)
{
  //## begin COpenGL3DEx::Load3DData%40EBAB53021A.body preserve=yes

//	void COpenGL3D::Load3DData(FILE* fp, char type)
{
	/*
	switch(type)
	{
	case 'A':
	case 'B':
		LoadSurferData(fp, type);
		break;
	case 'C':
	case 'D':
		Load3DMultipleMapsData(fp, type);
		break;
	}
	*/

//	delete this->m_pCMesh;
//	this->m_pCMesh = NULL;
	this->LoadSurferRAWData(hRaw, col, row);
	m_p3D = &m_DS[0];
	m_pDS = m_p3D;

/////////////////////////////////////////////{{ by zbf 20040706   
	m_pDS->m_MinCr.r=0.0;
	m_pDS->m_MinCr.g=0.0;
	m_pDS->m_MinCr.b=1.0;
	m_pDS->m_MaxCr.r=1.0;
	m_pDS->m_MaxCr.g=0.0;
	m_pDS->m_MaxCr.b=0.0;

	m_pDS->m_nCrLevel = 30;
//////////////////////////////////////////////}}by zbf 20040706


}

  //## end COpenGL3DEx::Load3DData%40EBAB53021A.body
}

bool COpenGL3DEx::InitOpenGL (HWND hWnd)
{
  //## begin COpenGL3DEx::InitOpenGL%40EDFAA8027E.body preserve=yes
	bool bRet = COpenGL3D::InitOpenGL (hWnd);
/////////////////////{{Set Lighting 
/*
   GLfloat mat_specular[] = { 0.5, 0.2, 0.5, 1.0 };
   GLfloat mat_diffuse[]={0.5, 0.5, 0.2, 1.0};
   GLfloat mat_shininess[] = { 100.0 };
//   GLfloat light_position[] = { 3.0, 3.0, 1.0, 0.0 };
    GLfloat light_position[] = { 30.0, 30.0, 1.0, 0.0 };

   GLfloat light1_ambient[]={ 1.0, 1.0, 0.5, 1.0};
   GLfloat light1_diffuse[]={0.0, 0.8, 1.0, 1.0};
   GLfloat light1_specular[]={1.0, 1.0, 1.0, 1.0};
//   GLfloat light1_position[]={-3.0, 0.0, 1.0, 1.0};
   GLfloat light1_position[]={-30.0, 0.0, 1.0, 1.0};
//   GLfloat spot_direction[]={1.0,4.0, 0.0};
   GLfloat spot_direction[]={0.0,0.0, 0.0};

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
   glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
   glLightfv(GL_LIGHT1,GL_SPECULAR,light1_specular);
   glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
   glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION, 0.5);
   glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION, 0.1);
   glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION, 0.0);
	
   glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,60.0);
   glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spot_direction);
   glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,2.0);

   
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT1);
   glEnable(GL_DEPTH_TEST);
   */

	
	//added by xiedonghai,2004,10,21
	//initialize the lighting :
	GLfloat mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	   
	GLfloat lm_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
	   
	glMaterialfv(GL_FRONT, GL_AMBIENT,  mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 80.0);
	   
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  lm_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuseProperties);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
	//////////////////////////////////////////////////////////////////////////

    //if (this->m_bLightingOn)
	     //this->LightingOn(TRUE);
	/*
		{
	   GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	   GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	   GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	   glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
    
	   glEnable(GL_LIGHTING);
	   glEnable(GL_LIGHT0);
	   glEnable(GL_DEPTH_TEST);
	   glShadeModel (GL_FLAT);

	   glClearColor(0.0, 0.0, 0.0, 0.0);
	   glClearAccum(0.0, 0.0, 0.0, 0.0);
	}
*/
/////////////////////}} Set Lighting 

	return bRet;
  //## end COpenGL3DEx::InitOpenGL%40EDFAA8027E.body
}

void COpenGL3DEx::LightingOn (BOOL bOn)
{
  //## begin COpenGL3DEx::LightingOn%40F23A3D0159.body preserve=yes
	m_bLightingOn = bOn;

	if(bOn)
	{
	   //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
	   glEnable(GL_LIGHTING);
	   glEnable(GL_LIGHT0);
	   glEnable(GL_DEPTH_TEST);

	   glShadeModel (/*GL_FLAT*/GL_SMOOTH);

	   //glClearColor(0.0, 0.0, 0.0, 0.0);
	   glClearAccum(0.0, 0.0, 0.0, 0.0);
	}
	else
	{
	   glDisable(GL_LIGHTING);
	   glDisable(GL_LIGHT0);
	}

  //## end COpenGL3DEx::LightingOn%40F23A3D0159.body
}

void COpenGL3DEx::OpenGLRendering ()
{
  //## begin COpenGL3DEx::OpenGLRendering%40F4966E0227.body preserve=yes
{
//void COpenGL::OpenGLRendering()
{
	if (!m_bDraw) 
		return;

	m_bDraw = false;

	// begin OpenGL call
	//wglMakeCurrent(m_hDC, m_hGLContext);	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPushMatrix();
	
	if (m_pDS == NULL)
		OpenGLDrawBanner();
	else
	{
		if (this->m_pCMesh == NULL)
			OpenGLDrawMap();
		else
			OpenGL3DExDrawMesh();  
	}
	
	//#define TESTINGGLOBE
#ifdef TESTINGGLOBE
	///////Test, test, test//////////////{{
	//this->DrawAxis ();
	
	GLUquadricObj* pQ = gluNewQuadric(  );
	gluSphere(
		pQ,//GLUquadricObj *qobj,
		0.5,//GLdouble radius,
		32,//GLint slices,
		32 //GLint stacks
		);
	gluDeleteQuadric(
		pQ//GLUquadricObj *state
		);
#endif

	/////////////////////////////////////}}
	//glPopMatrix();

	glFinish();
	
	//SwapBuffers(wglGetCurrentDC());

	// begin GDI call
	if (m_pDS == NULL)
		GDIDrawBanner();
	else
		GDIDrawMap();

	m_bDraw = true;
}
}

  //## end COpenGL3DEx::OpenGLRendering%40F4966E0227.body
}



void COpenGL3DEx::DrawMeshSurface ()
{
  //## begin COpenGL3DEx::DrawMeshSurface%40F4A52C0075.body preserve=yes

//void COpenGL3D::DrawSurface()
{

	if (m_DataGroup != GTRUE3D)
	{
//		for( int j=0; j<m_p3D->nY-1; j++ )
//			for( int i=0; i<m_p3D->nX-1; i++ )
		int n = this->m_pCMesh->m_CTriangleArray.GetSize();
//		for( int i=0; i<m_p3D->nX-1; i++ )
		int m;
		if(this->m_bAcceleratingOn)
		{
			m=g_CGlobals.m_pDlgTab01->m_nAccMul;
			if(m==10 && n/m>32767)
				m=n/32767;
			m--;
		}

		for( int i=0; i<n; i++ )
		{
			float a[3],b[3],c[3];
			a[0] = (float)(this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dX);
			a[1] = (float)(this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dY);
			a[2] = (float)(this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dZ);

			b[0] = (float)(this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dX);
			b[1] = (float)(this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dY);
			b[2] = (float)(this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dZ);
					  
			c[0] = (float)(this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dX);
			c[1] = (float)(this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dY);
			c[2] = (float)(this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dZ);
//			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			
			if ( this->m_bLightingOn)
			{
				double p[3];
				double q[3];
				double r[3];
				p[0] = a[0] - b[0];
				p[1] = a[1] - b[1];
				p[2] = a[2] - b[2];
			
				q[0] = c[0] - b[0];
				q[1] = c[1] - b[1];
				q[2] = c[2] - b[2];
			
				this->cross_product (p, q, r);
				this->normalize(r,3);

//				double z[3] = {0.0,0.0,1.0};
//				if (pp(r,z,3)>0)
//					glNormal3d( r[0], r[1], r[2] );
//				else
				if( this->normalize(r,3) )
				{
					glNormal3f( (float)(-r[0]), (float)(-r[1]), (float)(-r[2]) );
				}
				else
				{
					glNormal3f( 0.0,  0.0 , 1.0 );
				}
				glEnable(GL_NORMALIZE );
  			}		
			
			glBegin(/*GL_QUADS*/GL_TRIANGLES);
			
//			ColorLevel(m_p3D->m_XYZ.z[j*m_p3D->nX+i]);
//			glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX+i]);//1
			ColorLevel((float)a[2]);
			glVertex3f((float)a[0], (float)a[1], (float)a[2]);//1
//			ColorLevel(m_p3D->m_XYZ.z[j*m_p3D->nX+i+1]);
//			glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX+i+1]);//2
			ColorLevel((float)b[2]);
			glVertex3f((float)b[0],(float) b[1], (float)b[2]);//2
//			ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i+1]);
//			glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i+1]);//3
			ColorLevel((float)c[2]);
			glVertex3f((float)c[0], (float)c[1], (float)c[2]);//3

//			ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i]);
//			glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i]);//4
            if(this->m_bAcceleratingOn)
				i+=m;
			glEnd();
		}
	}
	else
	{
		for( int j=0; j<m_p3D->nY-1; j++ )
			for( int i=0; i<m_p3D->nX-1; i++ )
		{
			glBegin(GL_QUADS);
			ColorLevel(m_p3D->m_XYZ.w[j*m_p3D->nX+i]);
			glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX+i]);
			ColorLevel(m_p3D->m_XYZ.w[j*m_p3D->nX+i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX+i+1]);
			ColorLevel(m_p3D->m_XYZ.w[(j+1)*m_p3D->nX+i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i+1]);
			ColorLevel(m_p3D->m_XYZ.w[(j+1)*m_p3D->nX+i]);
			glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i]);
			glEnd();
		}
	}
}



  //## end COpenGL3DEx::DrawMeshSurface%40F4A52C0075.body
}

void COpenGL3DEx::OpenGL3DExDrawMesh ()
{
  //## begin COpenGL3DEx::OpenGL3DExDrawMesh%40F4DCC70175.body preserve=yes


	if(m_p3D->m_MapType != FLAT)	// Draw 3D
	{
		glScalef(m_p3D->m_xScale, m_p3D->m_yScale, m_p3D->m_zScale);
	}
	else	//Draw 2D
	{   
		glScalef(m_p3D->m_xScale, m_p3D->m_yScale, 0);
	}

	switch (m_pCMesh->m_nType)
	{
	case 4:
	    this->DrawMeshSurface();

		break;
	case 3:
        DrawGeoimgMesh();
		DrawColorBar();	

		break;
	case 2:
		if( m_ModeType == SURFACE)
		{
			Draw3DMeshWithStrethRatio();

			if(m_bIsShowArc)
				Draw3DMeshWithArc();
		}
		else if(m_ModeType == GRID)
		{				
			if(m_bIsShowLine || m_bIsShowLabel)
				Draw3DEdgeWithStretchRatio();
			
			if(m_bIsShowArc)
			{				
				Draw3DMeshWithArc();
			}
		}
		else
			Draw3DMeshWithStrethRatio();
			
		DrawAxis();
		DrawColorBar();	
    	//DrawStatistics();
		break;
	case 1:
		Draw3DEdgeWithStretchRatio();
		Draw2DMeshEdgeVertex();

		DrawAxis();
		DrawColorBar();	
		//DrawStatistics();
		//Print(" Text!!! ");
		//DrawBorder();
		break;

	case 0:
		//			DrawSurface();
		this->DrawMeshSurface();
		DrawAxis();
		DrawBorder();
		break;
	}
	
	if(m_p3D->m_bLegend)	
		DrawLegend();  // Must keep to draw legend the last

	//## end COpenGL3DEx::OpenGL3DExDrawMesh%40F4DCC70175.body
}

void COpenGL3DEx::DrawAxis ()
{
  //## begin COpenGL3DEx::DrawAxis%40F72D6A011C.body preserve=yes
	COpenGL::DrawAxis ();
  //## end COpenGL3DEx::DrawAxis%40F72D6A011C.body
}

void COpenGL3DEx::DrawBorder ()
{
  //## begin COpenGL3DEx::DrawBorder%40F72D730197.body preserve=yes
	COpenGL::DrawBorder ();
  //## end COpenGL3DEx::DrawBorder%40F72D730197.body
}



void  COpenGL3DEx::Draw3DEdgeWithStretchRatio ()
{
  //## begin COpenGL3DEx::Draw3DEdgeWithStretchRatio%418843A90053.body preserve=yes
//void COpenGL3DEx::Draw3DEdgeWithStretchRatio()
{
	
	
	int n = this->m_pCMesh->m_CTriangleArray.GetSize();
	
	for( int i=0; i<n; i++ )
	{
		COLORREF c12, c31, c23;
		double   r12, r23, r31;
		
		float a[3],b[3],c[3];
		a[0] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dX ) ;
		a[1] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dY );
		a[2] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dZ );
		
		b[0] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dX );
		b[1] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dY );
		b[2] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dZ );
		
		c[0] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dX );
		c[1] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dY );
		c[2] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dZ );
		
		/*
		float norm[3];
		norm[0] = (float)sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
		norm[1] = (float)sqrt(b[0]*b[0] + b[1]*b[1] + b[2]*b[2]);
		norm[2] = (float)sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
			
		for(int ii=0; ii<3; ii++)
		{
			a[ii] = (float)(( a[ii] / norm[0] )*0.5);
			b[ii] = (float)(( b[ii] / norm[1] )*0.5);
			c[ii] = (float)(( c[ii] / norm[2] )*0.5);
		}
		*/
				
		double p[3];
		double q[3];
		double r[3];
		p[0] = a[0] - b[0];
		p[1] = a[1] - b[1];
		p[2] = a[2] - b[2];
		
		q[0] = c[0] - b[0];
		q[1] = c[1] - b[1];
		q[2] = c[2] - b[2];
		
		this->cross_product (p, q, r);
		
		if(this->normalize(r,3))
		{								
			if (m_bIsShowLine)
			{						
				glNormal3f( (float)(-r[0]), (float)(-r[1]), (float)(-r[2]) );
				glEnable(GL_NORMALIZE );
				
				c12 = this->m_pCMesh->m_CTriangleArray[i].dC12;
				c31 = this->m_pCMesh->m_CTriangleArray[i].dC31;
				c23 = this->m_pCMesh->m_CTriangleArray[i].dC23;
				
				r12 = this->m_pCMesh->m_CTriangleArray[i].dfRatio12;
				r31 = this->m_pCMesh->m_CTriangleArray[i].dfRatio31;
				r23 = this->m_pCMesh->m_CTriangleArray[i].dfRatio23;
				
				float cr,cg,cb;
				cr = (float)GetRValue(c12)/(float)255.0;
				cg = (float)GetGValue(c12)/(float)255.0;
				cb = (float)GetBValue(c12)/(float)255.0;
				
				glColor3f(cr, cg, cb);
				
				glEnable(GL_LINE_STIPPLE);						
				if (r12 < 1)
				{
					glEnable(GL_LINE_STIPPLE);
					glLineStipple(1, 0x8888);
				}
				else
				{
					glDisable(GL_LINE_STIPPLE);
				}
				
				glBegin(GL_LINES);   			
				glVertex3f((float)a[0], (float)a[1], (float)a[2]);//1
				glVertex3f((float)b[0], (float)b[1], (float)b[2]);//2
				glEnd();
				
				
				cr = (float)GetRValue(c31)/(float)255.0;
				cg = (float)GetGValue(c31)/(float)255.0;
				cb = (float)GetBValue(c31)/(float)255.0;
				
				glColor3f(cr, cg, cb);
				if (r31 < 1)
				{
					glEnable(GL_LINE_STIPPLE);
					glLineStipple(1, 0x8888);
				}
				else
				{
					glDisable(GL_LINE_STIPPLE);
				}
				glBegin(GL_LINES);     
				glVertex3f((float)a[0], (float)a[1], (float)a[2]);//1
				glVertex3f((float)c[0], (float)c[1], (float)c[2]);//3
				glEnd();
				
				
				cr = (float)GetRValue(c23)/(float)255.0;
				cg = (float)GetGValue(c23)/(float)255.0;
				cb = (float)GetBValue(c23)/(float)255.0;
				glColor3f(cr, cg, cb);
				if (r23 < 1)
				{
					glEnable(GL_LINE_STIPPLE);
					glLineStipple(1, 0x8888);
				}
				else
				{
					glDisable(GL_LINE_STIPPLE);
				}	
				
				glBegin(GL_LINES);     
				glVertex3f((float)c[0], (float)c[1], (float)c[2]);//2
				glVertex3f((float)b[0], (float) b[1], (float)b[2]);//3
				glEnd();
			}					
		}	
	}	

	if(m_bIsShowLabel)
		DrawLabelPts();

	glDisable(GL_LINE_STIPPLE);
}

  //## end COpenGL3DEx::Draw3DEdgeWithStretchRatio%418843A90053.body
}

void COpenGL3DEx::Draw2DMeshEdgeVertex ()
{
  //## begin COpenGL3DEx::Draw2DMeshEdge%418843CB01ED.body preserve=yes
//void COpenGL3DEx::Draw2DMeshEdge()
{	
	glPushMatrix();
	int nEdgeNum = m_pCMesh->GetBorderLen();

	for (int i=0; i<nEdgeNum; i++)
	{
		float x, y, z;

		x = (float)m_pCMesh->m_CVertexArray[i].dX;
		y = (float)m_pCMesh->m_CVertexArray[i].dY;
		z = (float)(m_pCMesh->m_CVertexArray[i].dZ );
        
		glColor3f(1, 0, 0);
		glPointSize(5.0f);
		//glEnable(GL_SMOOTH);
		glBegin(GL_POINTS);
		  glVertex3f(x, y, z);		
		glEnd();
	}
	glPopMatrix();

	glPointSize(1.0f);

}

	
  //## end COpenGL3DEx::Draw2DMeshEdge%418843CB01ED.body
}

/*
void COpenGL3DEx::DrawStatistics ()
{
  //## begin COpenGL3DEx::DrawStatistics%418843E602B4.body preserve=yes
//void COpenGL3DEx::DrawStatistics()
{	
	
	glLoadIdentity();

	//if (m_pCMesh->m_pCRelxationLabelling != NULL)
	//if (m_pCMesh != NULL)
	{
		CString strAverage;
		CString strConv;
		CString strGeoStretch;
		
		//strAverage = g_CGlobals.m_pCMy3dViewerDoc->m_pCRelxationLabelling->GetStrAverageRatio();
		strAverage = m_pCMesh->GetStrAverageRatio();
		strConv = m_pCMesh->GetStrConvarianceRatio();
		strGeoStretch = m_pCMesh->GetStrGeoRation();
		
		//strAverage.Format("%lf", 
		//	m_pCMesh->m_pCRelxationLabelling->GetAvarageOfRatio());
		//strConv.Format("%lf",
		//	m_pCMesh->m_pCRelxationLabelling->GetCovOfRatio());
		//strGeoStretch.Format("%lf", m_pCMesh->GetGeometryStretch());
		
		glPushMatrix();
		
		glColor3f(1, 1, 0);
		
		glRasterPos3f((GLfloat)-0.6, (GLfloat)-1.1, (GLfloat)-0.5);
		PrintString("Average of Stretch Ratio: " + strAverage);
		
		glRasterPos3f((GLfloat)-0.6, (GLfloat)-1.2, (GLfloat)-0.5);
		PrintString("Covariance of Stretch Ratio: " + strConv);
		
		glRasterPos3f((GLfloat)-0.6, (GLfloat)-1.3, (GLfloat)-0.5);
		PrintString("Geometry Stretch Ratio: " + strGeoStretch);
		
		glPopMatrix();
	}
}
  //## end COpenGL3DEx::DrawStatistics%418843E602B4.body
}
*/

void COpenGL3DEx::SetMeshFlag (bool   bIsNew)
{
  //## begin COpenGL3DEx::SetMeshFlag%418843EB031F.body preserve=yes

//void COpenGL3DEx::SetMeshFlag(bool bIsNew)
{
	m_bIsNewMesh = bIsNew;	
}

	
  //## end COpenGL3DEx::SetMeshFlag%418843EB031F.body
}

// Additional Declarations
  //## begin COpenGL3DEx%40EB695901F5.declarations preserve=yes

void COpenGL3DEx::Draw3DMeshWithStrethRatio()
{

		int n = this->m_pCMesh->m_CTriangleArray.GetSize();
		
		for( int i=0; i<n; i++ )
		{
			COLORREF c12, c31, c23;
			
			float a[3],b[3],c[3];
			a[0] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dX );
			a[1] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dY );
			a[2] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dZ );
			
			b[0] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dX );
			b[1] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dY );
			b[2] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dZ );
			
			c[0] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dX );
			c[1] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dY );
			c[2] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dZ );
			
			
			float norm[3];
			norm[0] = (float)sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
			norm[1] = (float)sqrt(b[0]*b[0] + b[1]*b[1] + b[2]*b[2]);
			norm[2] = (float)sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
			
			for(int ii=0; ii<3; ii++)
			{
				a[ii] = (float)(( a[ii] / norm[0] )*0.5);
				b[ii] = (float)(( b[ii] / norm[1] )*0.5);
				c[ii] = (float)(( c[ii] / norm[2] )*0.5);
			}
									
			//if ( this->m_bLightingOn)
			
			double p[3];
			double q[3];
			double r[3];
			p[0] = a[0] - b[0];
			p[1] = a[1] - b[1];
			p[2] = a[2] - b[2];
			
			q[0] = c[0] - b[0];
			q[1] = c[1] - b[1];
			q[2] = c[2] - b[2];
			
			this->cross_product (p, q, r);
			
			if( this->normalize(r,3) )
			{
				//SetFaceMode();
				glNormal3f( (float)(-r[0]), (float)(-r[1]), (float)(-r[2]) );
				glEnable(GL_NORMALIZE );
				
				c12 = this->m_pCMesh->m_CTriangleArray[i].dC12;
				c31 = this->m_pCMesh->m_CTriangleArray[i].dC31;
				c23 = this->m_pCMesh->m_CTriangleArray[i].dC23;
				
				float cr1, cg1 ,cb1;
				float cr2, cg2 ,cb2;
				float cr3, cg3 ,cb3;
				
				float cr12,cg12,cb12;
				cr12 = (float)GetRValue(c12)/(float)255.0;
				cg12 = (float)GetGValue(c12)/(float)255.0;
				cb12 = (float)GetBValue(c12)/(float)255.0;
				
				float cr31,cg31,cb31;
				cr31 = (float)GetRValue(c31)/(float)255.0;
				cg31 = (float)GetGValue(c31)/(float)255.0;
				cb31 = (float)GetBValue(c31)/(float)255.0;
				
				float cr23,cg23,cb23;
				cr23 = (float)GetRValue(c23)/(float)255.0;
				cg23 = (float)GetGValue(c23)/(float)255.0;
				cb23 = (float)GetBValue(c23)/(float)255.0;
				
				cr1 = (cr12 + cr31) / 2;
				cr2 = (cr12 + cr23) / 2;
				cr3 = (cr23 + cr31) / 2;
				
				cg1 = (cg12 + cg31) / 2;
				cg2 = (cg12 + cg23) / 2;
				cg3 = (cg23 + cg31) / 2;
				
				cb1 = (cb12 + cb31) / 2;
				cb2 = (cb12 + cb23) / 2;
				cb3 = (cb23 + cb31) / 2;
				
				
				glBegin(GL_TRIANGLES);
				//glBegin(GL_POINTS);
				glColor3f(cr1, cg1, cb1);
				glVertex3f(a[0], a[1], a[2]);
				glColor3f(cr2, cg2, cb2);
				glVertex3f(b[0], b[1], b[2]);
				glColor3f(cr3, cg3, cb3);
				glVertex3f(c[0], c[1], c[2]);
				glEnd();	
			}
		}	
		
		if(m_bIsShowLabel)
			DrawLabelPts();

}
//## end COpenGL3DEx%40EB695901F5.declarations
//## begin module%40EB6BE5029D.epilog preserve=yes
void COpenGL3DEx::DrawArc(float a1, float a2, float a3, 
						  float b1, float b2, float b3)
{

	float a[3],b[3];
	a[0] = a1;
	a[1] = a2;
	a[2] = a3;
	b[0] = b1;
	b[1] = b2;
	b[2] = b3;

	#define PI 3.14159265358979323846
	
	struct POINT3D
	{
		double x,y,z;
	};

	//compute the angle between A([a1,a2,a3]) and B([b1,b2,b3])
	
    float norma = (float)sqrt(a1*a1 + a2*a2 + a3*a3);
	float normb = (float)sqrt(b1*b1 + b2*b2 + b3*b3);
	
	a1 = a1 / norma; 
	a2 = a2 / norma;
	a3 = a3 / norma;
	
	b1 = b1 / normb; 
	b2 = b2 / normb;
	b3 = b3 / normb;

	float dotv = a1*b1 + a2*b2 + a3*b3;

	float angle = (float)acos(dotv);

	//compute the normal;
	float n[3] ; 
    n[0] = a2*b3 - a3*b2;
	n[1] = a3*b1 - a1*b3;
	n[2] = a1*b2 - a2*b1;

	double A[3][3];
	memset(*A, 0 ,sizeof(double)*9);
	
	A[0][0] = a1;   A[0][1] = a2;    A[0][2] = a3;
	A[1][0] = b1;   A[1][1] = b2;    A[1][2] = b3;
	A[2][0] = n[0]; A[2][1] = n[1];  A[2][2] = n[2];
	invers_matrix(*A,3);
	
	//compute the points on the arc
	float fIntervalOfAngle =(float)(( 5.0 / 180.0 ) * PI );
	
	int   m = (int) ( angle / fIntervalOfAngle );
	POINT3D *points	 = new POINT3D[m+2];

	float angle1 = 0;

	for (int i=0; i<=m ; i++)	
	{
		angle1 = i * fIntervalOfAngle;
		double L[3];
		L[0] = cos(angle1);  L[1] = cos(angle - angle1);  L[2] = 0;
	    double P[3];
		mult(*A, L, P, 3, 3 ,1);		
		
		points[i].x = P[0] ;
		points[i].y = P[1] ;
		points[i].z = P[2] ;

		//float norm = sqrt(P[0]*P[0] + P[1]*P[1] + P[2]*P[2]);
	}

	points[0].x = a1; points[0].y = a2; points[0].z = a3; 
	points[m+1].x = b1; points[m+1].y = b2; points[m+1].z = b3; 

	//draw the arc
	//because the original data is normalized to 0.5, so we multiply 0.5
	for (int i=0; i<(m+1); i++ )
	{
		glBegin(GL_LINES);
			glVertex3f((GLfloat)points[i].x * (float)0.5, (GLfloat)points[i].y * (float)0.5, (GLfloat)points[i].z * (float)0.5);
			glVertex3f((GLfloat)points[i+1].x * (float)0.5, (GLfloat)points[i+1].y * (float)0.5, (GLfloat)points[i+1].z * (float)0.5);
		glEnd();
	}
	delete points;
}

void COpenGL3DEx::Draw3DMeshWithArc()
{
	{
		int n = this->m_pCMesh->m_CTriangleArray.GetSize();
		
		for( int i=0; i<n; i++ )
		{
			COLORREF c12, c31, c23;
			double   r12, r23, r31;
			
			float a[3],b[3],c[3];
			a[0] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dX ) ;
			a[1] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dY );
			a[2] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[0]].dZ );
									
			b[0] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dX );
			b[1] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dY );
			b[2] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[1]].dZ );
						
			c[0] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dX );
			c[1] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dY );
			c[2] = (float)( this->m_pCMesh->m_CVertexArray[this->m_pCMesh->m_CTriangleArray[i].pnV[2]].dZ );
	
			
			double p[3];
			double q[3];
			double r[3];
			p[0] = a[0] - b[0];
			p[1] = a[1] - b[1];
			p[2] = a[2] - b[2];
			
			q[0] = c[0] - b[0];
			q[1] = c[1] - b[1];
			q[2] = c[2] - b[2];
			
			this->cross_product (p, q, r);
			glNormal3f( (float)(-r[0]), (float)(-r[1]), (float)(-r[2]) );
			glEnable(GL_NORMALIZE );
						
			c12 = this->m_pCMesh->m_CTriangleArray[i].dC12;
			c31 = this->m_pCMesh->m_CTriangleArray[i].dC31;
			c23 = this->m_pCMesh->m_CTriangleArray[i].dC23;

			r12 = this->m_pCMesh->m_CTriangleArray[i].dfRatio12;
			r31 = this->m_pCMesh->m_CTriangleArray[i].dfRatio31;
			r23 = this->m_pCMesh->m_CTriangleArray[i].dfRatio23;


			float cr,cg,cb;
			cr = (float)GetRValue(c12)/(float)255.0;
			cg = (float)GetGValue(c12)/(float)255.0;
			cb = (float)GetBValue(c12)/(float)255.0;
		
			glColor3f(cr, cg, cb);
			glEnable(GL_LINE_STIPPLE);
			if (r12 < 1)
			{
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1, 0x8888);
			}
			else
			{
				glDisable(GL_LINE_STIPPLE);
			}			
			DrawArc(a[0], a[1], a[2], b[0], b[1], b[2]);
			
			cr = (float)GetRValue(c31)/(float)255.0;
			cg = (float)GetGValue(c31)/(float)255.0;
			cb = (float)GetBValue(c31)/(float)255.0;
		
			glColor3f(cr, cg, cb);
			if (r31 < 1)
			{
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1, 0x8888);
			}
			else
			{
				glDisable(GL_LINE_STIPPLE);
			}

			DrawArc(c[0], c[1], c[2], a[0], a[1], a[2]);

			cr = (float)GetRValue(c23)/(float)255.0;
			cg = (float)GetGValue(c23)/(float)255.0;
			cb = (float)GetBValue(c23)/(float)255.0;
			glColor3f(cr, cg, cb);
			if (r23 < 1)
			{
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1, 0x8888);
			}
			else
			{
				glDisable(GL_LINE_STIPPLE);
			}	

			DrawArc(c[0], c[1], c[2], b[0], b[1], b[2]);
		}			
	}

	glDisable(GL_LINE_STIPPLE);
}

void COpenGL3DEx::DrawGeoimgMesh()
{
   switch( ((C2DMesh*)m_pCMesh)->GetDispType() ) 
   {
   case 0:
   	   DrawMeshSurface();
	   break;
   case 1:
	   DrawMeshSurface();
	   ((C2DMesh*)m_pCMesh)->DisplayCutpath();
	   break;
   case 2:
	   DrawMeshSurface();
	   ((C2DMesh*)m_pCMesh)->DisplayCutpath();
	   break;
   case 3:
	   //DrawMeshSurface();
	   ((C2DMesh*)m_pCMesh)->DisplayBoundaryMap();
	   break;
   case 4:
	   //DrawMeshSurface();
	   ((C2DMesh*)m_pCMesh)->DisplayInterior2();
   	   break;
   case 5:
	   ((C2DMesh*)m_pCMesh)->DisplayGeoImage();
 	   break;
   case 6:
	   //((C2DMesh*)m_pCMesh)->DisplayReconstruct();
	   DrawMeshSurface();
	   break;
   default:
	   break;
   }
		
}

void COpenGL3DEx::DrawLabelPts()
{
	if(m_pCMesh != NULL)
	{
		if( &m_pCMesh->GetLabel() != NULL )
		{			
			int nPtNum = m_pCMesh->GetLabel().GetSize();
			
			for(int i=0; i<nPtNum; i++)
			{			
				POINT3D p = m_pCMesh->GetLabel().GetPt(i);
				
				glPointSize(1.0f);
				//glNormal3f(0,0,1);
				glColor3f(1,1,0);
				glBegin(GL_POINTS);
				glVertex3f(p.x *(float)0.5 , p.y*(float)0.5 , p.z*(float)0.5 );
				glEnd();
			}
		}
	}
}

void COpenGL3DEx::DrawText(CString title, CString txt, float x, float y, float z)
{
	//glNormal3f(0,0,1);
	glPushMatrix();
		glColor3f(1, 1, 0);
		glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
		PrintString(title + txt);
	glPopMatrix();		
}

//## end module%40EB6BE5029D.epilog

