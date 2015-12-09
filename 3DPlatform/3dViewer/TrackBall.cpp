#include "stdafx.h"
#include "TrackBall.h"

#include "math.h"


#define M_PI 3.14159 /* if not in math.h */


CTrackBall::CTrackBall()
{
	m_winWidth = 0;
	m_winHeight = 0;

	m_lastPos[0] = 0;
	m_lastPos[1] = 0;
	m_lastPos[2] = 1;	

	trackingMouse = FALSE;
}

CTrackBall::~CTrackBall()
{

}

void CTrackBall::Trackball_ptov(int x, int y, int width, int height, float *v)
{
    float d, a;
    /* project x,y onto a hemisphere centered
	within width, height , note z is up here*/
    v[0] = ((float)2.0*x - width) / width;            //translate to the center of screen 
    v[1] = (height - (float)2.0F*y) / height;         //translate to the center of screen  
    d = (float)sqrt(v[0]*v[0] + v[1]*v[1]);          
    v[2] = (float)cos((M_PI/2.0) * ((d < 1.0) ? d : 1.0));
    a = (float)(1.0 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) );
    v[0] *= a;    v[1] *= a;    v[2] *= a;
}

void CTrackBall::StartMotion(CPoint pt)
{
  trackingMouse = true;
  redrawContinue = false;

  startX = pt.x;
  startY = pt.y;
  curx = pt.x;
  cury = pt.y;
  
  Trackball_ptov(pt.x, pt.y, m_winWidth, m_winHeight, m_lastPos);
    
  trackballMove = true;
}

void CTrackBall::StopMotion(int x, int y)
{
    trackingMouse = false;

  /* check if position has changed */
    if (startX != x || startY != y)
	     redrawContinue = true;
    else 
    {
	     //angle = 0.0;
	     redrawContinue = false;
	     trackballMove = false;
    }
}

void CTrackBall::SetSize(int w, int h)
{
	m_winHeight = h;
	m_winWidth = w;
}

void CTrackBall::Track(CPoint pt,float *axis,float &angle)
{
	if (trackingMouse)
	{		
		float curPos[3];
		float dx, dy, dz;
		
		int winWidth = m_winWidth;
		int winHeight= m_winHeight;
		
		/* compute position on hemisphere */
		Trackball_ptov(pt.x, pt.y, winWidth, winHeight, curPos);
		
		//if(trackingMouse)
		{    
		/* compute the change in position 
			on the hemisphere */
			dx = curPos[0]-m_lastPos[0];
			dy = curPos[1]-m_lastPos[1];
			dz = curPos[2]-m_lastPos[2];
			
			if (dx||dy||dz) 
			{
				/* compute theta and cross product */
				angle = float(90.0 * sqrt(dx*dx+dy*dy+dz*dz));
				
				axis[0] = m_lastPos[1]*curPos[2]-m_lastPos[2]*curPos[1];
				axis[1] = m_lastPos[2]*curPos[0]-m_lastPos[0]*curPos[2];
				axis[2] = m_lastPos[0]*curPos[1]-m_lastPos[1]*curPos[0];
				
				/* update position */
				m_lastPos[0] = curPos[0];
				m_lastPos[1] = curPos[1];
				m_lastPos[2] = curPos[2];
			}
		}
	}
}