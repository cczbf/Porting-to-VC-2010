#ifndef READ_PLY_H
#define READ_PLY_H


#include "typedef.h"


class CReadPLY
{

public:
	POINT3D* GetNormal();
	stFACE* GetFaces();
	POINT3D* GetPoints();
	int GetFaceNum();
	void enableLight(bool enable);

	CReadPLY();
	~CReadPLY();

	bool Load(CString strFile);
	void Render();
	int GetPointNum();

protected:
    POINT3D *m_pPts;	
	int     m_nPtSum;
	stFACE  *m_pFaces;
	int     m_nFaceSum;
	
	POINT3D  *m_pNormals;
	
};


#endif