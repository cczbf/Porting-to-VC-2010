#include "stdafx.h"
#include "COpenGL3D.h"

COpenGL3D::COpenGL3D()
{
	m_p3D		= NULL;
	m_DataGroup	= G3D;
}

void COpenGL3D::OpenGLDrawMap()
{
	if(m_p3D->m_MapType != FLAT)	// Draw 3D
	{
		glTranslated(m_p3D->m_xTrans, m_p3D->m_yTrans, m_p3D->m_zTrans);
		glRotatef(m_p3D->m_xRotat, 1.0, 0.0, 0.0);
		glRotatef(m_p3D->m_yRotat, 0.0, 1.0, 0.0);
		glRotatef(m_p3D->m_zRotat, 0.0, 0.0, 1.0);
		if(m_p3D->m_xRotat > 360)	m_p3D->m_xRotat -= 360;
		if(m_p3D->m_xRotat < -360)	m_p3D->m_xRotat += 360;
		if(m_p3D->m_yRotat > 360)	m_p3D->m_yRotat -= 360;
		if(m_p3D->m_yRotat < -360)	m_p3D->m_yRotat += 360;
		if(m_p3D->m_zRotat > 360)	m_p3D->m_zRotat -= 360;
		if(m_p3D->m_zRotat < -360)	m_p3D->m_zRotat += 360;
		glScalef(m_p3D->m_xScale, m_p3D->m_yScale, m_p3D->m_zScale);
	}
	else	//Draw 2D
	{
		glTranslated(m_p3D->m_xTrans, m_p3D->m_yTrans, m_p3D->m_zTrans);
		glRotatef(0, 1.0, 0.0, 0.0);
		glRotatef(0, 0.0, 1.0, 0.0);
		glRotatef(0, 0.0, 0.0, 1.0);
		glScalef(m_p3D->m_xScale, m_p3D->m_yScale, 0);
	}

	switch(m_p3D->m_Move)
	{
		case MOVE:
		case RANDROTAT:
			glOrtho(-m_fC + m_p3D->m_fDx, m_fC + m_p3D->m_fDx, 
					-m_fC + m_p3D->m_fDy, m_fC + m_p3D->m_fDy, 1., -1.);
			break;
		case CENTERROTAT:
			break;	//Centre rotation: do not call glOrtho
	}

	if(m_p3D->m_bAxes)		DrawAxis();
	if(m_p3D->m_bBorder)	DrawBorder();
	if(m_p3D->m_bSide)		DrawSide();
	if(m_p3D->m_bBase)		DrawBase();
	if(m_p3D->m_bMark)		DrawMarker();
	switch(m_p3D->m_MapType)
	{
		case FLAT:
			DrawFlat();
			break;
		case GRID:
			DrawGrid();
			break;
		case SURFACE:
			DrawSurface();
			break;
	}
	if(m_p3D->m_bLegend)	DrawLegend();  // Must keep to draw legend the last
}

void COpenGL3D::GDIDrawMap()
{

}

void COpenGL3D::DrawBase()
{
	glBegin(GL_QUADS);
		ColorLevel(-m_fC);
		glVertex3f(-m_fC, -m_fC, -m_fC);
		glVertex3f(m_fC, -m_fC, -m_fC);
		glVertex3f(m_fC, m_fC, -m_fC);
		glVertex3f(-m_fC, m_fC, -m_fC);
	glEnd();
}

void COpenGL3D::DrawBox()
{
	Draw3DLine(-m_fC, -m_fC, -m_fC, -m_fC, -m_fC, 6 * m_fC / 5);
	Draw3DLine(-m_fC, m_fC, -m_fC, -m_fC, m_fC,   6 * m_fC / 5);
	Draw3DLine(m_fC, m_fC, -m_fC, m_fC, m_fC,     6 * m_fC / 5);
	Draw3DLine(m_fC, -m_fC, -m_fC, m_fC, -m_fC,   6 * m_fC / 5);

	glBegin( GL_LINE_STRIP );
		glVertex3f( -m_fC, -m_fC, 6 * m_fC / 5);
		glVertex3f( -m_fC, m_fC,  6 * m_fC / 5);
		glVertex3f( m_fC, m_fC,   6 * m_fC / 5);
		glVertex3f( m_fC, -m_fC,  6 * m_fC / 5);
		glVertex3f( -m_fC, -m_fC, 6 * m_fC / 5);
	glEnd();
}

