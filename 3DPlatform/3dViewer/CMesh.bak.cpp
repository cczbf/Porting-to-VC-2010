//## begin module%40F3459A01F0.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40F3459A01F0.cm

//## begin module%40F3459A01F0.cp preserve=no
//## end module%40F3459A01F0.cp

//## Module: CMesh%40F3459A01F0; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CMesh.cpp

//## begin module%40F3459A01F0.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%40F3459A01F0.additionalIncludes

//## begin module%40F3459A01F0.includes preserve=yes
#include "math.h"
#include "float.h"
#include "readply.h"
#include "ply.h"
#include "CGlobals.h"
#include <stdio.h>
#include "progressbar.h"

//## end module%40F3459A01F0.includes

// CMesh
#include "CMesh.h"
//## begin module%40F3459A01F0.declarations preserve=no
//## end module%40F3459A01F0.declarations

//## begin module%40F3459A01F0.additionalDeclarations preserve=yes
/////////////////////////////////////////////{{
//#pragma  C4244

//#pragma warning(disable:4244)

typedef struct Vertex {
  float x,y,z;             /* the usual 3-space position of a vertex */
} Vertex;

typedef struct Face {
  unsigned char intensity; /* this user attaches intensity to faces */
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
} Face;

void mnewt(int ntrial, float x[], int n, float tolx, float tolf);
void mnewt_global(int ntrial, float x[], int n, float tolx, float tolf);

/* polygon description of an object (a cube) */

Vertex verts[] = {  /* vertices */
  { 0.0, 0.0, 0.0},
  { 1.0, 0.0, 0.0},
  { 1.0, 1.0, 0.0},
  { 0.0, 1.0, 0.0},
  { 0.0, 0.0, 1.0},
  { 1.0, 0.0, 1.0},
  { 1.0, 1.0, 1.0},
  { 0.0, 1.0, 1.0},
};

