#ifndef READ_OBJ_H
#define READ_OBJ_H

#include "glm.h"

class  CReadObj
{

public:
	bool SaveAsPLY(LPCTSTR strFileName);

	CReadObj();
	~CReadObj();

	bool Load(LPCTSTR strFileName);
	bool Render();
	void GetMaxMin(float &xMin,float &xMax,
				   float &yMin,float &yMax,
				   float &zMin,float &zMax);
	//
	void GetVertex(float &x, float &y, float &z, int index);
	void GetTriangle(int &a, int &b, int &c, int index);
	
	//
	int  GetVertexNum();
	int  GetTriangleNum();

protected:
	GLMmodel *m_Model;

	int      *m_TriangleIndexA;
	int      *m_TriangleIndexB;
	int      *m_TriangleIndexC;

private:
};










#endif