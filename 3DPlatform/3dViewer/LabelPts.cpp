// LabelPts.cpp: implementation of the CLabelPts class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dviewer.h"
#include "LabelPts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLabelPts::CLabelPts()
{
	m_vecPts.clear();	
}

CLabelPts::CLabelPts(const CLabelPts &labelPts)
{	
	//if(this == &labelPts)
	//	return;
		
	if( (&labelPts) == NULL)
	{
		CLabelPts();
		return;
	}
				
	int nSize = labelPts.GetSize();
	if(nSize)
	{
		m_vecPts.reserve(nSize);
		
		for(int i=0; i<nSize; i++)
			m_vecPts.push_back( labelPts.GetPt(i) );
	}
}

CLabelPts& CLabelPts::operator = (const CLabelPts &labelPts)
{	
	if(this == &labelPts)
		return *this;
    
	if( (&labelPts) == NULL)
	{
		CLabelPts();
		return *this;
	}
	
    m_vecPts.clear();
	int nSize = labelPts.GetSize();
	m_vecPts.reserve(nSize);

	for(int i=0; i<nSize; i++)
		m_vecPts.push_back( labelPts.GetPt(i) );
	
	return *this;
}

CLabelPts::~CLabelPts()
{
	m_vecPts.clear();
}

void CLabelPts::AddPt(POINT3D pt)
{
   m_vecPts.push_back(pt);
}

POINT3D CLabelPts::GetPt(int i) const
{
	ASSERT(i < m_vecPts.size());
	
	if(i < m_vecPts.size())
		return m_vecPts[i];
	else
	{
		AfxMessageBox(" Exceed the size of points !");
		POINT3D p;
		p.x = 0;
		p.y = 0;
		p.z = 0;
		
		return p;
	}
}

int CLabelPts::GetSize() const
{
	if(!m_vecPts.empty())
		return m_vecPts.size();
	else
		return -1;
}