void COpenGL3D::Legend(RECT& rt)
{
	char Tstr[12];
	float oldMin = m_p3D->m_fZmin;
	m_p3D->m_fZmin = m_p3D->m_fZmin + (m_p3D->m_fCrThreshold + 0.5f) *
						(m_p3D->m_fZmax - m_p3D->m_fZmin);
	if (m_p3D->m_fZmin < 0.000001 && m_p3D->m_fZmin > -0.000001) m_p3D->m_fZmin = 0;

	if(m_p3D->m_bAutoCr)
	{
		int i;
		for( int i=0; i<m_p3D->m_nCrLevel; i++ )
		{
			glColor3f(m_p3D->m_MinCr.r + i * (m_p3D->m_MaxCr.r-m_p3D->m_MinCr.r) / 
					 (m_p3D->m_nCrLevel - 1),
					  m_p3D->m_MinCr.g + i * (m_p3D->m_MaxCr.g-m_p3D->m_MinCr.g) / 
					 (m_p3D->m_nCrLevel - 1),
					  m_p3D->m_MinCr.b + i * (m_p3D->m_MaxCr.b-m_p3D->m_MinCr.b) / 
					 (m_p3D->m_nCrLevel - 1));

			glBegin(GL_QUADS);
				glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs, 
						  (rt.bottom - rt.top - m_nD * m_p3D->m_nCrLevel) / 2 + 
						  i * m_nD + m_p3D->m_yLgdOs, 0);
				glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs, 
						  (rt.bottom - rt.top - m_nD * m_p3D->m_nCrLevel) / 2 + 
						  (i + 1) * m_nD + m_p3D->m_yLgdOs, 0);
				glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs + 2 * m_nD, 
						  (rt.bottom - rt.top - m_nD * m_p3D->m_nCrLevel) / 2 + 
						  (i + 1) * m_nD + m_p3D->m_yLgdOs, 0);
				glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs + 2 * m_nD, 
						  (rt.bottom - rt.top - m_nD * m_p3D->m_nCrLevel) / 2 + 
						  i * m_nD + m_p3D->m_yLgdOs, 0);
			glEnd();
		}
		glColor3f(m_p3D->m_BorderCr.r, m_p3D->m_BorderCr.g, m_p3D->m_BorderCr.b);
		glBegin(GL_LINE_LOOP);
			glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs - 1, 
					  (rt.bottom - rt.top - m_nD * m_p3D->m_nCrLevel) / 2 - 1 + 
					  m_p3D->m_yLgdOs, 0);
			glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs - 1, 
					  (rt.bottom - rt.top - m_nD * m_p3D->m_nCrLevel) / 2 + 
					  i * m_nD + 1 + m_p3D->m_yLgdOs, 0);
			glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs + 2 * m_nD + 1, 
					  (rt.bottom - rt.top - m_nD * m_p3D->m_nCrLevel) / 2 + 
					  i * m_nD + 1 + m_p3D->m_yLgdOs, 0);
			glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs + 2 * m_nD + 1, 
					  (rt.bottom - rt.top - m_nD * m_p3D->m_nCrLevel) / 2 - 1 + 
					  m_p3D->m_yLgdOs, 0);
		glEnd();
		for(int i=0; i<=m_p3D->m_nCrLevel; i++)
		{
			glRasterPos2i(rt.right-rt.left + m_p3D->m_xLgdOs + 5 * m_nD / 2,
					     (rt.bottom-rt.top - m_nD * m_p3D->m_nCrLevel) / 2 +
					     i * m_nD - m_nD / 4 + m_p3D->m_yLgdOs);
			sprintf(Tstr, "%-10.2f", m_p3D->m_fZmin + 
					i * (m_p3D->m_fZmax - m_p3D->m_fZmin) / m_p3D->m_nCrLevel);
			PrintString(Tstr);
		}
	}
	else
	{
		int i;
		for(int i=0; i<m_p3D->m_nUserCrNum; i++)
		{
			glColor3f(GetRValue(m_p3D->m_UserCr.GetAt(i)) / 255.f, 
					  GetGValue(m_p3D->m_UserCr.GetAt(i)) / 255.f,
					  GetBValue(m_p3D->m_UserCr.GetAt(i)) / 255.f);

			glBegin(GL_QUADS);
				glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs, 
						  (rt.bottom - rt.top - m_nD * m_p3D->m_nUserCrNum) / 2 + 
						   i * m_nD + m_p3D->m_yLgdOs, 0);
				glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs, 
						  (rt.bottom - rt.top - m_nD * m_p3D->m_nUserCrNum) / 2 + 
						  (i + 1) * m_nD + m_p3D->m_yLgdOs, 0);
				glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs + 2 * m_nD, 
						  (rt.bottom - rt.top - m_nD * m_p3D->m_nUserCrNum) / 2 + 
						  (i + 1) * m_nD + m_p3D->m_yLgdOs, 0);
				glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs + 2 * m_nD, 
						  (rt.bottom - rt.top - m_nD * m_p3D->m_nUserCrNum) / 2 + 
						  i * m_nD + m_p3D->m_yLgdOs, 0);
			glEnd();
		}
		glColor3f(m_p3D->m_BorderCr.r, m_p3D->m_BorderCr.g, m_p3D->m_BorderCr.b);
		glBegin(GL_LINE_LOOP);
			glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs - 1, 
					  (rt.bottom - rt.top - m_nD * m_p3D->m_nUserCrNum) / 2 + 
					  m_p3D->m_yLgdOs, 0);
			glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs - 1, 
					  (rt.bottom - rt.top - m_nD * m_p3D->m_nUserCrNum) / 2 + 
					  i * m_nD + m_p3D->m_yLgdOs, 0 );
			glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs + 2 * m_nD + 1, 
					  (rt.bottom - rt.top - m_nD * m_p3D->m_nUserCrNum) / 2 + 
					  i * m_nD + m_p3D->m_yLgdOs, 0);
			glVertex3i(rt.right-rt.left + m_p3D->m_xLgdOs + 2 * m_nD + 1, 
					  (rt.bottom - rt.top - m_nD * m_p3D->m_nUserCrNum) / 2 + 
					  m_p3D->m_yLgdOs, 0);
		glEnd();
		for(int i=0; i<=m_p3D->m_nUserCrNum; i++)
		{
			glRasterPos2i(rt.right-rt.left + m_p3D->m_xLgdOs + 5 * m_nD / 2,
						 (rt.bottom-rt.top - m_nD * m_p3D->m_nUserCrNum) / 2 +
						 i * m_nD - m_nD / 4 + m_p3D->m_yLgdOs);
			sprintf(Tstr, "%-10.2f", m_p3D->m_fZmin + 
					i * (m_p3D->m_fZmax - m_p3D->m_fZmin) / m_p3D->m_nUserCrNum);
			PrintString(Tstr);
		}
	}
	m_p3D->m_fZmin = oldMin;
}

