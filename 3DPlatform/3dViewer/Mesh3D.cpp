// Mesh3D.cpp: implementation of the Mesh3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mesh3D.h"

#include "math.h"
#include "matrix.h"

#include <set>

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glaux.h"

#include <stdarg.h>			// Header File For Variable Argument Routines
#include "typedef.h"


#include "stdio.h"
#include "conio.h"
#include "malloc.h"
#include "stdlib.h"

const   NPARAS=2*MAXNEIGHBORPOINT;
#define PI                           3.1416926535;
#define REL_SENSOR_TOLERANCE_ftol    1.0E-5      /* [pix] */
#define REL_PARAM_TOLERANCE_xtol     1.0E-7
#define ORTHO_TOLERANCE_gtol         0.0
#define MAXFEV                       (1000*n)
#define EPSFCN                       1.0E-16     /* Do not set to zero! */
#define MODE                         1           /* variables are scalled internally */
#define FACTOR                       100.0 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define MAXJOINTEDGES 10
#define MAXJOINTCUTS  5
#define STATPARANUM  5
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace std;

Mesh3D::Mesh3D()
{
	int i;
//	int j;
	m_bHaveBoundary=FALSE;
	
	m_nAddPnt = 0;
	m_nAddFaces = 0;
	m_pAddPts=(POINT3D *) malloc (sizeof (POINT3D ) * NUMALLOC);
	m_pAddFaces = (stFACE*) malloc (sizeof(stFACE) * NUMALLOC);
	


	m_tempAng = NULL;
	m_ppConnect = NULL;
	m_pGeoMap = NULL;
	m_pInPnts = NULL;
	m_pOutPnts = NULL;
	m_tempP = NULL;
	m_pPts = NULL;
	m_pFaces = NULL;
		
	m_pNormals = NULL;
	m_pGeoMap = NULL;
 
	m_lfStat = (double*)malloc(STATPARANUM*sizeof(double));

	for(i=0; i < STATPARANUM; i++)
		m_lfStat[i]=-1;
	
	m_nAddInPnt=0;

	m_nAugCount=0;
}

Mesh3D::~Mesh3D()
{
 	free(m_pAddPts);
	m_pAddPts = NULL;

	free(m_pAddFaces);
	m_pAddPts = NULL;
 
	free(m_pGeoMap);
	m_pGeoMap = NULL;
	
	free(m_pFaces);
	m_pFaces = NULL;

	free(m_pPts);
	m_pPts =NULL;
	
	free(m_pInPnts);
	m_pInPnts = NULL;
}

void Mesh3D::LoadData(CString strFileName)
{
	m_pPts =NULL;
	m_pFaces = NULL;

	PLYobj.Load(strFileName);
	
	m_nPtSum = PLYobj.GetPointNum();
	m_nFaceSum = PLYobj.GetFaceNum();

	m_pPts = PLYobj.GetPoints();
	m_pFaces = PLYobj.GetFaces();

	CString string=strFileName;
	string=string.Left(string.Find(".ply"));
	m_strPlyName=string;

	m_vEdge.clear();
}
 
void Mesh3D::DrawEdges()
{
	int i,j,n;
	int pos;
	GEOIMG_EDGE edgeTemp1,edgeTemp2;

	if(m_vEdge.size()!=0)
	{ 
		AfxMessageBox("It has been done!");
		return;
	}
//	DeleteSameTri();
	for(i=0; i < m_nFaceSum; i++)
	{
		for(j = 0; j < 3; j++)
		{
			//检查该边是否已经存在
			for(pos=0; pos < m_vEdge.size(); pos++)
			{ 
				edgeTemp1=m_vEdge.at(pos);
				if(((edgeTemp1.start==m_pFaces[i].vI[j]) && (edgeTemp1.end==m_pFaces[i].vI[(j+1)%3])) ||
					((edgeTemp1.end==m_pFaces[i].vI[j]) && (edgeTemp1.start==m_pFaces[i].vI[(j+1)%3])) )
					break;
			}
			//如果已经存在
			if(pos < m_vEdge.size())
			{
				for(n=0; n<MAXNEIGHBOR; n++)
					if(edgeTemp1.neighborTri[n]==-1) break;
				m_vEdge.at(pos).neighborTri[n]=i;
			}
			//如果不存在，添加一条新边的信息；
			else
			{
				edgeTemp2.start = m_pFaces[i].vI[j];
				edgeTemp2.end = m_pFaces[i].vI[(j+1)%3];
				edgeTemp2.neighborTri[0]=i;
				edgeTemp2.IsNull=FALSE;
				for(n=1; n<MAXNEIGHBOR; n++)
					edgeTemp2.neighborTri[n]=-1;
				m_vEdge.push_back(edgeTemp2);
			}
		}
	}
 	int nEdges;
	
	nEdges = m_vEdge.size();
	m_nGenus = (2-(m_nPtSum+m_nFaceSum - nEdges))/2;

	if(m_nGenus<0)
		AfxMessageBox("genus is negative!");
}


//检查这个mesh是否含有边界
bool Mesh3D::IsHaveBoundary()
{
	int i,n;
	int count;
	bool flag;
	int triIndex;
	GEOIMG_EDGE temp;
	EDGE_ edge_Temp;
	flag=FALSE;

 	DrawEdges();
	
	for(i=0; i < m_vEdge.size(); i++)
	{
		temp=m_vEdge.at(i);
		count=0;
		for(n=0; n<MAXNEIGHBOR; n++)
		{
			if(temp.neighborTri[n]!=-1) 
			{
				count++;
				triIndex=n;
			}
		}
		if(count==1)
		{
			flag=TRUE;
			m_vEdge.at(i).IsNull=TRUE;//去掉该条边
			m_pFaces[temp.neighborTri[triIndex]].IsNull=TRUE; //去掉与边界相连的唯一的三角形
		
			edge_Temp.start=temp.start;
			edge_Temp.end = temp.end;
//			fprintf(fp,"%d : %d , %d \n",i,temp.start,temp.end);
			edge_Temp.P[0] = meshPnt(temp.start);
			edge_Temp.P[1] = meshPnt(temp.end);
			edge_Temp.length = sqrt((edge_Temp.P[0].x-edge_Temp.P[1].x)*(edge_Temp.P[0].x-edge_Temp.P[1].x)
									+(edge_Temp.P[0].y-edge_Temp.P[1].y)*(edge_Temp.P[0].y-edge_Temp.P[1].y)
									+(edge_Temp.P[0].z-edge_Temp.P[1].z)*(edge_Temp.P[0].z-edge_Temp.P[1].z)); 
			//edge_Temp.length = lf2sf(edge_Temp.length);
			edge_Temp.bIsBound = TRUE;
			m_vBoundary.push_back(edge_Temp);
		}
	}

//	fclose(fp);
	
	m_bHaveBoundary = flag;

	return flag;
}


//找初始切线
void Mesh3D::FindInitialCut()
{
	int i,j,pos,n,m,k;
	int moveIndex;
	GEOIMG_EDGE edgeTemp1;
	int count;
	int triCnt;
	int triIndex;
	bool flag;
	
	m_vCutPath.clear();
	if(m_bHaveBoundary)
	{
		//如果边界存在，将mesh边界加入cutpath集合中
		m_vCutPath=m_vBoundary;
	}
	else
	{
		//如果不存在边界，则任意移走一个三角形
		moveIndex= 0;
		//moveIndex= rand()%m_nPtSum; //移走第一个三角形 ， 可以考虑生成一个随机数，移走该三角形
		m_pFaces[moveIndex].IsNull=TRUE;
		for(j=0; j < 3; j++)
		{
			for( pos = 0; pos < m_vEdge.size(); pos++)
			{
				edgeTemp1=m_vEdge.at(pos);
				if(((edgeTemp1.start==meshFaces(moveIndex).vI[j]) && (edgeTemp1.end==meshFaces(moveIndex).vI[(j+1)%3])) ||
				   ((edgeTemp1.end==meshFaces(moveIndex).vI[j])   && (edgeTemp1.start==meshFaces(moveIndex).vI[(j+1)%3])))
				break;
			}
		}
	}
 	
	while (1)
	{
		count=0;
		for(pos=0; pos < m_vEdge.size(); pos++)
		{
			triCnt=0;
			triIndex=-1;
			flag=FALSE;
			//计算与第pos个边相邻的有效三角形的个数
			for(n=0; n< MAXNEIGHBOR; n++)
			{
				int t;
				t=m_vEdge.at(pos).neighborTri[n];
				if(t!=-1 && m_pFaces[t].IsNull!=TRUE) 
				{
					triCnt++;
					//如果只有一个相邻三角形，可以记录下来该三角形的索引号
					triIndex=m_vEdge.at(pos).neighborTri[n]; 
				}
			}
			//如果该边只有一个有效相邻三角形，检查该边是否在边界中
			if(triCnt==1)
			{
				for(m=0; m < m_vCutPath.size(); m++)
				{
					if((m_vEdge.at(pos).start==m_vBoundary.at(m).start&&m_vEdge.at(pos).end==m_vBoundary.at(m).end)||
						(m_vEdge.at(pos).start==m_vBoundary.at(m).end&&m_vEdge.at(pos).end==m_vBoundary.at(m).start))
					{ 
						flag=TRUE; 
						break;
					}
				}
				//相邻三角形链表中只有一个三角形&这条边不属于边界Cutpath
				if(!flag)
				{
					m_pFaces[triIndex].IsNull = TRUE; //三角形无效
					m_vEdge.at(pos).IsNull = TRUE;//边无效
					count++;
				}
			}
		}
		if(count==0) break;
	}

	//此时边列表中还剩余一部分边
	while(1)
	{
		count=0;
		for(pos=0; pos < m_vEdge.size(); pos++)
		{
			//只有当边没有被删除时需要检查
			if(m_vEdge.at(pos).IsNull==FALSE)
			{
				//检查起始点在其他得边中是否还存在
				flag=FALSE;
				for(k=0; k < m_vEdge.size(); k++)
				{
					if(m_vEdge.at(k).IsNull==FALSE && k!=pos)
					{
						int tstart1,tstart2,tend2;
						tstart1=m_vEdge.at(pos).start;
						if(tstart1==346)
						{
							int aa;
							aa=1;
						}
						tstart2=m_vEdge.at(k).start;
						tend2=m_vEdge.at(k).end;
						if((m_vEdge.at(pos).start==m_vEdge.at(k).start) 
							|| (m_vEdge.at(pos).start==m_vEdge.at(k).end))
						{
							flag=TRUE; //存在
							break;
						}
					}
				}
				//其他边中没有该点,删除该边和该点
				if(!flag)
				{
					m_vEdge.at(pos).IsNull=TRUE;
					int t;
					t=m_vEdge.at(pos).start;
					m_pPts[t].IsNull=TRUE;
					count++;
				}
				
				if(flag==TRUE)
				{
					//检查边的终点在其他得边中是否还存在
					flag=FALSE;
					for(k=0; k < m_vEdge.size() ; k++)
					{
						if(m_vEdge.at(k).IsNull==FALSE && k!=pos)
						{
							int tstart2,tend1,tend2;
							tend1=m_vEdge.at(pos).end;
							tend2=m_vEdge.at(k).end;
							tstart2=m_vEdge.at(k).start;

							if((m_vEdge.at(pos).end==m_vEdge.at(k).start) 
							|| (m_vEdge.at(pos).end==m_vEdge.at(k).end))
							{
								flag=TRUE; //存在
								break;
							}
						}
					}
					//其他边中没有该点,删除该边和该点
					if(!flag)
					{
						m_vEdge.at(pos).IsNull=TRUE;
						int t;
						t=m_vEdge.at(pos).end;
						m_pPts[t].IsNull=TRUE;
						count++;
					}
				}
			}
		}
		if(count==0) break;
	}

	int nLeftPt=0;
	int PtIndex;
	int EdgeCnt=0;

	//如果最后只剩下一个点,则将该点相邻的两条边作为初始切线
	for(i=0; i< m_nPtSum; i++)
	{
		if(m_pPts[i].IsNull==FALSE)
		{
			nLeftPt ++;
			PtIndex=i;
		}
	}


	if(nLeftPt==1)
	{
		for(pos=0 ; pos < m_vEdge.size(); pos++)
		{
			if((m_vEdge.at(pos).start==PtIndex||m_vEdge.at(pos).end==PtIndex) && EdgeCnt < 2)
			{ 
				m_vEdge.at(pos).IsNull=FALSE;
				EdgeCnt++;
			}
			if(EdgeCnt==2) break;
		}
	}


	
	EDGE_ edge_Temp;
	GEOIMG_EDGE edgeTemp2;
	FILE *fp;
	fp=fopen("initialcut.txt","w+");
	//检查最后剩下的边，加入到cutpath中
	for(pos=0 ; pos < m_vEdge.size(); pos++)
	{
		edgeTemp2=m_vEdge.at(pos);
		if(edgeTemp2.IsNull==FALSE)
		{
			edge_Temp.start = edgeTemp2.start;
			edge_Temp.end   = edgeTemp2.end;
			fprintf(fp,"%d : %d, %d\n" , pos, edge_Temp.start, edge_Temp.end);
			edge_Temp.bIsBound=FALSE;
			edge_Temp.P[0] = meshPnt(edgeTemp2.start);
			edge_Temp.P[1] = meshPnt(edgeTemp2.end);
			edge_Temp.length = sqrt((edge_Temp.P[0].x-edge_Temp.P[1].x)*(edge_Temp.P[0].x-edge_Temp.P[1].x)
									+(edge_Temp.P[0].y-edge_Temp.P[1].y)*(edge_Temp.P[0].y-edge_Temp.P[1].y)
									+(edge_Temp.P[0].z-edge_Temp.P[1].z)*(edge_Temp.P[0].z-edge_Temp.P[1].z)); 
		//	edge_Temp.length= lf2sf(edge_Temp.length);
			edge_Temp.bIsBound = FALSE;
			m_vCutPath.push_back(edge_Temp);
		}
	}
	fclose(fp);

	if(m_nGenus>0)
	{
		StraightenCut();
		m_vCutPath.clear();
		m_vCutPath=m_vStrCutPath;
		m_vStrCutPath.clear();
	}
/*
	else
		m_vStrCutPath=m_vCutPath;
*/
}


