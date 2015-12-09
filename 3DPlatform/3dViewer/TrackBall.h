

class CTrackBall
{

	int 	m_winWidth, m_winHeight;
	
	//float  angle,axis[3],trans[3];
	
	bool 	trackingMouse ;
	bool 	redrawContinue;
	bool    trackballMove ;
	
	//float   lastPos[3];

	int     curx, cury;
	int     startX, startY;
	float   m_lastPos[3];
	
public:
	void SetSize(int w,int h);
	CTrackBall();
	~CTrackBall();

	void Trackball_ptov(int x, int y, int width, int height, float *v);
	void StartMotion(CPoint pt);
	void StopMotion(int x, int y);

	//integrate all functions
	void Track(CPoint pt,float *axis,float &angle);
};
