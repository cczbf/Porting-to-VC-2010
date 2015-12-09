//## begin module%3FCA8CD003E7.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3FCA8CD003E7.cm

//## begin module%3FCA8CD003E7.cp preserve=no
//## end module%3FCA8CD003E7.cp

//## Module: CMeshOpti%3FCA8CD003E7; Pseudo Package body
//## Source file: C:\zbf\Research\Spherical Para\Foundmentals of SP\fsp\CMeshOpti.cpp

//## begin module%3FCA8CD003E7.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%3FCA8CD003E7.additionalIncludes

//## begin module%3FCA8CD003E7.includes preserve=yes
#include "math.h"
#include "float.h"
#include "readply.h"
#include "ply.h"
#include "CGlobals.h"
#include <stdio.h>

#include"CMesh.h"

//## end module%3FCA8CD003E7.includes

// CMeshOpti
#include "CMeshOpti.h"

//## begin module%3FCA8CD003E7.additionalDeclarations preserve=yes
typedef struct Vertex1 {
  float x,y,z;             /* the usual 3-space position of a vertex */
} Vertex1;

typedef struct Face1 {
  unsigned char intensity; /* this user attaches intensity to faces */
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
} Face1;

void mnewt(int ntrial, float x[], int n, float tolx, float tolf);
void mnewt_global(int ntrial, float x[], int n, float tolx, float tolf);

/* polygon description of an object (a cube) */

Vertex1 verts1[] = {  /* vertices */
  { 0.0, 0.0, 0.0},
  { 1.0, 0.0, 0.0},
  { 1.0, 1.0, 0.0},
  { 0.0, 1.0, 0.0},
  { 0.0, 0.0, 1.0},
  { 1.0, 0.0, 1.0},
  { 1.0, 1.0, 1.0},
  { 0.0, 1.0, 1.0},
};

Face1 faces1[] = {  /* faces */
  { '\001', 4, NULL },  /* intensity, vertex list count, vertex list (empty) */
  { '\004', 4, NULL },
  { '\010', 4, NULL },
  { '\020', 4, NULL },
  { '\144', 4, NULL },
  { '\377', 4, NULL },
};

/* list of vertices for each face */
/* (notice that indices begin at zero) */

typedef int Vertex_Indices[4];
Vertex_Indices vert_ptrs1[] = {
  { 0, 1, 2, 3 },
  { 7, 6, 5, 4 },
  { 0, 4, 5, 1 },
  { 1, 5, 6, 2 },
  { 2, 6, 7, 3 },
  { 3, 7, 4, 0 },
};

/* information needed to describe the user's data to the PLY routines */
char *elem_names1[]={ /* list of the kinds of elements in the user's object */
  "vertex", "face","facecolor"
};


PlyProperty vert_props1[] = { /* list of property information for a vertex */
  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex1,x), 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex1,y), 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex1,z), 0, 0, 0, 0},
};

PlyProperty face_props1[] = { /* list of property information for a vertex */
  {"intensity", PLY_UCHAR, PLY_UCHAR, offsetof(Face1,intensity), 0, 0, 0, 0},
  {"vertex_indices", PLY_INT, PLY_INT, offsetof(Face1,verts),1, 
  PLY_UCHAR, PLY_UCHAR, offsetof(Face1,nverts)},
};

/*
PlyProperty facecolor_props1[]=
{
  {"r", PLY_INT, PLY_INT, offsetof(FaceColor,r), 0, 0,0,0},
  {"g", PLY_INT, PLY_INT, offsetof(FaceColor,g), 0, 0,0,0},
  {"b", PLY_INT, PLY_INT, offsetof(FaceColor,b), 0, 0,0,0},
};
*/

extern int g_nNumOfVars;
//extern CGlobals g_CGlobals;

//## end module%3FCA8CD003E7.additionalDeclarations


// Class CMeshOpti 