void Mesh3D::StraightenCut()
{ 
	int i,j,k;
	int nStart,nEnd;
    int tempStart,tempEnd,tempCutIndex;
	int nJointCutNode;
	bool *found;
	float *dist;
	float **cost;
	int v,w;
	EDGE_ edgeTemp;

	double tempLen;
  	int nCnt;
	int preCutIndex;
	vector<int> vShortPath;
	set<int> partCut;

	bool *bVisited;
	int *nPath;
	FILE *strcutfp;
	int ttStart,ttEnd;


	found=(bool*)malloc((m_nPtSum+m_nAddPnt)*sizeof(bool));
	dist=(float*)malloc((m_nPtSum+m_nAddPnt)*sizeof(float));
	cost=(float**)malloc((m_nPtSum+m_nAddPnt)*sizeof(float*));
	
	m_vStrCutPath.clear();
 	nCnt=0;
	i=0;
	
	strcutfp=fopen("straightCut.txt","w+");

	nPath=(int*)malloc((m_nPtSum+m_nAddPnt)*sizeof(int));
 
	bVisited=(bool*)malloc(m_vCutPath.size() * sizeof(bool));
	
	for(i=0; i < m_vCutPath.size(); i++)
		bVisited[i]=FALSE;
	
	//找出第一个valence大于2的cutnode	
	for(i=0; i < m_vCutPath.size(); i++)
	{ 
		nJointCutNode=0;
		for(j=0; j < m_vCutPath.size();j++)
		{ 
			if((j!=i)&&
				(m_vCutPath.at(j).start==m_vCutPath.at(i).start||m_vCutPath.at(j).end==m_vCutPath.at(i).start))
			nJointCutNode++;
		}
		if(nJointCutNode>1)
			break;
	}

	nStart=m_vCutPath.at(i).start;
	
	//对各段cutnode进行分别处理，找出最短路径
	while(nCnt < m_vCutPath.size())
	{ 
		//对cost进行初始化为最大值
		for(i=0; i < (m_nPtSum+m_nAddPnt); i++)
		{ 
			cost[i]=(float*)malloc((m_nPtSum+m_nAddPnt)*sizeof(float));
			for(j=0; j < (m_nPtSum+m_nAddPnt); j++)
				cost[i][j]=INFINITE_MAX;
		}

		vShortPath.clear();
		partCut.clear();
		
		for(j=0; j < m_vCutPath.size();j++)
		{ 
			//如果这个cut从前没有被访问过并且与当前的节点相连接
			if(!bVisited[j]&&
				(m_vCutPath.at(j).start==nStart||m_vCutPath.at(j).end==nStart))
			{
				tempEnd = (m_vCutPath.at(j).end==nStart)?m_vCutPath.at(j).start:m_vCutPath.at(j).end;
				bVisited[j]=TRUE;
				ttStart =m_vCutPath.at(j).start;
				ttEnd = m_vCutPath.at(j).end;
				partCut.insert(ttStart);
				partCut.insert(ttEnd);
				break;
			}
		}
		tempCutIndex=j;
		nJointCutNode=0;
		//如果节点valence不大于2，则接着往下找，直到找到下一个分支口
		while (nJointCutNode<=1) 
		{
			nJointCutNode=0;
			
		    for(j=0; j < m_vCutPath.size(); j++)
			{ 
				if((j!=tempCutIndex)&&
				(m_vCutPath.at(j).start==tempEnd||m_vCutPath.at(j).end==tempEnd))
				{ 
					nJointCutNode++;
					preCutIndex=j;
				}
			}
 			nCnt++;
			if(nJointCutNode<=1)
			{ 
				tempEnd = (m_vCutPath.at(preCutIndex).start==tempEnd)?m_vCutPath.at(preCutIndex).end:m_vCutPath.at(preCutIndex).start;
				tempCutIndex = preCutIndex;
				bVisited[preCutIndex]=true;
				ttStart =m_vCutPath.at(preCutIndex).start;
				ttEnd = m_vCutPath.at(preCutIndex).end;
				partCut.insert(ttStart);
				partCut.insert(ttEnd);
			}
		}
		bVisited[tempCutIndex]=TRUE;
		ttStart =m_vCutPath.at(tempCutIndex).start;
		ttEnd = m_vCutPath.at(tempCutIndex).end;
		partCut.insert(ttStart);
		partCut.insert(ttEnd);

		nEnd = tempEnd;
		//////////////////////////////////////////////////////////////////////////
		//找出从nStart到nEnd之间的最短距离
		//////////////////////////////////////////////////////////////////////////
	    //只在当前切线段点集内找最短路径
		for(i=0; i < m_vEdge.size(); i++)
		{ 
			tempStart = m_vEdge.at(i).start;
			tempEnd = m_vEdge.at(i).end;
			if(partCut.find(tempStart)!=partCut.end()&&partCut.find(tempEnd)!=partCut.end())
			{
				tempLen = sqrt((meshPnt(tempStart).x - meshPnt(tempEnd).x)*(meshPnt(tempStart).x - meshPnt(tempEnd).x)+
							   (meshPnt(tempStart).y - meshPnt(tempEnd).y)*(meshPnt(tempStart).y - meshPnt(tempEnd).y)+
							   (meshPnt(tempStart).z - meshPnt(tempEnd).z)*(meshPnt(tempStart).z - meshPnt(tempEnd).z));
				cost[tempStart][tempEnd]=tempLen;
				cost[tempEnd][tempStart]=tempLen;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//初始化
		for(i=0; i < (m_nPtSum+m_nAddPnt); i++)
			nPath[i]=nStart;

		for(v=0;  v< (m_nPtSum+m_nAddPnt); v++)
		{ 
			found[v]=false;
			dist[v]=cost[nStart][v];
		}
		found[nStart]=true;
		dist[nStart]=0;
		//////////////////////////////////////////////////////////////////////////
		float min;
		for(k=0; k < (m_nPtSum+m_nAddPnt)&&!found[nEnd]; k++)
		{ 
			min=INFINITE_MAX;
			for(w = 0; w < (m_nPtSum+m_nAddPnt)&&!found[nEnd]; w++)
			{ 
				if(!found[w])
				{ 
					if(dist[w]<min)
					{
						v=w;
						min=dist[w];
					}
				}
			}
			found[v]=true;
			for(w=0; w <(m_nPtSum+m_nAddPnt); w++)
			{ 
				if(!found[w])
				{ 
					if((min+cost[v][w]) < dist[w])
					{ 
						dist[w]=min+cost[v][w];
						nPath[w]=v;
					}
				}
			}
		}
		//找出最短路径中的各个点
		int nextNode;
		vShortPath.push_back(nEnd);
		nextNode=nEnd;
		while(nextNode!=nStart)
		{
			vShortPath.push_back(nPath[nextNode]);
			nextNode = nPath[nextNode];
		}

			
		for(v=vShortPath.size()-1; v>0; v--)
		{ 
			edgeTemp.start = vShortPath.at(v);	
			edgeTemp.end = vShortPath.at(v-1);
			edgeTemp.bIsBound=FALSE;
			edgeTemp.P[0] = meshPnt(edgeTemp.start);
			edgeTemp.P[1] = meshPnt(edgeTemp.end);
			edgeTemp.length = sqrt((edgeTemp.P[0].x-edgeTemp.P[1].x)*(edgeTemp.P[0].x-edgeTemp.P[1].x)
									+(edgeTemp.P[0].y-edgeTemp.P[1].y)*(edgeTemp.P[0].y-edgeTemp.P[1].y)
									+(edgeTemp.P[0].z-edgeTemp.P[1].z)*(edgeTemp.P[0].z-edgeTemp.P[1].z)); 
		//	edgeTemp.length= lf2sf(edgeTemp.length);
			m_vStrCutPath.push_back(edgeTemp);
 			fprintf(strcutfp,"%d   %d\n",edgeTemp.start,edgeTemp.end);
		}
		// //////
		//进行下一个cutnode的搜索
		nStart=nEnd;		
	}

	fclose(strcutfp);
	free(found);
	found = NULL;
	free(dist);
	dist = NULL;
	for(i=0; i < (m_nPtSum+m_nAddPnt); i++)
		free(cost[i]);
	free(cost);
	cost = NULL;
	free(bVisited);
	bVisited = NULL;
}


 
void Mesh3D::AugCut()
{

	int i;
	int MaxStrTri;
	double GeoStretch,lastGeoStretch;
	vector<EDGE_> vlastCutPath;
	vector<GEOIMG_EDGE> vPreEdge;

	static POINT3D *pPrePts;	
	stFACE  *pPreFaces;

	static POINT3D *pPreAddPts;
	stFACE  *pPreAddFaces;

	int nPrePtSum, nPreAddPtSum, nPreFaceSum, nPreAddFaceSum;
    	
	m_nAugCount = 0;

	CircleBndParam();
	FloaterParam(3);
   	MaxStrTri = FindMaxStrTri();
 	BoundaryParam();
	FloaterParam(1);
	computeGeoStretch(&GeoStretch);

	vlastCutPath = m_vCutPath;
	vPreEdge = m_vEdge;
	//////////////////////////////////////////////////////////////////////////
	pPrePts = (POINT3D*)malloc(m_nPtSum * sizeof(POINT3D));
	//pPreAddPts = (POINT3D*)malloc(m_nAddPnt * sizeof(POINT3D));
	pPreAddPts = (POINT3D*)malloc(NUMALLOC * sizeof(POINT3D));

	pPreFaces = (stFACE*)malloc(m_nFaceSum * sizeof(stFACE));
	//pPreAddFaces = (stFACE*)malloc(m_nAddFaces * sizeof(stFACE));
	pPreAddFaces = (stFACE*)malloc(NUMALLOC * sizeof(stFACE));
	
	nPrePtSum = m_nPtSum;
	for(i = 0; i < m_nPtSum ; i++)
		pPrePts[i]=m_pPts[i];
	
	nPreAddPtSum = m_nAddPnt;
	for(i=0; i < m_nAddPnt; i++)
		pPreAddPts[i] = m_pAddPts[i];

	nPreFaceSum = m_nFaceSum;
	for(i = 0; i < m_nFaceSum ; i++)
		pPreFaces[i] = m_pFaces[i];

	nPreAddFaceSum = m_nAddFaces;
	for(i = 0; i < m_nAddFaces; i++)
		pPreAddFaces[i] = m_pAddFaces[i];
	//////////////////////////////////////////////////////////////////////////


	while (1)
	{
		lastGeoStretch=GeoStretch;
	 
			AddCutPath(MaxStrTri);
			if(m_nAddCutCnt==0)//如果切线没有改进，则停止
			{
				break;
			}
			else
			{ 
				CircleBndParam();
				FloaterParam(3);
		 		MaxStrTri=FindMaxStrTri();
		 		BoundaryParam();
		 		FloaterParam(1);
				computeGeoStretch(&GeoStretch);
				if(GeoStretch >=lastGeoStretch) 
				{
					m_vCutPath.clear();
					m_vCutPath=vlastCutPath;

					m_vEdge.clear();
					m_vEdge = vPreEdge;
					
					//////////////////////////////////////////////////////////////////////////
					
					m_nPtSum = nPrePtSum;
					for(i = 0; i < m_nPtSum ; i++)
						m_pPts[i] =	pPrePts[i];
					
					m_nAddPnt = nPreAddPtSum;
					for(i=0; i < m_nAddPnt; i++)
						m_pAddPts[i] = pPreAddPts[i] ;

					m_nFaceSum = nPreFaceSum;
					for(i = 0; i < m_nFaceSum ; i++)
						m_pFaces[i] = pPreFaces[i];

					m_nAddFaces = nPreAddFaceSum;
					for(i = 0; i < m_nAddFaces; i++)
						m_pAddFaces[i] = pPreAddFaces[i];
					//////////////////////////////////////////////////////////////////////////
					//AfxMessageBox("DONE!");
					break;

				}
				vlastCutPath.clear();
				vlastCutPath = m_vCutPath;

				vPreEdge.clear();
				vPreEdge = m_vEdge;

				
				//////////////////////////////////////////////////////////////////////////
				free(pPrePts);
				free(pPreAddPts);
				free(pPreFaces);
				free(pPreAddFaces);
				
				pPrePts = (POINT3D*)malloc(m_nPtSum * sizeof(POINT3D));
				pPreAddPts = (POINT3D*)malloc(m_nAddPnt * sizeof(POINT3D));

				pPreFaces = (stFACE*)malloc(m_nFaceSum * sizeof(stFACE));
				pPreAddFaces = (stFACE*)malloc(m_nAddFaces * sizeof(stFACE));
				
				nPrePtSum = m_nPtSum;
				for(i = 0; i < m_nPtSum ; i++)
					pPrePts[i]=m_pPts[i];
				
				nPreAddPtSum = m_nAddPnt;
				for(i=0; i < m_nAddPnt; i++)
					pPreAddPts[i] = m_pAddPts[i];

				nPreFaceSum = m_nFaceSum;
				for(i = 0; i < m_nFaceSum ; i++)
					pPreFaces[i] = m_pFaces[i];

				nPreAddFaceSum = m_nAddFaces;
				for(i = 0; i < m_nAddFaces; i++)
					pPreAddFaces[i] = m_pAddFaces[i];
				//////////////////////////////////////////////////////////////////////////
				m_nAugCount++;
			}		
/*
			if(m_nAugCount==3)
				break;	
*/

	} 
	
	
	FILE *fp;
	fp=fopen("newcut.txt","w+");
	for(i=0; i < m_vCutPath.size(); i++)
	{ 
		fprintf(fp,"%d  %d\n",m_vCutPath.at(i).start,m_vCutPath.at(i).end);
	}
	fclose(fp);

    free(pPrePts);
	pPrePts = NULL;
	free(pPreAddPts);
	pPreAddPts = NULL;
	free(pPreFaces);
	pPreFaces = NULL;
	free(pPreAddFaces);
	pPreAddFaces  = NULL;
	vPreEdge.clear();
	vlastCutPath.clear();
 }

void Mesh3D::AddCutPath(int MaxStrTri)
{
	int i,j,k,c;
	bool *found;
	float *dist;
	float **cost;
	int v,w;
	int nStart,nEnd;
	int tempStart,tempEnd;
	int *nPath;
	double *minStartDist,*minEndDist;
	float min;
	double tempLen;


	
	found=(bool*)malloc((m_nPtSum+m_nAddPnt)*sizeof(bool));
	dist=(float*)malloc((m_nPtSum+m_nAddPnt)*sizeof(float));
	cost=(float**)malloc((m_nPtSum+m_nAddPnt)*sizeof(float*));
	nPath=(int*)malloc((m_nPtSum+m_nAddPnt)*sizeof(int));
	minStartDist = (double*)malloc(m_vCutPath.size()*sizeof(double));
	minEndDist = (double*)malloc(m_vCutPath.size()*sizeof(double));
	//////////////////////////////////////////////////////////////////////////
	//初始化
	//////////////////////////////////////////////////////////////////////////
	for(i=0; i < (m_nPtSum+m_nAddPnt); i++)
	{ 
		cost[i]=(float*)malloc((m_nPtSum+m_nAddPnt)*sizeof(float));
		for(j=0; j < (m_nPtSum+m_nAddPnt); j++)
			cost[i][j]=INFINITE_MAX;
	}
	
	for(i=0; i < m_vEdge.size(); i++)
	{ 
		tempStart = m_vEdge.at(i).start;
		tempEnd = m_vEdge.at(i).end;
 		tempLen = sqrt((meshPnt(tempStart).x - meshPnt(tempEnd).x)*(meshPnt(tempStart).x - meshPnt(tempEnd).x)+
					   (meshPnt(tempStart).y - meshPnt(tempEnd).y)*(meshPnt(tempStart).y - meshPnt(tempEnd).y)+
					   (meshPnt(tempStart).z - meshPnt(tempEnd).z)*(meshPnt(tempStart).z - meshPnt(tempEnd).z));
		cost[tempStart][tempEnd]=tempLen;
		cost[tempEnd][tempStart]=tempLen;
	}
	/////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	nEnd=meshFaces(MaxStrTri).vI[0];
	
	for(c=0; c < m_vCutPath.size();c++)
	{ 
		nStart=m_vCutPath.at(c).start;
 		for(v=0;  v< (m_nPtSum+m_nAddPnt); v++)
		{ 
			found[v]=false;
			dist[v]=cost[nStart][v];
		}
		found[nStart]=true;
		dist[nStart]=0;
		//////////////////////////////////////////////////////////////////////////
		for(k=0; k < (m_nPtSum+m_nAddPnt)&&!found[nEnd]; k++)
		{ 
			min=INFINITE_MAX;
			for(w = 0; w < (m_nPtSum+m_nAddPnt)&&!found[nEnd]; w++)
			{ 
				if(!found[w])
				{ 
					if(dist[w]<min)
					{
						v=w;
						min=dist[w];
					}
				}
			}
			found[v]=true;
			for(w=0; w <(m_nPtSum+m_nAddPnt); w++)
			{ 
				if(!found[w])
				{ 
					if((min+cost[v][w]) < dist[w])
						dist[w]=min+cost[v][w];
				}
			}
		}
		minStartDist[c]=dist[nEnd];
	}

	for(c=0; c < m_vCutPath.size();c++)
	{ 
		nStart=m_vCutPath.at(c).end;
 		for(v=0;  v<(m_nPtSum+m_nAddPnt); v++)
		{ 
			found[v]=false;
			dist[v]=cost[nStart][v];
		}
		found[nStart]=true;
		dist[nStart]=0;
		//////////////////////////////////////////////////////////////////////////
		for(k=0; k < (m_nPtSum+m_nAddPnt)&&!found[nEnd]; k++)
		{ 
			min=INFINITE_MAX;
			for(w = 0; w < (m_nPtSum+m_nAddPnt)&&!found[nEnd]; w++)
			{ 
				if(!found[w])
				{ 
					if(dist[w]<min)
					{
						v=w;
						min=dist[w];
					}
				}
			}
			found[v]=true;
			for(w=0; w <(m_nPtSum+m_nAddPnt); w++)
			{ 
				if(!found[w])
				{ 
					if((min+cost[v][w]) < dist[w])
						dist[w]=min+cost[v][w];
				}
			}
		}
		minEndDist[c]=dist[nEnd];
	}
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	double minDist;
	int tStart;
	minDist=100000;

	for(c=0;  c <m_vCutPath.size(); c++)
	{ 
		if(minStartDist[c]<minDist) 
		{
			minDist=minStartDist[c];
			tStart=m_vCutPath.at(c).start;
		}
		if(minEndDist[c]<minDist) 
		{
			minDist=minEndDist[c];
			tStart=m_vCutPath.at(c).end;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	for(v=0;  v< (m_nPtSum+m_nAddPnt); v++)
	{ 
		found[v]=false;
		dist[v]=cost[tStart][v];
	}
	found[tStart]=true;
	dist[tStart]=0;
	for(i=0; i < (m_nPtSum+m_nAddPnt); i++)
		nPath[i]=tStart;

	//////////////////////////////////////////////////////////////////////////
	for(k=0; k < (m_nPtSum+m_nAddPnt)&&!found[nEnd]; k++)
	{ 
		min=INFINITE_MAX;
		for(w = 0; w < (m_nPtSum+m_nAddPnt)&&!found[nEnd]; w++)
		{ 
			if(!found[w])
			{ 
				if(dist[w]<min)
				{
					v=w;
					min=dist[w];
				}
			}
		}
		found[v]=true;
		for(w=0; w <(m_nPtSum+m_nAddPnt); w++)
		{ 
			if(!found[w])
			{ 
				if((min+cost[v][w]) < dist[w])
				{
					dist[w]=min+cost[v][w];
					nPath[w]=v;
				} 
			}
		}
	}

	//找出最短路径中的各个点
	int nextNode;
	//stack<int> ShortPath;
	vector<int> vShortPath;
	vShortPath.push_back(nEnd);
//	ShortPath.push(nEnd);
	nextNode=nEnd;
	while(nextNode!=tStart)
	{
	 	vShortPath.push_back(nPath[nextNode]);
		//ShortPath.push(nPath[nextNode]);
		nextNode = nPath[nextNode];
	}
	
	//如果genus为0，而且这是第一次进行切线优化，则完全抛弃原来的那条任意切线
	if(m_nGenus==0&&m_nAugCount==0&&vShortPath.size()>1)
		m_vCutPath.clear();
	EDGE_ edge_Temp;
	int nAddCut=0;
 	for(i=(vShortPath.size()-1); i > 0; i--)
	{ 
		ASSERT(i<vShortPath.size());
		tempStart = vShortPath.at(i);
		tempEnd   = vShortPath.at(i-1);
		edge_Temp.start = tempStart;
		edge_Temp.end   = tempEnd;
		edge_Temp.bIsBound=FALSE;
		edge_Temp.P[0] = meshPnt(tempStart);
		edge_Temp.P[1] = meshPnt(tempEnd);
		edge_Temp.length = sqrt((edge_Temp.P[0].x-edge_Temp.P[1].x)*(edge_Temp.P[0].x-edge_Temp.P[1].x)
								+(edge_Temp.P[0].y-edge_Temp.P[1].y)*(edge_Temp.P[0].y-edge_Temp.P[1].y)
								+(edge_Temp.P[0].z-edge_Temp.P[1].z)*(edge_Temp.P[0].z-edge_Temp.P[1].z)); 
		//edge_Temp.length= lf2sf(edge_Temp.length);
		edge_Temp.bIsBound = FALSE;
		m_vCutPath.push_back(edge_Temp);
		nAddCut++;
	}
	int m;
    if(m_vCutPath.size()==1)
	{ 
		for(m=0; m< 3 ; m++)
		{ 
			if(meshFaces(MaxStrTri).vI[m]!=nEnd)
				break;
		}
		tempStart = meshFaces(MaxStrTri).vI[m];
		tempEnd   = nEnd;
		edge_Temp.start = tempStart;
		edge_Temp.end   = tempEnd;
		edge_Temp.bIsBound=FALSE;
		edge_Temp.P[0] = meshPnt(tempStart);
		edge_Temp.P[1] = meshPnt(tempEnd);
		edge_Temp.length = sqrt((edge_Temp.P[0].x-edge_Temp.P[1].x)*(edge_Temp.P[0].x-edge_Temp.P[1].x)
								+(edge_Temp.P[0].y-edge_Temp.P[1].y)*(edge_Temp.P[0].y-edge_Temp.P[1].y)
								+(edge_Temp.P[0].z-edge_Temp.P[1].z)*(edge_Temp.P[0].z-edge_Temp.P[1].z)); 
		//edge_Temp.length= lf2sf(edge_Temp.length);
		edge_Temp.bIsBound = FALSE;
		m_vCutPath.push_back(edge_Temp);
		nAddCut++;
	}
	m_nAddCutCnt=nAddCut;


	FILE *fp;
	fp=fopen("tempnewcut.txt","w+");
	for(i=0; i < m_vCutPath.size(); i++)
	{ 
		fprintf(fp,"%d  %d\n",m_vCutPath.at(i).start,m_vCutPath.at(i).end);
	}
	fclose(fp);	

}

void Mesh3D::CircleBndParam()
{
	int i;
	double CutLength;
	double coordX,coordY;
	double ScaleCoff;
	double tempAngle,preAngle;
	int idTemp;
	MapPoint MapPnt_temp;

	m_vBndMapPnt.clear();

	//////////////////////////////////////////////////////////////////////////
	CutUnwrap2();
		
	//计算切线总长度
	if(m_vSideBand.size()==0)
	{
		AfxMessageBox("the cutpath does not exist!");
		return;
	}

	CutLength=0;
	for(i=0; i < m_vSideBand.size(); i++)
	{ 
		CutLength+=m_vSideBand.at(i).length;
	}
	const double pi=3.1415926;
	ScaleCoff = 2*pi/CutLength;
	coordX=1;
	coordY=0;
	tempAngle=0;
	preAngle=0;

	idTemp=m_vSideBand.at(0).start;
	MapPnt_temp.pointIndex = idTemp;
	MapPnt_temp.p.x=coordX;
	MapPnt_temp.p.y=coordY;
	MapPnt_temp.fx=meshPnt(idTemp).x;
	MapPnt_temp.fy=meshPnt(idTemp).y;
	MapPnt_temp.fz=meshPnt(idTemp).z;
	m_vBndMapPnt.push_back(MapPnt_temp);
 	
	for(i=1; i < m_vSideBand.size(); i++)
	{ 
		tempAngle=m_vSideBand.at(i-1).length * ScaleCoff;
		tempAngle= preAngle+tempAngle;
		coordX = cos(tempAngle);
		coordY = sin(tempAngle);
		
		idTemp=m_vSideBand.at(i).start;
		MapPnt_temp.pointIndex = idTemp;
		MapPnt_temp.p.x=coordX;
		MapPnt_temp.p.y=coordY;
		MapPnt_temp.fx=meshPnt(idTemp).x;
		MapPnt_temp.fy=meshPnt(idTemp).y;
		MapPnt_temp.fz=meshPnt(idTemp).z;
		m_vBndMapPnt.push_back(MapPnt_temp);
		preAngle=tempAngle;
	}
/*
	idTemp=m_vSideBand.at(0).start;
	MapPnt_temp.pointIndex = idTemp;
	MapPnt_temp.p.x=1.0;
	MapPnt_temp.p.y=0;
	MapPnt_temp.fx=meshPnt(idTemp).x;
	MapPnt_temp.fy=meshPnt(idTemp).y;
	MapPnt_temp.fz=meshPnt(idTemp).z;
	m_vBndMapPnt.push_back(MapPnt_temp);
*/
}


void Mesh3D::empty()
{
	m_vCutPath.clear();
	m_vBoundary.clear();
	m_vEdge.clear();
}

void Mesh3D::DisplayCutpath()
{
 	int i;
	float t1,t2,t3,t4,t5,t6;
	
 	glColor3f(1.0f, 1.0f, 1.0f);
 	glLineWidth(5.0);
	
  	for(i=0; i< m_vCutPath.size(); i++)
 	 	{
 	 		t1=m_vCutPath.at(i).P[0].x;
 	 		t2=m_vCutPath.at(i).P[0].y;
 	 		t3=m_vCutPath.at(i).P[0].z;
 	 		t4=m_vCutPath.at(i).P[1].x;
 	 		t5=m_vCutPath.at(i).P[1].y;
 	 		t6=m_vCutPath.at(i).P[1].z;
 	 
 	 		glBegin(GL_LINES);
 	 			glVertex3f(t1,t2,t3);
 	 			glVertex3f(t4,t5,t6);
 	 		glEnd();
 	 	} 
 	
 	glColor3f(1.0f, 1.0f, 1.0f);
 	glLineWidth(1.0f);
}

void Mesh3D::DeleteSameTri()
{
	int i,j;
	for(i=0; i < m_nFaceSum; i++)
	{
		for(j=0; j < i; j++)
		{
			if(m_pFaces[j].IsNull==FALSE)
			{
				POINT3D temp1,temp2;
				temp1.x=m_pFaces[i].vI[0];
				temp1.y=m_pFaces[i].vI[1];
				temp1.z=m_pFaces[i].vI[2];

				temp2.x=m_pFaces[j].vI[0];
				temp2.y=m_pFaces[j].vI[1];
				temp2.z=m_pFaces[j].vI[2];

				if(IsSame(temp1,temp2))
				{
					m_pFaces[i].IsNull=TRUE;
					break;
				}
			}
		}
	}
}

bool Mesh3D::IsSame(POINT3D a, POINT3D b)
{
	int t1,t2,t3;
	int s1,s2,s3;

	t1=MaxOf3(a.x, a.y, a.z);
	t2=MiddleOf3(a.x, a.y, a.z);
	t3=MinOf3(a.x, a.y, a.z);

	s1=MaxOf3(b.x, b.y, b.z);
	s2=MiddleOf3(b.x, b.y, b.z);
	s3=MinOf3(b.x, b.y, b.z);

	if(s1==t1 && s2==t2 && s3==t3)
		return(TRUE);
	else
		return(FALSE);
}



int Mesh3D::MaxOf3(int t1, int t2, int t3)
{
	int max;
	max=t1;
	if(max < t2) 
		max = t2;
	if(max < t3)
		max = t3;
	return(max);
}

int Mesh3D::MiddleOf3(int t1, int t2, int t3)
{
	int mid;
	
	if((t1 > t2 && t1 < t3)||(t1 > t3 && t1 < t2)) mid=t1;
	else if((t2 > t1 && t2 < t3)||(t2 > t3 && t2 < t1)) mid=t2;
	else if((t3 > t1 && t3 < t2)||(t3 > t2 && t3 < t1)) mid=t3;
	return(mid);
}


int Mesh3D::MinOf3(int t1, int t2, int t3)
{
	int min;
	min=t1;
	if(min > t2) 
		min=t2;
	if(min > t3)
		min= t3;
	return(min);
}



void Mesh3D::DisplayBoundary()
{
	int i;
	
	glColor3f(0.0f, 1.0f, 1.0f);
	glLineWidth(5.0);
	for(i=0; i< m_vBoundary.size(); i++)
	{
		float t1,t2,t3,t4,t5,t6;
		t1=m_vBoundary.at(i).P[0].x;
		t2=m_vBoundary.at(i).P[0].y;
		t3=m_vBoundary.at(i).P[0].z;
		t4=m_vBoundary.at(i).P[1].x;
		t5=m_vBoundary.at(i).P[1].y;
		t6=m_vBoundary.at(i).P[1].z;

		glBegin(GL_LINES);
			glVertex3f(t1,t2,t3);
			glVertex3f(t4,t5,t6);
		glEnd();
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);	
}


void Mesh3D::BoundaryParam()
{
	double CutLength;
	MapPoint MapPnt_temp;
	int i,j,k;
	double dist;
	int dir;
	double tempx, tempy;
	double ScaleCoff;
	int idTemp;
	bool flag=true;
	POINT2D p1,p2,p3;
	int t1,t2,t3;
	double prex,prey;

	FILE *cutfp;
	cutfp=fopen("newcut.txt","w+");
	for(i=0; i < m_vCutPath.size(); i++)
	{ 
		fprintf(cutfp,"%d  %d\n",m_vCutPath.at(i).start,m_vCutPath.at(i).end);
	}
	fclose(cutfp);


	if(m_vSideBand.size()==0)
		CutUnwrap2();
	
	m_vBndMapPnt.clear();
	//标识方向，1为y＋，2为x＋，3为y－，4为x－；
	
	//计算切线总长度

	if(m_vSideBand.size()==0)
	{
		AfxMessageBox("the cutpath does not exist!");
		return;
	}


	CutLength=0;
	for(i=0; i < m_vSideBand.size(); i++)
	{ 
	 //	CutLength+=lf2sf(m_vSideBand.at(i).length);
		CutLength+=(m_vSideBand.at(i).length);
	}

	//确定切线段在单位正方形上的映射系数
	ScaleCoff=4/double(CutLength);

	m_lfScale = ScaleCoff;
	//ScaleCoff = lf2sf(ScaleCoff);

	//将第一个切点放在(0,0)
	idTemp=m_vSideBand.at(0).start;
	MapPnt_temp.pointIndex=idTemp;
	MapPnt_temp.p.x=0;
	MapPnt_temp.p.y=0;
 	MapPnt_temp.fx=meshPnt(idTemp).x;
	MapPnt_temp.fy=meshPnt(idTemp).y;
	MapPnt_temp.fz=meshPnt(idTemp).z;
	m_vBndMapPnt.push_back(MapPnt_temp);

	dir=1;//初始朝y＋方向
	prey=0.0;
	
	for(i=1; i < m_vSideBand.size()&&flag; i++)
	{
		//计算该段切线在正方形上的映射长度
		dist=m_vSideBand.at(i-1).length * ScaleCoff;
	//	dist = lf2sf(dist);
		 
		switch(dir) {
		case 1: //y+  x=0
			{
				tempx=0;
 				tempy=m_vBndMapPnt.at(i-1).p.y+dist;
		//		tempy=lf2sf(tempy);
			//	if(tempy==prey)
			//		tempy=tempy+lf2sf(1/(double)NGRID);
 				if(tempy >1.0 && fabs(tempy-1.0) > Infinite_Min) 
				{
					if(m_vSideBand.at(i-1).mate !=-1)
						BreakEdge(&dir, dist, tempy,ScaleCoff, i-1);	
					else
						BreakEdge1(&dir, dist, tempy,ScaleCoff, i-1);	
				}
				else //存储正方形边界点的信息（包括映射的3D点的位置以及点的index）
				{ 
					idTemp=m_vSideBand.at(i).start;
					MapPnt_temp.pointIndex = idTemp;
					MapPnt_temp.p.x=tempx;
					MapPnt_temp.p.y=tempy;
					MapPnt_temp.fx=meshPnt(idTemp).x;
					MapPnt_temp.fy=meshPnt(idTemp).y;
					MapPnt_temp.fz=meshPnt(idTemp).z;
					m_vBndMapPnt.push_back(MapPnt_temp);
					if(fabs(tempy-1.0) < Infinite_Min) 
						dir = 2;
				}
				prey=tempy;
				prex=tempx;
			}
			break;
		case 2: //x+ y=1
			{
				tempy=1;
				tempx=m_vBndMapPnt.at(i-1).p.x+dist;
			//	tempx=lf2sf(tempx);

			//	if(tempx==prex)
			//		tempx=tempx+lf2sf(1/(double)NGRID);

 				if (tempx > 1.0 && fabs(tempx-1.0) > Infinite_Min) 
				{
 					if(m_vSideBand.at(i-1).mate !=-1)
						BreakEdge(&dir, dist, tempx,ScaleCoff, i-1);	
					else
						BreakEdge1(&dir, dist, tempx,ScaleCoff, i-1);
				}
				else
				{ 
					idTemp=m_vSideBand.at(i).start;
					MapPnt_temp.pointIndex = idTemp;
					MapPnt_temp.p.x=tempx;
					MapPnt_temp.p.y=tempy;
					MapPnt_temp.fx=meshPnt(idTemp).x;
					MapPnt_temp.fy=meshPnt(idTemp).y;
					MapPnt_temp.fz=meshPnt(idTemp).z;
					m_vBndMapPnt.push_back(MapPnt_temp);
					if(fabs(tempx-1.0) < Infinite_Min) 
						dir = 3;
				}
				prex=tempx;
				prey=tempy;
			}
			break;
		case 3: //x=1 y-
			{
				tempx=1;
				tempy=m_vBndMapPnt.at(i-1).p.y-dist;
			//	tempy = lf2sf(tempy);

			//	if(tempy==prey)
			//		tempy=tempy+lf2sf(1/(double)NGRID);

 				if (tempy < 0.0 && fabs(tempy-0.0) > Infinite_Min) 
				{
 					if(m_vSideBand.at(i-1).mate !=-1)
						BreakEdge(&dir, dist, -tempy+1,ScaleCoff, i-1);	
					else
						BreakEdge1(&dir, dist, -tempy+1,ScaleCoff, i-1);
				}
				else
				{ 
					idTemp=m_vSideBand.at(i).start;
					MapPnt_temp.pointIndex = idTemp;
					MapPnt_temp.p.x=tempx;
					MapPnt_temp.p.y=tempy;
					MapPnt_temp.fx=meshPnt(idTemp).x;
					MapPnt_temp.fy=meshPnt(idTemp).y;
					MapPnt_temp.fz=meshPnt(idTemp).z;
					m_vBndMapPnt.push_back(MapPnt_temp);
					if(fabs(tempy-0.0) < Infinite_Min) 
						dir = 4;
				}
				prey=tempy;
				prex=tempx;
			}
			break;
		case 4: //y=1 x- 
			{
				tempy=0;
				tempx=m_vBndMapPnt.at(i-1).p.x-dist;
			//	tempx=lf2sf(tempx);

//				if(tempx==prex)
//					tempx=tempx+lf2sf(1/(double)NGRID);

 				if ((tempx-0.0) <= Infinite_Min)  //如果超过原点，则使之近似到原点
				{
 					dir=1;
					flag=false;
				}
				else
				{
 					idTemp=m_vSideBand.at(i).start;
					MapPnt_temp.pointIndex = idTemp;
					MapPnt_temp.p.x=tempx;
					MapPnt_temp.p.y=tempy;
					MapPnt_temp.fx=meshPnt(idTemp).x;
					MapPnt_temp.fy=meshPnt(idTemp).y;
					MapPnt_temp.fz=meshPnt(idTemp).z;
					m_vBndMapPnt.push_back(MapPnt_temp);
					prex=tempx;
					prey=tempy;
				}
			}
			break;
		default:
			break;
		}	
	}
	//确保最后一个点也回到原点
/*	idTemp=m_vSideBand.at(i-1).end;
	MapPnt_temp.pointIndex=idTemp;
	MapPnt_temp.p.x=0.0;
	MapPnt_temp.p.y=0.0;
	MapPnt_temp.fx=m_pPts[idTemp].x;
	MapPnt_temp.fy=m_pPts[idTemp].y;
	MapPnt_temp.fz=m_pPts[idTemp].z;
	m_vBndMapPnt.push_back(MapPnt_temp);
*/
	double tempPrex;

	tempPrex=prex;
	if(i<m_vSideBand.size())
	{ 
 		idTemp=m_vSideBand.at(i-1).start;
		MapPnt_temp.pointIndex = idTemp;
		MapPnt_temp.p.x=prex-tempPrex/(m_vSideBand.size()-i+2);
		MapPnt_temp.p.y=tempy;
		MapPnt_temp.fx=meshPnt(idTemp).x;
		MapPnt_temp.fy=meshPnt(idTemp).y;
		MapPnt_temp.fz=meshPnt(idTemp).z;
		m_vBndMapPnt.push_back(MapPnt_temp);
		prex=MapPnt_temp.p.x;
	}

	
	for(j=0; j <m_vSideBand.size()-i; j++)
	{ 
		idTemp=m_vSideBand.at(i+j-1).end;
		MapPnt_temp.pointIndex=idTemp;
		MapPnt_temp.p.x=prex-tempPrex/(m_vSideBand.size()-i+2);
		MapPnt_temp.p.y=0.0;
		MapPnt_temp.fx=meshPnt(idTemp).x;
		MapPnt_temp.fy=meshPnt(idTemp).y;
		MapPnt_temp.fz=meshPnt(idTemp).z;
		m_vBndMapPnt.push_back(MapPnt_temp);
		prex=MapPnt_temp.p.x;
	}
/*
 	idTemp=m_vSideBand.at(i+j-1).end;
	MapPnt_temp.pointIndex=idTemp;
	MapPnt_temp.p.x=0.0;
	MapPnt_temp.p.y=0.0;
	MapPnt_temp.fx=meshPnt(idTemp).x;
	MapPnt_temp.fy=meshPnt(idTemp).y;
	MapPnt_temp.fz=meshPnt(idTemp).z;
	m_vBndMapPnt.push_back(MapPnt_temp);
*/

	int edgeStart,edgeEnd;
	GEOIMG_EDGE tempEdge;
	int m,n,faceIndex,vexIndex;
	int tempSize;
	int t;


	FILE *facefp;
	facefp=fopen("facedebug.txt","w+");
	for(i=0; i < m_nFaceSum+m_nAddFaces; i++)
		fprintf(facefp,"%d--%d  %d  %d\n",i,meshFaces(i).vI[0],meshFaces(i).vI[1],meshFaces(i).vI[2]);
	fclose(facefp);

	int nTri;
	for(k=0; k < m_vEdge.size(); k++)
	{
		nTri=0;
	
		for(j=0; j < m_nFaceSum+m_nAddFaces; j++)
		{ 
			for(m = 0; m < 3; m++)
			{ 
				if((meshFaces(j).vI[m] == m_vEdge.at(k).start && meshFaces(j).vI[(m+1)%3] == m_vEdge.at(k).end)||
					(meshFaces(j).vI[m] == m_vEdge.at(k).end && meshFaces(j).vI[(m+1)%3] == m_vEdge.at(k).start))
				break;
			}
			if(m<3)
				m_vEdge.at(k).neighborTri[nTri++]=j;
		}
		for(j=nTri; j < MAXNEIGHBOR; j++)
			m_vEdge.at(k).neighborTri[j]=-1;
	}

 
	//如果一个三角形的三个顶点同时映射到同一边
	//则在非边界的一条边中引入一个点，将原三角形划分成两个三角形
	for(i=0; i < m_vBndMapPnt.size(); i++)
	{ 
		tempSize = m_vBndMapPnt.size()-1;

		p1 = m_vBndMapPnt.at(i).p;
		p2 = m_vBndMapPnt.at((i+1)%tempSize).p;
		p3 = m_vBndMapPnt.at((i+2)%tempSize).p;
		
		if(i==4)
			i=i;
		t1=m_vBndMapPnt.at(i).pointIndex;
		t2=m_vBndMapPnt.at((i+1)%tempSize).pointIndex;
		t3=m_vBndMapPnt.at((i+2)%tempSize).pointIndex;
		if((p1.x == p2.x&&p2.x==p3.x)||
			(p1.y == p2.y&&p2.y==p3.y))
		{ 
			for(t=0; t < m_nFaceSum+m_nAddFaces; t++)
			{ 
				if((t1==meshFaces(t).vI[0]&&t2==meshFaces(t).vI[1]&&t3==meshFaces(t).vI[2])||
					(t1==meshFaces(t).vI[1]&&t2==meshFaces(t).vI[2]&&t3==meshFaces(t).vI[0])||
					(t1==meshFaces(t).vI[2]&&t2==meshFaces(t).vI[0]&&t3==meshFaces(t).vI[1]))

				{ 
					edgeStart=t1;
					edgeEnd=t3;
					//插入一个点
					m_pAddPts[m_nAddPnt].x = ( meshPnt(edgeStart).x + meshPnt(edgeEnd).x)/2;
					m_pAddPts[m_nAddPnt].y = ( meshPnt(edgeStart).y + meshPnt(edgeEnd).y)/2;
					m_pAddPts[m_nAddPnt].z = ( meshPnt(edgeStart).z + meshPnt(edgeEnd).z)/2;

					//分裂三角形
					for(k=0 ; k < m_vEdge.size(); k++)
					{
						if((m_vEdge.at(k).start == edgeStart && m_vEdge.at(k).end == edgeEnd)
						|| (m_vEdge.at(k).start == edgeEnd && m_vEdge.at(k).end == edgeStart))
						break;
					}
	
					int node1,node2;

					for(n=0; n < MAXNEIGHBOR; n++)
					{
						faceIndex=m_vEdge.at(k).neighborTri[n];
 						if(faceIndex>-1)
						{
							//找出三角形的另外一个顶点vexIndex
							for(m= 0 ; m < 3 ; m++)
							{ 
								if(meshFaces(faceIndex).vI[m]!=edgeStart && meshFaces(faceIndex).vI[m]!=edgeEnd)
								{
									vexIndex = meshFaces(faceIndex).vI[m];
									break;
								}
							}
							node1 = meshFaces(faceIndex).vI[(m+1)%3];
							node2 = meshFaces(faceIndex).vI[(m+2)%3];

							//修改一个三角形信息
							if(faceIndex<m_nFaceSum)
							{
								m_pFaces[faceIndex].vI[0]=vexIndex;
								m_pFaces[faceIndex].vI[1]=m_nPtSum+m_nAddPnt;
								m_pFaces[faceIndex].vI[2]=node2;
							}
							else
							{ 
								m_pAddFaces[faceIndex-m_nFaceSum].vI[0]=vexIndex;
								m_pAddFaces[faceIndex-m_nFaceSum].vI[1]=m_nPtSum+m_nAddPnt;
								m_pAddFaces[faceIndex-m_nFaceSum].vI[2]=node2;
							}
							//另外再添加一个三角形
						 //	m_pAddFaces[m_nAddFaces] =(stFACE*)malloc(sizeof(stFACE));
							m_pAddFaces[m_nAddFaces].n=3;
							m_pAddFaces[m_nAddFaces].vI[0]=vexIndex;
							m_pAddFaces[m_nAddFaces].vI[1]=node1;
							m_pAddFaces[m_nAddFaces].vI[2]=m_nPtSum+m_nAddPnt;

							//插入新边
							tempEdge.start=vexIndex;
							tempEdge.end = m_nPtSum+m_nAddPnt;
							tempEdge.IsNull = FALSE;
							tempEdge.neighborTri[0]=faceIndex;
							tempEdge.neighborTri[1] = m_nAddFaces + m_nFaceSum;
							for(j=2; j < MAXNEIGHBOR; j++)
								tempEdge.neighborTri[j]=-1;
							m_vEdge.push_back(tempEdge);
							
							m_nAddFaces++;
						}
					}

					m_vEdge.at(k).start = edgeStart;
					m_vEdge.at(k).end = m_nPtSum +m_nAddPnt;
					m_vEdge.at(k).IsNull = FALSE;
					int nTri=0;

					for(n=0; n < m_nFaceSum+m_nAddFaces; n++)
					{ 
						for(m = 0; m < 3; m++)
						{ 
							if((meshFaces(n).vI[m] == m_vEdge.at(k).start && meshFaces(n).vI[(m+1)%3] == m_vEdge.at(k).end)||
								(meshFaces(n).vI[m] == m_vEdge.at(k).end && meshFaces(n).vI[(m+1)%3] == m_vEdge.at(k).start))
							break;
						}
						if(m<3)
							m_vEdge.at(k).neighborTri[nTri++]=n;
					}
					for(n=nTri; n < MAXNEIGHBOR; n++)
						m_vEdge.at(k).neighborTri[n]=-1;

					tempEdge.start = edgeEnd;
					tempEdge.end = m_nPtSum + m_nAddPnt;
					tempEdge.IsNull = FALSE;
					nTri = 0;

					for(j=0; j < m_nFaceSum+m_nAddFaces; j++)
					{ 
						for(m = 0; m < 3; m++)
						{ 
							if((meshFaces(j).vI[m] == tempEdge.start && meshFaces(j).vI[(m+1)%3] == tempEdge.end)||
								(meshFaces(j).vI[m] == tempEdge.end && meshFaces(j).vI[(m+1)%3] == tempEdge.start))
							break;
						}
						if(m < 3)
							tempEdge.neighborTri[nTri++]=j;
					}
					for(j=nTri; j < MAXNEIGHBOR; j++)
						m_vEdge.at(k).neighborTri[j]=-1;
					m_vEdge.push_back(tempEdge);

					m_nAddInPnt++;
					m_nAddPnt++;
					//////////////////////////////////////////////////////////////////////////
					//修改t1t2线段及t2t3线段的相邻三角形信息
					int nNeighborTri2=0;
   
					for(k=0; k < m_vEdge.size(); k++)
					{ 
						if((m_vEdge.at(k).start==t1&&m_vEdge.at(k).end==t2)||
							(m_vEdge.at(k).start==t2&&m_vEdge.at(k).end==t1))
							break;
					}
					for(j=0; j < MAXNEIGHBOR; j++)
						m_vEdge.at(k).neighborTri[j]=-1;

					for(n=0; n < m_nFaceSum+m_nAddFaces; n++)
					{ 
						for(m=0; m<3; m++)
						{ 
							if((meshFaces(n).vI[m]==t1&&meshFaces(n).vI[(m+1)%3]==t2)||
								(meshFaces(n).vI[m]==t2&&meshFaces(n).vI[(m+1)%3]==t1))
								break;
						}
						if(m<3)
						{ 
							m_vEdge.at(k).neighborTri[nNeighborTri2++]=n;
						}
					}
					////////////
					nNeighborTri2=0;
					for(k=0; k < m_vEdge.size(); k++)
					{ 
						if((m_vEdge.at(k).start==t3&&m_vEdge.at(k).end==t2)||
							(m_vEdge.at(k).start==t2&&m_vEdge.at(k).end==t3))
							break;
					}
					
					for(j=0; j < MAXNEIGHBOR; j++)
						m_vEdge.at(k).neighborTri[j]=-1;

					for(n=0; n < m_nFaceSum+m_nAddFaces; n++)
					{ 
						for(m=0; m<3; m++)
						{ 
							if((meshFaces(n).vI[m]==t3&&meshFaces(n).vI[(m+1)%3]==t2)||
								(meshFaces(n).vI[m]==t2&&meshFaces(n).vI[(m+1)%3]==t3))
								break;
						}
						if(m<3)
						{ 
							m_vEdge.at(k).neighborTri[nNeighborTri2++]=n;
						}
					}
				//////////////////////////////////////////////////////////////////////////
				}
			}
		}	
	}
	//////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////
	FILE *pointfp;
	pointfp=fopen("addpoint.txt","w+");
	for(i = 0; i<m_nAddPnt ;i++)
	{ 
		fprintf(pointfp,"%d----%lf  %lf  %lf\n",i,meshPnt(m_nPtSum+i).x,meshPnt(m_nPtSum+i).y,meshPnt(m_nPtSum+i).z);
	}
	fclose(pointfp);

	FILE* bfp;
	bfp=fopen("bound1.txt","w+");
	for(i = 0; i<m_vBndMapPnt.size(); i++)
	{ 
		fprintf(bfp,"%d----%lf  %lf  \n",i,m_vBndMapPnt.at(i).p.x,m_vBndMapPnt.at(i).p.y);
	}
	fclose(bfp);

 	FILE *fp;
	fp=fopen("sidebandafterbndparam.txt","w+");
	int tStart, tEnd,tMate;
	tStart=m_vSideBand.at(0).start;
	tEnd = m_vSideBand.at(0).end;
	tMate = m_vSideBand.at(0).mate;
	
 	for(i = 0; i < m_vSideBand.size(); i++)
	{ 
		tStart=m_vSideBand.at(i).start;
		tEnd = m_vSideBand.at(i).end;
		tMate = m_vSideBand.at(i).mate;
		fprintf(fp,"%d----%d      %d\n",tStart, tEnd,tMate);
	}
	 fclose(fp);

	FILE *edgefp;
	edgefp=fopen("newedge.txt","w+");
	for(i=0; i < m_vEdge.size(); i++)
		fprintf(edgefp,"%d  %d---%d  %d\n",m_vEdge.at(i).start,m_vEdge.at(i).end,m_vEdge.at(i).neighborTri[0],m_vEdge.at(i).neighborTri[1]);
	fclose(edgefp);

	FILE *cutfp2;
	cutfp2=fopen("cutafterbndParam.txt","w+");
	for(i=0; i < m_vCutPath.size(); i++)
	{ 
		fprintf(cutfp2,"%d---%d %d\n",i,m_vCutPath.at(i).start,m_vCutPath.at(i).end);
	}
	fclose(cutfp2);

		FILE *facefp2;
	facefp2=fopen("faceafterbndparam.txt","w+");
	for(i=0; i < m_nFaceSum+m_nAddFaces; i++)
		fprintf(facefp2,"%d--%d  %d  %d\n",i,meshFaces(i).vI[0],meshFaces(i).vI[1],meshFaces(i).vI[2]);
	fclose(facefp2);

 }

void Mesh3D::FinalBoundaryParam()
{
	double CutLength;
	MapPoint MapPnt_temp;
	int i,j,k;
	double dist;
	int dir;
	double tempx, tempy;
	double ScaleCoff;
	int idTemp;
	bool flag=true;
	POINT2D p1,p2,p3;
	int t1,t2,t3;
	double prex,prey;

	m_vSideBand.clear();
	CutUnwrap2();
	
	m_vBndMapPnt.clear();
	//标识方向，1为y＋，2为x＋，3为y－，4为x－；
	
	//计算切线总长度

	if(m_vSideBand.size()==0)
	{
		AfxMessageBox("the cutpath does not exist!");
		return;
	}


	CutLength=0;
	for(i=0; i < m_vSideBand.size(); i++)
	{ 
		CutLength+=lf2sf(m_vSideBand.at(i).length);
	}

	//确定切线段在单位正方形上的映射系数
	ScaleCoff=4/double(CutLength);
	ScaleCoff = lf2sf(ScaleCoff);

	//将第一个切点放在(0,0)
	idTemp=m_vSideBand.at(0).start;
	MapPnt_temp.pointIndex=idTemp;
	MapPnt_temp.p.x=0;
	MapPnt_temp.p.y=0;
 	MapPnt_temp.fx=meshPnt(idTemp).x;
	MapPnt_temp.fy=meshPnt(idTemp).y;
	MapPnt_temp.fz=meshPnt(idTemp).z;
	m_vBndMapPnt.push_back(MapPnt_temp);

	dir=1;//初始朝y＋方向
	prey=0.0;
	
	for(i=1; i < m_vSideBand.size()&&flag; i++)
	{
		//计算该段切线在正方形上的映射长度
		dist=m_vSideBand.at(i-1).length * ScaleCoff;
	 	dist = lf2sf(dist);
		 
		switch(dir) {
		case 1: //y+  x=0
			{
				tempx=0;
 				tempy=m_vBndMapPnt.at(i-1).p.y+dist;
		 		tempy=lf2sf(tempy);
			 	if(tempy==prey)
			 		tempy=tempy+lf2sf(1/(double)NGRID);
 				if(tempy >1.0 && fabs(tempy-1.0) > Infinite_Min) 
				{
					if(m_vSideBand.at(i-1).mate !=-1)
						BreakEdge(&dir, dist, tempy,ScaleCoff, i-1);	
					else
						BreakEdge1(&dir, dist, tempy,ScaleCoff, i-1);	
				}
				else //存储正方形边界点的信息（包括映射的3D点的位置以及点的index）
				{ 
					idTemp=m_vSideBand.at(i).start;
					MapPnt_temp.pointIndex = idTemp;
					MapPnt_temp.p.x=tempx;
					MapPnt_temp.p.y=tempy;
					MapPnt_temp.fx=meshPnt(idTemp).x;
					MapPnt_temp.fy=meshPnt(idTemp).y;
					MapPnt_temp.fz=meshPnt(idTemp).z;
					m_vBndMapPnt.push_back(MapPnt_temp);
					if(fabs(tempy-1.0) < Infinite_Min) 
						dir = 2;
				}
				prey=tempy;
				prex=tempx;
			}
			break;
		case 2: //x+ y=1
			{
				tempy=1;
				tempx=m_vBndMapPnt.at(i-1).p.x+dist;
			 	tempx=lf2sf(tempx);

			 	if(tempx==prex)
			 		tempx=tempx+lf2sf(1/(double)NGRID);

 				if (tempx > 1.0 && fabs(tempx-1.0) > Infinite_Min) 
				{
 					if(m_vSideBand.at(i-1).mate !=-1)
						BreakEdge(&dir, dist, tempx,ScaleCoff, i-1);	
					else
						BreakEdge1(&dir, dist, tempx,ScaleCoff, i-1);
				}
				else
				{ 
					idTemp=m_vSideBand.at(i).start;
					MapPnt_temp.pointIndex = idTemp;
					MapPnt_temp.p.x=tempx;
					MapPnt_temp.p.y=tempy;
					MapPnt_temp.fx=meshPnt(idTemp).x;
					MapPnt_temp.fy=meshPnt(idTemp).y;
					MapPnt_temp.fz=meshPnt(idTemp).z;
					m_vBndMapPnt.push_back(MapPnt_temp);
					if(fabs(tempx-1.0) < Infinite_Min) 
						dir = 3;
				}
				prex=tempx;
				prey=tempy;
			}
			break;
		case 3: //x=1 y-
			{
				tempx=1;
				tempy=m_vBndMapPnt.at(i-1).p.y-dist;
			 	tempy = lf2sf(tempy);

			 	if(tempy==prey)
			 		tempy=tempy-lf2sf(1/(double)NGRID);

 				if (tempy < 0.0 && fabs(tempy-0.0) > Infinite_Min) 
				{
 					if(m_vSideBand.at(i-1).mate !=-1)
						BreakEdge(&dir, dist, -tempy+1,ScaleCoff, i-1);	
					else
						BreakEdge1(&dir, dist, -tempy+1,ScaleCoff, i-1);
				}
				else
				{ 
					idTemp=m_vSideBand.at(i).start;
					MapPnt_temp.pointIndex = idTemp;
					MapPnt_temp.p.x=tempx;
					MapPnt_temp.p.y=tempy;
					MapPnt_temp.fx=meshPnt(idTemp).x;
					MapPnt_temp.fy=meshPnt(idTemp).y;
					MapPnt_temp.fz=meshPnt(idTemp).z;
					m_vBndMapPnt.push_back(MapPnt_temp);
					if(fabs(tempy-0.0) < Infinite_Min) 
						dir = 4;
				}
				prey=tempy;
				prex=tempx;
			}
			break;
		case 4: //y=1 x- 
			{
				tempy=0;
				tempx=m_vBndMapPnt.at(i-1).p.x-dist;
		 	tempx=lf2sf(tempx);

 				if(tempx==prex)
 					tempx=tempx-lf2sf(1/(double)NGRID);

 				if ((tempx-0.0) <= Infinite_Min)  //如果超过原点，则使之近似到原点
				{
 					dir=1;
					flag=false;
				}
				else
				{
 					idTemp=m_vSideBand.at(i).start;
					MapPnt_temp.pointIndex = idTemp;
					MapPnt_temp.p.x=tempx;
					MapPnt_temp.p.y=tempy;
					MapPnt_temp.fx=meshPnt(idTemp).x;
					MapPnt_temp.fy=meshPnt(idTemp).y;
					MapPnt_temp.fz=meshPnt(idTemp).z;
					m_vBndMapPnt.push_back(MapPnt_temp);
					prex=tempx;
					prey=tempy;
				}
			}
			break;
		default:
			break;
		}	
	}
 	double tempPrex;

	tempPrex=prex;
	if(i<m_vSideBand.size())
	{ 
 		idTemp=m_vSideBand.at(i-1).start;
		MapPnt_temp.pointIndex = idTemp;
		MapPnt_temp.p.x=prex-tempPrex/(m_vSideBand.size()-i+2);
		MapPnt_temp.p.y=tempy;
		MapPnt_temp.fx=meshPnt(idTemp).x;
		MapPnt_temp.fy=meshPnt(idTemp).y;
		MapPnt_temp.fz=meshPnt(idTemp).z;
		m_vBndMapPnt.push_back(MapPnt_temp);
		prex=MapPnt_temp.p.x;
	}

	
	for(j=0; j <m_vSideBand.size()-i; j++)
	{ 
		idTemp=m_vSideBand.at(i+j-1).end;
		MapPnt_temp.pointIndex=idTemp;
		MapPnt_temp.p.x=prex-tempPrex/(m_vSideBand.size()-i+2);
		MapPnt_temp.p.y=0.0;
		MapPnt_temp.fx=meshPnt(idTemp).x;
		MapPnt_temp.fy=meshPnt(idTemp).y;
		MapPnt_temp.fz=meshPnt(idTemp).z;
		m_vBndMapPnt.push_back(MapPnt_temp);
		prex=MapPnt_temp.p.x;
	}
 
	int edgeStart,edgeEnd;
	GEOIMG_EDGE tempEdge;
	int m,n,faceIndex,vexIndex;
	int tempSize;
	int t;


/*
	FILE *facefp;
	facefp=fopen("facedebug.txt","w+");
	for(i=0; i < m_nFaceSum+m_nAddFaces; i++)
		fprintf(facefp,"%d--%d  %d  %d\n",i,meshFaces(i).vI[0],meshFaces(i).vI[1],meshFaces(i).vI[2]);
	fclose(facefp);
*/

	int nTri;
	for(k=0; k < m_vEdge.size(); k++)
	{
		nTri=0;
	
		for(j=0; j < m_nFaceSum+m_nAddFaces; j++)
		{ 
			for(m = 0; m < 3; m++)
			{ 
				if((meshFaces(j).vI[m] == m_vEdge.at(k).start && meshFaces(j).vI[(m+1)%3] == m_vEdge.at(k).end)||
					(meshFaces(j).vI[m] == m_vEdge.at(k).end && meshFaces(j).vI[(m+1)%3] == m_vEdge.at(k).start))
				break;
			}
			if(m<3)
				m_vEdge.at(k).neighborTri[nTri++]=j;
		}
		for(j=nTri; j < MAXNEIGHBOR; j++)
			m_vEdge.at(k).neighborTri[j]=-1;
	}

 
	//如果一个三角形的三个顶点同时映射到同一边
	//则在非边界的一条边中引入一个点，将原三角形划分成两个三角形
	for(i=0; i < m_vBndMapPnt.size(); i++)
	{ 
		tempSize = m_vBndMapPnt.size()-1;

		p1 = m_vBndMapPnt.at(i).p;
		p2 = m_vBndMapPnt.at((i+1)%tempSize).p;
		p3 = m_vBndMapPnt.at((i+2)%tempSize).p;

		
		if(i==4)
			i=i;
		t1=m_vBndMapPnt.at(i).pointIndex;
		t2=m_vBndMapPnt.at((i+1)%tempSize).pointIndex;
		t3=m_vBndMapPnt.at((i+2)%tempSize).pointIndex;
		if((p1.x == p2.x&&p2.x==p3.x)||
			(p1.y == p2.y&&p2.y==p3.y))
		{ 
			for(t=0; t < m_nFaceSum+m_nAddFaces; t++)
			{ 
				if((t1==meshFaces(t).vI[0]&&t2==meshFaces(t).vI[1]&&t3==meshFaces(t).vI[2])||
					(t1==meshFaces(t).vI[1]&&t2==meshFaces(t).vI[2]&&t3==meshFaces(t).vI[0])||
					(t1==meshFaces(t).vI[2]&&t2==meshFaces(t).vI[0]&&t3==meshFaces(t).vI[1]))

				{ 
					edgeStart=t1;
					edgeEnd=t3;
					//插入一个点
					m_pAddPts[m_nAddPnt].x = ( meshPnt(edgeStart).x + meshPnt(edgeEnd).x)/2;
					m_pAddPts[m_nAddPnt].y = ( meshPnt(edgeStart).y + meshPnt(edgeEnd).y)/2;
					m_pAddPts[m_nAddPnt].z = ( meshPnt(edgeStart).z + meshPnt(edgeEnd).z)/2;

					//分裂三角形
					for(k=0 ; k < m_vEdge.size(); k++)
					{
						if((m_vEdge.at(k).start == edgeStart && m_vEdge.at(k).end == edgeEnd)
						|| (m_vEdge.at(k).start == edgeEnd && m_vEdge.at(k).end == edgeStart))
						break;
					}
	
					int node1,node2;

					for(n=0; n < MAXNEIGHBOR; n++)
					{
						faceIndex=m_vEdge.at(k).neighborTri[n];
 						if(faceIndex>-1)
						{
							//找出三角形的另外一个顶点vexIndex
							for(m= 0 ; m < 3 ; m++)
							{ 
								if(meshFaces(faceIndex).vI[m]!=edgeStart && meshFaces(faceIndex).vI[m]!=edgeEnd)
								{
									vexIndex = meshFaces(faceIndex).vI[m];
									break;
								}
							}
							node1 = meshFaces(faceIndex).vI[(m+1)%3];
							node2 = meshFaces(faceIndex).vI[(m+2)%3];

							//修改一个三角形信息
							if(faceIndex<m_nFaceSum)
							{
								m_pFaces[faceIndex].vI[0]=vexIndex;
								m_pFaces[faceIndex].vI[1]=m_nPtSum+m_nAddPnt;
								m_pFaces[faceIndex].vI[2]=node2;
							}
							else
							{ 
								m_pAddFaces[faceIndex-m_nFaceSum].vI[0]=vexIndex;
								m_pAddFaces[faceIndex-m_nFaceSum].vI[1]=m_nPtSum+m_nAddPnt;
								m_pAddFaces[faceIndex-m_nFaceSum].vI[2]=node2;
							}
							//另外再添加一个三角形
						 //	m_pAddFaces[m_nAddFaces] =(stFACE*)malloc(sizeof(stFACE));
							m_pAddFaces[m_nAddFaces].n=3;
							m_pAddFaces[m_nAddFaces].vI[0]=vexIndex;
							m_pAddFaces[m_nAddFaces].vI[1]=node1;
							m_pAddFaces[m_nAddFaces].vI[2]=m_nPtSum+m_nAddPnt;

							//插入新边
							tempEdge.start=vexIndex;
							tempEdge.end = m_nPtSum+m_nAddPnt;
							tempEdge.IsNull = FALSE;
							tempEdge.neighborTri[0]=faceIndex;
							tempEdge.neighborTri[1] = m_nAddFaces + m_nFaceSum;
							for(j=2; j < MAXNEIGHBOR; j++)
								tempEdge.neighborTri[j]=-1;
							m_vEdge.push_back(tempEdge);
							
							m_nAddFaces++;
						}
					}

					m_vEdge.at(k).start = edgeStart;
					m_vEdge.at(k).end = m_nPtSum +m_nAddPnt;
					m_vEdge.at(k).IsNull = FALSE;
					int nTri=0;

					for(n=0; n < m_nFaceSum+m_nAddFaces; n++)
					{ 
						for(m = 0; m < 3; m++)
						{ 
							if((meshFaces(n).vI[m] == m_vEdge.at(k).start && meshFaces(n).vI[(m+1)%3] == m_vEdge.at(k).end)||
								(meshFaces(n).vI[m] == m_vEdge.at(k).end && meshFaces(n).vI[(m+1)%3] == m_vEdge.at(k).start))
							break;
						}
						if(m<3)
							m_vEdge.at(k).neighborTri[nTri++]=n;
					}
					for(n=nTri; n < MAXNEIGHBOR; n++)
						m_vEdge.at(k).neighborTri[n]=-1;

					tempEdge.start = edgeEnd;
					tempEdge.end = m_nPtSum + m_nAddPnt;
					tempEdge.IsNull = FALSE;
					nTri = 0;

					for(j=0; j < m_nFaceSum+m_nAddFaces; j++)
					{ 
						for(m = 0; m < 3; m++)
						{ 
							if((meshFaces(j).vI[m] == tempEdge.start && meshFaces(j).vI[(m+1)%3] == tempEdge.end)||
								(meshFaces(j).vI[m] == tempEdge.end && meshFaces(j).vI[(m+1)%3] == tempEdge.start))
							break;
						}
						if(m < 3)
							tempEdge.neighborTri[nTri++]=j;
					}
					for(j=nTri; j < MAXNEIGHBOR; j++)
						m_vEdge.at(k).neighborTri[j]=-1;
					m_vEdge.push_back(tempEdge);

					m_nAddInPnt++;
					m_nAddPnt++;
					//////////////////////////////////////////////////////////////////////////
					//修改t1t2线段及t2t3线段的相邻三角形信息
					int nNeighborTri2=0;
   
					for(k=0; k < m_vEdge.size(); k++)
					{ 
						if((m_vEdge.at(k).start==t1&&m_vEdge.at(k).end==t2)||
							(m_vEdge.at(k).start==t2&&m_vEdge.at(k).end==t1))
							break;
					}
					for(j=0; j < MAXNEIGHBOR; j++)
						m_vEdge.at(k).neighborTri[j]=-1;

					for(n=0; n < m_nFaceSum+m_nAddFaces; n++)
					{ 
						for(m=0; m<3; m++)
						{ 
							if((meshFaces(n).vI[m]==t1&&meshFaces(n).vI[(m+1)%3]==t2)||
								(meshFaces(n).vI[m]==t2&&meshFaces(n).vI[(m+1)%3]==t1))
								break;
						}
						if(m<3)
						{ 
							m_vEdge.at(k).neighborTri[nNeighborTri2++]=n;
						}
					}
					////////////
					nNeighborTri2=0;
					for(k=0; k < m_vEdge.size(); k++)
					{ 
						if((m_vEdge.at(k).start==t3&&m_vEdge.at(k).end==t2)||
							(m_vEdge.at(k).start==t2&&m_vEdge.at(k).end==t3))
							break;
					}
					
					for(j=0; j < MAXNEIGHBOR; j++)
						m_vEdge.at(k).neighborTri[j]=-1;

					for(n=0; n < m_nFaceSum+m_nAddFaces; n++)
					{ 
						for(m=0; m<3; m++)
						{ 
							if((meshFaces(n).vI[m]==t3&&meshFaces(n).vI[(m+1)%3]==t2)||
								(meshFaces(n).vI[m]==t2&&meshFaces(n).vI[(m+1)%3]==t3))
								break;
						}
						if(m<3)
						{ 
							m_vEdge.at(k).neighborTri[nNeighborTri2++]=n;
						}
					}
				//////////////////////////////////////////////////////////////////////////
				}
			}
		}	
	}
	//////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////
	FILE *pointfp;
	pointfp=fopen("addpoint.txt","w+");
	for(i = 0; i<m_nAddPnt ;i++)
	{ 
		fprintf(pointfp,"%d----%lf  %lf  %lf\n",i,meshPnt(m_nPtSum+i).x,meshPnt(m_nPtSum+i).y,meshPnt(m_nPtSum+i).z);
	}
	fclose(pointfp);

	FILE* bfp;
	bfp=fopen("final_bound.txt","w+");
	for(i = 0; i<m_vBndMapPnt.size(); i++)
	{ 
		fprintf(bfp,"%d----%lf  %lf  \n",m_vBndMapPnt.at(i).pointIndex,m_vBndMapPnt.at(i).p.x,m_vBndMapPnt.at(i).p.y);
	}
	fclose(bfp);

 	FILE *fp;
	fp=fopen("final_sideband.txt","w+");
	int tStart, tEnd,tMate;
	tStart=m_vSideBand.at(0).start;
	tEnd = m_vSideBand.at(0).end;
	tMate = m_vSideBand.at(0).mate;
	
 	for(i = 0; i < m_vSideBand.size(); i++)
	{ 
		tStart=m_vSideBand.at(i).start;
		tEnd = m_vSideBand.at(i).end;
		tMate = m_vSideBand.at(i).mate;
		fprintf(fp,"%d----%d      %d\n",tStart, tEnd,tMate);
	}
	 fclose(fp);

	FILE *edgefp;
	edgefp=fopen("final_edge.txt","w+");
	for(i=0; i < m_vEdge.size(); i++)
		fprintf(edgefp,"%d  %d---%d  %d\n",m_vEdge.at(i).start,m_vEdge.at(i).end,m_vEdge.at(i).neighborTri[0],m_vEdge.at(i).neighborTri[1]);
	fclose(edgefp);

	FILE *cutfp2;
	cutfp2=fopen("final_cut.txt","w+");
	for(i=0; i < m_vCutPath.size(); i++)
	{ 
		fprintf(cutfp2,"%d---%d %d\n",i,m_vCutPath.at(i).start,m_vCutPath.at(i).end);
	}
	fclose(cutfp2);

		FILE *facefp2;
	facefp2=fopen("final_face.txt","w+");
	for(i=0; i < m_nFaceSum+m_nAddFaces; i++)
		fprintf(facefp2,"%d--%d  %d  %d\n",i,meshFaces(i).vI[0],meshFaces(i).vI[1],meshFaces(i).vI[2]);
	fclose(facefp2);

 }
//通过确定切线cutpath的拓扑结构来构造topological sideband
//暂时只处理genus为0的情况
void Mesh3D::CutUnwrap()
{
	int type;
	int i;
	SideBand tempSideBand;

	if(m_vCutPath.size()==0)
	{
		AfxMessageBox("the cutpath does not exist!");
		return;
	}
	if(m_vCutPath.at(0).start==m_vCutPath.at(1).start)
		type=1;
	else if(m_vCutPath.at(0).start==m_vCutPath.at(1).end)
		type =2;
	else if(m_vCutPath.at(0).end==m_vCutPath.at(1).start)
		type=3;
	else if(m_vCutPath.at(0).end==m_vCutPath.at(1).end)
		type=4;
	int vex1,vex2,vex3;

	switch(type) {
	case 1:
		{ 
			vex1= m_vCutPath.at(0).end;
			vex2 = m_vCutPath.at(0).start;
			vex3 = m_vCutPath.at(1).end;
		}
		break;
	case 2:
		{ 
			vex1=m_vCutPath.at(0).end;
			vex2=m_vCutPath.at(0).start;
			vex3=m_vCutPath.at(1).start;
		}
		break;
	case 3:
		{ 
			vex1=m_vCutPath.at(0).start;
			vex2=m_vCutPath.at(0).end;
			vex3=m_vCutPath.at(1).end;
		}
		break;
	case 4:
		{ 
			vex1=m_vCutPath.at(0).start;
			vex2=m_vCutPath.at(0).end;
			vex3=m_vCutPath.at(1).start;
		}
		break;
	default:
		break;
	}
	tempSideBand.start=vex1;
	tempSideBand.end = vex2;
	tempSideBand.length= m_vCutPath.at(0).length;
	tempSideBand.mate=3;
	m_vSideBand.push_back(tempSideBand);
			
	tempSideBand.start=vex2;
	tempSideBand.end = vex3;
	tempSideBand.length= m_vCutPath.at(1).length;
	tempSideBand.mate=2;
	m_vSideBand.push_back(tempSideBand);

	tempSideBand.start=vex3;
	tempSideBand.end = vex2;
	tempSideBand.length= m_vCutPath.at(1).length;
	tempSideBand.mate=1;
	m_vSideBand.push_back(tempSideBand);

	tempSideBand.start=vex2;
	tempSideBand.end = vex1;
	tempSideBand.length= m_vCutPath.at(0).length;
	tempSideBand.mate=0;
	m_vSideBand.push_back(tempSideBand);

	
	FILE *fp;
	fp=fopen("sidebanddebug.txt","w+");
	int tStart, tEnd,tMate;
	tStart=m_vSideBand.at(0).start;
	tEnd = m_vSideBand.at(0).end;
	tMate = m_vSideBand.at(0).mate;

 	for(i = 0; i < m_vSideBand.size(); i++)
	{ 
		tStart=m_vSideBand.at(i).start;
		tEnd = m_vSideBand.at(i).end;
		tMate = m_vSideBand.at(i).mate;
		fprintf(fp,"%d----%d      %d\n",tStart, tEnd,tMate);
	}
	fclose(fp);
}


//处理边参数化遇到的特殊问题，添加一个新的顶点，并将原来的三角形切分为两个三角形
void Mesh3D::BreakEdge(int *dir, double dist ,double tempLen , double ScaleCoff, int edgeIndex)
{
	//超过边界长度，添加一个新点
	SideBand tempSideBand;
	MapPoint MapPnt_temp;
	int mateTemp;
	int idTemp;
	int i;
	i=edgeIndex;
		
	int j;
	int k;
	int m,n;

	int edgeStart , edgeEnd;
	int faceIndex;
	int vexIndex;
	//在插入线段之前记录下要被分裂的线段的起点和终点
 	edgeStart = m_vSideBand.at(edgeIndex).start;
 	edgeEnd = m_vSideBand.at(edgeIndex).end;


	vector<SideBand>::iterator insertPos;
	vector<MapPoint>::iterator insertP;
			
			
	mateTemp=m_vSideBand.at(i).mate;

	//////////////////////////////////////////////////////////////////////////
	//由于需要插入两个线段，所以需要修改其他线段的mate线段的信息
	if(mateTemp > i)
	{
		for(j=0; j < m_vSideBand.size(); j++)
		{
			if(m_vSideBand.at(j).mate < i) 
				m_vSideBand.at(j).mate+=0;
			else if(m_vSideBand.at(j).mate > i && m_vSideBand.at(j).mate < mateTemp)
				m_vSideBand.at(j).mate+=1;
			else if(m_vSideBand.at(j).mate > mateTemp)
				m_vSideBand.at(j).mate+=2;
		}

		//在第i个位置插入一个新线段
		tempSideBand.start = m_vSideBand.at(i).start;
		tempSideBand.end = m_nPtSum + m_nAddPnt; ////
		tempSideBand.length=(dist-(tempLen-1))/ScaleCoff;
		//tempSideBand.length=lf2sf(tempSideBand.length);
		tempSideBand.mate=m_vSideBand.at(i).mate+2;
		insertPos=m_vSideBand.begin()+i;
		m_vSideBand.insert(insertPos,tempSideBand); 

		// 在mate的相应位置也插入同样的线段
		tempSideBand.mate=i;
		tempSideBand.start = m_nPtSum + m_nAddPnt;//
		tempSideBand.end = m_vSideBand.at(i).start;
		insertPos=m_vSideBand.begin()+mateTemp+2;
		m_vSideBand.insert(insertPos,tempSideBand);
		//修改原本线段的信息
		m_vSideBand.at(i+1).start= m_nPtSum + m_nAddPnt;/////
		m_vSideBand.at(i+1).length = ((tempLen-1)/ScaleCoff);
		m_vSideBand.at(i+1).mate= mateTemp+1;

		m_vSideBand.at(mateTemp+1).end= m_nPtSum + m_nAddPnt;//
		m_vSideBand.at(mateTemp+1).length = ((tempLen-1)/ScaleCoff);
		m_vSideBand.at(mateTemp+1).mate=i+1;
	}
	//////////////////////////////////////////////////////////////////////////
	else if(mateTemp < i)
	{ 
		for(j = 0; j <m_vSideBand.size(); j++)
		{
			if(m_vSideBand.at(j).mate < mateTemp )
				m_vSideBand.at(j).mate+=0;
			if(m_vSideBand.at(j).mate < i && m_vSideBand.at(j).mate > mateTemp)
				m_vSideBand.at(j).mate+=1;
			else if(m_vSideBand.at(j).mate > i)
				m_vSideBand.at(j).mate+=2;
		}
		//在第i个位置插入一个新线段
		tempSideBand.start = edgeStart;
		tempSideBand.end = m_nPtSum + m_nAddPnt; ////
		tempSideBand.length=(dist-(tempLen-1))/ScaleCoff;
		//tempSideBand.length=lf2sf(tempSideBand.length);
		tempSideBand.mate=m_vSideBand.at(i).mate+1;
		insertPos=m_vSideBand.begin()+i;
		m_vSideBand.insert(insertPos,tempSideBand); 

		// 在mate的相应位置也插入同样的线段
		tempSideBand.mate=i+1;
		tempSideBand.start = m_nPtSum + m_nAddPnt;//
		tempSideBand.end = edgeStart;
		insertPos=m_vSideBand.begin()+mateTemp+1;
		m_vSideBand.insert(insertPos,tempSideBand);
		//修改原本线段的信息
		m_vSideBand.at(i+2).start= m_nPtSum + m_nAddPnt;/////
		m_vSideBand.at(i+2).length = ((tempLen-1)/ScaleCoff);
		m_vSideBand.at(i+2).mate= mateTemp;

		m_vSideBand.at(mateTemp).end= m_nPtSum + m_nAddPnt;//
		m_vSideBand.at(mateTemp).length = ((tempLen-1)/ScaleCoff);
		m_vSideBand.at(mateTemp).mate=i+2;
	}
	
	
	
	

	//////////////////////////////////////////////////////////////////////////
	//添加新的顶点后，需要修改mesh结构，添加第m_nPtSum+m_nAddPnt个点
 	//需要做的工作：在顶点数组里添加新点（如何确定新添加的三角形的3D位置信息？？？）
	//              将原三角形分裂成两个三角形 （8.28）
 
	
	//插入新点
	//新顶点的3D按比例分配
	m_pAddPts[m_nAddPnt].x = ((dist - tempLen+1) * meshPnt(edgeStart).x + (tempLen-1)*meshPnt(edgeEnd).x)/dist;
	m_pAddPts[m_nAddPnt].y = ((dist - tempLen+1) * meshPnt(edgeStart).y + (tempLen-1)*meshPnt(edgeEnd).y)/dist;
	m_pAddPts[m_nAddPnt].z = ((dist - tempLen+1) * meshPnt(edgeStart).z + (tempLen-1)*meshPnt(edgeEnd).z)/dist;
	

	//插入新边
	GEOIMG_EDGE tempEdge;

	//分裂三角形
	for(k=0 ; k < m_vEdge.size(); k++)
	{
		if((m_vEdge.at(k).start == edgeStart && m_vEdge.at(k).end == edgeEnd)
			|| (m_vEdge.at(k).start == edgeEnd && m_vEdge.at(k).end == edgeStart))
			break;
	}
 	ASSERT(k < m_vEdge.size());

	int node1,node2;
	int nTri;

	for(n=0; n < MAXNEIGHBOR; n++)
	{
		faceIndex=m_vEdge.at(k).neighborTri[n];
		if(faceIndex>-1)
		{
			//找出三角形的另外一个顶点vexIndex
			for(m= 0 ; m < 3 ; m++)
			{ 
				if(meshFaces(faceIndex).vI[m]!=edgeStart && meshFaces(faceIndex).vI[m]!=edgeEnd)
				{
					vexIndex = meshFaces(faceIndex).vI[m];
					break;
				}
			}
			node1 = meshFaces(faceIndex).vI[(m+1)%3];
			node2 = meshFaces(faceIndex).vI[(m+2)%3];
			//修改一个三角形信息
			if(faceIndex<m_nFaceSum)
			{
				m_pFaces[faceIndex].vI[0]=vexIndex;
				m_pFaces[faceIndex].vI[1]=m_nPtSum+m_nAddPnt;
				m_pFaces[faceIndex].vI[2]=node2;
			}
			else
			{ 
				m_pAddFaces[faceIndex-m_nFaceSum].vI[0]=vexIndex;
				m_pAddFaces[faceIndex-m_nFaceSum].vI[1]=m_nPtSum+m_nAddPnt;
				m_pAddFaces[faceIndex-m_nFaceSum].vI[2]=node2;
			}
			//另外再添加一个三角形
 			m_pAddFaces[m_nAddFaces].n=3;
			m_pAddFaces[m_nAddFaces].vI[0]=vexIndex;
			m_pAddFaces[m_nAddFaces].vI[1]=node1;
			m_pAddFaces[m_nAddFaces].vI[2]=m_nPtSum+m_nAddPnt;

			//插入新边
			tempEdge.start=vexIndex;
			tempEdge.end = m_nPtSum+m_nAddPnt;
			tempEdge.IsNull = FALSE;
			nTri=0;
			tempEdge.neighborTri[0]=faceIndex;
			tempEdge.neighborTri[1] = m_nAddFaces + m_nFaceSum;
			for(j=2; j < MAXNEIGHBOR; j++)
				tempEdge.neighborTri[j]=-1;
			m_vEdge.push_back(tempEdge);

			m_nAddFaces++;
		}
	}

	m_vEdge.at(k).start = edgeStart;
	m_vEdge.at(k).end = m_nPtSum +m_nAddPnt;
	m_vEdge.at(k).IsNull = FALSE;
    nTri=0;

	for(j=0; j < m_nFaceSum+m_nAddFaces; j++)
	{ 
		for(m = 0; m < 3; m++)
		{ 
			if((meshFaces(j).vI[m] == m_vEdge.at(k).start && meshFaces(j).vI[(m+1)%3] == m_vEdge.at(k).end)||
				(meshFaces(j).vI[m] == m_vEdge.at(k).end && meshFaces(j).vI[(m+1)%3] == m_vEdge.at(k).start))
			break;
		}
		if(m<3)
			m_vEdge.at(k).neighborTri[nTri++]=j;
	}
	for(j=nTri; j < MAXNEIGHBOR; j++)
		m_vEdge.at(k).neighborTri[j]=-1;

	tempEdge.start = edgeEnd;
	tempEdge.end = m_nPtSum + m_nAddPnt;
	tempEdge.IsNull = FALSE;
	nTri = 0;

	for(j=0; j < m_nFaceSum+m_nAddFaces; j++)
	{ 
		for(m = 0; m < 3; m++)
		{ 
			if((meshFaces(j).vI[m] == tempEdge.start && meshFaces(j).vI[(m+1)%3] == tempEdge.end)||
				(meshFaces(j).vI[m] == tempEdge.end && meshFaces(j).vI[(m+1)%3] == tempEdge.start))
			break;
		}
		if(m < 3)
			tempEdge.neighborTri[nTri++]=j;
	}
	for(j=nTri; j < MAXNEIGHBOR; j++)
		m_vEdge.at(k).neighborTri[j]=-1;
	m_vEdge.push_back(tempEdge);
	
	m_nAddPnt++;

	//对cutpath进行相应的分裂
	for(k=0; k < m_vCutPath.size(); k++)
	{ 
		if((m_vCutPath.at(k).start==edgeStart&&m_vCutPath.at(k).end==edgeEnd)||
			(m_vCutPath.at(k).start==edgeEnd&&m_vCutPath.at(k).end==edgeStart))
			break;
	}
	EDGE_ edge_Temp;
	edge_Temp.start=m_nPtSum+m_nAddPnt-1;
	edge_Temp.end=m_vCutPath.at(k).end;
	edge_Temp.bIsBound=FALSE;
	edge_Temp.P[0] = meshPnt(edge_Temp.start);
	edge_Temp.P[1] = meshPnt(edge_Temp.end);
	edge_Temp.length = sqrt((edge_Temp.P[0].x-edge_Temp.P[1].x)*(edge_Temp.P[0].x-edge_Temp.P[1].x)
							+(edge_Temp.P[0].y-edge_Temp.P[1].y)*(edge_Temp.P[0].y-edge_Temp.P[1].y)
							+(edge_Temp.P[0].z-edge_Temp.P[1].z)*(edge_Temp.P[0].z-edge_Temp.P[1].z)); 
	m_vCutPath.push_back(edge_Temp);

	m_vCutPath.at(k).end=m_nPtSum+m_nAddPnt-1;
	m_vCutPath.at(k).start=m_vCutPath.at(k).start;
	m_vCutPath.at(k).bIsBound=FALSE;
	m_vCutPath.at(k).P[0]=m_vCutPath.at(k).P[0];
	m_vCutPath.at(k).P[1]=meshPnt(m_nPtSum+m_nAddPnt-1);
	m_vCutPath.at(k).length = sqrt((m_vCutPath.at(k).P[0].x-m_vCutPath.at(k).P[1].x)*(m_vCutPath.at(k).P[0].x-m_vCutPath.at(k).P[1].x)
							+(m_vCutPath.at(k).P[0].y-m_vCutPath.at(k).P[1].y)*(m_vCutPath.at(k).P[0].y-m_vCutPath.at(k).P[1].y)
							+(m_vCutPath.at(k).P[0].z-m_vCutPath.at(k).P[1].z)*(m_vCutPath.at(k).P[0].z-m_vCutPath.at(k).P[1].z)); 

	////////////////////////////////////
	////////////////////////////////////
	//记录下映射点的信息  
	//第一个点（插入点）
	if(mateTemp > i)
	{
		idTemp=m_vSideBand.at(i+1).start;
		MapPnt_temp.pointIndex=idTemp;
		switch(*dir) {
		case 1:
			{
				MapPnt_temp.p.x=0.0;
				MapPnt_temp.p.y=1.0;
				*dir = 2;
			}
			break;
		case 2:
			{
				MapPnt_temp.p.x=1.0;
				MapPnt_temp.p.y=1.0;
				*dir = 3;
			}
			break;
		case 3:
			{
				MapPnt_temp.p.x=1.0;
				MapPnt_temp.p.y=0.0;
				*dir =4; 
			}
			break;
 		default:
			break;
		}
		MapPnt_temp.fx=meshPnt(idTemp).x;
		MapPnt_temp.fy=meshPnt(idTemp).y;
		MapPnt_temp.fz=meshPnt(idTemp).z;
		m_vBndMapPnt.push_back(MapPnt_temp);
	}
	else
	{ 
		int tempSize;
		int tempDir;
		tempSize=m_vBndMapPnt.size();
		idTemp=m_nPtSum+m_nAddPnt-1;
		MapPnt_temp.pointIndex=idTemp;
		//////////////////////////////////////////////////////////////////////////
		if(m_vBndMapPnt.at(mateTemp).p.x==0 && m_vBndMapPnt.at(mateTemp).p.y < 1.0)
			tempDir=1;
		else if(m_vBndMapPnt.at(mateTemp).p.y==1&&m_vBndMapPnt.at(mateTemp).p.x <1.0)
			tempDir=2;
		else if(m_vBndMapPnt.at(mateTemp).p.x==1&&m_vBndMapPnt.at(mateTemp).p.y > 0.0)
			tempDir=3;
		else	if(m_vBndMapPnt.at(mateTemp).p.y==0)
			tempDir=4;
		switch(tempDir) {
		case 1:
			{
				MapPnt_temp.p.x=0.0;
				MapPnt_temp.p.y=m_vBndMapPnt.at(mateTemp+1).p.y - (dist-(tempLen-1));
			}
			break;
		case 2:
			{
				MapPnt_temp.p.x=m_vBndMapPnt.at(mateTemp+1).p.x-(dist-(tempLen-1));
				MapPnt_temp.p.y=1.0;
			}
			break;
		case 3:
			{
				MapPnt_temp.p.x=1.0;
				MapPnt_temp.p.y=m_vBndMapPnt.at(mateTemp+1).p.y+(dist-(tempLen-1));
			}
			break;
 		default:
			break;
		}
		MapPnt_temp.fx=meshPnt(idTemp).x;
		MapPnt_temp.fy=meshPnt(idTemp).y;
		MapPnt_temp.fz=meshPnt(idTemp).z;
		insertP=m_vBndMapPnt.begin()+mateTemp+1;
		m_vBndMapPnt.insert(insertP,MapPnt_temp);
	}
}


//当要分裂的是边界边的时候，由于sideband中边界边只走了一遍，所以不需要分裂其对应边，处理起来相应简单些
void Mesh3D::BreakEdge1(int *dir, double dist, double tempLen, double ScaleCoff, int edgeIndex)
{
//超过边界长度，添加一个新点
	SideBand tempSideBand;
	MapPoint MapPnt_temp;
	GEOIMG_EDGE tempEdge;
	int idTemp;
	int i;
	i=edgeIndex;
		
	int j;
	int k;
	int m,n;

	vector<SideBand>::iterator insertPos;
			
 	//////////////////////////////////////////////////////////////////////////
	//由于需要插入两个线段，所以需要修改其他线段的mate线段的信息
	for(j=0; j < m_vSideBand.size(); j++)
	{
		if(m_vSideBand.at(j).mate < i) 
			m_vSideBand.at(j).mate+=0;
		else if(m_vSideBand.at(j).mate > i)
			m_vSideBand.at(j).mate+=1;
	}
	
	int edgeStart , edgeEnd;
	int faceIndex;
	int vexIndex;
	int node1,node2;
	//在插入线段之前记录下要被分裂的线段的起点和终点
 	edgeStart = m_vSideBand.at(edgeIndex).start;
 	edgeEnd = m_vSideBand.at(edgeIndex).end;
	
	//在第i个位置插入一个新线段
	tempSideBand.start = m_vSideBand.at(i).start;
	tempSideBand.end = m_nPtSum + m_nAddPnt; ////
	tempSideBand.length=(dist-(tempLen-1))/ScaleCoff;
	tempSideBand.mate=-1;
	insertPos=m_vSideBand.begin()+i;
	m_vSideBand.insert(insertPos,tempSideBand); 

	//修改原本线段的信息
	m_vSideBand.at(i+1).start= m_nPtSum + m_nAddPnt;/////
	m_vSideBand.at(i+1).length = ((tempLen-1)/ScaleCoff);
	m_vSideBand.at(i+1).mate= -1;
	//////////////////////////////////////////////////////////////////////////
	//添加新的顶点后，需要修改mesh结构，添加第m_nPtSum+m_nAddPnt个点
 	//需要做的工作：在顶点数组里添加新点（如何确定新添加的三角形的3D位置信息？？？）
	//              将原三角形分裂成两个三角形 （8.28）
 
	//插入新点
	//	m_pAddPts[m_nAddPnt]=(POINT3D *) malloc(sizeof(POINT3D));
	//新顶点的3D信息取原边的两个顶点的中点
	m_pAddPts[m_nAddPnt].x = ((dist - tempLen+1) * meshPnt(edgeStart).x + (tempLen-1)*meshPnt(edgeEnd).x)/dist;
	m_pAddPts[m_nAddPnt].y = ((dist - tempLen+1) * meshPnt(edgeStart).y + (tempLen-1)*meshPnt(edgeEnd).y)/dist;
	m_pAddPts[m_nAddPnt].z = ((dist - tempLen+1) * meshPnt(edgeStart).z + (tempLen-1)*meshPnt(edgeEnd).z)/dist;

	//分裂三角形
	for(k=0 ; k < m_vEdge.size(); k++)
	{
		if((m_vEdge.at(k).start == edgeStart && m_vEdge.at(k).end == edgeEnd)
			|| (m_vEdge.at(k).start == edgeEnd && m_vEdge.at(k).end == edgeStart))
			break;
	}
	//如果没找到怎么办？？？？？应该处理一下
    if(k == m_vEdge.size())
	{ 
		AfxMessageBox("ERROR!");
		return;
	}
	for(n=0; n < MAXNEIGHBOR; n++)
	{
		faceIndex=m_vEdge.at(k).neighborTri[n];
		if(faceIndex!=-1)
		{
			//找出三角形的另外一个顶点vexIndex
			for(m= 0 ; m < 3 ; m++)
			{ 
				if(meshFaces(faceIndex).vI[m]!=edgeStart && meshFaces(faceIndex).vI[m]!=edgeEnd)
				{
					vexIndex = meshFaces(faceIndex).vI[m];
					break;
				}
			}
			node1 = meshFaces(faceIndex).vI[(m+1)%3];
			node2 = meshFaces(faceIndex).vI[(m+2)%3];
			//修改一个三角形信息
			m_pFaces[faceIndex].vI[0]=vexIndex;
			m_pFaces[faceIndex].vI[1]=m_nPtSum+m_nAddPnt;
			m_pFaces[faceIndex].vI[2]=node2;
			//另外再添加一个三角形
		//	m_pAddFaces[m_nAddFaces] =(stFACE*)malloc(sizeof(stFACE));
			m_pAddFaces[m_nAddFaces].n=3;
			m_pAddFaces[m_nAddFaces].vI[0]=vexIndex;
			m_pAddFaces[m_nAddFaces].vI[1]=node1;
			m_pAddFaces[m_nAddFaces].vI[2]=m_nPtSum+m_nAddPnt;

			//插入新边
			tempEdge.start=vexIndex;
			tempEdge.end = m_nPtSum+m_nAddPnt;
			tempEdge.IsNull = FALSE;
			tempEdge.neighborTri[0]=faceIndex;
			tempEdge.neighborTri[1] = m_nAddFaces + m_nFaceSum;
			m_vEdge.push_back(tempEdge);

			m_nAddFaces++;
		}
	}

	m_vEdge.at(k).start = edgeStart;
	m_vEdge.at(k).end = m_nPtSum +m_nAddPnt;
	m_vEdge.at(k).IsNull = FALSE;
	int nTri=0;

	for(j=0; j < m_nFaceSum+m_nAddFaces; j++)
	{ 
		for(m = 0; m < 3; m++)
		{ 
			if((meshFaces(j).vI[m] == m_vEdge.at(k).start && meshFaces(j).vI[(m+1)%3] == m_vEdge.at(k).end)||
				(meshFaces(j).vI[m] == m_vEdge.at(k).end && meshFaces(j).vI[(m+1)%3] == m_vEdge.at(k).start))
			break;
		}
		if(m<3)
			m_vEdge.at(k).neighborTri[nTri++]=j;
	}

	tempEdge.start = edgeEnd;
	tempEdge.end = m_nPtSum + m_nAddPnt;
	tempEdge.IsNull = FALSE;
	nTri = 0;

	for(j=0; j < m_nFaceSum+m_nAddFaces; j++)
	{ 
		for(m = 0; m < 3; m++)
		{ 
			if((meshFaces(j).vI[m] == tempEdge.start && meshFaces(j).vI[(m+1)%3] == tempEdge.end)||
				(meshFaces(j).vI[m] == tempEdge.end && meshFaces(j).vI[(m+1)%3] == tempEdge.start))
			break;
		}
		if(m < 3)
			tempEdge.neighborTri[nTri++]=j;
	}
	m_vEdge.push_back(tempEdge);
	
	m_nAddPnt++;
	////////////////////////////////////
	////////////////////////////////////
	//记录下映射点的信息  
	//第一个点（插入点）8.27
	idTemp=m_vSideBand.at(i+1).start;
	MapPnt_temp.pointIndex=idTemp;
	switch(*dir) {
	case 1:
		{
			MapPnt_temp.p.x=0.0;
			MapPnt_temp.p.y=1.0;
			*dir = 2;
		}
		break;
	case 2:
		{
			MapPnt_temp.p.x=1.0;
			MapPnt_temp.p.y=1.0;
			*dir = 3;
		}
		break;
	case 3:
		{
			MapPnt_temp.p.x=1.0;
			MapPnt_temp.p.y=0.0;
			*dir =4; 
		}
		break;
 	default:
		break;
	}
	
	MapPnt_temp.fx=meshPnt(idTemp).x;
	MapPnt_temp.fy=meshPnt(idTemp).y;
	MapPnt_temp.fz=meshPnt(idTemp).z;
	m_vBndMapPnt.push_back(MapPnt_temp);
}

void Mesh3D::DisplayCircleBndMap()
{
	int i;
	double tempX,tempY;
	const double pi=3.1415926;

/*
	vector<MapPoint>::iterator  index;
	index=m_vBndMapPnt.begin();
*/
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for(i=0; i < 360; i++)
	{ 
		tempX = cos((double)(i*pi)/180);
		tempY = sin((double)(i*pi)/180);
		glVertex2f(tempX,tempY);
	}
	glEnd();

	glColor3f(1.0, 0.0, 1.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	for(i=0; i < m_vBndMapPnt.size(); i++)
			glVertex2f(m_vBndMapPnt.at(i).p.x,m_vBndMapPnt.at(i).p.y);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	for(i=0; i < m_vBndMapPnt.size(); i++)
			glVertex2f(m_vBndMapPnt.at(i).p.x,m_vBndMapPnt.at(i).p.y);
 	glEnd();

	glLineWidth(1.0);
	glPointSize(1.0);
}




//显示边界参数化后的结果
void Mesh3D::DisplayBoundaryMap()
{
	int i;
	vector<MapPoint>::iterator  index;
	index=m_vBndMapPnt.begin();
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, 0.0f);
	glEnd();

	glColor3f(1.0, 0.0, 1.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	for(i=0; i < m_vBndMapPnt.size(); i++)
			glVertex2f(m_vBndMapPnt.at(i).p.x,m_vBndMapPnt.at(i).p.y);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	for(i=0; i < m_vBndMapPnt.size(); i++)
			glVertex2f(m_vBndMapPnt.at(i).p.x,m_vBndMapPnt.at(i).p.y);
	glVertex2f(0.0,0.0);
 	glEnd();

	glLineWidth(1.0);
	glPointSize(1.0);
}


double Mesh3D::lf2sf(double lf)
{
	double t1;
	int t2;
	double diff;
	double sf;

	t1=lf * NGRID;
	t2=int(lf * NGRID);
	diff = t1-t2;
	if(diff < 0.5)
		sf = t2 * (1/(double)NGRID);
	else 
		sf = (t2+1) * (1/(double)NGRID);
	return(sf);
}




void Mesh3D::CutUnwrap2()
{ 
	SideBand tempSideBand;
 	vector<EDGE_> m_vDirCut;

	int i,j, k, m,n; 
	int nJointCutCnt;
	int JointCuts[MAXJOINTCUTS];
	int tempCutIndex;
	int tempEdgeIndex;
	int tempStart, tempEnd;
	int nextEnd;
	int preStart;
	int nextEdgeIndex;
	bool flag;
	int neighborTri1, neighborTri2;
	int start1,start2,choose;
	int type;
	int *nVisited;
	int nSideBand;

	if(m_vCutPath.size()==0)
	{
		AfxMessageBox("the cutpath does not exist!");
		return;
	}
	
	m_vDirCut.clear();
	m_vSideBand.clear();

	m_vDirCut = m_vCutPath;
	
	//计算sideband的总数，如果是边界只走一遍，而如果是非边界则走2遍
	nSideBand=0;
	for(i=0; i < m_vDirCut.size(); i++)
	{ 
		if(m_vDirCut.at(i).bIsBound)
			nSideBand+=1;
		else
			nSideBand+=2;
	}

	nVisited = (int*)malloc(m_vDirCut.size()* sizeof(int)) ;
	for(i=0; i < m_vDirCut.size(); i++)
		nVisited[i]=0;

	tempSideBand.start = m_vDirCut.at(0).start;
	tempSideBand.end = m_vDirCut.at(0).end;
	tempSideBand.length = m_vDirCut.at(0).length;
	tempSideBand.mate = -1;
	m_vSideBand.push_back(tempSideBand);
	nVisited[0]++;

	tempCutIndex = 0;

	for (i = 1; i < nSideBand; i++)
	{ 
		tempStart = tempSideBand.start;
		tempEnd = tempSideBand.end;
		for(k=0; k < MAXJOINTCUTS; k++)
			JointCuts[k]=-1;
		nJointCutCnt = 0;
		//找到与该边的终点相连接的切线边
		for(j=0; j < m_vDirCut.size(); j++)
		{ 
			if((j!=tempCutIndex)&&
				(m_vDirCut.at(j).start==tempEnd||m_vDirCut.at(j).end==tempEnd))
			{ 
				JointCuts[nJointCutCnt] = j;
				nJointCutCnt++;
				if(m_vDirCut.at(j).start==tempEnd)
					type=1; //start
				else
					type=2; //end
			}
		}

		if(nJointCutCnt==1) //如果只有一个切线边与该切线相连
		{
			switch(type) {
			case 1:
				{ 
					tempSideBand.start = m_vDirCut.at(JointCuts[0]).start;
					tempSideBand.end = m_vDirCut.at(JointCuts[0]).end;
					tempSideBand.length = m_vDirCut.at(JointCuts[0]).length;
					tempSideBand.mate = -1;
					m_vSideBand.push_back(tempSideBand);
					nVisited[JointCuts[0]]++;
				}
				break;
			case 2:
				{ 
					tempSideBand.start = m_vDirCut.at(JointCuts[0]).end;
					tempSideBand.end = m_vDirCut.at(JointCuts[0]).start;
					tempSideBand.length = m_vDirCut.at(JointCuts[0]).length;
					tempSideBand.mate = -1;
					m_vSideBand.push_back(tempSideBand);
					nVisited[JointCuts[0]]++;
				}
				break;
			default:
				break;
			}
			tempCutIndex = JointCuts[0];
		}
		else if(nJointCutCnt==0)//如果到达端点,则按原路返回
		{ 
			tempSideBand.start = tempEnd;
			tempSideBand.end = tempStart;
			tempSideBand.length = m_vDirCut.at(tempCutIndex).length;
			tempSideBand.mate = -1;
			m_vSideBand.push_back(tempSideBand);
			nVisited[tempCutIndex]++;
			tempCutIndex=tempCutIndex;
		}
		else//如果有多条切线与当前切线的终点相连
		{
			//找出当前切线在列表中的序列号，从而可以找到与之它连接的三角形
			for(j = 0 ; j < m_vEdge.size(); j++)
			{ 
				if((m_vEdge.at(j).start == tempEnd && m_vEdge.at(j).end == tempStart)||
					(m_vEdge.at(j).end == tempEnd && m_vEdge.at(j).start == tempStart))
				    tempEdgeIndex = j;
			}
			neighborTri1 = m_vEdge.at(tempEdgeIndex).neighborTri[0];
			neighborTri2 = m_vEdge.at(tempEdgeIndex).neighborTri[1];

			for(m=0; m < 3 ; m++)
			{ 
				if(meshFaces(neighborTri1).vI[m] == tempStart) 
				{ 
					start1 = m;
					break;
				}
			}
			for(m=0; m < 3 ; m++)
			{ 
				if(meshFaces(neighborTri2).vI[m] == tempStart) 
				{
					start2 = m;
					break;
				}
			}
			//选定一个边方向与切线走向相同的三角形
			if(meshFaces(neighborTri1).vI[(start1+1)%3] == tempEnd)
			{
				 choose = 1;
				 nextEnd = meshFaces(neighborTri1).vI[(start1+2)%3];
			}
			else
			{
				 choose = 2;
				 nextEnd = meshFaces(neighborTri2).vI[(start2+2)%3];
			}
			
			flag =TRUE;
			preStart=tempStart;
			while(flag) //知道找到一个确定的方向后停止
			{
				for(n = 0; n < nJointCutCnt; n++)
				{ 
					//如果选的下一条边正好是切线
					if((m_vDirCut.at(JointCuts[n]).start==nextEnd||
					 	m_vDirCut.at(JointCuts[n]).end==nextEnd) )
					{ 
						//如果是切线，还需要判断这条切线是否还能再走
						//判断是否还能再走：
						// 如果是边界，则只能走一遍，如果是非边界，则可以走两遍
						if((m_vDirCut.at(JointCuts[n]).bIsBound==TRUE && nVisited[JointCuts[n]]<1)
							||(m_vDirCut.at(JointCuts[n]).bIsBound==FALSE && nVisited[JointCuts[n]]<2))
						{
							if(m_vDirCut.at(JointCuts[n]).start==nextEnd)
							{
								tempSideBand.start = m_vDirCut.at(JointCuts[n]).end;
								tempSideBand.end = m_vDirCut.at(JointCuts[n]).start;
							}
							else
							{ 
								tempSideBand.start = m_vDirCut.at(JointCuts[n]).start;
								tempSideBand.end = m_vDirCut.at(JointCuts[n]).end;
							}
							tempSideBand.length = m_vDirCut.at(JointCuts[n]).length;
							tempSideBand.mate = -1;
							m_vSideBand.push_back(tempSideBand);
							nVisited[JointCuts[n]]++;
							flag = FALSE;
							tempCutIndex = JointCuts[n];
							break;
						}
					}
				}
				//如果选的下一条边不是切线，则应该继续往下搜索
				if(n==nJointCutCnt)
				{
					for(j = 0 ; j < m_vEdge.size(); j++)
					{ 
						if((m_vEdge.at(j).start == tempEnd && m_vEdge.at(j).end == nextEnd)||
							(m_vEdge.at(j).end == tempEnd && m_vEdge.at(j).start == nextEnd))
						nextEdgeIndex = j;
					}
					neighborTri1 = m_vEdge.at(nextEdgeIndex).neighborTri[0];
					neighborTri2 = m_vEdge.at(nextEdgeIndex).neighborTri[1];
					int end1, end2;
					for(m=0; m < 3; m++)
					{
						if(meshFaces(neighborTri1).vI[m]!=tempEnd &&
							meshFaces(neighborTri1).vI[m]!=nextEnd)
							break;
					}
					end1 = meshFaces(neighborTri1).vI[m];
					for(m=0; m < 3; m++)
					{
						if(meshFaces(neighborTri2).vI[m]!=tempEnd &&
							meshFaces(neighborTri2).vI[m]!=nextEnd)
							break;
					}
					end2 = meshFaces(neighborTri2).vI[m];
					if(end1 == preStart)
					{ 
						preStart = nextEnd;
						nextEnd = end2;
					}
					else
					{ 
						preStart = nextEnd;
						nextEnd = end1;
					}
				}
			}
			
		}			
	}
	
	//找到各条sideband的匹配sideband
	for(i=0; i < m_vSideBand.size(); i++)
	{ 
		if(m_vSideBand.at(i).mate==-1)
		{
			for(j=i+1; j < m_vSideBand.size(); j++)
			{
				if(m_vSideBand.at(j).end == m_vSideBand.at(i).start &&
					m_vSideBand.at(j).start == m_vSideBand.at(i).end)
				{ 
					m_vSideBand.at(i).mate = j;
					m_vSideBand.at(j).mate = i;
					break;
				}
			}
		}
	}

/*
	FILE *fp;
	fp=fopen("sidebandafterunwrap.txt","w+");
	int tStart, tEnd,tMate;
	tStart=m_vSideBand.at(0).start;
	tEnd = m_vSideBand.at(0).end;
	tMate = m_vSideBand.at(0).mate;

	for(i = 0; i < m_vSideBand.size(); i++)
	{ 
		tStart=m_vSideBand.at(i).start;
		tEnd = m_vSideBand.at(i).end;
		tMate = m_vSideBand.at(i).mate;
		fprintf(fp,"%d----%d      %d\n",tStart, tEnd,tMate);
	}
	fclose(fp);
*/
	m_vDirCut.clear();
}

 
void mult(double *,double *,double *,int,int,int);
int  invers_matrix(double *,int);

void Mesh3D::FloaterParam(int nFloaterType)
{
	int i ,j,k;
	bool *bIsBound;
	double **w;
	int n,N;
	int *inPt, *outPt;
	double *u,*v;

/*
	FILE* bfp;
	bfp=fopen("bound0.txt","w+");
	for(i = 0; i<m_vBndMapPnt.size(); i++)
	{ 
		fprintf(bfp,"%d----  %lf  %lf  \n",i,m_vBndMapPnt.at(i).p.x,m_vBndMapPnt.at(i).p.y);
	}
	fclose(bfp);
*/

	if(m_vBndMapPnt.size()==0)
	{
		AfxMessageBox("the cutpath has not been laid on the boundary of the square!");
		return;
	}

	m_vInMapPnt.clear();
	m_bIsBound = NULL;
	bIsBound = (bool*)malloc((m_nPtSum+m_nAddPnt) * sizeof(bool));

	for(i=0; i < m_nPtSum+m_nAddPnt; i++)
	    bIsBound[i] = FALSE;

	//区分边界顶点与非边界顶点
	for(i=0; i < m_vSideBand.size(); i++)
		bIsBound[m_vSideBand.at(i).start]=TRUE;

	m_bIsBound = bIsBound;

	int nBoundPntCnt=0;
	for(i = 0; i < m_nPtSum+m_nAddPnt; i++)
	{ 
		if(bIsBound[i]) 
			nBoundPntCnt++;
	}
	N =m_nPtSum+m_nAddPnt; //所有点的个数
	n= N-nBoundPntCnt; //内部点的个数
	
	int nBoundPnt;
	nBoundPnt = m_vBndMapPnt.size();

	inPt = (int*)malloc(n*sizeof(int));
	outPt = (int*)malloc(nBoundPnt*sizeof(int));
	
	int nInPtCnt=0;
	int nOutPtCnt=0;

	for(i=0; i < N; i++)
	{ 
		if(!bIsBound[i])
			inPt[nInPtCnt++] = i;
	}

	for(i = 0; i< m_vBndMapPnt.size(); i++)
		outPt[nOutPtCnt++] = m_vBndMapPnt.at(i).pointIndex;

//////////////////////////////////////////////////////////////////////////

	double **OutW;
	OutW = (double**)malloc(nInPtCnt * sizeof(double*));
	for(i=0; i < nInPtCnt; i++)
	{ 
		OutW[i] = (double*)malloc(nOutPtCnt*sizeof(double));
		for(j =0 ;j < nOutPtCnt; j++)
			OutW[i][j] = 0;
	}
//////////////////////////////////////////////////////////////////////////
	
	w=(double**)malloc(nInPtCnt * sizeof(double*));

	//计算权值Wij=1/di;
    switch(nFloaterType) 
	{
    case 1:
		ComputeTutteWeight(inPt,w,nInPtCnt);
    	break;
    case 2:
	    ComputeChordWeight(inPt,w,nInPtCnt);
	   	break;
	case 3:
		ComputeWeight(bIsBound,w);
		break;
	case 4:
		ComputeMeanValueWeght(bIsBound,w);
    default:
		break;
    } 

	double *A;
	A = (double*)malloc((n*n) * sizeof(double));
	for(i = 0; i < n ; i++)
	{ 
		for(j=0 ; j < n; j++)
		{ 
			if(j==i)
				A[i*n+j] = 1;
			else
				A[i*n+j] = - w[i][inPt[j]];
		}
	} 

	//计算b1,b2；b1,b2: n*1;
	//先找出各边界点对应的2D 坐标
	
	POINT2D *p;
	p = (POINT2D*)malloc(nOutPtCnt*sizeof(POINT2D));
	for(i = 0; i < m_vBndMapPnt.size(); i++)
	{ 
		p[i] = m_vBndMapPnt.at(i).p; 
	}

	double *b1,*b2;
 	b1 = (double*)malloc(n * sizeof(double));
	b2 = (double*)malloc(n * sizeof(double));
	


    /////////////////////////////////////////////////////////////////////////
	SepPoints();
    //////////////////////////////////////////////////////////////////////////

	

	
	FILE *facefp;
	facefp=fopen("newfaces0.txt","w+");
	
	for(i = 0; i < m_nFaceSum+m_nAddFaces; i++)
	{ 
		fprintf(facefp,"%d --- %d   %d   %d\n",i, meshFaces(i).vI[0],meshFaces(i).vI[1],meshFaces(i).vI[2]);
	}

	fclose(facefp);


	int neig;
	int bndPntIndex;
	int pntIndex;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	for(i =0 ;i < m_vBndPnt.size();i++)
	{ 
		for(j = 0; j<MAXNEIGHBORPOINT; j++)
		{ 
			neig = m_vBndPnt.at(i).neighborPnt[j];
			if(neig != -1&&bIsBound[neig]==FALSE)
			{
				bndPntIndex= m_vBndPnt.at(i).sideBandIndex;
				pntIndex = m_vBndPnt.at(i).pointIndex;
				for(k = 0; k < nInPtCnt; k++)
				{ 
					if(inPt[k]==neig) 
						break;
				}
				OutW[k][bndPntIndex] = w[k][pntIndex];
			}
		}
	}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	for(i = 0; i < n ; i++)
	{ 
		b1[i] = 0;
		for(j = 0;  j < nOutPtCnt; j++)
 			b1[i]+= OutW[i][j] * p[j].x; 
	}
	
	for(i = 0; i < n ; i++)
	{ 
		b2[i] = 0;
		for(j = 0;  j < nOutPtCnt; j++)
 			b2[i]+= OutW[i][j] * p[j].y; 
	}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	//求解u和v，即内部点的x和y坐标
	u = (double*)malloc(n*sizeof(double));
	v = (double*)malloc(n*sizeof(double));
	if(invers_matrix(A,n))
	{ 
		mult(A,b1,u,n,n,1);
		mult(A,b2,v,n,n,1);
	}
	FILE* interfp;
	interfp = fopen("inter.txt","w+");
	//if(fp == NULL)
	//	return;

	for(i = 0; i<n; i++)
	{ 
		if(u[i]==0||v[i]==0)
		{
			if(u[i]==0)
				u[i]=0.01;
			else
				v[i]=0.01;
			
			//AfxMessageBox("the number of grid is not proper!   0");
			//return;
		}
		if(u[i]==1||v[i]==1)
		{
			if(u[i]==1)
				u[i]=0.99;
			else
				v[i]=0.99;
			
			//AfxMessageBox("the number of grid is not proper!   1");
			//return;
		}
		fprintf(interfp,"%lf  %lf  \n",u[i],v[i] );
	}
	fclose(interfp);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	MapPoint tempMP;

	 m_pInPnts=(POINT2D*)malloc((nOutPtCnt+nInPtCnt)*sizeof(POINT2D));
	for(i=0; i < n; i++)
	{
		m_pInPnts[inPt[i]].x = u[i];
		m_pInPnts[inPt[i]].y = v[i];
		tempMP.fx = meshPnt(inPt[i]).x;
		tempMP.fy = meshPnt(inPt[i]).y;
		tempMP.fz = meshPnt(inPt[i]).z;
		tempMP.p.x = u[i];
		tempMP.p.y = v[i];
		tempMP.pointIndex = inPt[i];
		m_vInMapPnt.push_back(tempMP);
	}


	free(u);
	u = NULL;
	free(v);
	v = NULL; 
	free(A);
	A = NULL;
	//free(bIsBound);
	free(inPt);
	inPt = NULL; 
	free(outPt);
	outPt = NULL;
	free(p);
	p = NULL;

/*
	for(i=0; i < n; i++)
		free(OutW[i]);
	free(OutW);
	OutW = NULL;
*/

	//////////////////////////////////////////////////////////////////////////
 	CreateNewFaces();
	
	//OutputResult();
}

void Mesh3D::OutputResult()
{
	CString strfilename,str;
	CStdioFile outFile;
	int m,i;

	strfilename = m_strPlyName+"_result.txt";
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
	str.Format("%lf\n",m_lfScale);
	outFile.WriteString(str);
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
	vector<stFACE> vFace;
	stFACE tempFace;
	
	str.Format("%d\n",m_vNewFaces.size());
	outFile.WriteString(str);

	vFace.clear();
	for(m=0; m < m_vNewFaces.size(); m++)
	{
		for(i=0; i < 3 ; i++)
		{ 
			if(m_vNewFaces.at(m).flag[i]==0)
				pntIndex[i]=m_vNewFaces.at(m).index[i]+m_vBndMapPnt.size();
			else 
				pntIndex[i]=m_vNewFaces.at(m).index[i];
		}
		tempFace.n=3;
		tempFace.vI[0]=pntIndex[0];
		tempFace.vI[1]=pntIndex[1];
		tempFace.vI[2]=pntIndex[2];
		vFace.push_back(tempFace);
		
		
		str.Format("%d %d %d\n",pntIndex[0],pntIndex[1],pntIndex[2]);	
		outFile.WriteString(str);
	}
	outFile.Close(); 
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
	vector<GEOIMG_EDGE> vNewEdges;
	GEOIMG_EDGE edgeTemp1;
	GEOIMG_EDGE edgeTemp2;

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
}

void Mesh3D::DisplayInterior2()
{
	int i,j;
 	int tstart,tend;
	double t1,t2,t3,t4;
 	double lf3dlen,lf2dlen;
	double R,G,B;
	double H,S,I;
	double lfRatio;
	double lfRatioMax,lfRatioMin;;
	double min,max;
	int tempIndex;
	double T,P,Q,F;
	int TEMP;
	double lfx;
	double *lfStat;
	int nCnt;


	 /*
	FILE *meshPntFp;
	meshPntFp=fopen("meshpoint.txt","w+");
	for(i=0; i < m_nPtSum+m_nAddPnt; i++)
	{ 
		fprintf(meshPntFp,"%lf  %lf  %lf\n",meshPnt(i).x,meshPnt(i).y,meshPnt(i).z);
	}
	fclose(meshPntFp);

   
	FILE *fp1,*fp2;
	fp1=fopen("in_corr.txt","w+");
	for(i=0; i < m_vInMapPnt.size();i++)
	{ 
		fprintf(fp1,"%d %d\n",i,m_vInMapPnt.at(i).pointIndex);
	}
	fclose(fp1);

	fp2=fopen("bound_corr.txt","w+");

	for(i=0; i < m_vBndMapPnt.size();i++)
	{ 
		fprintf(fp2,"%d %d\n",i,m_vBndMapPnt.at(i).pointIndex);
	}
	fclose(fp2);
	*/

		//geoStretch=(double*)malloc((m_vNewFaces.size())*sizeof(double));
		lfStat = (double*) malloc ((3*m_vNewFaces.size())*sizeof(double));
		nCnt = 0;
		
	    glColor3f(0.0f, 1.0f, 1.0f);
		glLineWidth(1.0);
	 	
		I=1.0;
		S=1.0;
		
		max = I + S - I * S;
		min = 2*I - max;
	
	 	//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		lfRatioMin = 1000;
		lfRatioMax = -100;
		for(i = 0; i < m_vNewFaces.size(); i++)
		{ 
			for(j=0; j < 3; j++)
			{ 
				if(m_vNewFaces.at(i).flag[j]==0)
				{
					tempIndex = m_vNewFaces.at(i).index[j];
					t1=m_vInMapPnt.at(tempIndex).p.x;
					t2=m_vInMapPnt.at(tempIndex).p.y;
					tstart=m_vInMapPnt.at(tempIndex).pointIndex;
				}
				else if(m_vNewFaces.at(i).flag[j]==1)
				{ 
					tempIndex = m_vNewFaces.at(i).index[j];
					t1=m_vBndMapPnt.at(tempIndex).p.x;
					t2=m_vBndMapPnt.at(tempIndex).p.y;
					tstart=m_vBndMapPnt.at(tempIndex).pointIndex;
				}
	
				if(m_vNewFaces.at(i).flag[(j+1)%3]==0)
				{
					tempIndex = m_vNewFaces.at(i).index[(j+1)%3];
					t3=m_vInMapPnt.at(tempIndex).p.x;
					t4=m_vInMapPnt.at(tempIndex).p.y;
					tend=m_vInMapPnt.at(tempIndex).pointIndex;
				}
				else if(m_vNewFaces.at(i).flag[(j+1)%3]==1)
				{ 
					tempIndex = m_vNewFaces.at(i).index[(j+1)%3];
					t3=m_vBndMapPnt.at(tempIndex).p.x;
					t4=m_vBndMapPnt.at(tempIndex).p.y;
					tend=m_vBndMapPnt.at(tempIndex).pointIndex;
				}
				
			 
				lf3dlen = sqrt((meshPnt(tstart).x - meshPnt(tend).x)*(meshPnt(tstart).x - meshPnt(tend).x)+
							(meshPnt(tstart).y - meshPnt(tend).y)*(meshPnt(tstart).y - meshPnt(tend).y)+
							(meshPnt(tstart).z - meshPnt(tend).z)*(meshPnt(tstart).z - meshPnt(tend).z));
				lf2dlen = sqrt((t1-t3)*(t1-t3)+(t2-t4)*(t2-t4)); 

				lf2dlen = lf2dlen/m_lfScale;
					
				lfx=fabs(lf3dlen-lf2dlen);
	
				lfRatio = (MyE - pow(MyE,(1/(lfx+1))))/(MyE-1);
	
				if(lfRatio > lfRatioMax)  lfRatioMax = lfRatio;
				if(lfRatio < lfRatioMin)  lfRatioMin = lfRatio;
	
				lfStat[nCnt++]= lfRatio;
				
				H = 240-(int)(lfRatio * 240)%240;
				H/=60.0;
				TEMP=floor(H);
				F = H -TEMP;
				P = I *(1.0 -S);
				Q = I * (1.0 - (S*F));
				T = I *(1.0 - (S *(1.0 -F)));
				switch(TEMP) 
				{
				case 0:R =I; G =T; B =P; break;
				case 1:R =Q; G =I; B = P ;break;
				case 2:R= P; G = I; B = T; break;
				case 3: R = P; G =Q; B= I;break; 
				case 4:R = T; G = P; B= I;break;
				case 5: R= I; G = P; B = Q;break;
				}
				glColor3f(R,G,B);
				if(lf2dlen > lf3dlen)
				{
					glLineStipple(3,0xAAAA);
					glEnable(GL_LINE_STIPPLE);
				}
				glBegin(GL_LINES);
				glVertex2f(t1,t2);
				glVertex2f(t3,t4);
				glEnd();
				glDisable(GL_LINE_STIPPLE);
			}	
		}
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
		int t;
		double x[3],y[3];
		t=FindMaxStrTri();
	
		for(j=0; j < 3; j++)
			{ 
				if(m_vNewFaces.at(t).flag[j]==0)
				{
					tempIndex = m_vNewFaces.at(t).index[j];
					x[j]=m_vInMapPnt.at(tempIndex).p.x;
					y[j]=m_vInMapPnt.at(tempIndex).p.y;
				}
				else if(m_vNewFaces.at(t).flag[j]==1)
				{ 
					tempIndex = m_vNewFaces.at(t).index[j];
					x[j]=m_vBndMapPnt.at(tempIndex).p.x;
					y[j]=m_vBndMapPnt.at(tempIndex).p.y;
				}
	
			}
		glColor3f(1.0f,0.0f,0.0f);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glBegin(GL_TRIANGLES);
			glVertex2f(x[0],y[0]);
			glVertex2f(x[1],y[1]);
			glVertex2f(x[2],y[2]);
		glEnd();
			
	
		double lfMean;
		double lfCov;
		double lfSum=0.0;
		//计算平均值
		for(i=0; i < nCnt; i++)
		{ 
			lfSum+=lfStat[i];
		}
		lfMean=(lfSum)/(double)nCnt;
	
		lfSum=0;
		for(i=0; i < nCnt; i++)
		{ 
			lfSum+=(lfStat[i]-lfMean)*(lfStat[i]-lfMean);
		}
		lfCov=sqrt(lfSum/(nCnt-1));
	
		//geometric stretch
		double geoS;
		computeGeoStretch(&geoS);
	
	 	m_lfStat[0]=lfRatioMax;
		m_lfStat[1]=lfRatioMin;
		m_lfStat[2]=lfMean;
		m_lfStat[3]=lfCov;
		m_lfStat[4]=geoS;
	 
		FILE *statfp;
		statfp = fopen("statistics.txt","w+");
		fprintf(statfp,"Max----%lf\n",lfRatioMax);
		fprintf(statfp,"Min----%lf\n",lfRatioMin);
		fprintf(statfp,"Mean----%lf\n",lfMean);
		fprintf(statfp,"Covariance----%lf\n",lfCov);
	
		fprintf(statfp,"Geometry Stretch---\n");
		fprintf(statfp,"\n\n\n%lf\n",geoS);
		fclose(statfp);
	
		glLineWidth(1.0f);
}



//DEL void Mesh3D::DisplayInterior()
//DEL {
//DEL 	
//DEL 	int i,j;
//DEL  	int tstart,tend;
//DEL 	double t1,t2,t3,t4;
//DEL 	int tempNeig;
//DEL 	double lf3dlen,lf2dlen;
//DEL 	double R,G,B;
//DEL 	double H;
//DEL 	double lfRatio;
//DEL 	double lfRatioMax;
//DEL 	double min,max;
//DEL 	 
//DEL 	glColor3f(1.0f, 1.0f, 1.0f);
//DEL 	glBegin(GL_POLYGON);
//DEL 		glVertex2f(0.0f, 0.0f);
//DEL 		glVertex2f(0.0f, 1.0f);
//DEL 		glVertex2f(1.0f, 1.0f);
//DEL 		glVertex2f(1.0f, 0.0f);
//DEL 	glEnd();
//DEL /*	
//DEL 	glPointSize(2.0);
//DEL 	glColor3f(1.0f, 1.0f, 0.0f);
//DEL 	glBegin(GL_POINTS);
//DEL 		for(i = 0; i < m_vInMapPnt.size(); i++)
//DEL 			glVertex2f(m_vInMapPnt.at(i).p.x,m_vInMapPnt.at(i).p.y);
//DEL 		for(i = 0; i < m_vBndMapPnt.size(); i++)
//DEL 			glVertex2f(m_vBndMapPnt.at(i).p.x,m_vBndMapPnt.at(i).p.y);
//DEL 	glEnd();
//DEL 	*/
//DEL 
//DEL 	glColor3f(0.0f, 1.0f, 1.0f);
//DEL 	glLineWidth(2.0);
//DEL 	//????
//DEL 	
//DEL 	min=0.0;
//DEL 	max=1.0;
//DEL 
//DEL 	//////////////////////////////////////////////////////////////////////////
//DEL 	//////////////////////////////////////////////////////////////////////////
//DEL 	//////////////////////////////////////////////////////////////////////////
//DEL 	lfRatio=0.0;
//DEL 	for(i = 0; i < m_vBndPnt.size(); i++)
//DEL 	{ 
//DEL 		for( j =0; j < MAXNEIGHBORPOINT; j++)
//DEL 		{ 
//DEL 			tempNeig = m_vBndPnt.at(i).neighborPnt[j];
//DEL 			if(m_vBndPnt.at(i).neighborPnt[j]!=-1 )
//DEL 			{ 
//DEL 				//t1 = m_pInPnts[m_vBndPnt.at(i).pointIndex].x;
//DEL 				//t2 = m_pInPnts[m_vBndPnt.at(i).pointIndex].y;
//DEL 				if(m_bIsBound[tempNeig]==FALSE)
//DEL 				{
//DEL 					t1 = m_vBndMapPnt.at(m_vBndPnt.at(i).sideBandIndex).p.x;
//DEL 					t2 = m_vBndMapPnt.at(m_vBndPnt.at(i).sideBandIndex).p.y;
//DEL 					t3 = m_pInPnts[tempNeig].x;
//DEL 					t4 = m_pInPnts[tempNeig].y;
//DEL 					lf3dlen = sqrt((meshPnt(m_vBndPnt.at(i).pointIndex).x - meshPnt(tempNeig).x)*(meshPnt(m_vBndPnt.at(i).pointIndex).x - meshPnt(tempNeig).x)+
//DEL 						(meshPnt(m_vBndPnt.at(i).pointIndex).y - meshPnt(tempNeig).y)*(meshPnt(m_vBndPnt.at(i).pointIndex).y - meshPnt(tempNeig).y)+
//DEL 						(meshPnt(m_vBndPnt.at(i).pointIndex).z - meshPnt(tempNeig).z)*(meshPnt(m_vBndPnt.at(i).pointIndex).z - meshPnt(tempNeig).z));
//DEL 					lf2dlen = sqrt((t1-t3)*(t1-t3)+(t2-t4)*(t2-t4));
//DEL 					lfRatio = lf2dlen/lf3dlen;
//DEL 					if(lfRatio > lfRatioMax) lfRatioMax = lfRatio;
//DEL 				}
//DEL 			}
//DEL 		}
//DEL 	}
//DEL 	for(i = 0; i < m_vEdge.size(); i++)
//DEL 	{   
//DEL  			
//DEL 		tstart = m_vEdge.at(i).start;
//DEL 		tend = m_vEdge.at(i).end;
//DEL 		if(m_bIsBound[tstart]==FALSE && m_bIsBound[tend]==FALSE)
//DEL 		{
//DEL 			t1 = m_pInPnts[tstart].x;
//DEL 			t2 = m_pInPnts[tstart].y;
//DEL 			t3 = m_pInPnts[tend].x;
//DEL 			t4 = m_pInPnts[tend].y;
//DEL 
//DEL 			lf3dlen = sqrt((meshPnt(tstart).x - meshPnt(tend).x)*(meshPnt(tstart).x - meshPnt(tend).x)+
//DEL 						(meshPnt(tstart).y - meshPnt(tend).y)*(meshPnt(tstart).y - meshPnt(tend).y)+
//DEL 						(meshPnt(tstart).z - meshPnt(tend).z)*(meshPnt(tstart).z - meshPnt(tend).z));
//DEL 			
//DEL 			lf2dlen = sqrt((t1-t3)*(t1-t3)+(t2-t4)*(t2-t4));
//DEL 			lfRatio = lf2dlen/lf3dlen;
//DEL 			if(lfRatio > lfRatioMax)  lfRatioMax = lfRatio;
//DEL 		}
//DEL 	}
//DEL 
//DEL     //////////////////////////////////////////////////////////////////////////
//DEL 	for(i = 0; i < m_vBndPnt.size(); i++)
//DEL 	{ 
//DEL 		for( j =0; j < MAXNEIGHBORPOINT; j++)
//DEL 		{ 
//DEL 			tempNeig = m_vBndPnt.at(i).neighborPnt[j];
//DEL 			if(m_vBndPnt.at(i).neighborPnt[j]!=-1 )
//DEL 			{ 
//DEL 				//t1 = m_pInPnts[m_vBndPnt.at(i).pointIndex].x;
//DEL 				//t2 = m_pInPnts[m_vBndPnt.at(i).pointIndex].y;
//DEL 				if(m_bIsBound[tempNeig]==FALSE)
//DEL 				{
//DEL 					t1 = m_vBndMapPnt.at(m_vBndPnt.at(i).sideBandIndex).p.x;
//DEL 					t2 = m_vBndMapPnt.at(m_vBndPnt.at(i).sideBandIndex).p.y;
//DEL 					t3 = m_pInPnts[tempNeig].x;
//DEL 					t4 = m_pInPnts[tempNeig].y;
//DEL 					lf3dlen = sqrt((meshPnt(m_vBndPnt.at(i).pointIndex).x - meshPnt(tempNeig).x)*(meshPnt(m_vBndPnt.at(i).pointIndex).x - meshPnt(tempNeig).x)+
//DEL 						(meshPnt(m_vBndPnt.at(i).pointIndex).y - meshPnt(tempNeig).y)*(meshPnt(m_vBndPnt.at(i).pointIndex).y - meshPnt(tempNeig).y)+
//DEL 						(meshPnt(m_vBndPnt.at(i).pointIndex).z - meshPnt(tempNeig).z)*(meshPnt(m_vBndPnt.at(i).pointIndex).z - meshPnt(tempNeig).z));
//DEL 					lf2dlen = sqrt((t1-t3)*(t1-t3)+(t2-t4)*(t2-t4));
//DEL 					lfRatio = lf2dlen/lf3dlen;
//DEL 					
//DEL 					lfRatio = lfRatio/lfRatioMax;
//DEL 					H = (int)(lfRatio * 360)%360;
//DEL 					//////////////////////////////////////////////////////////////////////////
//DEL 					//HSI -> RGB
//DEL 					//如果 H < 60, R = m + (M – m) * (H / 60);
//DEL 					//如果 H < 180, R = M;
//DEL 					//如果 H < 240, R = m + (M – m) * ((240 – H) / 60);
//DEL 					//如果 H < 360, R = m;
//DEL 
//DEL 					if(H < 60)  R =  min + (max - min) * (H / 60);
//DEL 					else if(H < 180 ) R = max;
//DEL 					else if(H < 240 ) R = min + (max - min) * ((240 - H) / 60);
//DEL 					else if(H < 360 ) R = min;
//DEL             
//DEL 					//G的范围为0到1.0，计算G的公式为：
//DEL 					//如果 H < 120, G = m;
//DEL 					//如果 H < 180, G = m + (M – m) * ((H - 120) / 60);
//DEL 					//如果 H < 300, G = M;
//DEL 					//如果 H < 360, G = m + (M – m) * ((360 – H) / 60);
//DEL 
//DEL 					if (H < 120 ) G = min;
//DEL 					else if (H < 180) G = min + (max - min) * ((H -120) / 60);
//DEL 					else if (H < 300) G = max;
//DEL 					else if (H < 360) G = min + (max - min) * ((360 - H) / 60);
//DEL 					
//DEL 					//B的范围为0到1.0，计算B的公式为：
//DEL 					//如果 H < 60, B = M;
//DEL 					//如果 H < 180, B = m + (M – m) * ((180 – H) / 60);
//DEL 					//如果 H < 240, B = m;
//DEL 					//如果 H < 360, B = m + (M – m) * ((H – 240) / 60);
//DEL 
//DEL 					if ( H < 60 ) B = max;
//DEL 					else if ( H < 180 ) B = min + (max - min) * ((180 - H) / 60);
//DEL 					else if ( H < 240 ) B = min;
//DEL 					else if ( H < 360 ) B = min + (max - min) * ((H - 240 ) / 60);
//DEL 					
//DEL 					//R=lfRatio;
//DEL 					//G =R;
//DEL 					//B=R;
//DEL 					glColor3f(R,G,B);
//DEL 					glBegin(GL_LINES);
//DEL 					glVertex2f(t1,t2);
//DEL 					glVertex2f(t3,t4);
//DEL 					glEnd();
//DEL 				}
//DEL 			}
//DEL 		}
//DEL 	}
//DEL 
//DEL 	//////////////////////////////////////////////////////////////////////////
//DEL 	//////////////////////////////////////////////////////////////////////////
//DEL 	//////////////////////////////////////////////////////////////////////////
//DEL     //////////////////////////////////////////////////////////////////////////
//DEL  	for(i = 0; i < m_vEdge.size(); i++)
//DEL 	{   
//DEL  			
//DEL 		tstart = m_vEdge.at(i).start;
//DEL 		tend = m_vEdge.at(i).end;
//DEL 		if(m_bIsBound[tstart]==FALSE && m_bIsBound[tend]==FALSE)
//DEL 		{
//DEL 			t1 = m_pInPnts[tstart].x;
//DEL 			t2 = m_pInPnts[tstart].y;
//DEL 			t3 = m_pInPnts[tend].x;
//DEL 			t4 = m_pInPnts[tend].y;
//DEL 
//DEL 			lf3dlen = sqrt((meshPnt(tstart).x - meshPnt(tend).x)*(meshPnt(tstart).x - meshPnt(tend).x)+
//DEL 						(meshPnt(tstart).y - meshPnt(tend).y)*(meshPnt(tstart).y - meshPnt(tend).y)+
//DEL 						(meshPnt(tstart).z - meshPnt(tend).z)*(meshPnt(tstart).z - meshPnt(tend).z));
//DEL 			
//DEL 			lf2dlen = sqrt((t1-t3)*(t1-t3)+(t2-t4)*(t2-t4));
//DEL 			lfRatio = lf2dlen/lf3dlen;
//DEL 			
//DEL 			lfRatio = lfRatio/lfRatioMax;
//DEL 			H = lfRatio * 360;
//DEL 			//////////////////////////////////////////////////////////////////////////
//DEL 			//HSI -> RGB
//DEL 			if(H < 60)  R =  min + (max - min) * (H / 60);
//DEL 			else if(H < 180 ) R = max;
//DEL 			else if(H < 240 ) R = min + (max - min) * ((240 - H) / 60);
//DEL 			else if(H < 360 ) R = min;
//DEL             
//DEL 			if (H < 120 ) G = min;
//DEL 			else if (H < 180) G = min + (max - min) * ((H -120) / 60);
//DEL 			else if (H < 300) G = max;
//DEL 			else if (H < 360) G = min + (max - min) * ((360 - H) / 60);
//DEL 			
//DEL 			//B的范围为0到1.0，计算B的公式为：
//DEL 			if ( H < 60 ) B = max;
//DEL 			else if ( H < 180 ) B = min + (max - min) * ((180 - H) / 60);
//DEL 			else if ( H < 240 ) B = min;
//DEL 			else if ( H < 360 ) B = min + (max - min) * ((H - 240 ) / 60);
//DEL 
//DEL 			glColor3f(R,G,B);
//DEL 			glBegin(GL_LINES);
//DEL 				glVertex2f(t1,t2);
//DEL 				glVertex2f(t3,t4);
//DEL 			glEnd();
//DEL 		}
//DEL 	}  
//DEL 	glColor3f(1.0f, 1.0f, 1.0f);
//DEL 	glLineWidth(1.0);
//DEL 	glPointSize(1.0);
//DEL }


POINT3D Mesh3D::meshPnt(int n) const
{
	POINT3D  p;
	
	if(n < m_nPtSum)
	{
		p = m_pPts[n];
	}
	else if( n < m_nPtSum+m_nAddPnt)
		p = m_pAddPts[n-m_nPtSum];
	else
	{ 
		AfxMessageBox("Exceed the range of the points!");
	}
	return(p);
}

stFACE  Mesh3D::meshFaces(int n) const
{
	stFACE  p;
	if(n < m_nFaceSum )
		p = m_pFaces[n];
	else if(n < m_nFaceSum+ m_nAddFaces)
		p = m_pAddFaces[n - m_nFaceSum];
	else
		AfxMessageBox("Exceed the range of the faces!");
	return(p);
}


void Mesh3D::ComputeTutteWeight(int *inPnt, double **w,int nInPnt)
{
	int i,j,k; 

	int n, N;
	int nNeighbor;
	int neighborPnt[MAXNEIGHBORPOINT];

	N = m_nPtSum + m_nAddPnt;
	n = nInPnt;
	
	

	for(k = 0; k < n ; k++)
	{ 
		i = inPnt[k]; 
		nNeighbor = 0; 
		for(j=0; j < MAXNEIGHBORPOINT; j++)
		neighborPnt[j]=-1;
		w[k] = (double*)malloc(N*sizeof(double));
		for(j = 0; j < N ; j++)
			w[k][j]=0;

		for(j =0; j < m_vEdge.size(); j++)
		{ 
			if(m_vEdge.at(j).start==i)
			{ 
				neighborPnt[nNeighbor] = m_vEdge.at(j).end;
				nNeighbor++;
			}
			else if(m_vEdge.at(j).end == i)
			{ 
				neighborPnt[nNeighbor] = m_vEdge.at(j).start;
				nNeighbor++;
			}
		}
		
		for(j = 0; j < nNeighbor; j++)
			w[k][neighborPnt[j]] = (double)1.0/nNeighbor;
	}
}


void Mesh3D::ComputeChordWeight(int *inPnt, double **w, int nInPnt)
{
		int i,j,k; 

	int n, N;
	int nNeighbor;
	int neighborPnt[MAXNEIGHBORPOINT];
	double length;
	double sum;

	N = m_nPtSum + m_nAddPnt;
	n = nInPnt;
	
	
 
	for(k = 0; k < n ; k++)
	{ 
		i = inPnt[k]; 
		nNeighbor = 0;
		for(j=0; j < MAXNEIGHBORPOINT; j++)
		neighborPnt[j]=-1;
		w[k] = (double*)malloc(N*sizeof(double));
		for(j = 0; j < N ; j++)
			w[k][j]=0;

		for(j =0; j < m_vEdge.size(); j++)
		{ 
			if(m_vEdge.at(j).start==i)
			{ 
				neighborPnt[nNeighbor] = m_vEdge.at(j).end;
				nNeighbor++;
			}
			else if(m_vEdge.at(j).end == i)
			{ 
				neighborPnt[nNeighbor] = m_vEdge.at(j).start;
				nNeighbor++;
			}
		}
		sum=0;
		for(j = 0; j < nNeighbor; j++)
		{
			length = sqrt((meshPnt(i).x-meshPnt(neighborPnt[j]).x)*(meshPnt(i).x-meshPnt(neighborPnt[j]).x)+
						  (meshPnt(i).y-meshPnt(neighborPnt[j]).y)*(meshPnt(i).y-meshPnt(neighborPnt[j]).y)+
						  (meshPnt(i).z-meshPnt(neighborPnt[j]).z)*(meshPnt(i).z-meshPnt(neighborPnt[j]).z));
			w[k][neighborPnt[j]] = (double)1.0/length;
			sum+=(double)1.0/length;
		}
		for(j=0; j < nNeighbor; j++)
		{ 
			w[k][neighborPnt[j]] = w[k][neighborPnt[j]]/sum;
		}
	}	
}

//分离切线内侧的点与切线外侧的点
void Mesh3D::SepPoints()
{
	int i,j;
	int preNode, nextNode,currentNode;
	BndPoint tempBndPnt;
	int preEnd, tempEnd, nextEnd;
	int m;
	int start1,start2;
	int choose;
	int nNeighbor;
 	int neighborTri1,neighborTri2;
	int neighbor1,neighbor2;
	int TriIndex;
	int neighbor[2];
	int nN;


	m_vBndPnt.clear();

	int *sidePnts;
	int n;
	n = m_vSideBand.size();
	sidePnts = (int*)malloc(n*sizeof(int));
	for(i = 0; i < n; i++)
		sidePnts[i] = m_vSideBand.at(i).start;

	for(i = 0; i < n; i++)
	{ 
		preNode =  sidePnts[((i-1)>=0)?(i-1):(n+i-1)];
		currentNode = sidePnts[i];
		nextNode = sidePnts[(i+1)%n];
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		for(j=0; j < MAXNEIGHBORPOINT; j++ )
			tempBndPnt.neighborPnt[j]=-1;
        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////
 		nN = 0;
		for(j = 0; j < m_nFaceSum + m_nAddFaces; j++)
		{ 
			for(m = 0; m < 3; m++)
			{ 
				if((meshFaces(j).vI[m] == preNode && meshFaces(j).vI[(m+1)%3] ==currentNode)||
				   (meshFaces(j).vI[m] == currentNode && meshFaces(j).vI[(m+1)%3] ==preNode))	
				   break;
			}
			if(m<3)
				neighbor[nN++] = j;
		}
		neighborTri1 =  neighbor[0];
		neighborTri2 =  neighbor[1];

		for(m=0; m < 3 ; m++)
		{ 
			if(meshFaces(neighborTri1).vI[m] == preNode) 
			{ 
				start1 = m;
				break;
			}
		}
		for(m=0; m < 3 ; m++)
		{ 
			if(meshFaces(neighborTri2).vI[m] == preNode) 
			{
				start2 = m;
				break;
			}
		}
		//选定一个边方向与切线走向相同的三角形
		if(meshFaces(neighborTri1).vI[(start1+1)%3] == currentNode)
		{
			 choose = 1;
			 nextEnd = meshFaces(neighborTri1).vI[(start1+2)%3];
		}
		else
		{
			 choose = 2;
			 nextEnd = meshFaces(neighborTri2).vI[(start2+2)%3];
		}
		
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		tempEnd = nextEnd;
		preEnd = preNode;
		nNeighbor = 0;
		while(1)
		{ 
			if(tempEnd==nextNode)
				break;
			else
			{
				if(tempEnd!=currentNode)
				{
					tempBndPnt.neighborPnt[nNeighbor] = tempEnd;
					nNeighbor++;
					ASSERT(nNeighbor < MAXNEIGHBORPOINT);
				}
			}
 			nN = 0;
			for(j = 0; j < m_nFaceSum + m_nAddFaces; j++)
			{ 
				for(m = 0; m < 3; m++)
				{ 
					if((meshFaces(j).vI[m] == tempEnd && meshFaces(j).vI[(m+1)%3] ==currentNode)||
					   (meshFaces(j).vI[m] == currentNode && meshFaces(j).vI[(m+1)%3] ==tempEnd))	
					  break;
				}
				if(m<3)
					neighbor[nN++] = j;
			}
			neighbor1 =  neighbor[0];
			neighbor2 =  neighbor[1];
		
			for(m=0; m < 3; m++)
			{ 
				if(meshFaces(neighbor1).vI[m] == preEnd) break;
			}
			if(m==3) //下一个三角形不是neighbor2，而是neighbor1
			   TriIndex = neighbor1;
			else //下一个三角形是neighbor2
				TriIndex = neighbor2;
			for(m =0; m < 3 ; m++)
			{ 
				if(meshFaces(TriIndex).vI[m] != currentNode && meshFaces(TriIndex).vI[m]!=tempEnd)
					break;
			}
			preEnd = tempEnd;
			tempEnd = meshFaces(TriIndex).vI[m];
			
		}
		tempBndPnt.pointIndex = currentNode;
		tempBndPnt.sideBandIndex = i;
		m_vBndPnt.push_back(tempBndPnt);
	}

	/*
	FILE* sfp;
	sfp = fopen("sepside.txt","w+");
	for(i = 0; i < m_vBndPnt.size(); i++)
	{ 
		fprintf(sfp, "%d  %d   ",m_vBndPnt.at(i).sideBandIndex, m_vBndPnt.at(i).pointIndex);
		for(j = 0; j< MAXNEIGHBORPOINT; j++)
			fprintf(sfp, "%d ",m_vBndPnt.at(i).neighborPnt[j]);
		fprintf(sfp,"\n");			   
	}
	fclose(sfp);
	*/
}

void Mesh3D::ComputeWeight(bool *bIsBound,double **w)
{
	int i,j,k; 
	int m ;
	int end1,end2;
	int TriIndex;
	int preEnd,tempEnd;
	int neighbor1, neighbor2;
	int *neighborPnt ;
	int nNeighbor;
	POINT3D v1,v2,v3;
	POINT2D *p;
	double *ang;
	double totalAng;
	double **u;
	double *tempu;
	int nInPnts;

	nInPnts=0;
	m_ppConnect = (int**)malloc((m_nPtSum+m_nAddPnt) * sizeof(int*));

	for(i = 0; i < m_nPtSum+m_nAddPnt; i++)
	{ 
		if(i==225)
			i=i;
		if(!bIsBound[i])
		{ 
			m_currentCenterPnt =i;
			///////////////////////////////////
			//找出点i 的邻点，并按逆时针顺序排列
			neighborPnt = (int*)malloc(MAXNEIGHBORPOINT * sizeof(int));
			for(j=0; j < MAXNEIGHBORPOINT; j++ )
				neighborPnt[j]=-1;
			for(j = 0; j < m_vEdge.size(); j++)
			{
				if(m_vEdge.at(j).start == i || m_vEdge.at(j).end == i)
				{ 
					TriIndex = m_vEdge.at(j).neighborTri[0];
					for(m=0; m < 3 ; m++)
					{ 
						if(meshFaces(TriIndex).vI[m] == i)
							break;
					}
					end1 = meshFaces(TriIndex).vI[(m+1)%3];
					end2 = meshFaces(TriIndex).vI[(m+2)%3];
					break;
				}
			}
			neighborPnt[0]=end1;
			neighborPnt[1]=end2;
			tempEnd = end2;
			preEnd = end1;
			nNeighbor = 2;
			while(1)
			{ 
				for(j=0; j < m_vEdge.size(); j++)
				{ 
					if((m_vEdge.at(j).start == i && m_vEdge.at(j).end==tempEnd) ||
						(m_vEdge.at(j).start == tempEnd && m_vEdge.at(j).end ==i))
						break;
				}
				
				ASSERT(j < m_vEdge.size());

				neighbor1 = m_vEdge.at(j).neighborTri[0];
				neighbor2 = m_vEdge.at(j).neighborTri[1];

				for(m=0; m < 3; m++)
				{ 
					if(meshFaces(neighbor1).vI[m] == preEnd) break;
				}
				if(m==3) //下一个三角形不是neighbor1，而是neighbor2
				   TriIndex = neighbor1;
				else //下一个三角形是neighbor1
					TriIndex = neighbor2;
				for(m =0; m < 3 ; m++)
				{ 
					if(meshFaces(TriIndex).vI[m] != i && meshFaces(TriIndex).vI[m]!=tempEnd)
						break;
				}
				preEnd = tempEnd;
				tempEnd = meshFaces(TriIndex).vI[m];
				if(tempEnd==end1)
					break;
				else
				{
					neighborPnt[nNeighbor] = tempEnd;
					nNeighbor++;
				}
			}
			m_ppConnect[i]=neighborPnt;

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			ang=(double*)malloc(nNeighbor*sizeof(double));
			totalAng = 0 ;
			//计算Xjk, Xi, Xjk+1之间的夹角
			for(j=0; j < nNeighbor; j++)
			{ 
				v1=meshPnt(neighborPnt[j]);
				v2=meshPnt(i);
				v3=meshPnt(neighborPnt[(j+1)%nNeighbor]);
				ang[j]=ComputeAng(v1,v2,v3);
				totalAng += ang[j];
			}
			m_tempAng = ang;
			m_totalAng = totalAng;
			m_ntempNneighbor = nNeighbor;
			w[nInPnts]=(double*)malloc(sizeof(double)*(m_nPtSum+m_nAddPnt));
			for(j = 0; j < m_nPtSum + m_nAddPnt; j++)
				w[nInPnts][j] = 0;
			
			u=(double**)malloc( nNeighbor * sizeof(double*));

			for(j=0; j < nNeighbor; j++)
			{ 
				tempu=(double*)malloc(sizeof(double)*nNeighbor);
				for(k=0; k < nNeighbor; k++)
					tempu[k]=0.0;
				u[j]=tempu;
			}
			 			
			//解方程组
 			p=(POINT2D*)malloc(nNeighbor*sizeof(POINT2D));
			solvePSEQ2(i,ang,nNeighbor, neighborPnt,p);
			m_tempP=p;
			//求Pj与Pi连接而成得直线方程PjPi
			//找出与PjPi相交的线段
			double dfuncRes1,dfuncRes2;
			double delta[3];
			for(j=0; j < nNeighbor; j++)
			{  
				double qq=fabs(p[j].x);
				double pp =fabs(p[j].y);
				if((fabs(p[j].y) > Infinite_Min ) && (fabs(p[j].x) > Infinite_Min)) 
				{
					for(k=0; k < nNeighbor; k++)
					{ 
						if((k!=j)&&(k+1)!=j)
						{
							dfuncRes1 = (p[k].y - 0)/(p[j].y - 0) -(p[k].x - 0)/(p[j].x-0);
							dfuncRes2 = (p[(k+1)%nNeighbor].y - 0)/(p[j].y - 0) -(p[(k+1)%nNeighbor].x - 0)/(p[j].x-0);
							if(((dfuncRes1*dfuncRes2) <= 0))
								break;
						}
					}
				}
				else if(fabs(p[j].x) <= Infinite_Min)
				{ 
					for(k=0; k < nNeighbor; k++)
					{ 
						if((k!=j)&&(k+1)!=j)
						{ 
							if(fabs(p[k].x) <=Infinite_Min) break;
							else
							{ 
								dfuncRes1 = p[k].x;
								dfuncRes2 = p[(k+1)%nNeighbor].x;
								if(((dfuncRes1*dfuncRes2) <= 0))
									break;
							}
						}
					}
				}
				else if(fabs(p[j].y) <= Infinite_Min)
				{ 
					for(k=0; k < nNeighbor; k++)
					{ 
						if((k!=j)&&(k+1)!=j)
						{ 
							if(fabs(p[k].y) <=Infinite_Min) break;
							else
							{ 
								dfuncRes1 = p[k].y;
								dfuncRes2 = p[(k+1)%nNeighbor].y;
								if(((dfuncRes1*dfuncRes2) <= 0))
									break;
							}
						}
					}
				}
				m_tempL=k;
				solveDelta2(j,delta);
 				u[j][j]=delta[0];
				u[k][j]=delta[1];
				u[(k+1)%nNeighbor][j]=delta[2];
			}
 			for(j = 0; j < nNeighbor; j++)
			{ 
				for(k = 0; k < nNeighbor; k++)
					w[nInPnts][neighborPnt[j]]+=u[j][k];
				w[nInPnts][neighborPnt[j]]=w[nInPnts][neighborPnt[j]]/(double)nNeighbor;
/*
				fprintf(fp,"%lf  ",w[nInPnts][neighborPnt[j]]);
*/
			}
/*
			fprintf(fp,"\n");
*/
			free(ang);
			ang = NULL;
			free(neighborPnt); 
			neighborPnt = NULL;
		 	free(p);
			p = NULL;
		    for(j= 0; j < nNeighbor; j++)
		 		free(u[j]);
		 	free(u);
			u = NULL;
			nInPnts++;
		}
	}

/*
	fclose(fp);
*/
}


void Mesh3D::ComputeMeanValueWeght(bool *bIsBound, double **w)
{
	int i,j,k; 
	int m ;
	int end1,end2;
	int TriIndex;
	int preEnd,tempEnd;
	int neighbor1, neighbor2;
	int *neighborPnt ;
	int nNeighbor;
	POINT3D v1,v2,v3;
	POINT2D *p;
	double *ang;
	double totalAng;
	//double **u;
	double *tempu;
	int nInPnts;
	double ab,bc,ac;
	double sinvalue,cosvalue,tanvalue1,tanvalue2;
	double sumW;

	nInPnts=0;

	for(i = 0; i < m_nPtSum+m_nAddPnt; i++)
	{ 
		if(!bIsBound[i])
		{ 
			m_currentCenterPnt =i;
			///////////////////////////////////
			//找出点i 的邻点，并按逆时针顺序排列
			neighborPnt = (int*)malloc(MAXNEIGHBORPOINT * sizeof(int));
			for(j=0; j < MAXNEIGHBORPOINT; j++ )
				neighborPnt[j]=-1;
			//找到与顶点i相连的两个点end1,end2
			for(j = 0; j < m_vEdge.size(); j++)
			{
				if(m_vEdge.at(j).start == i || m_vEdge.at(j).end == i)
				{ 
					TriIndex = m_vEdge.at(j).neighborTri[0];
					for(m=0; m < 3 ; m++)
					{ 
						if(meshFaces(TriIndex).vI[m] == i)
							break;
					}
					end1 = meshFaces(TriIndex).vI[(m+1)%3];
					end2 = meshFaces(TriIndex).vI[(m+2)%3];
					break;
				}
			}
			//从end2出发按逆时针方向寻找下一个i的邻接点
			neighborPnt[0]=end1;
			neighborPnt[1]=end2;
			tempEnd = end2;
			preEnd = end1;
			nNeighbor = 2;
			//直到回到顶点end1，说明已经搜索完一圈
			while(1)
			{ 
				for(j=0; j < m_vEdge.size(); j++)
				{ 
					if((m_vEdge.at(j).start == i && m_vEdge.at(j).end==tempEnd) ||
						(m_vEdge.at(j).start == tempEnd && m_vEdge.at(j).end ==i))
						break;
				}
				neighbor1 = m_vEdge.at(j).neighborTri[0];
				neighbor2 = m_vEdge.at(j).neighborTri[1];

				for(m=0; m < 3; m++)
				{ 
					if(meshFaces(neighbor1).vI[m] == preEnd) break;
				}
				if(m==3) //下一个三角形不是neighbor1，而是neighbor2
				   TriIndex = neighbor1;
				else //下一个三角形是neighbor1
					TriIndex = neighbor2;
				for(m =0; m < 3 ; m++)
				{ 
					if(meshFaces(TriIndex).vI[m] != i && meshFaces(TriIndex).vI[m]!=tempEnd)
						break;
				}
				preEnd = tempEnd;
				tempEnd = meshFaces(TriIndex).vI[m];
				if(tempEnd==end1)
					break;
				else
				{
					neighborPnt[nNeighbor] = tempEnd;
					nNeighbor++;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			ang=(double*)malloc(nNeighbor*sizeof(double));
			totalAng = 0 ;
			//计算Xjk, Xi, Xjk+1之间的夹角
			for(j=0; j < nNeighbor; j++)
			{ 
				v1=meshPnt(neighborPnt[j]);
				v2=meshPnt(i);
				v3=meshPnt(neighborPnt[(j+1)%nNeighbor]);
				ang[j]=ComputeAng(v1,v2,v3);
				totalAng += ang[j];
			}
			//权值初始化
			m_tempAng = ang;
			m_totalAng = totalAng;
			m_ntempNneighbor = nNeighbor;
			w[nInPnts]=(double*)malloc(sizeof(double)*(m_nPtSum+m_nAddPnt));
			for(j = 0; j < m_nPtSum + m_nAddPnt; j++)
				w[nInPnts][j] = 0;
			//////////////////////////////////////////////////////////////////////////
 			//解方程组得到顶点i的邻点在平面上的映射坐标
 			p=(POINT2D*)malloc(nNeighbor*sizeof(POINT2D));
			solvePSEQ2(i,ang,nNeighbor, neighborPnt,p);

			tempu=(double*)malloc(nNeighbor*sizeof(double));
			sumW=0.0;

			//计算权值--Mean Value Coordinates
			for(k=0; k < nNeighbor; k++)
			{ 
				//计算由i,k,k+1组成的三角形的alpha角
				 ab=sqrt((p[k].x-0)*(p[k].x-0)+
					 (p[k].y-0)*(p[k].y-0));

				 ac=sqrt((0-p[(k+1)%nNeighbor].x)*(0-p[(k+1)%nNeighbor].x)+
					 (0-p[(k+1)%nNeighbor].y)*(0-p[(k+1)%nNeighbor].y));

				 bc=sqrt((p[k].x-p[(k+1)%nNeighbor].x)*(p[k].x-p[(k+1)%nNeighbor].x)+
					 (p[k].y-p[(k+1)%nNeighbor].y)*(p[k].y-p[(k+1)%nNeighbor].y));
				 
				 cosvalue=(ab*ab+ac*ac-bc*bc)/(2*ab*ac);
				 sinvalue=sqrt(1-cosvalue*cosvalue);
				 tanvalue1=sinvalue/(1+cosvalue);
				//计算由i,k+1,k+2组成的三角形的alpha角
				ab=sqrt((p[(k+1)%nNeighbor].x-0)*(p[(k+1)%nNeighbor].x-0)+
					 (p[(k+1)%nNeighbor].y-0)*(p[(k+1)%nNeighbor].y-0));

				ac=sqrt((0-p[(k+2)%nNeighbor].x)*(0-p[(k+2)%nNeighbor].x)+
					 (0-p[(k+2)%nNeighbor].y)*(0-p[(k+2)%nNeighbor].y));

				bc=sqrt((p[(k+1)%nNeighbor].x-p[(k+2)%nNeighbor].x)*(p[(k+1)%nNeighbor].x-p[(k+2)%nNeighbor].x)+
				        (p[(k+1)%nNeighbor].y-p[(k+2)%nNeighbor].y)*(p[(k+1)%nNeighbor].y-p[(k+2)%nNeighbor].y));
				
				cosvalue=(ab*ab+ac*ac-bc*bc)/(2*ab*ac);
				sinvalue=sqrt(1-cosvalue*cosvalue);
				tanvalue2=sinvalue/(1+cosvalue);
				
				int tempIndex;
				tempIndex=(k+1)%nNeighbor;
				tempu[tempIndex]=(tanvalue1+tanvalue2)/ab;
				sumW+=tempu[tempIndex];
			}
			for(k=0; k < nNeighbor; k++)
			{ 
				w[nInPnts][neighborPnt[k]] = tempu[k]/sumW;
			}
			free(ang);
			ang = NULL;
			free(neighborPnt);
			neighborPnt = NULL;
		 	free(p);
			p = NULL;
			free(tempu);
			tempu = NULL;
			nInPnts++;
		}
	}
			
}

void Mesh3D::solvePSEQ2(int centerPoint, double *ang, int nNeighbor, int *neighborPnt, POINT2D *p)
{
	
	int i;
	double lfLen,lfAngle,sumAngle;
	double ang1;

	p[0].x = sqrt((meshPnt(neighborPnt[0]).x - meshPnt(m_currentCenterPnt).x)*(meshPnt(neighborPnt[0]).x - meshPnt(m_currentCenterPnt).x) +
				  (meshPnt(neighborPnt[0]).y - meshPnt(m_currentCenterPnt).y)*(meshPnt(neighborPnt[0]).y - meshPnt(m_currentCenterPnt).y) +
			 	  (meshPnt(neighborPnt[0]).z - meshPnt(m_currentCenterPnt).z)*(meshPnt(neighborPnt[0]).z - meshPnt(m_currentCenterPnt).z)); 
    p[0].y = 0;
	
	m_preP.x = p[0].x;
	m_preP.y = p[0].y;
	sumAngle=0;

	for(i=1; i < nNeighbor; i++)
	{ 
		m_tempPID = i;
		lfLen = sqrt((meshPnt(neighborPnt[i]).x - meshPnt(m_currentCenterPnt).x)*(meshPnt(neighborPnt[i]).x - meshPnt(m_currentCenterPnt).x) +
				     (meshPnt(neighborPnt[i]).y - meshPnt(m_currentCenterPnt).y)*(meshPnt(neighborPnt[i]).y - meshPnt(m_currentCenterPnt).y) +
			 	     (meshPnt(neighborPnt[i]).z - meshPnt(m_currentCenterPnt).z)*(meshPnt(neighborPnt[i]).z - meshPnt(m_currentCenterPnt).z)); 
		ang1=m_tempAng[(m_tempPID-1)]*2*PI;
		lfAngle = ang1/m_totalAng; 
		sumAngle = sumAngle + lfAngle;
		p[i].x = cos(sumAngle);
		p[i].y = sin(sumAngle);
	}
	
}


void Mesh3D::solveDelta2(int lIndex,double *delta)
{
	double *A;
	double *b;
	A = (double*)malloc(9 * sizeof(double));
	b = (double*)malloc(3 * sizeof(double));
	A[0] = 1.0;
	A[1] = 1.0;
	A[2] = 1.0;

	A[3] = m_tempP[lIndex].x;
	A[4] = m_tempP[m_tempL].x;
	A[5] = m_tempP[(m_tempL+1)%m_ntempNneighbor].x;

	A[6] = m_tempP[lIndex].y;
	A[7] = m_tempP[m_tempL].y;
	A[8] = m_tempP[(m_tempL+1)%m_ntempNneighbor].y;

	b[0] = 1.0;
	b[1] = 0.0;
	b[2] = 0.0;

	if(invers_matrix(A,3))
	{ 
		mult(A,b,delta,3,3,1);
	}
}

double Mesh3D::ComputeAng(POINT3D A, POINT3D B, POINT3D C)
{
	double ang;
	double dAB,dBC,dAC;
	double temp;

	dAB=sqrt((A.x-B.x)*(A.x-B.x) +
			  (A.y-B.y)*(A.y-B.y) +
			  (A.z-B.z)*(A.z-B.z));
	dBC = sqrt((C.x-B.x)*(C.x-B.x) +
			  (C.y-B.y)*(C.y-B.y) +
			  (C.z-B.z)*(C.z-B.z));
	dAC = sqrt((A.x-C.x)*(A.x-C.x) +
			  (A.y-C.y)*(A.y-C.y) +
			  (A.z-C.z)*(A.z-C.z));
	temp=(dAB*dAB + dBC*dBC -dAC*dAC)/(2*dAB*dBC);
	ang=acos(temp);
	return(ang);
}


double Mesh3D::ComputeAng2(POINT2D A, POINT2D B, POINT2D C)
{
	double ang;
	double dAB,dBC,dAC;
	double temp;

	dAB = sqrt((A.x-B.x)*(A.x-B.x) +
			  (A.y-B.y)*(A.y-B.y)) ;
 	dBC = sqrt((C.x-B.x)*(C.x-B.x) +
			  (C.y-B.y)*(C.y-B.y) );
 	dAC = sqrt((A.x-C.x)*(A.x-C.x) +
			  (A.y-C.y)*(A.y-C.y));
 	temp=(dAB*dAB + dBC*dBC -dAC*dAC)/(2*dAB*dBC);
	ang=acos(temp);
	return(ang);
}


//DEL void Mesh3D::solveP(int centerPoint, double *ang, int nNeighbor, int *neighborPnt, POINT2D *p)
//DEL {
//DEL 	int     m ;       //点数
//DEL     int     n ;      //未知参数的个数；
//DEL     //double  ix[NPARAS];
//DEL 	double  *ix;
//DEL 	int i;
//DEL 
//DEL 	//m = 2*(nNeighbor -1);
//DEL 	m=2 * nNeighbor;
//DEL 	n = m;
//DEL 	ix= (double*)malloc(n * sizeof(double));
//DEL 
//DEL     double *fvec;
//DEL     double  ftol = REL_SENSOR_TOLERANCE_ftol;
//DEL     double  xtol = REL_PARAM_TOLERANCE_xtol;
//DEL     double  gtol = ORTHO_TOLERANCE_gtol;
//DEL     int     maxfev = MAXFEV;
//DEL     double  epsfcn = EPSFCN;
//DEL     double  diag[NPARAS]={0};
//DEL     int     mode = MODE;
//DEL     double  factor = FACTOR;
//DEL     int     nprint = 0;
//DEL     int     info;
//DEL     int     nfev;
//DEL     double  *fjac;
//DEL     int     ldfjac = m;
//DEL     int     ipvt[NPARAS]={0};
//DEL     double  qtf[NPARAS]={0};
//DEL     double  wa1[NPARAS]={0};
//DEL     double  wa2[NPARAS]={0};
//DEL     double  wa3[NPARAS]={0};
//DEL     double  *wa4;
//DEL 
//DEL     /* allocate some workspace */
//DEL     if (( fvec = (double *) calloc ( (unsigned int) m, (unsigned int) sizeof(double) )) == NULL ) {
//DEL 		fprintf(stderr,"calloc: Cannot allocate workspace fvec\n");
//DEL 		exit(-1);
//DEL     }
//DEL     if (( fjac = (double *) calloc ( (unsigned int) m*n, (unsigned int) sizeof(double) )) == NULL ) {
//DEL 		fprintf(stderr,"calloc: Cannot allocate workspace fjac\n");
//DEL 		exit(-1);
//DEL     }
//DEL     if (( wa4 = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double) )) == NULL ) {
//DEL 		fprintf(stderr,"calloc: Cannot allocate workspace wa4\n");
//DEL 		exit(-1);
//DEL     }
//DEL 	if ( mode == 2 )
//DEL 	{
//DEL 		for (int i = 0; i < NPARAS; i++)
//DEL 			diag[i] = 1.0;             /* some user-defined values */
//DEL     }
//DEL 
//DEL 	//为未知参数赋初值：
//DEL 	for(i =0 ; i < n ; i=i+2)
//DEL 	{ 
//DEL 		ix[i] = m_pPts[m_ppConnect[m_currentCenterPnt][(i/2)]].x - m_pPts[m_currentCenterPnt].x;
//DEL 		ix[i+1] = m_pPts[m_ppConnect[m_currentCenterPnt][(i/2)]].y - m_pPts[m_currentCenterPnt].y;
//DEL 	}
//DEL 
//DEL 	//
//DEL 	minpack_non    nonlinear; 
//DEL     nonlinear.lmdif_(cc_full_error,
//DEL 		&m, &n,ix, fvec, &ftol, &xtol, &gtol, &maxfev, &epsfcn,
//DEL 		diag, &mode, &factor, &nprint, &info, &nfev, fjac, &ldfjac,
//DEL 		ipvt, qtf, wa1, wa2, wa3, wa4);	
//DEL  	for(i = 0; i < nNeighbor; i++)
//DEL 	{ 
//DEL 		p[i].x = ix[i*2];
//DEL 		p[i].y = ix[i*2+1];
//DEL 	}
//DEL 
//DEL }


//DEL void Mesh3D::cc_full_error(int *m_ptr, int *n_ptr, double *params, double *err)
//DEL {
//DEL 	int k;
//DEL 	int tempNeighbor;
//DEL 	POINT2D A, B,C;
//DEL 	double tempAng;
//DEL 	double t1;
//DEL 
//DEL 	for(k = 0; k < (*m_ptr) ; k=k+2)
//DEL 	{ 
//DEL  		tempNeighbor = m_ppConnect[m_currentCenterPnt][(k/2)];
//DEL 		
//DEL 		A.x = params[k];
//DEL 		A.y = params[k+1];
//DEL 		B.x = 0;
//DEL 		B.y = 0;
//DEL  		C.x = params[(k+2)%(*m_ptr)];
//DEL 		C.y = params[(k+3)%(*m_ptr)];
//DEL 
//DEL  		tempAng = ComputeAng2(A,B,C);
//DEL 		t1 = m_tempAng[(k/2)] * 2 * PI;
//DEL 		err[k] =sqrt( params[k]*params[k] + params[k+1]*params[k+1])-
//DEL 			    sqrt((meshPnt(tempNeighbor).x - meshPnt(m_currentCenterPnt).x)*(meshPnt(tempNeighbor).x - meshPnt(m_currentCenterPnt).x)+
//DEL 				     (meshPnt(tempNeighbor).y - meshPnt(m_currentCenterPnt).y)*(meshPnt(tempNeighbor).y - meshPnt(m_currentCenterPnt).y)+
//DEL 				     (meshPnt(tempNeighbor).z - meshPnt(m_currentCenterPnt).z)*(meshPnt(tempNeighbor).z - meshPnt(m_currentCenterPnt).z));
//DEL 		err[k+1] = tempAng - (t1/m_totalAng);
//DEL 	}
//DEL }

//DEL void Mesh3D::solveDelta(double *delta)
//DEL {
//DEL    int     m ;       //点数
//DEL     int     n ;      //未知参数的个数；
//DEL 	double  *ix;
//DEL 	int i;
//DEL 
//DEL 	m = 3;
//DEL 	n = 3;
//DEL 	ix= (double*)malloc(n * sizeof(double));
//DEL 
//DEL     double *fvec;
//DEL     double  ftol = REL_SENSOR_TOLERANCE_ftol;
//DEL     double  xtol = REL_PARAM_TOLERANCE_xtol;
//DEL     double  gtol = ORTHO_TOLERANCE_gtol;
//DEL     int     maxfev = MAXFEV;
//DEL     double  epsfcn = EPSFCN;
//DEL     double  diag[NPARAS]={0};
//DEL     int     mode = MODE;
//DEL     double  factor = FACTOR;
//DEL     int     nprint = 0;
//DEL     int     info;
//DEL     int     nfev;
//DEL     double  *fjac;
//DEL     int     ldfjac = m;
//DEL     int     ipvt[NPARAS]={0};
//DEL     double  qtf[NPARAS]={0};
//DEL     double  wa1[NPARAS]={0};
//DEL     double  wa2[NPARAS]={0};
//DEL     double  wa3[NPARAS]={0};
//DEL     double  *wa4;
//DEL 
//DEL     /* allocate some workspace */
//DEL     if (( fvec = (double *) calloc ( (unsigned int) m, (unsigned int) sizeof(double) )) == NULL ) {
//DEL 		fprintf(stderr,"calloc: Cannot allocate workspace fvec\n");
//DEL 		exit(-1);
//DEL     }
//DEL     if (( fjac = (double *) calloc ( (unsigned int) m*n, (unsigned int) sizeof(double) )) == NULL ) {
//DEL 		fprintf(stderr,"calloc: Cannot allocate workspace fjac\n");
//DEL 		exit(-1);
//DEL     }
//DEL     if (( wa4 = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double) )) == NULL ) {
//DEL 		fprintf(stderr,"calloc: Cannot allocate workspace wa4\n");
//DEL 		exit(-1);
//DEL     }
//DEL 	if ( mode == 2 )
//DEL 	{
//DEL 		for (int i = 0; i < NPARAS; i++)
//DEL 			diag[i] = 1.0;             /* some user-defined values */
//DEL     }
//DEL 
//DEL 	//为未知参数赋初值：
//DEL 	for(i =0 ; i < n ; i++)
//DEL 	{ 
//DEL 		ix[i] = (double)1.0/3.0;
//DEL 	}
//DEL 
//DEL 	//
//DEL 	minpack_non    nonlinear; 
//DEL     nonlinear.lmdif_(full_error_2,
//DEL 		&m, &n,ix, fvec, &ftol, &xtol, &gtol, &maxfev, &epsfcn,
//DEL 		diag, &mode, &factor, &nprint, &info, &nfev, fjac, &ldfjac,
//DEL 		ipvt, qtf, wa1, wa2, wa3, wa4);	
//DEL 	delta[0] = ix[0];
//DEL 	delta[1] = ix[1];
//DEL 	delta[2] = ix[2];
//DEL 
//DEL }

//DEL void Mesh3D::full_error_2(int *m_ptr, int *n_ptr, double *params, double *err)
//DEL {
//DEL  	err[0] = params[0] + params[1] + params[2]-1;
//DEL 	
//DEL 	err[1] = params[0] * m_tempP[m_currentCenterPnt].x +
//DEL 			 params[1] * m_tempP[m_tempL].x +
//DEL 			 params[2] * m_tempP[(m_tempL+1)%m_ntempNneighbor].x;
//DEL 
//DEL 	err[2] = params[0] * m_tempP[m_currentCenterPnt].y+
//DEL 			 params[1] * m_tempP[m_tempL].y +
//DEL 			 params[2] * m_tempP[(m_tempL+1)%m_ntempNneighbor].y;		     
//DEL }


//DEL void Mesh3D::solvePSEQ(int centerPoint, double *ang, int nNeighbor, int *neighborPnt, POINT2D *p)
//DEL {
//DEL 	int     m ;       //点数
//DEL     int     n ;      //未知参数的个数；
//DEL     //double  ix[NPARAS];
//DEL 	double  *ix;
//DEL 	int i;
//DEL 
//DEL 	m = 2;
//DEL 	n = 2;
//DEL 	ix= (double*)malloc(n * sizeof(double));
//DEL 
//DEL     double *fvec;
//DEL     double  ftol = REL_SENSOR_TOLERANCE_ftol;
//DEL     double  xtol = REL_PARAM_TOLERANCE_xtol;
//DEL     double  gtol = ORTHO_TOLERANCE_gtol;
//DEL     int     maxfev = MAXFEV;
//DEL     double  epsfcn = EPSFCN;
//DEL     double  diag[NPARAS]={0};
//DEL     int     mode = MODE;
//DEL     double  factor = FACTOR;
//DEL     int     nprint = 0;
//DEL     int     info;
//DEL     int     nfev;
//DEL     double  *fjac;
//DEL     int     ldfjac = m;
//DEL     int     ipvt[NPARAS]={0};
//DEL     double  qtf[NPARAS]={0};
//DEL     double  wa1[NPARAS]={0};
//DEL     double  wa2[NPARAS]={0};
//DEL     double  wa3[NPARAS]={0};
//DEL     double  *wa4;
//DEL 
//DEL     /* allocate some workspace */
//DEL     if (( fvec = (double *) calloc ( (unsigned int) m, (unsigned int) sizeof(double) )) == NULL ) {
//DEL 		fprintf(stderr,"calloc: Cannot allocate workspace fvec\n");
//DEL 		exit(-1);
//DEL     }
//DEL     if (( fjac = (double *) calloc ( (unsigned int) m*n, (unsigned int) sizeof(double) )) == NULL ) {
//DEL 		fprintf(stderr,"calloc: Cannot allocate workspace fjac\n");
//DEL 		exit(-1);
//DEL     }
//DEL     if (( wa4 = (double *) calloc ((unsigned int) m, (unsigned int) sizeof(double) )) == NULL ) {
//DEL 		fprintf(stderr,"calloc: Cannot allocate workspace wa4\n");
//DEL 		exit(-1);
//DEL     }
//DEL 	if ( mode == 2 )
//DEL 	{
//DEL 		for ( i = 0; i < NPARAS; i++)
//DEL 			diag[i] = 1.0;             /* some user-defined values */
//DEL     }
//DEL 
//DEL 	//////////////////////////////////////////////////////////////////////////
//DEL 	//////////////////////////////////////////////////////////////////////////
//DEL 	//////////////////////////////////////////////////////////////////////////
//DEL 	p[0].x = sqrt((meshPnt(neighborPnt[0]).x - meshPnt(m_currentCenterPnt).x)*(meshPnt(neighborPnt[0]).x - meshPnt(m_currentCenterPnt).x) +
//DEL 				  (meshPnt(neighborPnt[0]).y - meshPnt(m_currentCenterPnt).y)*(meshPnt(neighborPnt[0]).y - meshPnt(m_currentCenterPnt).y) +
//DEL 			 	  (meshPnt(neighborPnt[0]).z - meshPnt(m_currentCenterPnt).z)*(meshPnt(neighborPnt[0]).z - meshPnt(m_currentCenterPnt).z)); 
//DEL     p[0].y = 0;
//DEL 	
//DEL 	m_preP.x = p[0].x;
//DEL 	m_preP.y = p[0].y;
//DEL 
//DEL 	for(i=1; i < nNeighbor; i++)
//DEL 	{ 
//DEL 		m_tempPID = i;
//DEL 		//////////////////////////////////////////////////////////////////////////
//DEL 		///?????如何赋初值?
//DEL 		///??????????????????????????????????????????
//DEL 		
//DEL 		minpack_non    nonlinear; 
//DEL 		nonlinear.lmdif_(cc_full_error_1,
//DEL 		&m, &n,ix, fvec, &ftol, &xtol, &gtol, &maxfev, &epsfcn,
//DEL 		diag, &mode, &factor, &nprint, &info, &nfev, fjac, &ldfjac,
//DEL 		ipvt, qtf, wa1, wa2, wa3, wa4);	
//DEL 
//DEL 		p[i].x = ix[0];
//DEL 		p[i].y = ix[1];
//DEL 		m_preP.x= ix[0];
//DEL 		m_preP.y = ix[1];
//DEL 	}
//DEL }




//DEL void Mesh3D::cc_full_error_1(int *m_ptr, int *n_ptr, double *params, double *err)
//DEL {
//DEL 	POINT2D A,B,C;
//DEL 	double t1;
//DEL 	double tempAng;
//DEL 
//DEL 	A.x = m_preP.x;
//DEL 	A.y = m_preP.y;
//DEL 	B.x = 0;
//DEL 	B.y = 0;
//DEL 	 
//DEL 	C.x = params[0];
//DEL 	C.y = params[1]; 
//DEL 
//DEL  	tempAng = ComputeAng2(A,B,C);
//DEL 	t1 = m_tempAng[m_tempPID-1] * 2 * PI;
//DEL 	err[0] =sqrt( params[0]*params[0] + params[1]*params[1])-
//DEL 		    sqrt((meshPnt(m_tempPID).x - meshPnt(m_currentCenterPnt).x)*(meshPnt(m_tempPID).x - meshPnt(m_currentCenterPnt).x)+
//DEL 			     (meshPnt(m_tempPID).y - meshPnt(m_currentCenterPnt).y)*(meshPnt(m_tempPID).y - meshPnt(m_currentCenterPnt).y)+
//DEL 				 (meshPnt(m_tempPID).z - meshPnt(m_currentCenterPnt).z)*(meshPnt(m_tempPID).z - meshPnt(m_currentCenterPnt).z));
//DEL 	err[1] = tempAng - (t1/m_totalAng);
//DEL }


bool Mesh3D::OutPutCutPath(CString sstring)
{
 
	CStdioFile outFile;
	CString str=sstring+".cut";
	int nIsSBound;
	outFile.Open(str,CFile::modeCreate|CFile::modeWrite);
	if(!outFile)
	{
		AfxMessageBox("Create File Error!");
		return(true);
	}
 	str=sstring.Right(sstring.GetLength() - sstring.ReverseFind('\\')-1);
	str+="\n";
	outFile.WriteString(str);
	str.Format("%d\n",m_vCutPath.size());
	outFile.WriteString(str);
	for(int m=0;m<m_vCutPath.size();m++)
	{
		if(m_vCutPath.at(m).bIsBound==false)
			nIsSBound = 0;
		else
			nIsSBound = 1;
		str.Format("%d %d %d\n",m_vCutPath[m].start,m_vCutPath[m].end,nIsSBound);
		outFile.WriteString(str);
	}
	outFile.Close(); 
	return(true);
}

void Mesh3D::ReadInCut(CString csCutFileName,CString csPlyFileName)
{
	CStdioFile cutFile;
	cutFile.Open(csCutFileName,CFile::modeRead);		
	CString str,cut_file_name,ply_file_name,temp;
	int nstart, nend;
	int bIsBound;
	EDGE_ tempCutpath; 
	cutFile.ReadString(cut_file_name);
	ply_file_name=csPlyFileName.Right(csPlyFileName.GetLength() - csPlyFileName.ReverseFind('\\')-1);
	ply_file_name = ply_file_name.Left(ply_file_name.GetLength()-4);
	if(cut_file_name!=ply_file_name)
	{ 
		AfxMessageBox("the cut file does not match the ply file!");
		return;
	}
	cutFile.ReadString(str);
    
	str.TrimLeft();
	str.TrimRight();
	UINT num;
	num=atoi(str);

	while(cutFile.ReadString(str))
	{
		temp=str.Left(str.Find(' '));
		nstart=atoi(temp);
		str.Delete(0,str.Find(' ')+1);
		str.TrimLeft();
		temp=str.Left(str.Find(' '));
		nend=atoi(temp);
		str.Delete(0,str.Find(' ')+1);
		str.TrimLeft();
		str.TrimRight();
		bIsBound=atoi(str);
		tempCutpath.start = nstart;
		tempCutpath.end = nend;
		tempCutpath.bIsBound = bool(bIsBound); 
		tempCutpath.P[0] = meshPnt(nstart);
		tempCutpath.P[1] = meshPnt(nend);
		tempCutpath.length = sqrt((tempCutpath.P[0].x-tempCutpath.P[1].x)*(tempCutpath.P[0].x-tempCutpath.P[1].x)
								+(tempCutpath.P[0].y-tempCutpath.P[1].y)*(tempCutpath.P[0].y-tempCutpath.P[1].y)
								+(tempCutpath.P[0].z-tempCutpath.P[1].z)*(tempCutpath.P[0].z-tempCutpath.P[1].z)); 
	//	tempCutpath.length= lf2sf(tempCutpath.length);
		m_vCutPath.push_back(tempCutpath);
	}
	m_vStrCutPath=m_vCutPath;

}

void Mesh3D::CreateNewFaces()
{
	int i,j,k,m;
	int nCnt;
	int pntIndex;
	newFACE tempNFace;
	int nBndPntIndex;
	bool flag1,flag2;
	int preIndex,nextIndex;
	int vertex1,vertex2;
/*

 	FILE *fp;
	fp=fopen("newfaces.txt","w+");
	
	for(i = 0; i < m_nFaceSum+m_nAddFaces; i++)
	{ 
		fprintf(fp,"%d --- %d   %d   %d\n",i, meshFaces(i).vI[0],meshFaces(i).vI[1],meshFaces(i).vI[2]);
	}

	fclose(fp);
*/


	m_vNewFaces.clear();

	for(i = 0; i < m_nFaceSum+m_nAddFaces; i++)
	{ 
		for(j =0; j < 3; j++)
		{ 
			nCnt = 0;
			pntIndex = meshFaces(i).vI[j];
			for(k = 0; k < m_vBndPnt.size(); k++)
			{
				if(m_vBndPnt.at(k).pointIndex==pntIndex)  
				{
					nCnt++;
					nBndPntIndex=k;
				}
			}
			if(nCnt==0) //说明该顶点在内部
			{ 
				for(m=0; m < m_vInMapPnt.size(); m++)
				{ 
					if(m_vInMapPnt.at(m).pointIndex==pntIndex)
						break;
				}
				tempNFace.flag[j] = 0;
				tempNFace.index[j] = m;
			}
			else if(nCnt ==1)
			{ 
				tempNFace.flag[j] = 1;
				tempNFace.index[j] = nBndPntIndex; 
			}
			else if(nCnt >=2)
			{ 
				for(k = 0; k < m_vBndPnt.size(); k++)	
				{ 
					if(m_vBndPnt.at(k).pointIndex == pntIndex)
					{ 
						flag1 = false;
						flag2 = false;
						//////////////////////////////////////////////////////////////////////////
						preIndex = ((k-1)>=0)?(k-1):(k-1+m_vBndPnt.size());
						nextIndex = (k+1)%(m_vBndPnt.size());
						preIndex = m_vBndPnt.at(preIndex).pointIndex;
						nextIndex = m_vBndPnt.at(nextIndex).pointIndex;
						vertex1=meshFaces(i).vI[(j+1)%3];
						if(nextIndex==vertex1)
						   flag1=true;
						else
						{
							for(m=0; m < MAXNEIGHBORPOINT; m++)
							{ 
								if(m_vBndPnt.at(k).neighborPnt[m]==vertex1)
									flag1=true;
							}
						}
						
						vertex2=meshFaces(i).vI[(j+2)%3];
						if(preIndex==vertex2)
						   flag2=true;
						else
						{
							for(m=0; m < MAXNEIGHBORPOINT; m++)
							{ 
								if(m_vBndPnt.at(k).neighborPnt[m]==vertex2)
									flag2=true;
							}
						}
						if(flag1==true&&flag2==true)
						{ 
							tempNFace.flag[j] = 1;
							tempNFace.index[j] = k; 
							break;
						}
					}
				}
			}
		}
		m_vNewFaces.push_back(tempNFace);
	}
	/*
	FILE* fp2;
	fp2=fopen("newface2.txt","w+");
	for(i=0; i < m_vNewFaces.size(); i++)
	{ 
		fprintf(fp2,"(%d)%d  ", m_vNewFaces.at(i).flag[0],m_vNewFaces.at(i).index[0]);
		fprintf(fp2,"(%d)%d  ", m_vNewFaces.at(i).flag[1],m_vNewFaces.at(i).index[1]);
		fprintf(fp2,"(%d)%d  ", m_vNewFaces.at(i).flag[2],m_vNewFaces.at(i).index[2]);
		fprintf(fp2,"\n");
	}
	fclose(fp2);
	*/
}

void Mesh3D::CreateGeoImage()
{
	int i,j,k,m;
	POINT2D p[3];
	POINT3D q[3];
	double tempT[3],tempR[3];
	double a[9],b[9],x[9];
	int tempIndex;
	double left, right,up,bottom;
	int nl,nr,nu,nb;
	int nJointCnt;
	bool flag1,flag2;
	double currenty,currentx,jointx[3],leftJointx,rightJointx;
	
	FILE* fp;
	fp = fopen("3d.txt","w+");

	if(m_vNewFaces.size()==0)
	{
		AfxMessageBox("the mesh points have not been embeded!");
		return;
	}

	m_pGeoMap = (MapPoint**)malloc((NGRID+1)*sizeof(MapPoint*));
	for(i=0; i < NGRID+1; i++)
	{
	 
		m_pGeoMap[i] = (MapPoint*)malloc((NGRID+1)*sizeof(MapPoint));
		for(j = 0; j < NGRID+1; j++)
		{
			m_pGeoMap[i][j].fx =0;
			m_pGeoMap[i][j].fy =0;
			m_pGeoMap[i][j].fz =0;
			m_pGeoMap[i][j].p.x = (double)j/NGRID;
			m_pGeoMap[i][j].p.y = (double)i/NGRID;
			m_pGeoMap[i][j].pointIndex=-1;
		}
	}

	int xx;

	for(i=0; i < m_vNewFaces.size(); i++)
	{ 
		//////////////////////////////////////////////////////////////////////////
		for(j=0; j < 3; j++)
		{
			tempIndex = m_vNewFaces.at(i).index[j];
			
			if(m_vNewFaces.at(i).flag[j]==0)
			{
				p[j] = m_vInMapPnt.at(tempIndex).p;
				xx=m_vInMapPnt.at(tempIndex).pointIndex;

				q[j] = meshPnt(xx);
			}
			else
			{
				p[j]= m_vBndMapPnt.at(tempIndex).p;
				
				q[j] = meshPnt(m_vBndMapPnt.at(tempIndex).pointIndex);
			}
		}

		/*
		for(j=0; j < 3; j++)
		{ 
			if(m_vNewFaces.at(i).flag[j]==1&&m_vNewFaces.at(i).flag[(j+1)%3]==1)
			{ 
				if(p[j].y==p[(j+1)%3].y && p[j].x==0)
				{ 
					if(m_vNewFaces.at(i).index[j]>m_vNewFaces.at(i).index[(j+1)%3])
						p[j].y+=1/(double)NGRID;
					else
						p[(j+1)%3].y+=1/(double)NGRID;
				}
				else if(p[j].x==p[(j+1)%3].x && p[j].y==1)
				{
					if(m_vNewFaces.at(i).index[j]>m_vNewFaces.at(i).index[(j+1)%3])
						p[j].x+=1/(double)NGRID;
					else
						p[(j+1)%3].x+=1/(double)NGRID;
				}
				else if(p[j].y==p[(j+1)%3].y && p[j].x==1)
				{ 
					if(m_vNewFaces.at(i).index[j]>m_vNewFaces.at(i).index[(j+1)%3])
						p[j].y-=1/(double)NGRID;
					else
						p[(j+1)%3].y-=1/(double)NGRID;
				}
				else if(p[j].x==p[(j+1)%3].x && p[j].y==0)
				{ 
					if(m_vNewFaces.at(i).index[j]>m_vNewFaces.at(i).index[(j+1)%3])
						p[j].x-=1/(double)NGRID;
					else
						p[(j+1)%3].x-=1/(double)NGRID;
				}

			}
		}
		*/
		//////////////////////////////////////////////////////////////////////////
		for(j=0 ;  j <3; j++)
		{ 
			a[j*3+0]=p[j].x;
			a[j*3+1]=p[j].y;
			a[j*3+2]=1.0;

			b[j*3+0]=q[j].x;
			b[j*3+1]=q[j].y;
			b[j*3+2]=q[j].z;
		}
		if(invers_matrix(a,3))
		{ 
			mult(a,b,x,3,3,3);
		}
		//////////////////////////////////////////////////////////////////////////
		left=2;
		right=-1;
		up=-1;
		bottom=2;
		for(j=0; j < 3; j++)
		{ 
			if(p[j].x > right) right = p[j].x;
			if(p[j].x < left)  left = p[j].x;
			if(p[j].y > up)    up = p[j].y;
			if(p[j].y < bottom) bottom = p[j].y;
		}
		nb = ceil(bottom*NGRID);
		nl = ceil(left*NGRID);
		nu = floor(up*NGRID);
		nr = floor(right*NGRID);
		if(nb<0)
			nb=0;
		if(nl<0)
			nl=0;
		if(nu>NGRID)
			nu=NGRID;
		if(nr>NGRID)
			nr=NGRID;

		for(k=nb; k <=nu; k++)
		{ 
			currenty=(double)k/double(NGRID);
			nJointCnt=0;
			for(j=0; j <3 ; j++)
			{ 
					if(i==393)
					{ 
						currenty=currenty;
					}

				flag1 = (p[j].y >=currenty)?true:false;
				flag2 = (p[(j+1)%3].y >= currenty)?true:false;
				if(flag1!=flag2)
				{ 
					if(p[(j+1)%3].y!=p[j].y)
						jointx[nJointCnt++]=(currenty - p[j].y)*(p[(j+1)%3].x-p[j].x)/(p[(j+1)%3].y-p[j].y) + p[j].x;
					else
						jointx[nJointCnt++] = p[j+1].x;
				}
				else if(p[j].y==currenty)
				{ 
					if(p[(j+1)%3].y!=p[j].y)
				    	jointx[nJointCnt++]=(currenty - p[j].y)*(p[(j+1)%3].x-p[j].x)/(p[(j+1)%3].y-p[j].y) + p[j].x;
					else
						jointx[nJointCnt++] = (p[(j+1)%3].x > p[j].x)?p[(j+1)%3].x:p[j].x;

				}
			}
			if(nJointCnt==3)
			{
				if(jointx[0]==jointx[1])
				  jointx[1]=jointx[2];
			}
			if(nJointCnt ==1)
				jointx[1] = jointx[0];
			if(jointx[0]>jointx[1])
			{ 
				leftJointx=jointx[1];
				rightJointx = jointx[0];
			}
			else
			{ 
				leftJointx=jointx[0];
				rightJointx = jointx[1];
			}
			//////////////////////////////////////////////////////////////////////////
			for(m = nl; m <= nr; m++)
			{ 
			//	ASSERT(k<101);
			//	ASSERT(m<101);


				currentx = double(m)/double(NGRID);
				if(currentx >=leftJointx && currentx <= rightJointx)
				{ 
					tempT[0] = currentx;
					tempT[1] = currenty;
					tempT[2] = 1;
					mult(tempT,x,tempR,1,3,3);
					//if((tempR[0]*tempR[0]+tempR[1]*tempR[1]+tempR[2]*tempR[2]) < 1.2)
					if(tempR[0] >10.0)
						tempR[0] = tempR[0];
					fprintf(fp,"%lf %lf %lf\n",tempR[0],tempR[1],tempR[2]);
					m_pGeoMap[k][m].fx = tempR[0];
					m_pGeoMap[k][m].fy = tempR[1];
					m_pGeoMap[k][m].fz = tempR[2];
					m_pGeoMap[k][m].p.x = currentx;
					m_pGeoMap[k][m].p.y = currenty;
					m_pGeoMap[k][m].pointIndex = -1;
				}
			}
		}
	}
	fclose(fp);

}

void Mesh3D::DisplayGeoImage()
{
	int i,j;
	double lfmax,lfmin;
	double t1,t2,t3;

	
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, 0.0f);
	glEnd();

 	glPointSize(5.0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable(GL_SMOOTH);
	
	lfmax = -100;
	lfmin = 100;
	for(i=0; i <=NGRID; i++)
		for(j = 0; j<=NGRID; j++)
		{ 
			if(m_pGeoMap[i][j].fx > lfmax)
				lfmax = m_pGeoMap[i][j].fx;
			if(m_pGeoMap[i][j].fy>lfmax)
				lfmax = m_pGeoMap[i][j].fy;
			if(m_pGeoMap[i][j].fz > lfmax)
				lfmax = m_pGeoMap[i][j].fz;
		
			if(m_pGeoMap[i][j].fx < lfmin)
				lfmin = m_pGeoMap[i][j].fx;
			if(m_pGeoMap[i][j].fy < lfmin)
				lfmin = m_pGeoMap[i][j].fy;
			if(m_pGeoMap[i][j].fz < lfmin)
				lfmin = m_pGeoMap[i][j].fz;
		}
	
		
	for(i = 0; i<NGRID ; i++)
		for(j = 0; j <NGRID ; j++)
		{ 
		   glBegin(GL_POLYGON);
		   t1=(m_pGeoMap[i][j].fx-lfmin)/(lfmax-lfmin);
		   t2=(m_pGeoMap[i][j].fy-lfmin)/(lfmax-lfmin);
		   t3=(m_pGeoMap[i][j].fz-lfmin)/(lfmax-lfmin);
		   glColor3f(t1,t2,t3);
		   glVertex2f(m_pGeoMap[i][j].p.x,m_pGeoMap[i][j].p.y);
		   
		   t1=(m_pGeoMap[i+1][j].fx-lfmin)/(lfmax-lfmin);
		   t2=(m_pGeoMap[i+1][j].fy-lfmin)/(lfmax-lfmin);
		   t3=(m_pGeoMap[i+1][j].fz-lfmin)/(lfmax-lfmin);
		   
		   glColor3f(t1,t2,t3);
		   glVertex2f(m_pGeoMap[i+1][j].p.x,m_pGeoMap[i+1][j].p.y);

		   t1=(m_pGeoMap[i+1][j+1].fx-lfmin)/(lfmax-lfmin);
		   t2=(m_pGeoMap[i+1][j+1].fy-lfmin)/(lfmax-lfmin);
		   t3=(m_pGeoMap[i+1][j+1].fz-lfmin)/(lfmax-lfmin);
		   glColor3f(t1,t2,t3);
		   glVertex2f(m_pGeoMap[i+1][j+1].p.x,m_pGeoMap[i+1][j+1].p.y);

		   t1=(m_pGeoMap[i][j+1].fx-lfmin)/(lfmax-lfmin);
		   t2=(m_pGeoMap[i][j+1].fy-lfmin)/(lfmax-lfmin);
		   t3=(m_pGeoMap[i][j+1].fz-lfmin)/(lfmax-lfmin);
		   
		   glColor3f(t1,t2,t3);
		   glVertex2f(m_pGeoMap[i][j+1].p.x,m_pGeoMap[i][j+1].p.y);
		   glEnd();	
		}

}

void Mesh3D::Reconstruct()
{
	int i,j;
	double lfAC,lfBD;
	TriAngle tempTri;

	m_vRemeshFaces.clear();

	for(i = 0; i < NGRID; i++)
		for(j = 0; j < NGRID; j++)
		{ 
			lfAC=sqrt((m_pGeoMap[i][j].p.x-m_pGeoMap[i+1][j+1].p.x)*(m_pGeoMap[i][j].p.x-m_pGeoMap[i+1][j+1].p.x)+
				(m_pGeoMap[i][j].p.y-m_pGeoMap[i+1][j+1].p.y)*(m_pGeoMap[i][j].p.y-m_pGeoMap[i+1][j+1].p.y));
			lfBD=sqrt((m_pGeoMap[i][j+1].p.x-m_pGeoMap[i+1][j].p.x)*(m_pGeoMap[i][j+1].p.x-m_pGeoMap[i+1][j].p.x)+
				(m_pGeoMap[i][j+1].p.y-m_pGeoMap[i+1][j].p.y)*(m_pGeoMap[i][j+1].p.y-m_pGeoMap[i+1][j].p.y));
			if(lfAC > lfBD)
			{ 
				tempTri.v1.x = m_pGeoMap[i][j].fx;
				tempTri.v1.y = m_pGeoMap[i][j].fy;
				tempTri.v1.z = m_pGeoMap[i][j].fz;

				tempTri.v2.x = m_pGeoMap[i][j+1].fx;
				tempTri.v2.y = m_pGeoMap[i][j+1].fy;
				tempTri.v2.z = m_pGeoMap[i][j+1].fz;

				tempTri.v3.x = m_pGeoMap[i+1][j].fx;
				tempTri.v3.y = m_pGeoMap[i+1][j].fy;
				tempTri.v3.z = m_pGeoMap[i+1][j].fz;
				m_vRemeshFaces.push_back(tempTri);

				tempTri.v1.x = m_pGeoMap[i][j+1].fx;
				tempTri.v1.y = m_pGeoMap[i][j+1].fy;
				tempTri.v1.z = m_pGeoMap[i][j+1].fz;

				tempTri.v2.x = m_pGeoMap[i+1][j+1].fx;
				tempTri.v2.y = m_pGeoMap[i+1][j+1].fy;
				tempTri.v2.z = m_pGeoMap[i+1][j+1].fz;

				tempTri.v3.x = m_pGeoMap[i+1][j].fx;
				tempTri.v3.y = m_pGeoMap[i+1][j].fy;
				tempTri.v3.z = m_pGeoMap[i+1][j].fz;
				m_vRemeshFaces.push_back(tempTri);
			}
			else
			{ 
				tempTri.v1.x = m_pGeoMap[i][j].fx;
				tempTri.v1.y = m_pGeoMap[i][j].fy;
				tempTri.v1.z = m_pGeoMap[i][j].fz;

				tempTri.v3.x = m_pGeoMap[i+1][j+1].fx;
				tempTri.v3.y = m_pGeoMap[i+1][j+1].fy;
				tempTri.v3.z = m_pGeoMap[i+1][j+1].fz;

				tempTri.v2.x = m_pGeoMap[i+1][j].fx;
				tempTri.v2.y = m_pGeoMap[i+1][j].fy;
				tempTri.v2.z = m_pGeoMap[i+1][j].fz;
				m_vRemeshFaces.push_back(tempTri);

				tempTri.v1.x = m_pGeoMap[i][j].fx;
				tempTri.v1.y = m_pGeoMap[i][j].fy;
				tempTri.v1.z = m_pGeoMap[i][j].fz;

				tempTri.v3.x = m_pGeoMap[i][j+1].fx;
				tempTri.v3.y = m_pGeoMap[i][j+1].fy;
				tempTri.v3.z = m_pGeoMap[i][j+1].fz;

				tempTri.v2.x = m_pGeoMap[i+1][j+1].fx;
				tempTri.v2.y = m_pGeoMap[i+1][j+1].fy;
				tempTri.v2.z = m_pGeoMap[i+1][j+1].fz;
				m_vRemeshFaces.push_back(tempTri);
			}
		}
}

void Mesh3D::DisplayReconstruct()
{
	int i;

	glColor3f(0.0f, 1.0f, 1.0f);
	glLineWidth(2.0);
	
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	for(i = 0; i < m_vRemeshFaces.size(); i++)
	{ 
		glBegin(GL_POLYGON);
			glVertex3f(m_vRemeshFaces.at(i).v1.x,m_vRemeshFaces.at(i).v1.y,m_vRemeshFaces.at(i).v1.z);
			glVertex3f(m_vRemeshFaces.at(i).v2.x,m_vRemeshFaces.at(i).v2.y,m_vRemeshFaces.at(i).v2.z);
			glVertex3f(m_vRemeshFaces.at(i).v3.x,m_vRemeshFaces.at(i).v3.y,m_vRemeshFaces.at(i).v3.z);
		glEnd();
	}
	glLineWidth(1.0f);
}

void Mesh3D::computeGeoStretch(double *geoS)
{
	int i,j;
	double Ss[3],St[3],L2;
	int t[3];
	double tx[3],ty[3];
	double a,b,c;
	int tempIndex;
	double *A;
	double* geoStretch;

	double lfSum1,lfSum2;

	geoStretch=(double*)malloc((m_vNewFaces.size())*sizeof(double));
	A = (double*)malloc(m_vNewFaces.size()*sizeof(double));

	for(i=0; i < m_vNewFaces.size(); i++)
	{
		A[i]=0;
		geoStretch[i]=0;
	}

	for(i=0; i<m_vNewFaces.size(); i++)
	{ 
		if(1)
		{
		if(m_vNewFaces.at(i).flag[0]==1 ||
			m_vNewFaces.at(i).flag[1]==1 ||
			m_vNewFaces.at(i).flag[2]==1)
			continue;
		}
		for(j=0; j < 3; j++)
		{ 
			if(m_vNewFaces.at(i).flag[j]==0)
			{
				tempIndex = m_vNewFaces.at(i).index[j];
				tx[j]=m_vInMapPnt.at(tempIndex).p.x;
				ty[j]=m_vInMapPnt.at(tempIndex).p.y;
				t[j]=m_vInMapPnt.at(tempIndex).pointIndex;
			}
			else if(m_vNewFaces.at(i).flag[j]==1)
			{ 
				tempIndex = m_vNewFaces.at(i).index[j];
				tx[j]=m_vBndMapPnt.at(tempIndex).p.x;
				ty[j]=m_vBndMapPnt.at(tempIndex).p.y;
				t[j]=m_vBndMapPnt.at(tempIndex).pointIndex;
			}
		}
		A[i]=((tx[1]-tx[0])*(ty[2]-ty[0])-(tx[2]-tx[0])*(ty[1]-ty[0]))/2;

		if(A[i]==0)
			A[i]=0;
	//	ASSERT(A[i]!=0);

		Ss[0]=((meshPnt(t[0]).x)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).x)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).x)*(ty[0]-ty[1]))/(2*A[i]);

		St[0]=((meshPnt(t[0]).x)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).x)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).x)*(tx[1]-tx[0]))/(2*A[i]);

		Ss[1]=((meshPnt(t[0]).y)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).y)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).y)*(ty[0]-ty[1]))/(2*A[i]);

		St[1]=((meshPnt(t[0]).y)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).y)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).y)*(tx[1]-tx[0]))/(2*A[i]);

		Ss[2]=((meshPnt(t[0]).z)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).z)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).z)*(ty[0]-ty[1]))/(2*A[i]);

		St[2]=((meshPnt(t[0]).z)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).z)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).z)*(tx[1]-tx[0]))/(2*A[i]);
		
		a=Ss[0]*Ss[0]+Ss[1]*Ss[1]+Ss[2]*Ss[2];
		b=Ss[0]*St[0]+Ss[1]*St[1]+Ss[2]*St[2];
		c=St[0]*St[0]+St[1]*St[1]+St[2]*St[2];

		L2=sqrt((a+c)/2);
		geoStretch[i]=L2;
	}

	lfSum1=0;
	lfSum2=0;
	for(i=0; i < m_vNewFaces.size(); i++)
	{ 
		lfSum1+=geoStretch[i]*geoStretch[i]*A[i];
		lfSum2+=A[i];
	}
	*geoS=sqrt(lfSum1/lfSum2);
	free(A);
}


