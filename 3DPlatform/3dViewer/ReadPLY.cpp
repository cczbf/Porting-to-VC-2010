#include "stdafx.h"
#include "readply.h"
#include "ply.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include "stdlib.h"

#include "GraphicsAlgorithms.h"


typedef struct Vertex {
  float x,y,z;             /* the usual 3-space position of a vertex */
} Vertex;

typedef struct Face {
  unsigned char intensity; /* this user attaches intensity to faces */
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
} Face;
  
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
  "vertex", "face"
};

PlyProperty vert_props[] = { /* list of property information for a vertex */
  {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
};

PlyProperty face_props[] = { /* list of property information for a vertex */
  {"intensity", PLY_UCHAR, PLY_UCHAR, offsetof(Face,intensity), 0, 0, 0, 0},
  {"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
   1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)},
};


CReadPLY::CReadPLY()
{
	m_pPts=NULL;
	m_pFaces=NULL;
	m_pNormals=NULL;
	m_nFaceSum = 0;
	m_nPtSum = 0;
}

CReadPLY::~CReadPLY()
{

	if(m_pPts!=NULL)
	{
		delete m_pPts;
		m_pPts =NULL;
	}
	if(m_pFaces!=NULL)
	{ 
		delete m_pFaces;
		m_pFaces=NULL;
	}
	if(m_pNormals!=NULL)
	{ 
		delete m_pNormals;
		m_pNormals =NULL;
	}
}

bool CReadPLY::Load(CString strFile)
{
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
  char *elem_name;
 
  char chFile[100];
  strcpy(chFile,strFile);

  /* open a PLY file for reading */
  ply = ply_open_for_reading(chFile, &nelems, &elist, &file_type, &version);

  /* go through each kind of element that we learned is in the file */
  /* and read them */

  for (i = 0; i < nelems; i++) 
  {
    /* get the description of the first element */
    elem_name = elist[i];
    plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

 
    /* if we're on vertex elements, read them in */
    if (equal_strings ("vertex", elem_name)) 
	{

      /* create a vertex list to hold all the vertices */
      vlist = (Vertex **) malloc (sizeof (Vertex *) * num_elems);

      /* set up for getting vertex elements */

      ply_get_property (ply, elem_name, &vert_props[0]);
      ply_get_property (ply, elem_name, &vert_props[1]);
      ply_get_property (ply, elem_name, &vert_props[2]);

	  if(m_pPts!=NULL)
	  {
		  delete m_pPts;
		  m_pPts=NULL;
		  m_pPts=new POINT3D[num_elems];
	  }
	  else
		  m_pPts=new POINT3D[num_elems];

	  m_nPtSum=num_elems;

      /* grab all the vertex elements */
      for (j = 0; j < num_elems; j++)
	  {

        /* grab and element from the file */
        vlist[j] = (Vertex *) malloc (sizeof (Vertex));
        ply_get_element (ply, (void *) vlist[j]);

   		m_pPts[j].x=vlist[j]->x;
		m_pPts[j].y=vlist[j]->y;
		m_pPts[j].z=vlist[j]->z;
		m_pPts[j].IsNull=FALSE;

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

	  if(m_pFaces!=NULL)
	  {
		  delete m_pFaces;
		  m_pFaces=NULL;
		  m_pFaces=new stFACE[num_elems];
	  }
	  else
		  m_pFaces=new stFACE[num_elems];
	
	  m_nFaceSum=num_elems;

      /* grab all the face elements */
      for (j = 0; j < num_elems; j++) {

        /* grab and element from the file */
        flist[j] = (Face *) malloc (sizeof (Face));
        ply_get_element (ply, (void *) flist[j]);
 		m_pFaces[j].n=flist[j]->nverts;
		m_pFaces[j].IsNull=FALSE;
        for (k = 0; k < flist[j]->nverts; k++)
		{
			m_pFaces[j].vI[k]=flist[j]->verts[k];
		}
 		free(flist[j]);
      }
    }
    
  }

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
  ply_close (ply);
  return TRUE;
}

void CReadPLY::Render()
{
 	//glPushMatrix();
	//glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
	//glTranslatef(.0f,.0f,-1.0f);

		int colorR,colorG,colorB;

		for(int i=0;i<m_nFaceSum;i++)
		{
			int n=m_pFaces[i].n;

			glNormal3f((float)m_pNormals[i].x, (float)m_pNormals[i].y, (float)m_pNormals[i].z);
			glEnable(GL_NORMALIZE);
		
			srand( (unsigned)time( NULL ) );

			glBegin(GL_POLYGON);
			for(int j=0;j<n;j++)
				{
					colorR = int((rand()*j)%255);
					colorG = int((rand()*2*j)%255);
					colorB = int((rand()*7*j)%255);

					glColor3f((float)colorR, (float)colorG, (float)colorB);
					glColor3f(1.0f, 1.0f, 1.0f);
					int nv;
					nv=m_pFaces[i].vI[j];
					glVertex3f((float)m_pPts[nv].x, (float)m_pPts[nv].y, (float)m_pPts[nv].z);
				}
			glEnd();
		}
	//glPopMatrix();
}

void CReadPLY::enableLight(bool enable)
{
	if(enable)
	{	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	}
	else
	{	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	}
}
	

int CReadPLY::GetPointNum()
{
	return(m_nPtSum);
}


int CReadPLY::GetFaceNum()
{
	return(m_nFaceSum);
}

POINT3D* CReadPLY::GetPoints()
{
	return(m_pPts);
}

stFACE* CReadPLY::GetFaces()
{
	return(m_pFaces);
}

POINT3D* CReadPLY::GetNormal()
{
	return(m_pNormals);
}