void COpenGL3D::DrawFlat()
{
	if (m_DataGroup != GTRUE3D)
	{
		for(int j=0; j<m_p3D->nY - 1; j++)
			for(int i=0; i<m_p3D->nX - 1; i++)
		{
			glBegin(GL_QUADS);
				ColorLevel(m_p3D->m_XYZ.z[j*m_p3D->nX+i]);
				glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j], -m_fC);
				ColorLevel(m_p3D->m_XYZ.z[j*m_p3D->nX+i+1]);
				glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j], -m_fC);
				ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i+1]);
				glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j+1], -m_fC);
				ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i]);
				glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j+1], -m_fC);
			glEnd();
		}
	}
	else
	{
		for(int j=0; j<m_p3D->nY - 1; j++)
			for(int i=0; i<m_p3D->nX - 1; i++)
		{
			glBegin(GL_QUADS);
				ColorLevel(m_p3D->m_XYZ.w[j*m_p3D->nX+i]);
				glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j], -m_fC);
				ColorLevel(m_p3D->m_XYZ.w[j*m_p3D->nX+i+1]);
				glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j], -m_fC);
				ColorLevel(m_p3D->m_XYZ.w[(j+1)*m_p3D->nX+i+1]);
				glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j+1], -m_fC);
				ColorLevel(m_p3D->m_XYZ.w[(j+1)*m_p3D->nX+i]);
				glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j+1], -m_fC);
			glEnd();
		}
	}

	glColor3f(m_p3D->m_BorderCr.r, m_p3D->m_BorderCr.g, m_p3D->m_BorderCr.b );
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-m_fC, -m_fC, -m_fC);
	glVertex3f(m_fC, -m_fC, -m_fC);
	glVertex3f(m_fC, m_fC, -m_fC);
	glVertex3f(-m_fC, m_fC, -m_fC);
	glVertex3f(-m_fC, -m_fC, -m_fC);
	glEnd();
	glLineWidth(1);
}

void COpenGL3D::DrawSide()
{
	if(m_p3D->m_bAutoCr)
	{
		for( int i=0; i<m_p3D->nX-1; i++ )  // Y==-m_fC
		{
			glBegin(GL_QUADS);
			ColorLevel(-m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i], -m_fC, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], -m_fC, -m_fC);
			ColorLevel(m_p3D->m_XYZ.z[i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i+1], -m_fC, m_p3D->m_XYZ.z[i+1]);
			ColorLevel(m_p3D->m_XYZ.z[i]);
			glVertex3f(m_p3D->m_XYZ.x[i], -m_fC, m_p3D->m_XYZ.z[i] );
			glEnd();
		}

		for( int j=0; j<m_p3D->nY-1; j++ )	//X==m_fC
		{
			glBegin(GL_QUADS);
			ColorLevel(-m_fC);
			glVertex3f(m_fC, m_p3D->m_XYZ.y[j], -m_fC);
			glVertex3f(m_fC, m_p3D->m_XYZ.y[j+1], -m_fC);
			ColorLevel(m_p3D->m_XYZ.z[(j+2)*m_p3D->nX-1]);
			glVertex3f(m_fC, m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+2)*m_p3D->nX-1]);
			ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX-1]);
			glVertex3f(m_fC, m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX-1]);
			glEnd();
		}

		for(int i=0; i<m_p3D->nX-1; i++ )	// Y==m_fC
		{
			glBegin(GL_QUADS);
			ColorLevel(-m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i], m_fC, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_fC, -m_fC);
			ColorLevel(m_p3D->m_XYZ.z[m_p3D->nX*(m_p3D->nY-1)+i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_fC, m_p3D->m_XYZ.z[m_p3D->nX*(m_p3D->nY-1)+i+1]);
			ColorLevel(m_p3D->m_XYZ.z[m_p3D->nX*(m_p3D->nY-1)+i]);
			glVertex3f(m_p3D->m_XYZ.x[i], m_fC, m_p3D->m_XYZ.z[m_p3D->nX*(m_p3D->nY-1)+i]);
			glEnd();
		}
		
		for(int j=0; j<m_p3D->nY-1; j++ )	//	X==-m_fC
		{
			glBegin(GL_QUADS);
			ColorLevel(-m_fC);
			glVertex3f(-m_fC, m_p3D->m_XYZ.y[j], -m_fC);
			glVertex3f(-m_fC, m_p3D->m_XYZ.y[j+1], -m_fC);
			ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX]);
			glVertex3f(-m_fC, m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX]);
			ColorLevel(m_p3D->m_XYZ.z[j*m_p3D->nX]);
			glVertex3f(-m_fC, m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX]);
			glEnd();
		}
	}
	else
	{
		glColor3f(m_p3D->m_BorderCr.r, m_p3D->m_BorderCr.g, m_p3D->m_BorderCr.b);
		for( int i=0; i<m_p3D->nX-1; i++ )  // Y==-m_fC
		{
			glBegin(GL_QUADS);
			glVertex3f(m_p3D->m_XYZ.x[i], -m_fC, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], -m_fC, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], -m_fC, m_p3D->m_XYZ.z[i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i], -m_fC, m_p3D->m_XYZ.z[i] );
			glEnd();
		}

		for( int j=0; j<m_p3D->nY-1; j++ )	//X==m_fC
		{
			glBegin(GL_QUADS);
			glVertex3f(m_fC, m_p3D->m_XYZ.y[j], -m_fC);
			glVertex3f(m_fC, m_p3D->m_XYZ.y[j+1], -m_fC);
			glVertex3f(m_fC, m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+2)*m_p3D->nX-1]);
			glVertex3f(m_fC, m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX-1]);
			glEnd();
		}

		for(int i=0; i<m_p3D->nX-1; i++ )	// Y==m_fC
		{
			glBegin(GL_QUADS);
			glVertex3f(m_p3D->m_XYZ.x[i], m_fC, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_fC, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_fC, m_p3D->m_XYZ.z[m_p3D->nX*(m_p3D->nY-1)+i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i], m_fC, m_p3D->m_XYZ.z[m_p3D->nX*(m_p3D->nY-1)+i]);
			glEnd();
		}
		
		for(int j=0; j<m_p3D->nY-1; j++ )	//	X==-m_fC
		{
			glBegin(GL_QUADS);
			glVertex3f(-m_fC, m_p3D->m_XYZ.y[j], -m_fC);
			glVertex3f(-m_fC, m_p3D->m_XYZ.y[j+1], -m_fC);
			glVertex3f(-m_fC, m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX]);
			glVertex3f(-m_fC, m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX]);
			glEnd();
		}

		glColor3f(m_p3D->m_BackCr.r, m_p3D->m_BackCr.g, m_p3D->m_BackCr.b);
		for(int i=0; i<m_p3D->nX-1; i++ )  // Y==-m_fC
		{
			glBegin(GL_LINE_LOOP);
			glVertex3f(m_p3D->m_XYZ.x[i], -m_fC-m_fDelta, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], -m_fC-m_fDelta, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], -m_fC-m_fDelta, m_p3D->m_XYZ.z[i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i], -m_fC-m_fDelta, m_p3D->m_XYZ.z[i] );
			glEnd();
		}

		for(int j=0; j<m_p3D->nY-1; j++ )	//X==m_fC
		{
			glBegin(GL_LINE_LOOP);
			glVertex3f(m_fC+m_fDelta, m_p3D->m_XYZ.y[j], -m_fC);
			glVertex3f(m_fC+m_fDelta, m_p3D->m_XYZ.y[j+1], -m_fC);
			glVertex3f(m_fC+m_fDelta, m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+2)*m_p3D->nX-1]);
			glVertex3f(m_fC+m_fDelta, m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX-1]);
			glEnd();
		}

		for(int i=0; i<m_p3D->nX-1; i++ )	// Y==m_fC
		{
			glBegin(GL_LINE_LOOP);
			glVertex3f(m_p3D->m_XYZ.x[i], m_fC+m_fDelta, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_fC+m_fDelta, -m_fC);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_fC+m_fDelta, m_p3D->m_XYZ.z[m_p3D->nX*(m_p3D->nY-1)+i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i], m_fC+m_fDelta, m_p3D->m_XYZ.z[m_p3D->nX*(m_p3D->nY-1)+i]);
			glEnd();
		}
		
		for(int j=0; j<m_p3D->nY-1; j++ )	//	X==-m_fC
		{
			glBegin(GL_LINE_LOOP);
			glVertex3f(-m_fC-m_fDelta, m_p3D->m_XYZ.y[j], -m_fC);
			glVertex3f(-m_fC-m_fDelta, m_p3D->m_XYZ.y[j+1], -m_fC);
			glVertex3f(-m_fC-m_fDelta, m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX]);
			glVertex3f(-m_fC-m_fDelta, m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX]);
			glEnd();
		}
	}
}

