
/*   CPolyTri:  Polygon Triangulation
 *   "Fast Polygon Triangulation based on Seidel's Algorithm"
 *   Atul Narkhede , Dinesh Manocha. Department of Computer Science,
 *   UNC Chapel Hill.
 *   Encapsulated by Xiedonghai, 2005,1,12
 *  
 *
 *   How to use  
 *   Sample:
 *
 *   CPolyTri plyTri;
 *   plyTri.Init(fileName);   //fileName is the Contour file 
 *   int *v1 = new int[..];
 *   int *v2 = new int[..];
 *   int *v3 = new int[..];
 *   int nTriangle = plyTri.Triangulate(v1,v2,v3);
 *
 *
 *   Contour file Format:
 *        Including the number of contour, number of each contour 
 *        and the x ,y coordinate of each point on contour
 *   Remarks:
 *        the Outer contour points is couterclockwise
 *        the Inner contour points is clockwise
 *
 *   Sample:
 *   1      //the number of contour
 *   4      //the point number of first contour
 *   0.0  0.0   // x y
 *   6.0  0.0
 *   6.0  6.0
 *   0.0  6.0
 *   ...    //the point number of second contour
 *   .. 
 */


#ifndef POLYTRI_H
#define POLYTRI_H

#include "PolyTriGlobal.h"
 
class CPolyTri
{

public:
	CPolyTri();
	CPolyTri(const int nContour,const int *pCntr, const double* x, const double* y);
	~CPolyTri();

	void Init(char *fileName);
	void Init(const int nContour, const int *pCntr, const double* x, const double* y);

	int  GetContourNum() const;
	int  Triangulate(int *v1, int *v2, int *v3);

private:
	int read_segments(char *filename, int *genus);
	int  triangulate_polygon(int ncontours, const int *cntr,
                             const double *x, const double *y,
						     int *v1, 
						     int *v2,
						     int *v3);
	int  generate_random_ordering(int n);
	int  choose_segment();
	int  initialise(int n);
	int  triangulate_monotone_polygons(int nvert,int nmonpoly, 
								  int *v1, int *v2, int *v3);
	int  monotonate_trapezoids(int n);
	int  construct_trapezoids(int nseg);
	int  triangulate_single_polygon(int nvert,int posmax,int side,
									  int *v1, int *v2, int *v3);
	int inside_polygon(trap_t *t);
	int traverse_polygon(int mcur,int trnum,int from,int dir);
	int newmon()
	{
		return ++mon_idx;
	}

	int init_query_structure(int segnum);
	int add_segment(int segnum);
	int find_new_roots(int segnum);
	int make_new_monotone_poly(int mcur, int v0, int v1);
	int newnode();
	int newtrap();
	int inserted(int segnum, int whichpt);
	int is_left_of(int segnum, point_t *v);
	int merge_trapezoids( int segnum,int tfirst,int tlast,int side);
	int get_vertex_positions(int v0, int v1,int *ip, int *iq);
	int locate_endpoint(point_t *v, point_t *vo, int r);


	/* return a new chain element from the table */
	int new_chain_element()
	{
		return ++chain_idx;
	}
	double get_angle(point_t *vp0, point_t *vpnext, point_t *vp1);


    //////////////////////////////////////////////////////////////////////////
	//{{from extra library
	node_t     qs[QSIZE];	    	/* Query structure */
	trap_t     tr[TRSIZE];		    /* Trapezoid structure */
	segment_t  seg[SEGSIZE];		/* Segment table */
	int q_idx;
	int tr_idx;
	int choose_idx;
    int permute[SEGSIZE];

	
    monchain_t    mchain[TRSIZE]; /* Table to hold all the monotone */
				  /* polygons . Each monotone polygon */
				  /* is a circularly linked list */

    vertexchain_t vert[SEGSIZE]; /* chain init. information. This */
				    /* is used to decide which */
				    /* monotone polygon to split if */
				    /* there are several other */
				    /* polygons touching at the same */
				    /* vertex  */

    int mon[SEGSIZE];	/* contains position of any vertex in */
				/* the monotone chain for the polygon */
    int visited[TRSIZE];
    int chain_idx, op_idx, mon_idx;
	//}}from extra library
	
    //{{self-declared
    double  *m_pX;       //the x coordinate of point on the contour
	double  *m_pY;       //the y coordinate of point on the contour
	int     m_nContour;  //the number of Contour
	int     *m_pCntr;    //the point number of each Contour
	//}}self-declared

};


//global functions
int _max(point_t *yval,point_t *v0, point_t *v1);
int _min(point_t *yval, point_t *v0, point_t *v1);
int _equal_to(point_t *v0, point_t *v1);
int _less_than(point_t *v0, point_t *v1);
int _greater_than(point_t *v0, point_t *v1);
int _greater_than_equal_to(point_t *v0, point_t *v1);

int math_logstar_n(int n);
int math_N(int n, int h);


#endif