CMeshOpti::CMeshOpti()
  //## begin CMeshOpti::CMeshOpti%.hasinit preserve=no
      : m_nNumOfVertexs(0), m_nNumOfTriangles(0), m_MinX(FLT_MAX), m_MinY(FLT_MAX), m_MinZ(FLT_MAX), m_MaxX(FLT_MIN), m_MaxY(FLT_MIN), m_MaxZ(FLT_MIN), m_CenterX(0.0), m_CenterY(0.0), m_CenterZ(0.0), m_nOptIterationLimit(100), m_dErrorBound((float)0.01), m_dIncErrorBound((float)0.0001), m_nOptTarget(0), m_dW(1.0)
  //## end CMeshOpti::CMeshOpti%.hasinit
  //## begin CMeshOpti::CMeshOpti%.initialization preserve=yes
  //## end CMeshOpti::CMeshOpti%.initialization
{
  //## begin CMeshOpti::CMeshOpti%.body preserve=yes
  //## end CMeshOpti::CMeshOpti%.body
}


CMeshOpti::~CMeshOpti()
{
  //## begin CMeshOpti::~CMeshOpti%.body preserve=yes
  //## end CMeshOpti::~CMeshOpti%.body
}

//## Other Operations (implementation)
BOOL CMeshOpti::LoadFromPly (CString strFile)
{
	//## begin CMeshOpti::LoadFromPly%3FCA8D0E0168.body preserve=yes
	//	bool CReadPLY::Load(CString strFile)
	////befor  reuse,clear the previous contents:
	this->m_nNumOfTriangles = 0;
	this->m_nNumOfVertexs   = 0;
	////////////////////////
	
	int i,j,k;
	PlyFile *ply = NULL;
	int nelems;
	char **elist = NULL;
	int file_type;
	float version;
	int nprops;
	int num_elems;
	
	PlyProperty **plist;
	Vertex1 **vlist;
	Face1 **flist;
	//  FaceColor **fclist;
	
	char *elem_name;
	char chFile[50];
	strcpy(chFile,strFile);
	
	/* open a PLY file for reading */
	ply = ply_open_for_reading(chFile, &nelems, &elist, &file_type, &version);
	
	
	for (i = 0; i < nelems; i++) 
	{
		/* get the description of the first element */
		elem_name = elist[i];
		plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);
		
		/* if we're on vertex elements, read them in */
		if (equal_strings ("vertex", elem_name)) 
		{
			/* create a vertex list to hold all the vertices */
			vlist = (Vertex1 **) malloc (sizeof (Vertex1 *) * num_elems);
			/* set up for getting vertex elements */
			ply_get_property (ply, elem_name, &vert_props1[0]);
			ply_get_property (ply, elem_name, &vert_props1[1]);
			ply_get_property (ply, elem_name, &vert_props1[2]);
			
			this->m_CVertexArray.SetSize(num_elems);
			this->m_nNumOfVertexs = num_elems;
			/* grab all the vertex elements*/
			for (j = 0; j < num_elems; j++)
			{
				/* grab and element from the file */
				vlist[j] = (Vertex1 *) malloc (sizeof (Vertex1));
				ply_get_element (ply, (void *) vlist[j]);
				
				this->m_CVertexArray[j].dX = vlist[j]->x;
				this->m_CVertexArray[j].dY = vlist[j]->y;
				this->m_CVertexArray[j].dZ = vlist[j]->z;
				
				free(vlist[j]);
			}
		}
		/* if we're on face elements, read them in */
		if (equal_strings ("face", elem_name)) 
		{
			/* create a list to hold all the face elements */
			flist = (Face1 **) malloc (sizeof (Face1 *) * num_elems);
			
			/* set up for getting face elements */
			ply_get_property (ply, elem_name, &face_props1[0]);
			ply_get_property (ply, elem_name, &face_props1[1]);
			
			this->m_CTriangleArray.SetSize(num_elems);
			this->m_nNumOfTriangles = num_elems;
			
			/* grab all the face elements */
			for (j = 0; j < num_elems; j++) 
			{
				/* grab and element from the file */
				flist[j] = (Face1 *) malloc (sizeof (Face1));
				ply_get_element (ply, (void *) flist[j]);
				ASSERT(flist[j]->nverts==3);
				for (k = 0; k < flist[j]->nverts; k++)
				{
					this->m_CTriangleArray[j].pnV[k] = flist[j]->verts[k];
				}
				free(flist[j]);
			}
		}
		if(equal_strings ("facecolor", elem_name))//read into face color
		{
			for(i=0;i<this->m_nNumOfTriangles;i++ )
				fscanf(ply->fp,"%lf %lf %lf ",
				&(this->m_CTriangleArray[i].dColorR),
				&(this->m_CTriangleArray[i].dColorG),
				&(this->m_CTriangleArray[i].dColorB)			
				);
			
		}
	}
	/* close the PLY file */
	ply_close (ply);
	
	return TRUE;
  //## end CMeshOpti::LoadFromPly%3FCA8D0E0168.body
}