int Mesh3D::FindMaxStrTri()
{
	int i,j;
	double Ss[3],St[3],L2;
	int t[3];
	double tx[3],ty[3];
	double a,b,c;
	int tempIndex;
	double *A;
	double *geoStretch;

	//double lfSum1,lfSum2;

	geoStretch=(double*)malloc(m_vNewFaces.size()*sizeof(double));
    A = (double*)malloc(m_vNewFaces.size()*sizeof(double));

	for(i=0; i<m_vNewFaces.size(); i++)
	{ 
		for(j=0; j < 3; j++)
		{ 
			if(m_vNewFaces.at(i).flag[j]==0)
			{
				tempIndex = m_vNewFaces.at(i).index[j];
				tx[j]=m_vInMapPnt.at(tempIndex).p.x;
				ty[j]=m_vInMapPnt.at(tempIndex).p.y;
				t[j]=m_vInMapPnt.at(tempIndex).pointIndex;
			}
			else if(m_vNewFaces.at(i).flag[j]==1)
			{ 
				tempIndex = m_vNewFaces.at(i).index[j];
				tx[j]=m_vBndMapPnt.at(tempIndex).p.x;
				ty[j]=m_vBndMapPnt.at(tempIndex).p.y;
				t[j]=m_vBndMapPnt.at(tempIndex).pointIndex;
			}
		}
		A[i]=((tx[1]-tx[0])*(ty[2]-ty[0])-(tx[2]-tx[0])*(ty[1]-ty[0]))/2;

		Ss[0]=((meshPnt(t[0]).x)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).x)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).x)*(ty[0]-ty[1]))/(2*A[i]);

		St[0]=((meshPnt(t[0]).x)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).x)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).x)*(tx[1]-tx[0]))/(2*A[i]);

		Ss[1]=((meshPnt(t[0]).y)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).y)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).y)*(ty[0]-ty[1]))/(2*A[i]);

		St[1]=((meshPnt(t[0]).y)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).y)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).y)*(tx[1]-tx[0]))/(2*A[i]);

		Ss[2]=((meshPnt(t[0]).z)*(ty[1]-ty[2])
			  +(meshPnt(t[1]).z)*(ty[2]-ty[0])
			  +(meshPnt(t[2]).z)*(ty[0]-ty[1]))/(2*A[i]);

		St[2]=((meshPnt(t[0]).z)*(tx[2]-tx[1])
			  +(meshPnt(t[1]).z)*(tx[0]-tx[2])
			  +(meshPnt(t[2]).z)*(tx[1]-tx[0]))/(2*A[i]);
		
		a=Ss[0]*Ss[0]+Ss[1]*Ss[1]+Ss[2]*Ss[2];
		b=Ss[0]*St[0]+Ss[1]*St[1]+Ss[2]*St[2];
		c=St[0]*St[0]+St[1]*St[1]+St[2]*St[2];

		L2=sqrt((a+c)/2);
		geoStretch[i]=L2;
	}

	double geoMax=-1000.0;
	int maxIndex;
	for(i=0; i < m_vNewFaces.size(); i++)
	{ 
		if(geoStretch[i]>geoMax) 
		{
			maxIndex =i;
			geoMax = geoStretch[i];
		}
	}
	free(A);
	A = NULL;
	return(maxIndex);
}
 
void Mesh3D::NewMeshRender()
{
	int i,j;
	int nv;

	for(i=0; i<m_nFaceSum+m_nAddFaces; i++)
	{
 		glBegin(GL_POLYGON);
		for(j=0; j < 3; j++)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			nv=meshFaces(i).vI[j];
			if(nv > m_nPtSum)
			{
				glPointSize(4.0);
				glColor3f(1.0f,0.0f, 0.0f);
			}
			glVertex3f(meshPnt(nv).x,meshPnt(nv).y,meshPnt(nv).z);
		}
		glEnd();
	}
	glPointSize(5.0);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
	for(i=m_nPtSum; i < m_nAddPnt+m_nPtSum; i++)
	{ 
		glVertex3f(meshPnt(i).x,meshPnt(i).y,meshPnt(i).z);
	}
	glEnd();
 
}



