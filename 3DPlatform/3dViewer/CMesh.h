 
#ifndef CMesh_h 
#define CMesh_h 1 

#include "CVertex.h"
#include "CTriangle.h"
  
class CLabelPts;
 
///for edges list:
struct EdgeNode
{
  int index;
  EdgeNode *pNext;		  
};
////////////////////
  
class CMesh 
{
public:
	
	  CMesh();
	  CMesh(const CMesh &mesh);
	  virtual ~CMesh();
			
	  //basic read
	  virtual bool Load(CString file);
	  BOOL LoadFromRaw (HANDLE hRaw, int w, int h);
	  BOOL LoadFromINI (CString strFile);
	  	
	  //basic write
      virtual BOOL Write (CString strFile);
	  BOOL    Write2INI (CString strFile);
	  		
	  //////////////////////////////////////////////////////////////////////////
	  //basic output: 
	  int     GetVertexNum() const;  //the number of vertices
	  void    GetVertex(float &x, float &y, float &z, int index) const; //the coordinates of vertex
	  int     GetTriangleNum() const;//the number of triangles
	  void    GetTriangleIndices(int &i, int &j, int &k, int index) const;//the index of three vertices of a triangle
	  int     GetEdgeNum() ;   //the number of edges
	  void    GetEdgeIndices(int &i, int &j, int index) const; //the indexs of two vertices of an edge
	  double  GetLenOfShortestEdge();
	  double  GetLenOfLongestEdge();
	  void    GetVertexNormal(float &nx, float &ny, float &nz, int index) const; // the normal of vertex
	  int     GetGenus() const;
   	         
	  //extended output:
	  virtual int  GetBorderLen();	  
      void    GetMesh (int* nTriangle,
		               int** nV1, int** nV2, int** nV3, 
					   int** c12, int** c23, int** c31,
					   int* nVlength, double** x, double** y, double** z, 
					   int* nEdgeNodeNum = NULL);
     
	  //manage edges
	  void    ExtractEdges();  //extract the edges information from triangles
	  void    ReleaseEdges();  //release the memory for edges;
	  
	  //
	  void    Normalize (float fScaleFactor = 1.0f);

	  //set the data
      virtual void    SetMesh( int nTriangle, int* nV1, int* nV2, int* nV3, 
		               DWORD* c12, DWORD* c23, DWORD* c31, 
					   double* ratio12, double* ratio23, double* ratio31, 
					   int nVlength, double* x, double* y, double* z, 
					   int nEdgeNodeNum, 
					   CString strAverage, CString strConviance, CString strGeoRatio);
	  
      void    SetMesh( int nVlength, double* x, double* y, double* z, 
		               int nTriangle = 0, 
					   int* nV1 = NULL, int* nV2 = NULL, int* nV3 = NULL);
      
	  void    SetLabelPts(const CLabelPts& labelPts ); //set label points
	  const   CLabelPts& GetLabel() const;

 	  double  ComputeScale(double inits);  //compute scale to make the area of mesh same as a unit sphere


//Attributes:
//protected:
      int m_nNumOfVertexs;
      int m_nNumOfTriangles;

      float   m_dW;
      double  m_dMeshScale;
      CArray< CTriangle,CTriangle > m_CTriangleArray; //triangles array
      CArray< CVertex,CVertex >     m_CVertexArray;   //vertices array
      CArray< CVertex,CVertex >     m_NormalArray;    //normals array
	  int      m_nEdgePointsNum;  //added by xdh, the number of edge points
	  
	  int      m_nType;           // the type of mesh: 0---3D; 1---2D;
	  int      m_nEdgeNum;
	  	  
private:
	  //for edge
	  EdgeNode  *m_pEdgeNodes;	   
	  //for Label Points
	  CLabelPts *m_pLabelPts;
};

void  InsertNode(EdgeNode *pHead, EdgeNode node);

#endif