BOOL CMeshOpti::LoadFromINI (CString strFile)
{
  //## begin CMeshOpti::LoadFromINI%3FCA8D1C001A.body preserve=yes
	return TRUE;
  //## end CMeshOpti::LoadFromINI%3FCA8D1C001A.body
}

BOOL CMeshOpti::Write2Ply (CString strFile)
{
  //## begin CMeshOpti::Write2Ply%3FCA8D24016B.body preserve=yes
	FILE *pFPly;
	pFPly = fopen( strFile, "w" );
    if (pFPly == NULL )
		return false;
	fprintf(pFPly, "ply\nformat ascii 1.0\ncomment author: Zhou BingFeng\nobj_info random information\n");
    fprintf(pFPly, "element vertex %d\n", this->m_nNumOfVertexs);
	fprintf(pFPly, "property float x\nproperty float y\nproperty float z\n");
	fprintf(pFPly, "element face %d\n", this->m_nNumOfTriangles);
	fprintf(pFPly, "property list uchar int vertex_indices\nelement facecolor 0\nend_header\n");
	for( int i = 0; i < this->m_nNumOfVertexs; i++)
	{
		fprintf(pFPly, "%f %f %f\n", this->m_CVertexArray[i].dX, this->m_CVertexArray[i].dY, this->m_CVertexArray[i].dZ );
	}
	for( i = 0; i < this->m_nNumOfTriangles; i++)
	{
		fprintf(pFPly, "3 %d %d %d\n", 
			this->m_CTriangleArray[i].pnV[0],
			this->m_CTriangleArray[i].pnV[1],
			this->m_CTriangleArray[i].pnV[2] );
	}

for( i = 0; i < this->m_nNumOfTriangles; i++)
	{
		fprintf(pFPly, "%d %d %d\n", 
			(int)this->m_CTriangleArray[i].dColorR,
			(int)this->m_CTriangleArray[i].dColorG,
			(int)this->m_CTriangleArray[i].dColorB);
	}

	_fcloseall();
//	fclose( pFPly); 
  return TRUE;
  //## end CMeshOpti::Write2Ply%3FCA8D24016B.body
}

BOOL CMeshOpti::Write2INI (CString strFile)
{
  //## begin CMeshOpti::Write2INI%3FCA8D2B0273.body preserve=yes
return TRUE;
  //## end CMeshOpti::Write2INI%3FCA8D2B0273.body
}

void CMeshOpti::CalculateLw ()
{
  //## begin CMeshOpti::CalculateLw%3FCAB28C010D.body preserve=yes
	int nN0, nN1, nN2;
	float nDeg;
	int i,j;
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
		for (j = 0 ; j < this->m_nNumOfVertexs; j++)
			this->m_CLw[i][j] = 0;

	
	for ( i = 0; i < this->m_nNumOfTriangles; i++)
	{
		nN0 = this->m_CTriangleArray[i].pnV[0];
		nN1 = this->m_CTriangleArray[i].pnV[1];
		nN2 = this->m_CTriangleArray[i].pnV[2];

		//e01
		this->m_CLw[nN0][nN1]=-1;
		this->m_CLw[nN1][nN0]=-1;
		//e12
		this->m_CLw[nN2][nN1]=-1;
		this->m_CLw[nN1][nN2]=-1;
		//e20
		this->m_CLw[nN0][nN2]=-1;
		this->m_CLw[nN2][nN0]=-1;

	}
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
	{
		nDeg = 0;
		for (j = 0; j < this->m_nNumOfVertexs; j++)
			nDeg += (float)this->m_CLw[i][j];
		this->m_CLw[i][i] = -nDeg;
	}
  //## end CMeshOpti::CalculateLw%3FCAB28C010D.body
}