void COpenGL3D::DrawSurface()
{
	if (m_DataGroup != GTRUE3D)
	{
		for( int j=0; j<m_p3D->nY-1; j++ )
			for( int i=0; i<m_p3D->nX-1; i++ )
		{
			glBegin(GL_QUADS);
			if ( this->m_bLightingOn)
			{
				double p[3];
				double q[3];
				double r[3];
				p[0] = m_p3D->m_XYZ.x[i] - m_p3D->m_XYZ.x[i+1];
				p[1] = m_p3D->m_XYZ.y[j] - m_p3D->m_XYZ.y[j];
				p[2] = m_p3D->m_XYZ.z[j*m_p3D->nX+i] - m_p3D->m_XYZ.z[j*m_p3D->nX+i+1];
			
				q[0] = m_p3D->m_XYZ.x[i+1] - m_p3D->m_XYZ.x[i+1];
				q[1] = m_p3D->m_XYZ.y[j+1] - m_p3D->m_XYZ.y[j];
				q[2] = m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i+1] - m_p3D->m_XYZ.z[j*m_p3D->nX+i+1];
			
				this->cross_product (p, q, r);
				this->normalize(r,3);
				double z[3] = {0.0,0.0,1.0};
				if (pp(r,z,3)>0)
					glNormal3d( r[0], r[1], r[2] );
				else
					glNormal3d( -r[0], -r[1], -r[2] );
  			}	
			ColorLevel(m_p3D->m_XYZ.z[j*m_p3D->nX+i]);
			glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX+i]);//1
			ColorLevel(m_p3D->m_XYZ.z[j*m_p3D->nX+i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX+i+1]);//2
			ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i+1]);//3
			ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i]);
			glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i]);//4
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

