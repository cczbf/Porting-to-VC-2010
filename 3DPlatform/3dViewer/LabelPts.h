// LabelPts.h: interface for the CLabelPts class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LABELPTS_H__0C1DD8C7_B6F0_4767_9D9B_2712C0A55DCA__INCLUDED_)
#define AFX_LABELPTS_H__0C1DD8C7_B6F0_4767_9D9B_2712C0A55DCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using std::vector;
#include "typedef.h"


class CLabelPts  
{
public:
	
	CLabelPts();	
	CLabelPts(const CLabelPts &labelPts); //copy constructor	
	CLabelPts& operator = (const CLabelPts &labelPts);//assaign operator

	virtual ~CLabelPts();

	void    AddPt(POINT3D pt) ;
	POINT3D GetPt(int i) const;
	int     GetSize() const;
	
private:
	vector<POINT3D>	m_vecPts;
};

#endif // !defined(AFX_LABELPTS_H__0C1DD8C7_B6F0_4767_9D9B_2712C0A55DCA__INCLUDED_)
