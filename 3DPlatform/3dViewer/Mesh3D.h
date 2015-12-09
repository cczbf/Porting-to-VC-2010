// Mesh3D.h: interface for the Mesh3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESH3D_H__05D378E8_C6FB_4CBA_8660_E63256132268__INCLUDED_)
#define AFX_MESH3D_H__05D378E8_C6FB_4CBA_8660_E63256132268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ReadPLY.h"
#include <vector>
#include <string>

#include "typedef.h"	// Added by ClassView

using namespace std;


//#define  EDGE  GEOIMG_EDGE
#define  EDGE_ GEOIMG_EDGE_


class Mesh3D  
{
public:
	void FindInitialCut();
	void FinalBoundaryParam();
	bool IsHaveBoundary();
	void DrawEdges();
	void LoadData(CString strFileName);
	Init();

	POINT3D  meshPnt(int n) const;

	
	Mesh3D();
	virtual ~Mesh3D();	

protected:
	double m_lfScale;
	CString m_strPlyName;
	int m_nAddCutCnt;
	int m_nAugCount;
	int m_nAddInPnt;
	bool* m_bIsBound;

	POINT2D* m_pOutPnts;
	POINT2D* m_pInPnts;

	int m_tempPID;
	POINT2D m_preP;
	int m_ntempNneighbor;
	int m_tempL;
	POINT2D* m_tempP;
	double m_totalAng;
	int m_currentCenterPnt;
	double *m_tempAng;
	int m_nGenus;
	
	double lf2sf(double);
	int m_nAddFaces;
	int m_nAddPnt;
	void BreakEdge(int *dir, double dist, double tempLen, double ScaleCoff, int edgeIndex);
	int MinOf3(int , int , int);
	int MiddleOf3(int , int , int );
	int MaxOf3(int , int ,int );
	bool IsSame(POINT3D, POINT3D);
	int m_nFaceSum;
	int m_nPtSum;
	
	vector<GEOIMG_EDGE> m_vEdge;
	vector<EDGE_> m_vBoundary; //
	vector<EDGE_> m_vCutPath; //cut path
	vector<EDGE_> m_vStrCutPath;
	vector<SideBand> m_vSideBand; 
		
    POINT3D *m_pPts;	
	stFACE  *m_pFaces;

	POINT3D *m_pAddPts;
	stFACE  *m_pAddFaces;
		
	POINT3D  *m_pNormals;
	MapPoint **m_pGeoMap;

	vector<MapPoint> m_vBndMapPnt; //the result of border param
	vector<MapPoint> m_vInMapPnt;  //the points of Floater
	vector<BndPoint> m_vBndPnt;    //
	
	vector<newFACE>  m_vNewFaces;
	vector<TriAngle> m_vRemeshFaces;

public:
	void NewMeshRender();
	
	void OutputResult();
	void AddCutPath(int);
	int FindMaxStrTri();
	void DisplayCircleBndMap();
	void CircleBndParam();
	void AugCut();
	void StraightenCut();
	double* m_lfStat;
	void computeGeoStretch(double*);
	void DisplayReconstruct();
	void Reconstruct();
	void DisplayGeoImage();
	void CreateGeoImage();
	void DisplayInterior2();
	void CreateNewFaces();
	void solveDelta2(int lIndex,double *delta);
	void solvePSEQ2(int centerPoint, double *ang, int nNeighbor, int *neighborPnt, POINT2D *p);
	void ComputeChordWeight(int *inPnt, double **w, int nInPnt);
	stFACE  meshFaces(int n) const;
	void SepPoints();
	void ComputeTutteWeight(int *inPnt, double **w,int nBoundPntCnt);
	void ReadInCut(CString ,CString);
	bool OutPutCutPath(CString);
	double ComputeAng2(POINT2D A, POINT2D B, POINT2D C);
	double ComputeAng(POINT3D A, POINT3D B, POINT3D C);
	int ** m_ppConnect;
	void ComputeWeight(bool*,double**);
	void ComputeMeanValueWeght(bool*,double**);
	void FloaterParam(int);
	void BreakEdge1(int *dir, double dist, double tempLen, double ScaleCoff, int edgeIndex);
	void CutUnwrap2();
	void DisplayBoundaryMap();
	void CutUnwrap();
	void BoundaryParam();
	void DisplayBoundary();
	void DisplayCutpath();
	bool m_bHaveBoundary;
	void DeleteSameTri();
	void empty();

	CReadPLY PLYobj;

protected:
};

#endif // !defined(AFX_MESH3D_H__05D378E8_C6FB_4CBA_8660_E63256132268__INCLUDED_)
