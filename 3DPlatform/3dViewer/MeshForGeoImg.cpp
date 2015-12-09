// MeshForGeoImg.cpp: implementation of the CMeshForGeoImg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "float.h"
#include <algorithm>
using std::find_if;

#include "3dviewer.h"
#include "MeshForGeoImg.h"

#include "cglobals.h"
#include "3dViewerDoc.h"
#include "COpenGL3DEx.h"
#include "progressbar.h"


#include "CMesh.h"
#include "2DMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//declarate the funcion object to search points
class PointCriterion
{
	POINT3D p;

public:
	PointCriterion(POINT3D pt)
	{
		p.x = pt.x;
		p.y = pt.y;
		p.z = pt.z;
	}

	bool operator()(const POINT3D p1) const
	{
		if( fabs(p1.x-p.x)<FLT_MIN && 
			fabs(p1.y-p.y)<FLT_MIN &&
			fabs(p1.z-p.z)<FLT_MIN)
			return TRUE;
		else
			return FALSE;
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeshForGeoImg::CMeshForGeoImg()
{

}

CMeshForGeoImg::~CMeshForGeoImg()
{
}

void CMeshForGeoImg::SetData(CMesh &mesh)
{

	m_pPts =NULL;
	m_pFaces = NULL;

	m_nPtSum = mesh.GetVertexNum();
	m_nFaceSum = mesh.GetTriangleNum();
	
	if(m_nPtSum <= 0)
	{
		return;
	}   

	m_pPts = new POINT3D[m_nPtSum];
	m_pFaces = new stFACE[m_nFaceSum];

	for(int i=0; i<m_nPtSum; i++)
	{
		float x,y,z;
		mesh.GetVertex(x, y, z, i);

		m_pPts[i].x = x;
		m_pPts[i].y = y;
		m_pPts[i].z = z;
		m_pPts[i].IsNull = FALSE;
	}

	for(i=0; i<m_nFaceSum; i++)
	{
		int va,vb,vc;
		mesh.GetTriangleIndices(va, vb, vc, i);

		m_pFaces[i].vI[0] = va;
		m_pFaces[i].vI[1] = vb;
		m_pFaces[i].vI[2] = vc;
		
		m_pFaces[i].n = 3;
		m_pFaces[i].IsNull = FALSE;
	}
	
	m_vEdge.clear();

    m_vEdge.clear();
	m_vBoundary.clear(); //
	m_vCutPath.clear(); //cut path
    m_vStrCutPath.clear();
	m_vSideBand.clear();

	m_vBndMapPnt.clear(); //
	m_vInMapPnt.clear();  //
	m_vBndPnt.clear();    //
	m_vNewFaces.clear();
	m_vRemeshFaces.clear();
		
}

void CMeshForGeoImg::SetMesh()
{
	CMy3dViewerDoc* pDoc = g_CGlobals.get_m_pCBmpviewerDoc();	
	int nCurMeshIndex = pDoc->m_nCurrentMeshIndex;	
	if(pDoc->m_pMesh[nCurMeshIndex] != NULL)
		delete pDoc->m_pMesh[nCurMeshIndex];	
	
	if(m_enumType != 6)
	{
		C2DMesh *pMesh = new C2DMesh;
		pDoc->m_pCurrent2DMesh = pMesh;
		pDoc->m_pMesh[nCurMeshIndex] = pMesh;
		
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh = ( pDoc->m_pMesh[nCurMeshIndex] ); 
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->m_nType = 3; 
	
		((C2DMesh *)pDoc->m_pMesh[nCurMeshIndex] )->SetMesh(*this);		
	}
	else
	{
		int nPtSum = m_vecPts.size();
		
		double *x = new double[nPtSum];
		double *y = new double[nPtSum];
		double *z = new double[nPtSum];
		
		for(int i=0; i<nPtSum; i++)
		{
			x[i] = m_vecPts[i].x * 10;
			y[i] = m_vecPts[i].y * 10;
			z[i] = m_vecPts[i].z * 10;
		}
		
		int nFaceSum = m_vecTriangleIndices.size();
		int *v1 = new int[nFaceSum];
		int *v2 = new int[nFaceSum];
		int *v3 = new int[nFaceSum];
		
		for(i=0; i<nFaceSum; i++)
		{			
			v1[i] =	m_vecTriangleIndices[i].index[0] ;
			v2[i] = m_vecTriangleIndices[i].index[1] ;
			v3[i] = m_vecTriangleIndices[i].index[2] ;
		}	
		
		CMesh  *pMesh = new CMesh;
		pDoc->m_pMesh[nCurMeshIndex] = pMesh;
		pDoc->m_pCurrent2DMesh = NULL;
		
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh = ( pDoc->m_pMesh[nCurMeshIndex] ); 
		((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->m_nType = 4; 
		
		pDoc->m_pMesh[nCurMeshIndex]->SetMesh( nFaceSum, v1, v2, v3,
			NULL, NULL, NULL, 
			NULL, NULL, NULL,
			nPtSum, x, y, z,
			0,
			"",
			"",
			"");
				
		delete []x;
		delete []y;
		delete []z;
		
		delete []v1;
		delete []v2;
		delete []v3;
	}
	
	pDoc->m_strTitle[nCurMeshIndex] = pDoc->m_strFileName + " " ;
	pDoc->m_nCurrentMeshIndex =  (pDoc->m_nCurrentMeshIndex + 1) % 2;
	pDoc->m_strTitle[pDoc->m_nCurrentMeshIndex] = pDoc->m_strFileName + "*";
	pDoc->UpdateView();	

}


void    CMeshForGeoImg::SetDispType(int nType)
{
	switch(nType) {
	case 0:
		m_enumType = eBoundary;
		break;
	case 1:
		m_enumType = eInitCut;		
		break;
    case 2:
		m_enumType = eAugCut;		
		break;
    case 3:
		m_enumType = eBoundParam;		
		break;
    case 4:
		m_enumType = eInterParam;		
		break;
	case 5:
		m_enumType = eCreateGeoImage;		
		break;
	case 6:
		m_enumType = eReconstruct;		
		break;         
	default:
		break;
	}	
}

GeoimgMeshDispType CMeshForGeoImg::GetDispType() const
{
	return m_enumType;
}


const vector<MapPoint>& CMeshForGeoImg::GetBoundMapPointVec()
{
	return m_vBndMapPnt;
}
const vector<MapPoint>& CMeshForGeoImg::GetInnerMapPointVec()
{
	return m_vInMapPnt;
}

const vector<newFACE>&  CMeshForGeoImg::GetNewFaceVec()
{
	return m_vNewFaces;
}

void    CMeshForGeoImg::OutPut2DMesh(CString filename)
{

	CString strfilename,str;
	CStdioFile outFile;
	int m,i;
  
	int nStrLen = filename.GetLength() - 4;
	CString fileTile  = filename.Left(nStrLen);
	CString file2dmesh = fileTile + "_2DMeshResult.txt";

	strfilename = file2dmesh ;
	//////////////////////////////////////////////////////////////////////////
	//输出3d点的坐标
	outFile.Open(strfilename,CFile::modeCreate|CFile::modeWrite);
	if(!outFile)
	{
		AfxMessageBox("Create 3d Point File Error!");
		return;
	}
	str.Format("%d\n",(m_nPtSum+m_nAddPnt));
	outFile.WriteString(str);
	for(m=0; m<m_nPtSum+m_nAddPnt; m++)
	{
		str.Format("%lf %lf %lf\n",meshPnt(m).x,meshPnt(m).y,meshPnt(m).z);
		outFile.WriteString(str);
	}
	//outFile.Close(); 
	
	//////////////////////////////////////////////////////////////////////////////
	//输出2d点的坐标
	//str.Format("%lf\n",m_lfScale);
	//outFile.WriteString(str);
	
	str.Format("%d %d\n",m_vBndMapPnt.size(),m_vInMapPnt.size());
	outFile.WriteString(str);
	for(m=0; m < m_vBndMapPnt.size(); m++)
	{
		str.Format("%lf %lf %d\n",m_vBndMapPnt.at(m).p.x,m_vBndMapPnt.at(m).p.y,m_vBndMapPnt.at(m).pointIndex);
		outFile.WriteString(str);
	}
	for(m=0; m < m_vInMapPnt.size(); m++)
	{ 
		str.Format("%lf %lf %d\n",m_vInMapPnt.at(m).p.x, m_vInMapPnt.at(m).p.y,m_vInMapPnt.at(m).pointIndex);
		outFile.WriteString(str);
	}
	//outFile.Close(); 

	//////////////////////////////////////////////////////////////////////////
	///输出平面三角形的顶点
	int pntIndex[3];
	
	str.Format("%d\n",m_vNewFaces.size());
	outFile.WriteString(str);

	for(m=0; m < m_vNewFaces.size(); m++)
	{
		for(i=0; i < 3 ; i++)
		{ 
			if(m_vNewFaces.at(m).flag[i]==0)
				pntIndex[i]=m_vNewFaces.at(m).index[i]+m_vBndMapPnt.size();
			else 
				pntIndex[i]=m_vNewFaces.at(m).index[i];
		}
		str.Format("%d %d %d\n",pntIndex[0],pntIndex[1],pntIndex[2]);	
		outFile.WriteString(str);
	}
	outFile.Close(); 

	/*
	//////////////////////////////////////////////////////////////////////////
	//输出平面上的各条边的信息
	strfilename=m_strPlyName+"_edges.txt";
	outFile.Open(strfilename,CFile::modeCreate|CFile::modeWrite);
	if(!outFile)
	{
		AfxMessageBox("Create edges File Error!");
		return;
	}

	int j,k;
	vector<EDGE> vNewEdges;
	EDGE edgeTemp1;
	EDGE edgeTemp2;

	vNewEdges.clear();
	for(i=0; i < vFace.size(); i++)
	{
		for(j = 0; j < 3; j++)
		{
			//检查该边是否已经存在
			for(k=0; k < vNewEdges.size(); k++)
			{ 
				edgeTemp1=vNewEdges.at(k);
				if((vFace.at(i).vI[j]==edgeTemp1.start&&vFace.at(i).vI[(j+1)%3]==edgeTemp1.end)||
					(vFace.at(i).vI[j]==edgeTemp1.end&&vFace.at(i).vI[(j+1)%3]==edgeTemp1.start))
					 break;
			}
			//如果不存在，添加一条新边的信息；
			if(k>=vNewEdges.size())
			{
				 edgeTemp2.start=vFace.at(i).vI[j];
				 edgeTemp2.end =vFace.at(i).vI[(j+1)%3];
				 vNewEdges.push_back(edgeTemp2);
			}
		}
	}

	for(i=0; i < vNewEdges.size(); i++)
	{ 
		str.Format("%d %d\n",vNewEdges.at(i).start,vNewEdges.at(i).end);	
		outFile.WriteString(str);
	}
	outFile.Close(); 
	*/
}
/*
 *	Transform the discrete triangles to triangles indexed by vertex
 */
void    CMeshForGeoImg::FormatTransform()
{	
	int nFaceSize = m_vRemeshFaces.size();

	m_vecPts.clear();
	m_vecTriangleIndices.clear();

	typedef vector<POINT3D>::iterator iterator;
	int nPtIndex = 0;
	
	CProgressBar * pbar = NULL;
    pbar = new CProgressBar(_T("Transformation..."), 100, 100, TRUE);
    pbar->SetText("Transformation...");
    float step = 0;
	float prog = 0;
	step = float(100.0)/(float)(nFaceSize);
	
   	for(int i = 0; i < m_vRemeshFaces.size(); i++)
	{
		prog += step;
		pbar->SetPos((int)prog);
		
		POINT3D p1,p2,p3;
		newFACE face;

		p1.x = m_vRemeshFaces.at(i).v1.x;
		p1.y = m_vRemeshFaces.at(i).v1.y;
		p1.z = m_vRemeshFaces.at(i).v1.z;
		iterator it = std::find_if(m_vecPts.begin(), m_vecPts.end(), PointCriterion(p1));
		if(it == m_vecPts.end()) //not find
		{
            m_vecPts.push_back(p1);  
			face.index[0] = nPtIndex;			
			nPtIndex ++;
		}
		else
		{
			int index = std::distance(m_vecPts.begin(), it);			
			face.index[0] = index;
		}

		p2.x = m_vRemeshFaces.at(i).v2.x;
		p2.y = m_vRemeshFaces.at(i).v2.y;
		p2.z = m_vRemeshFaces.at(i).v2.z;
		it = std::find_if(m_vecPts.begin(), m_vecPts.end(), PointCriterion(p2));
		if(it == m_vecPts.end()) //not find
		{
            m_vecPts.push_back(p2);  
			face.index[1] = nPtIndex;			
			nPtIndex ++;
		}
		else
		{
			int index = std::distance(m_vecPts.begin(), it);	
			face.index[1] = index;			
		}

		p3.x = m_vRemeshFaces.at(i).v3.x;
		p3.y = m_vRemeshFaces.at(i).v3.y;
		p3.z = m_vRemeshFaces.at(i).v3.z;
		it = std::find_if(m_vecPts.begin(), m_vecPts.end(), PointCriterion(p3));
		if(it == m_vecPts.end()) //not find
		{
            m_vecPts.push_back(p3); 
			face.index[2] = nPtIndex;			
			nPtIndex ++;
		}
		else
		{
			int index = std::distance(m_vecPts.begin(), it);			
			face.index[2] = index;			
		}

		m_vecTriangleIndices.push_back(face);
	}

	delete pbar;
	
	//int nVertexNum = pts.size();
	//int nFaceNum = triangleIndices.size();
	
	//

}
/*
 *   Initialize the data for creating geoimage from 2D Mesh	
 */
void    CMeshForGeoImg::ReadyForCreateGeoimage(C2DMesh &mesh)
{
	int nSizeFace = m_vNewFaces.size();
	int nSizeInPts = m_vInMapPnt.size();
	int nSizeBoundPts = m_vBndMapPnt.size();
	
	int nBorderLen = mesh.GetBorderLen();
	int nVertexNum = mesh.GetVertexNum();

	int i=0;

	int n3DPtsNum = mesh.Get3DPointNum();
	m_pPts = new POINT3D[n3DPtsNum];
	m_nPtSum = n3DPtsNum;
	
	//3d points
	for(i=0; i<n3DPtsNum; i++)
	{
		float x,y,z;
		mesh.Get3DPoint(x, y, z, i);

		m_pPts[i].x = x;
		m_pPts[i].y = y;
		m_pPts[i].z = z;
	}

	//boundary points
	for(i=0; i<nBorderLen; i++)
	{
		MapPoint p;
		float x,y,z;
		mesh.GetVertex(x, y, z, i);
		int index;
		mesh.Get2DPtIndexTo3D(index, i);
		
		p.p.x = x;
		p.p.y = y;
		p.pointIndex = index;
		
		m_vBndMapPnt.push_back(p);
	}

	//inner points
	for(i=nBorderLen; i<nVertexNum; i++)
	{
		MapPoint p;
		float x,y,z;
		mesh.GetVertex(x, y, z, i);
		int index;
		mesh.Get2DPtIndexTo3D(index, i);
		
		p.p.x = x;
		p.p.y = y;
		p.pointIndex = index;

		m_vInMapPnt.push_back(p);		
	}

    //faces	
	int m_nFaceSum = mesh.GetTriangleNum();
	for( i=0; i<m_nFaceSum; i++)
	{
        newFACE face;
		
		int va,vb,vc;
		mesh.GetTriangleIndices(va, vb, vc, i);

		face.index[0] = va;
		face.index[1] = vb;
		face.index[2] = vc;
        
		for(int j=0; j<3; j++)
		{			
			if( face.index[j] < nBorderLen)
			{
				face.flag[j] = 1;						
			}
			else
			{
				face.index[j] = face.index[j] - nBorderLen;
				face.flag[j] = 0;
			}
		}		
		m_vNewFaces.push_back(face);
	}	
}

void    CMeshForGeoImg::OutputGeoimagePixel(int i, int j, float &r, float &g, float &b)
{
	ASSERT(i < (NGRID+1) );
	ASSERT(j < (NGRID+1) );

	r = m_pGeoMap[i][j].fx;
	g = m_pGeoMap[i][j].fy;
	b = m_pGeoMap[i][j].fz;
}

int     CMeshForGeoImg::GetGeoimageWid()
{
	return (NGRID+1);
}

int     CMeshForGeoImg::GetGeoimageHei()
{
	return (NGRID+1);
}

int     CMeshForGeoImg::GetTriangleNum() const
{
	return m_nFaceSum;
}
void    CMeshForGeoImg::GetTriangle(int &va, int &vb, int &vc, int index) const
{
	va = m_pFaces[index].vI[0] ;
	vb = m_pFaces[index].vI[1] ;
	vc = m_pFaces[index].vI[2] ;
}
int     CMeshForGeoImg::Get3DPtNum() const
{
	return m_nPtSum;
}
void    CMeshForGeoImg::GetVertex(double &x, double &y, double &z,int index) const
{
	x = m_pPts[index].x;
	y = m_pPts[index].y;
	z = m_pPts[index].z;
}

int     CMeshForGeoImg::GetAddPtNum() const
{
	return m_nAddPnt;
}

void    CMeshForGeoImg::GetAddPt(double &x, double &y, double &z, bool &bIsNull, int index) const
{
	
	x = m_pAddPts[index].x;
	y = m_pAddPts[index].y;
	z = m_pAddPts[index].z;
	bIsNull = m_pAddPts[index].IsNull;
}
int     CMeshForGeoImg::GetAddFaceNum() const
{
    return m_nAddFaces;
}
void    CMeshForGeoImg::GetAddFace(int &va, int &vb, int &vc, int &n, int index) const
{

	va = m_pAddFaces[index].vI[0];
	vb = m_pAddFaces[index].vI[1];
	vc = m_pAddFaces[index].vI[2];
	n  = m_pAddFaces[index].n; 
}

void    CMeshForGeoImg::GetMeshPt(double &x, double &y, double &z, int index) const
{
	x = meshPnt(index).x;
	y = meshPnt(index).y;
	z = meshPnt(index).z;
}
void    CMeshForGeoImg::GetMeshFace(int &va, int &vb, int &vc, int index) const
{
	va = meshFaces(index).vI[0];
	vb = meshFaces(index).vI[1];
	vc = meshFaces(index).vI[2];
}
double  CMeshForGeoImg::GetScale() const
{
	return m_lfScale;
}

vector<GEOIMG_EDGE_>  CMeshForGeoImg::GetCutPath() const
{
	return m_vCutPath;
}
vector<MapPoint>      CMeshForGeoImg::GetBndMapPnt() const
{
	return m_vBndMapPnt;
}
vector<MapPoint>      CMeshForGeoImg::GetInMapPnt() const
{
	return m_vInMapPnt;
}
vector<BndPoint>      CMeshForGeoImg::GetBndPnt() const
{
	return m_vBndPnt;
}
vector<newFACE>       CMeshForGeoImg::GetNewFaces() const
{
	return m_vNewFaces;
}
vector<TriAngle>      CMeshForGeoImg::GetRemeshFaces() const
{
	return m_vRemeshFaces;
}
vector<MapPoint>      CMeshForGeoImg::GetGeoMap() const
{	
	vector<MapPoint> vGeoMap;
	//vGeoMap.resize( (NGRID+1)*(NGRID+1) );
	if(m_pGeoMap != NULL)
	{	
		MapPoint mp;
		for(int i=0; i < NGRID+1; i++)
		{
			for( int j = 0; j < NGRID+1; j++)
			{
				mp.fx = m_pGeoMap[i][j].fx ;
				mp.fy = m_pGeoMap[i][j].fy ;
				mp.fz = m_pGeoMap[i][j].fz ;

				mp.p.x = m_pGeoMap[i][j].p.x ;
				mp.p.y = m_pGeoMap[i][j].p.y ;
				
				mp.pointIndex = m_pGeoMap[i][j].pointIndex;

				vGeoMap.push_back(mp);
			}
		}
	}

	return vGeoMap;
}


int     CMeshForGeoImg::GetReconstructPtNum() const
{
	return m_vecPts.size();
}
void    CMeshForGeoImg::GetReconstructPt(double &x, double &y, double &z, int index) const
{
	x = m_vecPts[index].x;
	y = m_vecPts[index].y;
	z = m_vecPts[index].z;
}
int     CMeshForGeoImg::GetReconstructTriangleNum() const
{
	return m_vecTriangleIndices.size();
}
void    CMeshForGeoImg::GetReconstructTriangle(int &va, int &vb, int &vc, int index) const
{
	va = m_vecTriangleIndices[index].index[0];
	vb = m_vecTriangleIndices[index].index[1];
	vc = m_vecTriangleIndices[index].index[2];
}


	/*
	double *x = NULL;
	double *y = NULL;
	double *z = NULL;
	int *v1 = NULL;
	int *v2 = NULL;
	int *v3 = NULL;

	int i = 0;
	int j = 0;

	int nPtSum = 0;
	int nFaceSum = 0;

    switch(m_enumType)
	{
    case 0:
        nPtSum = m_nPtSum;
		x = new double[m_nPtSum];
		y = new double[m_nPtSum];
		z = new double[m_nPtSum];
		
		for( i=0; i<m_nPtSum; i++)
		{
			x[i] = m_pPts[i].x ;
			y[i] = m_pPts[i].y ;
			z[i] = m_pPts[i].z ;
		}
		
		nFaceSum = m_nFaceSum;
		v1 = new int[m_nFaceSum];
		v2 = new int[m_nFaceSum];
		v3 = new int[m_nFaceSum];
		
		for(i=0; i<m_nFaceSum; i++)
		{
			
			v1[i] =	m_pFaces[i].vI[0] ;
			v2[i] = m_pFaces[i].vI[1] ;
			v3[i] = m_pFaces[i].vI[2] ;
		}
			
		break;
    case 1 :
		
		nPtSum =  m_nAddPnt + m_nPtSum;

		x = new double[nPtSum];
		y = new double[nPtSum];
		z = new double[nPtSum];

		for(i=0; i < m_nAddPnt + m_nPtSum; i++)
		{ 
			x[i] = meshPnt(i).x;
			y[i] = meshPnt(i).y;
			z[i] = meshPnt(i).z;
		}

		nFaceSum = m_nFaceSum + m_nAddFaces;

		v1 = new int[nFaceSum];
		v2 = new int[nFaceSum];
		v3 = new int[nFaceSum];
				
		for(i=0; i<m_nFaceSum+m_nAddFaces; i++)
		{			
				v1[i] = meshFaces(i).vI[0];
				v2[i] = meshFaces(i).vI[1];
				v3[i] = meshFaces(i).vI[2];
		}
		break;
		
    case 2 :

		nPtSum =  m_nAddPnt + m_nPtSum;

		x = new double[nPtSum];
		y = new double[nPtSum];
		z = new double[nPtSum];

		for(i=0; i < m_nAddPnt + m_nPtSum; i++)
		{ 
			x[i] = meshPnt(i).x;
			y[i] = meshPnt(i).y;
			z[i] = meshPnt(i).z;
		}

		nFaceSum = m_nFaceSum + m_nAddFaces;

		v1 = new int[nFaceSum];
		v2 = new int[nFaceSum];
		v3 = new int[nFaceSum];
				
		for(i=0; i<m_nFaceSum+m_nAddFaces; i++)
		{			
				v1[i] = meshFaces(i).vI[0];
				v2[i] = meshFaces(i).vI[1];
				v3[i] = meshFaces(i).vI[2];
		}
		break;
    case 3 :
	    nPtSum =  m_nAddPnt + m_nPtSum;

		x = new double[nPtSum];
		y = new double[nPtSum];
		z = new double[nPtSum];

		for(i=0; i < m_nAddPnt + m_nPtSum; i++)
		{ 
			x[i] = meshPnt(i).x;
			y[i] = meshPnt(i).y;
			z[i] = meshPnt(i).z;
		}

		nFaceSum = m_nFaceSum + m_nAddFaces;

		v1 = new int[nFaceSum];
		v2 = new int[nFaceSum];
		v3 = new int[nFaceSum];
				
		for(i=0; i<m_nFaceSum+m_nAddFaces; i++)
		{			
				v1[i] = meshFaces(i).vI[0];
				v2[i] = meshFaces(i).vI[1];
				v3[i] = meshFaces(i).vI[2];
		}
		

		break;
    case 4 :
	
		nPtSum =  m_nAddPnt + m_nPtSum;

		x = new double[nPtSum];
		y = new double[nPtSum];
		z = new double[nPtSum];

		for(i=0; i < m_nAddPnt + m_nPtSum; i++)
		{ 
			x[i] = meshPnt(i).x;
			y[i] = meshPnt(i).y;
			z[i] = meshPnt(i).z;
		}

		nFaceSum = m_nFaceSum + m_nAddFaces;

		v1 = new int[nFaceSum];
		v2 = new int[nFaceSum];
		v3 = new int[nFaceSum];
				
		for(i=0; i<m_nFaceSum+m_nAddFaces; i++)
		{			
				v1[i] = meshFaces(i).vI[0];
				v2[i] = meshFaces(i).vI[1];
				v3[i] = meshFaces(i).vI[2];
		}
		break;
    case 5 :
		break;
    case 6 :  //reconstuction result
		
		//g_CGlobals.get_m_pCBmpviewerDoc()->m_strFileName += "-Reconstruction Model" ;

		nPtSum = m_vecPts.size();

		x = new double[nPtSum];
		y = new double[nPtSum];
		z = new double[nPtSum];
		
		for( i=0; i<nPtSum; i++)
		{
			x[i] = m_vecPts[i].x * 10;
			y[i] = m_vecPts[i].y * 10;
			z[i] = m_vecPts[i].z * 10;
		}
		
		nFaceSum = m_vecTriangleIndices.size();
		v1 = new int[nFaceSum];
		v2 = new int[nFaceSum];
		v3 = new int[nFaceSum];
		
		for(i=0; i<nFaceSum; i++)
		{			
			v1[i] =	m_vecTriangleIndices[i].index[0] ;
			v2[i] = m_vecTriangleIndices[i].index[1] ;
			v3[i] = m_vecTriangleIndices[i].index[2] ;
		}		 	
		break;
    default:
		break;
    }   
	
	POINT3D *pAddPts = new POINT3D[m_nAddPnt]; 
	stFACE  *pAddFaces = new stFACE[m_nAddFaces];	
	for(i=0; i<m_nAddPnt; i++)
	{
		pAddPts[i].x = m_pAddPts[i].x;
		pAddPts[i].y = m_pAddPts[i].y;
		pAddPts[i].z = m_pAddPts[i].z;
		pAddPts[i].IsNull = m_pAddPts[i].IsNull;
	}
	for(i=0; i<m_nAddFaces; i++ )
	{
        pAddFaces[i].n = m_pAddFaces[i].n;		
   
		for(j=0; j<m_pAddFaces[i].n; j++)
		{
			pAddFaces[i].vI[j] = m_pAddFaces[i].vI[j];
		}
	}
	
	vector<MapPoint> vGeoMap;
	//vGeoMap.resize( (NGRID+1)*(NGRID+1) );
	if(m_pGeoMap != NULL)
	{	
		MapPoint mp;
		for(i=0; i < NGRID+1; i++)
		{
			for(j = 0; j < NGRID+1; j++)
			{
				mp.fx = m_pGeoMap[i][j].fx ;
				mp.fy = m_pGeoMap[i][j].fy ;
				mp.fz = m_pGeoMap[i][j].fz ;

				mp.p.x = m_pGeoMap[i][j].p.x ;
				mp.p.y = m_pGeoMap[i][j].p.y ;
				
				mp.pointIndex = m_pGeoMap[i][j].pointIndex;

				vGeoMap.push_back(mp);
			}
		}
	}
    */