void COpenGL3D::DrawGrid()
{
	if (m_DataGroup != GTRUE3D)
	{
		for( int j=0; j<m_p3D->nY-1; j++ )
			for( int i=0; i<m_p3D->nX-1; i++ )
		{
			glBegin(GL_LINE_LOOP );
			ColorLevel(m_p3D->m_XYZ.z[j*m_p3D->nX+i]);
			glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX+i]);
			ColorLevel(m_p3D->m_XYZ.z[j*m_p3D->nX+i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j], m_p3D->m_XYZ.z[j*m_p3D->nX+i+1]);
			ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i+1]);
			glVertex3f(m_p3D->m_XYZ.x[i+1], m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i+1]);
			ColorLevel(m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i]);
			glVertex3f(m_p3D->m_XYZ.x[i], m_p3D->m_XYZ.y[j+1], m_p3D->m_XYZ.z[(j+1)*m_p3D->nX+i]);
			glEnd();
		}
	}
	else
	{
		for( int j=0; j<m_p3D->nY-1; j++ )
			for( int i=0; i<m_p3D->nX-1; i++ )
		{
			glBegin(GL_LINE_LOOP );
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

void COpenGL3D::DrawMarker()
{
	glColor3f(m_p3D->m_MarkerCr.r, m_p3D->m_MarkerCr.g, m_p3D->m_MarkerCr.b );
	int n = m_p3D->m_Marker.GetSize();
	if(n < 1) return;

	if(m_p3D->m_MapType == FLAT)
	{
		if(m_p3D->m_bLable)
		{
			for(int i=0; i<n; i++)
			{
				glRasterPos3f(m_p3D->m_Marker[i].x, m_p3D->m_Marker[i].y, -0.99f * m_fC);
				PrintString((const char*)(m_p3D->m_Marker[i].Name));
			}
		}
	}
	else
	{
		float ft;
		if(m_p3D->m_bMarkLine)
		{
			for(int i=0; i<n; i++)
			{
				ft = 2.4f * m_fC * m_p3D->m_Marker[i].Hight - m_fC;
				Draw3DLine(m_p3D->m_Marker[i].x, m_p3D->m_Marker[i].y, -0.99f * m_fC, m_p3D->m_Marker[i].x, m_p3D->m_Marker[i].y, ft );
			}
		}
		if(m_p3D->m_bLable)
		{
			for(int i=0; i<n; i++)
			{
				ft = 2.4f * m_fC * m_p3D->m_Marker[i].Hight - m_fC;
				glRasterPos3f(m_p3D->m_Marker[i].x, m_p3D->m_Marker[i].y, ft);
				PrintString((const char*)(m_p3D->m_Marker[i].Name));
			}
		}
	}
}

void COpenGL3D::Init3DDataSet(int col, int row, int group, int n)
{
	for(int k=0; k<n; k++)
	{
		m_DS[k].nDataGroup = group;

		m_DS[k].nX		= col;
		m_DS[k].nY		= row;

		m_DS[k].m_XYZ.x = new float[col];
		m_DS[k].m_XYZ.y = new float[row];
		m_DS[k].m_XYZ.z = new float[col*row];
		if (group == GTRUE3D)
			m_DS[k].m_XYZ.w = new float[col*row];

		m_DS[k].m_fXmin = 1.e+30f;
		m_DS[k].m_fXmax = -1.e+30f;
		m_DS[k].m_fYmin = 1.e+30f;
		m_DS[k].m_fYmax = -1.e+30f;
		m_DS[k].m_fZmin = 1.e+30f;
		m_DS[k].m_fZmax = -1.e+30f;
		m_DS[k].m_fWmin = 1.e+30f;
		m_DS[k].m_fWmax = -1.e+30f;

		m_DS[k].m_nCrLevel   = 10;
		m_DS[k].m_nUserCrNum = 10;
		m_DS[k].m_bAutoCr    = 1;
		m_DS[k].m_MapType    = SURFACE;

		m_DS[k].m_bAxes      = 1;
		m_DS[k].m_bBase		 = 1;
		m_DS[k].m_bSide		 = 1;
		m_DS[k].m_bLegend	 = 0;
		m_DS[k].m_bBorder    = 1;
		m_DS[k].m_bMark		 = 0;

		m_DS[k].m_bLable	 = 1;
		m_DS[k].m_nMarkIndex = 0;
		m_DS[k].m_bMarkLine	 = 1;

		m_DS[k].m_Rotate	 = RZ;
		m_DS[k].m_Move		 = MOVE;
		m_DS[k].m_fDx		 = 0.0f;
		m_DS[k].m_fDy		 = 0.0f;
		m_DS[k].m_xLgdOs	 = 50;
		m_DS[k].m_yLgdOs	 = 0;

		m_DS[k].m_xRotat	 = -70;
		m_DS[k].m_yRotat	 = 0;
		m_DS[k].m_zRotat	 = 0;

		m_DS[k].m_xTrans	 = 0;
		m_DS[k].m_yTrans	 = 0;
		m_DS[k].m_zTrans	 = -4;

		m_DS[k].m_xScale	 = 1;
		m_DS[k].m_yScale	 = 1;
		m_DS[k].m_zScale	 = 1;

		m_DS[k].m_Link		 = NoLink;

		m_DS[k].m_UserCr.SetSize(20);
		m_DS[k].m_UserCr[0]  = RGB(0, 0, 0);
		m_DS[k].m_UserCr[1]  = RGB(255, 255, 0);
		m_DS[k].m_UserCr[2]  = RGB(128, 128, 0);
		m_DS[k].m_UserCr[3]  = RGB(255, 0, 0);
		m_DS[k].m_UserCr[4]  = RGB(128, 0, 0);
		m_DS[k].m_UserCr[5]  = RGB(255, 0, 255);
		m_DS[k].m_UserCr[6]  = RGB(128, 0, 128);
		m_DS[k].m_UserCr[7]  = RGB(0, 0, 255);
		m_DS[k].m_UserCr[8]  = RGB(0, 0, 128);
		m_DS[k].m_UserCr[9]  = RGB(0, 255, 255);
		m_DS[k].m_UserCr[10] = RGB(0, 128, 128);
		m_DS[k].m_UserCr[11] = RGB(0, 255, 0);
		m_DS[k].m_UserCr[12] = RGB(0, 128, 0);
		m_DS[k].m_UserCr[13] = RGB(192, 192, 192);
		m_DS[k].m_UserCr[14] = RGB(128, 128, 128);
		m_DS[k].m_UserCr[15] = RGB(255, 255, 255);
		m_DS[k].m_UserCr[16] = RGB(255, 251, 240);
		m_DS[k].m_UserCr[17] = RGB(192, 220, 192);
		m_DS[k].m_UserCr[18] = RGB(166, 202, 240);
		m_DS[k].m_UserCr[19] = RGB(160, 160, 164);
	
		m_DS[k].m_BackCr.r   = 0;
		m_DS[k].m_BackCr.g   = 0;
		m_DS[k].m_BackCr.b   = 128 / 255.f;
		m_DS[k].m_MinCr.r    = 0;
		m_DS[k].m_MinCr.g    = 0;
		m_DS[k].m_MinCr.b    = 1;
		m_DS[k].m_MaxCr.r    = 1;
		m_DS[k].m_MaxCr.g    = 0;
		m_DS[k].m_MaxCr.b	 = 0;
		m_DS[k].m_BorderCr.r = 1;
		m_DS[k].m_BorderCr.g = 1;
		m_DS[k].m_BorderCr.b = 1.0f - 128 / 255.f;
		m_DS[k].m_MarkerCr.r	 = 1;
		m_DS[k].m_MarkerCr.g	 = 1;
		m_DS[k].m_MarkerCr.b	 = 1.0f - 128 / 255.f;
		m_DS[k].m_fCrThreshold	 = -m_fC;
	}
}

void COpenGL3D::Normalisation(int type, int n)
{
	for(int m=0; m<n; m++)
	{
		for(int k=0; k<m_DS[m].nX; k++)
		{
			if(m_DS[m].m_XYZ.x[k] < m_DS[m].m_fXmin)
				m_DS[m].m_fXmin = m_DS[m].m_XYZ.x[k];
			if(m_DS[m].m_XYZ.x[k] > m_DS[m].m_fXmax)
				m_DS[m].m_fXmax = m_DS[m].m_XYZ.x[k];
		}
		for(int k=0; k<m_DS[m].nY; k++)
		{
			if(m_DS[m].m_XYZ.y[k] < m_DS[m].m_fYmin)
				m_DS[m].m_fYmin = m_DS[m].m_XYZ.y[k];
			if(m_DS[m].m_XYZ.y[k] > m_DS[m].m_fYmax)
				m_DS[m].m_fYmax = m_DS[m].m_XYZ.y[k];
		}
		int j = m_DS[m].nY*m_DS[m].nX;
		for(int k=0; k<j; k++)
		{
			if(m_DS[m].m_XYZ.z[k] < m_DS[m].m_fZmin)
				m_DS[m].m_fZmin = m_DS[m].m_XYZ.z[k];
			if(m_DS[m].m_XYZ.z[k] > m_DS[m].m_fZmax)
				m_DS[m].m_fZmax = m_DS[m].m_XYZ.z[k];
		}
		if (type == GTRUE3D)
		{
			for(int k=0; k<j; k++)
			{
				if(m_DS[m].m_XYZ.w[k] < m_DS[m].m_fWmin)
					m_DS[m].m_fWmin = m_DS[m].m_XYZ.w[k];
				if(m_DS[m].m_XYZ.w[k] > m_DS[m].m_fWmax)
					m_DS[m].m_fWmax = m_DS[m].m_XYZ.w[k];
			}
		}
	}

	for(int m=0; m<n; m++)
	{
		for(int k=0; k<m_DS[m].nX; k++)
			m_DS[m].m_XYZ.x[k] = ( m_DS[m].m_XYZ.x[k] - 
				m_DS[m].m_fXmin) / (m_DS[m].m_fXmax - m_DS[m].m_fXmin) - m_fC;
		for(int k=0; k<m_DS[m].nY; k++)
			m_DS[m].m_XYZ.y[k] = ( m_DS[m].m_XYZ.y[k] - 
				m_DS[m].m_fYmin) / (m_DS[m].m_fYmax - m_DS[m].m_fYmin) - m_fC;
		int j = m_DS[m].nY * m_DS[m].nX;
		for(int k=0; k<j; k++)
			m_DS[m].m_XYZ.z[k] = ( m_DS[m].m_XYZ.z[k] - 
				m_DS[m].m_fZmin) / (m_DS[m].m_fZmax - m_DS[m].m_fZmin) - m_fC;
		if (type == GTRUE3D)
		{
			for(int k=0; k<j; k++)
				m_DS[m].m_XYZ.w[k] = ( m_DS[m].m_XYZ.w[k] - 
					m_DS[m].m_fWmin) / (m_DS[m].m_fWmax - m_DS[m].m_fWmin) - m_fC;
		}
	}
}

void COpenGL3D::LoadSurferBinaryData(FILE* fp)
{
	int n;
	short col, row;
	double x1, x2, y1, y2, z1, z2;
	fseek(fp, 4, SEEK_SET);
	fread(&col, 2, 1, fp);
	fread(&row, 2, 1, fp);
	fread(&x1, 8, 1, fp);
	fread(&x2, 8, 1, fp);
	fread(&y1, 8, 1, fp);
	fread(&y2, 8, 1, fp);
	fread(&z1, 8, 1, fp);
	fread(&z2, 8, 1, fp);
	
	m_DS.RemoveAll(); 
	m_p3D = NULL;
	m_pDS = NULL;
	n = 1;
	m_DS.SetSize(n);

	Init3DDataSet(col, row, G3D, n);

	m_DS[0].m_fXmin = (float)x1;
	m_DS[0].m_fXmax = (float)x2;
	m_DS[0].m_fYmin = (float)y1;
	m_DS[0].m_fYmax = (float)y2;
	m_DS[0].m_fZmin = (float)z1;
	m_DS[0].m_fZmax = (float)z2;
	m_DS[0].m_MapName = "Surfer Binary File";

	float tValue;
	for(int k=0; k<row; k++)
	{
		for(int j=0; j<col; j++)
		{
			int i = k * col + j;
			fread(&tValue, 4, 1, fp);
			m_DS[0].m_XYZ.z[i] = tValue;
		}
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

void COpenGL3D::LoadSurferTextData(FILE* fp)
{
	char ch[100];
	int n;
	int col, row;
	int k;

	fseek(fp, 0, SEEK_SET);
	fgets(ch, 100, fp);
	fscanf(fp, "%d%d\n", &col, &row);

	m_DS.RemoveAll(); 
	m_p3D = NULL;
	m_pDS = NULL;
	n = 1;
	m_DS.SetSize(n);

	Init3DDataSet(col, row, G3D, n);

	float tmin, tmax;
	fscanf(fp, "%f%f\n", &tmin, &tmax);
	m_DS[0].m_fXmin = (float)tmin;
	m_DS[0].m_fXmax = (float)tmax;
	fscanf(fp, "%f%f\n", &tmin, &tmax);
	m_DS[0].m_fYmin = (float)tmin;
	m_DS[0].m_fYmax = (float)tmax;
	fscanf(fp, "%f%f\n", &tmin, &tmax);
	m_DS[0].m_fZmin = (float)tmin;
	m_DS[0].m_fZmax = (float)tmax;
	m_DS[0].m_MapName = "Surfer text File";

	float tValue;
	for(int k=0; k<row; k++)
	{
		for(int j=0; j<col; j++)
		{
			int i = k * col + j;
			fscanf(fp, "%f", &tValue);
			m_DS[0].m_XYZ.z[i] = tValue;
		}
	}

	for(k=0; k<col; k++)
		m_DS[0].m_XYZ.x[k] = 1.0f * k / (col - 1) - m_fC;

	for(k=0; k<row; k++)
		m_DS[0].m_XYZ.y[k] = 1.0f * k / (row - 1) - m_fC;


	n = row * col;
	for(k=0; k<n; k++)
			m_DS[0].m_XYZ.z[k] = (m_DS[0].m_XYZ.z[k] - m_DS[0].m_fZmin) / 
								 (m_DS[0].m_fZmax - m_DS[0].m_fZmin) - m_fC;
}

void COpenGL3D::LoadSurferData(FILE* fp, char type)
{
	m_DataGroup = G3D;
	if( type == 'B')
		LoadSurferBinaryData(fp);
	else
		LoadSurferTextData(fp);

	m_p3D = &m_DS[0];
	m_pDS = m_p3D;
}

void COpenGL3D::Load3DMultipleMapsData(FILE* fp, char type)
{
	char ch[32];
	int m, n, col, row;
	int k;

	fread(&n, sizeof(int), 1, fp); // n: number of maps

	if (type == 'C')
	{
		m_DataGroup = G3D;
		m = n + 2;
	}
	else
	{
		m_DataGroup = GTRUE3D;
		m = n + 3;
	}

	m_DS.RemoveAll(); 
	m_p3D = NULL;
	m_pDS = NULL;
	m_DS.SetSize(n);

	for(int k=0; k<n; k++)
	{
		fread(ch, sizeof(char), 32, fp);
		int j = strlen(ch);
		for(int i = 0; i < j; i++)
		{
			if (ch[i] == '\r' || ch[i] == '\n')
			{
				ch[i] = '\0';
				break;
			}
		}
		m_DS[k].m_MapName = ch;
	}
	fread(&col, sizeof(int), 1, fp);
	fread(&row, sizeof(int), 1, fp);

	Init3DDataSet(col, row, m_DataGroup, n);

	float* tValue = new float[m];
	for(k=0; k<row; k++)
	{
		for(int j=0; j<col; j++)
		{
			int i = k * col + j;
			fread(tValue, sizeof(float), m, fp);
			for(int h=0; h<n; h++)
			{
				if(k == 0)
					m_DS[h].m_XYZ.x[j] = tValue[0];
				if(j == 0)
					m_DS[h].m_XYZ.y[k] = tValue[1];

				if (type == 'C')
					m_DS[h].m_XYZ.z[i] = tValue[2+h];
				else
				{
					m_DS[h].m_XYZ.z[i] = tValue[2];
					m_DS[h].m_XYZ.w[i] = tValue[3+h];
				}
			}
		}
	}
	delete []tValue;
	Normalisation(m_DataGroup, n);
	m_p3D = &m_DS[0];
	m_pDS = m_p3D;
}

void COpenGL3D::Load3DData(FILE* fp, char type)
{
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

void COpenGL3D::OpenMaps(unsigned int hHandle)
{
	DWORD  len;
	int	   m, n;
	HANDLE handle = (HANDLE)hHandle;
	::ReadFile(handle, &m, sizeof(int), &len, NULL);
	m_DS.SetSize(m);
	for(int k=0; k<m; k++)
	{
		char str[32];
		::ReadFile(handle, str, 32, &len, NULL);
		m_DS[k].m_MapName = str;
		::ReadFile(handle, &m_DS[k].nDataGroup, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].nX, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].nY, sizeof(int), &len, NULL);
		m_DS[k].m_XYZ.x = new float[m_DS[k].nX];
		m_DS[k].m_XYZ.y = new float[m_DS[k].nY];
		m_DS[k].m_XYZ.z = new float[m_DS[k].nX*m_DS[k].nY];
		::ReadFile(handle, m_DS[k].m_XYZ.x, sizeof(float) * m_DS[k].nX, &len, NULL);
		::ReadFile(handle, m_DS[k].m_XYZ.y, sizeof(float) * m_DS[k].nY, &len, NULL);
		::ReadFile(handle, m_DS[k].m_XYZ.z, sizeof(float) * m_DS[k].nX  * m_DS[k].nY, &len, NULL);
		if (m_DS[k].nDataGroup == GTRUE3D)
		{
			m_DS[k].m_XYZ.w = new float[m_DS[k].nX*m_DS[k].nY];
			::ReadFile(handle, m_DS[k].m_XYZ.w, sizeof(float) * m_DS[k].nX  * m_DS[k].nY, &len, NULL);
		}

		::ReadFile(handle, &m_DS[k].m_fXmin, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_fXmax, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_fYmin, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_fYmax, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_fZmin, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_fZmax, sizeof(float), &len, NULL);
		if (m_DS[k].nDataGroup == GTRUE3D)
		{
			::ReadFile(handle, &m_DS[k].m_fWmin, sizeof(float), &len, NULL);
			::ReadFile(handle, &m_DS[k].m_fWmax, sizeof(float), &len, NULL);
		}

		::ReadFile(handle, &m_DS[k].m_nCrLevel, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_nUserCrNum, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_bAutoCr, sizeof(bool), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_MapType, sizeof(CMapType), &len, NULL);

		::ReadFile(handle, &m_DS[k].m_bAxes, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_bBase, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_bSide, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_bLegend, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_bBorder, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_bMark, sizeof(int), &len, NULL);

		::ReadFile(handle, &m_DS[k].m_bLable, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_nMarkIndex, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_bMarkLine, sizeof(int), &len, NULL);

		::ReadFile(handle, &m_DS[k].m_Rotate, sizeof(CRotateType), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_Move, sizeof(CMoveType), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_fDx, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_fDy, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_xLgdOs, sizeof(int), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_yLgdOs, sizeof(int), &len, NULL);

		::ReadFile(handle, &m_DS[k].m_xRotat, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_yRotat, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_zRotat, sizeof(float), &len, NULL);

		::ReadFile(handle, &m_DS[k].m_xTrans, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_yTrans, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_zTrans, sizeof(float), &len, NULL);

		::ReadFile(handle, &m_DS[k].m_xScale, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_yScale, sizeof(float), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_zScale, sizeof(float), &len, NULL);

		::ReadFile(handle, &m_DS[k].m_Link, sizeof(CLinkType), &len, NULL);

		::ReadFile(handle, &n, sizeof(int), &len, NULL);
		m_DS[k].m_UserCr.SetSize(n);
		for(int i = 0; i < n; i++)
			::ReadFile(handle, &m_DS[k].m_UserCr[i], sizeof(DWORD), &len, NULL);
	
		::ReadFile(handle, &m_DS[k].m_BackCr, sizeof(CFRGB), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_MinCr, sizeof(CFRGB), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_MaxCr, sizeof(CFRGB), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_BorderCr, sizeof(CFRGB), &len, NULL);
		::ReadFile(handle, &m_DS[k].m_MarkerCr, sizeof(CFRGB), &len, NULL);

		::ReadFile(handle, &m_DS[k].m_fCrThreshold, sizeof(float), &len, NULL);

		/////////Marker structure
		::ReadFile(handle, &n, sizeof(int), &len, NULL);
		m_DS[k].m_Marker.SetSize(n);
		for(int i=0; i<n; i++)
		{
			::ReadFile(handle, &m_DS[k].m_Marker[i].x, sizeof(float), &len, NULL);
			::ReadFile(handle, &m_DS[k].m_Marker[i].y, sizeof(float), &len, NULL);
			::ReadFile(handle, &m_DS[k].m_Marker[i].Hight, sizeof(float), &len, NULL);
			::ReadFile(handle, &m_DS[k].m_Marker[i].Flag, sizeof(int), &len, NULL);
			char str[12];
			::ReadFile(handle, str, 12, &len, NULL);
			m_DS[k].m_Marker[i].Name = str;
		}
	}
	m_p3D = &m_DS[0];
	m_pDS = m_p3D;
}

void COpenGL3D::SaveMaps(unsigned int hHandle)
{
	int m = m_DS.GetSize();
	if(m < 1)   return;

	HANDLE handle = (HANDLE)hHandle;
	DWORD  len;
	::WriteFile(handle, m_Version, 12, &len, NULL);
	::WriteFile(handle, &m_DataGroup, sizeof(CDataGroup), &len, NULL);
	::WriteFile(handle, &m, sizeof(int), &len, NULL);
	for(int k=0; k<m; k++)
	{
		::WriteFile(handle, (char*)(LPCTSTR)m_DS[k].m_MapName, 32, &len, NULL);
		::WriteFile(handle, &m_DS[k].nDataGroup, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].nX, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].nY, sizeof(int), &len, NULL);
		::WriteFile(handle, m_DS[k].m_XYZ.x, sizeof(float) * m_DS[k].nX, &len, NULL);
		::WriteFile(handle, m_DS[k].m_XYZ.y, sizeof(float) * m_DS[k].nY, &len, NULL);
		::WriteFile(handle, m_DS[k].m_XYZ.z, sizeof(float) * m_DS[k].nX  * m_DS[k].nY, &len, NULL);
		if (m_DS[k].nDataGroup == GTRUE3D)
			::WriteFile(handle, m_DS[k].m_XYZ.w, sizeof(float) * m_DS[k].nX  * m_DS[k].nY, &len, NULL);

		::WriteFile(handle, &m_DS[k].m_fXmin, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_fXmax, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_fYmin, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_fYmax, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_fZmin, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_fZmax, sizeof(float), &len, NULL);
		if (m_DS[k].nDataGroup == GTRUE3D)
		{
			::WriteFile(handle, &m_DS[k].m_fWmin, sizeof(float), &len, NULL);
			::WriteFile(handle, &m_DS[k].m_fWmax, sizeof(float), &len, NULL);
		}

		::WriteFile(handle, &m_DS[k].m_nCrLevel, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_nUserCrNum, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_bAutoCr, sizeof(bool), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_MapType, sizeof(CMapType), &len, NULL);

		::WriteFile(handle, &m_DS[k].m_bAxes, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_bBase, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_bSide, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_bLegend, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_bBorder, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_bMark, sizeof(int), &len, NULL);

		::WriteFile(handle, &m_DS[k].m_bLable, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_nMarkIndex, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_bMarkLine, sizeof(int), &len, NULL);

		::WriteFile(handle, &m_DS[k].m_Rotate, sizeof(CRotateType), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_Move, sizeof(CMoveType), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_fDx, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_fDy, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_xLgdOs, sizeof(int), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_yLgdOs, sizeof(int), &len, NULL);

		::WriteFile(handle, &m_DS[k].m_xRotat, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_yRotat, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_zRotat, sizeof(float), &len, NULL);

		::WriteFile(handle, &m_DS[k].m_xTrans, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_yTrans, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_zTrans, sizeof(float), &len, NULL);

		::WriteFile(handle, &m_DS[k].m_xScale, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_yScale, sizeof(float), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_zScale, sizeof(float), &len, NULL);

		::WriteFile(handle, &m_DS[k].m_Link, sizeof(CLinkType), &len, NULL);

		int n = m_DS[k].m_UserCr.GetSize();
		::WriteFile(handle, &n, sizeof(int), &len, NULL);
		for(int i = 0; i < n; i++)
			::WriteFile(handle, &m_DS[k].m_UserCr[i], sizeof(DWORD), &len, NULL);
	
		::WriteFile(handle, &m_DS[k].m_BackCr, sizeof(CFRGB), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_MinCr, sizeof(CFRGB), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_MaxCr, sizeof(CFRGB), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_BorderCr, sizeof(CFRGB), &len, NULL);
		::WriteFile(handle, &m_DS[k].m_MarkerCr, sizeof(CFRGB), &len, NULL);

		::WriteFile(handle, &m_DS[k].m_fCrThreshold, sizeof(float), &len, NULL);

		/////////Marker structure
		n = m_DS[k].m_Marker.GetSize();
		::WriteFile(handle, &n, sizeof(int), &len, NULL);
		for(int i=0; i<n; i++)
		{
			::WriteFile(handle, &m_DS[k].m_Marker[i].x, sizeof(float), &len, NULL);
			::WriteFile(handle, &m_DS[k].m_Marker[i].y, sizeof(float), &len, NULL);
			::WriteFile(handle, &m_DS[k].m_Marker[i].Hight, sizeof(float), &len, NULL);
			::WriteFile(handle, &m_DS[k].m_Marker[i].Flag, sizeof(int), &len, NULL);
			::WriteFile(handle, (char*)(LPCTSTR)m_DS[k].m_Marker[i].Name, 12, &len, NULL);
		}
	}
}