void CMeshOpti::CalculateLwInverseEdge ()
{
  //## begin CMeshOpti::CalculateLwInverseEdge%3FCF2A86032E.body preserve=yes
	int nN0, nN1, nN2;
	double dDeg;
	int i,j;
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
		for (j = 0 ; j < this->m_nNumOfVertexs; j++)
			this->m_CLw[i][j] = 0;

	
	for ( i = 0; i < this->m_nNumOfTriangles; i++)
	{
		nN0 = this->m_CTriangleArray[i].pnV[0];
		nN1 = this->m_CTriangleArray[i].pnV[1];
		nN2 = this->m_CTriangleArray[i].pnV[2];

		//e01
		this->m_CLw[nN0][nN1] =
		this->m_CLw[nN1][nN0] = (float)(-1.0 / this->Distance(nN0,nN1));
		//e12
		this->m_CLw[nN2][nN1] = 
		this->m_CLw[nN1][nN2] = (float)(-1.0 / this->Distance(nN2,nN1));
		//e20
		this->m_CLw[nN0][nN2] = 
		this->m_CLw[nN2][nN0] = (float)(-1.0 / this->Distance(nN0,nN2));

	}
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
	{
		dDeg = 0.0;
		for (j = 0; j < this->m_nNumOfVertexs; j++)
			dDeg += this->m_CLw[i][j];
		this->m_CLw[i][i] = (float)(-dDeg);
	}
/*
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
	{
		dDeg = this->m_CLw[i][i];

		for (j = 0; j < this->m_nNumOfVertexs; j++)
			this->m_CLw[i][j] = this->m_CLw[i][j]/ dDeg;
	}

*/
  //## end CMeshOpti::CalculateLwInverseEdge%3FCF2A86032E.body
}

void CMeshOpti::CalculateLw1 ()
{
  //## begin CMeshOpti::CalculateLw1%3FCD29D6006A.body preserve=yes
	this->CalculateLw();
	int j;
	for ( int i = 0; i < this->m_nNumOfVertexs; i++)
	{
		float d = (float)this->m_CLw[i][i];
		for ( j=0; j< this->m_nNumOfVertexs; j++ )
			this->m_CLw[i][j] /= d;
	}
  //## end CMeshOpti::CalculateLw1%3FCD29D6006A.body
}

void CMeshOpti::Purify ()
{
  //## begin CMeshOpti::Purify%3FCC578E003C.body preserve=yes

	CArray <int,int> CVtxMap;
	CVtxMap.SetSize(this->m_nNumOfVertexs);

	CArray<CVertex_,CVertex_> CCleanVertexes;
	CCleanVertexes.SetSize(this->m_nNumOfVertexs);
	int nNumOfVtxNew=0;
	//Find first:
	for (int i = 0; i < this->m_nNumOfVertexs;i++)
	{
		for ( int j = 0; j < nNumOfVtxNew;j++)
			if( this->m_CVertexArray[i] == CCleanVertexes[j] )
			{
			
				break;
			}
		if (j >= nNumOfVtxNew)
		{
            nNumOfVtxNew = j+1;
			CCleanVertexes[j] = this->m_CVertexArray[i];
		}
		CVtxMap[i] = j;
	}

	for ( i = 0; i < nNumOfVtxNew; i++)
		this->m_CVertexArray[i] = CCleanVertexes[i];
	this->m_nNumOfVertexs = nNumOfVtxNew;

	for ( i = 0; i < this->m_nNumOfTriangles; i++ )
	{
		for ( int j = 0; j < 3;j++)
		{
			int k;
			k = this->m_CTriangleArray[i].pnV[j];
			k = CVtxMap[k];
			this->m_CTriangleArray[i].pnV[j] = k;	
		}
	}
	
  //## end CMeshOpti::Purify%3FCC578E003C.body
}