Face faces[] = {  /* faces */
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
Vertex_Indices vert_ptrs[] = {
  { 0, 1, 2, 3 },
  { 7, 6, 5, 4 },
  { 0, 4, 5, 1 },
  { 1, 5, 6, 2 },
  { 2, 6, 7, 3 },
  { 3, 7, 4, 0 },
};

/* information needed to describe the user's data to the PLY routines */
char *elem_names[]={ /* list of the kinds of elements in the user's object */
  "vertex", "face","facecolor"
};


PlyProperty vert_props[] = { /* list of property information for a vertex */
  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
};

PlyProperty face_props[] = { /* list of property information for a vertex */
  {"intensity", PLY_UCHAR, PLY_UCHAR, offsetof(Face,intensity), 0, 0, 0, 0},
  {"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)},
};

PlyProperty facecolor_props[]=
{
  {"r", PLY_INT, PLY_INT, offsetof(FaceColor,r), 0, 0,0,0},
  {"g", PLY_INT, PLY_INT, offsetof(FaceColor,g), 0, 0,0,0},
  {"b", PLY_INT, PLY_INT, offsetof(FaceColor,b), 0, 0,0,0},
};

extern int g_nNumOfVars;

/////////////////////////////////////////////}}



//## end module%40F3459A01F0.additionalDeclarations


// Class CMesh 

























//## begin CMesh::m_pCRelxationLabelling%417C59490142.role preserve=no  public: static CRelxationLabelling { -> RHgAN}
CRelxationLabelling *CMesh::m_pCRelxationLabelling;
//## end CMesh::m_pCRelxationLabelling%417C59490142.role

CMesh::CMesh()
  //## begin CMesh::CMesh%.hasinit preserve=no
      : m_nNumOfVertexs(0), m_nNumOfTriangles(0), m_MinX(FLT_MAX), m_MinY(FLT_MAX), m_MinZ(FLT_MAX), m_MaxX(FLT_MIN), m_MaxY(FLT_MIN), m_MaxZ(FLT_MIN), m_CenterX(0.0), m_CenterY(0.0), m_CenterZ(0.0), m_nOptIterationLimit(100), m_dErrorBound((float)0.01), m_dIncErrorBound((float)0.0001), m_nOptTarget(0), m_dW(1.0)
  //## end CMesh::CMesh%.hasinit
  //## begin CMesh::CMesh%.initialization preserve=yes
  //## end CMesh::CMesh%.initialization
{
  //## begin CMesh::CMesh%.body preserve=yes
	m_nEdgePointsNum = 0;
	m_nType = 0;
	m_CTriangleArray.SetSize(0);
	m_CVertexArray.SetSize(0);
	m_pCRelxationLabelling = NULL;

	m_MinX = 0;  m_MaxX = 1;
	m_MinY = 0;  m_MaxY = 1;
	m_MinZ = 0;  m_MaxZ = 1;	

	m_nEdgeNum = 0;

	//m_dwRef = 0;
  //## end CMesh::CMesh%.body
}


CMesh::~CMesh()
{
  //## begin CMesh::~CMesh%.body preserve=yes
	 if ( m_pCRelxationLabelling !=NULL )
	 {
		 delete m_pCRelxationLabelling;
	     m_pCRelxationLabelling = NULL;
	 }
  //## end CMesh::~CMesh%.body
}



//## Other Operations (implementation)
BOOL CMesh::LoadFromPly (CString strFile)
{
  //## begin CMesh::LoadFromPly%3FCA8D0E0168.body preserve=yes
//	bool CReadPLY::Load(CString strFile)
	  ////befor  reuse,clear the previous contents:
/*
	if(m_pPts!=NULL)
	  {
		  delete m_pPts;
		  m_pPts=NULL;
	  }
	  if(m_pFaceColor!=NULL)
	  {
			delete m_pFaceColor;
			m_pFaceColor=NULL;
	  }
	  if(m_pFaces!=NULL)
	  {
		  delete m_pFaces;
		  m_pFaces=NULL;
	  }*/

//	  m_nFaceSum=0;
//	  m_nPtSum=0;
	  this->m_nNumOfTriangles = 0;
	  this->m_nNumOfVertexs   = 0;
	  ////////////////////////

  int i,j,k;
  PlyFile *ply;
  int nelems;
  char **elist;
  int file_type;
  float version;
  int nprops;
  int num_elems;
  
  PlyProperty **plist;
  Vertex **vlist;
  Face **flist;
//  FaceColor **fclist;

  char *elem_name;
//  int num_comments;
//  char **comments;
//  int num_obj_info;
//  char **obj_info;

  char chFile[50];
  strcpy(chFile,strFile);

  /* open a PLY file for reading */
  ply = ply_open_for_reading(chFile, &nelems, &elist, &file_type, &version);

  /* print what we found out about the file */
  //printf ("version %f\n", version); 
  //printf ("type %d\n", file_type);

  /* go through each kind of element that we learned is in the file */
  /* and read them */

  CProgressBar * pbar = NULL;

  for (i = 0; i < nelems; i++) 
  {
    /* get the description of the first element */
    elem_name = elist[i];
    plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

    /* print the name of the element, for debugging */
    //printf ("element %s %d\n", elem_name, num_elems);

    pbar = new CProgressBar(_T("Progress"), 100, 100, TRUE);
    pbar->SetText("Loading Ply file...");
  
    float step,prog = 0.0;

    /* if we're on vertex elements, read them in */
    if (equal_strings ("vertex", elem_name)) 
	{
      /* create a vertex list to hold all the vertices */
      vlist = (Vertex **) malloc (sizeof (Vertex *) * num_elems);
      /* set up for getting vertex elements */
      ply_get_property (ply, elem_name, &vert_props[0]);
      ply_get_property (ply, elem_name, &vert_props[1]);
      ply_get_property (ply, elem_name, &vert_props[2]);

//	  m_pPts=new POINT3D[num_elems];
	  this->m_CVertexArray.SetSize(num_elems);
     
	  step = float(100.0)/(float)(num_elems);

//	  m_nPtSum=num_elems;
	  this->m_nNumOfVertexs = num_elems;
      /* grab all the vertex elements*/
      for (j = 0; j < num_elems; j++)
	  {

        prog += step;
		pbar->SetPos(int(prog));

        /* grab and element from the file */
        vlist[j] = (Vertex *) malloc (sizeof (Vertex));
        ply_get_element (ply, (void *) vlist[j]);

        /* print out vertex x,y,z for debugging */
        //printf ("vertex: %g %g %g\n", vlist[j]->x, vlist[j]->y, vlist[j]->z);
		//fscanf(fp,"%s %lf %lf %lf",str,&m_pPts[i].x,&m_pPts[i].y,&m_pPts[i].z);
//		m_pPts[j].x=vlist[j]->x;//*10;
//		m_pPts[j].y=vlist[j]->y;//*10;
//		m_pPts[j].z=vlist[j]->z;//*10;
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
      flist = (Face **) malloc (sizeof (Face *) * num_elems);

      /* set up for getting face elements */
      ply_get_property (ply, elem_name, &face_props[0]);
      ply_get_property (ply, elem_name, &face_props[1]);

//	  m_pFaces=new stFACE[num_elems];
	  this->m_CTriangleArray.SetSize(num_elems);
//	  m_nFaceSum=num_elems;
	  this->m_nNumOfTriangles = num_elems;

      step = float(100.0)/(float)(num_elems);

      /* grab all the face elements */
      for (j = 0; j < num_elems; j++) 
	  {

		prog += step;
		pbar->SetPos(int(prog));

        /* grab and element from the file */
        flist[j] = (Face *) malloc (sizeof (Face));
        ply_get_element (ply, (void *) flist[j]);
        /* print out face info, for debugging */
        //printf ("face: %d, list = ", flist[j]->intensity);

//		m_pFaces[j].n=flist[j]->nverts;
		ASSERT(flist[j]->nverts==3);
        for (k = 0; k < flist[j]->nverts; k++)
          //printf ("%d ", flist[j]->verts[k]);
		{
//			m_pFaces[j].vI[k]=flist[j]->verts[k];
			this->m_CTriangleArray[j].pnV[k] = flist[j]->verts[k];
		}
		//printf ("\n");
		free(flist[j]);
      }
    }
	if(equal_strings ("facecolor", elem_name))//read into face color
	{
//		m_pFaceColor=new POINT3D[m_nFaceSum];
//		for(i=0;i<m_nFaceSum;i++ )
		for(i=0;i<this->m_nNumOfTriangles;i++ )
//			fscanf(ply->fp,"%lf %lf %lf ",&m_pFaceColor[i].x,&m_pFaceColor[i].y,
//			&m_pFaceColor[i].z);
			fscanf(ply->fp,"%lf %lf %lf ",
			&(this->m_CTriangleArray[i].dColorR),
			&(this->m_CTriangleArray[i].dColorG),
			&(this->m_CTriangleArray[i].dColorB)			
			);

	}
    /* print out the properties we got, for debugging */
    //for (j = 0; j < nprops; j++)
    // printf ("property %s\n", plist[j]->name);

	delete pbar;
  }


  //calculate the normals:
/*
  m_pNormals=new POINT3D[m_nFaceSum];
  for(i=0;i<m_nFaceSum;i++)
  {
	  int v1=m_pFaces[i].vI[0];
	  int v2=m_pFaces[i].vI[1];
	  int v3=m_pFaces[i].vI[2];
	  POINT3D p1,p2;

	  p1.x=m_pPts[v1].x-m_pPts[v2].x;
	  p1.y=m_pPts[v1].y-m_pPts[v2].y;
	  p1.z=m_pPts[v1].z-m_pPts[v2].z;
	  
	  p2.x=m_pPts[v1].x-m_pPts[v3].x;
	  p2.y=m_pPts[v1].y-m_pPts[v3].y;
	  p2.z=m_pPts[v1].z-m_pPts[v3].z;
	
	  m_pNormals[i]=CrossValue(p1,p2);
  }
  */
  /* grab and print out the comments in the file */
  // comments = ply_get_comments (ply, &num_comments);
  //for (i = 0; i < num_comments; i++)
  //  printf ("comment = '%s'\n", comments[i]);

  /* grab and print out the object information */
  //obj_info = ply_get_obj_info (ply, &num_obj_info);
  //for (i = 0; i < num_obj_info; i++)
  //  printf ("obj_info = '%s'\n", obj_info[i]);

  /* close the PLY file */
  ply_close (ply);

//  this->SetAdjacentMatrixEx(); //Removed by ZBF, we do not need adjacent matrix for ply dada fo now. 20041030
  
  ComputeEdgeNum();

  return TRUE;
  //## end CMesh::LoadFromPly%3FCA8D0E0168.body
}

BOOL CMesh::LoadFromRaw (HANDLE hRaw, int w, int h)
{
  //## begin CMesh::LoadFromRaw%41344610035B.body preserve=yes
//  this->m_nNumOfTriangles = 0;
//  this->m_nNumOfVertexs   = 0;
	
	this->m_nNumOfTriangles = (w-1)*(h-1)*2;
	this->m_nNumOfVertexs   = w*h;
	
	int i,j,k;
	unsigned char *lpRaw = (unsigned char*)GlobalLock(hRaw);
	
    /* get the description of the first element */
	this->m_CVertexArray.SetSize( this->m_nNumOfVertexs);
	
	/* grab all the vertex elements*/
	k = 0;
	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
			
		{
			/* grab and element from the file */
			this->m_CVertexArray[k].dX = (double)i/(double)(w-1);
			this->m_CVertexArray[k].dY = (double)j/(double)(h-1);
			this->m_CVertexArray[k].dZ = (double)lpRaw[k]/255.0;
			k++;
		}
		
		/* if we're on face elements, read them in */
		/* create a list to hold all the face elements */
		this->m_CTriangleArray.SetSize(this->m_nNumOfTriangles);
		
		/* grab all the face elements */
		k = 0;
		int m = 0;
		for ( i = 0; i < w-1; i++ )
			for ( j = 0; j < h-1; j++ ) 
			{
				/* grab and element from the file */
				k = j*w + i;
				this->m_CTriangleArray[m].pnV[0] = k;
				this->m_CTriangleArray[m].pnV[1] = k+1;
				this->m_CTriangleArray[m].pnV[2] = k+1+w;
				m++;
				
				this->m_CTriangleArray[m].pnV[0] = k;
				this->m_CTriangleArray[m].pnV[2] = k+w;
				this->m_CTriangleArray[m].pnV[1] = k+1+w;
				m++;
			}
			
			ASSERT(m==this->m_nNumOfTriangles);
			//calculate the normals:
			GlobalUnlock(hRaw);
			
//			this->SetAdjacentMatrixEx();
			
			return TRUE;

  //## end CMesh::LoadFromRaw%41344610035B.body
}

BOOL CMesh::LoadFromINI (CString strFile)
{
  //## begin CMesh::LoadFromINI%3FCA8D1C001A.body preserve=yes
return TRUE;
  //## end CMesh::LoadFromINI%3FCA8D1C001A.body
}

BOOL CMesh::Write2Ply (CString strFile)
{
  //## begin CMesh::Write2Ply%3FCA8D24016B.body preserve=yes
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
  //## end CMesh::Write2Ply%3FCA8D24016B.body
}

BOOL CMesh::Write2INI (CString strFile)
{
  //## begin CMesh::Write2INI%3FCA8D2B0273.body preserve=yes
return TRUE;
  //## end CMesh::Write2INI%3FCA8D2B0273.body
}

void CMesh::CalculateLw ()
{
  //## begin CMesh::CalculateLw%3FCAB28C010D.body preserve=yes
	int nN0, nN1, nN2;
	double nDeg;
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
			nDeg += this->m_CLw[i][j];
		this->m_CLw[i][i] =(float) -nDeg;
	}
  //## end CMesh::CalculateLw%3FCAB28C010D.body
}

void CMesh::CalculateLwInverseEdge ()
{
  //## begin CMesh::CalculateLwInverseEdge%3FCF2A86032E.body preserve=yes
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
  //## end CMesh::CalculateLwInverseEdge%3FCF2A86032E.body
}

void CMesh::CalculateLw1 ()
{
  //## begin CMesh::CalculateLw1%3FCD29D6006A.body preserve=yes
	this->CalculateLw();
	int j;
	for ( int i = 0; i < this->m_nNumOfVertexs; i++)
	{
		double d = this->m_CLw[i][i];
		for ( j=0; j< this->m_nNumOfVertexs; j++ )
			this->m_CLw[i][j] /= (float)d;
	}
  //## end CMesh::CalculateLw1%3FCD29D6006A.body
}

void CMesh::Purify ()
{
  //## begin CMesh::Purify%3FCC578E003C.body preserve=yes

	CArray <int,int> CVtxMap;
	CVtxMap.SetSize(this->m_nNumOfVertexs);

	CArray<CVertex,CVertex> CCleanVertexes;
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
	
  //## end CMesh::Purify%3FCC578E003C.body
}

void CMesh::Optimize (int nMode)
{
  //## begin CMesh::Optimize%3FCC56B203DE.body preserve=yes

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

  //## end CMesh::Optimize%3FCC56B203DE.body
}

void CMesh::Normalize (float fScaleFactor)
{
  //## begin CMesh::Normalize%3FCDAC5D0145.body preserve=yes
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
	dScale /= fScaleFactor;
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

	//reset 
	m_MinX = 0;  m_MaxX = 1;
	m_MinY = 0;  m_MaxY = 1;
	m_MinZ = 0;  m_MaxZ = 1;	

  //## end CMesh::Normalize%3FCDAC5D0145.body
}

void CMesh::SetAdjacentMatrixEx ()
{
  //## begin CMesh::SetAdjacentMatrixEx%413448380278.body preserve=yes
	//this->m_CAdjacentMatrixEx.SetDim( this->m_nNumOfVertexs, this->m_nNumOfVertexs);
	m_pCRelxationLabelling->SetAdjacentMatrixDim(m_nNumOfVertexs,m_nNumOfVertexs);	
	

	/*
    CProgressBar * pbar = NULL;
    pbar = new CProgressBar(_T("Setting Adjacent Matrix"), 100, 100, TRUE);
    pbar->SetText("Setting Adjacent Matrix...");
    float step = 0;
	float prog = 0;
	step = 100.0/(float)(m_nNumOfVertexs);

	for ( i = 0; i < this->m_nNumOfVertexs; i++)
	{
		prog += step;
		pbar->SetPos(prog);
		
		for ( j = 0; j < this->m_nNumOfVertexs; j++)
		{			
			this->m_CAdjacentMatrixEx[i][j] = 0;

		}
	}
	delete pbar;
	*/

    CProgressBar *pbar = new CProgressBar(_T("Setting Adjacent Matrix"), 100, 100, TRUE);
    pbar->SetText("Setting Adjacent Matrix...");
	double step = 100.0/(float)(m_nNumOfTriangles);
	double prog = 0;

    int v1,v2,v3;
	for (int i = 0; i < this->m_nNumOfTriangles; i++)
	{
		prog += step;
		pbar->SetPos((int)prog);

		v1 = this->m_CTriangleArray[i].pnV[0]; 
		v2 = this->m_CTriangleArray[i].pnV[1]; 
		v3 = this->m_CTriangleArray[i].pnV[2]; 

		m_pCRelxationLabelling->SetAdjacentMatrixEdge(v1, v2, v3);

		/*
		this->m_CAdjacentMatrixEx[v1][v2] += 1;
		this->m_CAdjacentMatrixEx[v2][v1] += 1;

		this->m_CAdjacentMatrixEx[v2][v3] += 1;
		this->m_CAdjacentMatrixEx[v3][v2] += 1;

		this->m_CAdjacentMatrixEx[v3][v1] += 1;
		this->m_CAdjacentMatrixEx[v1][v3] += 1;
		*/
	}
	delete pbar;
  
	//initialize the edge list:
//	m_pCRelxationLabelling->SetEdgeList();

  //## end CMesh::SetAdjacentMatrixEx%413448380278.body
}

void CMesh::SetMesh (int nTriangle, int* nV1, int* nV2, int* nV3, DWORD* c12, DWORD* c23, DWORD* c31, int nVlength, double* x, double* y, double* z)
{
  //## begin CMesh::SetMesh%4181ECA4023A.body preserve=yes
  
  //if the data is changed, refresh the point data
	if ( x != NULL )   
	{
		m_CVertexArray.SetSize(nVlength);
		for (int i=0; i<nVlength; i++ )
		{
     		m_CVertexArray[i].dX = x[i];
			m_CVertexArray[i].dY = y[i];
			m_CVertexArray[i].dZ = z[i]; 
		}				
	}

	m_nNumOfVertexs = nVlength;

  //refresh the triangle 
	m_CTriangleArray.SetSize(nTriangle);
	for (int i=0; i<nTriangle; i++)
	{
		m_CTriangleArray[i].pnV[0] = nV1[i];
		m_CTriangleArray[i].pnV[1] = nV2[i];
		m_CTriangleArray[i].pnV[2] = nV3[i];
		
		m_CTriangleArray[i].dC12 = c12[i]; 
		m_CTriangleArray[i].dC23 = c23[i];
		m_CTriangleArray[i].dC31 = c31[i];
	}

	m_nNumOfTriangles = nTriangle;
	this->Normalize(0.5);
	
  //## end CMesh::SetMesh%4181ECA4023A.body
}

void CMesh::GetMesh (int* nTriangle, int** nV1, int** nV2, int** nV3, int** c12, int** c23, int** c31, int* nVlength, double** x, double** y, double** z)
{
  //## begin CMesh::GetMesh%41882FD101F8.body preserve=yes
*nTriangle = m_CTriangleArray.GetSize();

  *nV1 = new int[*nTriangle];
  *nV2 = new int[*nTriangle];
  *nV3 = new int[*nTriangle];

  for (int i=0; i<*nTriangle; i++)
  {
	  (*nV1)[i] = m_CTriangleArray[i].pnV[0];
	  (*nV2)[i] = m_CTriangleArray[i].pnV[1];
	  (*nV3)[i] = m_CTriangleArray[i].pnV[2];
  }
	
  //## end CMesh::GetMesh%41882FD101F8.body
}

void CMesh::SetOptimizationIterationLimit (int nInterateLimit)
{
  //## begin CMesh::SetOptimizationIterationLimit%3FCE82D700D8.body preserve=yes
	this->m_nOptIterationLimit = nInterateLimit;
  //## end CMesh::SetOptimizationIterationLimit%3FCE82D700D8.body
}

double CMesh::Distance (int i, int j)
{
  //## begin CMesh::Distance%3FCF2B92029D.body preserve=yes
	double d =	(this->m_CVertexArray[i].dX - this->m_CVertexArray[j].dX)*(this->m_CVertexArray[i].dX - this->m_CVertexArray[j].dX) + 
				(this->m_CVertexArray[i].dY - this->m_CVertexArray[j].dY)*(this->m_CVertexArray[i].dY - this->m_CVertexArray[j].dY) +
				(this->m_CVertexArray[i].dZ - this->m_CVertexArray[j].dZ)*(this->m_CVertexArray[i].dZ - this->m_CVertexArray[j].dZ);
	d = sqrt (d);
	return (d);
  //## end CMesh::Distance%3FCF2B92029D.body
}

double CMesh::Distance (double x0, double y0, double z0, double x1, double y1, double z1)
{
  //## begin CMesh::Distance%3FCF4C6E00B8.body preserve=yes
	double d =	(x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) + (z0 - z1)*(z0 - z1);
	d = sqrt (d);
	return d;
  //## end CMesh::Distance%3FCF4C6E00B8.body
}

void CMesh::ToSphere ()
{
  //## begin CMesh::ToSphere%3FCF46110034.body preserve=yes
	
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
  //## end CMesh::ToSphere%3FCF46110034.body
}

void CMesh::RemoveIsolateVertex ()
{
  //## begin CMesh::RemoveIsolateVertex%3FCF542303E0.body preserve=yes
	int *pnMap = new int[this->m_nNumOfVertexs];
	
	int i,j;
	for ( i = 0; i < this->m_nNumOfVertexs; i++)
		pnMap[i] = -1; 

	for ( i = 0; i < this->m_nNumOfTriangles; i++ )
		for ( j = 0; j < 3; j++)
			pnMap[this->m_CTriangleArray[i].pnV[j]] = +1;
	CVertexArray CVtxArray;

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

  //## end CMesh::RemoveIsolateVertex%3FCF542303E0.body
}

void CMesh::Load2DMesh (CString fileName)
{
  //## begin CMesh::Load2DMesh%417C5C4D02DD.body preserve=yes
//	void CMesh::Load2DMesh(CString fileName)
{
	FILE *fp = NULL;
    fp = fopen(fileName, "r");
	
	if(fp != NULL)
	{		
		int n3DPtsNum = 0;
		int n2DInnerPtsNum = 0;
		int nTriangleNum = 0;
		
		float gX = 0;
		float gY = 0;
		float gZ = 0;
		float x = 0;
		float y = 0;
		int   index = 0;
		int va = 0;
		int vb = 0;
		int vc = 0;		

		ASSERT(m_pCRelxationLabelling != NULL);

		//read the 3D points
		fscanf(fp, "%d", &n3DPtsNum);
		
		//allocate memory for relaxation labeling 3D points
		m_pCRelxationLabelling->Set3DPointNum(n3DPtsNum);		
		for (int i=0; i<n3DPtsNum; i++)
		{
			fscanf(fp,"%f %f %f", &gX, &gY, &gZ);			
			CNode3D p;
			p.x = gX; p.y = gY; p.z = gZ;

			//insert 3D point into RelxationLabelling
			m_pCRelxationLabelling->Set3DPoint(p, i);						
		}

		int nEdgePtsNum = 0;
		//read the number of edge points, (Actually, it's number of border nodes.-zbf 041029)
		fscanf(fp, "%d", &nEdgePtsNum);
		m_nEdgePointsNum = nEdgePtsNum;
		//read the 2D mesh points (2D internal nodes -zbf 041029)
		fscanf(fp, "%d", &n2DInnerPtsNum);
        
		int n2DPtsNum = nEdgePtsNum + n2DInnerPtsNum;

		m_CVertexArray.SetSize(n2DPtsNum);
		m_nNumOfVertexs = n2DPtsNum;  //the number of triangle vertex		
		
		//allocate memory for relaxation labeling 2D points
		m_pCRelxationLabelling->Set2DPointNum(n2DPtsNum);
				
		for (i=0; i<n2DPtsNum; i++)
		{
			fscanf(fp, "%f %f %d", &x, &y, &index);
			
			m_CVertexArray[i].dX = x;
			m_CVertexArray[i].dY = y;
			m_CVertexArray[i].dZ = 0; //default set as 0
			m_CVertexArray[i].index = index;

			CNode2D p;
			p.u = x;  p.v = y;  p.m_n3DIndex = index;

			//insert 2D point into RelxationLabelling
			m_pCRelxationLabelling->Set2DPoint(p, i);
		}
		
		//read the triangle index	
		fscanf(fp, "%d", &nTriangleNum);
		m_CTriangleArray.SetSize(nTriangleNum);
        m_nNumOfTriangles = nTriangleNum;
		
		//typedef  set< Edge, less<Edge> >  EdgeSet;
   	    //EdgeSet  EdgeCollect;

		for (i=0; i<nTriangleNum; i++)
		{
			fscanf(fp, "%d %d %d", &va, &vb, &vc);

			m_CTriangleArray[i].pnV[0] = va;
			m_CTriangleArray[i].pnV[1] = vb;
			m_CTriangleArray[i].pnV[2] = vc;	
			
		}		
		fclose(fp);

		ComputeColor();

		this->SetAdjacentMatrixEx();
			
		//initialize the edge list of CRL, this function must be called after adjacent matrix is set
		this->m_pCRelxationLabelling->SetEdgeList();
		this->m_pCRelxationLabelling->SetBorderLen(m_nEdgePointsNum);
		//compute geometry stretch
		this->ComputeGeoStretch();
	}	
}

  //## end CMesh::Load2DMesh%417C5C4D02DD.body
}

// Additional Declarations
  //## begin CMesh%3FCA8CD003E7.declarations preserve=yes
  //## end CMesh%3FCA8CD003E7.declarations

// Class CAdjacentMatrixEx 





//## Other Operations (implementation)
int CAdjacentMatrixEx::FindBorder ()
{
  //## begin CAdjacentMatrixEx::FindBorder%41357E49017E.body preserve=yes
	double t = (*this)[0][0];
	return this->m_nBorderLength;
  //## end CAdjacentMatrixEx::FindBorder%41357E49017E.body
}

// Additional Declarations
  //## begin CAdjacentMatrixEx%413442F9005A.declarations preserve=yes
/*
//added by xiedonghai,2004,10,22.
void CMesh::Load2DMesh(CString fileName)
{
	FILE *fp = NULL;
    fp = fopen(fileName, "r");
	
	if(fp != NULL)
	{		
		int n3DPtsNum = 0;
		int n2DPtsNum = 0;
		int nTriangleNum = 0;
		
		float gX = 0;
		float gY = 0;
		float gZ = 0;
		float x = 0;
		float y = 0;
		int   index = 0;
		int va = 0;
		int vb = 0;
		int vc = 0;

		//read the 3D points
		fscanf(fp, "%d", &n3DPtsNum);
		for (int i=0; i<n3DPtsNum; i++)
		{
			fscanf(fp,"%f %f %f", &gX, &gY, &gZ);
		}
		//read the 2D mesh points
		fscanf(fp, "%d", &n2DPtsNum);
		m_CVertexArray.SetSize(n2DPtsNum);
		for (i=0; i<n2DPtsNum; i++)
		{
			fscanf(fp, "%f %f %d", &x, &y, &index);
			m_CVertexArray[i].dX = x;
			m_CVertexArray[i].dY = y;
			m_CVertexArray[i].dZ = 0; //default set as 0
		}
		//read the triangle index
		fscanf(fp, "%d", &nTriangleNum);
		m_CTriangleArray.SetSize(nTriangleNum);
		for (i=0; i<nTriangleNum; i++)
		{
			fscanf(fp, "%d %d %d", &va, &vb, &vc);
			m_CTriangleArray[i].pnV[0] = va;
			m_CTriangleArray[i].pnV[1] = vb;
			m_CTriangleArray[i].pnV[2] = vc;			
		}		
		fclose(fp);
	}	
}
*/
  //## end CAdjacentMatrixEx%413442F9005A.declarations
//## begin module%40F3459A01F0.epilog preserve=yes
/*
void CMesh::SerRL(CRelxationLabelling *pRL)
{
	m_pCRelxationLabelling = pRL;
}
*/

void CMesh::SetEmptyRL()
{
	if (m_pCRelxationLabelling != NULL)
		delete m_pCRelxationLabelling;
	m_pCRelxationLabelling = new CRelxationLabelling; 
}
float CMesh::ComputeStretchRatio(float d3l, float d2l)
{
	float ratio = 0;
		
	float E = (float)exp(1);
	float lfx = (float)fabs(d3l - d2l);
	ratio = float(( E - pow(E, (1/(lfx+1))) )/(E-1));
	
	/*
	if(lfRatio > lfRatioMax)  
		lfRatioMax = lfRatio;
	if(lfRatio < lfRatioMin)  
		lfRatioMin = lfRatio;
	
	lfStat[nCnt++]= lfRatio;
	
	H = 240-(int)(lfRatio * 240)%240;
	H/=60.0;
	TEMP=floor(H);
	F = H -TEMP;
	P = I *(1.0 -S);
	Q = I * (1.0 - (S*F));
	T = I *(1.0 - (S *(1.0 -F)));
	switch (TEMP)
	{
	case 0 :
	    R =I; G =T; B =P; 
		break;
	case 1 :
		R =Q; G =I; B = P ;
		break;
	case 2 : 
		R= P; G = I; B = T; 
		break;
	case 3 :
		R = P; G =Q; B= I;
		break; 
	case 4 : 
		R = T; G = P; B= I;
		break;
	case 5 :
		R = I; G = P; B = Q;
		break;
	}
	*/

	return ratio;
}

float CMesh::ComputeGeoStretch()
{	
	int i,j;
	double Ss[3],St[3],L2;
	int t[3];
	double tx[3],ty[3];
	double a,b,c;
	double *A = NULL;
	double *geoStretch = NULL;

	double lfSum1, lfSum2;
       
	int nFaceSize = m_CTriangleArray.GetSize();

	geoStretch = new double[nFaceSize];
	A = new double[nFaceSize];

	for(i=0; i<nFaceSize; i++)
	{ 
		
		for (j=0; j<3; j++)
		{
			int tempIndex = m_CTriangleArray[i].pnV[j];
			tx[j] = m_CVertexArray[tempIndex].dX;
			ty[j] = m_CVertexArray[tempIndex].dY;
			
			t[j] =  m_CVertexArray[tempIndex].index;
		}			
		
		A[i]=((tx[1]-tx[0])*(ty[2]-ty[0]) - (tx[2]-tx[0])*(ty[1]-ty[0]))/2;

		CNode3D p1,p2,p3;
		p1 = m_pCRelxationLabelling->Get3DPoint(t[0]);
		p2 = m_pCRelxationLabelling->Get3DPoint(t[1]);
		p3 = m_pCRelxationLabelling->Get3DPoint(t[2]);

		Ss[0]=( p1.x*(ty[1]-ty[2])
			  + p2.x*(ty[2]-ty[0])
			  + p3.x*(ty[0]-ty[1]) )/(2*A[i]);

		Ss[1]=( p1.y*(ty[1]-ty[2])
			  + p2.y*(ty[2]-ty[0])
			  + p3.y*(ty[0]-ty[1]))/(2*A[i]);
		
		Ss[2]=( p1.z*(ty[1]-ty[2])
			  + p2.z*(ty[2]-ty[0])
			  + p3.z*(ty[0]-ty[1]))/(2*A[i]);

			
		St[0]=( p1.x*(tx[2]-tx[1])
			  + p2.x*(tx[0]-tx[2])
			  + p3.x*(tx[1]-tx[0]) )/(2*A[i]);

		St[1]=( p1.y*(tx[2]-tx[1])
			  + p2.y*(tx[0]-tx[2])
			  + p3.y*(tx[1]-tx[0]))/(2*A[i]);

		St[2]=( p1.z*(tx[2]-tx[1])
			  + p2.z*(tx[0]-tx[2])
			  + p3.z*(tx[1]-tx[0]))/(2*A[i]);
		
		a=Ss[0]*Ss[0]+Ss[1]*Ss[1]+Ss[2]*Ss[2];
		b=Ss[0]*St[0]+Ss[1]*St[1]+Ss[2]*St[2];
		c=St[0]*St[0]+St[1]*St[1]+St[2]*St[2];

		L2=sqrt((a+c)/2);
		geoStretch[i]=L2;
	}

	lfSum1=0;
	lfSum2=0;
	for(i=0; i < nFaceSize; i++)
	{ 
		lfSum1+=geoStretch[i]*geoStretch[i]*A[i];
		lfSum2+=A[i];
	}
	
	delete []A;
	delete []geoStretch;

	double geoS = sqrt(lfSum1/lfSum2);
	m_geometryStretch = geoS;
	return float(geoS);
}

float CMesh::GetGeometryStretch()
{
	return (float)m_geometryStretch;
}

void  CMesh::ComputeColor()
{		
	int n = m_CTriangleArray.GetSize();
	
	for( int i=0; i<n; i++ )
	{
		int ia;
		int ib;
		int ic;
		
		CNode3D pa, pb, pc;
		
		float a[3],b[3],c[3];
		a[0] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[0] ].dX ) ;
		a[1] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[0] ].dY );
		a[2] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[0] ].dZ - 0.5 );
		ia = m_CVertexArray[m_CTriangleArray[i].pnV[0] ].index;
		pa = m_pCRelxationLabelling->Get3DPoint(ia);
		
		b[0] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[1] ].dX );
		b[1] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[1] ].dY );
		b[2] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[1] ].dZ - 0.5 );
		ib = m_CVertexArray[m_CTriangleArray[i].pnV[1] ].index;
		pb = m_pCRelxationLabelling->Get3DPoint(ib);
		
		c[0] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[2] ].dX );
		c[1] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[2] ].dY );
		c[2] = (float)( m_CVertexArray[ m_CTriangleArray[i].pnV[2] ].dZ - 0.5 );
		ic = m_CVertexArray[m_CTriangleArray[i].pnV[2] ].index;
		pc = m_pCRelxationLabelling->Get3DPoint(ic);
		
		float d2_ab = 0;
		float d3_ab = 0;
		
		float d2_ac = 0;
		float d3_ac = 0;
		
		float d2_bc = 0;
		float d3_bc = 0;
		
		d2_ab = (float)sqrt( (a[0]-b[0])*(a[0]-b[0]) + 
			(a[1]-b[1])*(a[1]-b[1]) +
			(a[2]-b[2])*(a[2]-b[2]) );
		d3_ab = (float)sqrt( (pa.x-pb.x)*(pa.x-pb.x) + 
			(pa.y-pb.y)*(pa.y-pb.y) +
			(pa.z-pb.z)*(pa.z-pb.z) );
		
		d2_ac = (float)sqrt( (a[0]-c[0])*(a[0]-c[0]) + 
			(a[1]-c[1])*(a[1]-c[1]) +
			(a[2]-c[2])*(a[2]-c[2]) );
		d3_ac = (float)sqrt( (pa.x-pc.x)*(pa.x-pc.x) + 
		          (pa.y-pc.y)*(pa.y-pc.y) +
				  (pa.z-pc.z)*(pa.z-pc.z) );
		
		d2_bc = (float)sqrt( (c[0]-b[0])*(c[0]-b[0]) + 
			(c[1]-b[1])*(c[1]-b[1]) +
			(c[2]-b[2])*(c[2]-b[2]) );
		d3_bc = (float)sqrt( (pb.x-pc.x)*(pb.x-pc.x) + 
			(pb.y-pc.y)*(pb.y-pc.y) +
			(pb.z-pc.z)*(pb.z-pc.z) );
		
		float ratio = 0;
		float cr,cg,cb;
		int   nr,ng,nb;
		
		ratio = ComputeStretchRatio(d3_ab, d2_ab);
		int H = 240-(int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		m_CTriangleArray[i].dC12 = RGB(nr, ng, nb);

		
		ratio = ComputeStretchRatio(d3_ac, d2_ac);
		H = 240 - (int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		m_CTriangleArray[i].dC31 = RGB(nr, ng, nb);

		
		ratio = ComputeStretchRatio(d3_bc, d2_bc);
		H = 240 - (int)(ratio * 240)%240;
		HSV_To_RGB(cr, cg, cb, (float)H, (float)1.0, (float)1.0);
		nr = (int)(cr * 255);
		ng = (int)(cg * 255);
		nb = (int)(cb * 255);		
		m_CTriangleArray[i].dC23 = RGB(nr, ng, nb);

	}
		
}

