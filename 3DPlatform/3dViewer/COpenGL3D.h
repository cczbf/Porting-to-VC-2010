#if !defined(__OPENGL3D_H__)
#define		 __OPENGL3D_H__

#include "COpenGL.h"

//##ModelId=40E2654301D1
class COpenGL3D : public COpenGL
{
public:
	//##ModelId=40E265430205
	COpenGL3D();
	//##ModelId=40E265430204
	~COpenGL3D() { ; }

	//##ModelId=40E2654301E3
	void Init3DDataSet(int col, int row, int group, int n);

	//##ModelId=40E265430203
	virtual void OpenGLDrawMap();
	//##ModelId=40E265430202
	virtual void GDIDrawMap();
	//##ModelId=40E265430201
	virtual void DrawBox();
	//##ModelId=40E2654301FF
	virtual void Legend(RECT& rt);
	//##ModelId=40E2654301FE
			void DrawBase();
	//##ModelId=40E2654301FD
			void DrawSide();
	//##ModelId=40E2654301FB
	virtual void OpenMaps(unsigned int);
	//##ModelId=40E2654301F9
	virtual void SaveMaps(unsigned int);

	//##ModelId=40E2654301F8
	void DrawFlat();
	//##ModelId=40E2654301F7
	void DrawSurface();
	//##ModelId=40E2654301F6
	void DrawGrid();
	//##ModelId=40E2654301F5
	void DrawMarker();

	//##ModelId=40E2654301F2
	void Load3DData(FILE* fp, char type);


private:
	//##ModelId=40E2654301F0
	void LoadSurferBinaryData(FILE* fp);
	//##ModelId=40E2654301EE
	void LoadSurferTextData(FILE* fp);
	//##ModelId=40E2654301EB
	void LoadSurferData(FILE* fp, char type);
	//##ModelId=40E2654301E8
	void Load3DMultipleMapsData(FILE* fp, char type);
	//##ModelId=40E2654301E0
	void Normalisation(int type, int n);
};

#endif