void CMeshOpti::Optimize (int nMode)
{
  //## begin CMeshOpti::Optimize%3FCC56B203DE.body preserve=yes

	float *x;
	int n,i,j=1;
	n = this->m_nNumOfVertexs*4;
	x = (float*) new float[n+1];  // Allocate memory for the array
	j = 1;
	for ( i = 0; i < this->m_nNumOfVertexs; i++ )
		x[j++] = (float)this->m_CVertexArray[i].dX;
	for ( i = 0; i < this->m_nNumOfVertexs; i++ )
		x[j++] = (float)this->m_CVertexArray[i].dY;
	for ( i = 0; i < this->m_nNumOfVertexs; i++ )
		x[j++] = (float)this->m_CVertexArray[i].dZ;
	for ( i = 0; i < this->m_nNumOfVertexs; i++ )
		x[j++] = 1.0;//alfai

	g_nNumOfVars = j-1;

	extern int matlab_fsolve(	int nIterationLimit,//int ntrial,
					float *x,			//float x[], 
	    			int	n,				//int n, 
					float dIncErrorBound,//FLT_MIN, //(float)0.001,//float tolx, // Incremental error limit
	    			float	dErrorBound//float tolf //Absolute error limit
					);
	if ( nMode == 4)
	{
//#define USEMATLAB
#ifdef USEMATLAB

		matlab_fsolve(	this->m_nOptIterationLimit,//int ntrial,
			x,//float x[], 
	    	n,	//int n, 
		    (float)this->m_dErrorBound,//FLT_MIN, //(float)0.001,//float tolx, // Incremental error limit
	    	(float)this->m_dIncErrorBound//float tolf //Absolute error limit
			);
#endif
			
	}
	else
	{

/*
	Given an initial guess x[1..n] for a root in n dimensions, take ntrial Newton-Raphson steps
to improve the root. Stop if the root converges in either summed absolute variable increments
tolx or summed absolute function values tolf.
	*/
#define GLOBALSEARCH
#ifdef GLOBALSEARCH
	if ( nMode & 0x0001 )
	{
		g_CGlobals.m_pDlgTab00->ResetOptiIterator();
		mnewt_global(	this->m_nOptIterationLimit,//int ntrial,
			x,//float x[], 
	    	n,	//int n, 
		    (float)this->m_dIncErrorBound,//FLT_MIN, //(float)0.001,//float tolx, // Incremental error limit
	    	(float)this->m_dErrorBound//float tolf //Absolute error limit
			);
	}
#endif
#define ITRATEAGAIN
#ifdef ITRATEAGAIN
	if ( nMode & 0x0002 )
	{
		printf( "Iterate (again) to finelly approach the solution,using N-S method.\n");
		CString msg;
		msg.Format("%s","Iterate (again) to finelly approach the solution,using N-S method.\n");
		
		g_CGlobals.m_pDlgTab00->m_HistoryEdit.AppendString(msg);
		
		g_CGlobals.m_pDlgTab00->ResetOptiIterator();

		mnewt(	this->m_nOptIterationLimit,//int ntrial,
			x,//float x[], 
	    	n,	//int n, 
		    (float)this->m_dErrorBound,//FLT_MIN, //(float)0.001,//float tolx, // Incremental error limit
	    	(float)this->m_dIncErrorBound//float tolf //Absolute error limit
			);
	}
#endif
	}
	j = 1;
	for ( i = 0; i < this->m_nNumOfVertexs; i++ )
		this->m_CVertexArray[i].dX = (double)x[j++];
	for ( i = 0; i < this->m_nNumOfVertexs; i++ )
		this->m_CVertexArray[i].dY = (double)x[j++];
	for ( i = 0; i < this->m_nNumOfVertexs; i++ )
		this->m_CVertexArray[i].dZ = (double)x[j++];
	
	delete [] x; 

  //## end CMeshOpti::Optimize%3FCC56B203DE.body
}