void  CMesh::AddRef()
{
	m_dwRef++;
}

void  CMesh::Release()
{
	m_dwRef--;

	if (m_dwRef == 0)
		delete this;   //	
}

int CMesh::GetBorderNodeNum()
{
	return m_nEdgePointsNum;
}

void CMesh::ComputeEdgeNum()
{
	/*
	typedef  set< Edge, less<Edge> >  EdgeSet;
	EdgeSet  EdgeCollect;
	
	int nTriangleNum = m_CTriangleArray.GetSize();

	int va = 0;
	int vb = 0;
	int vc = 0;

	for (int i=0; i<nTriangleNum; i++)
	{
		va = m_CTriangleArray[i].pnV[0] ;
		vb = m_CTriangleArray[i].pnV[1] ;
		vc = m_CTriangleArray[i].pnV[2] ;	
		
		Edge e;
		e.v1 = va;
		e.v2 = vb;
		if (e.v1 > e.v2)
		{
			int t;
			t = e.v1;
			e.v1 = e.v2;
			e.v2 = t;
		}
		EdgeCollect.insert(e);
		
		e.v1 = vc;
		e.v2 = vb;
		if (e.v1 > e.v2)
		{
			int t;
			t = e.v1;
			e.v1 = e.v2;
			e.v2 = t;				
		}
		EdgeCollect.insert(e);
		
		e.v1 = va;
		e.v2 = vc;
		if (e.v1 > e.v2)
		{
			int t;
			t = e.v1;
			e.v1 = e.v2;
			e.v2 = t;
		}
		EdgeCollect.insert(e);
		
		int size = EdgeCollect.size();
	}		
	
	m_nEdgeNum = EdgeCollect.size();
	EdgeSet::iterator p;
	FILE *fp = fopen("F:\\edges.txt", "w");
	for ( p=EdgeCollect.begin(); p!=EdgeCollect.end(); p++ )
	{
		fprintf(fp, "%d  %d \n", (*p).v1 ,(*p).v2);
	}
	fclose(fp);
	*/

}

//## end module%40F3459A01F0.epilog