void CMeshOpti::Normalize ()
{
  //## begin CMeshOpti::Normalize%3FCDAC5D0145.body preserve=yes
	int i;
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
	{
		this->m_MinX = min( this->m_MinX, this->m_CVertexArray[i].dX);
		this->m_MinY = min( this->m_MinY, this->m_CVertexArray[i].dY);
		this->m_MinZ = min( this->m_MinZ, this->m_CVertexArray[i].dZ);
		
		this->m_MaxX = max( this->m_MaxX, this->m_CVertexArray[i].dX);
		this->m_MaxY = max( this->m_MaxY, this->m_CVertexArray[i].dY);
		this->m_MaxZ = max( this->m_MaxZ, this->m_CVertexArray[i].dZ);
	}
	this->m_CenterX	= (this->m_MinX + this->m_MaxX) / 2.0;
	this->m_CenterY	= (this->m_MinY + this->m_MaxY) / 2.0;
	this->m_CenterZ	= (this->m_MinZ + this->m_MaxZ) / 2.0;
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
	{
		this->m_CVertexArray[i].dX -= this->m_CenterX;
		this->m_CVertexArray[i].dY -= this->m_CenterY;
		this->m_CVertexArray[i].dZ -= this->m_CenterZ;
	}

	this->m_MinX -= this->m_CenterX;
	this->m_MinY -= this->m_CenterY;
	this->m_MinZ -= this->m_CenterZ;
		
	this->m_MaxX -= this->m_CenterX;
	this->m_MaxY -= this->m_CenterY;
	this->m_MaxZ -= this->m_CenterZ;

	double dScaleX = max(-this->m_MinX, this->m_MaxX);
	double dScaleY = max(-this->m_MinY, this->m_MaxY);
	double dScaleZ = max(-this->m_MinZ, this->m_MaxZ);

	double dScale = max(dScaleX,max(dScaleY,dScaleZ));
//	dScale *= 10.;
	if ( dScale != 0.0)
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
	{
//		this->m_CVertexArray[i].dX = (1.0-0.618)*this->m_CVertexArray[i].dX / dScale;
//		this->m_CVertexArray[i].dY = (1.0-0.618)*this->m_CVertexArray[i].dY / dScale;
//		this->m_CVertexArray[i].dZ = (1.0-0.618)*this->m_CVertexArray[i].dZ / dScale;

		this->m_CVertexArray[i].dX = this->m_CVertexArray[i].dX / dScale;
		this->m_CVertexArray[i].dY = this->m_CVertexArray[i].dY / dScale;
		this->m_CVertexArray[i].dZ = this->m_CVertexArray[i].dZ / dScale;
	}
  //## end CMeshOpti::Normalize%3FCDAC5D0145.body
}

void CMeshOpti::SetOptimizationIterationLimit (int nInterateLimit)
{
  //## begin CMeshOpti::SetOptimizationIterationLimit%3FCE82D700D8.body preserve=yes
	this->m_nOptIterationLimit = nInterateLimit;
  //## end CMeshOpti::SetOptimizationIterationLimit%3FCE82D700D8.body
}

double CMeshOpti::Distance (int i, int j)
{
  //## begin CMeshOpti::Distance%3FCF2B92029D.body preserve=yes
	double d =	(this->m_CVertexArray[i].dX - this->m_CVertexArray[j].dX)*(this->m_CVertexArray[i].dX - this->m_CVertexArray[j].dX) + 
				(this->m_CVertexArray[i].dY - this->m_CVertexArray[j].dY)*(this->m_CVertexArray[i].dY - this->m_CVertexArray[j].dY) +
				(this->m_CVertexArray[i].dZ - this->m_CVertexArray[j].dZ)*(this->m_CVertexArray[i].dZ - this->m_CVertexArray[j].dZ);
	d = sqrt (d);
	return (d);
  //## end CMeshOpti::Distance%3FCF2B92029D.body
}

double CMeshOpti::Distance (double x0, double y0, double z0, double x1, double y1, double z1)
{
  //## begin CMeshOpti::Distance%3FCF4C6E00B8.body preserve=yes
	double d =	(x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) + (z0 - z1)*(z0 - z1);
	d = sqrt (d);
	return d;
  //## end CMeshOpti::Distance%3FCF4C6E00B8.body
}

void CMeshOpti::ToSphere ()
{
  //## begin CMeshOpti::ToSphere%3FCF46110034.body preserve=yes
	
	double r;
	for ( int i = 0; i < this->m_nNumOfVertexs; i++ )
	{
		r = this->Distance( 0.0,0.0,0.0,
							this->m_CVertexArray[i].dX,
							this->m_CVertexArray[i].dY,
							this->m_CVertexArray[i].dZ
							);
		this->m_CVertexArray[i].dX = this->m_CVertexArray[i].dX / r; 
		this->m_CVertexArray[i].dY = this->m_CVertexArray[i].dY / r;
		this->m_CVertexArray[i].dZ = this->m_CVertexArray[i].dZ / r;;
	}
  //## end CMeshOpti::ToSphere%3FCF46110034.body
}

void CMeshOpti::RemoveIsolateVertex ()
{
  //## begin CMeshOpti::RemoveIsolateVertex%3FCF542303E0.body preserve=yes
	int *pnMap = new int[this->m_nNumOfVertexs];
	
	int i,j;
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
		pnMap[i] = -1;

	for ( i = 0; i < this->m_nNumOfTriangles; i++ )
		for ( j = 0; j < 3; j++)
			pnMap[this->m_CTriangleArray[i].pnV[j]] = +1;
	CVertexArray_ CVtxArray;

	CVtxArray.SetSize(this->m_nNumOfVertexs);

	j = 0;
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
	{
		if (pnMap[i] == 1)
		{
			CVtxArray[j] = this->m_CVertexArray[i];
			pnMap[i] = j++;
		}
	}

	for ( i = 0; i < j; i++)
	{
		this->m_CVertexArray[i] = CVtxArray[i];
	}
	this->m_nNumOfVertexs = j;

	for ( i = 0; i < this->m_nNumOfTriangles; i++ )
		for ( j = 0; j < 3; j++)
			this->m_CTriangleArray[i].pnV[j] = pnMap[this->m_CTriangleArray[i].pnV[j]];

	delete [] pnMap;

  //## end CMeshOpti::RemoveIsolateVertex%3FCF542303E0.body
}

// Additional Declarations
//## begin CMeshOpti%3FCA8CD003E7.declarations preserve=yes

int CMeshOpti::GetVertexNum()
{
	return m_CVertexArray.GetSize();
}

int CMeshOpti::GetTriangleNum()
{
	return m_CTriangleArray.GetSize();
}

CVertex_ CMeshOpti::GetVertex(int index)
{
	CVertex_ v;

	v.dX = m_CVertexArray[index].dX;
	v.dY = m_CVertexArray[index].dY;
	v.dZ = m_CVertexArray[index].dZ;

	return v;
}

CTriangle_ CMeshOpti::GetTriangle(int index)
{
	CTriangle_ t; 

	t.pnV[0] = m_CTriangleArray[index].pnV[0];
	t.pnV[1] = m_CTriangleArray[index].pnV[1];
	t.pnV[2] = m_CTriangleArray[index].pnV[2];
	
	return t;
}
	
//## end CMeshOpti%3FCA8CD003E7.declarations

//## begin module%3FCA8CD003E7.epilog preserve=yes
void CMeshOpti::SetDataFromMesh(const CMesh& mesh)
{
	if(&mesh == NULL)
		return;

	m_CVertexArray.SetSize( mesh.GetVertexNum() );
    m_nNumOfVertexs = mesh.GetVertexNum();

    for(int i=0; i<mesh.GetVertexNum(); i++)
	{
		float x,y,z;
		mesh.GetVertex(x, y, z, i);
		m_CVertexArray[i].dX = x;
		m_CVertexArray[i].dY = y;
		m_CVertexArray[i].dZ = z;
	}

	m_CTriangleArray.SetSize(mesh.GetTriangleNum());
    m_nNumOfTriangles = mesh.GetTriangleNum();

	for(i=0; i<mesh.GetTriangleNum(); i++)
	{
		int va,vb,vc;
		mesh.GetTriangleIndices(va, vb, vc, i);
		m_CTriangleArray[i].pnV[0] = va;
		m_CTriangleArray[i].pnV[1] = vb;
		m_CTriangleArray[i].pnV[2] = vc;
	}
}
//## end module%3FCA8CD003E7.epilog

