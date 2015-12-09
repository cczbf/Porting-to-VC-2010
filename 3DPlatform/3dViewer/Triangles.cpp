#include "stdafx.h"
#pragma optimize( "", off )
/* To insert lots of self-checks for internal errors, define the SELF_CHECK  */
/*   symbol.  This will slow down the program significantly.  It is best to  */
/*   define the symbol using the -DSELF_CHECK compiler switch, but you could */
/*   write "#define SELF_CHECK" below.  If you are modifying this code, I    */
/*   recommend you turn self-checks on.                                      */

/* #define SELF_CHECK */

/* To compile Triangle as a callable object library (TRIANGLE.o), define the */
/*   TRILIBRARY symbol.  Read the file TRIANGLE.h for details on how to call */
/*   the procedure triangulate() that results.                               */

#define TRILIBRARY

/* It is possible to generate a smaller version of Triangle using one or     */
/*   both of the following symbols.  Define the REDUCED symbol to eliminate  */
/*   all features that are primarily of research interest; specifically, the */
/*   -i, -F, -s, and -C switches.  Define the CDT_ONLY symbol to eliminate   */
/*   all meshing algorithms above and beyond constrained Delaunay            */
/*   triangulation; specifically, the -r, -q, -a, -S, and -s switches.       */
/*   These reductions are most likely to be useful when generating an object */
/*   library (TRIANGLE.o) by defining the TRILIBRARY symbol.                 */

/* #define REDUCED */
/* #define CDT_ONLY */

/* On some machines, the exact arithmetic routines might be defeated by the  */
/*   use of internal extended precision floating-triPOINT registers.  Sometimes */
/*   this problem can be fixed by defining certain values to be volatile,    */
/*   thus forcing them to be stored to memory and rounded off.  This isn't   */
/*   a great solution, though, as it slows Triangle down.                    */
/*                                                                           */
/* To try this out, write "#define INEXACT volatile" below.  Normally,       */
/*   however, INEXACT should be defined to be nothing.  ("#define INEXACT".) */

#define INEXACT /* Nothing */
/* #define INEXACT volatile */

/* Maximum number of characters in a file name (including the null).         */

#define FILENAMESIZE 512

/* Maximum number of characters in a line read from a file (including the    */
/*   null).                                                                  */

#define INPUTLINESIZE 512

/* For efficiency, a variety of data structures are allocated in bulk.  The  */
/*   following constants determine how many of each structure is allocated   */
/*   at once.                                                                */

#define TRIPERBLOCK 4096           /* Number of m_triangles allocated at once. */
#define SHELLEPERBLOCK 512       /* Number of shell edges allocated at once. */
#define POINTPERBLOCK 4096            /* Number of m_points allocated at once. */
#define VIRUSPERBLOCK 1024   /* Number of virus m_triangles allocated at once. */
/* Number of encroached segments allocated at once. */
#define BADSEGMENTPERBLOCK 256
/* Number of skinny m_triangles allocated at once. */
#define BADTRIPERBLOCK 4096
/* Number of splay tree nodes allocated at once. */
#define SPLAYNODEPERBLOCK 512

/* The triPOINT marker DEADPOINT is an arbitrary number chosen large enough to  */
/*   (hopefully) not conflict with user boundary markers.  Make sure that it */
/*   is small enough to fit into your machine's integer size.                */

#define DEADPOINT -1073741824

/* The next line is used to outsmart some very stupid compilers.  If your    */
/*   compiler is smarter, feel free to replace the "int" with "void".        */
/*   Not that it matters.                                                    */

/* #define void CTINClass::int
*/

/* Two constants for algorithms based on random sampling.  Both constants    */
/*   have been chosen empirically to optimize their respective algorithms.   */

/* Used for the triPOINT location scheme of Mucke, Saias, and Zhu, to decide    */
/*   how large a random sample of m_triangles to inspect.                      */
#define SAMPLEFACTOR 11
/* Used in Fortune's sweepline Delaunay algorithm to determine what fraction */
/*   of boundary edges should be maintained in the splay tree for triPOINT      */
/*   location on the front.                                                  */
#define SAMPLERATE 10

/* A number that speaks for itself, every kissable digit.                    */

#define PI 3.141592653589793238462643383279502884197169399375105820974944592308

/* Another fave.                                                             */

#define SQUAREROOTTWO 1.4142135623730950488016887242096980785696718753769480732

/* And here's one for those of you who are intimidated by math.              */

#define ONETHIRD 0.333333333333333333333333333333333333333333333333333333333333


#define NO_TIMER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef NO_TIMER
#include <sys/time.h>
#endif /* NO_TIMER */

#ifdef TRILIBRARY
#endif /* TRILIBRARY */
#include "random.h"

#include "Triangles.h"

/********* Mesh manipulation primitives begin here                   *********/
/**                                                                         **/
/**                                                                         **/

/* Fast lookup arrays to speed some of the mesh manipulation primitives.     */

int plus1mod3[3] = {1, 2, 0};
int minus1mod3[3] = {2, 0, 1};


/********* User interaction routines begin here                      *********/
/**                                                                         **/
/**                                                                         **/


/*****************************************************************************/
/*                                                                           */
/*  internalerror()   Ask the user to send me the defective product.  Exit.  */
/*                                                                           */
/*****************************************************************************/
//int AFXAPI AfxMessageBox( UINT nIDPrompt, UINT nType = MB_OK, UINT nIDHelp = (UINT) -1 );

//int AfxMessageBox( LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0 );


void CTINClass::internalerror()
{
	printf("  Please report this bug to jrs@cs.cmu.edu\n");
	printf("  Include the message above, your input data set, and the exact\n");
	printf("    command line you used to run Triangle.\n");

	AfxMessageBox( "TINDLL: internalerror ");
	exit(1);
}

/*****************************************************************************/
/*                                                                           */
/*  parsecommandline()   Read the command line, identify switches, and set   */
/*                       up options and file names.                          */
/*                                                                           */
/*  The effects of this routine are felt entirely through global variables.  */
/*                                                                           */
/*****************************************************************************/

void CTINClass::parsecommandline(int argc,char **argv)
{
    #define STARTINDEX 0
	int i, j, k;
	char workstring[FILENAMESIZE];
	
	m_firstnumber = 0;	
	
	m_poly = refine = quality = vararea = fixedarea = regionattrib = m_convex = false;

	edgesout = voronoi = neighbors = geomview = 0;
	nobound = nopolywritten = nonodewritten = noelewritten = noiterationnum = 0;
	noholes = noexact = 0;
	incremental = sweepline = 0;
	dwyer = 1;
	m_splitSeg = false;
	docheck = 0;
	nobisect = 0;
	steiner = -1;
	m_order = 1;
	m_minangle = 0.0;
	m_maxarea = -1.0;
	quiet = false;
	m_verbose = 0;
	
	for (i = STARTINDEX; i < argc; i++) 
	{
		for (j = STARTINDEX; argv[i][j] != '\0'; j++) 
		{
			switch( argv[i][j] ) {
			case 'p': m_poly = 1; break;
				
#ifndef CDT_ONLY
			case 'r':
				refine = 1;break;
				
			case  'q':
				quality = 1;
				if (((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) || (argv[i][j + 1] == '.')) {
					k = 0;
					while (((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) || (argv[i][j + 1] == '.')) {
						j++;
						workstring[k] = argv[i][j];
						k++;
					}
					
					workstring[k] = '\0';
					m_minangle = (REAL) strtod(workstring, (char **) NULL);
				}
				else 	m_minangle = 10.0;
				
				break;
				
			case 'a':
				quality = 1;
				if (((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) || (argv[i][j + 1] == '.')) {
					fixedarea = 1;
					k = 0;
					while (((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) || (argv[i][j + 1] == '.')) {
						j++;
						workstring[k] = argv[i][j];
						k++;
					}
					workstring[k] = '\0';
					m_maxarea = (REAL) strtod(workstring, (char **) NULL);
					if (m_maxarea <= 0.0) {
						printf("Error:  Maximum area must be greater than zero.\n");
						exit(1);
					}
				}
				else	vararea = 1;
				break;
#endif /* not CDT_ONLY */
				
			case 'A': regionattrib = 1;	break;
			case 'c': m_convex = 1;		break;
			case 'z': m_firstnumber = 0;	break;
			case 'e': edgesout = 1;		break;
			case 'v': voronoi = 1;		break;
			case 'n': neighbors = 1;	break;
			case 'g': geomview = 1;		break;
			case 'B': nobound = 1;		break;
			case 'P': nopolywritten = 1;	break;
			case 'N': nonodewritten = 1;	break;
			case 'E': noelewritten = 1;	break;
			case 'O': noholes = 1;		break;
			case 'X': noexact = 1;		break;
			case 'o':
				if (argv[i][j + 1] == '2') {
					j++;
					m_order = 2;
				}
				break;
				
#ifndef CDT_ONLY
			case 'Y': nobisect++;	break;
			case 'S':
				steiner = 0;
				while ((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) {
					j++;
					steiner = steiner * 10 + (int) (argv[i][j] - '0');
				}
				break;
#endif /* not CDT_ONLY */
				
#ifndef REDUCED
			case 'i':incremental = 1;break;
			case 'F':sweepline = 1;	break;
#endif /* not REDUCED */
				
			case 'l':dwyer = 0;	break;
				
#ifndef REDUCED
#ifndef CDT_ONLY
			case 's':m_splitSeg = true;	break;
#endif /* not CDT_ONLY */
			case 'C':docheck = 1;	break;
#endif /* not REDUCED */
			case 'Q':quiet = 1;	break;
			case 'V':m_verbose++;	break;
			default:
				printf("invalid option : %c\n", argv[i][j]);
			}
	}}
	
	steinerleft = steiner;
	m_useShelles = m_poly || refine || quality || m_convex;
	m_goodangle = cos(m_minangle * PI / 180.0);
	m_goodangle *= m_goodangle;
	if (refine && noiterationnum) {
		printf(	"Error:  You cannot use the -I switch when refining a triangulation.\n");
		exit(1);
	}
	/* Be careful not to allocate space for element area constraints that */
	/*   will never be assigned any value (other than the default -1.0).  */
	if (!refine && !m_poly) 
		vararea = 0;
	/* Be careful not to add an extra attribute to each element unless the */
	/*   input supports it (PSLG in, but not refining a preexisting mesh). */
	if (refine || !m_poly) 
		regionattrib = 0;
}

/**                                                                         **/
/**                                                                         **/
/********* User interaction routines begin here                      *********/

/********* Debugging routines begin here                             *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  printtriangle()   Print out the details of a TRIANGLE/EDGE handle.       */
/*                                                                           */
/*  I originally wrote this procedure to simplify debugging; it can be       */
/*  called directly from the debugger, and presents information about a      */
/*  TRIANGLE/EDGE handle in digestible form.  It's also used when the        */
/*  highest level of verbosity (`-VVV') is specified.                        */
/*                                                                           */
/*****************************************************************************/
void CTINClass::printtriangle(triEDGE *t)
{
	triEDGE printtri;
	EDGE printsh;
	triPOINT *printpoint;
	
	printf("TRIANGLE x%lx with orientation %d:\n", (unsigned long) t->tri,	t->orient);

	decode(t->tri, printtri);

	if (printtri.tri == m_dummytri) 
		printf("    [0] = Outer space\n");
	else 	printf("    [0] = x%lx  %d\n", (unsigned long) printtri.tri,	printtri.orient);
	
	decode(t->tri, printtri);
	if (printtri.tri == m_dummytri) 
		printf("    [1] = Outer space\n");
	else 	printf("    [1] = x%lx  %d\n", (unsigned long) printtri.tri,	printtri.orient);
	
	decode(t->tri, printtri);
	if (printtri.tri == m_dummytri) 
		printf("    [2] = Outer space\n");
	else 	printf("    [2] = x%lx  %d\n", (unsigned long) printtri.tri,	printtri.orient);
	
	printpoint = org( *t );
	if (printpoint == (triPOINT *) NULL)
		printf("    Origin[%d] = NULL\n", (t->orient + 1) % 3 + 3);
	else	printf("    Origin[%d] = x%lx  (%.12g, %.12g)\n",(t->orient + 1) % 3 + 3, (unsigned long) printpoint,printpoint->x, printpoint->y);
	
	printpoint = dest( *t );
	if (printpoint == (triPOINT *) NULL)
		printf("    Dest  [%d] = NULL\n", (t->orient + 2) % 3 + 3);
	else	printf("    Dest  [%d] = x%lx  (%.12g, %.12g)\n",(t->orient + 2) % 3 + 3, (unsigned long) printpoint,	printpoint->x, printpoint->y);
	
	printpoint = apex(*t );
	if (printpoint == (triPOINT *) NULL)
		printf("    Apex  [%d] = NULL\n", t->orient + 3);
	else	printf("    Apex  [%d] = x%lx  (%.12g, %.12g)\n",t->orient + 3, (unsigned long) printpoint,printpoint->x, printpoint->y);
	if (m_useShelles) {
		shDecode(t->tri->sh[0], printsh);
		if (printsh.sh != m_dummysh) 
			printf("    [6] = x%lx  %d\n", (unsigned long) printsh.sh,printsh.shorient);
		
		shDecode(t->tri->sh[1], printsh);
		if (printsh.sh != m_dummysh) 
			printf("    [7] = x%lx  %d\n", (unsigned long) printsh.sh,printsh.shorient);
		
		shDecode(t->tri->sh[2], printsh);
		if (printsh.sh != m_dummysh) 
			printf("    [8] = x%lx  %d\n", (unsigned long) printsh.sh,printsh.shorient);
	}
	if (vararea) 
		printf("    Area constraint:  %.4g\n", areabound(*t));
}

/*****************************************************************************/
/*                                                                           */
/*  printshelle()   Print out the details of a shell EDGE handle.            */
/*                                                                           */
/*  I originally wrote this procedure to simplify debugging; it can be       */
/*  called directly from the debugger, and presents information about a      */
/*  shell EDGE handle in digestible form.  It's also used when the highest   */
/*  level of verbosity (`-VVV') is specified.                                */
/*                                                                           */
/*****************************************************************************/

void CTINClass::printshelle(EDGE *s)
{
	EDGE printsh;
	triEDGE printtri;
	triPOINT *printpoint;
	
	printf("shell EDGE x%lx with orientation %d and mark %d:\n",
		(unsigned long) s->sh, s->shorient, shMark(*s));
	
	shDecode(s->sh->adjoin[0], printsh);
	if (printsh.sh == m_dummysh) {
		printf("    [0] = No shell\n");
	} else {
		printf("    [0] = x%lx  %d\n", (unsigned long) printsh.sh,
			printsh.shorient);
	}
	shDecode(s->sh->adjoin[1], printsh);
	if (printsh.sh == m_dummysh) {
		printf("    [1] = No shell\n");
	} else {
		printf("    [1] = x%lx  %d\n", (unsigned long) printsh.sh,
			printsh.shorient);
	}
	printpoint = shOrg(*s );
	if (printpoint == (triPOINT *) NULL)
		printf("    Origin[%d] = NULL\n", 2 + s->shorient);
	else
		printf("    Origin[%d] = x%lx  (%.12g, %.12g)\n",
		2 + s->shorient, (unsigned long) printpoint,
		printpoint->x, printpoint->y);
	printpoint = shDest(*s );
	if (printpoint == (triPOINT *) NULL)
		printf("    Dest  [%d] = NULL\n", 3 - s->shorient);
	else
		printf("    Dest  [%d] = x%lx  (%.12g, %.12g)\n",
		3 - s->shorient, (unsigned long) printpoint,
		printpoint->x, printpoint->y);
	
	decode(s->sh->tri[0], printtri);
	if (printtri.tri == m_dummytri) {
		printf("    [4] = Outer space\n");
	} else {
		printf("    [4] = x%lx  %d\n", (unsigned long) printtri.tri,
			printtri.orient);
	}
	decode(s->sh->tri[1], printtri);
	if (printtri.tri == m_dummytri) {
		printf("    [5] = Outer space\n");
	} else {
		printf("    [5] = x%lx  %d\n", (unsigned long) printtri.tri,
			printtri.orient);
	}
}

/**                                                                         **/
/**                                                                         **/
/********* Debugging routines end here                               *********/


/*****************************************************************************/
/*                                                                           */
/*  dummyinit()   Initialize the TRIANGLE that fills "outer space" and the   */
/*                omnipresent shell EDGE.                                    */
/*                                                                           */
/*  The TRIANGLE that fills "outer space", called `m_dummytri', is pointed to  */
/*  by every TRIANGLE and shell EDGE on a boundary (be it outer or inner) of */
/*  the triangulation.  Also, `m_dummytri' m_points to one of the m_triangles on   */
/*  the m_convex hull (until the holes and concavities are carved), making it  */
/*  possible to find a starting TRIANGLE for triPOINT location.                 */
/*                                                                           */
/*  The omnipresent shell EDGE, `m_dummysh', is pointed to by every TRIANGLE   */
/*  or shell EDGE that doesn't have a full complement of real shell edges    */
/*  to triPOINT to.                                                             */
/*                                                                           */
/*****************************************************************************/

void CTINClass::dummyinit(int trianglewords,int shellewords)
{
	/* Set up `m_dummytri', the `TRIANGLE' that occupies "outer space". */
	m_dummytribase = new TRIANGLE[1];
	if (m_dummytribase == (TRIANGLE *) NULL) {
		printf("Error:  Out of memory.\n");
		exit(1);
	}
	/* Align `m_dummytri' on a `m_triangles.GetAlignBytes()'-byte boundary. */
	m_dummytri = m_dummytribase;

	/* Initialize the three adjoining m_triangles to be "outer space".  These  */
	/*   will eventually be changed by various bonding operations, but their */
	/*   values don't really matter, as long as they can legally be          */
	/*   dereferenced.                                                       */
	m_dummytri->adjoin[0] = (TRIANGLE *)m_dummytri;
	m_dummytri->adjoin[1] = (TRIANGLE *)m_dummytri;
	m_dummytri->adjoin[2] = (TRIANGLE *)m_dummytri;
	/* Three NULL vertex m_points. */
	m_dummytri->vertex[0] = (triPOINT *) NULL;
	m_dummytri->vertex[1] = (triPOINT *) NULL;
	m_dummytri->vertex[2] = (triPOINT *) NULL;
	
	if (m_useShelles) {
		/* Set up `m_dummysh', the omnipresent "shell EDGE" pointed to by any      */
		/*   TRIANGLE side or shell EDGE end that isn't attached to a real shell */
		/*   EDGE.                                                               */
		m_dummyshbase = new SHELLE[1];
		if (m_dummyshbase == (SHELLE *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
		/* Align `m_dummysh' on a `m_shelles.GetAlignBytes()'-byte boundary. */
		m_dummysh = m_dummyshbase;
		/* Initialize the two adjoining shell edges to be the omnipresent shell */
		/*   EDGE.  These will eventually be changed by various bonding         */
		/*   operations, but their values don't really matter, as long as they  */
		/*   can legally be dereferenced.                                       */
		m_dummysh->adjoin[0] = m_dummysh;
		m_dummysh->adjoin[1] = m_dummysh;
		/* Two NULL vertex m_points. */
		m_dummysh->vertex[0] = (triPOINT *) NULL;
		m_dummysh->vertex[1] = (triPOINT *) NULL;
		/* Initialize the two adjoining m_triangles to be "outer space". */
		m_dummysh->tri[0] = m_dummytri;
		m_dummysh->tri[1] = m_dummytri;
		/* Set the boundary marker to zero. */
		m_dummysh->marker = 0;
		
		/* Initialize the three adjoining shell edges of `m_dummytri' to be */
		/*   the omnipresent shell EDGE.                                  */
		m_dummytri->sh[0] = m_dummysh;
		m_dummytri->sh[1] = m_dummysh;
		m_dummytri->sh[2] = m_dummysh;
	}
}

/*****************************************************************************/
/*                                                                           */
/*  initializepointpool()   Calculate the size of the triPOINT data structure   */
/*                          and initialize its memory pool.                  */
/*                                                                           */
/*  This routine also computes the `PointMarkindex' and `Point2Triindex'     */
/*  indices used to find values within each triPOINT.                           */
/*                                                                           */
/*****************************************************************************/

void CTINClass::initializepointpool()
{
	/* Initialize the pool of m_points. */
	m_points.PoolInit( POINTPERBLOCK );
}

/*****************************************************************************/
/*                                                                           */
/*  InitializeTriSegPools()   Calculate the sizes of the TRIANGLE and shell  */
/*                            EDGE data structures and initialize their      */
/*                            memory pools.                                  */
/*                                                                           */
/*  This routine also computes the `highorderindex', `elemattribindex', and  */
/*  `areaboundindex' indices used to find values within each TRIANGLE.       */
/*                                                                           */
/*****************************************************************************/
void CTINClass::InitializeTriSegPools()
{
	int trisize;
	
	switch( m_order ) {
	case 1:
		trisize = sizeof(struct tagTRIANGLE1);
		break;
	case 2:
		trisize = sizeof(struct tagTRIANGLE2);
		break;
	case 3:
		trisize = sizeof(tagTRIANGLE3);
		break;
	default:
		printf("m_order must be < 4 \n");
		exit(1);
	}
	
	m_triangles.PoolInit( TRIPERBLOCK );
	
	if (m_useShelles) {
		/* Initialize the pool of shell edges. */
		m_shelles.PoolInit(  SHELLEPERBLOCK );
		
		/* Initialize the "outer space" TRIANGLE and omnipresent shell EDGE. */
	} 
	dummyinit(0,0);


	m_fixuptri.tri = m_dummytri;

}

/*****************************************************************************/
/*                                                                           */
/*  triangleDealloc()   Deallocate space for a TRIANGLE, marking it dead.    */
/*                                                                           */
/*****************************************************************************/

void CTINClass::triangleDealloc(TRIANGLE *dyingtriangle)
{
	/* Set TRIANGLE's vertices to NULL.  This makes it possible to        */
	/*   detect dead triangles when traversing the list of all triangles. */
	dyingtriangle->vertex[0] = (triPOINT *) NULL;
	dyingtriangle->vertex[1] = (triPOINT *) NULL;
	dyingtriangle->vertex[2] = (triPOINT *) NULL;

	m_triangles.PoolDealloc( dyingtriangle );

	m_selectedChanged = true;
}

/*****************************************************************************/
/*                                                                           */
/*  TriangleTraverse()   Traverse the m_triangles, skipping dead ones.         */
/*                                                                           */
/*****************************************************************************/

TRIANGLE *CTINClass::TriangleTraverse()
{
	TRIANGLE *newtriangle;
	
	do {
		newtriangle = (TRIANGLE *) m_triangles.Traverse();
		if (newtriangle == NULL) {
			return NULL;
		}
	} while ( newtriangle->vertex[0] == NULL );            /* Skip dead ones. */
	return newtriangle;
}

/*****************************************************************************/
/*                                                                           */
/*  shelleDealloc()   Deallocate space for a shell EDGE, marking it dead.    */
/*                                                                           */
/*****************************************************************************/

void CTINClass::shelleDealloc(SHELLE *dyingshelle)
{
	/* Set shell EDGE's vertices to NULL.  This makes it possible to */
	/*   detect dead shells when traversing the list of all shells.  */
	dyingshelle->vertex[0] = (triPOINT *) NULL;
	dyingshelle->vertex[1] = (triPOINT *) NULL;
	m_shelles.PoolDealloc( dyingshelle);
}

/*****************************************************************************/
/*                                                                           */
/*  shelleTraverse()   Traverse the shell edges, skipping dead ones.         */
/*                                                                           */
/*****************************************************************************/

SHELLE *CTINClass::shelleTraverse()
{
	SHELLE *newshelle;
	
	do {
		newshelle = (SHELLE *) m_shelles.Traverse();
		if (newshelle == (SHELLE *) NULL) {
			return (SHELLE *) NULL;
		}
	} while ( newshelle->vertex[0] == (triPOINT *) NULL);                /* Skip dead ones. */
	return newshelle;
}


/*****************************************************************************/
/*                                                                           */
/*  PointTraverse()   Traverse the m_points, skipping dead ones.               */
/*                                                                           */
/*****************************************************************************/

triPOINT *CTINClass::PointTraverse()
{
	triPOINT *newpoint;
	
	do {
		newpoint = (triPOINT *) m_points.Traverse();
		if (newpoint == (triPOINT *) NULL) 
			return (triPOINT *) NULL;
	}	while ( PointMark(newpoint) == DEADPOINT    // Skip dead ones
		|| PointDup ( newpoint ) );    // skip dupicate points
	
	return newpoint;
}


/*****************************************************************************/
/*                                                                           */
/*  badsegmentdealloc()   Deallocate space for a bad segment, marking it     */
/*                        dead.                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

void CTINClass::badsegmentdealloc(EDGE *dyingseg)
{
	/* Set segment's orientation to -1.  This makes it possible to      */
	/*   detect dead segments when traversing the list of all segments. */
	dyingseg->shorient = -1;
	m_badSegments.PoolDealloc( dyingseg);
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  badsegmenttraverse()   Traverse the bad segments, skipping dead ones.    */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

EDGE *CTINClass::badsegmenttraverse()
{
	EDGE *newseg;
	
	do {
		newseg = (EDGE *)m_badSegments.Traverse();
		if (newseg == (EDGE *) NULL) {
			return (EDGE *) NULL;
		}
	} while (newseg->shorient == -1);                       /* Skip dead ones. */
	return newseg;
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  GetPoint()   Get a specific triPOINT, by number, from the list.             */
/*                                                                           */
/*  The first triPOINT is number 'm_firstnumber'.                                 */
/*                                                                           */
/*  Note that this takes O(n) time (with a small constant, if POINTPERBLOCK  */
/*  is large).  I don't care to take the trouble to make it work in constant */
/*  time.                                                                    */
/*                                                                           */
/*****************************************************************************/
triPOINT *CTINClass::GetPoint(int number)
{
	return m_points.GetItem( number );
}

/*****************************************************************************/
/*                                                                           */
/*  triangledeinit()   Free all remaining allocated memory.                  */
/*                                                                           */
/*****************************************************************************/

void CTINClass::triangledeinit()
{
	m_triangles.PoolDeinit();
	if( m_dummytribase ) {
		delete m_dummytribase;
		m_dummytribase = NULL;
	}
	if (m_useShelles) {
		m_shelles.PoolDeinit();
		if( m_dummyshbase ) {
			delete m_dummyshbase;
			m_dummyshbase = NULL;
		}
	}
	m_points.PoolDeinit();
#ifndef CDT_ONLY
	if (quality) {
		m_badSegments.PoolDeinit();
		if ((m_minangle > 0.0) || vararea || fixedarea) {
			m_badTriangles.PoolDeinit();
		}
	}
#endif /* not CDT_ONLY */
}

/**                                                                         **/
/**                                                                         **/
/********* Memory management routines end here                       *********/

/********* Constructors begin here                                   *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  maketriangle()   Create a new TRIANGLE with orientation zero.            */
/*                                                                           */
/*****************************************************************************/

void CTINClass::maketriangle(triEDGE *newtriedge)
{
	int i;
	
	m_selectedChanged = true;

	newtriedge->tri = (TRIANGLE *) m_triangles.PoolAlloc();
	/* Initialize the three adjoining m_triangles to be "outer space". */
	newtriedge->tri->adjoin[0] = (TRIANGLE *) m_dummytri;
	newtriedge->tri->adjoin[1] = (TRIANGLE *) m_dummytri;
	newtriedge->tri->adjoin[2] = (TRIANGLE *) m_dummytri;
	/* Three NULL vertex m_points. */
	newtriedge->tri->vertex[0] = (triPOINT *) NULL;
	newtriedge->tri->vertex[1] = (triPOINT *) NULL;
	newtriedge->tri->vertex[2] = (triPOINT *) NULL;
	/* Initialize the three adjoining shell edges to be the omnipresent */
	/*   shell EDGE.                                                    */
	if (m_useShelles) {
		newtriedge->tri->sh[0] = m_dummysh;
		newtriedge->tri->sh[1] = m_dummysh;
		newtriedge->tri->sh[2] = m_dummysh;
	}
	for (i = 0; i < eextras; i++)
		setelemattribute(*newtriedge, i, 0.0);
	if (vararea) 
		setareabound(*newtriedge, -1.0);
	
	newtriedge->orient = 0;
}

/*****************************************************************************/
/*                                                                           */
/*  makeshelle()   Create a new shell EDGE with orientation zero.            */
/*                                                                           */
/*****************************************************************************/

void CTINClass::makeshelle(EDGE *newedge)
{
	newedge->sh = (SHELLE *) m_shelles.PoolAlloc();
	/* Initialize the two adjoining shell edges to be the omnipresent */
	/*   shell EDGE.                                                  */
	newedge->sh->adjoin[0] = (SHELLE *) m_dummysh;
	newedge->sh->adjoin[1] = (SHELLE *) m_dummysh;
	/* Two NULL vertex m_points. */
	newedge->sh->vertex[0] = (triPOINT *) NULL;
	newedge->sh->vertex[1] = (triPOINT *) NULL;
	/* Initialize the two adjoining m_triangles to be "outer space". */
	newedge->sh->tri[0] = (TRIANGLE *) m_dummytri;
	newedge->sh->tri[1] = (TRIANGLE *) m_dummytri;
	/* Set the boundary marker to zero. */
	SetShellMark(*newedge, 0);
	
	newedge->shorient = 0;
}

/**                                                                         **/
/**                                                                         **/
/********* Constructors end here                                     *********/

/********* Determinant evaluation routines begin here                *********/
/**                                                                         **/
/**                                                                         **/

/* The adaptive exact arithmetic geometric predicates implemented herein are */
/*   described in detail in my Technical Report CMU-CS-96-140.  The complete */
/*   reference is given in the header.                                       */

/* Which of the following two methods of finding the absolute values is      */
/*   fastest is compiler-dependent.  A few compilers can inline and optimize */
/*   the fabs() call; but most will incur the overhead of a function call,   */
/*   which is disastrously slow.  A faster way on IEEE machines might be to  */
/*   mask the appropriate bit, but that's difficult to do in C.              */

#define Absolute(a)  ((a) >= 0.0 ? (a) : -(a))
/* #define Absolute(a)  fabs(a) */

/* Many of the operations are broken up into two pieces, a main part that    */
/*   performs an approximate operation, and a "tail" that computes the       */
/*   roundoff error of that operation.                                       */
/*                                                                           */
/* The operations Fast_Two_Sum(), Fast_Two_Diff(), Two_Sum(), Two_Diff(),    */
/*   Split(), and Two_Product() are all implemented as described in the      */
/*   reference.  Each of these macros requires certain variables to be       */
/*   defined in the calling routine.  The variables `bvirt', `c', `abig',    */
/*   `_i', `_j', `_k', `_l', `_m', and `_n' are declared `INEXACT' because   */
/*   they store the result of an operation that may incur roundoff error.    */
/*   The input parameter `x' (or the highest numbered `x_' parameter) must   */
/*   also be declared `INEXACT'.                                             */

#define Fast_Two_Sum_Tail(a, b, x, y) \
	bvirt = x - a; \
y = b - bvirt

#define Fast_Two_Sum(a, b, x, y) \
	x = (REAL) (a + b); \
Fast_Two_Sum_Tail(a, b, x, y)

#define Two_Sum_Tail(a, b, x, y) \
	bvirt = (REAL) (x - a); \
	avirt = x - bvirt; \
	bround = b - bvirt; \
	around = a - avirt; \
y = around + bround

#define Two_Sum(a, b, x, y) \
	x = (REAL) (a + b); \
Two_Sum_Tail(a, b, x, y)

#define Two_Diff_Tail(a, b, x, y) \
	bvirt = (REAL) (a - x); \
	avirt = x + bvirt; \
	bround = bvirt - b; \
	around = a - avirt; \
y = around + bround

#define Two_Diff(a, b, x, y) \
	x = (REAL) (a - b); \
Two_Diff_Tail(a, b, x, y)

#define Split(a, ahi, alo) \
	c = (REAL) (splitter * a); \
	abig = (REAL) (c - a); \
	ahi = c - abig; \
alo = a - ahi

#define Two_Product_Tail(a, b, x, y) \
	Split(a, ahi, alo); \
	Split(b, bhi, blo); \
	err1 = x - (ahi * bhi); \
	err2 = err1 - (alo * bhi); \
	err3 = err2 - (ahi * blo); \
y = (alo * blo) - err3

#define Two_Product(a, b, x, y) \
	x = (REAL) (a * b); \
Two_Product_Tail(a, b, x, y)

/* Two_Product_Presplit() is Two_Product() where one of the inputs has       */
/*   already been split.  Avoids redundant splitting.                        */

#define Two_Product_Presplit(a, b, bhi, blo, x, y) \
	x = (REAL) (a * b); \
	Split(a, ahi, alo); \
	err1 = x - (ahi * bhi); \
	err2 = err1 - (alo * bhi); \
	err3 = err2 - (ahi * blo); \
y = (alo * blo) - err3

/* Square() can be done more quickly than Two_Product().                     */

#define Square_Tail(a, x, y) \
	Split(a, ahi, alo); \
	err1 = x - (ahi * ahi); \
	err3 = err1 - ((ahi + ahi) * alo); \
y = (alo * alo) - err3

#define Square(a, x, y) \
	x = (REAL) (a * a); \
Square_Tail(a, x, y)

/* Macros for summing expansions of various fixed lengths.  These are all    */
/*   unrolled versions of Expansion_Sum().                                   */

#define Two_One_Sum(a1, a0, b, x2, x1, x0) \
	Two_Sum(a0, b , _i, x0); \
Two_Sum(a1, _i, x2, x1)

#define Two_One_Diff(a1, a0, b, x2, x1, x0) \
	Two_Diff(a0, b , _i, x0); \
Two_Sum( a1, _i, x2, x1)

#define Two_Two_Sum(a1, a0, b1, b0, x3, x2, x1, x0) \
	Two_One_Sum(a1, a0, b0, _j, _0, x0); \
Two_One_Sum(_j, _0, b1, x3, x2, x1)

#define Two_Two_Diff(a1, a0, b1, b0, x3, x2, x1, x0) \
	Two_One_Diff(a1, a0, b0, _j, _0, x0); \
Two_One_Diff(_j, _0, b1, x3, x2, x1)

/*****************************************************************************/
/*                                                                           */
/*  exactinit()   Initialize the variables used for exact arithmetic.        */
/*                                                                           */
/*  `epsilon' is the largest power of two such that 1.0 + epsilon = 1.0 in   */
/*  floating-triPOINT arithmetic.  `epsilon' bounds the relative roundoff       */
/*  error.  It is used for floating-triPOINT error analysis.                    */
/*                                                                           */
/*  `splitter' is used to split floating-triPOINT numbers into two half-        */
/*  length significands for exact multiplication.                            */
/*                                                                           */
/*  I imagine that a highly optimizing compiler might be too smart for its   */
/*  own good, and somehow cause this routine to fail, if it pretends that    */
/*  floating-triPOINT arithmetic is too much like real arithmetic.              */
/*                                                                           */
/*  Don't change this routine unless you fully understand it.                */
/*                                                                           */
/*****************************************************************************/
void CTINClass::exactinit()
{
	REAL half;
	REAL check, lastcheck;
	int every_other;
	
	every_other = 1;
	half = 0.5;
	epsilon = 1.0;
	splitter = 1.0;
	check = 1.0;
	/* Repeatedly divide `epsilon' by two until it is too small to add to      */
	/*   one without causing roundoff.  (Also check if the sum is equal to     */
	/*   the previous sum, for machines that round up instead of using exact   */
	/*   rounding.  Not that these routines will work on such machines anyway. */
	do {
		lastcheck = check;
		epsilon *= half;
		if (every_other) {
			splitter *= 2.0;
		}
		every_other = !every_other;
		check = 1.0 + epsilon;
	} while ((check != 1.0) && (check != lastcheck));
	splitter += 1.0;
	if (m_verbose > 1) {
		printf("Floating triPOINT roundoff is of magnitude %.17g\n", epsilon);
		printf("Floating triPOINT splitter is %.17g\n", splitter);
	}
	/* Error bounds for orientation and incircle tests. */
	resulterrbound = (3.0 + 8.0 * epsilon) * epsilon;
	ccwerrboundA = (3.0 + 16.0 * epsilon) * epsilon;
	ccwerrboundB = (2.0 + 12.0 * epsilon) * epsilon;
	ccwerrboundC = (9.0 + 64.0 * epsilon) * epsilon * epsilon;
	iccerrboundA = (10.0 + 96.0 * epsilon) * epsilon;
	iccerrboundB = (4.0 + 48.0 * epsilon) * epsilon;
	iccerrboundC = (44.0 + 576.0 * epsilon) * epsilon * epsilon;
}

/*****************************************************************************/
/*                                                                           */
/*  fast_expansion_sum_zeroelim()   Sum two expansions, eliminating zero     */
/*                                  components from the output expansion.    */
/*                                                                           */
/*  Sets h = e + f.  See my Robust Predicates paper for details.             */
/*                                                                           */
/*  If round-to-even is used (as with IEEE 754), maintains the strongly      */
/*  nonoverlapping property.  (That is, if e is strongly nonoverlapping, h   */
/*  will be also.)  Does NOT maintain the nonoverlapping or nonadjacent      */
/*  properties.                                                              */
/*                                                                           */
/*****************************************************************************/

int CTINClass::fast_expansion_sum_zeroelim(int elen,REAL *e,int flen,REAL *f,REAL *h)
/* h cannot be e or f. */
{
	REAL Q;
	INEXACT REAL Qnew;
	INEXACT REAL hh;
	INEXACT REAL bvirt;
	REAL avirt, bround, around;
	int eindex, findex, hindex;
	REAL enow, fnow;
	
	enow = e[0];
	fnow = f[0];
	eindex = findex = 0;
	if ((fnow > enow) == (fnow > -enow)) {
		Q = enow;
		enow = e[++eindex];
	} else {
		Q = fnow;
		fnow = f[++findex];
	}
	hindex = 0;
	if ((eindex < elen) && (findex < flen)) {
		if ((fnow > enow) == (fnow > -enow)) {
			Fast_Two_Sum(enow, Q, Qnew, hh);
			enow = e[++eindex];
		} else {
			Fast_Two_Sum(fnow, Q, Qnew, hh);
			fnow = f[++findex];
		}
		Q = Qnew;
		if (hh != 0.0) {
			h[hindex++] = hh;
		}
		while ((eindex < elen) && (findex < flen)) {
			if ((fnow > enow) == (fnow > -enow)) {
				Two_Sum(Q, enow, Qnew, hh);
				enow = e[++eindex];
			} else {
				Two_Sum(Q, fnow, Qnew, hh);
				fnow = f[++findex];
			}
			Q = Qnew;
			if (hh != 0.0) {
				h[hindex++] = hh;
			}
		}
	}
	while (eindex < elen) {
		Two_Sum(Q, enow, Qnew, hh);
		enow = e[++eindex];
		Q = Qnew;
		if (hh != 0.0) {
			h[hindex++] = hh;
		}
	}
	while (findex < flen) {
		Two_Sum(Q, fnow, Qnew, hh);
		fnow = f[++findex];
		Q = Qnew;
		if (hh != 0.0) {
			h[hindex++] = hh;
		}
	}
	if ((Q != 0.0) || (hindex == 0)) {
		h[hindex++] = Q;
	}
	return hindex;
}

/*****************************************************************************/
/*                                                                           */
/*  scale_expansion_zeroelim()   Multiply an expansion by a scalar,          */
/*                               eliminating zero components from the        */
/*                               output expansion.                           */
/*                                                                           */
/*  Sets h = be.  See my Robust Predicates paper for details.                */
/*                                                                           */
/*  Maintains the nonoverlapping property.  If round-to-even is used (as     */
/*  with IEEE 754), maintains the strongly nonoverlapping and nonadjacent    */
/*  properties as well.  (That is, if e has one of these properties, so      */
/*  will h.)                                                                 */
/*                                                                           */
/*****************************************************************************/

int CTINClass::scale_expansion_zeroelim(int elen,REAL *e,REAL b,REAL *h)
/* e and h cannot be the same. */
{
	INEXACT REAL Q, sum;
	REAL hh;
	INEXACT REAL product1;
	REAL product0;
	int eindex, hindex;
	REAL enow;
	INEXACT REAL bvirt;
	REAL avirt, bround, around;
	INEXACT REAL c;
	INEXACT REAL abig;
	REAL ahi, alo, bhi, blo;
	REAL err1, err2, err3;
	
	Split(b, bhi, blo);
	Two_Product_Presplit(e[0], b, bhi, blo, Q, hh);
	hindex = 0;
	if (hh != 0) {
		h[hindex++] = hh;
	}
	for (eindex = 1; eindex < elen; eindex++) {
		enow = e[eindex];
		Two_Product_Presplit(enow, b, bhi, blo, product1, product0);
		Two_Sum(Q, product0, sum, hh);
		if (hh != 0) {
			h[hindex++] = hh;
		}
		Fast_Two_Sum(product1, sum, Q, hh);
		if (hh != 0) {
			h[hindex++] = hh;
		}
	}
	if ((Q != 0.0) || (hindex == 0)) {
		h[hindex++] = Q;
	}
	return hindex;
}

/*****************************************************************************/
/*                                                                           */
/*  estimate()   Produce a one-word estimate of an expansion's value.        */
/*                                                                           */
/*  See my Robust Predicates paper for details.                              */
/*                                                                           */
/*****************************************************************************/

REAL CTINClass::estimate(int elen,REAL *e)
{
	REAL Q;
	int eindex;
	
	Q = e[0];
	for (eindex = 1; eindex < elen; eindex++) {
		Q += e[eindex];
	}
	return Q;
}

/*****************************************************************************/
/*                                                                           */
/*  counterclockwise()   Return a positive value if the m_points pa, pb, and   */
/*                       pc occur in counterclockwise m_order; a negative      */
/*                       value if they occur in clockwise m_order; and zero    */
/*                       if they are collinear.  The result is also a rough  */
/*                       approximation of twice the signed area of the       */
/*                       TRIANGLE defined by the three m_points.               */
/*                                                                           */
/*  Uses exact arithmetic if necessary to ensure a correct answer.  The      */
/*  result returned is the determinant of a matrix.  This determinant is     */
/*  computed adaptively, in the sense that exact arithmetic is used only to  */
/*  the degree it is needed to ensure that the returned value has the        */
/*  correct sign.  Hence, this function is usually quite fast, but will run  */
/*  more slowly when the input m_points are collinear or nearly so.            */
/*                                                                           */
/*  See my Robust Predicates paper for details.                              */
/*                                                                           */
/*****************************************************************************/

REAL CTINClass::counterclockwiseadapt(triPOINT *pa,triPOINT *pb,triPOINT *pc,REAL detsum)
{
	INEXACT REAL acx, acy, bcx, bcy;
	REAL acxtail, acytail, bcxtail, bcytail;
	INEXACT REAL detleft, detright;
	REAL detlefttail, detrighttail;
	REAL det, errbound;
	REAL B[4], C1[8], C2[12], D[16];
	INEXACT REAL B3;
	int C1length, C2length, Dlength;
	REAL u[4];
	INEXACT REAL u3;
	INEXACT REAL s1, t1;
	REAL s0, t0;
	
	INEXACT REAL bvirt;
	REAL avirt, bround, around;
	INEXACT REAL c;
	INEXACT REAL abig;
	REAL ahi, alo, bhi, blo;
	REAL err1, err2, err3;
	INEXACT REAL _i, _j;
	REAL _0;
	
	acx = (REAL) (pa->x - pc->x);
	bcx = (REAL) (pb->x - pc->x);
	acy = (REAL) (pa->y - pc->y);
	bcy = (REAL) (pb->y - pc->y);
	
	Two_Product(acx, bcy, detleft, detlefttail);
	Two_Product(acy, bcx, detright, detrighttail);
	
	Two_Two_Diff(detleft, detlefttail, detright, detrighttail,
		B3, B[2], B[1], B[0]);
	B[3] = B3;
	
	det = estimate(4, B);
	errbound = ccwerrboundB * detsum;
	if ((det >= errbound) || (-det >= errbound)) {
		return det;
	}
	
	Two_Diff_Tail(pa->x, pc->x, acx, acxtail);
	Two_Diff_Tail(pb->x, pc->x, bcx, bcxtail);
	Two_Diff_Tail(pa->y, pc->y, acy, acytail);
	Two_Diff_Tail(pb->y, pc->y, bcy, bcytail);
	
	if ((acxtail == 0.0) && (acytail == 0.0)
		&& (bcxtail == 0.0) && (bcytail == 0.0)) {
		return det;
	}
	
	errbound = ccwerrboundC * detsum + resulterrbound * Absolute(det);
	det += (acx * bcytail + bcy * acxtail)
		- (acy * bcxtail + bcx * acytail);
	if ((det >= errbound) || (-det >= errbound)) {
		return det;
	}
	
	Two_Product(acxtail, bcy, s1, s0);
	Two_Product(acytail, bcx, t1, t0);
	Two_Two_Diff(s1, s0, t1, t0, u3, u[2], u[1], u[0]);
	u[3] = u3;
	C1length = fast_expansion_sum_zeroelim(4, B, 4, u, C1);
	
	Two_Product(acx, bcytail, s1, s0);
	Two_Product(acy, bcxtail, t1, t0);
	Two_Two_Diff(s1, s0, t1, t0, u3, u[2], u[1], u[0]);
	u[3] = u3;
	C2length = fast_expansion_sum_zeroelim(C1length, C1, 4, u, C2);
	
	Two_Product(acxtail, bcytail, s1, s0);
	Two_Product(acytail, bcxtail, t1, t0);
	Two_Two_Diff(s1, s0, t1, t0, u3, u[2], u[1], u[0]);
	u[3] = u3;
	Dlength = fast_expansion_sum_zeroelim(C2length, C2, 4, u, D);
	
	return(D[Dlength - 1]);
}


REAL CTINClass::counterclockwise(triPOINT * pa,triPOINT * pb,triPOINT * pc)
{
	REAL detleft, detright, det;
	REAL detsum, errbound;
	
	counterclockcount++;
	
	detleft = (pa->x - pc->x) * (pb->y - pc->y);
	detright = (pa->y - pc->y) * (pb->x - pc->x);
	det = detleft - detright;
	
	if(noexact) 
		return det;
	
	if (detleft > 0.0) {
		if (detright <= 0.0) 
			return det;
		else 	detsum = detleft + detright;
	}
	else if (detleft < 0.0) {
		if (detright >= 0.0) 
			return det;
		else 	detsum = -detleft - detright;
	} 
	else 	return det;
	
	errbound = ccwerrboundA * detsum;
	if ((det >= errbound) || (-det >= errbound)) 
		return det;
	
	return counterclockwiseadapt(pa, pb, pc, detsum);
}

/*****************************************************************************/
/*                                                                           */
/*  incircle()   Return a positive value if the point pd lies inside the     */
/*               circle passing through pa, pb, and pc; a negative value if  */
/*               it lies outside; and zero if the four points are cocircular.*/
/*               The points pa, pb, and pc must be in counterclockwise       */
/*               order, or the sign of the result will be reversed.          */
/*                                                                           */
/*  Uses exact arithmetic if necessary to ensure a correct answer.  The      */
/*  result returned is the determinant of a matrix.  This determinant is     */
/*  computed adaptively, in the sense that exact arithmetic is used only to  */
/*  the degree it is needed to ensure that the returned value has the        */
/*  correct sign.  Hence, this function is usually quite fast, but will run  */
/*  more slowly when the input points are cocircular or nearly so.           */
/*                                                                           */
/*  See my Robust Predicates paper for details.                              */
/*                                                                           */
/*****************************************************************************/

REAL CTINClass::incircleadapt(triPOINT * pa,triPOINT * pb,triPOINT * pc,triPOINT * pd,REAL permanent)
{
	INEXACT REAL adx, bdx, cdx, ady, bdy, cdy;
	REAL det, errbound;
	
	INEXACT REAL bdxcdy1, cdxbdy1, cdxady1, adxcdy1, adxbdy1, bdxady1;
	REAL bdxcdy0, cdxbdy0, cdxady0, adxcdy0, adxbdy0, bdxady0;
	REAL bc[4], ca[4], ab[4];
	INEXACT REAL bc3, ca3, ab3;
	REAL axbc[8], axxbc[16], aybc[8], ayybc[16], adet[32];
	int axbclen, axxbclen, aybclen, ayybclen, alen;
	REAL bxca[8], bxxca[16], byca[8], byyca[16], bdet[32];
	int bxcalen, bxxcalen, bycalen, byycalen, blen;
	REAL cxab[8], cxxab[16], cyab[8], cyyab[16], cdet[32];
	int cxablen, cxxablen, cyablen, cyyablen, clen;
	REAL abdet[64];
	int ablen;
	REAL fin1[1152], fin2[1152];
	REAL *finnow, *finother, *finswap;
	int finlength;
	
	REAL adxtail, bdxtail, cdxtail, adytail, bdytail, cdytail;
	INEXACT REAL adxadx1, adyady1, bdxbdx1, bdybdy1, cdxcdx1, cdycdy1;
	REAL adxadx0, adyady0, bdxbdx0, bdybdy0, cdxcdx0, cdycdy0;
	REAL aa[4], bb[4], cc[4];
	INEXACT REAL aa3, bb3, cc3;
	INEXACT REAL ti1, tj1;
	REAL ti0, tj0;
	REAL u[4], v[4];
	INEXACT REAL u3, v3;
	REAL temp8[8], temp16a[16], temp16b[16], temp16c[16];
	REAL temp32a[32], temp32b[32], temp48[48], temp64[64];
	int temp8len, temp16alen, temp16blen, temp16clen;
	int temp32alen, temp32blen, temp48len, temp64len;
	REAL axtbb[8], axtcc[8], aytbb[8], aytcc[8];
	int axtbblen, axtcclen, aytbblen, aytcclen;
	REAL bxtaa[8], bxtcc[8], bytaa[8], bytcc[8];
	int bxtaalen, bxtcclen, bytaalen, bytcclen;
	REAL cxtaa[8], cxtbb[8], cytaa[8], cytbb[8];
	int cxtaalen, cxtbblen, cytaalen, cytbblen;
	REAL axtbc[8], aytbc[8], bxtca[8], bytca[8], cxtab[8], cytab[8];
	int axtbclen, aytbclen, bxtcalen, bytcalen, cxtablen, cytablen;
	REAL axtbct[16], aytbct[16], bxtcat[16], bytcat[16], cxtabt[16], cytabt[16];
	int axtbctlen, aytbctlen, bxtcatlen, bytcatlen, cxtabtlen, cytabtlen;
	REAL axtbctt[8], aytbctt[8], bxtcatt[8];
	REAL bytcatt[8], cxtabtt[8], cytabtt[8];
	int axtbcttlen, aytbcttlen, bxtcattlen, bytcattlen, cxtabttlen, cytabttlen;
	REAL abt[8], bct[8], cat[8];
	int abtlen, bctlen, catlen;
	REAL abtt[4], bctt[4], catt[4];
	int abttlen, bcttlen, cattlen;
	INEXACT REAL abtt3, bctt3, catt3;
	REAL negate;
	
	INEXACT REAL bvirt;
	REAL avirt, bround, around;
	INEXACT REAL c;
	INEXACT REAL abig;
	REAL ahi, alo, bhi, blo;
	REAL err1, err2, err3;
	INEXACT REAL _i, _j;
	REAL _0;
	
	adx = (REAL) (pa->x - pd->x);
	bdx = (REAL) (pb->x - pd->x);
	cdx = (REAL) (pc->x - pd->x);
	ady = (REAL) (pa->y - pd->y);
	bdy = (REAL) (pb->y - pd->y);
	cdy = (REAL) (pc->y - pd->y);
	
	Two_Product(bdx, cdy, bdxcdy1, bdxcdy0);
	Two_Product(cdx, bdy, cdxbdy1, cdxbdy0);
	Two_Two_Diff(bdxcdy1, bdxcdy0, cdxbdy1, cdxbdy0, bc3, bc[2], bc[1], bc[0]);
	bc[3] = bc3;
	axbclen = scale_expansion_zeroelim(4, bc, adx, axbc);
	axxbclen = scale_expansion_zeroelim(axbclen, axbc, adx, axxbc);
	aybclen = scale_expansion_zeroelim(4, bc, ady, aybc);
	ayybclen = scale_expansion_zeroelim(aybclen, aybc, ady, ayybc);
	alen = fast_expansion_sum_zeroelim(axxbclen, axxbc, ayybclen, ayybc, adet);
	
	Two_Product(cdx, ady, cdxady1, cdxady0);
	Two_Product(adx, cdy, adxcdy1, adxcdy0);
	Two_Two_Diff(cdxady1, cdxady0, adxcdy1, adxcdy0, ca3, ca[2], ca[1], ca[0]);
	ca[3] = ca3;
	bxcalen = scale_expansion_zeroelim(4, ca, bdx, bxca);
	bxxcalen = scale_expansion_zeroelim(bxcalen, bxca, bdx, bxxca);
	bycalen = scale_expansion_zeroelim(4, ca, bdy, byca);
	byycalen = scale_expansion_zeroelim(bycalen, byca, bdy, byyca);
	blen = fast_expansion_sum_zeroelim(bxxcalen, bxxca, byycalen, byyca, bdet);
	
	Two_Product(adx, bdy, adxbdy1, adxbdy0);
	Two_Product(bdx, ady, bdxady1, bdxady0);
	Two_Two_Diff(adxbdy1, adxbdy0, bdxady1, bdxady0, ab3, ab[2], ab[1], ab[0]);
	ab[3] = ab3;
	cxablen = scale_expansion_zeroelim(4, ab, cdx, cxab);
	cxxablen = scale_expansion_zeroelim(cxablen, cxab, cdx, cxxab);
	cyablen = scale_expansion_zeroelim(4, ab, cdy, cyab);
	cyyablen = scale_expansion_zeroelim(cyablen, cyab, cdy, cyyab);
	clen = fast_expansion_sum_zeroelim(cxxablen, cxxab, cyyablen, cyyab, cdet);
	
	ablen = fast_expansion_sum_zeroelim(alen, adet, blen, bdet, abdet);
	finlength = fast_expansion_sum_zeroelim(ablen, abdet, clen, cdet, fin1);
	
	det = estimate(finlength, fin1);
	errbound = iccerrboundB * permanent;
	if ((det >= errbound) || (-det >= errbound)) {
		return det;
	}
	
	Two_Diff_Tail(pa->x, pd->x, adx, adxtail);
	Two_Diff_Tail(pa->y, pd->y, ady, adytail);
	Two_Diff_Tail(pb->x, pd->x, bdx, bdxtail);
	Two_Diff_Tail(pb->y, pd->y, bdy, bdytail);
	Two_Diff_Tail(pc->x, pd->x, cdx, cdxtail);
	Two_Diff_Tail(pc->y, pd->y, cdy, cdytail);
	if ((adxtail == 0.0) && (bdxtail == 0.0) && (cdxtail == 0.0)
		&& (adytail == 0.0) && (bdytail == 0.0) && (cdytail == 0.0)) {
		return det;
	}
	
	errbound = iccerrboundC * permanent + resulterrbound * Absolute(det);
	det += ((adx * adx + ady * ady) * ((bdx * cdytail + cdy * bdxtail)
		- (bdy * cdxtail + cdx * bdytail))
		+ 2.0 * (adx * adxtail + ady * adytail) * (bdx * cdy - bdy * cdx))
		+ ((bdx * bdx + bdy * bdy) * ((cdx * adytail + ady * cdxtail)
		- (cdy * adxtail + adx * cdytail))
		+ 2.0 * (bdx * bdxtail + bdy * bdytail) * (cdx * ady - cdy * adx))
		+ ((cdx * cdx + cdy * cdy) * ((adx * bdytail + bdy * adxtail)
		- (ady * bdxtail + bdx * adytail))
		+ 2.0 * (cdx * cdxtail + cdy * cdytail) * (adx * bdy - ady * bdx));
	if ((det >= errbound) || (-det >= errbound)) {
		return det;
	}
	
	finnow = fin1;
	finother = fin2;
	
	if ((bdxtail != 0.0) || (bdytail != 0.0)
		|| (cdxtail != 0.0) || (cdytail != 0.0)) {
		Square(adx, adxadx1, adxadx0);
		Square(ady, adyady1, adyady0);
		Two_Two_Sum(adxadx1, adxadx0, adyady1, adyady0, aa3, aa[2], aa[1], aa[0]);
		aa[3] = aa3;
	}
	if ((cdxtail != 0.0) || (cdytail != 0.0)
		|| (adxtail != 0.0) || (adytail != 0.0)) {
		Square(bdx, bdxbdx1, bdxbdx0);
		Square(bdy, bdybdy1, bdybdy0);
		Two_Two_Sum(bdxbdx1, bdxbdx0, bdybdy1, bdybdy0, bb3, bb[2], bb[1], bb[0]);
		bb[3] = bb3;
	}
	if ((adxtail != 0.0) || (adytail != 0.0)
		|| (bdxtail != 0.0) || (bdytail != 0.0)) {
		Square(cdx, cdxcdx1, cdxcdx0);
		Square(cdy, cdycdy1, cdycdy0);
		Two_Two_Sum(cdxcdx1, cdxcdx0, cdycdy1, cdycdy0, cc3, cc[2], cc[1], cc[0]);
		cc[3] = cc3;
	}
	
	if (adxtail != 0.0) {
		axtbclen = scale_expansion_zeroelim(4, bc, adxtail, axtbc);
		temp16alen = scale_expansion_zeroelim(axtbclen, axtbc, 2.0 * adx,
			temp16a);
		
		axtcclen = scale_expansion_zeroelim(4, cc, adxtail, axtcc);
		temp16blen = scale_expansion_zeroelim(axtcclen, axtcc, bdy, temp16b);
		
		axtbblen = scale_expansion_zeroelim(4, bb, adxtail, axtbb);
		temp16clen = scale_expansion_zeroelim(axtbblen, axtbb, -cdy, temp16c);
		
		temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
			temp16blen, temp16b, temp32a);
		temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
			temp32alen, temp32a, temp48);
		finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
			temp48, finother);
		finswap = finnow; finnow = finother; finother = finswap;
	}
	if (adytail != 0.0) {
		aytbclen = scale_expansion_zeroelim(4, bc, adytail, aytbc);
		temp16alen = scale_expansion_zeroelim(aytbclen, aytbc, 2.0 * ady,
			temp16a);
		
		aytbblen = scale_expansion_zeroelim(4, bb, adytail, aytbb);
		temp16blen = scale_expansion_zeroelim(aytbblen, aytbb, cdx, temp16b);
		
		aytcclen = scale_expansion_zeroelim(4, cc, adytail, aytcc);
		temp16clen = scale_expansion_zeroelim(aytcclen, aytcc, -bdx, temp16c);
		
		temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
			temp16blen, temp16b, temp32a);
		temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
			temp32alen, temp32a, temp48);
		finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
			temp48, finother);
		finswap = finnow; finnow = finother; finother = finswap;
	}
	if (bdxtail != 0.0) {
		bxtcalen = scale_expansion_zeroelim(4, ca, bdxtail, bxtca);
		temp16alen = scale_expansion_zeroelim(bxtcalen, bxtca, 2.0 * bdx,
			temp16a);
		
		bxtaalen = scale_expansion_zeroelim(4, aa, bdxtail, bxtaa);
		temp16blen = scale_expansion_zeroelim(bxtaalen, bxtaa, cdy, temp16b);
		
		bxtcclen = scale_expansion_zeroelim(4, cc, bdxtail, bxtcc);
		temp16clen = scale_expansion_zeroelim(bxtcclen, bxtcc, -ady, temp16c);
		
		temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
			temp16blen, temp16b, temp32a);
		temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
			temp32alen, temp32a, temp48);
		finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
			temp48, finother);
		finswap = finnow; finnow = finother; finother = finswap;
	}
	if (bdytail != 0.0) {
		bytcalen = scale_expansion_zeroelim(4, ca, bdytail, bytca);
		temp16alen = scale_expansion_zeroelim(bytcalen, bytca, 2.0 * bdy,
			temp16a);
		
		bytcclen = scale_expansion_zeroelim(4, cc, bdytail, bytcc);
		temp16blen = scale_expansion_zeroelim(bytcclen, bytcc, adx, temp16b);
		
		bytaalen = scale_expansion_zeroelim(4, aa, bdytail, bytaa);
		temp16clen = scale_expansion_zeroelim(bytaalen, bytaa, -cdx, temp16c);
		
		temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
			temp16blen, temp16b, temp32a);
		temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
			temp32alen, temp32a, temp48);
		finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
			temp48, finother);
		finswap = finnow; finnow = finother; finother = finswap;
	}
	if (cdxtail != 0.0) {
		cxtablen = scale_expansion_zeroelim(4, ab, cdxtail, cxtab);
		temp16alen = scale_expansion_zeroelim(cxtablen, cxtab, 2.0 * cdx,
			temp16a);
		
		cxtbblen = scale_expansion_zeroelim(4, bb, cdxtail, cxtbb);
		temp16blen = scale_expansion_zeroelim(cxtbblen, cxtbb, ady, temp16b);
		
		cxtaalen = scale_expansion_zeroelim(4, aa, cdxtail, cxtaa);
		temp16clen = scale_expansion_zeroelim(cxtaalen, cxtaa, -bdy, temp16c);
		
		temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
			temp16blen, temp16b, temp32a);
		temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
			temp32alen, temp32a, temp48);
		finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
			temp48, finother);
		finswap = finnow; finnow = finother; finother = finswap;
	}
	if (cdytail != 0.0) {
		cytablen = scale_expansion_zeroelim(4, ab, cdytail, cytab);
		temp16alen = scale_expansion_zeroelim(cytablen, cytab, 2.0 * cdy,
			temp16a);
		
		cytaalen = scale_expansion_zeroelim(4, aa, cdytail, cytaa);
		temp16blen = scale_expansion_zeroelim(cytaalen, cytaa, bdx, temp16b);
		
		cytbblen = scale_expansion_zeroelim(4, bb, cdytail, cytbb);
		temp16clen = scale_expansion_zeroelim(cytbblen, cytbb, -adx, temp16c);
		
		temp32alen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
			temp16blen, temp16b, temp32a);
		temp48len = fast_expansion_sum_zeroelim(temp16clen, temp16c,
			temp32alen, temp32a, temp48);
		finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
			temp48, finother);
		finswap = finnow; finnow = finother; finother = finswap;
	}
	
	if ((adxtail != 0.0) || (adytail != 0.0)) {
		if ((bdxtail != 0.0) || (bdytail != 0.0)
			|| (cdxtail != 0.0) || (cdytail != 0.0)) {
			Two_Product(bdxtail, cdy, ti1, ti0);
			Two_Product(bdx, cdytail, tj1, tj0);
			Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
			u[3] = u3;
			negate = -bdy;
			Two_Product(cdxtail, negate, ti1, ti0);
			negate = -bdytail;
			Two_Product(cdx, negate, tj1, tj0);
			Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
			v[3] = v3;
			bctlen = fast_expansion_sum_zeroelim(4, u, 4, v, bct);
			
			Two_Product(bdxtail, cdytail, ti1, ti0);
			Two_Product(cdxtail, bdytail, tj1, tj0);
			Two_Two_Diff(ti1, ti0, tj1, tj0, bctt3, bctt[2], bctt[1], bctt[0]);
			bctt[3] = bctt3;
			bcttlen = 4;
		} else {
			bct[0] = 0.0;
			bctlen = 1;
			bctt[0] = 0.0;
			bcttlen = 1;
		}
		
		if (adxtail != 0.0) {
			temp16alen = scale_expansion_zeroelim(axtbclen, axtbc, adxtail, temp16a);
			axtbctlen = scale_expansion_zeroelim(bctlen, bct, adxtail, axtbct);
			temp32alen = scale_expansion_zeroelim(axtbctlen, axtbct, 2.0 * adx,
				temp32a);
			temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp32alen, temp32a, temp48);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
				temp48, finother);
			finswap = finnow; finnow = finother; finother = finswap;
			if (bdytail != 0.0) {
				temp8len = scale_expansion_zeroelim(4, cc, adxtail, temp8);
				temp16alen = scale_expansion_zeroelim(temp8len, temp8, bdytail,
					temp16a);
				finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
					temp16a, finother);
				finswap = finnow; finnow = finother; finother = finswap;
			}
			if (cdytail != 0.0) {
				temp8len = scale_expansion_zeroelim(4, bb, -adxtail, temp8);
				temp16alen = scale_expansion_zeroelim(temp8len, temp8, cdytail,
					temp16a);
				finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
					temp16a, finother);
				finswap = finnow; finnow = finother; finother = finswap;
			}
			
			temp32alen = scale_expansion_zeroelim(axtbctlen, axtbct, adxtail,
				temp32a);
			axtbcttlen = scale_expansion_zeroelim(bcttlen, bctt, adxtail, axtbctt);
			temp16alen = scale_expansion_zeroelim(axtbcttlen, axtbctt, 2.0 * adx,
				temp16a);
			temp16blen = scale_expansion_zeroelim(axtbcttlen, axtbctt, adxtail,
				temp16b);
			temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp16blen, temp16b, temp32b);
			temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
				temp32blen, temp32b, temp64);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
				temp64, finother);
			finswap = finnow; finnow = finother; finother = finswap;
		}
		if (adytail != 0.0) {
			temp16alen = scale_expansion_zeroelim(aytbclen, aytbc, adytail, temp16a);
			aytbctlen = scale_expansion_zeroelim(bctlen, bct, adytail, aytbct);
			temp32alen = scale_expansion_zeroelim(aytbctlen, aytbct, 2.0 * ady,
				temp32a);
			temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp32alen, temp32a, temp48);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
				temp48, finother);
			finswap = finnow; finnow = finother; finother = finswap;
			
			
			temp32alen = scale_expansion_zeroelim(aytbctlen, aytbct, adytail,
				temp32a);
			aytbcttlen = scale_expansion_zeroelim(bcttlen, bctt, adytail, aytbctt);
			temp16alen = scale_expansion_zeroelim(aytbcttlen, aytbctt, 2.0 * ady,
				temp16a);
			temp16blen = scale_expansion_zeroelim(aytbcttlen, aytbctt, adytail,
				temp16b);
			temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp16blen, temp16b, temp32b);
			temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
				temp32blen, temp32b, temp64);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
				temp64, finother);
			finswap = finnow; finnow = finother; finother = finswap;
		}
	}
	if ((bdxtail != 0.0) || (bdytail != 0.0)) {
		if ((cdxtail != 0.0) || (cdytail != 0.0)
			|| (adxtail != 0.0) || (adytail != 0.0)) {
			Two_Product(cdxtail, ady, ti1, ti0);
			Two_Product(cdx, adytail, tj1, tj0);
			Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
			u[3] = u3;
			negate = -cdy;
			Two_Product(adxtail, negate, ti1, ti0);
			negate = -cdytail;
			Two_Product(adx, negate, tj1, tj0);
			Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
			v[3] = v3;
			catlen = fast_expansion_sum_zeroelim(4, u, 4, v, cat);
			
			Two_Product(cdxtail, adytail, ti1, ti0);
			Two_Product(adxtail, cdytail, tj1, tj0);
			Two_Two_Diff(ti1, ti0, tj1, tj0, catt3, catt[2], catt[1], catt[0]);
			catt[3] = catt3;
			cattlen = 4;
		} else {
			cat[0] = 0.0;
			catlen = 1;
			catt[0] = 0.0;
			cattlen = 1;
		}
		
		if (bdxtail != 0.0) {
			temp16alen = scale_expansion_zeroelim(bxtcalen, bxtca, bdxtail, temp16a);
			bxtcatlen = scale_expansion_zeroelim(catlen, cat, bdxtail, bxtcat);
			temp32alen = scale_expansion_zeroelim(bxtcatlen, bxtcat, 2.0 * bdx,
				temp32a);
			temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp32alen, temp32a, temp48);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
				temp48, finother);
			finswap = finnow; finnow = finother; finother = finswap;
			if (cdytail != 0.0) {
				temp8len = scale_expansion_zeroelim(4, aa, bdxtail, temp8);
				temp16alen = scale_expansion_zeroelim(temp8len, temp8, cdytail,
					temp16a);
				finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
					temp16a, finother);
				finswap = finnow; finnow = finother; finother = finswap;
			}
			if (adytail != 0.0) {
				temp8len = scale_expansion_zeroelim(4, cc, -bdxtail, temp8);
				temp16alen = scale_expansion_zeroelim(temp8len, temp8, adytail,
					temp16a);
				finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
					temp16a, finother);
				finswap = finnow; finnow = finother; finother = finswap;
			}
			
			temp32alen = scale_expansion_zeroelim(bxtcatlen, bxtcat, bdxtail,
				temp32a);
			bxtcattlen = scale_expansion_zeroelim(cattlen, catt, bdxtail, bxtcatt);
			temp16alen = scale_expansion_zeroelim(bxtcattlen, bxtcatt, 2.0 * bdx,
				temp16a);
			temp16blen = scale_expansion_zeroelim(bxtcattlen, bxtcatt, bdxtail,
				temp16b);
			temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp16blen, temp16b, temp32b);
			temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
				temp32blen, temp32b, temp64);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
				temp64, finother);
			finswap = finnow; finnow = finother; finother = finswap;
		}
		if (bdytail != 0.0) {
			temp16alen = scale_expansion_zeroelim(bytcalen, bytca, bdytail, temp16a);
			bytcatlen = scale_expansion_zeroelim(catlen, cat, bdytail, bytcat);
			temp32alen = scale_expansion_zeroelim(bytcatlen, bytcat, 2.0 * bdy,
				temp32a);
			temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp32alen, temp32a, temp48);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
				temp48, finother);
			finswap = finnow; finnow = finother; finother = finswap;
			
			
			temp32alen = scale_expansion_zeroelim(bytcatlen, bytcat, bdytail,
				temp32a);
			bytcattlen = scale_expansion_zeroelim(cattlen, catt, bdytail, bytcatt);
			temp16alen = scale_expansion_zeroelim(bytcattlen, bytcatt, 2.0 * bdy,
				temp16a);
			temp16blen = scale_expansion_zeroelim(bytcattlen, bytcatt, bdytail,
				temp16b);
			temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp16blen, temp16b, temp32b);
			temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
				temp32blen, temp32b, temp64);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
				temp64, finother);
			finswap = finnow; finnow = finother; finother = finswap;
		}
	}
	if ((cdxtail != 0.0) || (cdytail != 0.0)) {
		if ((adxtail != 0.0) || (adytail != 0.0)
			|| (bdxtail != 0.0) || (bdytail != 0.0)) {
			Two_Product(adxtail, bdy, ti1, ti0);
			Two_Product(adx, bdytail, tj1, tj0);
			Two_Two_Sum(ti1, ti0, tj1, tj0, u3, u[2], u[1], u[0]);
			u[3] = u3;
			negate = -ady;
			Two_Product(bdxtail, negate, ti1, ti0);
			negate = -adytail;
			Two_Product(bdx, negate, tj1, tj0);
			Two_Two_Sum(ti1, ti0, tj1, tj0, v3, v[2], v[1], v[0]);
			v[3] = v3;
			abtlen = fast_expansion_sum_zeroelim(4, u, 4, v, abt);
			
			Two_Product(adxtail, bdytail, ti1, ti0);
			Two_Product(bdxtail, adytail, tj1, tj0);
			Two_Two_Diff(ti1, ti0, tj1, tj0, abtt3, abtt[2], abtt[1], abtt[0]);
			abtt[3] = abtt3;
			abttlen = 4;
		} else {
			abt[0] = 0.0;
			abtlen = 1;
			abtt[0] = 0.0;
			abttlen = 1;
		}
		
		if (cdxtail != 0.0) {
			temp16alen = scale_expansion_zeroelim(cxtablen, cxtab, cdxtail, temp16a);
			cxtabtlen = scale_expansion_zeroelim(abtlen, abt, cdxtail, cxtabt);
			temp32alen = scale_expansion_zeroelim(cxtabtlen, cxtabt, 2.0 * cdx,
				temp32a);
			temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp32alen, temp32a, temp48);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
				temp48, finother);
			finswap = finnow; finnow = finother; finother = finswap;
			if (adytail != 0.0) {
				temp8len = scale_expansion_zeroelim(4, bb, cdxtail, temp8);
				temp16alen = scale_expansion_zeroelim(temp8len, temp8, adytail,
					temp16a);
				finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
					temp16a, finother);
				finswap = finnow; finnow = finother; finother = finswap;
			}
			if (bdytail != 0.0) {
				temp8len = scale_expansion_zeroelim(4, aa, -cdxtail, temp8);
				temp16alen = scale_expansion_zeroelim(temp8len, temp8, bdytail,
					temp16a);
				finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp16alen,
					temp16a, finother);
				finswap = finnow; finnow = finother; finother = finswap;
			}
			
			temp32alen = scale_expansion_zeroelim(cxtabtlen, cxtabt, cdxtail,
				temp32a);
			cxtabttlen = scale_expansion_zeroelim(abttlen, abtt, cdxtail, cxtabtt);
			temp16alen = scale_expansion_zeroelim(cxtabttlen, cxtabtt, 2.0 * cdx,
				temp16a);
			temp16blen = scale_expansion_zeroelim(cxtabttlen, cxtabtt, cdxtail,
				temp16b);
			temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp16blen, temp16b, temp32b);
			temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
				temp32blen, temp32b, temp64);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
				temp64, finother);
			finswap = finnow; finnow = finother; finother = finswap;
		}
		if (cdytail != 0.0) {
			temp16alen = scale_expansion_zeroelim(cytablen, cytab, cdytail, temp16a);
			cytabtlen = scale_expansion_zeroelim(abtlen, abt, cdytail, cytabt);
			temp32alen = scale_expansion_zeroelim(cytabtlen, cytabt, 2.0 * cdy,
				temp32a);
			temp48len = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp32alen, temp32a, temp48);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp48len,
				temp48, finother);
			finswap = finnow; finnow = finother; finother = finswap;
			
			
			temp32alen = scale_expansion_zeroelim(cytabtlen, cytabt, cdytail,
				temp32a);
			cytabttlen = scale_expansion_zeroelim(abttlen, abtt, cdytail, cytabtt);
			temp16alen = scale_expansion_zeroelim(cytabttlen, cytabtt, 2.0 * cdy,
				temp16a);
			temp16blen = scale_expansion_zeroelim(cytabttlen, cytabtt, cdytail,
				temp16b);
			temp32blen = fast_expansion_sum_zeroelim(temp16alen, temp16a,
				temp16blen, temp16b, temp32b);
			temp64len = fast_expansion_sum_zeroelim(temp32alen, temp32a,
				temp32blen, temp32b, temp64);
			finlength = fast_expansion_sum_zeroelim(finlength, finnow, temp64len,
				temp64, finother);
			finswap = finnow; finnow = finother; finother = finswap;
		}
	}
	
	return finnow[finlength - 1];
}


REAL CTINClass::incircle(triPOINT * pa,triPOINT * pb,triPOINT * pc,triPOINT * pd)
{
	REAL adx, bdx, cdx, ady, bdy, cdy;
	REAL bdxcdy, cdxbdy, cdxady, adxcdy, adxbdy, bdxady;
	REAL alift, blift, clift;
	REAL det;
	REAL permanent, errbound;
	
	incirclecount++;
	
	adx = pa->x - pd->x;
	bdx = pb->x - pd->x;
	cdx = pc->x - pd->x;
	ady = pa->y - pd->y;
	bdy = pb->y - pd->y;
	cdy = pc->y - pd->y;
	
	bdxcdy = bdx * cdy;
	cdxbdy = cdx * bdy;
	alift = adx * adx + ady * ady;
	
	cdxady = cdx * ady;
	adxcdy = adx * cdy;
	blift = bdx * bdx + bdy * bdy;
	
	adxbdy = adx * bdy;
	bdxady = bdx * ady;
	clift = cdx * cdx + cdy * cdy;
	
	det = alift * (bdxcdy - cdxbdy)
		+ blift * (cdxady - adxcdy)
		+ clift * (adxbdy - bdxady);
	
	if (noexact) {
		return det;
	}
	
	permanent = (Absolute(bdxcdy) + Absolute(cdxbdy)) * alift
		+ (Absolute(cdxady) + Absolute(adxcdy)) * blift
		+ (Absolute(adxbdy) + Absolute(bdxady)) * clift;
	errbound = iccerrboundA * permanent;
	if ((det > errbound) || (-det > errbound)) {
		return det;
	}
	
	return incircleadapt(pa, pb, pc, pd, permanent);
}

/**                                                                         **/
/**                                                                         **/
/********* Determinant evaluation routines end here                  *********/

/*****************************************************************************/
/*                                                                           */
/*  triangleinit()   Initialize some variables.                              */
/*                                                                           */
/*****************************************************************************/

void CTINClass::triangleinit()
{
/*
m_points.GetMaxNumberOfItems() = m_triangles.GetMaxNumberOfItems() = m_shelles.GetMaxNumberOfItems() = m_viri.GetMaxNumberOfItems() =
m_badSegments.GetMaxNumberOfItems() = m_badTriangles.GetMaxNumberOfItems() = m_splayNodes.GetMaxNumberOfItems() = 0l;

	m_points.GetItemBytes() = m_triangles.GetItemBytes() = m_shelles.GetItemBytes() = m_viri.GetItemBytes() =
	m_badSegments.GetItemBytes() = m_badTriangles.GetItemBytes() = m_splayNodes.GetItemBytes() = 0;
	*/
	
	m_recentTri.tri = (TRIANGLE *) NULL;    /* No TRIANGLE has been visited yet. */
	samples = 1;            /* Point location should take at least one sample. */
	m_checksegments = false;      /* There are no segments in the triangulation yet. */
	incirclecount = counterclockcount = hyperbolacount = 0;
	circumcentercount = circletopcount = 0;
	SetRandom(1);
	
	exactinit();                     /* Initialize exact arithmetic constants. */
}

/********* Mesh quality testing routines begin here                  *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  checkmesh()   Test the mesh for topological consistency.                 */
/*                                                                           */
/*****************************************************************************/

#ifndef REDUCED

void CTINClass::checkmesh()
{
	triEDGE triangleloop;
	triEDGE oppotri, oppooppotri;
	triPOINT *triorg, *tridest, *triapex;
	triPOINT *oppoorg, *oppodest;
	int horrors;
	bool saveexact;
	
	/* Temporarily turn on exact arithmetic if it's off. */
	saveexact = noexact;
	noexact = 0;
	if (!quiet) 
		printf("  Checking consistency of mesh...\n");
	
	horrors = 0;
	/* Run through the list of m_triangles, checking each one. */
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		/* Check all three edges of the TRIANGLE. */
		for (triangleloop.orient = 0; triangleloop.orient < 3;	triangleloop.orient++) {
			triorg = org(triangleloop);
			tridest = dest(triangleloop );
			if (triangleloop.orient == 0) {       /* Only test for inversion once. */
				/* Test if the TRIANGLE is flat or inverted. */
				triapex = apex(triangleloop);
				if (counterclockwise(triorg, tridest, triapex) <= 0.0) {
					printf("  !! !! Inverted ");
					printtriangle(&triangleloop);
					horrors++;
				}
			}
			/* Find the neighboring TRIANGLE on this EDGE. */
			SymmTri(triangleloop, oppotri);
			if (oppotri.tri != m_dummytri) {
				/* Check that the TRIANGLE's neighbor knows it's a neighbor. */
				SymmTri(oppotri, oppooppotri);
				if ((triangleloop.tri != oppooppotri.tri) || (triangleloop.orient != oppooppotri.orient)) {
					printf("  !! !! Asymmetric TRIANGLE-TRIANGLE bond:\n");
					if (triangleloop.tri == oppooppotri.tri) 
						printf("   (Right TRIANGLE, wrong orientation)\n");
					printf("    First ");
					printtriangle(&triangleloop);
					printf("    Second (nonreciprocating) ");
					printtriangle(&oppotri);
					horrors++;
				}
				/* Check that both m_triangles agree on the identities */
				/*   of their shared vertices.                       */
				oppoorg = org(oppotri );
				oppodest = dest(oppotri);
				if ((triorg != oppodest) || (tridest != oppoorg)) {
					printf("  !! !! Mismatched EDGE coordinates between two m_triangles:\n");
					printf("    First mismatched ");
					printtriangle(&triangleloop);
					printf("    Second mismatched ");
					printtriangle(&oppotri);
					horrors++;
				}
			}
		}
		triangleloop.tri = TriangleTraverse();
	}
	if (horrors == 0) {
		if (!quiet) 
			printf("  In my studied opinion, the mesh appears to be consistent.\n");
	}
	else if (horrors == 1) 
		printf("  !! !! !! !! Precisely one festering wound discovered.\n");
	else 	printf("  !! !! !! !! %d abominations witnessed.\n", horrors);
	
	/* Restore the status of exact arithmetic. */
	noexact = saveexact;
}

#endif /* not REDUCED */

/*****************************************************************************/
/*                                                                           */
/*  checkdelaunay()   Ensure that the mesh is (constrained) Delaunay.        */
/*                                                                           */
/*****************************************************************************/

#ifndef REDUCED

void CTINClass::checkdelaunay()
{
	triEDGE triangleloop;
	triEDGE oppotri;
	EDGE opposhelle;
	triPOINT *triorg, *tridest, *triapex;
	triPOINT *oppoapex;
	int shouldbedelaunay;
	int horrors;
	int saveexact;
	
	/* Temporarily turn on exact arithmetic if it's off. */
	saveexact = noexact;
	noexact = 0;
	if (!quiet) {
		printf("  Checking Delaunay property of mesh...\n");
	}
	horrors = 0;
	/* Run through the list of m_triangles, checking each one. */
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		/* Check all three edges of the TRIANGLE. */
		for (triangleloop.orient = 0; triangleloop.orient < 3;	triangleloop.orient++) {
			triorg = org(triangleloop);
			tridest = dest(triangleloop);
			triapex = apex(triangleloop);
			SymmTri(triangleloop, oppotri);
			oppoapex = apex(oppotri);
			/* Only test that the EDGE is locally Delaunay if there is an   */
			/*   adjoining TRIANGLE whose pointer is larger (to ensure that */
			/*   each pair isn't tested twice).                             */
			shouldbedelaunay = (oppotri.tri != m_dummytri)
				&& (triapex != (triPOINT *) NULL) && (oppoapex != (triPOINT *) NULL)
				&& (triangleloop.tri < oppotri.tri);
			if (m_checksegments && shouldbedelaunay) {
				/* If a shell EDGE separates the m_triangles, then the EDGE is */
				/*   constrained, so no local Delaunay test should be done.  */
				tspivot(triangleloop, opposhelle);
				if( opposhelle.sh != m_dummysh ){
					shouldbedelaunay = 0;
				}
			}
			if (shouldbedelaunay) {
				if (incircle(triorg, tridest, triapex, oppoapex) > 0.0) {
					printf("  !! !! Non-Delaunay pair of m_triangles:\n");
					printf("    First non-Delaunay ");
					printtriangle(&triangleloop);
					printf("    Second non-Delaunay ");
					printtriangle(&oppotri);
					horrors++;
				}
			}
		}
		triangleloop.tri = TriangleTraverse();
	}
	if (horrors == 0) {
		if (!quiet) {
			printf(
				"  By virtue of my perceptive intelligence, I declare the mesh Delaunay.\n");
		}
	} else if (horrors == 1) {
		printf(
			"  !! !! !! !! Precisely one terrifying transgression identified.\n");
	} else {
		printf("  !! !! !! !! %d obscenities viewed with horror.\n", horrors);
	}
	/* Restore the status of exact arithmetic. */
	noexact = saveexact;
}

#endif /* not REDUCED */

/*****************************************************************************/
/*                                                                           */
/*  enqueuebadtri()   Add a bad TRIANGLE to the end of a queue.              */
/*                                                                           */
/*  The queue is actually a set of 64 queues.  I use multiple queues to give */
/*  priority to smaller angles.  I originally implemented a heap, but the    */
/*  queues are (to my surprise) much faster.                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

void CTINClass::enqueuebadtri(triEDGE *instri,REAL angle,triPOINT *insapex,triPOINT *inshOrg,triPOINT *inshDest)
{
	badFACE *newface;
	int queuenumber;
	
	if (m_verbose > 2) {
		printf("  Queueing bad TRIANGLE:\n");
		printf("    (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n", inshOrg->x,
			inshOrg->y, inshDest->x, inshDest->y, insapex->x, insapex->y);
	}
	/* Allocate space for the bad TRIANGLE. */
	newface = (badFACE *)m_badTriangles.PoolAlloc();
	TriEdgeCopy(*instri, newface->badfacetri);
	newface->key = angle;
	newface->faceapex = insapex;
	newface->faceorg = inshOrg;
	newface->facedest = inshDest;
	newface->nextface = (badFACE *) NULL;
	/* Determine the appropriate queue to put the bad TRIANGLE into. */
	if (angle > 0.6) {
		queuenumber = (int) (160.0 * (angle - 0.6));
		if (queuenumber > 63) {
			queuenumber = 63;
		}
	} else {
		/* It's not a bad angle; put the TRIANGLE in the lowest-priority queue. */
		queuenumber = 0;
	}
	/* Add the TRIANGLE to the end of a queue. */
	*queuetail[queuenumber] = newface;
	/* Maintain a pointer to the NULL pointer at the end of the queue. */
	queuetail[queuenumber] = &newface->nextface;
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  dequeuebadtri()   Remove a TRIANGLE from the front of the queue.         */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

badFACE *CTINClass::dequeuebadtri()
{
	badFACE *result;
	int queuenumber;
	
	/* Look for a nonempty queue. */
	for (queuenumber = 63; queuenumber >= 0; queuenumber--) {
		result = queuefront[queuenumber];
		if (result != (badFACE *) NULL) {
			/* Remove the TRIANGLE from the queue. */
			queuefront[queuenumber] = result->nextface;
			/* Maintain a pointer to the NULL pointer at the end of the queue. */
			if (queuefront[queuenumber] == (badFACE *) NULL) {
				queuetail[queuenumber] = &queuefront[queuenumber];
			}
			return result;
		}
	}
	return (badFACE *) NULL;
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  checkedge4encroach()   Check a segment to see if it is encroached; add   */
/*                         it to the list if it is.                          */
/*                                                                           */
/*  An encroached segment is an unflippable EDGE that has a triPOINT in its     */
/*  diametral circle (that is, it faces an angle greater than 90 degrees).   */
/*  This definition is due to Ruppert.                                       */
/*                                                                           */
/*  Returns a nonzero value if the EDGE is encroached.                       */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

int CTINClass::checkedge4encroach(EDGE *testedge)
{
	triEDGE neighbortri;
	EDGE testsym;
	EDGE *badedge;
	int addtolist;
	int sides;
	triPOINT *eorg, *edest, *eapex;
	
	addtolist = 0;
	sides = 0;
	
	eorg = shOrg(*testedge );
	edest = shDest(*testedge );
	/* Check one neighbor of the shell EDGE. */
	stpivot(*testedge, neighbortri);
	/* Does the neighbor exist, or is this a boundary EDGE? */
	if (neighbortri.tri != m_dummytri) {
		sides++;
		/* Find a vertex opposite this EDGE. */
		eapex = apex(neighbortri );
		/* Check whether the vertex is inside the diametral circle of the  */
		/*   shell EDGE.  Pythagoras' Theorem is used to check whether the */
		/*   angle at the vertex is greater than 90 degrees.               */
		if (eapex->x * (eorg->x + edest->x) + eapex->y * (eorg->y + edest->y) >
			eapex->x * eapex->x + eorg->x * edest->x + eapex->y * eapex->y + eorg->y * edest->y) 
			addtolist = 1;
	}
	/* Check the other neighbor of the shell EDGE. */
	shSymmTri(*testedge, testsym);
	stpivot(testsym, neighbortri);
	/* Does the neighbor exist, or is this a boundary EDGE? */
	if (neighbortri.tri != m_dummytri) {
		sides++;
		/* Find the other vertex opposite this EDGE. */
		eapex = apex(neighbortri);
		/* Check whether the vertex is inside the diametral circle of the  */
		/*   shell EDGE.  Pythagoras' Theorem is used to check whether the */
		/*   angle at the vertex is greater than 90 degrees.               */
		if (eapex->x * (eorg->x + edest->x) + eapex->y * (eorg->y + edest->y) >
			eapex->x * eapex->x + eorg->x * edest->x + eapex->y * eapex->y + eorg->y * edest->y) 
			addtolist += 2;
	}
	
	if (addtolist && (!nobisect || ((nobisect == 1) && (sides == 2)))) {
		if (m_verbose > 2) 
			printf("  Queueing encroached segment (%.12g, %.12g) (%.12g, %.12g).\n",eorg->x, eorg->y, edest[0], edest[1]);
		/* Add the shell EDGE to the list of encroached segments. */
		/*   Be sure to get the orientation right.                */
		badedge = (EDGE *) m_badSegments.PoolAlloc();
		if (addtolist == 1) {
			shellecopy(*testedge, *badedge);
		}
		else	{
			shellecopy(testsym, *badedge);
		}
	}
	return addtolist;
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  testtriangle()   Test a face for quality measures.                       */
/*                                                                           */
/*  Tests a TRIANGLE to see if it satisfies the minimum angle condition and  */
/*  the maximum area condition.  m_triangles that aren't up to spec are added  */
/*  to the bad TRIANGLE queue.                                               */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

void CTINClass::testtriangle(triEDGE*testtri)
{
	triEDGE sametesttri;
	EDGE edge1, edge2;
	triPOINT *torg, *tdest, *tapex;
	triPOINT *anglevertex;
	REAL dxod, dyod, dxda, dyda, dxao, dyao;
	REAL dxod2, dyod2, dxda2, dyda2, dxao2, dyao2;
	REAL apexlen, orglen, destlen;
	REAL angle;
	REAL area;
	
	torg = org(*testtri);
	tdest = dest(*testtri);
	tapex = apex(*testtri);
	dxod = torg->x - tdest->x;
	dyod = torg->y - tdest->y;
	dxda = tdest->x - tapex->x;
	dyda = tdest->y - tapex->y;
	dxao = tapex->x - torg->x;
	dyao = tapex->y - torg->y;
	dxod2 = dxod * dxod;
	dyod2 = dyod * dyod;
	dxda2 = dxda * dxda;
	dyda2 = dyda * dyda;
	dxao2 = dxao * dxao;
	dyao2 = dyao * dyao;
	/* Find the lengths of the TRIANGLE's three edges. */
	apexlen = dxod2 + dyod2;
	orglen = dxda2 + dyda2;
	destlen = dxao2 + dyao2;
	if ((apexlen < orglen) && (apexlen < destlen)) {
		/* The EDGE opposite the apex is shortest. */
		/* Find the square of the cosine of the angle at the apex. */
		angle = dxda * dxao + dyda * dyao;
		angle = angle * angle / (orglen * destlen);
		anglevertex = tapex;
		NextEdge(*testtri, sametesttri);
		tspivot(sametesttri, edge1);
		NextEdgeSelf(sametesttri);
		tspivot(sametesttri, edge2);
	}
	else if (orglen < destlen) {
		/* The EDGE opposite the origin is shortest. */
		/* Find the square of the cosine of the angle at the origin. */
		angle = dxod * dxao + dyod * dyao;
		angle = angle * angle / (apexlen * destlen);
		anglevertex = torg;
		tspivot(*testtri, edge1);
		PrevEdge(*testtri, sametesttri);
		tspivot(sametesttri, edge2);
	} 
	else {
		/* The EDGE opposite the destination is shortest. */
		/* Find the square of the cosine of the angle at the destination. */
		angle = dxod * dxda + dyod * dyda;
		angle = angle * angle / (apexlen * orglen);
		anglevertex = tdest;
		tspivot(*testtri, edge1);
		NextEdge(*testtri, sametesttri);
		tspivot(sametesttri, edge2);
	}
	/* Check if both edges that form the angle are segments. */
	if ((edge1.sh != m_dummysh) && (edge2.sh != m_dummysh)) {
		/* The angle is a segment intersection. */
		if ((angle > 0.9924) && !quiet) {                  /* Roughly 5 degrees. */
			if (angle > 1.0) {
				/* Beware of a floating exception in acos(). */
				angle = 1.0;
			}
			
			/* Find the actual angle in degrees, for printing. */
			angle = acos(sqrt(angle)) * (180.0 / PI);
			printf("Warning:  Small angle (%.4g degrees) between segments at triPOINT\n",angle);
			printf("  (%.12g, %.12g)\n", anglevertex[0], anglevertex[1]);
		}
		/* Don't add this bad TRIANGLE to the list; there's nothing that */
		/*   can be done about a small angle between two segments.       */
		angle = 0.0;
	}
	
	/* Check whether the angle is smaller than permitted. */
	if (angle > m_goodangle) {
		/* Add this TRIANGLE to the list of bad m_triangles. */
		enqueuebadtri(testtri, angle, tapex, torg, tdest);
		return;
	}
	
	if (vararea || fixedarea) {
		/* Check whether the area is larger than permitted. */
		area = 0.5 * (dxod * dyda - dyod * dxda);
		if (fixedarea && (area > m_maxarea)) {
			/* Add this TRIANGLE to the list of bad m_triangles. */
			enqueuebadtri(testtri, angle, tapex, torg, tdest);
		} else if (vararea) {
			/* Nonpositive area constraints are treated as unconstrained. */
			if ((area > areabound(*testtri)) && (areabound(*testtri) > 0.0)) {
				/* Add this TRIANGLE to the list of bad m_triangles. */
				enqueuebadtri(testtri, angle, tapex, torg, tdest);
			}
		}
	}
}

#endif /* not CDT_ONLY */

/**                                                                         **/
/**                                                                         **/
/********* Mesh quality testing routines end here                    *********/

/********* Point location routines begin here                        *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  MakePointMap()   Construct a mapping from m_points to m_triangles to improve  */
/*                  the speed of triPOINT location for segment insertion.       */
/*                                                                           */
/*  Traverses all the m_triangles, and provides each corner of each TRIANGLE   */
/*  with a pointer to that TRIANGLE.  Of course, pointers will be            */
/*  overwritten by other pointers because (almost) each triPOINT is a corner    */
/*  of several triangles, but in the end every triPOINT will triPOINT to some      */
/*  TRIANGLE that contains it.                                               */
/*                                                                           */
/*****************************************************************************/

void CTINClass::MakePointMap()
{
	triEDGE triangleloop;
	triPOINT *triorg;

	if (m_verbose) {
		printf("    Constructing mapping from m_points to m_triangles.\n");
	}
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		/* Check all three points of the triangle. */
		for (triangleloop.orient = 0; triangleloop.orient < 3;	triangleloop.orient++) {
			triorg = org(triangleloop);
			SetPoint2Tri(triorg, encode(triangleloop));
		}
		triangleloop.tri = TriangleTraverse();
	}
}

/*****************************************************************************/
/*                                                                           */
/*  PreciseLocate()   Find a TRIANGLE or EDGE containing a given triPOINT.      */
/*                                                                           */
/*  Begins its search from `searchtri'.  It is important that `searchtri'    */
/*  be a handle with the property that `searchpoint' is strictly to the left */
/*  of the EDGE denoted by `searchtri', or is collinear with that EDGE and   */
/*  does not intersect that EDGE.  (In particular, `searchpoint' should not  */
/*  be the origin or destination of that EDGE.)                              */
/*                                                                           */
/*  These conditions are imposed because PreciseLocate() is normally used in */
/*  one of two situations:                                                   */
/*                                                                           */
/*  (1)  To try to find the location to insert a new triPOINT.  Normally, we    */
/*       know an EDGE that the triPOINT is strictly to the left of.  In the     */
/*       incremental Delaunay algorithm, that EDGE is a bounding box EDGE.   */
/*       In Ruppert's Delaunay refinement algorithm for quality meshing,     */
/*       that EDGE is the shortest EDGE of the TRIANGLE whose circumcenter   */
/*       is being inserted.                                                  */
/*                                                                           */
/*  (2)  To try to find an existing triPOINT.  In this case, any EDGE on the    */
/*       m_convex hull is a good starting EDGE.  The possibility that the      */
/*       vertex one seeks is an endpoint of the starting EDGE must be        */
/*       screened out before PreciseLocate() is called.                      */
/*                                                                           */
/*  On completion, `searchtri' is a TRIANGLE that contains `searchpoint'.    */
/*                                                                           */
/*  This implementation differs from that given by Guibas and Stolfi.  It    */
/*  walks from TRIANGLE to TRIANGLE, crossing an EDGE only if `searchpoint'  */
/*  is on the other side of the line containing that EDGE.  After entering   */
/*  a TRIANGLE, there are two edges by which one can leave that TRIANGLE.    */
/*  If both edges are valid (`searchpoint' is on the other side of both      */
/*  edges), one of the two is chosen by drawing a line perpendicular to      */
/*  the entry EDGE (whose endpoints are `forg' and `fdest') passing through  */
/*  `fapex'.  Depending on which side of this perpendicular `searchpoint'    */
/*  falls on, an exit EDGE is chosen.                                        */
/*                                                                           */
/*  This implementation is empirically faster than the Guibas and Stolfi     */
/*  triPOINT location routine (which I originally used), which tends to spiral  */
/*  in toward its target.                                                    */
/*                                                                           */
/*  Returns ONVERTEX if the triPOINT lies on an existing vertex.  `searchtri'   */
/*  is a handle whose origin is the existing vertex.                         */
/*                                                                           */
/*  Returns ONEDGE if the triPOINT lies on a mesh EDGE.  `searchtri' is a       */
/*  handle whose primary EDGE is the EDGE on which the triPOINT lies.           */
/*                                                                           */
/*  Returns INTRIANGLE if the triPOINT lies strictly within a TRIANGLE.         */
/*  `searchtri' is a handle on the TRIANGLE that contains the triPOINT.         */
/*                                                                           */
/*  Returns OUTSIDE if the triPOINT lies outside the mesh.  `searchtri' is a    */
/*  handle whose primary EDGE the triPOINT is to the right of.  This might      */
/*  occur when the circumcenter of a TRIANGLE falls just slightly outside    */
/*  the mesh due to floating-triPOINT roundoff error.  It also occurs when      */
/*  seeking a hole or region triPOINT that a foolish user has placed outside    */
/*  the mesh.                                                                */
/*                                                                           */
/*  WARNING:  This routine is designed for m_convex triangulations, and will   */
/*  not generally work after the holes and concavities have been carved.     */
/*  However, it can still be used to find the circumcenter of a TRIANGLE, as */
/*  long as the search is begun from the TRIANGLE in question.               */
/*                                                                           */
/*****************************************************************************/

enum locateresult CTINClass::PreciseLocate(triPOINT *searchpoint,triEDGE*searchtri)
{
	triEDGE backtracktri;
	triPOINT *forg, *fdest, *fapex;
	REAL orgorient, destorient;
	int moveleft;
	
	if (m_verbose > 2) 
		printf("  Searching for triPOINT (%.12g, %.12g).\n",searchpoint->x, searchpoint->y);
	
	
	/* Where are we? */
	forg = org(*searchtri );
	fdest = dest(*searchtri );
	fapex = apex(*searchtri );
	while (1) {
		if (m_verbose > 2) {
			printf("    At (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
				forg->x, forg->y, fdest->x, fdest->y, fapex->x, fapex->y);
		}
		
		if( forg == fdest || fdest == fapex || forg == fapex ) {
			printf("Internal error\n");
			return OUTSIDE;
		}

		if( fapex == NULL )
			return OUTSIDE;

		/* Check whether the apex is the triPOINT we seek. */
		if ((fapex->x == searchpoint->x) && (fapex->y == searchpoint->y)) {
			PrevEdgeSelf(*searchtri);
			return ONVERTEX;
		}
		/* Does the triPOINT lie on the other side of the line defined by the */
		/*   TRIANGLE EDGE opposite the TRIANGLE's destination?            */
		destorient = counterclockwise(forg, fapex, searchpoint);
		
		/* Does the triPOINT lie on the other side of the line defined by the */
		/*   TRIANGLE EDGE opposite the TRIANGLE's origin?                 */
		orgorient = counterclockwise(fapex, fdest, searchpoint);
		
		if (destorient > 0.0) {
			if (orgorient > 0.0) {
				/* Move left if the inner product of (fapex - searchpoint) and  */
				/*   (fdest - forg) is positive.  This is equivalent to drawing */
				/*   a line perpendicular to the line (forg, fdest) passing     */
				/*   through `fapex', and determining which side of this line   */
				/*   `searchpoint' falls on.                                    */
				moveleft = (fapex->x - searchpoint->x) * (fdest->x - forg->x) +
					(fapex->y - searchpoint->y) * (fdest->y - forg->y) > 0.0;
			} 
			else 	moveleft = 1;
		} else {
			if (orgorient > 0.0) 
				moveleft = 0;
			else {
				/* The triPOINT we seek must be on the boundary of or inside this TRIANGLE. */
				if (destorient == 0.0) {
					PrevEdgeSelf(*searchtri);
					return ONEDGE;
				}
				if (orgorient == 0.0) {
					NextEdgeSelf(*searchtri);
					return ONEDGE;
				}
				return INTRIANGLE;
			}
		}
		
		/* Move to another TRIANGLE.  Leave a trace `backtracktri' in case */
		/*   floating-triPOINT roundoff or some such bogey causes us to walk  */
		/*   off a boundary of the triangulation.  We can just bounce off  */
		/*   the boundary as if it were an elastic band.                   */
		if( moveleft ) {
			PrevEdge(*searchtri, backtracktri);
			fdest = fapex;
		} 
		else {
			NextEdge(*searchtri, backtracktri);
			forg = fapex;
		}
		SymmTri(backtracktri, *searchtri);
		
		/* Check for walking off the EDGE. */
		if (searchtri->tri == m_dummytri) {
			return OUTSIDE;	// directly return by jws

			/* Turn around. */
			TriEdgeCopy(backtracktri, *searchtri);

		   // by jws
			forg = org(*searchtri );
			fdest = dest(*searchtri );
			fapex = apex(*searchtri );

			/* Check if the triPOINT really is beyond the triangulation boundary. */
			destorient = counterclockwise(forg, fapex, searchpoint);
			orgorient = counterclockwise(fapex, fdest, searchpoint);
			if ((orgorient < 0.0) && (destorient < 0.0)) 
				return OUTSIDE;
		} 
		else {
			fapex = apex(*searchtri);
		}
	}
}

/*****************************************************************************/
/*                                                                           */
/*  locate()   Find a TRIANGLE or EDGE containing a given triPOINT.             */
/*                                                                           */
/*  Searching begins from one of:  the input `searchtri', a recently         */
/*  encountered TRIANGLE `m_recentTri', or from a TRIANGLE chosen from a       */
/*  random sample.  The choice is made by determining which TRIANGLE's       */
/*  origin is closest to the triPOINT we are searcing for.  Normally,           */
/*  `searchtri' should be a handle on the m_convex hull of the triangulation.  */
/*                                                                           */
/*  Details on the random sampling method can be found in the Mucke, Saias,  */
/*  and Zhu paper cited in the header of this code.                          */
/*                                                                           */
/*  On completion, `searchtri' is a TRIANGLE that contains `searchpoint'.    */
/*                                                                           */
/*  Returns ONVERTEX if the triPOINT lies on an existing vertex.  `searchtri'   */
/*  is a handle whose origin is the existing vertex.                         */
/*                                                                           */
/*  Returns ONEDGE if the triPOINT lies on a mesh EDGE.  `searchtri' is a       */
/*  handle whose primary EDGE is the EDGE on which the triPOINT lies.           */
/*                                                                           */
/*  Returns INTRIANGLE if the triPOINT lies strictly within a TRIANGLE.         */
/*  `searchtri' is a handle on the TRIANGLE that contains the triPOINT.         */
/*                                                                           */
/*  Returns OUTSIDE if the triPOINT lies outside the mesh.  `searchtri' is a    */
/*  handle whose primary EDGE the triPOINT is to the right of.  This might      */
/*  occur when the circumcenter of a TRIANGLE falls just slightly outside    */
/*  the mesh due to floating-triPOINT roundoff error.  It also occurs when      */
/*  seeking a hole or region triPOINT that a foolish user has placed outside    */
/*  the mesh.                                                                */
/*                                                                           */
/*  WARNING:  This routine is designed for m_convex triangulations, and will   */
/*  not generally work after the holes and concavities have been carved.     */
/*                                                                           */
/*****************************************************************************/

enum locateresult CTINClass::locate(triPOINT *searchpoint,triEDGE*searchtri)
{
	triEDGE sampletri;
	triPOINT *torg, *tdest;
	REAL searchdist, dist;
	REAL ahead;
	long sampleblocks, samplesperblock, samplenum;
	long triblocks;
	long i, j;
	
	if (m_verbose > 2) {
		printf("  Randomly sampling for a TRIANGLE near triPOINT (%.12g, %.12g).\n",
			searchpoint->x, searchpoint->y);
	}
	/* Record the distance from the suggested starting TRIANGLE to the */
	/*   triPOINT we seek.                                                */
	torg = org(*searchtri);
	searchdist = (searchpoint->x - torg->x) * (searchpoint->x - torg->x)
		+ (searchpoint->y - torg->y) * (searchpoint->y - torg->y);
	if (m_verbose > 2) {
		printf("    Boundary TRIANGLE has origin (%.12g, %.12g).\n",
			torg->x, torg->y);
	}
	
	/* If a recently encountered TRIANGLE has been recorded and has not been */
	/*   deallocated, test it as a good starting triPOINT.                      */
	if( m_recentTri.tri != (TRIANGLE *) NULL) {
		if( m_recentTri.tri->vertex[0] != (triPOINT *) NULL) {
			torg = org(m_recentTri);
			if ((torg->x == searchpoint->x) && (torg->y == searchpoint->y)) {
				TriEdgeCopy(m_recentTri, *searchtri);
				return ONVERTEX;
			}
			dist = (searchpoint->x - torg->x) * (searchpoint->x - torg->x)
				+ (searchpoint->y - torg->y) * (searchpoint->y - torg->y);
			if (dist < searchdist) {
				TriEdgeCopy(m_recentTri, *searchtri);
				searchdist = dist;
				if (m_verbose > 2) {
					printf("    Choosing recent TRIANGLE with origin (%.12g, %.12g).\n",
						torg->x, torg->y);
				}
			}
		}
	}
	
	/* The number of random samples taken is proportional to the cube root of */
	/*   the number of m_triangles in the mesh.  The next bit of code assumes   */
	/*   that the number of m_triangles increases monotonically.                */
	while (SAMPLEFACTOR * samples * samples * samples < m_triangles.GetNumberOfItems()) {
		samples++;
	}
	triblocks = (m_triangles.GetMaxNumberOfItems() + TRIPERBLOCK - 1) / TRIPERBLOCK;
	samplesperblock = 1 + (samples / triblocks);
	sampleblocks = samples / samplesperblock;

	m_triangles.GotoFirstBlock();

	sampletri.orient = 0;
	for (i = 0; i < sampleblocks; i++) {
		for (j = 0; j < samplesperblock; j++) {
			if (i == triblocks - 1) 
				samplenum = randomnation((int)
					(m_triangles.GetMaxNumberOfItems() - (i * TRIPERBLOCK)));
			else 
				samplenum = randomnation( TRIPERBLOCK );

			sampletri.tri = m_triangles.GetPathBlockItem( samplenum );

			if( sampletri.tri->vertex[0] != (triPOINT *) NULL) {
				torg = org(sampletri);
				dist = (searchpoint->x - torg->x) * (searchpoint->x - torg->x)
					+ (searchpoint->y - torg->y) * (searchpoint->y - torg->y);
				if (dist < searchdist) {
					TriEdgeCopy(sampletri, *searchtri);
					searchdist = dist;
					if (m_verbose > 2) {
						printf("    Choosing TRIANGLE with origin (%.12g, %.12g).\n",
							torg->x, torg->y);
					}
				}
			}
		}
		m_triangles.GotoNextBlock();
	}

	/* Where are we? */
	torg = org(*searchtri);
	tdest = dest(*searchtri);
	/* Check the starting TRIANGLE's vertices. */
	if ((torg->x == searchpoint->x) && (torg->y == searchpoint->y)) {
		return ONVERTEX;
	}
	if ((tdest->x == searchpoint->x) && (tdest->y == searchpoint->y)) {
		NextEdgeSelf(*searchtri);
		return ONVERTEX;
	}
	/* Orient `searchtri' to fit the preconditions of calling PreciseLocate(). */
	ahead = counterclockwise(torg, tdest, searchpoint);
	if (ahead < 0.0) {
		/* Turn around so that `searchpoint' is to the left of the */
		/*   EDGE specified by `searchtri'.                        */
		SymmTriSelf(*searchtri);
	} else if (ahead == 0.0) {
		/* Check if `searchpoint' is between `torg' and `tdest'. */
		if (((torg->x < searchpoint->x) == (searchpoint->x < tdest->x))
			&& ((torg->y < searchpoint->y) == (searchpoint->y < tdest->y))) {
			return ONEDGE;
		}
	}
	return PreciseLocate(searchpoint, searchtri);
}

/**                                                                         **/
/**                                                                         **/
/********* Point location routines end here                          *********/

/********* Mesh transformation routines begin here                   *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  insertshelle()   Create a new shell EDGE and insert it between two       */
/*                   m_triangles.                                              */
/*                                                                           */
/*  The new shell EDGE is inserted at the EDGE described by the handle       */
/*  `tri'.  Its vertices are properly initialized.  The marker `shellemark'  */
/*  is applied to the shell EDGE and, if appropriate, its vertices.          */
/*                                                                           */
/*****************************************************************************/

void CTINClass::insertshelle(triEDGE*tri,int shellemark)
/* triEDGE*tri;  Edge at which to insert the new shell EDGE. */
/* int shellemark;   Marker for the new shell EDGE. */
{
	triEDGE oppotri;
	EDGE newshelle;
	triPOINT *triorg, *tridest;
	
	/* Mark points if possible. */
	triorg = org(*tri);
	tridest = dest(*tri);
	if (PointMark(triorg) == 0) {
		SetPointMark(triorg, shellemark);
	}
	if (PointMark(tridest) == 0) {
		SetPointMark(tridest, shellemark);
	}
	/* Check if there's already a shell EDGE here. */
	tspivot(*tri, newshelle);
	if (newshelle.sh == m_dummysh) {
		/* Make new shell EDGE and initialize its vertices. */
		makeshelle(&newshelle);
		setshOrg(newshelle, tridest);
		setshDest(newshelle, triorg);
		/* Bond new shell EDGE to the two m_triangles it is sandwiched between. */
		/*   Note that the facing TRIANGLE `oppotri' might be equal to        */
		/*   `m_dummytri' (outer space), but the new shell EDGE is bonded to it */
		/*   all the same.                                                    */
		tshBond(*tri, newshelle);
		SymmTri(*tri, oppotri);
		shSymmTriSelf(newshelle);
		tshBond(oppotri, newshelle);
		SetShellMark(newshelle, shellemark);
		if (m_verbose > 2) {
			printf("  Inserting new ");
			printshelle(&newshelle);
		}
	} else {
		if ( shMark(newshelle) == 0) {
			SetShellMark(newshelle, shellemark);
		}
	}
}

/*****************************************************************************/
/*                                                                           */
/*  Terminology                                                              */
/*                                                                           */
/*  A "local transformation" replaces a small set of m_triangles with another  */
/*  set of m_triangles.  This may or may not involve inserting or deleting a   */
/*  triPOINT.                                                                   */
/*                                                                           */
/*  The term "casing" is used to describe the set of m_triangles that are      */
/*  attached to the m_triangles being transformed, but are not transformed     */
/*  themselves.  Think of the casing as a fixed hollow structure inside      */
/*  which all the action happens.  A "casing" is only defined relative to    */
/*  a single transformation; each occurrence of a transformation will        */
/*  involve a different casing.                                              */
/*                                                                           */
/*  A "shell" is similar to a "casing".  The term "shell" describes the set  */
/*  of shell edges (if any) that are attached to the m_triangles being         */
/*  transformed.  However, I sometimes use "shell" to refer to a single      */
/*  shell EDGE, so don't get confused.                                       */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  flip()   Transform two m_triangles to two different m_triangles by flipping  */
/*           an EDGE within a quadrilateral.                                 */
/*                                                                           */
/*  Imagine the original m_triangles, abc and bad, oriented so that the        */
/*  shared EDGE ab lies in a horizontal plane, with the triPOINT b on the left  */
/*  and the triPOINT a on the right.  The triPOINT c lies below the EDGE, and the  */
/*  triPOINT d lies above the EDGE.  The `flipedge' handle holds the EDGE ab    */
/*  of TRIANGLE abc, and is directed left, from vertex a to vertex b.        */
/*                                                                           */
/*  The m_triangles abc and bad are deleted and replaced by the m_triangles cdb  */
/*  and dca.  The m_triangles that represent abc and bad are NOT deallocated;  */
/*  they are reused for dca and cdb, respectively.  Hence, any handles that  */
/*  may have held the original m_triangles are still valid, although not       */
/*  directed as they were before.                                            */
/*                                                                           */
/*  Upon completion of this routine, the `flipedge' handle holds the EDGE    */
/*  dc of TRIANGLE dca, and is directed down, from vertex d to vertex c.     */
/*  (Hence, the two m_triangles have rotated counterclockwise.)                */
/*                                                                           */
/*  WARNING:  This transformation is geometrically valid only if the         */
/*  quadrilateral adbc is m_convex.  Furthermore, this transformation is       */
/*  valid only if there is not a shell EDGE between the m_triangles abc and    */
/*  bad.  This routine does not check either of these preconditions, and     */
/*  it is the responsibility of the calling routine to ensure that they are  */
/*  met.  If they are not, the streets shall be filled with wailing and      */
/*  gnashing of teeth.                                                       */
/*                                                                           */
/*****************************************************************************/

void CTINClass::flip(triEDGE*flipedge)
/* Handle for the TRIANGLE abc. */
{
	triEDGE botleft, botright;
	triEDGE topleft, topright;
	triEDGE top;
	triEDGE botlcasing, botrcasing;
	triEDGE toplcasing, toprcasing;
	EDGE botlshelle, botrshelle;
	EDGE toplshelle, toprshelle;
	triPOINT *leftpoint, *rightpoint, *botpoint;
	triPOINT *farpoint;
	
	/* Identify the vertices of the quadrilateral. */
	rightpoint = org(*flipedge);
	leftpoint = dest(*flipedge);
	botpoint = apex(*flipedge);
	SymmTri(*flipedge, top);
	
#ifdef SELF_CHECK
	if (top.tri == m_dummytri) {
		printf("Internal error in flip():  Attempt to flip on boundary.\n");
		NextEdgeSelf(*flipedge);
		return;
	}
	if (m_checksegments) {
		tspivot(*flipedge, toplshelle);
		if (toplshelle.sh != m_dummysh) {
			printf("Internal error in flip():  Attempt to flip a segment.\n");
			NextEdgeSelf(*flipedge);
			return;
		}
	}
#endif /* SELF_CHECK */
	
	farpoint = apex(top);
	
	/* Identify the casing of the quadrilateral. */
	PrevEdge(top, topleft);
	SymmTri(topleft, toplcasing);
	NextEdge(top, topright);
	SymmTri(topright, toprcasing);
	NextEdge(*flipedge, botleft);
	SymmTri(botleft, botlcasing);
	PrevEdge(*flipedge, botright);
	SymmTri(botright, botrcasing);
	/* Rotate the quadrilateral one-quarter turn counterclockwise. */
	bond(topleft, botlcasing);
	bond(botleft, botrcasing);
	bond(botright, toprcasing);
	bond(topright, toplcasing);
	
	if (m_checksegments) {
		/* Check for shell edges and rebond them to the quadrilateral. */
		tspivot(topleft, toplshelle);
		tspivot(botleft, botlshelle);
		tspivot(botright, botrshelle);
		tspivot(topright, toprshelle);
		if (toplshelle.sh == m_dummysh) {
			tshDissolve(topright);
		} else {
			tshBond(topright, toplshelle);
		}
		if (botlshelle.sh == m_dummysh) {
			tshDissolve(topleft);
		} else {
			tshBond(topleft, botlshelle);
		}
		if (botrshelle.sh == m_dummysh) {
			tshDissolve(botleft);
		} else {
			tshBond(botleft, botrshelle);
		}
		if (toprshelle.sh == m_dummysh) {
			tshDissolve(botright);
		} else {
			tshBond(botright, toprshelle);
		}
	}
	
	/* New triPOINT assignments for the rotated quadrilateral. */
//	if( farpoint == botpoint || botpoint == rightpoint || rightpoint == farpoint ) {
//		printf("internal error\n");
//	}

	SetOrg(*flipedge, farpoint);
	SetDest(*flipedge, botpoint);
	SetApex(*flipedge, rightpoint);

//	if( botpoint == leftpoint || leftpoint == farpoint ) {
//		printf("internal error\n");
//	}

	SetOrg(top, botpoint);
	SetDest(top, farpoint);
	SetApex(top, leftpoint);
	if (m_verbose > 2) {
		printf("  Edge flip results in left ");
		NextEdgeSelf(topleft);
		printtriangle(&topleft);
		printf("  and right ");
		printtriangle(flipedge);
	}
}



/************************** distance of a triPOINT to a plane  *******************
triPOINT : p0
plane : p1,p2,p3
| x  y  z  1 |
| x1 y1 z1 1 | 
| x2 y2 z2 1 | = a*x + b*y + c*z +d = 0;
| x3 y3 z3 1 |

 distance : d = t*sqrt(a*a+b*b+c*c)
	x = x0 + a*t = x0 + cosA * d
	y = y0 + b*t = y0 + cosB * d
	z = z0 + c*t = z0 + cosC * d
	
	 t = (a*x0 + b*y0 + c*z0 + d)/ (a*a+b*b+c*c)
	 d = t * sqrt(a*a+b*b+c*c) = (a*x0 + b*y0 + c*z0 + d)/sqrt(a*a+b*b+c*c)
**********************************************************************************/
typedef struct tag_tinPOINT3d {
	REAL x,y,z;
}	POINT3d;


double P0ToPlane(POINT3d *p0,POINT3d *p1,POINT3d *p2,POINT3d *p3)
{
	double d3;
	double a,b,c,d;			
	
	a =  p1->y * (p2->z - p3->z ) + p2->y * ( p3->z - p1->z) + p3->y * ( p1->z - p2->z );
	b =  p1->z * (p2->x - p3->x ) + p2->z * ( p3->x - p1->x) + p3->z * ( p1->x - p2->x );
	c=   p1->x * (p2->y - p3->y ) + p2->x * ( p3->y - p1->y) + p3->x * ( p1->y - p2->y );
	d= p1->x*(p2->y*p3->z - p3->y*p2->z)+ p2->x*(p3->y*p1->z - p1->y*p3->z) + p3->x*( p1->y*p2->z - p2->y*p1->z);
	
	d3 = (a*p0->x+b*p0->y+c*p0->z+d)/sqrt(a*a+b*b+c*c);
	
	/*
	if( fabs(d3)  < 1) {
	printf( "p0,d3 = %.3f %.3f %.3f, %.3f \n", p0->x,p0->y,p0->z,d3);
	printf( "p1,p2,p3,d3 = %.3f %.3f %.3f,  %.3f %.3f %.3f,  %.3f %.3f %.3f\n", 
	p1->x,p1->y,p1->z,p2->x,p2->y,p2->z,p3->x,p3->y,p3->z);
	}
	*/
	if( d3 < 0 ) 
		return (-d3);
	else	return d3;
}

/*****************************************************************************

 InsertSite()   Insert a vertex into a Delaunay triangulation,            
 performing flips as necessary to maintain the Delaunay    
 property.                                                 
 
  The triPOINT `insertpoint' is located.  If `searchtri.tri' is not NULL,     
  the search for the containing TRIANGLE begins from `searchtri'.  If      
  `searchtri.tri' is NULL, a full triPOINT location procedure is called.      
  
   If `insertpoint' is found inside a TRIANGLE, the TRIANGLE is split into  
   three;
   
	if `insertpoint' lies on an EDGE, the EDGE is split in two,       		
	thereby splitting the two adjacent m_triangles into four.  Edge flips are  
	used to restore the Delaunay property.  I					
	
	 if `insertpoint' lies on an existing vertex, no action is taken, and 
	 the value DUPLICATEPOINT is returned.  On return, `searchtri' is set 
	 to a handle whose origin is the existing vertex.                                                         
	 
	  Normally, the parameter `splitedge' is set to NULL, implying that no     
	  segment should be split.  In this case, if `insertpoint' is found to     
	  lie on a segment, no action is taken, and the value VIOLATINGPOINT is    
	  returned.  On return, `searchtri' is set to a handle whose primary EDGE  
	  is the violated segment.                                                 
	  
	   If the calling routine wishes to split a segment by inserting a triPOINT in 
	   it, the parameter `splitedge' should be that segment.  In this case,     
	   `searchtri' MUST be the TRIANGLE handle reached by pivoting from that    
	   segment; no triPOINT location is done.                                      
	   
		`segmentflaws' and `triflaws' are flags that indicate whether or not     
		there should be checks for the creation of encroached segments or bad    
		quality faces.  If a newly inserted triPOINT encroaches upon segments,      
		these segments are added to the list of segments to be split if          
		`segmentflaws' is set.  If bad m_triangles are created, these are added    
		to the queue if `triflaws' is set.                                       
		
		 If a duplicate triPOINT or violated segment does not prevent the triPOINT      
		 from being inserted, the return value will be ENCROACHINGPOINT if the    
		 triPOINT encroaches upon a segment (and checking is enabled), or            
		 SUCCESSFULPOINT otherwise.  In either case, `searchtri' is set to a      
		 handle whose origin is the newly inserted vertex.                        
		 
		  InsertSite() does not use flip() for reasons of speed; some              
		  information can be reused from EDGE flip to EDGE flip, like the          
		  locations of shell edges.                                                
		  
****************************************************************************/
enum insertsiteresult CTINClass::InsertSite(triPOINT *insertpoint,triEDGE*searchtri,
											EDGE *splitedge,int segmentflaws,int triflaws)
{
	int i;
	triEDGE horiz;
	triEDGE top;
	triEDGE botleft, botright;
	triEDGE topleft, topright;
	triEDGE newbotleft, newbotright;
	triEDGE newtopright;
	triEDGE botlcasing, botrcasing;
	triEDGE toplcasing, toprcasing;
	triEDGE testtri;
	EDGE botlshelle, botrshelle;
	EDGE toplshelle, toprshelle;
	EDGE brokenshelle;
	EDGE checkshelle;
	EDGE rightedge;
	EDGE newedge;
	EDGE *encroached;
	triPOINT *first;
	triPOINT *leftpoint, *rightpoint, *botpoint, *toppoint, *farpoint;
	float attrib, area;
	enum insertsiteresult success;
	enum locateresult intersect;
	int doflip;
	int mirrorflag;
	
	if (m_verbose > 1) 
		printf("  Inserting (%.12g, %.12g).\n", insertpoint->x, insertpoint->y);
	
	if (splitedge == (EDGE *) NULL) {
		/* Find the location of the triPOINT to be inserted.  Check if a good */
		/*   starting TRIANGLE has already been provided by the caller.    */
		if (searchtri->tri == (TRIANGLE *) NULL) {
			/* Find a boundary TRIANGLE. */
			horiz.tri = m_dummytri;
			horiz.orient = 0;
			SymmTriSelf(horiz);
			
			/* Search for a TRIANGLE containing `insertpoint'. */
			intersect = locate(insertpoint, &horiz);
		} 
		else {
			/* Start searching from the TRIANGLE provided by the caller. */
			TriEdgeCopy(*searchtri, horiz);
			intersect = PreciseLocate(insertpoint, &horiz);
		}
		
		if( intersect == INTRIANGLE ) {
			rightpoint = org(horiz);
			leftpoint = dest(horiz);
			botpoint = apex(horiz);
			//if( P0ToPlane((POINT3d*)insertpoint,(POINT3d*)rightpoint,(POINT3d*)leftpoint,(POINT3d*)botpoint) < 1 )
			//	return 	DUPLICATEPOINT;
		}		
	}
	else {
		/* The calling routine provides the EDGE in which the triPOINT is inserted. */
		TriEdgeCopy(*searchtri, horiz);
		intersect = ONEDGE;
	}
	if (intersect == ONVERTEX) {
		/* There's already a vertex there.  Return in `searchtri' a TRIANGLE */
		/*   whose origin is the existing vertex.                            */
		TriEdgeCopy(horiz, *searchtri);
		TriEdgeCopy(horiz, m_recentTri);

		return DUPLICATEPOINT;
	}
	
	if ((intersect == ONEDGE) || (intersect == OUTSIDE)) {
		/* The vertex falls on an EDGE or boundary. */
		if (m_checksegments && (splitedge == (EDGE *) NULL)) {
			/* Check whether the vertex falls on a shell EDGE. */
			tspivot(horiz, brokenshelle);
			if (brokenshelle.sh != m_dummysh) {
				/* The vertex falls on a shell EDGE. */
				if (segmentflaws) {
					if (nobisect == 0) {
						/* Add the shell EDGE to the list of encroached segments. */
						encroached = (EDGE *) m_badSegments.PoolAlloc();
						shellecopy(brokenshelle, *encroached);
					} else if ((nobisect == 1) && (intersect == ONEDGE)) {
						/* This segment may be split only if it is an internal boundary. */
						SymmTri(horiz, testtri);
						if (testtri.tri != m_dummytri) {
							/* Add the shell EDGE to the list of encroached segments. */
							encroached = (EDGE *) m_badSegments.PoolAlloc();
							shellecopy(brokenshelle, *encroached);
						}
					}
				}
				/* Return a handle whose primary EDGE contains the triPOINT, */
				/*   which has not been inserted.                         */
				TriEdgeCopy(horiz, *searchtri);
				TriEdgeCopy(horiz, m_recentTri);
				return VIOLATINGPOINT;
			}
		}
		/* Insert the triPOINT on an EDGE, dividing one TRIANGLE into two (if */
		/*   the EDGE lies on a boundary) or two m_triangles into four.      */
		PrevEdge(horiz, botright);
		SymmTri(botright, botrcasing);
		SymmTri(horiz, topright);
		/* Is there a second TRIANGLE?  (Or does this EDGE lie on a boundary?) */
		mirrorflag = topright.tri != m_dummytri;
		if (mirrorflag) {
			NextEdgeSelf(topright);
			SymmTri(topright, toprcasing);
			maketriangle(&newtopright);
		}
		else {
			/* Splitting the boundary EDGE increases the number of boundary edges. */
			hullsize++;
		}
		maketriangle(&newbotright);
		
		/* Set the vertices of changed and new triangles. */
		rightpoint = org(horiz);
		leftpoint = dest(horiz);
		botpoint = apex(horiz);
		SetOrg(newbotright, botpoint);
		SetDest(newbotright, rightpoint);
		SetApex(newbotright, insertpoint);
		SetOrg(horiz, insertpoint);
		for (i = 0; i < eextras; i++) {
			/* Set the element attributes of a new TRIANGLE. */
			setelemattribute(newbotright, i, elemattribute(botright, i));
		}
		if (vararea) {
			/* Set the area constraint of a new TRIANGLE. */
			setareabound(newbotright, (float)areabound(botright));
		}
		if (mirrorflag) {
			toppoint = dest(topright);
			SetOrg(newtopright, rightpoint);
			SetDest(newtopright, toppoint);
			SetApex(newtopright, insertpoint);
			SetOrg(topright, insertpoint);
			for (i = 0; i < eextras; i++) {
				/* Set the element attributes of another new TRIANGLE. */
				setelemattribute(newtopright, i, elemattribute(topright, i));
			}
			if (vararea) {
				/* Set the area constraint of another new TRIANGLE. */
				setareabound(newtopright, (float)areabound(topright));
			}
		}
		
		/* There may be shell edges that need to be bonded */
		/*   to the new TRIANGLE(s).                       */
		if (m_checksegments) {
			tspivot(botright, botrshelle);
			if (botrshelle.sh != m_dummysh) {
				tshDissolve(botright);
				tshBond(newbotright, botrshelle);
			}
			if (mirrorflag) {
				tspivot(topright, toprshelle);
				if (toprshelle.sh != m_dummysh) {
					tshDissolve(topright);
					tshBond(newtopright, toprshelle);
				}
			}
		}
		
		/* Bond the new TRIANGLE(s) to the surrounding m_triangles. */
		bond(newbotright, botrcasing);
		PrevEdgeSelf(newbotright);
		bond(newbotright, botright);
		PrevEdgeSelf(newbotright);
		if (mirrorflag) {
			bond(newtopright, toprcasing);
			NextEdgeSelf(newtopright);
			bond(newtopright, topright);
			NextEdgeSelf(newtopright);
			bond(newtopright, newbotright);
		}
		
		if (splitedge != (EDGE *) NULL) {
			/* Split the shell EDGE into two. */
			setshDest(*splitedge, insertpoint);
			shSymmTriSelf(*splitedge);
			spivot(*splitedge, rightedge);
			insertshelle(&newbotright, shMark(*splitedge));
			tspivot(newbotright, newedge);
			shBond(*splitedge, newedge);
			shSymmTriSelf(newedge);
			shBond(newedge, rightedge);
			shSymmTriSelf(*splitedge);
		}
		
#ifdef SELF_CHECK
		if (counterclockwise(rightpoint, leftpoint, botpoint) < 0.0) {
			printf("Internal error in InsertSite():\n");
			printf("  Clockwise TRIANGLE prior to EDGE triPOINT insertion (bottom).\n");
		}
		if (mirrorflag) {
			if (counterclockwise(leftpoint, rightpoint, toppoint) < 0.0) {
				printf("Internal error in InsertSite():\n");
				printf("  Clockwise TRIANGLE prior to EDGE triPOINT insertion (top).\n");
			}
			if (counterclockwise(rightpoint, toppoint, insertpoint) < 0.0) {
				printf("Internal error in InsertSite():\n");
				printf("  Clockwise TRIANGLE after EDGE triPOINT insertion (top right).\n");
			}
			if (counterclockwise(toppoint, leftpoint, insertpoint) < 0.0) {
				printf("Internal error in InsertSite():\n");
				printf("  Clockwise TRIANGLE after EDGE triPOINT insertion (top left).\n");
			}
		}
		if (counterclockwise(leftpoint, botpoint, insertpoint) < 0.0) {
			printf("Internal error in InsertSite():\n");
			printf("  Clockwise TRIANGLE after EDGE triPOINT insertion (bottom left).\n");
		}
		if (counterclockwise(botpoint, rightpoint, insertpoint) < 0.0) {
			printf("Internal error in InsertSite():\n");
			printf("  Clockwise TRIANGLE after EDGE triPOINT insertion (bottom right).\n");
		}
#endif /* SELF_CHECK */
		
		if (m_verbose > 2) {
			printf("  Updating bottom left ");
			printtriangle(&botright);
			if (mirrorflag) {
				printf("  Updating top left ");
				printtriangle(&topright);
				printf("  Creating top right ");
				printtriangle(&newtopright);
			}
			printf("  Creating bottom right ");
			printtriangle(&newbotright);
		}
		
		/* Position `horiz' on the first EDGE to check for */
		/*   the Delaunay property.                        */
		NextEdgeSelf(horiz);
	} else {
		/* Insert the triPOINT in a TRIANGLE, splitting it into three. */
		NextEdge(horiz, botleft);
		PrevEdge(horiz, botright);
		SymmTri(botleft, botlcasing);
		SymmTri(botright, botrcasing);
		maketriangle(&newbotleft);
		maketriangle(&newbotright);
		
		/* Set the vertices of changed and new m_triangles. */
		rightpoint = org(horiz);
		leftpoint = dest(horiz);
		botpoint = apex(horiz);
		SetOrg(newbotleft, leftpoint);
		SetDest(newbotleft, botpoint);
		SetApex(newbotleft, insertpoint);
		SetOrg(newbotright, botpoint);
		SetDest(newbotright, rightpoint);
		SetApex(newbotright, insertpoint);
		SetApex(horiz, insertpoint);
		
		for (i = 0; i < eextras; i++) {
			/* Set the element attributes of the new m_triangles. */
			attrib = elemattribute(horiz, i);
			setelemattribute(newbotleft, i, attrib);
			setelemattribute(newbotright, i, attrib);
		}
		if (vararea) {
			/* Set the area constraint of the new m_triangles. */
			area = (float)areabound(horiz);
			setareabound(newbotleft, area);
			setareabound(newbotright, area);
		}
		
		/* There may be shell edges that need to be bonded */
		/*   to the new m_triangles.                         */
		if (m_checksegments) {
			tspivot(botleft, botlshelle);
			if (botlshelle.sh != m_dummysh) {
				tshDissolve(botleft);
				tshBond(newbotleft, botlshelle);
			}
			tspivot(botright, botrshelle);
			if (botrshelle.sh != m_dummysh) {
				tshDissolve(botright);
				tshBond(newbotright, botrshelle);
			}
		}
		
		/* Bond the new m_triangles to the surrounding m_triangles. */
		bond(newbotleft, botlcasing);
		bond(newbotright, botrcasing);
		NextEdgeSelf(newbotleft);
		PrevEdgeSelf(newbotright);
		bond(newbotleft, newbotright);
		NextEdgeSelf(newbotleft);
		bond(botleft, newbotleft);
		PrevEdgeSelf(newbotright);
		bond(botright, newbotright);
		
#ifdef SELF_CHECK
		if (counterclockwise(rightpoint, leftpoint, botpoint) < 0.0) {
			printf("Internal error in InsertSite():\n");
			printf("  Clockwise TRIANGLE prior to triPOINT insertion.\n");
		}
		if (counterclockwise(rightpoint, leftpoint, insertpoint) < 0.0) {
			printf("Internal error in InsertSite():\n");
			printf("  Clockwise TRIANGLE after triPOINT insertion (top).\n");
		}
		if (counterclockwise(leftpoint, botpoint, insertpoint) < 0.0) {
			printf("Internal error in InsertSite():\n");
			printf("  Clockwise TRIANGLE after triPOINT insertion (left).\n");
		}
		if (counterclockwise(botpoint, rightpoint, insertpoint) < 0.0) {
			printf("Internal error in InsertSite():\n");
			printf("  Clockwise TRIANGLE after triPOINT insertion (right).\n");
		}
#endif /* SELF_CHECK */
		
		if (m_verbose > 2) {
			printf("  Updating top ");
			printtriangle(&horiz);
			printf("  Creating left ");
			printtriangle(&newbotleft);
			printf("  Creating right ");
			printtriangle(&newbotright);
		}
	}
	
	/* The insertion is successful by default, unless an encroached */
	/*   EDGE is found.                                             */
	success = SUCCESSFULPOINT;
	/* Circle around the newly inserted vertex, checking each EDGE opposite */
	/*   it for the Delaunay property.  Non-Delaunay edges are flipped.     */
	/*   `horiz' is always the EDGE being checked.  `first' marks where to  */
	/*   stop circling.                                                     */
	first = org(horiz);
	rightpoint = first;
	leftpoint = dest(horiz);
	/* Circle until finished. */
	while (1) {
		/* By default, the EDGE will be flipped. */
		doflip = 1;
		if (m_checksegments) {
			/* Check for a segment, which cannot be flipped. */
			tspivot(horiz, checkshelle);
			if (checkshelle.sh != m_dummysh) {
				/* The EDGE is a segment and cannot be flipped. */
				doflip = 0;
#ifndef CDT_ONLY
				if (segmentflaws) {
					/* Does the new triPOINT encroach upon this segment? */
					if (checkedge4encroach(&checkshelle)) {
						success = ENCROACHINGPOINT;
					}
				}
#endif /* not CDT_ONLY */
			}
		}
		if (doflip) {
			/* Check if the EDGE is a boundary EDGE. */
			SymmTri(horiz, top);
			if (top.tri == m_dummytri) {
				/* The EDGE is a boundary EDGE and cannot be flipped. */
				doflip = 0;
			} else {
				/* Find the triPOINT on the other side of the EDGE. */
				farpoint = apex(top);
				/* In the incremental Delaunay triangulation algorithm, any of    */
				/*   `leftpoint', `rightpoint', and `farpoint' could be vertices  */
				/*   of the triangular bounding box.  These vertices must be      */
				/*   treated as if they are infinitely distant, even though their */
				/*   "coordinates" are not.                                       */
				if( (leftpoint == m_infpoint1) || (leftpoint == m_infpoint2)
					|| (leftpoint == m_infpoint3)) {
					/* `leftpoint' is infinitely distant.  Check the convexity of */
					/*   the boundary of the triangulation.  'farpoint' might be  */
					/*   infinite as well, but trust me, this same condition      */
					/*   should be applied.                                       */
					doflip = counterclockwise(insertpoint, rightpoint, farpoint) > 0.0;
				} else if ((rightpoint == m_infpoint1) || (rightpoint == m_infpoint2)
					|| (rightpoint == m_infpoint3)) {
					/* `rightpoint' is infinitely distant.  Check the convexity of */
					/*   the boundary of the triangulation.  'farpoint' might be  */
					/*   infinite as well, but trust me, this same condition      */
					/*   should be applied.                                       */
					doflip = counterclockwise(farpoint, leftpoint, insertpoint) > 0.0;
				} else if ((farpoint == m_infpoint1) || (farpoint == m_infpoint2)
					|| (farpoint == m_infpoint3)) {
					/* `farpoint' is infinitely distant and cannot be inside */
					/*   the circumcircle of the TRIANGLE `horiz'.           */
					doflip = 0;
				} else {
					/* Test whether the EDGE is locally Delaunay. */
					doflip = incircle(leftpoint, insertpoint, rightpoint, farpoint)
						> 0.0;
				}
				if (doflip) {
					/* We made it!  Flip the EDGE `horiz' by rotating its containing */
					/*   quadrilateral (the two m_triangles adjacent to `horiz').      */
					/* Identify the casing of the quadrilateral. */
					PrevEdge(top, topleft);
					SymmTri(topleft, toplcasing);
					NextEdge(top, topright);
					SymmTri(topright, toprcasing);
					NextEdge(horiz, botleft);
					SymmTri(botleft, botlcasing);
					PrevEdge(horiz, botright);
					SymmTri(botright, botrcasing);
					/* Rotate the quadrilateral one-quarter turn counterclockwise. */
					bond(topleft, botlcasing);
					bond(botleft, botrcasing);
					bond(botright, toprcasing);
					bond(topright, toplcasing);
					if (m_checksegments) {
						/* Check for shell edges and rebond them to the quadrilateral. */
						tspivot(topleft, toplshelle);
						tspivot(botleft, botlshelle);
						tspivot(botright, botrshelle);
						tspivot(topright, toprshelle);
						if (toplshelle.sh == m_dummysh) {
							tshDissolve(topright);
						} else {
							tshBond(topright, toplshelle);
						}
						if (botlshelle.sh == m_dummysh) {
							tshDissolve(topleft);
						} else {
							tshBond(topleft, botlshelle);
						}
						if (botrshelle.sh == m_dummysh) {
							tshDissolve(botleft);
						} else {
							tshBond(botleft, botrshelle);
						}
						if (toprshelle.sh == m_dummysh) {
							tshDissolve(botright);
						} else {
							tshBond(botright, toprshelle);
						}
					}
					/* New triPOINT assignments for the rotated quadrilateral. */
					SetOrg(horiz, farpoint);
					SetDest(horiz, insertpoint);
					SetApex(horiz, rightpoint);
					SetOrg(top, insertpoint);
					SetDest(top, farpoint);
					SetApex(top, leftpoint);
					for (i = 0; i < eextras; i++) {
						/* Take the average of the two m_triangles' attributes. */
						attrib = (float) (0.5 * (elemattribute(top, i) + elemattribute(horiz, i)));
						setelemattribute(top, i, attrib);
						setelemattribute(horiz, i, attrib);
					}
					if (vararea) {
						if ((areabound(top) <= 0.0) || (areabound(horiz) <= 0.0)) {
							area = -1.0;
						} else {
							/* Take the average of the two m_triangles' area constraints.    */
							/*   This prevents small area constraints from migrating a     */
							/*   long, long way from their original location due to flips. */
							area = (float)(0.5 * (areabound(top) + areabound(horiz)));
						}
						setareabound(top, area);
						setareabound(horiz, area);
					}
#ifdef SELF_CHECK
					if (insertpoint != (triPOINT *) NULL) {
						if (counterclockwise(leftpoint, insertpoint, rightpoint) < 0.0) {
							printf("Internal error in InsertSite():\n");
							printf("  Clockwise TRIANGLE prior to EDGE flip (bottom).\n");
						}
						/* The following test has been removed because constrainededge() */
						/*   sometimes generates inverted m_triangles that InsertSite()    */
						/*   removes.                                                    */
						/*
						if (counterclockwise(rightpoint, farpoint, leftpoint) < 0.0) {
						printf("Internal error in InsertSite():\n");
						printf("  Clockwise TRIANGLE prior to EDGE flip (top).\n");
						}
						*/
						if (counterclockwise(farpoint, leftpoint, insertpoint) < 0.0) {
							printf("Internal error in InsertSite():\n");
							printf("  Clockwise TRIANGLE after EDGE flip (left).\n");
						}
						if (counterclockwise(insertpoint, rightpoint, farpoint) < 0.0) {
							printf("Internal error in InsertSite():\n");
							printf("  Clockwise TRIANGLE after EDGE flip (right).\n");
						}
					}
#endif /* SELF_CHECK */
					if (m_verbose > 2) {
						printf("  Edge flip results in left ");
						NextEdgeSelf(topleft);
						printtriangle(&topleft);
						printf("  and right ");
						printtriangle(&horiz);
					}
					/* On the next iterations, consider the two edges that were  */
					/*   exposed (this is, are now visible to the newly inserted */
					/*   triPOINT) by the EDGE flip.                                */
					PrevEdgeSelf(horiz);
					leftpoint = farpoint;
				}
			}
		}
		if (!doflip) {
			/* The handle `horiz' is accepted as locally Delaunay. */
#ifndef CDT_ONLY
			if (triflaws) {
				/* Check the TRIANGLE `horiz' for quality. */
				testtriangle(&horiz);
			}
#endif /* not CDT_ONLY */
			/* Look for the next EDGE around the newly inserted triPOINT. */
			NextEdgeSelf(horiz);
			SymmTri(horiz, testtri);
			/* Check for finishing a complete revolution about the new triPOINT, or */
			/*   falling off the EDGE of the triangulation.  The latter will     */
			/*   happen when a triPOINT is inserted at a boundary.                  */
			if ((leftpoint == first) || (testtri.tri == m_dummytri)) {
				/* We're done.  Return a TRIANGLE whose origin is the new triPOINT. */
				NextEdge(horiz, *searchtri);
				NextEdge(horiz, m_recentTri);
				return success;
			}
			/* Finish finding the next EDGE around the newly inserted triPOINT. */
			NextEdge(testtri, horiz);
			rightpoint = leftpoint;
			leftpoint = dest(horiz );
		}
	}
}

/*****************************************************************************/
/*                                                                           */
/*  triangulatepolygon()   Find the Delaunay triangulation of a polygon that */
/*                         has a certain "nice" shape.  This includes the    */
/*                         polygons that result from deletion of a triPOINT or  */
/*                         insertion of a segment.                           */
/*                                                                           */
/*  This is a conceptually difficult routine.  The starting assumption is    */
/*  that we have a polygon with n sides.  n - 1 of these sides are currently */
/*  represented as edges in the mesh.  One side, called the "base", need not */
/*  be.                                                                      */
/*                                                                           */
/*  Inside the polygon is a structure I call a "fan", consisting of n - 1    */
/*  m_triangles that share a common origin.  For each of these m_triangles, the  */
/*  EDGE opposite the origin is one of the sides of the polygon.  The        */
/*  primary EDGE of each TRIANGLE is the EDGE directed from the origin to    */
/*  the destination; note that this is not the same EDGE that is a side of   */
/*  the polygon.  `firstedge' is the primary EDGE of the first TRIANGLE.     */
/*  From there, the m_triangles follow in counterclockwise m_order about the     */
/*  polygon, until `lastedge', the primary EDGE of the last TRIANGLE.        */
/*  `firstedge' and `lastedge' are probably connected to other m_triangles     */
/*  beyond the extremes of the fan, but their identity is not important, as  */
/*  long as the fan remains connected to them.                               */
/*                                                                           */
/*  Imagine the polygon oriented so that its base is at the bottom.  This    */
/*  puts `firstedge' on the far right, and `lastedge' on the far left.       */
/*  The right vertex of the base is the destination of `firstedge', and the  */
/*  left vertex of the base is the apex of `lastedge'.                       */
/*                                                                           */
/*  The challenge now is to find the right sequence of EDGE flips to         */
/*  transform the fan into a Delaunay triangulation of the polygon.  Each    */
/*  EDGE flip effectively removes one TRIANGLE from the fan, committing it   */
/*  to the polygon.  The resulting polygon has one fewer EDGE.  If `doflip'  */
/*  is set, the final flip will be performed, resulting in a fan of one      */
/*  (useless?) TRIANGLE.  If `doflip' is not set, the final flip is not      */
/*  performed, resulting in a fan of two m_triangles, and an unfinished        */
/*  triangular polygon that is not yet filled out with a single TRIANGLE.    */
/*  On completion of the routine, `lastedge' is the last remaining TRIANGLE, */
/*  or the leftmost of the last two.                                         */
/*                                                                           */
/*  Although the flips are performed in the m_order described above, the       */
/*  decisions about what flips to perform are made in precisely the reverse  */
/*  m_order.  The recursive triangulatepolygon() procedure makes a decision,   */
/*  uses up to two recursive calls to triangulate the "subproblems"          */
/*  (polygons with fewer edges), and then performs an EDGE flip.             */
/*                                                                           */
/*  The "decision" it makes is which vertex of the polygon should be         */
/*  connected to the base.  This decision is made by testing every possible  */
/*  vertex.  Once the best vertex is found, the two edges that connect this  */
/*  vertex to the base become the bases for two smaller polygons.  These     */
/*  are triangulated recursively.  Unfortunately, this approach can take     */
/*  O(n^2) time not only in the worst case, but in many common cases.  It's  */
/*  rarely a big deal for triPOINT deletion, where n is rarely larger than ten, */
/*  but it could be a big deal for segment insertion, especially if there's  */
/*  a lot of long segments that each cut many m_triangles.  I ought to code    */
/*  a faster algorithm some time.                                            */
/*                                                                           */
/*  The `edgecount' parameter is the number of sides of the polygon,         */
/*  including its base.  `triflaws' is a flag that determines whether the    */
/*  new m_triangles should be tested for quality, and enqueued if they are     */
/*  bad.                                                                     */
/*                                                                           */
/*****************************************************************************/

void CTINClass::triangulatepolygon(triEDGE*firstedge,triEDGE*lastedge,
								int edgecount,int doflip,int triflaws)
{
	triEDGE testtri;
	triEDGE besttri;
	triEDGE tempedge;
	triPOINT *leftbasepoint, *rightbasepoint;
	triPOINT *testpoint;
	triPOINT *bestpoint;
	int bestnumber;
	int i;
	
	/* Identify the base vertices. */
	leftbasepoint = apex(*lastedge);
	rightbasepoint = dest(*firstedge);
	if (m_verbose > 2) {
		printf("  Triangulating interior polygon at EDGE\n");
		printf("    (%.12g, %.12g) (%.12g, %.12g)\n", leftbasepoint->x,
			leftbasepoint->y, rightbasepoint->x, rightbasepoint->y);
	}
	/* Find the best vertex to connect the base to. */
	oNextSpinEdge(*firstedge, besttri);
	bestpoint = dest(besttri );
	TriEdgeCopy(besttri, testtri);
	bestnumber = 1;
	for (i = 2; i <= edgecount - 2; i++) {
		oNextSpinEdgeSelf(testtri);
		testpoint = dest(testtri);
		/* Is this a better vertex? */
		if (incircle(leftbasepoint, rightbasepoint, bestpoint, testpoint) > 0.0) {
			TriEdgeCopy(testtri, besttri);
			bestpoint = testpoint;
			bestnumber = i;
		}
	}
	if (m_verbose > 2) {
		printf("    Connecting EDGE to (%.12g, %.12g)\n", bestpoint->x,
			bestpoint->y);
	}
	if (bestnumber > 1) {
		/* Recursively triangulate the smaller polygon on the right. */
		oPrevSpinEdge(besttri, tempedge);
		triangulatepolygon(firstedge, &tempedge, bestnumber + 1, 1, triflaws);
	}
	if (bestnumber < edgecount - 2) {
		/* Recursively triangulate the smaller polygon on the left. */
		SymmTri(besttri, tempedge);
		triangulatepolygon(&besttri, lastedge, edgecount - bestnumber, 1,
			triflaws);
		/* Find `besttri' again; it may have been lost to EDGE flips. */
		SymmTri(tempedge, besttri);
	}
	if (doflip) {
		/* Do one final EDGE flip. */
		flip(&besttri);
#ifndef CDT_ONLY
		if (triflaws) {
			/* Check the quality of the newly committed TRIANGLE. */
			SymmTri(besttri, testtri);
			testtriangle(&testtri);
		}
#endif /* not CDT_ONLY */
	}
	/* Return the base TRIANGLE. */
	TriEdgeCopy(besttri, *lastedge);
}

/*****************************************************************************/
/*                                                                           */
/*  DeletesSite()   Delete a vertex from a Delaunay triangulation, ensuring   */
/*                 that the triangulation remains Delaunay.                  */
/*                                                                           */
/*  The origin of `deltri' is deleted.  The union of the triangles adjacent  */
/*  to this triPOINT is a polygon, for which the Delaunay triangulation is      */
/*  found.  Two triangles are removed from the mesh.                         */
/*                                                                           */
/*  Only interior points that do not lie on segments (shell edges) or        */
/*  boundaries may be deleted.                                               */
/*                                                                           */
/*****************************************************************************/
#ifndef CDT_ONLY
void CTINClass::DeletesSite(triEDGE*deltri)
{
	triEDGE countingtri;
	triEDGE firstedge, lastedge;
	triEDGE deltriright;
	triEDGE leftTri, rightTri;
	triEDGE leftcasing, rightcasing;
	EDGE leftshelle, rightshelle;
	triPOINT *delpoint;
	triPOINT *neworg;
	int edgecount;
	
	delpoint = org(*deltri );
	if (m_verbose > 1) 
		printf("  Deleting (%.12g, %.12g).\n", delpoint->x, delpoint->y);
	
	PointDealloc(delpoint);
	
	/* Count the degree of the triPOINT being deleted. */
	oNextSpinEdge(*deltri, countingtri);
	edgecount = 1;
	while ( !TriEdgeEqual(*deltri, countingtri)) {
#ifdef SELF_CHECK
		if (countingtri.tri == m_dummytri) {
			printf("Internal error in DeletesSite():\n");
			printf("  Attempt to delete boundary triPOINT.\n");
			internalerror();
		}
#endif /* SELF_CHECK */
		edgecount++;
		oNextSpinEdgeSelf(countingtri);
	}
	
#ifdef SELF_CHECK
	if (edgecount < 3) {
		printf("Internal error in DeletesSite():\n  Point has degree %d.\n",edgecount);
		internalerror();
	}
#endif /* SELF_CHECK */
	if (edgecount > 3) {
		/* Triangulate the polygon defined by the union of all m_triangles */
		/*   adjacent to the triPOINT being deleted.  Check the quality of  */
		/*   the resulting m_triangles.                                    */
		oNextSpinEdge(*deltri, firstedge);
		oPrevSpinEdge(*deltri, lastedge);
		triangulatepolygon(&firstedge, &lastedge, edgecount, 0, !nobisect);
	}
	/* Splice out two m_triangles. */
	PrevEdge(*deltri, deltriright);
	dNextSpinEdge(*deltri, leftTri);
	SymmTri(leftTri, leftcasing);
	oPrevSpinEdge(deltriright, rightTri);
	SymmTri(rightTri, rightcasing);
	bond(*deltri, leftcasing);
	bond(deltriright, rightcasing);
	tspivot(leftTri, leftshelle);
	if (leftshelle.sh != m_dummysh) 
		tshBond(*deltri, leftshelle);
	tspivot(rightTri, rightshelle);
	if (rightshelle.sh != m_dummysh) 
		tshBond(deltriright, rightshelle);
	
	/* Set the new origin of `deltri' and check its quality. */
	neworg = org(leftTri );
	SetOrg(*deltri, neworg);
	if (!nobisect) 
		testtriangle(deltri);
	
	/* Delete the two spliced-out m_triangles. */
	triangleDealloc(leftTri.tri);
	triangleDealloc(rightTri.tri);
}

#endif /* not CDT_ONLY */



/**                                                                         **/
/**                                                                         **/
/********* Mesh transformation routines end here                     *********/

/********* Divide-and-conquer Delaunay triangulation begins here     *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  The divide-and-conquer bounding box                                      */
/*                                                                           */
/*  I originally implemented the divide-and-conquer and incremental Delaunay */
/*  triangulations using the EDGE-based data structure presented by Guibas   */
/*  and Stolfi.  Switching to a TRIANGLE-based data structure doubled the    */
/*  speed.  However, I had to think of a few extra tricks to maintain the    */
/*  elegance of the original algorithms.                                     */
/*                                                                           */
/*  The "bounding box" used by my variant of the divide-and-conquer          */
/*  algorithm uses one TRIANGLE for each EDGE of the m_convex hull of the      */
/*  triangulation.  These bounding m_triangles all share a common apical       */
/*  vertex, which is represented by NULL and which represents nothing.       */
/*  The bounding m_triangles are linked in a circular fan about this NULL      */
/*  vertex, and the edges on the m_convex hull of the triangulation appear     */
/*  opposite the NULL vertex.  You might find it easiest to imagine that     */
/*  the NULL vertex is a triPOINT in 3D space behind the center of the          */
/*  triangulation, and that the bounding m_triangles form a sort of cone.      */
/*                                                                           */
/*  This bounding box makes it easy to represent degenerate cases.  For      */
/*  instance, the triangulation of two vertices is a single EDGE.  This EDGE */
/*  is represented by two bounding box m_triangles, one on each "side" of the  */
/*  EDGE.  These m_triangles are also linked together in a fan about the NULL  */
/*  vertex.                                                                  */
/*                                                                           */
/*  The bounding box also makes it easy to Traverse the m_convex hull, as the  */
/*  divide-and-conquer algorithm needs to do.                                */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  pointsort()   Sort an array of m_points by x-coordinate, using the         */
/*                y-coordinate as a secondary key.                           */
/*                                                                           */
/*  Uses quicksort.  Randomized O(n log n) time.  No, I did not make any of  */
/*  the usual quicksort mistakes.                                            */
/*                                                                           */
/*****************************************************************************/
void CTINClass::pointsort(triPOINT **sortarray,int arraysize)
{
	int left, right;
	int pivot;
	REAL pivotx, pivoty;
	triPOINT *temp;
	
	if( arraysize == 2 ) {
		/* Recursive base case. */
		if( (sortarray[0]->x > sortarray[1]->x ) ||
			((sortarray[0]->x == sortarray[1]->x) &&
			(sortarray[0]->y > sortarray[1]->y))) {
			temp = sortarray[1];
			sortarray[1] = sortarray[0];
			sortarray[0] = temp;
		}
		return;
	}
	/* Choose a random pivot to split the array. */
	pivot = (int) randomnation(arraysize);
	pivotx = sortarray[pivot]->x;
	pivoty = sortarray[pivot]->y;
	/* Split the array. */
	left = -1;
	right = arraysize;
	while (left < right) {
		/* Search for a triPOINT whose x-coordinate is too large for the left. */
		do {
			left++;
		} while ((left <= right) && ((sortarray[left]->x < pivotx) ||
			((sortarray[left]->x == pivotx) &&
			(sortarray[left]->y < pivoty))));
		/* Search for a triPOINT whose x-coordinate is too small for the right. */
		do {
			right--;
		} while ((left <= right) && ((sortarray[right]->x > pivotx) ||
			((sortarray[right]->x == pivotx) &&
			(sortarray[right]->y > pivoty))));
		if (left < right) {
			/* Swap the left and right m_points. */
			temp = sortarray[left];
			sortarray[left] = sortarray[right];
			sortarray[right] = temp;
		}
	}
	if (left > 1) {
		/* Recursively sort the left subset. */
		pointsort(sortarray, left);
	}
	if (right < arraysize - 2) {
		/* Recursively sort the right subset. */
		pointsort(&sortarray[right + 1], arraysize - right - 1);
	}
}

/*****************************************************************************/
/*                                                                           */
/*  pointmedian()   An m_order statistic algorithm, almost.  Shuffles an array */
/*                  of m_points so that the first `median' m_points occur        */
/*                  lexicographically before the remaining m_points.           */
/*                                                                           */
/*  Uses the x-coordinate as the primary key if axis == 0; the y-coordinate  */
/*  if axis == 1.  Very similar to the pointsort() procedure, but runs in    */
/*  randomized linear time.                                                  */
/*                                                                           */
/*****************************************************************************/

void CTINClass::pointmedian(REAL **sortarray,int arraysize,int median,int axis)
{
	int left, right;
	int pivot;
	REAL pivot1, pivot2;
	REAL *temp;
	
	if (arraysize == 2) {
		/* Recursive base case. */
		if ((sortarray[0][axis] > sortarray[1][axis]) ||
			((sortarray[0][axis] == sortarray[1][axis]) &&
			(sortarray[0][1 - axis] > sortarray[1][1 - axis]))) {
			temp = sortarray[1];
			sortarray[1] = sortarray[0];
			sortarray[0] = temp;
		}
		return;
	}
	/* Choose a random pivot to split the array. */
	pivot = (int) randomnation(arraysize);
	pivot1 = sortarray[pivot][axis];
	pivot2 = sortarray[pivot][1 - axis];
	/* Split the array. */
	left = -1;
	right = arraysize;
	while (left < right) {
		/* Search for a triPOINT whose x-coordinate is too large for the left. */
		do {
			left++;
		} while ((left <= right) && ((sortarray[left][axis] < pivot1) ||
			((sortarray[left][axis] == pivot1) &&
			(sortarray[left][1 - axis] < pivot2))));
		/* Search for a triPOINT whose x-coordinate is too small for the right. */
		do {
			right--;
		} while ((left <= right) && ((sortarray[right][axis] > pivot1) ||
			((sortarray[right][axis] == pivot1) &&
			(sortarray[right][1 - axis] > pivot2))));
		if (left < right) {
			/* Swap the left and right m_points. */
			temp = sortarray[left];
			sortarray[left] = sortarray[right];
			sortarray[right] = temp;
		}
	}
	/* Unlike in pointsort(), at most one of the following */
	/*   conditionals is true.                             */
	if (left > median) {
		/* Recursively shuffle the left subset. */
		pointmedian(sortarray, left, median, axis);
	}
	if (right < median - 1) {
		/* Recursively shuffle the right subset. */
		pointmedian(&sortarray[right + 1], arraysize - right - 1,
			median - right - 1, axis);
	}
}

/*****************************************************************************/
/*                                                                           */
/*  alternateaxes()   Sorts the m_points as appropriate for the divide-and-    */
/*                    conquer algorithm with alternating cuts.               */
/*                                                                           */
/*  Partitions by x-coordinate if axis == 0; by y-coordinate if axis == 1.   */
/*  For the base case, subsets containing only two or three m_points are       */
/*  always sorted by x-coordinate.                                           */
/*                                                                           */
/*****************************************************************************/

void CTINClass::alternateaxes(triPOINT **sortarray,int arraysize,int axis)
{
	int divider;
	
	divider = arraysize >> 1;
	if (arraysize <= 3) {
		/* Recursive base case:  subsets of two or three m_points will be      */
		/*   handled specially, and should always be sorted by x-coordinate. */
		axis = 0;
	}
	/* Partition with a horizontal or vertical cut. */
	pointmedian((REAL **)sortarray, arraysize, divider, axis);
	/* Recursively partition the subsets with a cross cut. */
	if (arraysize - divider >= 2) {
		if (divider >= 2) {
			alternateaxes(sortarray, divider, 1 - axis);
		}
		alternateaxes(&sortarray[divider], arraysize - divider, 1 - axis);
	}
}

/*****************************************************************************/
/*                                                                           */
/*  mergehulls()   Merge two adjacent Delaunay triangulations into a         */
/*                 single Delaunay triangulation.                            */
/*                                                                           */
/*  This is similar to the algorithm given by Guibas and Stolfi, but uses    */
/*  a TRIANGLE-based, rather than EDGE-based, data structure.                */
/*                                                                           */
/*  The algorithm walks up the gap between the two triangulations, knitting  */
/*  them together.  As they are merged, some of their bounding m_triangles     */
/*  are converted into real m_triangles of the triangulation.  The procedure   */
/*  pulls each hull's bounding m_triangles apart, then knits them together     */
/*  like the teeth of two gears.  The Delaunay property determines, at each  */
/*  step, whether the next "tooth" is a bounding TRIANGLE of the left hull   */
/*  or the right.  When a bounding TRIANGLE becomes real, its apex is        */
/*  changed from NULL to a real triPOINT.                                       */
/*                                                                           */
/*  Only two new m_triangles need to be allocated.  These become new bounding  */
/*  m_triangles at the top and bottom of the seam.  They are used to connect   */
/*  the remaining bounding m_triangles (those that have not been converted     */
/*  into real m_triangles) into a single fan.                                  */
/*                                                                           */
/*  On entry, `farleft' and `innerleft' are bounding m_triangles of the left   */
/*  triangulation.  The origin of `farleft' is the leftmost vertex, and      */
/*  the destination of `innerleft' is the rightmost vertex of the            */
/*  triangulation.  Similarly, `innerright' and `farright' are bounding      */
/*  m_triangles of the right triangulation.  The origin of `innerright' and    */
/*  destination of `farright' are the leftmost and rightmost vertices.       */
/*                                                                           */
/*  On completion, the origin of `farleft' is the leftmost vertex of the     */
/*  merged triangulation, and the destination of `farright' is the rightmost */
/*  vertex.                                                                  */
/*                                                                           */
/*****************************************************************************/

void CTINClass::mergehulls(triEDGE*farleft,triEDGE*innerleft,
						triEDGE*innerright,triEDGE*farright,int axis)
{
	triEDGE leftcand, rightcand;
	triEDGE baseedge;
	triEDGE nextedge;
	triEDGE sidecasing, topcasing, outercasing;
	triEDGE checkedge;
	triPOINT *innerleftdest;
	triPOINT *innerrightorg;
	triPOINT *innerleftapex, *innerrightapex;
	triPOINT *farleftpt, *farrightpt;
	triPOINT *farleftapex, *farrightapex;
	triPOINT *lowerleft, *lowerright;
	triPOINT *upperleft, *upperright;
	triPOINT *nextapex;
	triPOINT *checkvertex;
	int changemade;
	int badedge;
	int leftfinished, rightfinished;
	
	innerleftdest = dest(*innerleft);
	innerleftapex = apex(*innerleft);
	innerrightorg = org(*innerright);
	innerrightapex = apex(*innerright);
	/* Special treatment for horizontal cuts. */
	if (dwyer && (axis == 1)) {
		farleftpt = org(*farleft);
		farleftapex = apex(*farleft);
		farrightpt = dest(*farright);
		farrightapex = apex(*farright);
		/* The pointers to the extremal m_points are shifted to triPOINT to the */
		/*   topmost and bottommost triPOINT of each hull, rather than the    */
		/*   leftmost and rightmost m_points.                                */
		while (farleftapex->y < farleftpt->y) {
			NextEdgeSelf(*farleft);
			SymmTriSelf(*farleft);
			farleftpt = farleftapex;
			farleftapex = apex(*farleft);
		}
		SymmTri(*innerleft, checkedge);
		checkvertex = apex(checkedge);
		while (checkvertex->y > innerleftdest->y) {
			NextEdge(checkedge, *innerleft);
			innerleftapex = innerleftdest;
			innerleftdest = checkvertex;
			SymmTri(*innerleft, checkedge);
			checkvertex = apex(checkedge);
		}
		while (innerrightapex->y < innerrightorg->y) {
			NextEdgeSelf(*innerright);
			SymmTriSelf(*innerright);
			innerrightorg = innerrightapex;
			innerrightapex = apex(*innerright);
		}
		SymmTri(*farright, checkedge);
		checkvertex = apex(checkedge);
		while (checkvertex->y > farrightpt->y) {
			NextEdge(checkedge, *farright);
			farrightapex = farrightpt;
			farrightpt = checkvertex;
			SymmTri(*farright, checkedge);
			checkvertex = apex(checkedge);
		}
	}
	/* Find a line tangent to and below both hulls. */
	do {
		changemade = 0;
		/* Make innerleftdest the "bottommost" triPOINT of the left hull. */
		if (counterclockwise(innerleftdest, innerleftapex, innerrightorg) > 0.0) {
			PrevEdgeSelf(*innerleft);
			SymmTriSelf(*innerleft);
			innerleftdest = innerleftapex;
			innerleftapex = apex(*innerleft);
			changemade = 1;
		}
		/* Make innerrightorg the "bottommost" triPOINT of the right hull. */
		if (counterclockwise(innerrightapex, innerrightorg, innerleftdest) > 0.0) {
			NextEdgeSelf(*innerright);
			SymmTriSelf(*innerright);
			innerrightorg = innerrightapex;
			innerrightapex = apex(*innerright);
			changemade = 1;
		}
	} while (changemade);
	/* Find the two candidates to be the next "gear tooth". */
	SymmTri(*innerleft, leftcand);
	SymmTri(*innerright, rightcand);
	/* Create the bottom new bounding TRIANGLE. */
	maketriangle(&baseedge);
	/* Connect it to the bounding boxes of the left and right triangulations. */
	bond(baseedge, *innerleft);
	NextEdgeSelf(baseedge);
	bond(baseedge, *innerright);
	NextEdgeSelf(baseedge);
	SetOrg(baseedge, innerrightorg);
	SetDest(baseedge, innerleftdest);
	/* Apex is intentionally left NULL. */
	if (m_verbose > 2) {
		printf("  Creating base bounding ");
		printtriangle(&baseedge);
	}
	/* Fix the extreme m_triangles if necessary. */
	farleftpt = org(*farleft);
	if (innerleftdest == farleftpt) {
		NextEdge(baseedge, *farleft);
	}
	farrightpt = dest(*farright);
	if (innerrightorg == farrightpt) {
		PrevEdge(baseedge, *farright);
	}
	/* The vertices of the current knitting EDGE. */
	lowerleft = innerleftdest;
	lowerright = innerrightorg;
	/* The candidate vertices for knitting. */
	upperleft = apex(leftcand);
	upperright = apex(rightcand);
	/* Walk up the gap between the two triangulations, knitting them together. */
	while (1) {
		/* Have we reached the top?  (This isn't quite the right question,       */
		/*   because even though the left triangulation might seem finished now, */
		/*   moving up on the right triangulation might reveal a new triPOINT of    */
		/*   the left triangulation.  And vice-versa.)                           */
		leftfinished = counterclockwise(upperleft, lowerleft, lowerright) <= 0.0;
		rightfinished = counterclockwise(upperright, lowerleft, lowerright) <= 0.0;
		if (leftfinished && rightfinished) {
			/* Create the top new bounding TRIANGLE. */
			maketriangle(&nextedge);
			SetOrg(nextedge, lowerleft);
			SetDest(nextedge, lowerright);
			/* Apex is intentionally left NULL. */
			/* Connect it to the bounding boxes of the two triangulations. */
			bond(nextedge, baseedge);
			NextEdgeSelf(nextedge);
			bond(nextedge, rightcand);
			NextEdgeSelf(nextedge);
			bond(nextedge, leftcand);
			if (m_verbose > 2) {
				printf("  Creating top bounding ");
				printtriangle(&baseedge);
			}
			/* Special treatment for horizontal cuts. */
			if (dwyer && (axis == 1)) {
				farleftpt = org(*farleft);
				farleftapex = apex(*farleft);
				farrightpt = dest(*farright);
				farrightapex = apex(*farright);
				SymmTri(*farleft, checkedge);
				checkvertex = apex(checkedge);
				/* The pointers to the extremal m_points are restored to the leftmost */
				/*   and rightmost m_points (rather than topmost and bottommost).     */
				while (checkvertex->x < farleftpt->x) {
					PrevEdge(checkedge, *farleft);
					farleftapex = farleftpt;
					farleftpt = checkvertex;
					SymmTri(*farleft, checkedge);
					checkvertex = apex(checkedge);
				}
				while (farrightapex->x > farrightpt->x) {
					PrevEdgeSelf(*farright);
					SymmTriSelf(*farright);
					farrightpt = farrightapex;
					farrightapex = apex(*farright);
				}
			}
			return;
		}
		/* Consider eliminating edges from the left triangulation. */
		if (!leftfinished) {
			/* What vertex would be exposed if an EDGE were deleted? */
			PrevEdge(leftcand, nextedge);
			SymmTriSelf(nextedge);
			nextapex = apex(nextedge);
			/* If nextapex is NULL, then no vertex would be exposed; the */
			/*   triangulation would have been eaten right through.      */
			if (nextapex != (triPOINT *) NULL) {
				/* Check whether the EDGE is Delaunay. */
				badedge = incircle(lowerleft, lowerright, upperleft, nextapex) > 0.0;
				while (badedge) {
					/* Eliminate the EDGE with an EDGE flip.  As a result, the    */
					/*   left triangulation will have one more boundary TRIANGLE. */
					NextEdgeSelf(nextedge);
					SymmTri(nextedge, topcasing);
					NextEdgeSelf(nextedge);
					SymmTri(nextedge, sidecasing);
					bond(nextedge, topcasing);
					bond(leftcand, sidecasing);
					NextEdgeSelf(leftcand);
					SymmTri(leftcand, outercasing);
					PrevEdgeSelf(nextedge);
					bond(nextedge, outercasing);
					/* Correct the vertices to reflect the EDGE flip. */
					SetOrg(leftcand, lowerleft);
					SetDest(leftcand, NULL);
					SetApex(leftcand, nextapex);
					SetOrg(nextedge, NULL);
					SetDest(nextedge, upperleft);
					SetApex(nextedge, nextapex);
					/* Consider the newly exposed vertex. */
					upperleft = nextapex;
					/* What vertex would be exposed if another EDGE were deleted? */
					TriEdgeCopy(sidecasing, nextedge);
					nextapex = apex(nextedge);
					if (nextapex != (triPOINT *) NULL) {
						/* Check whether the EDGE is Delaunay. */
						badedge = incircle(lowerleft, lowerright, upperleft, nextapex)
							> 0.0;
					} else {
						/* Avoid CTINClass::eating right through the triangulation. */
						badedge = 0;
					}
				}
			}
		}
		/* Consider eliminating edges from the right triangulation. */
		if (!rightfinished) {
			/* What vertex would be exposed if an EDGE were deleted? */
			NextEdge(rightcand, nextedge);
			SymmTriSelf(nextedge);
			nextapex = apex(nextedge);
			/* If nextapex is NULL, then no vertex would be exposed; the */
			/*   triangulation would have been eaten right through.      */
			if (nextapex != (triPOINT *) NULL) {
				/* Check whether the EDGE is Delaunay. */
				badedge = incircle(lowerleft, lowerright, upperright, nextapex) > 0.0;
				while (badedge) {
					/* Eliminate the EDGE with an EDGE flip.  As a result, the     */
					/*   right triangulation will have one more boundary TRIANGLE. */
					PrevEdgeSelf(nextedge);
					SymmTri(nextedge, topcasing);
					PrevEdgeSelf(nextedge);
					SymmTri(nextedge, sidecasing);
					bond(nextedge, topcasing);
					bond(rightcand, sidecasing);
					PrevEdgeSelf(rightcand);
					SymmTri(rightcand, outercasing);
					NextEdgeSelf(nextedge);
					bond(nextedge, outercasing);
					/* Correct the vertices to reflect the EDGE flip. */
					SetOrg(rightcand, NULL);
					SetDest(rightcand, lowerright);
					SetApex(rightcand, nextapex);
					SetOrg(nextedge, upperright);
					SetDest(nextedge, NULL);
					SetApex(nextedge, nextapex);
					/* Consider the newly exposed vertex. */
					upperright = nextapex;
					/* What vertex would be exposed if another EDGE were deleted? */
					TriEdgeCopy(sidecasing, nextedge);
					nextapex = apex(nextedge);
					if (nextapex != (triPOINT *) NULL) {
						/* Check whether the EDGE is Delaunay. */
						badedge = incircle(lowerleft, lowerright, upperright, nextapex)
							> 0.0;
					} else {
						/* Avoid CTINClass::eating right through the triangulation. */
						badedge = 0;
					}
				}
			}
		}
		if (leftfinished || (!rightfinished &&
			(incircle(upperleft, lowerleft, lowerright, upperright) > 0.0))) {
			/* Knit the triangulations, adding an EDGE from `lowerleft' */
			/*   to `upperright'.                                       */
			bond(baseedge, rightcand);
			PrevEdge(rightcand, baseedge);
			SetDest(baseedge, lowerleft);
			lowerright = upperright;
			SymmTri(baseedge, rightcand);
			upperright = apex(rightcand);
		} else {
			/* Knit the triangulations, adding an EDGE from `upperleft' */
			/*   to `lowerright'.                                       */
			bond(baseedge, leftcand);
			NextEdge(leftcand, baseedge);
			SetOrg(baseedge, lowerright);
			lowerleft = upperleft;
			SymmTri(baseedge, leftcand);
			upperleft = apex(leftcand);
		}
		if (m_verbose > 2) {
			printf("  Connecting ");
			printtriangle(&baseedge);
		}
	}
}

/*****************************************************************************/
/*                                                                           */
/*  divconqrecurse()   Recursively form a Delaunay triangulation by the      */
/*                     divide-and-conquer method.                            */
/*                                                                           */
/*  Recursively breaks down the problem into smaller pieces, which are       */
/*  knitted together by mergehulls().  The base cases (problems of two or    */
/*  three m_points) are handled specially here.                                */
/*                                                                           */
/*  On completion, `farleft' and `farright' are bounding m_triangles such that */
/*  the origin of `farleft' is the leftmost vertex (breaking ties by         */
/*  choosing the highest leftmost vertex), and the destination of            */
/*  `farright' is the rightmost vertex (breaking ties by choosing the        */
/*  lowest rightmost vertex).                                                */
/*                                                                           */
/*****************************************************************************/

void CTINClass::divconqrecurse(triPOINT **sortarray,int vertices,int axis,triEDGE*farleft,triEDGE*farright)
{
	triEDGE midtri, tri1, tri2, tri3;
	triEDGE innerleft, innerright;
	REAL area;
	int divider;
	
	if (m_verbose > 2) {
		printf("  Triangulating %d m_points.\n", vertices);
	}
	if (vertices == 2) {
		/* The triangulation of two vertices is an EDGE.  An EDGE is */
		/*   represented by two bounding m_triangles.                  */
		// 左边，从0－>1的边， 顶点空
		maketriangle(farleft);
		SetOrg(*farleft, sortarray[0]);
		SetDest(*farleft, sortarray[1]);
		/* The apex is intentionally left NULL.  apex == NULL */
		// 右边，从1－>0的边，顶点空
		maketriangle(farright);
		SetOrg(*farright, sortarray[1]);
		SetDest(*farright, sortarray[0]);
		/* The apex is intentionally left NULL.  apex == NULL */
		// 绑定两条边
		bond(*farleft, *farright);
		// 前一条边
		PrevEdgeSelf(*farleft);
		// 下一条边
		NextEdgeSelf(*farright);
		// 绑定两条边
		bond(*farleft, *farright);

		PrevEdgeSelf(*farleft);
		NextEdgeSelf(*farright);
		bond(*farleft, *farright);

		if (m_verbose > 2) {
			printf("  Creating ");
			printtriangle(farleft);
			printf("  Creating ");
			printtriangle(farright);
		}
		/* Ensure that the origin of `farleft' is sortarray[0]. */


		PrevEdge(*farright, *farleft);
		return;
	} else if (vertices == 3) {
		/* The triangulation of three vertices is either a TRIANGLE (with */
		/*   three bounding m_triangles) or two edges (with four bounding   */
		/*   m_triangles).  In either case, four m_triangles are created.     */
		maketriangle(&midtri);
		maketriangle(&tri1);
		maketriangle(&tri2);
		maketriangle(&tri3);
		area = counterclockwise(sortarray[0], sortarray[1], sortarray[2]);
		if (area == 0.0) {
			/* Three collinear m_points; the triangulation is two edges. */
			SetOrg(midtri, sortarray[0]);
			SetDest(midtri, sortarray[1]);
			SetOrg(tri1, sortarray[1]);
			SetDest(tri1, sortarray[0]);
			SetOrg(tri2, sortarray[2]);
			SetDest(tri2, sortarray[1]);
			SetOrg(tri3, sortarray[1]);
			SetDest(tri3, sortarray[2]);
			/* All apices are intentionally left NULL. */
			bond(midtri, tri1);
			bond(tri2, tri3);
			NextEdgeSelf(midtri);
			PrevEdgeSelf(tri1);
			NextEdgeSelf(tri2);
			PrevEdgeSelf(tri3);
			bond(midtri, tri3);
			bond(tri1, tri2);
			NextEdgeSelf(midtri);
			PrevEdgeSelf(tri1);
			NextEdgeSelf(tri2);
			PrevEdgeSelf(tri3);
			bond(midtri, tri1);
			bond(tri2, tri3);
			/* Ensure that the origin of `farleft' is sortarray[0]. */
			TriEdgeCopy(tri1, *farleft);
			/* Ensure that the destination of `farright' is sortarray[2]. */
			TriEdgeCopy(tri2, *farright);
		} else {
			/* The three m_points are not collinear; the triangulation is one */
			/*   TRIANGLE, namely `midtri'.                                 */
			SetOrg(midtri, sortarray[0]);
			SetDest(tri1, sortarray[0]);
			SetOrg(tri3, sortarray[0]);
			/* Apices of tri1, tri2, and tri3 are left NULL. */
			if (area > 0.0) {
				/* The vertices are in counterclockwise m_order. */
				SetDest(midtri, sortarray[1]);
				SetOrg(tri1, sortarray[1]);
				SetDest(tri2, sortarray[1]);
				SetApex(midtri, sortarray[2]);
				SetOrg(tri2, sortarray[2]);
				SetDest(tri3, sortarray[2]);
			} else {
				/* The vertices are in clockwise m_order. */
				SetDest(midtri, sortarray[2]);
				SetOrg(tri1, sortarray[2]);
				SetDest(tri2, sortarray[2]);
				SetApex(midtri, sortarray[1]);
				SetOrg(tri2, sortarray[1]);
				SetDest(tri3, sortarray[1]);
			}
			/* The topology does not depend on how the vertices are ordered. */
			bond(midtri, tri1);
			NextEdgeSelf(midtri);
			bond(midtri, tri2);
			NextEdgeSelf(midtri);
			bond(midtri, tri3);
			PrevEdgeSelf(tri1);
			NextEdgeSelf(tri2);
			bond(tri1, tri2);
			PrevEdgeSelf(tri1);
			PrevEdgeSelf(tri3);
			bond(tri1, tri3);
			NextEdgeSelf(tri2);
			PrevEdgeSelf(tri3);
			bond(tri2, tri3);
			/* Ensure that the origin of `farleft' is sortarray[0]. */
			// sortarray 按x从小到大排列
			TriEdgeCopy(tri1, *farleft);
			/* Ensure that the destination of `farright' is sortarray[2]. */
			if (area > 0.0) {
				TriEdgeCopy(tri2, *farright);
			} else {
				NextEdge(*farleft, *farright);
			}
		}
		if (m_verbose > 2) {
			printf("  Creating ");
			printtriangle(&midtri);
			printf("  Creating ");
			printtriangle(&tri1);
			printf("  Creating ");
			printtriangle(&tri2);
			printf("  Creating ");
			printtriangle(&tri3);
		}
		return;
	} else {
		/* Split the vertices in half. */
		divider = vertices >> 1;
		/* Recursively triangulate each half. */
		divconqrecurse(sortarray, divider, 1 - axis, farleft, &innerleft);
		divconqrecurse(&sortarray[divider], vertices - divider, 1 - axis,
			&innerright, farright);
		if (m_verbose > 1) {
			printf("  Joining triangulations with %d and %d vertices.\n", divider,
				vertices - divider);
		}
		/* Merge the two triangulations into one. */
		mergehulls(farleft, &innerleft, &innerright, farright, axis);
	}
}


long CTINClass::removeghosts(triEDGE*startghost)
{
	triEDGE searchedge;
	triEDGE dissolveedge;
	triEDGE deadtri;
	triPOINT *markorg;
	long hullsize;
	
	if (m_verbose) {
		printf("  Removing ghost m_triangles.\n");
	}
	/* Find an EDGE on the m_convex hull to start triPOINT location from. */
	PrevEdge(*startghost, searchedge);
	SymmTriSelf(searchedge);
	m_dummytri->adjoin[0] = encode(searchedge);
	/* Remove the bounding box and count the m_convex hull edges. */
	TriEdgeCopy(*startghost, dissolveedge);
	hullsize = 0;
	do {
		hullsize++;
		NextEdge(dissolveedge, deadtri);
		PrevEdgeSelf(dissolveedge);
		SymmTriSelf(dissolveedge);
		/* If no PSLG is involved, set the boundary markers of all the m_points */
		/*   on the m_convex hull.  If a PSLG is used, this step is done later. */
		if (!m_poly) {
			/* Watch out for the case where all the input m_points are collinear. */
			if (dissolveedge.tri != m_dummytri) {
				markorg = org(dissolveedge);
				if (PointMark(markorg) == 0) {
					SetPointMark(markorg, 1);
				}
			}
		}
		/* Remove a bounding TRIANGLE from a m_convex hull TRIANGLE. */
		dissolve(dissolveedge);
		/* Find the next bounding TRIANGLE. */
		SymmTri(deadtri, dissolveedge);
		/* Delete the bounding TRIANGLE. */
		triangleDealloc(deadtri.tri);
	} while (!TriEdgeEqual(dissolveedge, *startghost));
	
	return hullsize;
}

/*****************************************************************************/
/*                                                                           */
/*  divconqdelaunay()   Form a Delaunay triangulation by the divide-and-     */
/*                      conquer method.                                      */
/*                                                                           */
/*  Sorts the m_points, calls a recursive procedure to triangulate them, and   */
/*  removes the bounding box, setting boundary markers as appropriate.       */
/*                                                                           */
/*****************************************************************************/

long CTINClass::divconqdelaunay()
{
	triPOINT **sortarray;
	triEDGE hullleft, hullright;
	int divider;
	int i, j;
	
	/* Allocate an array of pointers to m_points for sorting. */
	sortarray = (triPOINT **) malloc( m_points.GetNumberOfItems() * sizeof(triPOINT *));
	if (sortarray == (triPOINT **) NULL) {
		printf("Error:  Out of memory.\n");
		exit(1);
	}
	m_points.TraversalInit();
	for (i = 0; i < m_points.GetNumberOfItems(); i++) 
		sortarray[i] = PointTraverse();
	
	if (m_verbose) 
		printf("  Sorting m_points.\n");
	
	/* Sort the m_points. */
	pointsort(sortarray, m_points.GetNumberOfItems() );
	/* Discard duplicate m_points, which can really mess up the algorithm. */
	i = 0;
	for (j = 1; j < m_points.GetNumberOfItems(); j++) {
		if ((sortarray[i]->x == sortarray[j]->x)
			&& (sortarray[i]->y == sortarray[j]->y)) {
			
			if (!quiet) {
				printf("Warning:  A duplicate triPOINT at (%.12g, %.12g) appeared and was ignored.\n",
					sortarray[j]->x, sortarray[j]->y);
			}
			/*  Commented out - would eliminate triPOINT from output .node file, but causes
			a failure if some segment has this triPOINT as an endpoint.
			SetPointMark(sortarray[j], DEADPOINT);
			*/
			/* set its "dup" to the duplcate triPOINT , by jws */
			SetPointDup ( sortarray[j], sortarray[i] );
		} 
		else {
			i++;
			sortarray[i] = sortarray[j];
		}
	}
	i++;
	if (dwyer) {
		/* Re-sort the array of m_points to accommodate alternating cuts. */
		divider = i >> 1;
		if (i - divider >= 2) {
			if (divider >= 2) {
				alternateaxes(sortarray, divider, 1);
			}
			alternateaxes(&sortarray[divider], i - divider, 1);
		}
	}
	if (m_verbose) {
		printf("  Forming triangulation.\n");
	}
	/* Form the Delaunay triangulation. */
	divconqrecurse(sortarray, i, 0, &hullleft, &hullright);
	
	free(sortarray);
	
	return removeghosts(&hullleft);
}

/**                                                                         **/
/**                                                                         **/
/********* Divide-and-conquer Delaunay triangulation ends here       *********/

/********* Incremental Delaunay triangulation begins here            *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  boundingbox()   Form an "infinite" bounding TRIANGLE to insert m_points    */
/*                  into.                                                    */
/*                                                                           */
/*  The m_points at "infinity" are assigned finite coordinates, which are used */
/*  by the triPOINT location routines, but (mostly) ignored by the Delaunay     */
/*  EDGE flip routines.                                                      */
/*                                                                           */
/*****************************************************************************/

#ifndef REDUCED

void CTINClass::boundingbox()
{
	triEDGE inftri;          /* Handle for the triangular bounding box. */
	REAL width;
	
	if (m_verbose) {
		printf("  Creating triangular bounding box.\n");
	}
	/* Find the width (or height, whichever is larger) of the triangulation. */
	width = m_xmax - m_xmin;
	if (m_ymax - m_ymin > width) 
		width = m_ymax - m_ymin;
	
	if (width == 0.0) 
		width = 1.0;
	
	/* Create the vertices of the bounding box. */
	m_infpoint1 = new triPOINT[1];
	m_infpoint2 = new triPOINT[1];
	m_infpoint3 = new triPOINT[1];
	
	if ((m_infpoint1 == NULL) || (m_infpoint2 ==  NULL)	|| (m_infpoint3 == NULL)) {
		printf("Error:  Out of memory.\n");
		exit(1);
	}
	
	m_infpoint1->x = m_xmin - 50.0 * width;
	m_infpoint1->y = m_ymin - 40.0 * width;
	m_infpoint1->marker = -9999;
	m_infpoint2->x = m_xmax + 50.0 * width;
	m_infpoint2->y = m_ymin - 40.0 * width;
	m_infpoint2->marker = -9999;
	m_infpoint3->x = 0.5 * (m_xmin + m_xmax);
	m_infpoint3->y = m_ymax + 60.0 * width;
	m_infpoint3->marker = -9999;
	
	/* Create the bounding box. */
	maketriangle(&inftri);
	SetOrg(inftri, m_infpoint1);
	SetDest(inftri, m_infpoint2);
	SetApex(inftri, m_infpoint3);
	
	/* Link m_dummytri to the bounding box so we can always find an */
	/*   EDGE to begin searching (triPOINT location) from.           */
	m_dummytri->adjoin[0] = inftri.tri;
	if (m_verbose > 2) {
		printf("  Creating ");
		printtriangle(&inftri);
	}
}

#endif /* not REDUCED */

/*****************************************************************************/
/*                                                                           */
/*  removebox()   Remove the "infinite" bounding TRIANGLE, setting boundary  */
/*                markers as appropriate.                                    */
/*                                                                           */
/*  The triangular bounding box has three boundary m_triangles (one for each   */
/*  side of the bounding box), and a bunch of m_triangles fanning out from     */
/*  the three bounding box vertices (one TRIANGLE for each EDGE of the       */
/*  m_convex hull of the inner mesh).  This routine removes these m_triangles.   */
/*                                                                           */
/*****************************************************************************/

#ifndef REDUCED

long CTINClass::removebox()
{
	triEDGE deadtri;
	triEDGE searchedge;
	triEDGE checkedge;
	triEDGE nextedge, finaledge, dissolveedge;
	triPOINT *markorg;
	long hullsize;
	
	if (m_verbose) {
		printf("  Removing triangular bounding box.\n");
	}
	/* Find a boundary TRIANGLE. */
	nextedge.tri = m_dummytri;
	nextedge.orient = 0;
	SymmTriSelf(nextedge);
	/* Mark a place to stop. */
	PrevEdge(nextedge, finaledge);
	NextEdgeSelf(nextedge);
	SymmTriSelf(nextedge);
	/* Find a TRIANGLE (on the boundary of the triPOINT set) that isn't */
	/*   a bounding box TRIANGLE.                                    */
	PrevEdge(nextedge, searchedge);
	SymmTriSelf(searchedge);
	/* Check whether nextedge is another boundary TRIANGLE */
	/*   adjacent to the first one.                        */
	NextEdge(nextedge, checkedge);
	SymmTriSelf(checkedge);
	if (checkedge.tri == m_dummytri) {
		/* Go on to the next TRIANGLE.  There are only three boundary   */
		/*   m_triangles, and this next TRIANGLE cannot be the third one, */
		/*   so it's safe to stop here.                                 */
		PrevEdgeSelf(searchedge);
		SymmTriSelf(searchedge);
	}
	/* Find a new boundary EDGE to search from, as the current search */
	/*   EDGE lies on a bounding box TRIANGLE and will be deleted.    */
	m_dummytri->adjoin[0] = encode(searchedge);
	hullsize = -2l;
	while (!TriEdgeEqual(nextedge, finaledge)) {
		hullsize++;
		PrevEdge(nextedge, dissolveedge);
		SymmTriSelf(dissolveedge);
		/* If not using a PSLG, the vertices should be marked now. */
		/*   (If using a PSLG, markhull() will do the job.)        */
		if (!m_poly) {
			/* Be careful!  One must check for the case where all the input   */
			/*   m_points are collinear, and thus all the m_triangles are part of */
			/*   the bounding box.  Otherwise, the SetPointMark() call below  */
			/*   will cause a bad pointer reference.                          */
			if (dissolveedge.tri != m_dummytri) {
				markorg = org(dissolveedge);
				if (PointMark(markorg) == 0) {
					SetPointMark(markorg, 1);
				}
			}
		}
		/* Disconnect the bounding box TRIANGLE from the mesh TRIANGLE. */
		dissolve(dissolveedge);
		NextEdge(nextedge, deadtri);
		SymmTri(deadtri, nextedge);
		/* Get rid of the bounding box TRIANGLE. */
		triangleDealloc(deadtri.tri);
		/* Do we need to turn the corner? */
		if (nextedge.tri == m_dummytri) {
			/* Turn the corner. */
			TriEdgeCopy(dissolveedge, nextedge);
		}
	}
	triangleDealloc(finaledge.tri);
	
	delete m_infpoint1;                  ///Deallocate the bounding box vertices. *
	delete m_infpoint2;
	delete m_infpoint3;
	
	return hullsize;
}

#endif /* not REDUCED */

/*****************************************************************************/
/*                                                                           */
/*  incrementaldelaunay()   Form a Delaunay triangulation by incrementally   */
/*                          adding vertices.                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef REDUCED

long CTINClass::incrementaldelaunay()
{
	triEDGE starttri;
	triPOINT *pointloop;
	int i;
	
	/* Create a triangular bounding box. */
	boundingbox();
	if (m_verbose) 
		printf("  Incrementally inserting m_points.\n");
	
	m_points.TraversalInit();
	pointloop = PointTraverse();
	
	i = 1;
	while (pointloop != (triPOINT *) NULL) {
		/* Find a boundary TRIANGLE to search from. */
		starttri.tri = (TRIANGLE *) NULL;
		if( InsertSite(pointloop, &starttri, (EDGE *) NULL, 0, 0) == DUPLICATEPOINT) {
		/*			if (!quiet) 
		printf("Warning:  A duplicate triPOINT at (%.12g, %.12g) appeared and was ignored.\n",
		pointloop->x, pointloop->y);
			*/
			/*  Commented out - would eliminate triPOINT from output .node file. 
			SetPointMark(pointloop, DEADPOINT);				*/
		}
		pointloop = PointTraverse();
		i++;
	}
	/* Remove the bounding box. */
	return removebox();
}

#endif /* not REDUCED */

/**                                                                         **/
/**                                                                         **/
/********* Incremental Delaunay triangulation ends here              *********/

/********* Sweepline Delaunay triangulation begins here              *********/
/**                                                                         **/
/**                                                                         **/

#ifndef REDUCED

void CTINClass::eventHeapInsert(sweepEVENT **heap,int heapsize,sweepEVENT *newEvent)
{
	REAL eventx, eventy;
	int eventnum;
	int parent;
	
	eventx = newEvent->xkey;
	eventy = newEvent->ykey;
	eventnum = heapsize;

	while ( eventnum > 0 ) {
		parent = (eventnum - 1) >> 1;
		if ( (heap[parent]->ykey < eventy ) ||
			((heap[parent]->ykey == eventy ) && ( heap[parent]->xkey <= eventx) ) ) {
			break;
		} else {
			heap[eventnum] = heap[parent];
			heap[eventnum]->heapposition = eventnum;
			
			eventnum = parent;
		}
	}

	heap[eventnum] = newEvent;
	newEvent->heapposition = eventnum;
}

#endif /* not REDUCED */

#ifndef REDUCED

void CTINClass::eventHeapify(sweepEVENT **heap,int heapsize,int eventnum)
{
	sweepEVENT *thisevent;
	REAL eventx, eventy;
	int leftchild, rightchild;
	int smallest;
	int notdone;
	
	thisevent = heap[eventnum];
	eventx = thisevent->xkey;
	eventy = thisevent->ykey;
	leftchild = 2 * eventnum + 1;
	notdone = leftchild < heapsize;
	while (notdone) {
		if ((heap[leftchild]->ykey < eventy) ||
			((heap[leftchild]->ykey == eventy)
			&& (heap[leftchild]->xkey < eventx))) {
			smallest = leftchild;
		} else {
			smallest = eventnum;
		}
		rightchild = leftchild + 1;
		if (rightchild < heapsize) {
			if ((heap[rightchild]->ykey < heap[smallest]->ykey) ||
				((heap[rightchild]->ykey == heap[smallest]->ykey)
				&& (heap[rightchild]->xkey < heap[smallest]->xkey))) {
				smallest = rightchild;
			}
		}
		if (smallest == eventnum) {
			notdone = 0;
		} else {
			heap[eventnum] = heap[smallest];
			heap[eventnum]->heapposition = eventnum;
			heap[smallest] = thisevent;
			thisevent->heapposition = smallest;
			
			eventnum = smallest;
			leftchild = 2 * eventnum + 1;
			notdone = leftchild < heapsize;
		}
	}
}

#endif /* not REDUCED */

#ifndef REDUCED

void CTINClass::eventheapdelete(sweepEVENT **heap,int heapsize,int eventnum)
{
	sweepEVENT *moveevent;
	REAL eventx, eventy;
	int parent;
	int notdone;
	
	moveevent = heap[heapsize - 1];
	if (eventnum > 0) {
		eventx = moveevent->xkey;
		eventy = moveevent->ykey;
		do {
			parent = (eventnum - 1) >> 1;
			if ((heap[parent]->ykey < eventy) ||
				((heap[parent]->ykey == eventy)
				&& (heap[parent]->xkey <= eventx))) {
				notdone = 0;
			} else {
				heap[eventnum] = heap[parent];
				heap[eventnum]->heapposition = eventnum;
				
				eventnum = parent;
				notdone = eventnum > 0;
			}
		} while (notdone);
	}
	heap[eventnum] = moveevent;
	moveevent->heapposition = eventnum;
	eventHeapify(heap, heapsize - 1, eventnum);
}

#endif /* not REDUCED */

#ifndef REDUCED

void CTINClass::CreateEventHeap(sweepEVENT ***eventheap,sweepEVENT **events,sweepEVENT **freeEvents)
{
	triPOINT *thisPoint;
	int maxEvents;
	int i;
	
	maxEvents = (3 * m_points.GetNumberOfItems()) / 2;
	*eventheap = (sweepEVENT **) malloc(maxEvents * sizeof(sweepEVENT *) );
	if (*eventheap == (sweepEVENT **) NULL) {
		printf("Error:  Out of memory.\n");
		exit(1);
	}
	*events = (sweepEVENT *) malloc(maxEvents * sizeof(sweepEVENT));
	if (*events == (sweepEVENT *) NULL) {
		printf("Error:  Out of memory.\n");
		exit(1);
	}
	m_points.TraversalInit();
	for (i = 0; i < m_points.GetNumberOfItems(); i++) 
	{
		thisPoint = PointTraverse();
		(*events)->eventPtr->pt = thisPoint;
		(*events)->xkey = thisPoint->x;
		(*events)->ykey = thisPoint->y;
		eventHeapInsert(*eventheap, i, *events + i);
	}
	*freeEvents = (sweepEVENT *) NULL;
	for (i = maxEvents - 1; i >= m_points.GetNumberOfItems(); i--) {
		(*events)->eventPtr->event = *freeEvents;
		*freeEvents = *events + i;
	}
}

#endif /* not REDUCED */

#ifndef REDUCED

int CTINClass::rightofhyperbola(triEDGE*fronttri,triPOINT *newsite)
{
	triPOINT *leftpoint, *rightpoint;
	REAL dxa, dya, dxb, dyb;
	
	hyperbolacount++;
	
	leftpoint = dest(*fronttri);
	rightpoint = apex(*fronttri);
	if ((leftpoint->y < rightpoint->y)
		|| ((leftpoint->y == rightpoint->y) && (leftpoint->x < rightpoint->x))) {
		if (newsite->x >= rightpoint->x) {
			return 1;
		}
	} else {
		if (newsite->x <= leftpoint->x) {
			return 0;
		}
	}
	dxa = leftpoint->x - newsite->x;
	dya = leftpoint->y - newsite->y;
	dxb = rightpoint->x - newsite->x;
	dyb = rightpoint->y - newsite->y;
	return dya * (dxb * dxb + dyb * dyb) > dyb * (dxa * dxa + dya * dya);
}

#endif /* not REDUCED */

#ifndef REDUCED

REAL CTINClass::circletop(triPOINT *pa,triPOINT *pb,triPOINT *pc,REAL ccwabc)
{
	REAL xac, yac, xbc, ybc, xab, yab;
	REAL aclen2, bclen2, ablen2;
	
	circletopcount++;
	
	xac = pa->x - pc->x;
	yac = pa->y - pc->y;
	xbc = pb->x - pc->x;
	ybc = pb->y - pc->y;
	xab = pa->x - pb->x;
	yab = pa->y - pb->y;
	aclen2 = xac * xac + yac * yac;
	bclen2 = xbc * xbc + ybc * ybc;
	ablen2 = xab * xab + yab * yab;
	return pc->y + (xac * bclen2 - xbc * aclen2 + sqrt(aclen2 * bclen2 * ablen2))
		/ (2.0 * ccwabc);
}

#endif /* not REDUCED */

#ifndef REDUCED

void CTINClass::Check4DeadEvent(triEDGE*checktri,sweepEVENT **freeEvents,sweepEVENT **eventheap,long *heapsize)
{
	sweepEVENT *deadevent;
	triPOINT *eventpoint;
	int eventnum;
	
	eventpoint = org(*checktri);
	if (eventpoint != (triPOINT *) NULL) {
		deadevent = (sweepEVENT *) eventpoint;
		eventnum = deadevent->heapposition;
		deadevent->eventPtr->event = *freeEvents;
		*freeEvents = deadevent;
		eventheapdelete(eventheap, *heapsize, eventnum);
		(*heapsize)--;
		SetOrg(*checktri, NULL);
	}
}

#endif /* not REDUCED */

#ifndef REDUCED

splayNODE *CTINClass::splay(splayNODE *splaytree,triPOINT *searchpoint,triEDGE*searchtri)
{
	splayNODE *child, *grandchild;
	splayNODE *lefttree, *righttree;
	splayNODE *leftright;
	triPOINT *checkpoint;
	int rightofroot, rightofchild;
	
	if (splaytree == (splayNODE *) NULL) {
		return (splayNODE *) NULL;
	}
	checkpoint = dest(splaytree->keyedge);
	if ( checkpoint == splaytree->keydest ) {
		rightofroot = rightofhyperbola(&splaytree->keyedge, searchpoint);
		if (rightofroot) {
			TriEdgeCopy(splaytree->keyedge, *searchtri);
			child = splaytree->rchild;
		} else {
			child = splaytree->lchild;
		}
		if (child == (splayNODE *) NULL) {
			return splaytree;
		}
		checkpoint = dest(child->keyedge);
		if (checkpoint != child->keydest) {
			child = splay(child, searchpoint, searchtri);
			if (child == (splayNODE *) NULL) {
				if (rightofroot) {
					splaytree->rchild = (splayNODE *) NULL;
				} else {
					splaytree->lchild = (splayNODE *) NULL;
				}
				return splaytree;
			}
		}
		rightofchild = rightofhyperbola(&child->keyedge, searchpoint);
		if (rightofchild) {
			TriEdgeCopy(child->keyedge, *searchtri);
			grandchild = splay(child->rchild, searchpoint, searchtri);
			child->rchild = grandchild;
		} else {
			grandchild = splay(child->lchild, searchpoint, searchtri);
			child->lchild = grandchild;
		}
		if (grandchild == (splayNODE *) NULL) {
			if (rightofroot) {
				splaytree->rchild = child->lchild;
				child->lchild = splaytree;
			} else {
				splaytree->lchild = child->rchild;
				child->rchild = splaytree;
			}
			return child;
		}
		if (rightofchild) {
			if (rightofroot) {
				splaytree->rchild = child->lchild;
				child->lchild = splaytree;
			} else {
				splaytree->lchild = grandchild->rchild;
				grandchild->rchild = splaytree;
			}
			child->rchild = grandchild->lchild;
			grandchild->lchild = child;
		} else {
			if (rightofroot) {
				splaytree->rchild = grandchild->lchild;
				grandchild->lchild = splaytree;
			} else {
				splaytree->lchild = child->rchild;
				child->rchild = splaytree;
			}
			child->lchild = grandchild->rchild;
			grandchild->rchild = child;
		}
		return grandchild;
	} else {
		lefttree = splay(splaytree->lchild, searchpoint, searchtri);
		righttree = splay(splaytree->rchild, searchpoint, searchtri);
		
		m_splayNodes.PoolDealloc( splaytree);
		if (lefttree == (splayNODE *) NULL) {
			return righttree;
		} else if (righttree == (splayNODE *) NULL) {
			return lefttree;
		} else if (lefttree->rchild == (splayNODE *) NULL) {
			lefttree->rchild = righttree->lchild;
			righttree->lchild = lefttree;
			return righttree;
		} else if (righttree->lchild == (splayNODE *) NULL) {
			righttree->lchild = lefttree->rchild;
			lefttree->rchild = righttree;
			return lefttree;
		} else {
			/*      printf("Holy Toledo!!!\n"); */
			leftright = lefttree->rchild;
			while (leftright->rchild != (splayNODE *) NULL) {
				leftright = leftright->rchild;
			}
			leftright->rchild = righttree;
			return lefttree;
		}
	}
}

#endif /* not REDUCED */

#ifndef REDUCED

splayNODE *CTINClass::splayinsert(splayNODE *splayroot,triEDGE*newkey,triPOINT *searchpoint)
{
	splayNODE *newsplaynode;
	
	newsplaynode = (splayNODE *) m_splayNodes.PoolAlloc();
	TriEdgeCopy(*newkey, newsplaynode->keyedge);
	newsplaynode->keydest = dest(*newkey);
	if (splayroot == (splayNODE *) NULL) {
		newsplaynode->lchild = (splayNODE *) NULL;
		newsplaynode->rchild = (splayNODE *) NULL;
	} else if (rightofhyperbola(&splayroot->keyedge, searchpoint)) {
		newsplaynode->lchild = splayroot;
		newsplaynode->rchild = splayroot->rchild;
		splayroot->rchild = (splayNODE *) NULL;
	} else {
		newsplaynode->lchild = splayroot->lchild;
		newsplaynode->rchild = splayroot;
		splayroot->lchild = (splayNODE *) NULL;
	}
	return newsplaynode;
}

#endif /* not REDUCED */

#ifndef REDUCED

splayNODE *CTINClass::circletopinsert(splayNODE *splayroot,triEDGE*newkey,
										  triPOINT *pa,triPOINT *pb,triPOINT *pc,REAL topy)
{
	REAL ccwabc;
	REAL xac, yac, xbc, ybc;
	REAL aclen2, bclen2;
	triPOINT searchpoint[1];
	triEDGE m_dummytri;
	
	ccwabc = counterclockwise(pa, pb, pc);
	xac = pa->x - pc->x;
	yac = pa->y - pc->y;
	xbc = pb->x - pc->x;
	ybc = pb->y - pc->y;
	aclen2 = xac * xac + yac * yac;
	bclen2 = xbc * xbc + ybc * ybc;
	
	searchpoint->x = pc->x - (yac * bclen2 - ybc * aclen2) / (2.0 * ccwabc);
	searchpoint->y = topy;
	
	return splayinsert(splay(splayroot, (triPOINT *) searchpoint, &m_dummytri), newkey,
		searchpoint);
}

#endif /* not REDUCED */

#ifndef REDUCED

splayNODE *CTINClass::frontlocate(splayNODE *splayroot,triEDGE*bottommost,
									  triPOINT *searchpoint,triEDGE*searchtri,long *farright)
{
	int farrightflag;
	
	TriEdgeCopy(*bottommost, *searchtri);
	splayroot = splay(splayroot, searchpoint, searchtri);
	
	farrightflag = 0;
	while (!farrightflag && rightofhyperbola(searchtri, searchpoint)) {
		oNextSpinEdgeSelf(*searchtri);
		farrightflag = TriEdgeEqual(*searchtri, *bottommost);
	}
	*farright = farrightflag;
	return splayroot;
}

#endif /* not REDUCED */

#ifndef REDUCED

long CTINClass::SweepLineDelaunay()
{
	sweepEVENT **eventheap;
	sweepEVENT *events;
	sweepEVENT *freeEvents;
	sweepEVENT *nextEvent;
	sweepEVENT *newEvent;
	splayNODE *splayroot;
	triEDGE bottommost;
	triEDGE searchtri;
	triEDGE flipTri;
	triEDGE leftTri, rightTri, farLeftTri, farRightTri;
	triEDGE inserttri;
	triPOINT *firstpoint, *secondpoint;
	triPOINT *nextpoint, *lastpoint;
	triPOINT *connectpoint;
	triPOINT *leftpoint, *midpoint, *rightpoint;
	REAL lefttest, righttest;
	long heapsize;
	long check4Events, farrightflag;
	
	m_splayNodes.PoolInit( SPLAYNODEPERBLOCK );
	splayroot = (splayNODE *) NULL;
	
	if (m_verbose) {
		printf("  Placing m_points in event heap.\n");
	}
	CreateEventHeap(&eventheap, &events, &freeEvents);
	heapsize = m_points.GetNumberOfItems();
	
	if (m_verbose) {
		printf("  Forming triangulation.\n");
	}
	maketriangle(&leftTri);
	maketriangle(&rightTri);
	bond(leftTri, rightTri);
	NextEdgeSelf(leftTri);
	PrevEdgeSelf(rightTri);
	bond(leftTri, rightTri);
	NextEdgeSelf(leftTri);
	PrevEdgeSelf(rightTri);
	bond(leftTri, rightTri);

	firstpoint = eventheap[0]->eventPtr->pt;
	eventheap[0]->eventPtr->event = freeEvents;
	freeEvents = eventheap[0];
	eventheapdelete(eventheap, heapsize, 0);
	heapsize--;
	do {
		if (heapsize == 0) {
			printf("Error:  Input m_points are all identical.\n");
			exit(1);
		}

		secondpoint = (triPOINT *) eventheap[0]->eventPtr->pt;
		eventheap[0]->eventPtr->event = freeEvents;
		freeEvents = eventheap[0];
		eventheapdelete(eventheap, heapsize, 0);
		heapsize--;
		if ((firstpoint->x == secondpoint->x)
			&& (firstpoint->y == secondpoint->y)) {
			printf("Warning:  A duplicate triPOINT at (%.12g, %.12g) appeared and was ignored.\n",
				secondpoint->x, secondpoint->y);
				/*  Commented out - would eliminate triPOINT from output .node file.
				SetPointMark(secondpoint, DEADPOINT);
			*/
		}
	} while ((firstpoint->x == secondpoint->x)
		&& (firstpoint->y == secondpoint->y));
	SetOrg(leftTri, firstpoint);
	SetDest(leftTri, secondpoint);
	SetOrg(rightTri, secondpoint);
	SetDest(rightTri, firstpoint);
	PrevEdge(leftTri, bottommost);
	lastpoint = secondpoint;
	while (heapsize > 0) {
		nextEvent = eventheap[0];
		eventheapdelete(eventheap, heapsize, 0);
		heapsize--;
		check4Events = 1;
		if (nextEvent->xkey < m_xmin) {
			decode( nextEvent->eventPtr->tri,  flipTri);
			oPrevSpinEdge(flipTri, farLeftTri);
			Check4DeadEvent(&farLeftTri, &freeEvents, eventheap, &heapsize);
			oNextSpinEdge(flipTri, farRightTri);
			Check4DeadEvent(&farRightTri, &freeEvents, eventheap, &heapsize);
			
			if (TriEdgeEqual(farLeftTri, bottommost)) {
				PrevEdge(flipTri, bottommost);
			}
			flip(&flipTri);
			SetApex(flipTri, NULL);
			PrevEdge(flipTri, leftTri);
			NextEdge(flipTri, rightTri);
			SymmTri(leftTri, farLeftTri);
			
			if (randomnation(SAMPLERATE) == 0) {
				SymmTriSelf(flipTri);
				leftpoint = dest(flipTri);
				midpoint = apex(flipTri);
				rightpoint = org(flipTri);
				splayroot = circletopinsert(splayroot, &leftTri, leftpoint, midpoint,
					rightpoint, nextEvent->ykey);
			}
		} else {
			nextpoint = nextEvent->eventPtr->pt;
			if ((nextpoint->x == lastpoint->x) && (nextpoint->y == lastpoint->y)) {
				printf("Warning:  A duplicate triPOINT at (%.12g, %.12g) appeared and was ignored.\n",
					nextpoint->x, nextpoint->y);
					/*  Commented out - would eliminate triPOINT from output .node file.
					SetPointMark(nextpoint, DEADPOINT);
				*/
				check4Events = 0;
			} else {
				lastpoint = nextpoint;
				
				splayroot = frontlocate(splayroot, &bottommost, nextpoint, &searchtri,
					&farrightflag);
					/*
					TriEdgeCopy(bottommost, searchtri);
					farrightflag = 0;
					while (!farrightflag && rightofhyperbola(&searchtri, nextpoint)) {
					oNextSpinEdgeSelf(searchtri);
					farrightflag = TriEdgeEqual(searchtri, bottommost);
					}
				*/
				
				Check4DeadEvent(&searchtri, &freeEvents, eventheap, &heapsize);
				
				TriEdgeCopy(searchtri, farRightTri);
				SymmTri(searchtri, farLeftTri);
				maketriangle(&leftTri);
				maketriangle(&rightTri);
				connectpoint = dest(farRightTri );
				SetOrg(leftTri, connectpoint);
				SetDest(leftTri, nextpoint);
				SetOrg(rightTri, nextpoint);
				SetDest(rightTri, connectpoint);
				bond(leftTri, rightTri);
				NextEdgeSelf(leftTri);
				PrevEdgeSelf(rightTri);
				bond(leftTri, rightTri);
				NextEdgeSelf(leftTri);
				PrevEdgeSelf(rightTri);
				bond(leftTri, farLeftTri);
				bond(rightTri, farRightTri);
				if (!farrightflag && TriEdgeEqual(farRightTri, bottommost)) {
					TriEdgeCopy(leftTri, bottommost);
				}
				
				if (randomnation(SAMPLERATE) == 0) {
					splayroot = splayinsert(splayroot, &leftTri, nextpoint);
				} else if (randomnation(SAMPLERATE) == 0) {
					NextEdge(rightTri, inserttri);
					splayroot = splayinsert(splayroot, &inserttri, nextpoint);
				}
			}
		}
		nextEvent->eventPtr->event = freeEvents;
		freeEvents = nextEvent;
		
		if (check4Events) {
			leftpoint = apex(farLeftTri );
			midpoint = dest(leftTri);
			rightpoint = apex(leftTri);
			lefttest = counterclockwise(leftpoint, midpoint, rightpoint);
			if (lefttest > 0.0) {
				newEvent = freeEvents;
				freeEvents = freeEvents->eventPtr->event;
				newEvent->xkey = m_xminextreme;
				newEvent->ykey = circletop(leftpoint, midpoint, rightpoint,
					lefttest);
				newEvent->eventPtr->tri =  encode(leftTri);
				eventHeapInsert(eventheap, heapsize, newEvent);
				heapsize++;
				SetOrg(leftTri, newEvent->eventPtr->pt );
			}
			leftpoint = apex(rightTri);
			midpoint = org(rightTri);
			rightpoint = apex(farRightTri);
			righttest = counterclockwise(leftpoint, midpoint, rightpoint);
			if (righttest > 0.0) {
				newEvent = freeEvents;
				freeEvents = (sweepEVENT *) freeEvents->eventPtr->event;
				newEvent->xkey = m_xminextreme;
				newEvent->ykey = circletop(leftpoint, midpoint, rightpoint,
					righttest);

				newEvent->eventPtr->tri = encode( farRightTri );
				eventHeapInsert(eventheap, heapsize, newEvent);
				heapsize++;

				SetOrg( farRightTri, newEvent->eventPtr->pt );
			}
		}
	}
	
	m_splayNodes.PoolDeinit();
	PrevEdgeSelf(bottommost);
	return removeghosts(&bottommost);
}

#endif /* not REDUCED */

/**                                                                         **/
/**                                                                         **/
/********* Sweepline Delaunay triangulation ends here                *********/

/********* General mesh construction routines begin here             *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  delaunay()   Form a Delaunay triangulation.                              */
/*                                                                           */
/*****************************************************************************/

long CTINClass::delaunay()
{
	eextras = 0;
	InitializeTriSegPools();
	
#ifdef REDUCED
	if (!quiet) 
		printf(	"Constructing Delaunay triangulation by divide-and-conquer method.\n");
	return divconqdelaunay();
	
#else /* not REDUCED */
	
	if( !quiet ) {
		printf("Constructing Delaunay triangulation ");
		
		if (incremental) 
			printf("by incremental method.\n");
		else if (sweepline) 
			printf("by sweepline method.\n");
		else 	printf("by divide-and-conquer method.\n");
	}
	
	if (incremental) 
		return incrementaldelaunay();
	else if (sweepline) 
		return SweepLineDelaunay();
	else	return divconqdelaunay();
	
#endif /* not REDUCED */
}

/*****************************************************************************/
/*                                                                           */
/*  reconstruct()   Reconstruct a triangulation from its .ele (and possibly  */
/*                  .poly) file.  Used when the -r switch is used.           */
/*                                                                           */
/*  Reads an .ele file and reconstructs the original mesh.  If the -p switch */
/*  is used, this procedure will also read a .poly file and reconstruct the  */
/*  shell edges of the original mesh.  If the -a switch is used, this        */
/*  procedure will also read an .area file and set a maximum area constralong */
/*  on each TRIANGLE.                                                        */
/*                                                                           */
/*  m_points that are not corners of m_triangles, such as nodes on edges of      */
/*  subparametric elements, are discarded.                                   */
/*                                                                           */
/*  This routine finds the adjacencies between m_triangles (and shell edges)   */
/*  by forming one stack of m_triangles for each vertex.  Each TRIANGLE is on  */
/*  three different stacks simultaneously.  Each TRIANGLE's shell EDGE       */
/*  pointers are used to link the items in each stack.  This memory-saving   */
/*  feature makes the code harder to read.  The most important thing to keep */
/*  in mind is that each TRIANGLE is removed from a stack precisely when     */
/*  the corresponding pointer is adjusted to refer to a shell EDGE rather    */
/*  than the next TRIANGLE of the stack.                                     */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

int CTINClass::reconstruct(long *triList,float *triangleattriblist,float *triAreaList,int elements,
						int corners,int attribs,long *segmentList,long *segMarkList,int numOfSegments)
{
	int pointindex;
	int attribindex;
	triEDGE triangleloop;
	triEDGE triangleleft;
	triEDGE checktri;
	triEDGE checkleft;
	triEDGE checkneighbor;
	EDGE shelleloop;
	TRIANGLE **vertexarray;
	TRIANGLE **prevlink;
	TRIANGLE *nexttri;
	triPOINT *tdest, *tapex;
	triPOINT *checkdest, *checkapex;
	triPOINT *shorg;
	triPOINT *killpoint;
	REAL area;
	int corner[3];
	int end[2];
	int killpointindex;
	int incorners;
	int segmentmarkers;
	int boundmarker;
	int aroundpoint;
	long hullsize;
	int notfound;
	int elementnumber, segmentnumber;
	int i, j;
	
	inelements = elements;
	incorners = corners;
	if (incorners < 3) {
		printf("Error:  m_triangles must have at least 3 m_points.\n");
		exit(1);
	}
	eextras = attribs;
	
	InitializeTriSegPools();
	
	/* Create the m_triangles. */
	for (elementnumber = 1; elementnumber <= inelements; elementnumber++) {
		maketriangle(&triangleloop);
		/* Mark the TRIANGLE as living. */
		triangleloop.tri->vertex[0] = (triPOINT *) triangleloop.tri;
	}
	
	if (m_poly) {
		insegments = numOfSegments;
		segmentmarkers = ( segMarkList !=  NULL );
		
		/* Create the shell edges. */
		for (segmentnumber = 1; segmentnumber <= insegments; segmentnumber++) {
			makeshelle(&shelleloop);
			/* Mark the shell EDGE as living. */
			shelleloop.sh->vertex[0] =(triPOINT *) shelleloop.sh;
		}
	}
	
	pointindex = 0;
	attribindex = 0;
	
	if (!quiet) {
		printf("Reconstructing mesh.\n");
	}
	/* Allocate a temporary array that maps each triPOINT to some adjacent  */
	/*   TRIANGLE.  I took care to allocate all the permanent memory for */
	/*   m_triangles and shell edges first.                                */
	vertexarray = (TRIANGLE **) malloc(m_points.GetNumberOfItems() * sizeof(TRIANGLE *));
	if (vertexarray == (TRIANGLE **) NULL) {
		printf("Error:  Out of memory.\n");
		exit(1);
	}
	/* Each triPOINT is initially unrepresented. */
	for (i = 0; i < m_points.GetNumberOfItems(); i++) {
		vertexarray[i] =  m_dummytri;
	}
	
	if (m_verbose) {
		printf("  Assembling m_triangles.\n");
	}
	/* Read the m_triangles from the .ele file, and link */
	/*   together those that share an EDGE.            */
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	elementnumber = m_firstnumber;
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		/* Copy the TRIANGLE's three corners. */
		for (j = 0; j < 3; j++) {
			corner[j] = triList[pointindex++];
			if ((corner[j] < m_firstnumber) || (corner[j] >= m_firstnumber + m_points.GetNumberOfItems() ) ) {
				printf("Error:  Triangle %d has an invalid vertex index.\n",
					elementnumber);
				exit(1);
			}
		}
		
		/* Find out about (and throw away) extra nodes. */
		for (j = 3; j < incorners; j++) {
			killpointindex = triList[pointindex++];
			if ((killpointindex >= m_firstnumber) &&
				(killpointindex < m_firstnumber + m_points.GetNumberOfItems() )) {
				/* Delete the non-corner triPOINT if it's not already deleted. */
				killpoint = GetPoint(killpointindex);
				if (PointMark(killpoint) != DEADPOINT) {
					PointDealloc(killpoint);
				}
			}
		}
		
		/* Read the TRIANGLE's attributes. */
		for (j = 0; j < eextras; j++) {
			setelemattribute(triangleloop, j, triangleattriblist[attribindex++]);
		}
		
		if (vararea) {
			area = triAreaList[elementnumber - m_firstnumber];
			setareabound(triangleloop, (float)area);
		}
		
		/* Set the TRIANGLE's vertices. */
		triangleloop.orient = 0;
		SetOrg(triangleloop, GetPoint(corner[0]));
		SetDest(triangleloop, GetPoint(corner[1]));
		SetApex(triangleloop, GetPoint(corner[2]));
		/* Try linking the TRIANGLE to others that share these vertices. */
		for (triangleloop.orient = 0; triangleloop.orient < 3;	triangleloop.orient++) {
			/* Take the number for the origin of triangleloop. */
			aroundpoint = corner[triangleloop.orient];
			/* Look for other m_triangles having this vertex. */
			nexttri = vertexarray[aroundpoint - m_firstnumber];
			/* Link the current TRIANGLE to the next one in the stack. */
			triangleloop.tri->sh[triangleloop.orient] = (SHELLE *)nexttri;
			/* Push the current TRIANGLE onto the stack. */
			vertexarray[aroundpoint - m_firstnumber] = encode(triangleloop);
			decode(nexttri, checktri);
			if (checktri.tri != m_dummytri) {
				tdest = dest(triangleloop);
				tapex = apex(triangleloop);
				/* Look for other m_triangles that share an EDGE. */
				do {
					checkdest = dest(checktri);
					checkapex = apex(checktri);
					if (tapex == checkdest) {
						/* The two m_triangles share an EDGE; bond them together. */
						PrevEdge(triangleloop, triangleleft);
						bond(triangleleft, checktri);
					}
					if (tdest == checkapex) {
						/* The two m_triangles share an EDGE; bond them together. */
						PrevEdge(checktri, checkleft);
						bond(triangleloop, checkleft);
					}
					/* Find the next TRIANGLE in the stack. */
					nexttri =  (TRIANGLE *)checktri.tri->sh[checktri.orient];
					decode(nexttri, checktri);
				} while (checktri.tri != m_dummytri);
			}
		}
		triangleloop.tri = TriangleTraverse();
		elementnumber++;
	}
	
	pointindex = 0;
	hullsize = 0;                      /* Prepare to count the boundary edges. */
	if (m_poly) {
		if (m_verbose) {
			printf("  Marking segments in triangulation.\n");
		}
		/* Read the segments from the .poly file, and link them */
		/*   to their neighboring m_triangles.                    */
		boundmarker = 0;
		m_shelles.TraversalInit();
		shelleloop.sh = shelleTraverse();
		segmentnumber = m_firstnumber;
		while (shelleloop.sh != (SHELLE *) NULL) {
			end[0] = segmentList[pointindex++];
			end[1] = segmentList[pointindex++];
			if (segmentmarkers) {
				boundmarker = segMarkList[segmentnumber - m_firstnumber];
			}
			for (j = 0; j < 2; j++) {
				if ((end[j] < m_firstnumber) || (end[j] >= m_firstnumber + m_points.GetNumberOfItems()  )) {
					printf("Error:  Segment %d has an invalid vertex index.\n", 
						segmentnumber);
					exit(1);
				}
			}
			
			/* set the shell EDGE's vertices. */
			shelleloop.shorient = 0;
			setshOrg(shelleloop, GetPoint(end[0]));
			setshDest(shelleloop, GetPoint(end[1]));
			SetShellMark(shelleloop, boundmarker);
			/* Try linking the shell EDGE to m_triangles that share these vertices. */
			for (shelleloop.shorient = 0; shelleloop.shorient < 2;
			shelleloop.shorient++) {
				/* Take the number for the destination of shelleloop. */
				aroundpoint = end[1 - shelleloop.shorient];
				/* Look for m_triangles having this vertex. */
				prevlink = &vertexarray[aroundpoint - m_firstnumber];
				nexttri = vertexarray[aroundpoint - m_firstnumber];
				decode(nexttri, checktri);
				shorg = shOrg(shelleloop);
				notfound = 1;
				/* Look for m_triangles having this EDGE.  Note that I'm only       */
				/*   comparing each TRIANGLE's destination with the shell EDGE;   */
				/*   each TRIANGLE's apex is handled through a different vertex.  */
				/*   Because each TRIANGLE appears on three vertices' lists, each */
				/*   occurrence of a TRIANGLE on a list can (and does) represent  */
				/*   an EDGE.  In this way, most edges are represented twice, and */
				/*   every TRIANGLE-segment bond is represented once.             */
				while (notfound && (checktri.tri != m_dummytri)) {
					checkdest = dest(checktri);
					if (shorg == checkdest) {
						/* We have a match.  Remove this TRIANGLE from the list. */
						*prevlink = (TRIANGLE *)checktri.tri->sh[checktri.orient];
						/* Bond the shell EDGE to the TRIANGLE. */
						tshBond(checktri, shelleloop);
						/* Check if this is a boundary EDGE. */
						SymmTri(checktri, checkneighbor);
						if (checkneighbor.tri == m_dummytri) {
							/* The next line doesn't insert a shell EDGE (because there's */
							/*   already one there), but it sets the boundary markers of  */
							/*   the existing shell EDGE and its vertices.                */
							insertshelle(&checktri, 1);
							hullsize++;
						}
						notfound = 0;
					}
					/* Find the next TRIANGLE in the stack. */
					prevlink = ( TRIANGLE **)(&checktri.tri->sh[checktri.orient]);
					nexttri = (TRIANGLE *)checktri.tri->sh[checktri.orient];
					decode(nexttri, checktri);
				}
			}
			shelleloop.sh = shelleTraverse();
			segmentnumber++;
		}
	}
	
	/* Mark the remaining edges as not being attached to any shell EDGE. */
	/* Also, count the (yet uncounted) boundary edges.                   */
	for (i = 0; i < m_points.GetNumberOfItems(); i++) {
		/* Search the stack of m_triangles adjacent to a triPOINT. */
		nexttri = vertexarray[i];
		decode(nexttri, checktri);
		while (checktri.tri != m_dummytri) {
			/* Find the next TRIANGLE in the stack before this */
			/*   information gets overwritten.                 */
			nexttri = (TRIANGLE *)checktri.tri->sh[checktri.orient];
			/* No adjacent shell EDGE.  (This overwrites the stack info.) */
			tshDissolve(checktri);
			SymmTri(checktri, checkneighbor);
			if (checkneighbor.tri == m_dummytri) {
				insertshelle(&checktri, 1);
				hullsize++;
			}
			decode(nexttri, checktri);
		}
	}
	
	free(vertexarray);
	return hullsize;
}

#endif /* not CDT_ONLY */

/**                                                                         **/
/**                                                                         **/
/********* General mesh construction routines end here               *********/

/********* Segment (shell EDGE) insertion begins here                *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  finddirection()   Find the first TRIANGLE on the path from one triPOINT     */
/*                    to another.                                            */
/*                                                                           */
/*  Finds the TRIANGLE that intersects a line segment drawn from the         */
/*  origin of `searchtri' to the triPOINT `endpoint', and returns the result    */
/*  in `searchtri'.  The origin of `searchtri' does not change, even though  */
/*  the TRIANGLE returned may differ from the one passed in.  This routine   */
/*  is used to find the direction to move in to get from one triPOINT to        */
/*  another.                                                                 */
/*                                                                           */
/*  The return value notes whether the destination or apex of the found      */
/*  TRIANGLE is collinear with the two m_points in question.                   */
/*                                                                           */
/*****************************************************************************/

enum finddirectionresult CTINClass::finddirection(triEDGE*searchtri,triPOINT *endpoint)
{
	triEDGE checktri;
	triPOINT *startpoint;
	triPOINT *leftpoint, *rightpoint;
	REAL leftccw, rightccw;
	int leftflag, rightflag;
	
	startpoint = org(*searchtri);
	rightpoint = dest(*searchtri);
	leftpoint = apex(*searchtri);
	/* Is `endpoint' to the left? */
	leftccw = counterclockwise(endpoint, startpoint, leftpoint);
	leftflag = leftccw > 0.0;
	/* Is `endpoint' to the right? */
	rightccw = counterclockwise(startpoint, endpoint, rightpoint);
	rightflag = rightccw > 0.0;
	if (leftflag && rightflag) {
		/* `searchtri' faces directly away from `endpoint'.  We could go */
		/*   left or right.  Ask whether it's a TRIANGLE or a boundary   */
		/*   on the left.                                                */
		oNextSpinEdge(*searchtri, checktri);
		if (checktri.tri == m_dummytri) {
			leftflag = 0;
		} else {
			rightflag = 0;
		}
	}
	while (leftflag) {
		/* Turn left until satisfied. */
		oNextSpinEdgeSelf(*searchtri);
		if (searchtri->tri == m_dummytri) {
			printf("Internal error in finddirection():  Unable to find a\n");
			printf("  TRIANGLE leading from (%.12g, %.12g) to", startpoint->x,
				startpoint->y);
			printf("  (%.12g, %.12g).\n", endpoint->x, endpoint->y);
			internalerror();
		}
		leftpoint = apex(*searchtri);
		rightccw = leftccw;
		leftccw = counterclockwise(endpoint, startpoint, leftpoint);
		leftflag = leftccw > 0.0;
	}
	while (rightflag) {
		/* Turn right until satisfied. */
		oPrevSpinEdgeSelf(*searchtri);
		if (searchtri->tri == m_dummytri) {
			printf("Internal error in finddirection():  Unable to find a\n");
			printf("  TRIANGLE leading from (%.12g, %.12g) to", startpoint->x,
				startpoint->y);
			printf("  (%.12g, %.12g).\n", endpoint->x, endpoint->y);
			internalerror();
		}
		rightpoint = dest(*searchtri);
		leftccw = rightccw;
		rightccw = counterclockwise(startpoint, endpoint, rightpoint);
		rightflag = rightccw > 0.0;
	}
	if (leftccw == 0.0) {
		return LEFTCOLLINEAR;
	} else if (rightccw == 0.0) {
		return RIGHTCOLLINEAR;
	} else {
		return WITHIN;
	}
}

/*****************************************************************************/
/*                                                                           */
/*  segmentintersection()   Find the intersection of an existing segment     */
/*                          and a segment that is being inserted.  Insert    */
/*                          a triPOINT at the intersection, splitting an        */
/*                          existing shell EDGE.                             */
/*                                                                           */
/*  The segment being inserted connects the apex of splittri to endpoint2.   */
/*  splitshelle is the shell EDGE being split, and MUST be opposite          */
/*  splittri.  Hence, the EDGE being split connects the origin and           */
/*  destination of splittri.                                                 */
/*                                                                           */
/*  On completion, splittri is a handle having the newly inserted            */
/*  intersection triPOINT as its origin, and endpoint1 as its destination.      */
/*                                                                           */
/*****************************************************************************/

void CTINClass::segmentintersection(triEDGE*splittri,EDGE * splitshelle,triPOINT *endpoint2)
{
	triPOINT *endpoint1;
	triPOINT *torg, *tdest;
	triPOINT *leftpoint, *rightpoint;
	triPOINT *newpoint;
	enum insertsiteresult success;
	enum finddirectionresult collinear;
	REAL ex, ey;
	REAL tx, ty;
	REAL etx, ety;
	REAL split, denom;
	
	// usually cause error, cancel it temporarily
	//return ;

	/* Find the other three segment endpoints. */
	endpoint1 = apex(*splittri);
	torg = org(*splittri);
	tdest = dest(*splittri);
	/* Segment intersection formulae; see the Antonio reference. */
	tx = tdest->x - torg->x;
	ty = tdest->y - torg->y;
	ex = endpoint2->x - endpoint1->x;
	ey = endpoint2->y - endpoint1->y;
	etx = torg->x - endpoint2->x;
	ety = torg->y - endpoint2->y;
	denom = ty * ex - tx * ey;
	if (denom == 0.0) {
		printf("Internal error in segmentintersection():");
		printf("  Attempt to find intersection of parallel segments.\n");
		internalerror();
	}
	split = (ey * etx - ex * ety) / denom;
	/* Create the new triPOINT. */
	newpoint = (triPOINT *) PointAlloc();
	/* Interpolate its coordinate and attributes. */
	newpoint->x = torg->x + split * (tdest->x - torg->x);
	newpoint->y = torg->y + split * (tdest->y - torg->y);
	newpoint->attr = (float)(torg->attr + split * (tdest->attr - torg->attr));
	SetPointDup ( newpoint, NULL );
	SetPointMark( newpoint, shMark(*splitshelle) );

	if (m_verbose > 1) {
		printf(
			"  Splitting EDGE (%.12g, %.12g) (%.12g, %.12g) at (%.12g, %.12g).\n",
			torg->x, torg->y, tdest->x, tdest->y, newpoint->x, newpoint->y);
	}

	//////////////////////////////////////
	
	/* Insert the intersection triPOINT.  This should always succeed. */
	success = InsertSite(newpoint, splittri, splitshelle, 0, 0);
	if (success != SUCCESSFULPOINT) {
		printf("Internal error in segmentintersection():\n");
		printf("  Failure to split a segment.\n");
		internalerror();
	}
	if (steinerleft > 0) {
		steinerleft--;
	}

	/* Inserting the triPOINT may have caused EDGE flips.  We wish to rediscover */
	/*   the EDGE connecting endpoint1 to the new intersection triPOINT.         */
	collinear = finddirection(splittri, endpoint1);
	rightpoint = dest(*splittri);
	leftpoint = apex(*splittri);
	if ((leftpoint->x == endpoint1->x) && (leftpoint->y == endpoint1->y)) {
		oNextSpinEdgeSelf(*splittri);
	} else if ((rightpoint->x != endpoint1->x) ||
		(rightpoint->y != endpoint1->y)) {
		printf("Internal error in segmentintersection():\n");
		printf("  Topological inconsistency after splitting a segment.\n");
		internalerror();
	}
	/* `splittri' should have destination endpoint1. */
}

/*****************************************************************************/
/*  scout : 侦察                                                                         */
/*  scoutsegment()   Scout the first TRIANGLE on the path from one endpoint  */
/*                   to another, and check for completion (reaching the      */
/*                   second endpoint), a collinear triPOINT, and the            */
/*                   intersection of two segments.                           */
/*                                                                           */
/*  Returns one if the entire segment is successfully inserted, and zero if  */
/*  the job must be finished by conformingedge() or constrainededge().       */
/*                                                                           */
/*  If the first TRIANGLE on the path has the second endpoint as its         */
/*  destination or apex, a shell EDGE is inserted and the job is done.       */
/*                                                                           */
/*  If the first TRIANGLE on the path has a destination or apex that lies on */
/*  the segment, a shell EDGE is inserted connecting the first endpoint to   */
/*  the collinear triPOINT, and the search is continued from the collinear      */
/*  triPOINT.                                                                   */
/*                                                                           */
/*  If the first TRIANGLE on the path has a shell EDGE opposite its origin,  */
/*  then there is a segment that intersects the segment being inserted.      */
/*  Their intersection triPOINT is inserted, splitting the shell EDGE.          */
/*                                                                           */
/*  Otherwise, return zero.                                                  */
/*                                                                           */
/*****************************************************************************/

int CTINClass::scoutsegment(triEDGE*searchtri,triPOINT *endpoint2,int newmark)
{
	triEDGE crosstri;
	EDGE crossedge;
	triPOINT *leftpoint, *rightpoint;
	triPOINT *endpoint1;
	enum finddirectionresult collinear;
	
	collinear = finddirection(searchtri, endpoint2);
	rightpoint = dest(*searchtri);
	leftpoint = apex(*searchtri);
	if (((leftpoint->x == endpoint2->x) && (leftpoint->y == endpoint2->y)) ||
		((rightpoint->x == endpoint2->x) && (rightpoint->y == endpoint2->y))) {
		/* The segment is already an EDGE in the mesh. */
		if ((leftpoint->x == endpoint2->x) && (leftpoint->y == endpoint2->y)) {
			PrevEdgeSelf(*searchtri);
		}
		/* Insert a shell EDGE, if there isn't already one there. */
		insertshelle(searchtri, newmark);
		return 1;
	} else if (collinear == LEFTCOLLINEAR) {
		/* We've collided with a triPOINT between the segment's endpoints. */
		/* Make the collinear triPOINT be the TRIANGLE's origin. */
		PrevEdgeSelf(*searchtri);
		insertshelle(searchtri, newmark);
		/* Insert the remainder of the segment. */
		return scoutsegment(searchtri, endpoint2, newmark);
	} else if (collinear == RIGHTCOLLINEAR) {
		/* We've collided with a triPOINT between the segment's endpoints. */
		insertshelle(searchtri, newmark);
		/* Make the collinear triPOINT be the TRIANGLE's origin. */
		NextEdgeSelf(*searchtri);
		/* Insert the remainder of the segment. */
		return scoutsegment(searchtri, endpoint2, newmark);
	} else {
		NextEdge(*searchtri, crosstri);
		tspivot(crosstri, crossedge);
		/* Check for a crossing segment. */
		if (crossedge.sh == m_dummysh) {
			return 0;
		} else {
			if( !m_segmentIntersectionEnabled ) 
				internalerror();

			endpoint1 = org(*searchtri);
			/* Insert a triPOINT at the intersection. */
			segmentintersection(&crosstri, &crossedge, endpoint2);
			TriEdgeCopy(crosstri, *searchtri);
			insertshelle(searchtri, newmark);
			/* Insert the remainder of the segment. */
			return scoutsegment(searchtri, endpoint2, newmark);
		}
	}
}

/*****************************************************************************/
/*                                                                           */
/*  conformingedge()   Force a segment into a conforming Delaunay            */
/*                     triangulation by inserting a triPOINT at its midpoint,   */
/*                     and recursively forcing in the two half-segments if   */
/*                     necessary.                                            */
/*                                                                           */
/*  Generates a sequence of edges connecting `endpoint1' to `endpoint2'.     */
/*  `newmark' is the boundary marker of the segment, assigned to each new    */
/*  splitting triPOINT and shell EDGE.                                          */
/*                                                                           */
/*  Note that conformingedge() does not always maintain the conforming       */
/*  Delaunay property.  Once inserted, segments are locked into place;       */
/*  m_points inserted later (to force other segments in) may render these      */
/*  fixed segments non-Delaunay.  The conforming Delaunay property will be   */
/*  restored by enforcequality() by splitting encroached segments.           */
/*                                                                           */
/*****************************************************************************/

#ifndef REDUCED
#ifndef CDT_ONLY

void CTINClass::conformingedge(triPOINT * endpoint1,triPOINT * endpoint2,int newmark)
{
	triEDGE searchtri1, searchtri2;
	EDGE brokenshelle;
	triPOINT * newpoint;
	triPOINT * midpoint1, *midpoint2;
	enum insertsiteresult success;
	int result1, result2;
	
	if (m_verbose > 2) {
		printf("Forcing segment into triangulation by recursive splitting:\n");
		printf("  (%.12g, %.12g) (%.12g, %.12g)\n", endpoint1->x, endpoint1->y,
			endpoint2->x, endpoint2->y);
	}
	/* Create a new triPOINT to insert in the middle of the segment. */
	newpoint = (triPOINT *) PointAlloc();
	/* Interpolate coordinates and attributes. */
	newpoint->x = 0.5 * (endpoint1->x + endpoint2->x);
	newpoint->y = 0.5 * (endpoint1->y + endpoint2->y);
	newpoint->attr = (float)(0.5 * (endpoint1->attr + endpoint2->attr));
	SetPointDup( newpoint, NULL );
	
	SetPointMark(newpoint, newmark);
	/* Find a boundary TRIANGLE to search from. */
	searchtri1.tri = (TRIANGLE *) NULL;
	/* Attempt to insert the new triPOINT. */
	success = InsertSite(newpoint, &searchtri1, (EDGE *) NULL, 0, 0);
	if (success == DUPLICATEPOINT) {
		if (m_verbose > 2) {
			printf("  Segment intersects existing triPOINT (%.12g, %.12g).\n",
				newpoint->x, newpoint->y);
		}
		/* Use the triPOINT that's already there. */
		PointDealloc(newpoint);
		newpoint = org(searchtri1);
	} else {
		if (success == VIOLATINGPOINT) {
			if (m_verbose > 2) {
				printf("  Two segments intersect at (%.12g, %.12g).\n",
					newpoint->x, newpoint->y);
			}
			/* By fluke, we've landed right on another segment.  Split it. */
			tspivot(searchtri1, brokenshelle);
			success = InsertSite(newpoint, &searchtri1, &brokenshelle, 0, 0);
			if (success != SUCCESSFULPOINT) {
				printf("Internal error in conformingedge():\n");
				printf("  Failure to split a segment.\n");
				internalerror();
			}
		}
		/* The triPOINT has been inserted successfully. */
		if (steinerleft > 0) {
			steinerleft--;
		}
	}
	TriEdgeCopy(searchtri1, searchtri2);
	result1 = scoutsegment(&searchtri1, endpoint1, newmark);
	result2 = scoutsegment(&searchtri2, endpoint2, newmark);
	if (!result1) {
		/* The origin of searchtri1 may have changed if a collision with an */
		/*   intervening vertex on the segment occurred.                    */
		midpoint1 = org(searchtri1);
		conformingedge(midpoint1, endpoint1, newmark);
	}
	if (!result2) {
		/* The origin of searchtri2 may have changed if a collision with an */
		/*   intervening vertex on the segment occurred.                    */
		midpoint2 = org(searchtri2);
		conformingedge(midpoint2, endpoint2, newmark);
	}
}

#endif /* not CDT_ONLY */
#endif /* not REDUCED */

/*****************************************************************************/
/*                                                                           */
/*  delaunayfixup()   Enforce the Delaunay condition at an EDGE, fanning out */
/*                    recursively from an existing triPOINT.  Pay special       */
/*                    attention to stacking inverted triangles.              */
/*                                                                           */
/*  This is a support routine for inserting segments into a constrained      */
/*  Delaunay triangulation.                                                  */
/*                                                                           */
/*  The origin of fixuptri is treated as if it has just been inserted, and   */
/*  the local Delaunay condition needs to be enforced.  It is only enforced  */
/*  in one sector, however, that being the angular range defined by          */
/*  fixuptri.                                                                */
/*                                                                           */
/*  This routine also needs to make decisions regarding the "stacking" of    */
/*  m_triangles.  (Read the description of constrainededge() below before      */
/*  reading on here, so you understand the algorithm.)  If the position of   */
/*  the new triPOINT (the origin of fixuptri) indicates that the vertex before  */
/*  it on the polygon is a reflex vertex, then "stack" the TRIANGLE by       */
/*  doing nothing.  (fixuptri is an inverted TRIANGLE, which is how stacked  */
/*  m_triangles are identified.)                                               */
/*                                                                           */
/*  Otherwise, check whether the vertex before that was a reflex vertex.     */
/*  If so, perform an EDGE flip, thereby eliminating an inverted TRIANGLE    */
/*  (popping it off the stack).  The EDGE flip may result in the creation    */
/*  of a new inverted TRIANGLE, depending on whether or not the new vertex   */
/*  is visible to the vertex three edges behind on the polygon.              */
/*                                                                           */
/*  If neither of the two vertices behind the new vertex are reflex          */
/*  vertices, fixuptri and fartri, the TRIANGLE opposite it, are not         */
/*  inverted; hence, ensure that the EDGE between them is locally Delaunay.  */
/*                                                                           */
/*  `leftside' indicates whether or not fixuptri is to the left of the       */
/*  segment being inserted.  (Imagine that the segment is pointing up from   */
/*  endpoint1 to endpoint2.)                                                 */
/*                                                                           */
/*****************************************************************************/

bool CTINClass::delaunayfixup(triEDGE*fixuptri,int  leftside)
{
	triEDGE neartri;
	triEDGE fartri;
	EDGE faredge;
	triPOINT *nearpoint, *leftpoint, *rightpoint, *farpoint;
	
	NextEdge(*fixuptri, neartri);
	SymmTri(neartri, fartri);
	/* Check if the EDGE opposite the origin of fixuptri can be flipped. */
	if (fartri.tri == m_dummytri) {
		return false;
	}
	tspivot(neartri, faredge);
	if (faredge.sh != m_dummysh) {
		return false;
	}
	/* Find all the relevant vertices. */
	nearpoint = apex(neartri);
	leftpoint = org(neartri);
	rightpoint = dest(neartri);
	farpoint = apex(fartri);
	/* Check whether the previous polygon vertex is a reflex vertex. */
	if (leftside) {
		if (counterclockwise(nearpoint, leftpoint, farpoint) <= 0.0) {
			/* leftpoint is a reflex vertex too.  Nothing can */
			/*   be done until a m_convex section is found.     */
			return false;
		}
	} else {
		if (counterclockwise(farpoint, rightpoint, nearpoint) <= 0.0) {
			/* rightpoint is a reflex vertex too.  Nothing can */
			/*   be done until a m_convex section is found.      */
			return false;
		}
	}
	if (counterclockwise(rightpoint, leftpoint, farpoint) > 0.0) {
		/* fartri is not an inverted TRIANGLE, and farpoint is not a reflex */
		/*   vertex.  As there are no reflex vertices, fixuptri isn't an    */
		/*   inverted TRIANGLE, either.  Hence, test the EDGE between the   */
		/*   m_triangles to ensure it is locally Delaunay.                    */
		if (incircle(leftpoint, farpoint, rightpoint, nearpoint) <= 0.0) {
			return false;
		}
		/* Not locally Delaunay; go on to an EDGE flip. */
	}        /* else fartri is inverted; remove it from the stack by flipping. */
	else	{
		printf("");
	}

	flip(&neartri);
	PrevEdgeSelf(*fixuptri);    /* Restore the origin of fixuptri after the flip. */
	/* Recursively process the two m_triangles that result from the flip. */
	delaunayfixup(fixuptri, leftside);
	delaunayfixup(&fartri, leftside);

	return true;
}

/*****************************************************************************/
/*                                                                           */
/*  constrainededge()   Force a segment into a constrained Delaunay          */
/*                      triangulation by deleting the m_triangles it           */
/*                      intersects, and triangulating the polygons that      */
/*                      form on each side of it.                             */
/*                                                                           */
/*  Generates a single EDGE connecting `endpoint1' to `endpoint2'.  The      */
/*  TRIANGLE `starttri' has `endpoint1' as its origin.  `newmark' is the     */
/*  boundary marker of the segment.                                          */
/*                                                                           */
/*  To insert a segment, every TRIANGLE whose interior intersects the        */
/*  segment is deleted.  The union of these deleted m_triangles is a polygon   */
/*  (which is not necessarily monotone, but is close enough), which is       */
/*  divided into two polygons by the new segment.  This routine's task is    */
/*  to generate the Delaunay triangulation of these two polygons.            */
/*                                                                           */
/*  You might think of this routine's behavior as a two-step process.  The   */
/*  first step is to walk from endpoint1 to endpoint2, flipping each EDGE    */
/*  encountered.  This step creates a fan of edges connected to endpoint1,   */
/*  including the desired EDGE to endpoint2.  The second step enforces the   */
/*  Delaunay condition on each side of the segment in an incremental manner: */
/*  proceeding along the polygon from endpoint1 to endpoint2 (this is done   */
/*  independently on each side of the segment), each vertex is "enforced"    */
/*  as if it had just been inserted, but affecting only the previous         */
/*  vertices.  The result is the same as if the vertices had been inserted   */
/*  in the m_order they appear on the polygon, so the result is Delaunay.      */
/*                                                                           */
/*  In truth, constrainededge() interleaves these two steps.  The procedure  */
/*  walks from endpoint1 to endpoint2, and each time an EDGE is encountered  */
/*  and flipped, the newly exposed vertex (at the far end of the flipped     */
/*  EDGE) is "enforced" upon the previously flipped edges, usually affecting */
/*  only one side of the polygon (depending upon which side of the segment   */
/*  the vertex falls on).                                                    */
/*                                                                           */
/*  The algorithm is complicated by the need to handle polygons that are not */
/*  m_convex.  Although the polygon is not necessarily monotone, it can be     */
/*  triangulated in a manner similar to the stack-based algorithms for       */
/*  monotone polygons.  For each reflex vertex (local concavity) of the      */
/*  polygon, there will be an inverted TRIANGLE formed by one of the EDGE    */
/*  flips.  (An inverted TRIANGLE is one with negative area - that is, its   */
/*  vertices are arranged in clockwise m_order - and is best thought of as a   */
/*  wrinkle in the fabric of the mesh.)  Each inverted TRIANGLE can be       */
/*  thought of as a reflex vertex pushed on the stack, waiting to be fixed   */
/*  later.                                                                   */
/*                                                                           */
/*  A reflex vertex is popped from the stack when a vertex is inserted that  */
/*  is visible to the reflex vertex.  (However, if the vertex behind the     */
/*  reflex vertex is not visible to the reflex vertex, a new inverted        */
/*  TRIANGLE will take its place on the stack.)  These details are handled   */
/*  by the delaunayfixup() routine above.                                    */
/*                                                                           */
/*****************************************************************************/

void CTINClass::constrainededge(triEDGE*starttri,triPOINT *endpoint2,int newmark)
{
	triEDGE fixuptri, fixuptri2;
	EDGE fixupedge;
	triPOINT *endpoint1;
	triPOINT *farpoint;
	REAL area;
	int collision;
	int done;
	
	endpoint1 = org(*starttri );
	NextEdge(*starttri, fixuptri);
	flip(&fixuptri);
	/* `collision' indicates whether we have found a triPOINT directly */
	/*   between endpoint1 and endpoint2.                           */
	collision = 0;
	done = 0;
	
	int i, times=100000;
	for ( i=0; i< times ; i++) 
	{
		farpoint = org(fixuptri);
		/* `farpoint' is the extreme triPOINT of the polygon we are "digging" */
		/*   to get from endpoint1 to endpoint2.                           */
		if ((farpoint->x == endpoint2->x) && (farpoint->y == endpoint2->y)) {
			oPrevSpinEdge(fixuptri, fixuptri2);
			/* Enforce the Delaunay condition around endpoint2. */
			delaunayfixup(&fixuptri, 0);
			delaunayfixup(&fixuptri2, 1);
			done = 1;
		} else {
			/* Check whether farpoint is to the left or right of the segment */
			/*   being inserted, to decide which EDGE of fixuptri to dig     */
			/*   through next.                                               */
			area = counterclockwise(endpoint1, endpoint2, farpoint);
			if (area == 0.0) {
				/* We've collided with a triPOINT between endpoint1 and endpoint2. */
				collision = 1;
				oPrevSpinEdge(fixuptri, fixuptri2);
				/* Enforce the Delaunay condition around farpoint. */
				delaunayfixup(&fixuptri, 0);
				delaunayfixup(&fixuptri2, 1);
				done = 1;
			} else {
				if (area > 0.0) {         /* farpoint is to the left of the segment. */
					oPrevSpinEdge(fixuptri, fixuptri2);
					/* Enforce the Delaunay condition around farpoint, on the */
					/*   left side of the segment only.                       */
					delaunayfixup(&fixuptri2, 1);
					/* Flip the EDGE that crosses the segment.  After the EDGE is */
					/*   flipped, one of its endpoints is the fan vertex, and the */
					/*   destination of fixuptri is the fan vertex.               */
					PrevEdgeSelf(fixuptri);
				} else {                 /* farpoint is to the right of the segment. */
					delaunayfixup(&fixuptri, 0);
					/* Flip the EDGE that crosses the segment.  After the EDGE is */
					/*   flipped, one of its endpoints is the fan vertex, and the */
					/*   destination of fixuptri is the fan vertex.               */
					oPrevSpinEdgeSelf(fixuptri);
				}
				/* Check for two intersecting segments. */
				tspivot(fixuptri, fixupedge);
				if (fixupedge.sh == m_dummysh) {
					flip(&fixuptri);   /* May create an inverted TRIANGLE on the left. */
				} else {
					if( !m_segmentIntersectionEnabled ) 
						internalerror();

					/* We've collided with a segment between endpoint1 and endpoint2. */
					collision = 1;
					/* Insert a triPOINT at the intersection. */
					segmentintersection(&fixuptri, &fixupedge, endpoint2);
					done = 1;
				}
			}
		}
		if( done ) break;
	}

	/* Insert a shell EDGE to make the segment permanent. */
	insertshelle(&fixuptri, newmark);
	/* If there was a collision with an interceding vertex, install another */
	/*   segment connecting that vertex with endpoint2.                     */
	if (collision) {
		/* Insert the remainder of the segment. */
		if (!scoutsegment(&fixuptri, endpoint2, newmark)) {
			constrainededge(&fixuptri, endpoint2, newmark);
		}
	}

	// for debug
	//NextEdge( fixuptri, m_fixuptri ); 
}


////////////////////////////////////////////////////////////////////////
//  Find a TRIANGLE whose origin is the segment's first endPoint.
////////////////////////////////////////////////////////////////////////
void CTINClass::LocatePoint( triPOINT *searchPoint, triEDGE &searchTri )
{
	TRIANGLE *encodedTri;
	triPOINT *checkpoint = NULL;
	
	encodedTri = Point2Tri(searchPoint);
	
	if( encodedTri == NULL) {
		printf("Error: segment end abuts no TriANGLE\n");
		return ;
	}
	
	decode( encodedTri, searchTri);
	checkpoint = org(searchTri);
	if( checkpoint != searchPoint ) {
		/* Find a boundary TriANGLE to search from. */
		searchTri.tri = m_dummytri;
		searchTri.orient = 0;
		SymmTriSelf(searchTri);
		/* Search for the segment's first endPoint by TriPOINT location. */
		locateresult ret = locate(searchPoint, &searchTri);
		if( ret != ONVERTEX) {
			printf("Internal error in Locate ():  Unable to locate point\n");
			printf("  (%.12g, %.12g) in Triangulation.\n",searchPoint->x, searchPoint->y);
			internalerror();
		}
	}
	/* Remember this TriANGLE to improve subsequent TriPOINT location. */
	TriEdgeCopy(searchTri, m_recentTri);
}


/////////////////////////////////////////////////////////////////////
// 
// check for insert segment with non intersection
//
/////////////////////////////////////////////////////////////////////
bool CTINClass::PreInsertSegment(triEDGE &searchtri, triPOINT *endpoint2 )
{
	EDGE crossedge;
	triPOINT *leftpoint, *rightpoint, *farpoint;
	triPOINT *endpoint1;
	enum finddirectionresult collinear;
	REAL area;
	
	endpoint1 = org ( searchtri );
	
	while( searchtri.tri != m_dummytri  )	{
		collinear = finddirection( &searchtri, endpoint2);
		
		rightpoint = dest(searchtri);
		leftpoint = apex(searchtri);
		
		if (((leftpoint->x == endpoint2->x) && (leftpoint->y == endpoint2->y)) ||
			((rightpoint->x == endpoint2->x) && (rightpoint->y == endpoint2->y))) 
			return true;
		else if (collinear == LEFTCOLLINEAR) 
			PrevEdgeSelf(searchtri);
		else if (collinear == RIGHTCOLLINEAR) 
			NextEdgeSelf(searchtri);
		else {	// cross
			NextEdgeSelf(searchtri);

			while( searchtri.tri != m_dummytri ) {
				tspivot(searchtri, crossedge);
	
				if( crossedge.sh != m_dummysh ) 
					return false;
				
				SymmTriSelf( searchtri );
				
				farpoint = apex(searchtri);
				
				if ((farpoint->x == endpoint2->x) && (farpoint->y == endpoint2->y)) 
					return true;
				else {
					area = counterclockwise(endpoint1, endpoint2, farpoint);
					if (area == 0.0) {
						PrevEdgeSelf( searchtri );
						break;
					} 
					else {
						if (area > 0.0)        // apex is to the left of the segment.
							NextEdgeSelf( searchtri );
						else 					// apex is to the right of the segment. 
							PrevEdgeSelf( searchtri );
					}
				}
			}
		}
	}
}


/*****************************************************************************/
/*                                                                           */
/*  insertsegment()   Insert a PSLG segment into a triangulation.            */
/*                                                                           */
/*****************************************************************************/
void CTINClass::insertsegment(triPOINT *endPoint1,triPOINT *endPoint2,int newmark)
{
	triEDGE searchtri1, searchtri2;
	
	//////////////////////////////////////////
	if( PointDup( endPoint1 ) != NULL )
		endPoint1 = PointDup( endPoint1 );
	
	if( PointDup( endPoint2 ) != NULL )
		endPoint2 = PointDup( endPoint2 );
	
	///////////////////////////////////////////////////////////////
	if( !m_segmentIntersectionEnabled ) {
		LocatePoint( endPoint1, searchtri1 );
		if( !PreInsertSegment( searchtri1, endPoint2) )
			return ;
	}
	
	///////////////////////////////////////////////////////////////
 
	m_checksegments = true;		/* Segments will be introduced next. */

	LocatePoint( endPoint1, searchtri1 );

	/* Scout the beginnings of a path from the first endPoint */
	/*   toward the second.                                   */
	if (scoutsegment(&searchtri1, endPoint2, newmark)) {
		/* The segment was easily inserted. */
		return;
	}
	/* The first endPoint may have changed if a collision with an intervening */
	/*   vertex on the segment occurred.                                      */
	endPoint1 = org(searchtri1);
	
	///////////////////////////////////////////////
	LocatePoint( endPoint2, searchtri2 );

	/* Scout the beginnings of a path from the second endPoint */
	/*   toward the first.                                     */
	if (scoutsegment(&searchtri2, endPoint1, newmark)) {
		/* The segment was easily inserted. */
		return;
	}
	/* The second endPoint may have changed if a collision with an intervening */
	/*   vertex on the segment occurred.                                       */
	endPoint2 = org(searchtri2);
	
#ifndef REDUCED
#ifndef CDT_ONLY
	if (m_splitSeg) {
		/* Insert vertices to force the segment into the triangulation. */
		conformingedge(endPoint1, endPoint2, newmark);
	} else {
#endif /* not CDT_ONLY */
#endif /* not REDUCED */
		/* Insert the segment directly into the triangulation. */
		constrainededge(&searchtri1, endPoint2, newmark);
#ifndef REDUCED
#ifndef CDT_ONLY
	}
#endif /* not CDT_ONLY */
#endif /* not REDUCED */
}

/*****************************************************************************/
/*                                                                           */
/*  markhull()   Cover the m_convex hull of a triangulation with shell edges.  */
/*                                                                           */
/*****************************************************************************/

void CTINClass::markhull()
{
	triEDGE hulltri;
	triEDGE nexttri;
	triEDGE starttri;
	
	/* Find a TRIANGLE handle on the hull. */
	hulltri.tri = m_dummytri;
	hulltri.orient = 0;
	SymmTriSelf(hulltri);
	/* Remember where we started so we know when to stop. */
	TriEdgeCopy(hulltri, starttri);
	/* Go once counterclockwise around the m_convex hull. */
	do {
		/* Create a shell EDGE if there isn't already one here. */
		insertshelle(&hulltri, 1);
		/* To find the next hull EDGE, go clockwise around the next vertex. */
		NextEdgeSelf(hulltri);
		oPrevSpinEdge(hulltri, nexttri);
		while (nexttri.tri != m_dummytri) {
			TriEdgeCopy(nexttri, hulltri);
			oPrevSpinEdge(hulltri, nexttri);
		}
	} while (!TriEdgeEqual(hulltri, starttri));
}

/*****************************************************************************/
/*                                                                           */
/*  formskeleton()   Create the shell edges of a triangulation, including    */
/*                   PSLG edges and edges on the m_convex hull.                */
/*                                                                           */
/*  The PSLG edges are read from a .poly file.  The return value is the      */
/*  number of segments in the file.                                          */
/*                                                                           */
/*****************************************************************************/
int CTINClass::formskeleton(long *segmentList,long *segMarkList,int numOfSegments)
{
	char polyfilename[6];
	long index;
	triPOINT *endpoint1, *endpoint2;
	long segments;
	int segmentmarkers;
	long end1, end2;
	int boundmarker;
	long i;
	
	if (m_poly) {
		if (!quiet) {
			printf("Inserting segments into Delaunay triangulation.\n");
		}
		strcpy(polyfilename, "input");
		segments = numOfSegments;
		segmentmarkers = (segMarkList != NULL );
		index = 0;
		/* If segments are to be inserted, compute a mapping */
		/*   from m_points to m_triangles.                       */
		if( segments > 0 ) {
			if (m_verbose) {
				printf("  Inserting PSLG segments.\n");
			}
			MakePointMap();
		}
		
		boundmarker = 0;
		/* Read and insert the segments. */
		for (i = 0; i < segments; i++) 
		{
			end1 = segmentList[index++];
			end2 = segmentList[index++];
			
			if (segmentmarkers) {
				boundmarker = segMarkList[i];
			}
			if ((end1 < m_firstnumber) || (end1 >= m_firstnumber + m_points.GetNumberOfItems() )) {
				if (!quiet) {
					printf("Warning:  Invalid first endpoint of segment %d in %s.\n", i,
						polyfilename);
				}
			} 
			else if ((end2 < m_firstnumber) || (end2 >= m_firstnumber + m_points.GetNumberOfItems() )) {
				if (!quiet) {
					printf("Warning:  Invalid second endpoint of segment %d in %s.\n", i,
						polyfilename);
				}
			} 
			else {
				endpoint1 = GetPoint(end1);
				endpoint2 = GetPoint(end2);
				if ((endpoint1->x == endpoint2->x) && (endpoint1->y == endpoint2->y)) {
					if( !quiet ) {
						printf("Warning:  Endpoints of segment %d are coincident in %s.\n",
							i, polyfilename);
					}
				} 
				else 
					insertsegment(endpoint1, endpoint2, boundmarker);
			}
		}
	}
	else segments = 0;
	
	if (m_convex || !m_poly) {
		/* Enclose the m_convex hull with shell edges. */
		if (m_verbose) {
			printf("  Enclosing m_convex hull with segments.\n");
		}
		markhull();
	}
	return segments;
}

/**                                                                         **/
/**                                                                         **/
/********* Segment (shell EDGE) insertion ends here                  *********/

/********* Carving out holes and concavities begins here             *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  infecthull()   Virally infect all of the m_triangles of the m_convex hull    */
/*                 that are not protected by shell edges.  Where there are   */
/*                 shell edges, set boundary markers as appropriate.         */
/*                                                                           */
/*****************************************************************************/

void CTINClass::InfectHull()
{
	triEDGE hulltri;
	triEDGE nexttri;
	triEDGE starttri;
	EDGE hulledge;
	TRIANGLE **deadtri;
	triPOINT *horg, *hdest;
	
	if (m_verbose) {
		printf("  Marking concavities (external m_triangles) for elimination.\n");
	}
	/* Find a TRIANGLE handle on the hull. */
	hulltri.tri = m_dummytri;
	hulltri.orient = 0;
	SymmTriSelf(hulltri);
	/* Remember where we started so we know when to stop. */
	TriEdgeCopy(hulltri, starttri);
	/* Go once counterclockwise around the m_convex hull. */
	do {
		/* Ignore m_triangles that are already infected. */
		if (!infected(hulltri)) {
			/* Is the TRIANGLE protected by a shell EDGE? */
			tspivot(hulltri, hulledge);
			if (hulledge.sh == m_dummysh) {
				/* The TRIANGLE is not protected; infect it. */
				infect(hulltri);
				deadtri = (TRIANGLE **) m_viri.PoolAlloc();
				*deadtri = hulltri.tri;
			} else {
				/* The TRIANGLE is protected; set boundary markers if appropriate. */
				if ( shMark( hulledge ) == 0) {
					SetShellMark(hulledge, 1);
					horg = org(hulltri);
					hdest = dest(hulltri);
					if (PointMark(horg) == 0) {
						SetPointMark(horg, 1);
					}
					if (PointMark(hdest) == 0) {
						SetPointMark(hdest, 1);
					}
				}
			}
		}
		/* To find the next hull EDGE, go clockwise around the next vertex. */
		NextEdgeSelf(hulltri);
		oPrevSpinEdge(hulltri, nexttri);
		while (nexttri.tri != m_dummytri) {
			TriEdgeCopy(nexttri, hulltri);
			oPrevSpinEdge(hulltri, nexttri);
		}
	} while (!TriEdgeEqual(hulltri, starttri));
}

/*****************************************************************************/
/*                                                                           */
/*  plague()   Spread the virus from all infected m_triangles to any neighbors */
/*             not protected by shell edges.  Delete all infected m_triangles. */
/*                                                                           */
/*  This is the procedure that actually creates holes and concavities.       */
/*                                                                           */
/*  This procedure operates in two phases.  The first phase identifies all   */
/*  the m_triangles that will die, and marks them as infected.  They are       */
/*  marked to ensure that each TRIANGLE is added to the virus pool only      */
/*  once, so the procedure will terminate.                                   */
/*                                                                           */
/*  The second phase actually eliminates the infected m_triangles.  It also    */
/*  eliminates orphaned m_points.                                              */
/*                                                                           */
/*****************************************************************************/

void CTINClass::Plague()
{
	triEDGE testtri;
	triEDGE neighbor;
	TRIANGLE **virusloop;
	TRIANGLE **deadtri;
	EDGE neighborshelle;
	triPOINT *testpoint;
	triPOINT *norg, *ndest;
	triPOINT *deadorg, *deaddest, *deadapex;
	int killorg;

	if (m_verbose) {
		printf("  Marking neighbors of marked m_triangles.\n");
	}
	/* Loop through all the infected m_triangles, spreading the virus to */
	/*   their neighbors, then to their neighbors' neighbors.          */
	m_viri.TraversalInit();
	virusloop = (TRIANGLE **) m_viri.Traverse();
	while (virusloop != (TRIANGLE **) NULL) {
		testtri.tri = *virusloop;
		/* A TRIANGLE is marked as infected by messing with one of its shell */
		/*   edges, setting it to an illegal value.  Hence, we have to       */
		/*   temporarily uninfect this TRIANGLE so that we can examine its   */
		/*   adjacent shell edges.                                           */
		uninfect(testtri);
		if (m_verbose > 2) {
			/* Assign the TRIANGLE an orientation for convenience in */
			/*   checking its m_points.                                */
			testtri.orient = 0;
			deadorg = org(testtri);
			deaddest = dest(testtri);
			deadapex = apex(testtri);
			printf("    Checking (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
				deadorg[0], deadorg[1], deaddest[0], deaddest[1],
				deadapex[0], deadapex[1]);
		}
		/* Check each of the TRIANGLE's three neighbors. */
		for (testtri.orient = 0; testtri.orient < 3; testtri.orient++) {
			/* Find the neighbor. */
			SymmTri(testtri, neighbor);
			/* Check for a shell between the TRIANGLE and its neighbor. */
			tspivot(testtri, neighborshelle);
			/* Check if the neighbor is nonexistent or already infected. */
			if ((neighbor.tri == m_dummytri) || infected(neighbor)) {
				if (neighborshelle.sh != m_dummysh) {
					/* There is a shell EDGE separating the TRIANGLE from its */
					/*   neighbor, but both m_triangles are dying, so the shell */
					/*   EDGE dies too.                                       */
					shelleDealloc(neighborshelle.sh);
					if (neighbor.tri != m_dummytri) {
						/* Make sure the shell EDGE doesn't get deallocated again */
						/*   later when the infected neighbor is visited.         */
						uninfect(neighbor);
						tshDissolve(neighbor);
						infect(neighbor);
					}
				}
			} else {                   /* The neighbor exists and is not infected. */
				if (neighborshelle.sh == m_dummysh) {
					/* There is no shell EDGE protecting the neighbor, so */
					/*   the neighbor becomes infected.                   */
					if (m_verbose > 2) {
						deadorg = org(neighbor);
						deaddest = dest(neighbor);
						deadapex = apex(neighbor);
						printf(
							"    Marking (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
							deadorg[0], deadorg[1], deaddest[0], deaddest[1],
							deadapex[0], deadapex[1]);
					}
					infect(neighbor);
					/* Ensure that the neighbor's neighbors will be infected. */
					deadtri = (TRIANGLE **) m_viri.PoolAlloc();
					*deadtri = neighbor.tri;
				} else {               /* The neighbor is protected by a shell EDGE. */
					/* Remove this TRIANGLE from the shell EDGE. */
					shtDissolve(neighborshelle);
					/* The shell EDGE becomes a boundary.  Set markers accordingly. */
					if( shMark( neighborshelle ) == 0) {
						SetShellMark(neighborshelle, 1);
					}
					norg = org(neighbor);
					ndest = dest(neighbor );
					if (PointMark(norg) == 0) {
						SetPointMark(norg, 1);
					}
					if (PointMark(ndest) == 0) {
						SetPointMark(ndest, 1);
					}
				}
			}
		}
		/* Remark the TRIANGLE as infected, so it doesn't get added to the */
		/*   virus pool again.                                             */
		infect(testtri);
		virusloop = (TRIANGLE **) m_viri.Traverse();
	}
	
	if (m_verbose) {
		printf("  Deleting marked m_triangles.\n");
	}
	m_viri.TraversalInit();
	virusloop = (TRIANGLE **) m_viri.Traverse();
	while (virusloop != (TRIANGLE **) NULL) {
		testtri.tri = *virusloop;
		
		/* Check each of the three corners of the TRIANGLE for elimination. */
		/*   This is done by walking around each triPOINT, checking if it is   */
		/*   still connected to at least one live TRIANGLE.                 */
		for (testtri.orient = 0; testtri.orient < 3; testtri.orient++) {
			testpoint = org(testtri );
			/* Check if the triPOINT has already been tested. */
			if (testpoint != (triPOINT *) NULL) {
				killorg = 1;
				/* Mark the corner of the TRIANGLE as having been tested. */
				SetOrg(testtri, NULL);
				/* Walk counterclockwise about the triPOINT. */
				oNextSpinEdge(testtri, neighbor);
				/* Stop upon reaching a boundary or the starting TRIANGLE. */
				while ((neighbor.tri != m_dummytri)
					&& (!TriEdgeEqual(neighbor, testtri))) {
					if (infected(neighbor)) {
						/* Mark the corner of this TRIANGLE as having been tested. */
						SetOrg(neighbor, NULL);
					} else {
						/* A live TRIANGLE.  The triPOINT survives. */
						killorg = 0;
					}
					/* Walk counterclockwise about the triPOINT. */
					oNextSpinEdgeSelf(neighbor);
				}
				/* If we reached a boundary, we must walk clockwise as well. */
				if (neighbor.tri == m_dummytri) {
					/* Walk clockwise about the triPOINT. */
					oPrevSpinEdge(testtri, neighbor);
					/* Stop upon reaching a boundary. */
					while (neighbor.tri != m_dummytri) {
						if (infected(neighbor)) {
							/* Mark the corner of this TRIANGLE as having been tested. */
							SetOrg(neighbor, NULL);
						} else {
							/* A live TRIANGLE.  The triPOINT survives. */
							killorg = 0;
						}
						/* Walk clockwise about the triPOINT. */
						oPrevSpinEdgeSelf(neighbor);
					}
				}
				if (killorg) {
					if (m_verbose > 1) {
						printf("    Deleting triPOINT (%.12g, %.12g)\n",
							testpoint->x, testpoint->y);
					}
					PointDealloc(testpoint);
				}
			}
		}
		
		/* Record changes in the number of boundary edges, and disconnect */
		/*   dead m_triangles from their neighbors.                         */
		for (testtri.orient = 0; testtri.orient < 3; testtri.orient++) {
			SymmTri(testtri, neighbor);
			if (neighbor.tri == m_dummytri) {
				/* There is no neighboring TRIANGLE on this EDGE, so this EDGE    */
				/*   is a boundary EDGE.  This TRIANGLE is being deleted, so this */
				/*   boundary EDGE is deleted.                                    */
				hullsize--;
			} else {
				/* Disconnect the TRIANGLE from its neighbor. */
				dissolve(neighbor);
				/* There is a neighboring TRIANGLE on this EDGE, so this EDGE */
				/*   becomes a boundary EDGE when this TRIANGLE is deleted.   */
				hullsize++;
			}
		}
		/* Return the dead TRIANGLE to the pool of m_triangles. */
		triangleDealloc(testtri.tri);
		virusloop = (TRIANGLE **) m_viri.Traverse();
	}
	/* Empty the virus pool. */
	m_viri.poolrestart();
}

/*****************************************************************************/
/*                                                                           */
/*  regionplague()   Spread regional attributes and/or area constraints      */
/*                   (from a .poly file) throughout the mesh.                */
/*                                                                           */
/*  This procedure operates in two phases.  The first phase spreads an       */
/*  attribute and/or an area constraint through a (segment-bounded) region.  */
/*  The m_triangles are marked to ensure that each TRIANGLE is added to the    */
/*  virus pool only once, so the procedure will terminate.                   */
/*                                                                           */
/*  The second phase uninfects all infected m_triangles, returning them to     */
/*  normal.                                                                  */
/*                                                                           */
/*****************************************************************************/

void CTINClass::regionplague(REAL attribute,REAL area)
{
	triEDGE testtri;
	triEDGE neighbor;
	TRIANGLE **virusloop;
	TRIANGLE **regiontri;
	EDGE neighborshelle;
	triPOINT *regionorg, *regiondest, *regionapex;
	
	if (m_verbose > 1) {
		printf("  Marking neighbors of marked m_triangles.\n");
	}
	/* Loop through all the infected m_triangles, spreading the attribute      */
	/*   and/or area constraint to their neighbors, then to their neighbors' */
	/*   neighbors.                                                          */
	m_viri.TraversalInit();
	virusloop = (TRIANGLE **) m_viri.Traverse();
	while (virusloop != (TRIANGLE **) NULL) {
		testtri.tri = *virusloop;
		/* A TRIANGLE is marked as infected by messing with one of its shell */
		/*   edges, setting it to an illegal value.  Hence, we have to       */
		/*   temporarily uninfect this TRIANGLE so that we can examine its   */
		/*   adjacent shell edges.                                           */
		uninfect(testtri);
		if (regionattrib) {
			/* Set an attribute. */
			setelemattribute(testtri, eextras, (float)attribute);
		}
		if (vararea) {
			/* Set an area constraint. */
			setareabound(testtri, (float)area);
		}
		if (m_verbose > 2) {
			/* Assign the TRIANGLE an orientation for convenience in */
			/*   checking its m_points.                                */
			testtri.orient = 0;
			regionorg = org(testtri);
			regiondest = dest(testtri);
			regionapex = apex(testtri);
			printf("    Checking (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
				regionorg->x, regionorg->y, regiondest->x, regiondest->y,
				regionapex[0], regionapex[1]);
		}
		/* Check each of the TRIANGLE's three neighbors. */
		for (testtri.orient = 0; testtri.orient < 3; testtri.orient++) {
			/* Find the neighbor. */
			SymmTri(testtri, neighbor);
			/* Check for a shell between the TRIANGLE and its neighbor. */
			tspivot(testtri, neighborshelle);
			/* Make sure the neighbor exists, is not already infected, and */
			/*   isn't protected by a shell EDGE.                          */
			if ((neighbor.tri != m_dummytri) && !infected(neighbor)
				&& (neighborshelle.sh == m_dummysh)) {
				if (m_verbose > 2) {
					regionorg = org(neighbor );
					regiondest = dest(neighbor );
					regionapex = apex(neighbor );
					printf("    Marking (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
						regionorg->x, regionorg->y, regiondest->x, regiondest->y,
						regionapex[0], regionapex[1]);
				}
				/* Infect the neighbor. */
				infect(neighbor);
				/* Ensure that the neighbor's neighbors will be infected. */
				regiontri = (TRIANGLE **) m_viri.PoolAlloc();
				*regiontri = neighbor.tri;
			}
		}
		/* Remark the TRIANGLE as infected, so it doesn't get added to the */
		/*   virus pool again.                                             */
		infect(testtri);
		virusloop = (TRIANGLE **) m_viri.Traverse();
	}
	
	/* Uninfect all m_triangles. */
	if (m_verbose > 1) {
		printf("  Unmarking marked m_triangles.\n");
	}
	m_viri.TraversalInit();
	virusloop = (TRIANGLE **) m_viri.Traverse();
	while (virusloop != (TRIANGLE **) NULL) {
		testtri.tri = *virusloop;
		uninfect(testtri);
		virusloop = (TRIANGLE **) m_viri.Traverse();
	}
	/* Empty the virus pool. */
	m_viri.poolrestart();
}

/*****************************************************************************/
/*                                                                           */
/*  carveholes()   Find the holes and infect them.  Find the area            */
/*                 constraints and infect them.  Infect the m_convex hull.     */
/*                 Spread the infection and kill m_triangles.  Spread the      */
/*                 area constraints.                                         */
/*                                                                           */
/*  This routine mainly calls other routines to carry out all these          */
/*  functions.                                                               */
/*                                                                           */
/*****************************************************************************/

void CTINClass::CarveHoles(REAL *holeList, int holes, REAL *regionList, int regions)
{
	triEDGE searchtri;
	triEDGE triangleloop;
	triEDGE *regiontris;
	TRIANGLE **holetri;
	TRIANGLE **regiontri;
	triPOINT *searchorg, *searchdest;
	enum locateresult intersect;
	int i;
	
	if (!(quiet || (noholes && m_convex))) {
		printf("Removing unwanted m_triangles.\n");
		if (m_verbose && (holes > 0)) {
			printf("  Marking holes for elimination.\n");
		}
	}
	
	if (regions > 0) {
		/* Allocate storage for the m_triangles in which region m_points fall. */
		regiontris = (triEDGE*) malloc(regions * sizeof(triEDGE));
		if (regiontris == (triEDGE*) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	
	if (((holes > 0) && !noholes) || !m_convex || (regions > 0)) {
		/* Initialize a pool of m_viri to be used for holes, concavities, */
		/*   regional attributes, and/or regional area constraints.     */
		m_viri.PoolInit( VIRUSPERBLOCK );
	}
	
	if (!m_convex) {
		/* Mark as infected any unprotected m_triangles on the boundary. */
		/*   This is one way by which concavities are created.         */
		InfectHull();
	}
	
	if ((holes > 0) && !noholes) {
		/* Infect each TRIANGLE in which a hole lies. */
		for (i = 0; i < 2 * holes; i += 2) {
			/* Ignore holes that aren't within the bounds of the mesh. */
			if ((holeList[i] >= m_xmin) && (holeList[i] <= m_xmax)
				&& (holeList[i + 1] >= m_ymin) && (holeList[i + 1] <= m_ymax)) {
				/* Start searching from some TRIANGLE on the outer boundary. */
				searchtri.tri = m_dummytri;
				searchtri.orient = 0;
				SymmTriSelf(searchtri);
				/* Ensure that the hole is to the left of this boundary EDGE; */
				/*   otherwise, locate() will falsely report that the hole    */
				/*   falls within the starting TRIANGLE.                      */
				searchorg = org(searchtri);
				searchdest = dest(searchtri);
				if (counterclockwise(searchorg, searchdest,  (triPOINT *)(holeList+i) ) > 0.0) {
					/* Find a TRIANGLE that contains the hole. */
					intersect = locate((triPOINT *)&holeList[i], &searchtri);
					if ((intersect != OUTSIDE) && (!infected(searchtri))) {
						/* Infect the TRIANGLE.  This is done by marking the TRIANGLE */
						/*   as infect and including the TRIANGLE in the virus pool.  */
						infect(searchtri);
						holetri = (TRIANGLE **) m_viri.PoolAlloc();
						*holetri = searchtri.tri;
					}
				}
			}
		}
	}
	
	/* Now, we have to find all the regions BEFORE we carve the holes, because */
	/*   locate() won't work when the triangulation is no longer m_convex.       */
	/*   (Incidentally, this is the reason why regional attributes and area    */
	/*   constraints can't be used when refining a preexisting mesh, which     */
	/*   might not be m_convex; they can only be used with a freshly             */
	/*   triangulated PSLG.)                                                   */
	if (regions > 0) {
		/* Find the starting TRIANGLE for each region. */
		for (i = 0; i < regions; i++) {
			regiontris[i].tri = m_dummytri;
			/* Ignore region m_points that aren't within the bounds of the mesh. */
			if ((regionList[4 * i] >= m_xmin) && (regionList[4 * i] <= m_xmax) &&
				(regionList[4 * i + 1] >= m_ymin) && (regionList[4 * i + 1] <= m_ymax)) {
				/* Start searching from some TRIANGLE on the outer boundary. */
				searchtri.tri = m_dummytri;
				searchtri.orient = 0;
				SymmTriSelf(searchtri);
				/* Ensure that the region triPOINT is to the left of this boundary */
				/*   EDGE; otherwise, locate() will falsely report that the     */
				/*   region triPOINT falls within the starting TRIANGLE.           */
				searchorg = org(searchtri);
				searchdest = dest(searchtri);
				if( counterclockwise(searchorg, searchdest,(triPOINT *)(regionList+4*i) ) >
					0.0) {
					/* Find a TRIANGLE that contains the region triPOINT. */
					intersect = locate( (triPOINT *)(regionList + 4 * i), &searchtri);
					if ((intersect != OUTSIDE) && (!infected(searchtri))) {
						/* Record the TRIANGLE for processing after the */
						/*   holes have been carved.                    */
						TriEdgeCopy(searchtri, regiontris[i]);
					}
				}
			}
		}
	}
	
	if (m_viri.GetNumberOfItems() > 0) {
		/* Carve the holes and concavities. */
		Plague();
	}
	/* The virus pool should be empty now. */
	
	if (regions > 0) {
		if (!quiet) {
			if (regionattrib) {
				if (vararea) {
					printf("Spreading regional attributes and area constraints.\n");
				} else {
					printf("Spreading regional attributes.\n");
				}
			} else { 
				printf("Spreading regional area constraints.\n");
			}
		}
		if (regionattrib && !refine) {
			/* Assign every TRIANGLE a regional attribute of zero. */
			m_triangles.TraversalInit();
			triangleloop.orient = 0;
			triangleloop.tri = TriangleTraverse();
			while (triangleloop.tri != (TRIANGLE *) NULL) {
				setelemattribute(triangleloop, eextras, 0.0);
				triangleloop.tri = TriangleTraverse();
			}
		}
		for (i = 0; i < regions; i++) {
			if (regiontris[i].tri != m_dummytri) {
				/* Make sure the TRIANGLE under consideration still exists. */
				/*   It may have been eaten by the virus.                   */
				if (regiontris[i].tri->vertex[0] != (triPOINT *) NULL) {
					/* Put one TRIANGLE in the virus pool. */
					infect(regiontris[i]);
					regiontri = (TRIANGLE **) m_viri.PoolAlloc();
					*regiontri = regiontris[i].tri;
					/* Apply one region's attribute and/or area constraint. */
					regionplague(regionList[4 * i + 2], regionList[4 * i + 3]);
					/* The virus pool should be empty now. */
				}
			}
		}
		if (regionattrib && !refine) {
			/* Note the fact that each TRIANGLE has an additional attribute. */
			eextras++;
		}
	}
	
	/* Free up memory. */
	if (((holes > 0) && !noholes) || !m_convex || (regions > 0)) {
		m_viri.PoolDeinit();
	}
	if (regions > 0) {
		free(regiontris);
	}

	MakePointMap();
}

/**                                                                         **/
/**                                                                         **/
/********* Carving out holes and concavities ends here               *********/

/********* Mesh quality maintenance begins here                      *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  tallyencs()   Traverse the entire list of shell edges, check each EDGE   */
/*                to see if it is encroached.  If so, add it to the list.    */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

void CTINClass::tallyencs()
{
	EDGE edgeloop;
	int dummy;
	
	m_shelles.TraversalInit();
	edgeloop.shorient = 0;
	edgeloop.sh = shelleTraverse();
	while (edgeloop.sh != (SHELLE *) NULL) {
		/* If the segment is encroached, add it to the list. */
		dummy = checkedge4encroach(&edgeloop);
		edgeloop.sh = shelleTraverse();
	}
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  precisionerror()  Print an error message for precision problems.         */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

void CTINClass::precisionerror()
{
	printf("Try increasing the area criterion and/or reducing the minimum\n");
	printf("  allowable angle so that tiny m_triangles are not created.\n");
#ifdef SINGLE
	printf("Alternatively, try recompiling me with double precision\n");
	printf("  arithmetic (by removing \"#define SINGLE\" from the\n");
	printf("  source file or \"-DSINGLE\" from the makefile).\n");
#endif /* SINGLE */
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  repairencs()   Find and repair all the encroached segments.              */
/*                                                                           */
/*  Encroached segments are repaired by splitting them by inserting a triPOINT  */
/*  at or near their centers.                                                */
/*                                                                           */
/*  `flaws' is a flag that specifies whether one should take note of new     */
/*  encroached segments and bad m_triangles that result from inserting m_points  */
/*  to repair existing encroached segments.                                  */
/*                                                                           */
/*  When a segment is split, the two resulting subsegments are always        */
/*  tested to see if they are encroached upon, regardless of the value       */
/*  of `flaws'.                                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

void CTINClass::repairencs(int flaws)
{
	triEDGE enctri;
	triEDGE testtri;
	EDGE *encloop;
	EDGE testsh;
	triPOINT *eorg, *edest;
	triPOINT *newpoint;
	enum insertsiteresult success;
	REAL segmentlength, nearestpoweroftwo;
	REAL split;
	int acuteorg, acutedest;
	int dummy;

	while ((m_badSegments.GetNumberOfItems() > 0) && (steinerleft != 0)) {
		m_badSegments.TraversalInit();
		encloop = badsegmenttraverse();
		while ((encloop != (EDGE *) NULL) && (steinerleft != 0)) {
			/* To decide where to split a segment, we need to know if the  */
			/*   segment shares an endpoint with an adjacent segment.      */
			/*   The concern is that, if we simply split every encroached  */
			/*   segment in its center, two adjacent segments with a small */
			/*   angle between them might lead to an infinite loop; each   */
			/*   triPOINT added to split one segment will encroach upon the   */
			/*   other segment, which must then be split with a triPOINT that */
			/*   will encroach upon the first segment, and so on forever.  */
			/* To avoid CTINClass::this, imagine a set of concentric circles, whose   */
			/*   radii are powers of two, about each segment endpoint.     */
			/*   These concentric circles determine where the segment is   */
			/*   split.  (If both endpoints are shared with adjacent       */
			/*   segments, split the segment in the middle, and apply the  */
			/*   concentric shells for later splittings.)                  */
			
			/* Is the origin shared with another segment? */
			stpivot(*encloop, enctri);
			NextEdge(enctri, testtri);
			tspivot(testtri, testsh);
			acuteorg = testsh.sh != m_dummysh;
			/* Is the destination shared with another segment? */
			NextEdgeSelf(testtri);
			tspivot(testtri, testsh);
			acutedest = testsh.sh != m_dummysh;
			/* Now, check the other side of the segment, if there's a TRIANGLE */
			/*   there.                                                        */
			SymmTri(enctri, testtri);
			if (testtri.tri != m_dummytri) {
				/* Is the destination shared with another segment? */
				NextEdgeSelf(testtri);
				tspivot(testtri, testsh);
				acutedest = acutedest || (testsh.sh != m_dummysh);
				/* Is the origin shared with another segment? */
				NextEdgeSelf(testtri);
				tspivot(testtri, testsh);
				acuteorg = acuteorg || (testsh.sh != m_dummysh);
			}
			
			eorg = shOrg(*encloop );
			edest = shDest(*encloop );
			/* Use the concentric circles if exactly one endpoint is shared */
			/*   with another adjacent segment.                             */
			if (acuteorg ^ acutedest) {
				segmentlength = sqrt((edest->x - eorg->x) * (edest->x - eorg->x)
					+ (edest->y - eorg->y) * (edest->y - eorg->y));
				/* Find the power of two nearest the segment's length. */
				nearestpoweroftwo = 1.0;
				while (segmentlength > SQUAREROOTTWO * nearestpoweroftwo) 
					nearestpoweroftwo *= 2.0;
				
				while (segmentlength < (0.5 * SQUAREROOTTWO) * nearestpoweroftwo) 
					nearestpoweroftwo *= 0.5;
				/* Where do we split the segment? */
				split = 0.5 * nearestpoweroftwo / segmentlength;
				if (acutedest) 
					split = 1.0 - split;
			}
			else {
				/* If we're not worried about adjacent segments, split */
				/*   this segment in the middle.                       */
				split = 0.5;
			}
			
			/* Create the new triPOINT. */
			newpoint = (triPOINT *) PointAlloc();
			/* Interpolate its coordinate and attributes. */
			newpoint->x = (1.0 - split) * eorg->x + split * edest->x;
			newpoint->y = (1.0 - split) * eorg->y + split * edest->y;
			newpoint->attr =(float) ((1.0 - split) * eorg->attr + split * edest->attr);

			SetPointDup( newpoint, NULL);
			
			SetPointMark(newpoint, shMark(*encloop));
			if (m_verbose > 1) 
				printf(	"  Splitting EDGE (%.12g, %.12g) (%.12g, %.12g) at (%.12g, %.12g).\n",
				eorg->x, eorg->y, edest[0], edest[1], newpoint->x, newpoint->y);
			
			/* Check whether the new triPOINT lies on an endpoint. */
			if (((newpoint->x == eorg->x) && (newpoint->y == eorg->y) )
				|| ((newpoint->x == edest->x ) && (newpoint->y == edest->y ) )) {
				
				printf("Error:  Ran out of precision at (%.12g, %.12g).\n",newpoint->x, newpoint->y);
				printf("I attempted to split a segment to a smaller size than can\n");
				printf("  be accommodated by the finite precision of floating triPOINT\n"	);
				printf("  arithmetic.\n");
				precisionerror();
				exit(1);
			}
			
			/* Insert the splitting triPOINT.  This should always succeed. */
			success = InsertSite(newpoint, &enctri, encloop, flaws, flaws);
			if ((success != SUCCESSFULPOINT) && (success != ENCROACHINGPOINT)) {
				printf("Internal error in repairencs():\n");
				printf("  Failure to split a segment.\n");
				internalerror();
			}
			if (steinerleft > 0) 
				steinerleft--;
			/* Check the two new subsegments to see if they're encroached. */
			dummy = checkedge4encroach(encloop);
			shNextself(*encloop);
			dummy = checkedge4encroach(encloop);
			
			badsegmentdealloc(encloop);
			encloop = badsegmenttraverse();
		}
	}
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  tallyfaces()   Test every TRIANGLE in the mesh for quality measures.     */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

void CTINClass::tallyfaces()
{
	triEDGE triangleloop;
	
	if (m_verbose) {
		printf("  Making a list of bad m_triangles.\n");
	}
	m_triangles.TraversalInit();
	triangleloop.orient = 0;
	triangleloop.tri = TriangleTraverse();
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		/* If the TRIANGLE is bad, enqueue it. */
		testtriangle(&triangleloop);
		triangleloop.tri = TriangleTraverse();
	}
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  findcircumcenter()   Find the circumcenter of a TRIANGLE.                */
/*                                                                           */
/*  The result is returned both in terms of x-y coordinates and xi-eta       */
/*  coordinates.  The xi-eta coordinate system is defined in terms of the    */
/*  TRIANGLE:  the origin of the TRIANGLE is the origin of the coordinate    */
/*  system; the destination of the TRIANGLE is one unit along the xi axis;   */
/*  and the apex of the TRIANGLE is one unit along the eta axis.             */
/*                                                                           */
/*  The return value indicates which EDGE of the TRIANGLE is shortest.       */
/*                                                                           */
/*****************************************************************************/

enum circumcenterresult CTINClass::findcircumcenter(triPOINT * torg,triPOINT * tdest,triPOINT * tapex,REAL * circumcenter,REAL *xi,REAL *eta)
{
	REAL xdo, ydo, xao, yao, xad, yad;
	REAL dodist, aodist, addist;
	REAL denominator;
	REAL dx, dy;
	
	circumcentercount++;
	
	/* Compute the circumcenter of the TRIANGLE. */
	xdo = tdest->x - torg->x;
	ydo = tdest->y - torg->y;
	xao = tapex->x - torg->x;
	yao = tapex->y - torg->y;
	dodist = xdo * xdo + ydo * ydo;
	aodist = xao * xao + yao * yao;
	if (noexact) {
		denominator = 0.5 / (xdo * yao - xao * ydo);
	} else {
		/* Use the counterclockwise() routine to ensure a positive (and */
		/*   reasonably accurate) result, avoiding any possibility of   */
		/*   division by zero.                                          */
		denominator = 0.5 / counterclockwise(tdest, tapex, torg);
		/* Don't count the above as an orientation test. */
		counterclockcount--;
	}
	circumcenter[0] = torg->x - (ydo * aodist - yao * dodist) * denominator;  
	circumcenter[1] = torg->y + (xdo * aodist - xao * dodist) * denominator;  
	
	/* To interpolate triPOINT attributes for the new triPOINT inserted at  */
	/*   the circumcenter, define a coordinate system with a xi-axis, */
	/*   directed from the TRIANGLE's origin to its destination, and  */
	/*   an eta-axis, directed from its origin to its apex.           */
	/*   Calculate the xi and eta coordinates of the circumcenter.    */
	dx = circumcenter[0] - torg->x;
	dy = circumcenter[1] - torg->y;
	*xi = (dx * yao - xao * dy) * (2.0 * denominator);
	*eta = (xdo * dy - dx * ydo) * (2.0 * denominator);
	
	xad = tapex->x - tdest->x;
	yad = tapex->y - tdest->y;
	addist = xad * xad + yad * yad;
	if ((addist < dodist) && (addist < aodist)) {
		return OPPOSITEORG;
	} else if (dodist < aodist) {
		return OPPOSITEAPEX;
	} else {
		return OPPOSITEDEST;
	}
}

/*****************************************************************************/
/*                                                                           */
/*  splittriangle()   Inserts a triPOINT at the circumcenter of a TRIANGLE.     */
/*                    Deletes the newly inserted triPOINT if it encroaches upon */
/*                    a segment.                                             */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

void CTINClass::splittriangle(badFACE *badtri)
{
	triPOINT *borg, *bdest, *bapex;
	triPOINT *newpoint;
	REAL xi, eta;
	enum insertsiteresult success;
	enum circumcenterresult shortedge;
	int errorflag;
	
	borg = org(badtri->badfacetri);
	bdest = dest(badtri->badfacetri);
	bapex = apex(badtri->badfacetri);
	/* Make sure that this TRIANGLE is still the same TRIANGLE it was      */
	/*   when it was tested and determined to be of bad quality.           */
	/*   Subsequent transformations may have made it a different TRIANGLE. */
	if ((borg == badtri->faceorg) && (bdest == badtri->facedest) &&
		(bapex == badtri->faceapex)) {
		if (m_verbose > 1) {
			printf("  Splitting this TRIANGLE at its circumcenter:\n");
			printf("    (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n", borg->x,
				borg->y, bdest->x, bdest->y, bapex->x, bapex->y);
		}
		errorflag = 0;
		/* Create a new triPOINT at the TRIANGLE's circumcenter. */
		newpoint = (triPOINT *) PointAlloc();
		SetPointDup( newpoint, NULL );
		
		shortedge = findcircumcenter(borg, bdest, bapex, (REAL *)newpoint, &xi, &eta);
		/* Check whether the new triPOINT lies on a TRIANGLE vertex. */
		if (((newpoint->x == borg->x) && (newpoint->y == borg->y))
			|| ((newpoint->x == bdest->x) && (newpoint->y == bdest->y))
			|| ((newpoint->x == bapex->x) && (newpoint->y == bapex->y))) {
			if (!quiet) {
				printf("Warning:  New triPOINT (%.12g, %.12g) falls on existing vertex.\n"
					, newpoint->x, newpoint->y);
				errorflag = 1;
			}
			PointDealloc(newpoint);
		} else {
			/* Interpolate the triPOINT attributes at the circumcenter. */
			newpoint->x = borg->x + xi * (bdest->x - borg->x)
				+ eta * (bapex->x - borg->x);
			newpoint->y = borg->y + xi * (bdest->y - borg->y)
				+ eta * (bapex->y - borg->y);
			newpoint->attr = (float)(borg->attr + xi * (bdest->attr - borg->attr)
				+ eta * (bapex->attr - borg->attr));
			
			/* The new triPOINT must be in the interior, and have a marker of zero. */
			SetPointMark(newpoint, 0);
			/* Ensure that the handle `badtri->badfacetri' represents the shortest */
			/*   EDGE of the TRIANGLE.  This ensures that the circumcenter must    */
			/*   fall to the left of this EDGE, so triPOINT location will work.       */
			if (shortedge == OPPOSITEORG) {
				NextEdgeSelf(badtri->badfacetri);
			} else if (shortedge == OPPOSITEDEST) {
				PrevEdgeSelf(badtri->badfacetri);
			}
			/* Insert the circumcenter, searching from the EDGE of the TRIANGLE, */
			/*   and maintain the Delaunay property of the triangulation.        */
			success = InsertSite(newpoint, &(badtri->badfacetri),
				(EDGE *) NULL, 1, 1);
			if (success == SUCCESSFULPOINT) {
				if (steinerleft > 0) {
					steinerleft--;
				}
			}
			else if (success == ENCROACHINGPOINT) {
				/* If the newly inserted triPOINT encroaches upon a segment, delete it. */
				DeletesSite(&(badtri->badfacetri));
			}
			else if (success == VIOLATINGPOINT) {
				/* Failed to insert the new triPOINT, but some segment was */
				/*   marked as being encroached.                        */
				PointDealloc(newpoint);
			} 
			else {                                  /* success == DUPLICATEPOINT */
				/* Failed to insert the new triPOINT because a vertex is already there. */
				if (!quiet) {
					printf(
						"Warning:  New triPOINT (%.12g, %.12g) falls on existing vertex.\n"
						, newpoint->x, newpoint->y);
					errorflag = 1;
				}
				PointDealloc(newpoint);
			}
		}
		if (errorflag) {
			if (m_verbose) {
				printf("  The new triPOINT is at the circumcenter of TRIANGLE\n");
				printf("    (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
					borg->x, borg->y, bdest->x, bdest->y, bapex->x, bapex->y);
			}
			printf("This probably means that I am trying to refine m_triangles\n");
			printf("  to a smaller size than can be accommodated by the finite\n");
			printf("  precision of floating triPOINT arithmetic.  (You can be\n");
			printf("  sure of this if I fail to terminate.)\n");
			precisionerror();
		}
	}
	/* Return the bad TRIANGLE to the pool. */
	m_badTriangles.PoolDealloc( badtri);
}

#endif /* not CDT_ONLY */

/*****************************************************************************/
/*                                                                           */
/*  enforcequality()   Remove all the encroached edges and bad m_triangles     */
/*                     from the triangulation.                               */
/*                                                                           */
/*****************************************************************************/

#ifndef CDT_ONLY

void CTINClass::enforcequality()
{
	int i;
	
	if (!quiet) 
		printf("Adding Steiner m_points to enforce quality.\n");
	
	/* Initialize the pool of encroached segments. */
	m_badSegments.PoolInit( BADSEGMENTPERBLOCK );
	if (m_verbose) 
		printf("  Looking for encroached segments.\n");
	
	/* Test all segments to see if they're encroached. */
	tallyencs();
	if (m_verbose && (m_badSegments.GetNumberOfItems() > 0)) 
		printf("  Splitting encroached segments.\n");
	
	/* Note that steinerleft == -1 if an unlimited number */
	/*   of Steiner m_points is allowed.                    */
	while ((m_badSegments.GetNumberOfItems() > 0) && (steinerleft != 0)) {
		/* Fix the segments without noting newly encroached segments or   */
		/*   bad m_triangles.  The reason we don't want to note newly       */
		/*   encroached segments is because some encroached segments are  */
		/*   likely to be noted multiple times, and would then be blindly */
		/*   split multiple times.  I should fix that some time.          */
		repairencs(0);
		/* Now, find all the segments that became encroached while adding */
		/*   m_points to split encroached segments.                         */
		tallyencs();
	}
	/* At this triPOINT, if we haven't run out of Steiner m_points, the */
	/*   triangulation should be (conforming) Delaunay.            */
	
	/* Next, we worry about enforcing TRIANGLE quality. */
	if ((m_minangle > 0.0) || vararea || fixedarea) {
		/* Initialize the pool of bad m_triangles. */
		m_badTriangles.PoolInit( BADTRIPERBLOCK );
		/* Initialize the queues of bad m_triangles. */
		for (i = 0; i < 64; i++) {
			queuefront[i] = (badFACE *) NULL;
			queuetail[i] = &queuefront[i];
		}
		/* Test all m_triangles to see if they're bad. */
		tallyfaces();
		if (m_verbose) 
			printf("  Splitting bad m_triangles.\n");
		while ((m_badTriangles.GetNumberOfItems() > 0) && (steinerleft != 0)) {
			/* Fix one bad TRIANGLE by inserting a triPOINT at its circumcenter. */
			splittriangle(dequeuebadtri());
			/* Fix any encroached segments that may have resulted.  Record */
			/*   any new bad m_triangles or encroached segments that result. */
			if (m_badSegments.GetNumberOfItems() > 0) 
				repairencs(1);
		}
	}
	/* At this triPOINT, if we haven't run out of Steiner m_points, the */
	/*   triangulation should be (conforming) Delaunay and have no */
	/*   low-quality m_triangles.                                    */
	
	/* Might we have run out of Steiner m_points too soon? */
	if (!quiet && (m_badSegments.GetNumberOfItems() > 0) && (steinerleft == 0)) {
		printf("\nWarning:  I ran out of Steiner m_points, but the mesh has\n");
		if (m_badSegments.GetNumberOfItems() == 1) 
			printf("  an encroached segment, and therefore might not be truly\n");
		else 	printf("  %ld encroached segments, and therefore might not be truly\n",	m_badSegments.GetNumberOfItems());
		printf("  Delaunay.  If the Delaunay property is important to you,\n");
		printf("  try increasing the number of Steiner m_points (controlled by\n");
		printf("  the -S switch) slightly and try again.\n\n");
	}
}

#endif /* not CDT_ONLY */

/**                                                                         **/
/**                                                                         **/
/********* Mesh quality maintenance ends here                        *********/

/*****************************************************************************/
/*                                                                           */
/*  highorder()   Create extra nodes for quadratic subparametric elements.   */
/*                                                                           */
/*****************************************************************************/

void CTINClass::highorder()
{
	triEDGE triangleloop, trisym;
	EDGE checkmark;
	triPOINT *newpoint;
	triPOINT *torg, *tdest;
	
	if (!quiet) {
		printf("Adding vertices for second-m_order m_triangles.\n");
	}
	/* The following line ensures that dead items in the pool of nodes    */
	/*   cannot be allocated for the extra nodes associated with high     */
	/*   m_order elements.  This ensures that the primary nodes (at the     */
	/*   corners of elements) will occur earlier in the output files, and */
	/*   have lower indices, than the extra nodes.                        */
	m_points.NullDeadItemStack();
	
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	/* To loop over the set of edges, loop over all m_triangles, and look at   */
	/*   the three edges of each TRIANGLE.  If there isn't another TRIANGLE  */
	/*   adjacent to the EDGE, operate on the EDGE.  If there is another     */
	/*   adjacent TRIANGLE, operate on the EDGE only if the current TRIANGLE */
	/*   has a smaller pointer than its neighbor.  This way, each EDGE is    */
	/*   considered only once.                                               */
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		for (triangleloop.orient = 0; triangleloop.orient < 3;	triangleloop.orient++) {
			SymmTri(triangleloop, trisym);
			if ((triangleloop.tri < trisym.tri) || (trisym.tri == m_dummytri)) {
				torg = org(triangleloop);
				tdest = dest(triangleloop);
				/* Create a new node in the middle of the EDGE.  Interpolate */
				/*   its attributes.                                         */
				newpoint = (triPOINT *) PointAlloc();
				newpoint->x = 0.5 * (torg->x + tdest->x);
				newpoint->y = 0.5 * (torg->y + tdest->y);
				newpoint->attr = (float)(0.5 * (torg->attr + tdest->attr));
				SetPointDup( newpoint, NULL );
				
				/* Set the new node's marker to zero or one, depending on */
				/*   whether it lies on a boundary.                       */
				SetPointMark(newpoint, trisym.tri == m_dummytri);
				if (m_useShelles) {
					tspivot(triangleloop, checkmark);
					/* If this EDGE is a segment, transfer the marker to the new node. */
					if (checkmark.sh != m_dummysh) {
						SetPointMark(newpoint, shMark( checkmark ));
					}
				}
				if (m_verbose > 1) {
					printf("  Creating (%.12g, %.12g).\n", newpoint->x, newpoint->y);
				}
				/* Record the new node in the (one or two) adjacent elements. */
				triangleloop.tri->highorder[triangleloop.orient] = newpoint;
				if (trisym.tri != m_dummytri) {
					trisym.tri->highorder[trisym.orient] = newpoint;
				}
			}
		}
		triangleloop.tri = TriangleTraverse();
	}
}


/*****************************************************************************/
/*                                                                           */
/*  transfernodes()   Read the m_points from memory.                           */
/*                                                                           */
/*****************************************************************************/
void CTINClass::transfernodes(REAL *pointList,float *pointattriblist,long *pointMarkList,
						   int numOfPoints,int numberofpointattribs)
{
	triPOINT *pointloop;
	REAL x, y;
	int i;
	int coordindex;
	int attribindex;
	
	initializepointpool();

	mesh_dim = 2;
	nextras = numberofpointattribs;
	readnodefile = 0;
	
	if ( numOfPoints < 3) {
		printf("Error:  Input must have at least three input m_points.\n");
		exit(1);
	}
	
	/* Read the m_points. */
	coordindex = 0;
	attribindex = 0;
	for (i = 0; i < numOfPoints; i++) 
	{
		pointloop = (triPOINT *) PointAlloc();
		
		/* Read the triPOINT coordinates. */
		x = pointloop->x = pointList[coordindex++];
		y = pointloop->y = pointList[coordindex++];
		/* Read the triPOINT attributes. */
		pointloop->attr = pointattriblist[attribindex++];
		
		SetPointDup( pointloop, NULL );	/* duplicate triPOINT is itself */
		
		pointloop->tri = (TRIANGLE *)NULL;
		
		if( pointMarkList != NULL) 
			pointloop->marker = pointMarkList[i];
		else pointloop->marker= 0;
		
		x = pointloop->x;
		y = pointloop->y;
		/* Determine the smallest and largest x and y coordinates. */
	
		m_xmin = (x < m_xmin) ? x : m_xmin;
		m_xmax = (x > m_xmax) ? x : m_xmax;
		m_ymin = (y < m_ymin) ? y : m_ymin;
		m_ymax = (y > m_ymax) ? y : m_ymax;
	}
	
	/*
	printf(" m_xmin , = %.3f,%.3f,%.3f,%.3f\n", m_xmin,m_ymin,m_xmax,m_ymax);
	*/
	/* Nonexistent x value used as a flag to mark circle events in sweepline */
	/*   Delaunay algorithm.                                                 */
	m_xminextreme = 10 * m_xmin - 9 * m_xmax;
}


/*****************************************************************************/
/*                                                                           */
/*  writenodes()   Number the m_points and write them to a .node file.         */
/*                                                                           */
/*  To save memory, the triPOINT numbers are written over the shell markers     */
/*  after the m_points are written to a file.                                  */
/*                                                                           */
/*****************************************************************************/
void CTINClass::writenodes(REAL **pointList,float **pointattriblist,long **pointMarkList)
{
	REAL *plist;
	float *palist;
	long *pmlist;
	int coordindex;
	int attribindex;
	triPOINT *pointloop;
	int pointnumber;
	int i;
	
	if (!quiet) 
		printf("Writing m_points.\n");
	
	/* Allocate memory for output m_points if necessary. */
	if (*pointList == (REAL *) NULL) {
		*pointList = (REAL *) malloc(m_points.GetNumberOfItems() * 2 * sizeof(REAL));
		if (*pointList == (REAL *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	
	/* Allocate memory for output triPOINT attributes if necessary. */
	if ((nextras > 0) && (*pointattriblist == NULL)) {
		*pointattriblist = (float *) malloc(m_points.GetNumberOfItems() * nextras * sizeof(float));
		if (*pointattriblist == NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	/* Allocate memory for output triPOINT markers if necessary. */
	if (!nobound && (*pointMarkList == NULL)) {
		*pointMarkList = (long *) malloc(m_points.GetNumberOfItems() * sizeof(long));
		if (*pointMarkList == NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	plist = *pointList;
	palist = *pointattriblist;
	pmlist = *pointMarkList;
	coordindex = 0;
	attribindex = 0;
	
	m_points.TraversalInit();
	pointloop = PointTraverse();
	pointnumber = m_firstnumber;
	
	while (pointloop != NULL) {
		
		/* X and y coordinates. */
		plist[coordindex++] = pointloop->x;
		plist[coordindex++] = pointloop->y;
		
		SetPointMark( pointloop, pointnumber );
		
		/* Point attributes. */
		for (i = 0; i < nextras; i++) 
			palist[attribindex++] = pointloop->attr;
		
		if (!nobound) {
			/* Copy the boundary marker. */
			pmlist[pointnumber - m_firstnumber] = PointMark(pointloop);
		}
		
		// does not need, because of dup
		SetPointMark(pointloop, pointnumber);
		pointloop = PointTraverse();
		pointnumber++;
	}
}

/*****************************************************************************/
/*                                                                           */
/*  numbernodes()   Number the m_points.                                       */
/*                                                                           */
/*  Each triPOINT is assigned a marker equal to its number.                     */
/*                                                                           */
/*  Used when writenodes() is not called because no .node file is written.   */
/*                                                                           */
/*****************************************************************************/

void CTINClass::numbernodes()
{
	triPOINT *pointloop;
	int pointnumber;
	
	m_points.TraversalInit();
	pointloop = PointTraverse();
	pointnumber = m_firstnumber;
	while (pointloop != (triPOINT *) NULL) {
		SetPointMark(pointloop, pointnumber);
		pointloop = PointTraverse();
		pointnumber++;
	}
}

/*****************************************************************************/
/*                                                                           */
/*  writeelements()   Write the m_triangles to an .ele file.                   */
/*                                                                           */
/*****************************************************************************/
void CTINClass::writeelements(long **triList,float **triangleattriblist)
{
	long *tlist;
	float *talist;
	int pointindex;
	int attribindex;
	
	triEDGE triangleloop;
	triPOINT *p1, *p2, *p3;
	triPOINT *mid1, *mid2, *mid3;
	int elementnumber;
	int i;
	
	if (!quiet) 
		printf("Writing m_triangles.\n");
	
	/* Allocate memory for output m_triangles if necessary. */
	if (*triList == (long *) NULL) {
		*triList = (long *) malloc(m_triangles.GetNumberOfItems() *((m_order + 1) * (m_order + 2) / 2) * sizeof(long));
		if (*triList == (long *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	/* Allocate memory for output TRIANGLE attributes if necessary. */
	if ((eextras > 0) && (*triangleattriblist == NULL)) {
		*triangleattriblist = (float *) malloc(m_triangles.GetNumberOfItems() * eextras *sizeof(float));
		if (*triangleattriblist == NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	tlist = *triList;
	talist = *triangleattriblist;
	pointindex = 0;
	attribindex = 0;
	
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	triangleloop.orient = 0;
	elementnumber = m_firstnumber;
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		p1 = org(triangleloop );
		p2 = dest(triangleloop );
		p3 = apex(triangleloop );
		if (m_order == 1) {
			
			tlist[pointindex++] = PointMark(p1);
			tlist[pointindex++] = PointMark(p2);
			tlist[pointindex++] = PointMark(p3);
		} else {
			mid1 = (triPOINT *) triangleloop.tri->highorder[1];
			mid2 = (triPOINT *) triangleloop.tri->highorder[2];
			mid3 = (triPOINT *) triangleloop.tri->highorder[0];
			
			tlist[pointindex++] = PointMark(p1);
			tlist[pointindex++] = PointMark(p2);
			tlist[pointindex++] = PointMark(p3);
			tlist[pointindex++] = PointMark(mid1);
			tlist[pointindex++] = PointMark(mid2);
			tlist[pointindex++] = PointMark(mid3);
		}
		
		for (i = 0; i < eextras; i++) 
			talist[attribindex++] = elemattribute(triangleloop, i);
		triangleloop.tri = TriangleTraverse();
		elementnumber++;
	}
}

/*****************************************************************************/
/*                                                                           */
/*  writepoly()   Write the segments and holes to a .poly file.              */
/*                                                                           */
/*****************************************************************************/
void CTINClass::writepoly(long **segmentList,long **segMarkList)
{
	long *slist;
	long *smlist;
	long index;
	EDGE shelleloop;
	triPOINT *endpoint1, *endpoint2;
	int shellenumber;
	
	if (!quiet) 
		printf("Writing segments.\n");
	
	/* Allocate memory for output segments if necessary. */
	if (*segmentList == (long *) NULL) {
		*segmentList = (long *) malloc(m_shelles.GetNumberOfItems() * 2 * sizeof(long));
		if (*segmentList == (long *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	/* Allocate memory for output segment markers if necessary. */
	if (!nobound && (*segMarkList == NULL)) {
		*segMarkList = (long *) malloc(m_shelles.GetNumberOfItems() * sizeof(long));
		if (*segMarkList == NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}	
	}
	slist = *segmentList;
	smlist = *segMarkList;
	index = 0;
	
	m_shelles.TraversalInit();
	shelleloop.sh = shelleTraverse();
	shelleloop.shorient = 0;
	shellenumber = m_firstnumber;
	while (shelleloop.sh != (SHELLE *) NULL) {
		endpoint1 = shOrg(shelleloop );
		endpoint2 = shDest(shelleloop );
		
		/* Copy indices of the segment's two endpoints. */
		slist[index++] = PointMark(endpoint1);
		slist[index++] = PointMark(endpoint2);
		if (!nobound) {
			/* Copy the boundary marker. */
			smlist[shellenumber - m_firstnumber] = shMark(shelleloop);
		}	
		
		shelleloop.sh = shelleTraverse();
		shellenumber++;
	}
}

/*****************************************************************************/
/*                                                                           */
/*  writeedges()   Write the edges to a .EDGE file.                          */
/*                                                                           */
/*****************************************************************************/
void CTINClass::writeedges(long **edgeList,long **edgeMarkList)
{
	long *elist;
	long *emlist;
	long index;
	triEDGE triangleloop, trisym;
	EDGE checkmark;
	triPOINT *p1, *p2;
	long edgenumber;
	
	if (!quiet) 
		printf("Writing edges.\n");
	/* Allocate memory for edges if necessary. */
	if (*edgeList == (long *) NULL) {
		*edgeList = (long *) malloc(edges * 2 * sizeof(long));
		if (*edgeList == (long *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	/* Allocate memory for EDGE markers if necessary. */
	if (!nobound && (*edgeMarkList ==  NULL)) {
		*edgeMarkList = (long *) malloc(edges * sizeof(long));
		if (*edgeMarkList ==  NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	elist = *edgeList;
	emlist = *edgeMarkList;
	index = 0;
	
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	edgenumber = m_firstnumber;
	/* To loop over the set of edges, loop over all m_triangles, and look at   */
	/*   the three edges of each TRIANGLE.  If there isn't another TRIANGLE  */
	/*   adjacent to the EDGE, operate on the EDGE.  If there is another     */
	/*   adjacent TRIANGLE, operate on the EDGE only if the current TRIANGLE */
	/*   has a smaller pointer than its neighbor.  This way, each EDGE is    */
	/*   considered only once.                                               */
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		for (triangleloop.orient = 0; triangleloop.orient < 3;	triangleloop.orient++) 
		{
			SymmTri(triangleloop, trisym);
			if ((triangleloop.tri < trisym.tri) || (trisym.tri == m_dummytri)) {
				p1 = org(triangleloop );
				p2 = dest(triangleloop );
				elist[index++] = PointMark(p1);
				elist[index++] = PointMark(p2);
				if (nobound) {
				} else {
					/* Edge number, indices of two endpoints, and a boundary marker. */
					/*   If there's no shell EDGE, the boundary marker is zero.      */
					if (m_useShelles) {
						tspivot(triangleloop, checkmark);
						if (checkmark.sh == m_dummysh) 
							emlist[edgenumber - m_firstnumber] = 0;
						else 	emlist[edgenumber - m_firstnumber] = shMark(checkmark);
					}
					else 	emlist[edgenumber - m_firstnumber] = trisym.tri == m_dummytri;
				}
				edgenumber++;
			}
		}
		triangleloop.tri = TriangleTraverse();
	}
}

/*****************************************************************************/
/*                                                                           */
/*  writevoronoi()   Write the Voronoi diagram to a .v.node and .v.EDGE      */
/*                   file.                                                   */
/*                                                                           */
/*  The Voronoi diagram is the geometric dual of the Delaunay triangulation. */
/*  Hence, the Voronoi vertices are listed by traversing the Delaunay        */
/*  m_triangles, and the Voronoi edges are listed by traversing the Delaunay   */
/*  edges.                                                                   */
/*                                                                           */
/*  WARNING:  In m_order to assign numbers to the Voronoi vertices, this       */
/*  procedure messes up the shell edges or the extra nodes of every          */
/*  element.  Hence, you should call this procedure last!!!!!!               */
/*                                                                           */
/*																			 */
/*****************************************************************************/

void CTINClass::writevoronoi(REAL **vpointList,float **vpointattriblist,long **vpointMarkList,long ** vedgeList,
						  long **vedgeMarkList, REAL **vnormList)
{
	REAL *plist;
	float *palist;
	long *elist;
	REAL *normList;
	int coordindex;
	int attribindex;
	triEDGE triangleloop, trisym;
	triPOINT *torg, *tdest, *tapex;
	REAL circumcenter[2];
	REAL xi, eta;
	int vnodenumber, vedgenumber;
	int p1, p2;
	
	if (!quiet) 
		printf("Writing Voronoi vertices.\n");
	
	/* Allocate memory for Voronoi vertices if necessary. */
	if (*vpointList == (REAL *) NULL) {
		*vpointList = (REAL *) malloc(m_triangles.GetNumberOfItems() * 2 * sizeof(REAL));
		if (*vpointList == (REAL *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	/* Allocate memory for Voronoi vertex attributes if necessary. */
	if (*vpointattriblist == NULL) {
		*vpointattriblist = (float *) malloc(m_triangles.GetNumberOfItems() * nextras *sizeof(float));
		if (*vpointattriblist == (float *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	
	*vpointMarkList = NULL;
	plist = *vpointList;
	palist = *vpointattriblist;
	coordindex = 0;
	attribindex = 0;
	
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	triangleloop.orient = 0;
	vnodenumber = m_firstnumber;
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		torg = org(triangleloop);
		tdest = dest(triangleloop);
		tapex = apex(triangleloop);
		findcircumcenter(torg, tdest, tapex, circumcenter, &xi, &eta);
		
		/* X and y coordinates. */
		plist[coordindex++] = circumcenter[0];
		plist[coordindex++] = circumcenter[1];
		/* Interpolate the triPOINT attributes at the circumcenter. */
		palist[attribindex++] = (float) (torg->x + xi * (tdest->x - torg->x) + eta * (tapex->x - torg->x));
		palist[attribindex++] = (float)(torg->y + xi * (tdest->y - torg->y) + eta * (tapex->y - torg->y));
		palist[attribindex++] = (float)(torg->attr + xi * (tdest->attr - torg->attr) + eta * (tapex->attr - torg->attr));
		
		triangleloop.tri->node = vnodenumber;
		triangleloop.tri = TriangleTraverse();
		vnodenumber++;
	}
	
	
	if (!quiet) 
		printf("Writing Voronoi edges.\n");
	/* Allocate memory for output Voronoi edges if necessary. */
	if (*vedgeList == (long *) NULL) {
		*vedgeList = (long *) malloc(edges * 2 * sizeof(long));
		if (*vedgeList == (long *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	*vedgeMarkList = NULL;
	/* Allocate memory for output Voronoi norms if necessary. */
	if (*vnormList == (REAL *) NULL) {
		*vnormList = (REAL *) malloc(edges * 2 * sizeof(REAL));
		if (*vnormList == (REAL *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	elist = *vedgeList;
	normList = *vnormList;
	coordindex = 0;
	
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	vedgenumber = m_firstnumber;
	/* To loop over the set of edges, loop over all m_triangles, and look at   */
	/*   the three edges of each TRIANGLE.  If there isn't another TRIANGLE  */
	/*   adjacent to the EDGE, operate on the EDGE.  If there is another     */
	/*   adjacent TRIANGLE, operate on the EDGE only if the current TRIANGLE */
	/*   has a smaller pointer than its neighbor.  This way, each EDGE is    */
	/*   considered only once.                                               */
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		for (triangleloop.orient = 0; triangleloop.orient < 3;	triangleloop.orient++) 
		{
			SymmTri(triangleloop, trisym);
			if ((triangleloop.tri < trisym.tri) || (trisym.tri == m_dummytri)) {
				/* Find the number of this TRIANGLE (and Voronoi vertex). */
				p1 = * (long *) (triangleloop.tri + 6);
				if (trisym.tri == m_dummytri) {
					torg = org(triangleloop);
					tdest = dest(triangleloop);
					
					/* Copy an infinite ray.  Index of one endpoint, and -1. */
					elist[coordindex] = p1;
					normList[coordindex++] = tdest->y - torg->y;
					elist[coordindex] = -1;
					normList[coordindex++] = torg->x - tdest->x;
				} else {
					/* Find the number of the adjacent TRIANGLE (and Voronoi vertex). */
					p2 = * (long *) (trisym.tri + 6);
					/* Finite EDGE.  Write indices of two endpoints. */
					elist[coordindex] = p1;
					normList[coordindex++] = 0.0;
					elist[coordindex] = p2;
					normList[coordindex++] = 0.0;
				}
				vedgenumber++;
			}
		}
		triangleloop.tri = TriangleTraverse();
	}
}


void CTINClass::writeneighbors(long **neighborList)
{
	long *nlist;
	int index;
	triEDGE triangleloop, trisym;
	int elementnumber;
	int neighbor1, neighbor2, neighbor3;
	
	if (!quiet) 
		printf("Writing neighbors.\n");
	
	/* Allocate memory for neighbors if necessary. */
	if( *neighborList == (long *) NULL ) {
		*neighborList = (long *) malloc(m_triangles.GetNumberOfItems() * 3 * sizeof(long));
		if (*neighborList == (long *) NULL) {
			printf("Error:  Out of memory.\n");
			exit(1);
		}
	}
	nlist = *neighborList;
	index = 0;
	
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	triangleloop.orient = 0;
	elementnumber = m_firstnumber;
	
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		triangleloop.tri->node = elementnumber;		/* use area for node */
		triangleloop.tri = TriangleTraverse();
		elementnumber++;
	}
	m_dummytri->node = -1L;
	
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	elementnumber = m_firstnumber;
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		triangleloop.orient = 1;
		SymmTri(triangleloop, trisym);
		neighbor1 = trisym.tri->node;	/* old: use area for node */
		triangleloop.orient = 2;
		SymmTri(triangleloop, trisym);
		neighbor2 = trisym.tri->node;	/* old: use area for node */
		triangleloop.orient = 0;
		SymmTri(triangleloop, trisym);
		neighbor3 = trisym.tri->node;	/* old: use area for node */
		nlist[index++] = neighbor1;
		nlist[index++] = neighbor2;
		nlist[index++] = neighbor3;
		
		triangleloop.tri = TriangleTraverse();
		elementnumber++;
	}
}


/*****************************************************************************/
/*                                                                           */
/*  quality_statistics()   Print statistics about the quality of the mesh.   */
/*                                                                           */
/*****************************************************************************/

void CTINClass::quality_statistics()
{
	triEDGE triangleloop;
	triPOINT *p[3];
	REAL cossquaretable[8];
	REAL ratiotable[16];
	REAL dx[3], dy[3];
	REAL edgelength[3];
	REAL dotproduct;
	REAL cossquare;
	REAL triarea;
	REAL shortest, longest;
	REAL trilongest2;
	REAL smallestarea, biggestarea;
	REAL triminaltitude2;
	REAL minaltitude;
	REAL triaspect2;
	REAL worstaspect;
	REAL smallestangle, biggestangle;
	REAL radconst, degconst;
	int angletable[18];
	int aspecttable[16];
	int aspectindex;
	int tendegree;
	int acutebiggest;
	int i, ii, j, k;
	
	printf("Mesh quality statistics:\n\n");
	radconst = PI / 18.0;
	degconst = 180.0 / PI;
	for (i = 0; i < 8; i++) {
		cossquaretable[i] = cos(radconst * (REAL) (i + 1));
		cossquaretable[i] = cossquaretable[i] * cossquaretable[i];
	}
	for (i = 0; i < 18; i++) {
		angletable[i] = 0;
	}
	
	ratiotable[0]  =      1.5;      ratiotable[1]  =     2.0;
	ratiotable[2]  =      2.5;      ratiotable[3]  =     3.0;
	ratiotable[4]  =      4.0;      ratiotable[5]  =     6.0;
	ratiotable[6]  =     10.0;      ratiotable[7]  =    15.0;
	ratiotable[8]  =     25.0;      ratiotable[9]  =    50.0;
	ratiotable[10] =    100.0;      ratiotable[11] =   300.0;
	ratiotable[12] =   1000.0;      ratiotable[13] = 10000.0;
	ratiotable[14] = 100000.0;      ratiotable[15] =     0.0;
	for (i = 0; i < 16; i++) {
		aspecttable[i] = 0;
	}
	
	worstaspect = 0.0;
	minaltitude = m_xmax - m_xmin + m_ymax - m_ymin;
	minaltitude = minaltitude * minaltitude;
	shortest = minaltitude;
	longest = 0.0;
	smallestarea = minaltitude;
	biggestarea = 0.0;
	worstaspect = 0.0;
	smallestangle = 0.0;
	biggestangle = 2.0;
	acutebiggest = 1;
	
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	triangleloop.orient = 0;
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		p[0] = org(triangleloop);
		p[1] = dest(triangleloop);
		p[2] = apex(triangleloop);
		trilongest2 = 0.0;
		
		for (i = 0; i < 3; i++) {
			j = plus1mod3[i];
			k = minus1mod3[i];
			dx[i] = p[j]->x - p[k]->x;
			dy[i] = p[j]->y - p[k]->y;
			edgelength[i] = dx[i] * dx[i] + dy[i] * dy[i];
			if (edgelength[i] > trilongest2) {
				trilongest2 = edgelength[i];
			}
			if (edgelength[i] > longest) {
				longest = edgelength[i];
			}
			if (edgelength[i] < shortest) {
				shortest = edgelength[i];
			}
		}
		
		triarea = counterclockwise(p[0], p[1], p[2]);
		if (triarea < smallestarea) {
			smallestarea = triarea;
		}
		if (triarea > biggestarea) {
			biggestarea = triarea;
		}
		triminaltitude2 = triarea * triarea / trilongest2;
		if (triminaltitude2 < minaltitude) {
			minaltitude = triminaltitude2;
		}
		triaspect2 = trilongest2 / triminaltitude2;
		if (triaspect2 > worstaspect) {
			worstaspect = triaspect2;
		}
		aspectindex = 0;
		while ((triaspect2 > ratiotable[aspectindex] * ratiotable[aspectindex])
			&& (aspectindex < 15)) {
			aspectindex++;
		}
		aspecttable[aspectindex]++;
		
		for (i = 0; i < 3; i++) {
			j = plus1mod3[i];
			k = minus1mod3[i];
			dotproduct = dx[j] * dx[k] + dy[j] * dy[k];
			cossquare = dotproduct * dotproduct / (edgelength[j] * edgelength[k]);
			tendegree = 8;
			for (ii = 7; ii >= 0; ii--) {
				if (cossquare > cossquaretable[ii]) {
					tendegree = ii;
				}
			}
			if (dotproduct <= 0.0) {
				angletable[tendegree]++;
				if (cossquare > smallestangle) {
					smallestangle = cossquare;
				}
				if (acutebiggest && (cossquare < biggestangle)) {
					biggestangle = cossquare;
				}
			} else {
				angletable[17 - tendegree]++;
				if (acutebiggest || (cossquare > biggestangle)) {
					biggestangle = cossquare;
					acutebiggest = 0;
				}
			}
		}
		triangleloop.tri = TriangleTraverse();
	}
	
	shortest = sqrt(shortest);
	longest = sqrt(longest);
	minaltitude = sqrt(minaltitude);
	worstaspect = sqrt(worstaspect);
	smallestarea *= 2.0;
	biggestarea *= 2.0;
	if (smallestangle >= 1.0) {
		smallestangle = 0.0;
	} else {
		smallestangle = degconst * acos(sqrt(smallestangle));
	}
	if (biggestangle >= 1.0) {
		biggestangle = 180.0;
	} else {
		if (acutebiggest) {
			biggestangle = degconst * acos(sqrt(biggestangle));
		} else {
			biggestangle = 180.0 - degconst * acos(sqrt(biggestangle));
		}
	}
	
	printf("  Smallest area: %16.5g   |  Largest area: %16.5g\n",
		smallestarea, biggestarea);
	printf("  Shortest EDGE: %16.5g   |  Longest EDGE: %16.5g\n",
		shortest, longest);
	printf("  Shortest altitude: %12.5g   |  Largest aspect ratio: %8.5g\n\n",
		minaltitude, worstaspect);
	printf("  Aspect ratio histogram:\n");
	printf("  1.1547 - %-6.6g    :  %8d    | %6.6g - %-6.6g     :  %8d\n",
		ratiotable[0], aspecttable[0], ratiotable[7], ratiotable[8],
		aspecttable[8]);
	for (i = 1; i < 7; i++) {
		printf("  %6.6g - %-6.6g    :  %8d    | %6.6g - %-6.6g     :  %8d\n",
			ratiotable[i - 1], ratiotable[i], aspecttable[i],
			ratiotable[i + 7], ratiotable[i + 8], aspecttable[i + 8]);
	}
	printf("  %6.6g - %-6.6g    :  %8d    | %6.6g -            :  %8d\n",
		ratiotable[6], ratiotable[7], aspecttable[7], ratiotable[14],
		aspecttable[15]);
	printf("  (Triangle aspect ratio is longest EDGE divided by shortest altitude)\n\n");
	printf("  Smallest angle: %15.5g   |  Largest angle: %15.5g\n\n",
		smallestangle, biggestangle);
	printf("  Angle histogram:\n");
	for (i = 0; i < 9; i++) {
		printf("    %3d - %3d degrees:  %8d    |    %3d - %3d degrees:  %8d\n",
			i * 10, i * 10 + 10, angletable[i],
			i * 10 + 90, i * 10 + 100, angletable[i + 9]);
	}
	printf("\n");
}

/*****************************************************************************/
/*                                                                           */
/*  statistics()   Print all sorts of cool facts.                            */
/*                                                                           */
/*****************************************************************************/

void CTINClass::statistics()
{
	printf("\nStatistics:\n\n");
	printf("  Input m_points: %d\n", m_points.GetNumberOfItems() );
	if (refine) {
		printf("  Input m_triangles: %d\n", inelements);
	}
	if (m_poly) {
		printf("  Input segments: %d\n", insegments);
		if (!refine) {
			printf("  Input holes: %d\n", holes);
		}
	}
	
	printf("\n  Mesh m_points: %ld\n", m_points.GetNumberOfItems());
	printf("  Mesh m_triangles: %ld\n", m_triangles.GetNumberOfItems());
	printf("  Mesh edges: %ld\n", edges);
	if ( m_poly || refine) {
		printf("  Mesh boundary edges: %ld\n", hullsize);
		printf("  Mesh segments: %ld\n\n", m_shelles.GetNumberOfItems());
	} else {
		printf("  Mesh m_convex hull edges: %ld\n\n", hullsize);
	}
	if (m_verbose) {
		quality_statistics();
		printf("Memory allocation statistics:\n\n");
		printf("  Maximum number of m_points: %ld\n", m_points.GetMaxNumberOfItems());
		printf("  Maximum number of m_triangles: %ld\n", m_triangles.GetMaxNumberOfItems());
		if (m_shelles.GetMaxNumberOfItems() > 0) {
			printf("  Maximum number of segments: %ld\n", m_shelles.GetMaxNumberOfItems());
		}
		if (m_viri.GetMaxNumberOfItems() > 0) {
			printf("  Maximum number of m_viri: %ld\n", m_viri.GetMaxNumberOfItems());
		}
		if (m_badSegments.GetMaxNumberOfItems() > 0) {
			printf("  Maximum number of encroached segments: %ld\n",
				m_badSegments.GetMaxNumberOfItems());
		}
		if (m_badTriangles.GetMaxNumberOfItems() > 0) {
			printf("  Maximum number of bad m_triangles: %ld\n",
				m_badTriangles.GetMaxNumberOfItems());
		}
		if (m_splayNodes.GetMaxNumberOfItems() > 0) {
			printf("  Maximum number of splay tree nodes: %ld\n",
				m_splayNodes.GetMaxNumberOfItems());
		}
		printf("  Approximate heap memory use (bytes): %ld\n\n",
			m_points.GetMaxNumberOfItems() * sizeof(triPOINT)
			+ m_triangles.GetMaxNumberOfItems() * sizeof( TRIANGLE )
			+ m_shelles.GetMaxNumberOfItems() * sizeof( SHELLE )
			+ m_viri.GetMaxNumberOfItems() * sizeof( TRIANGLE *)
			+ m_badSegments.GetMaxNumberOfItems() * sizeof(EDGE)
			+ m_badTriangles.GetMaxNumberOfItems() * sizeof( badFACE)
			+ m_splayNodes.GetMaxNumberOfItems() * sizeof( splayNODE) );
		
		printf("Algorithmic statistics:\n\n");
		printf("  Number of incircle tests: %ld\n", incirclecount);
		printf("  Number of orientation tests: %ld\n", counterclockcount);
		if (hyperbolacount > 0) {
			printf("  Number of right-of-hyperbola tests: %ld\n",
				hyperbolacount);
		}
		if (circumcentercount > 0) {
			printf("  Number of circumcenter computations: %ld\n",
				circumcentercount);
		}
		if (circletopcount > 0) {
			printf("  Number of circle top computations: %ld\n",
				circletopcount);
		}
		printf("\n");
	}
}

/*****************************************************************************/
/*                                                                           */
/*  main() or triangulate()   Gosh, do everything.                           */
/*                                                                           */
/*  The sequence is roughly as follows.  Many of these steps can be skipped, */
/*  depending on the command line switches.                                  */
/*                                                                           */
/*  - Initialize constants and parse the command line.                       */
/*  - Read the m_points from a file and either                                 */
/*    - triangulate them (no -r), or                                         */
/*    - read an old mesh from files and reconstruct it (-r).                 */
/*  - Insert the PSLG segments (-p), and possibly segments on the m_convex     */
/*      hull (-c).                                                           */
/*  - Read the holes (-p), regional attributes (-pA), and regional area      */
/*      constraints (-pa).  Carve the holes and concavities, and spread the  */
/*      regional attributes and area constraints.                            */
/*  - Enforce the constraints on minimum angle (-q) and maximum area (-a).   */
/*      Also enforce the conforming Delaunay property (-q and -a).           */
/*  - Compute the number of edges in the resulting mesh.                     */
/*  - Promote the mesh's linear m_triangles to higher m_order elements (-o).     */
/*  - Write the output files and print the statistics.                       */
/*  - Check the consistency and Delaunay property of the mesh (-C).          */
/*                                                                           */
/*****************************************************************************/
void CTINClass::triangulate(char *triswitches,TinIO *in,TinIO *out,TinIO *vorout)
{
	REAL *holearray;                                        /* Array of holes. */
	REAL *regionarray;   /* Array of regional attributes and area constraints. */
	
	parsecommandline(1, &triswitches);
	
	transfernodes(in->pointList, in->pointAttrList, in->pointMarkList,
		in->numOfPoints, in->numOfPointAttrs);
	
#ifdef CDT_ONLY
	hullsize = delaunay();                          /* Triangulate the m_points. */
#else /* not CDT_ONLY */
	if (refine) {
		/* Read and reconstruct a mesh. */
		hullsize = reconstruct(in->triList, in->triAttrList,
			in->triAreaList, in->numOfTriangles,
			in->numOfCorners, in->numOfTriAttrs,
			in->segmentList, in->segMarkList,	in->numOfSegments);
	} 
	else 	hullsize = delaunay();        /* Triangulate the m_points. */
	
#endif /* not CDT_ONLY */
	
	/* Ensure that no triPOINT can be mistaken for a triangular bounding */
	/*   box triPOINT in InsertSite().                                   */
	m_infpoint1 = (triPOINT *) NULL;
	m_infpoint2 = (triPOINT *) NULL;
	m_infpoint3 = (triPOINT *) NULL;
	
	if (m_useShelles) {
		m_checksegments = true;                  /* Segments will be introduced next. */
		if (!refine) {
			/* Insert PSLG segments and/or m_convex hull segments. */
			insegments = formskeleton(in->segmentList, in->segMarkList,in->numOfSegments);
		}
	}
	
	if (m_poly) {
		holearray = in->holeList;
		holes = in->numOfHoles;
		regionarray = in->regionList;
		regions = in->numOfRegions;
		if (!refine) {
			/* Carve out holes and concavities. */
			CarveHoles(holearray, holes, regionarray, regions);
		}
	} else {
		/* Without a PSLG, there can be no holes or regional attributes   */
		/*   or area constraints.  The following are set to zero to avoid CTINClass::*/
		/*   an accidental free() later.                                  */
		holes = 0;
		regions = 0;
	}
	
#ifndef CDT_ONLY
	if (quality) 
		enforcequality();                 /* Enforce angle and area constraints. */
#endif /* not CDT_ONLY */
	
	/* Compute the number of edges. */
	edges = (3l * m_triangles.GetNumberOfItems() + hullsize) / 2l;
	
	if (m_order > 1) 
		highorder();             /* Promote elements to higher polynomial m_order. */
	if (!quiet) 
		printf("\n");
	
	out->numOfPoints = m_points.GetNumberOfItems();
	out->numOfPointAttrs = nextras;
	out->numOfTriangles = m_triangles.GetNumberOfItems();
	out->numOfCorners = (m_order + 1) * (m_order + 2) / 2;
	out->numOfTriAttrs = eextras;
	out->numOfEdges = edges;
	
	if (m_useShelles) 
		out->numOfSegments = m_shelles.GetNumberOfItems();
	else 	out->numOfSegments = hullsize;
	
	if (vorout != (TinIO *) NULL) {
		vorout->numOfPoints = m_triangles.GetNumberOfItems();
		vorout->numOfPointAttrs = nextras;
		vorout->numOfEdges = edges;
	}
	
	/* If not using iteration numbers, don't write a .node file if one was */
	/*   read, because the original one would be overwritten!              */
	if (nonodewritten || (noiterationnum && readnodefile)) {
		if (!quiet) 
			printf("NOT writing m_points.\n");
		numbernodes();                 /* We must remember to number the m_points. */
	} 
	else 	writenodes(&out->pointList, &out->pointAttrList,&out->pointMarkList);
	
	if (noelewritten) {
		if (!quiet) 
			printf("NOT writing m_triangles.\n");
	} else 		writeelements(&out->triList, &out->triAttrList);
	
	/* The -c switch (m_convex switch) causes a PSLG to be written */
	/*   even if none was read.                                  */
	if ( m_poly || m_convex) {
		/* If not using iteration numbers, don't overwrite the .poly file. */
		if (nopolywritten || noiterationnum) {
			if (!quiet) 
				printf("NOT writing segments.\n");
		} 
		else {
			writepoly(&out->segmentList, &out->segMarkList);
			out->numOfHoles = holes;
			out->numOfRegions = regions;
			if (m_poly) {
				out->holeList = in->holeList;
				out->regionList = in->regionList;
			}
			else {
				out->holeList = (REAL *) NULL;
				out->regionList = (REAL *) NULL;
			}
		}
	}
	
	if (edgesout) 
		writeedges(&out->edgeList, &out->edgeMarkList);
	
	if (voronoi) {
		writevoronoi(&vorout->pointList, &vorout->pointAttrList,
			&vorout->pointMarkList, &vorout->edgeList,
			&vorout->edgeMarkList, &vorout->normList);
	}
	if (neighbors) 
		writeneighbors(&out->neighborList);
	
	if (!quiet) {
		statistics();
	}
	
#ifndef REDUCED
	if (docheck) {
		checkmesh();
		checkdelaunay();
	}
#endif /* not REDUCED */
	
	/* tri2point to make triPOINT map for triPOINT location */
	MakePointMap();

}


///////////////////////////////////////////////////////////////////////
// new functions added by JWS
///////////////////////////////////////////////////////////////////////

void CTINClass::SaveTIN(const char *fileName, double xOff, double yOff)
{
	FILE *fp;
	fp = fopen(fileName, "wt");
	if( fp == NULL ) {
		AfxMessageBox("Save TIN : Can not create TIN file");
		return ;
	}

	/////////////////////////////////////////
	int pointnumber=0, elementnumber=0, edgenumber=0;

	fprintf(fp, "%9ld %9ld %9ld\n", pointnumber, elementnumber, edgenumber);
	//////////////////////////////////////////////////
	triPOINT *pointloop;
	
	m_points.TraversalInit();
	pointloop = PointTraverse();
	pointnumber = m_firstnumber;

	while ( pointloop != NULL ) {
		if( PointDup( pointloop ) == NULL ) {
			fprintf(fp, "%15.3lf %15.3lf %15.4f \n", 
				xOff + pointloop->x, yOff + pointloop->y, pointloop->attr );
			
			SetPointMark( pointloop, pointnumber );
			pointnumber++;	
		}
		pointloop = PointTraverse();
	}

	fprintf(fp, "\n");
	////////////////////////////////////////////////
	triEDGE triangleloop;

	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	elementnumber = m_firstnumber;
	while (triangleloop.tri != (TRIANGLE *) NULL) {		
		triangleloop.tri->node = elementnumber++;
		triangleloop.tri = TriangleTraverse();
	}

	////////////////////////////////////////////////////
	triEDGE trisym;
	int neighbor1, neighbor2, neighbor3;
	triPOINT *p1, *p2, *p3;
	
	m_dummytri->node = -1L;
	
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	elementnumber = m_firstnumber;
	while (triangleloop.tri != (TRIANGLE *) NULL) {
		triangleloop.orient = 0;

		p1 = org(triangleloop );
		p2 = dest(triangleloop );
		p3 = apex(triangleloop );

		fprintf(fp, "%8ld %8ld %8ld ", 
			PointMark(p1), PointMark(p2), PointMark(p3));

		triangleloop.orient = 1;
		SymmTri(triangleloop, trisym);
		neighbor1 = trisym.tri->node;
		triangleloop.orient = 2;
		SymmTri(triangleloop, trisym);
		neighbor2 = trisym.tri->node;
		triangleloop.orient = 0;
		SymmTri(triangleloop, trisym);
		neighbor3 = trisym.tri->node;

		fprintf(fp, "%8ld %8ld %8ld \n", neighbor1, neighbor2, neighbor3 );
		
		triangleloop.tri = TriangleTraverse();
		elementnumber++;
	}
	fprintf(fp, "\n");

	//////////////////////////////////////////////
	m_triangles.TraversalInit();
	triangleloop.tri = TriangleTraverse();
	edgenumber = m_firstnumber;

	while (triangleloop.tri != (TRIANGLE *) NULL) {
		for (triangleloop.orient = 0; triangleloop.orient < 3;	triangleloop.orient++) 
		{
			SymmTri(triangleloop, trisym);
			if ((triangleloop.tri < trisym.tri) || (trisym.tri == m_dummytri)) {
				p1 = org(triangleloop );
				p2 = dest(triangleloop );
				fprintf(fp, "%8ld %8ld\n", PointMark(p1), PointMark(p2));
				edgenumber++;
			}
		}
		triangleloop.tri = TriangleTraverse();
	}

	///////////////////////////////////

	rewind( fp );
	fprintf(fp, "%9ld %9ld %9ld\n", pointnumber, elementnumber, edgenumber);	

	fclose( fp );
}


////////////////////////////////////////////////////////////////


CTINClass::CTINClass(char *triswitches)
{
	m_dummytribase = NULL;
	m_dummyshbase = NULL;

	parsecommandline(1, &triswitches);

	initializepointpool();
	triangleinit();

	m_checksegments = false;

	m_xmin = m_ymin =  9999999999.0;
	m_xmax = m_ymax = -9999999999.0;

	m_xMinSel = m_yMinSel = -9999999999.0;
	m_xMaxSel = m_yMaxSel =  9999999999.0;

	m_numberOfSelectedTriangles = 0;
	m_maxNumberOfSelectedTriangles = 0;

	m_selectedTriangles = NULL;

	m_selectedChanged = false;

	m_segmentIntersectionEnabled = false;
};


CTINClass::~CTINClass()
{
	triangledeinit();

	if( m_selectedTriangles )
		delete m_selectedTriangles;
};



/////////////////////////////////////////////////////////////
void CTINClass::BeginAddPoints()
{
	initializepointpool();
	m_xmin = m_ymin =  9999999999.0;
	m_xmax = m_ymax = -9999999999.0;
}


triPOINT *CTINClass::AddPoint( double x, double y, float attr, long marker)
{
	triPOINT *pointloop = PointAlloc();
	
	pointloop->x = x;
	pointloop->y = y;
	pointloop->attr = attr;
	SetPointDup( pointloop, NULL );

	pointloop->marker= marker;

	m_xmin = (x < m_xmin) ? x : m_xmin;
	m_xmax = (x > m_xmax) ? x : m_xmax;
	m_ymin = (y < m_ymin) ? y : m_ymin;
	m_ymax = (y > m_ymax) ? y : m_ymax;

	return pointloop;
}


void CTINClass::EndAddPoints()
{
	m_xminextreme = 10 * m_xmin - 9 * m_xmax;
}


//////////////////////////////////////////

void CTINClass::FastConstruct()
{
	eextras = 0;

	InitializeTriSegPools();
	
	hullsize = divconqdelaunay();
	
	/* Ensure that no triPOINT can be mistaken for a triangular bounding */
	/*   box triPOINT in InsertSite().                                   */
	m_infpoint1 = (triPOINT *) NULL;
	m_infpoint2 = (triPOINT *) NULL;
	m_infpoint3 = (triPOINT *) NULL;

	MakePointMap();
}



///////////////////////////////////////
// Get triangles in selected range
///////////////////////////////////////

TRIANGLE **CTINClass::SelectTriangles( long *numOfTriangles, REAL xMinSel, REAL yMinSel, REAL xMaxSel, REAL yMaxSel)
{
	long i;
	REAL x,y;

	if( xMinSel == yMinSel && xMaxSel == yMaxSel )
	{	// whole range
		if( m_selectedTriangles && GetNumberOfTriangles() > m_maxNumberOfSelectedTriangles-1 ) {
			delete m_selectedTriangles;
			m_selectedTriangles = NULL;
		}

		m_xMinSel = m_yMinSel = -9999999999.0;
		m_xMaxSel = m_yMaxSel =  9999999999.0;

		if( m_selectedTriangles == NULL ) {
			m_maxNumberOfSelectedTriangles = GetNumberOfTriangles() + 1;
			m_selectedTriangles = new TRIANGLE *[m_maxNumberOfSelectedTriangles];
		}
	
		m_numberOfSelectedTriangles = 0;

		TRIANGLE **theTri = m_selectedTriangles;

		TriangleTraversalInit();
		*theTri = TriangleTraverse();
		while( *theTri  ) {
			m_numberOfSelectedTriangles++;
			theTri++;
			*theTri = TriangleTraverse();
		}
	}
	else if( (xMinSel == m_xMinSel && yMinSel == m_yMinSel && 
		xMaxSel == m_xMaxSel && yMaxSel == m_yMaxSel) && !m_selectedChanged ) {
		// no change of range
		// check validation of triangles

		/*
		for( i=0; i<m_numberOfSelectedTriangles; i++)
		{
			if( m_selectedTriangles[i]->vertex[0] == NULL ) {
				m_numberOfSelectedTriangles--;
				memmove( m_selectedTriangles+i, m_selectedTriangles+1, m_numberOfSelectedTriangles - i);
			}
		}
		*/
	}
	else	{	// select
		m_xMinSel = xMinSel; m_yMinSel = yMinSel; 
		m_xMaxSel = xMaxSel; m_yMaxSel = yMaxSel;

		if( m_selectedTriangles && GetNumberOfTriangles() > m_maxNumberOfSelectedTriangles-1 ) {
			delete m_selectedTriangles;
			m_selectedTriangles = NULL;
		}
		if( m_selectedTriangles == NULL ) {
			m_maxNumberOfSelectedTriangles = GetNumberOfTriangles() + 1;
			m_selectedTriangles = new TRIANGLE *[m_maxNumberOfSelectedTriangles];
		}
	
		m_numberOfSelectedTriangles = 0;

		TRIANGLE **theTri = m_selectedTriangles;

		TriangleTraversalInit();
		*theTri = TriangleTraverse();
		while( *theTri  ) {
			for( i =0; i<3; i++)
			{
				x = (*theTri)->vertex[i]->x;
				y = (*theTri)->vertex[i]->y;
				if( x > xMinSel && x < xMaxSel && y > yMinSel && y < yMaxSel ) {
					m_numberOfSelectedTriangles++;
					theTri++;
					break;
				}
			}
			*theTri = TriangleTraverse();
		}
	}

	m_selectedChanged = false;
	*numOfTriangles = m_numberOfSelectedTriangles;
	return m_selectedTriangles;
}



/*****************************************************************************/
/*  PointAlloc()   allocate space for a triPOINT,                           */
//*****************************************************************************/
triPOINT *CTINClass::PointAlloc()
{	
	triPOINT *thePoint = m_points.PoolAlloc();
	// alloc extra attributes
	thePoint->extraAttr = AttributeAlloc();

	thePoint->tri = m_dummytri;

	return thePoint;
}
	
/*****************************************************************************/
/*  PointDealloc()   Deallocate space for a triPOINT, marking it dead.          */
//*****************************************************************************/
void CTINClass::PointDealloc(triPOINT *dyingpoint)
{
	/* Mark the triPOINT as dead.  This makes it possible to detect dead m_points */
	/*   when traversing the list of all m_points.                             */
	SetPointMark(dyingpoint, DEADPOINT);

	//dellaoc extra attributes
	AttributeDealloc( dyingpoint->extraAttr );

	m_points.PoolDealloc( dyingpoint );
}

	// for changeable attributes, it may be structure realized in derived clas
void *CTINClass::AttributeAlloc()
{
	return NULL;
}

void CTINClass::AttributeDealloc( void *dyingAttr )
{
}





///////////////////////////////////////////////////////////////////
void CTINClass::InsertSegment(triPOINT *endPoint1, triPOINT *endPoint2, int boundmarker)
{
	if( endPoint1 == NULL || endPoint2 == NULL )
		return ;

	if ((endPoint1->x != endPoint2->x) || (endPoint1->y != endPoint2->y)) 
		insertsegment(endPoint1, endPoint2, boundmarker);
}


void CTINClass::InsertSegments(triPOINT **ends, int numberOfSegments, int boundmarker)
{
	int i;
	triPOINT *endPoint1, *endPoint2;

	for( i=0; i < numberOfSegments; i++)
	{
		endPoint1 = *ends++;
		endPoint2 = *ends++;

		if ((endPoint1->x != endPoint2->x) || (endPoint1->y != endPoint2->y)) 
			insertsegment(endPoint1, endPoint2, boundmarker);
	}
}

void CTINClass::MarkHull()
{ 	
	if( m_convex || !m_poly ) 
		markhull(); 
};





/*****************************************************************************/
/*                                                                           */
/*  RemoveShelle()   remove a new shell EDGE between two triangles.			 */ 
/*                                                                           */
/*	triEDGE *tri:  Edge at which to remove the shell EDGE.					 */
/*																			 */
/*****************************************************************************/
bool CTINClass::NeedFlip(triEDGE &flipedge )
{
	triEDGE fartri;
	triPOINT *leftpoint, *rightpoint, *nearpoint, *farpoint;
	EDGE edge;

	if( flipedge.tri == m_dummytri ) 
		return false;

	tspivot( flipedge, edge);
	if ( edge.sh != m_dummysh) {
		return false;
	}

	SymmTri( flipedge, fartri);

	if( fartri.tri == m_dummytri ) 
		return false;

	/* Find all the relevant vertices. */
	leftpoint = org( flipedge );
	rightpoint = dest( flipedge );
	nearpoint = apex( flipedge );
	farpoint = apex( fartri );

	if( incircle( leftpoint,rightpoint, nearpoint, farpoint) > 0 )
		return true;
	else return false;
}


bool CTINClass::IsInverted(triEDGE &tri)
{
	triPOINT *p1, *p2, *p3;

	if( tri.tri == m_dummytri ) 
		return false;

	p1 = org( tri );
	p2 = dest( tri );
	p3 = apex( tri );

	if( counterclockwise(p1, p2, p3) < 0 )
		return true;
	else
		return false;
}


int CTINClass::DelaunayFixupForRemoveSegment(triEDGE &fixuptri, triPOINT *checkpoint, bool leftSide )
{
	triEDGE neartri, fartri;
	EDGE nearedge;
	triPOINT *nearpoint, *leftpoint, *rightpoint, *farpoint;
	int changed = 0;

	if( checkpoint == NULL )
		return 0;

	while( 1 ) {
		if( fixuptri .tri == m_dummytri ) {
			return changed;
		}

		NextEdge( fixuptri, neartri);
		SymmTri(neartri , fartri);

		if( fartri.tri == m_dummytri ) {
			return changed;
		}

		tspivot(fartri, nearedge);
		if ( nearedge.sh != m_dummysh) {
			return changed;
		}

		/* Find all the relevant vertices. */
		leftpoint = dest( neartri );
		rightpoint = org( neartri );
		nearpoint = apex( neartri );
		farpoint = apex( fartri );	// should not be null

		if( counterclockwise(leftpoint, checkpoint, farpoint ) <= 0 )
			break;
		if( counterclockwise(rightpoint, farpoint, checkpoint ) <= 0 )
			break;

/*		if( leftSide ) {
			if( counterclockwise(leftpoint, checkpoint, farpoint ) <= 0 )
				break;
		}
		else	{
			if( counterclockwise(checkpoint, rightpoint, farpoint ) <= 0 )
				break;
		}
*/
		if( nearpoint == farpoint ) 
			break;

		if( counterclockwise(leftpoint, rightpoint, farpoint ) > 0.0 ) {
			if( incircle( leftpoint, rightpoint, farpoint, checkpoint ) <= 0.0 ) 
				break;
		}
		else {
			if( incircle( leftpoint, rightpoint, farpoint, checkpoint ) >= 0.0 ) 
				break;
		}

		flip(&fartri);
		if( leftSide )
			oPrevSpinEdge( fartri, fixuptri);
		else
			TriEdgeCopy( fartri, fixuptri);

		changed++;
	}
	return changed;
}



void CTINClass::UnConstrainedEdge( triEDGE &fixuptri )
{
	triEDGE fixuptri2;
	EDGE leftedge, fixupedge, rightedge;
	triPOINT *endpoint1,  *endpoint2;
	triPOINT *farpoint = NULL;
	triPOINT *p0, *p1, *p2, *p3, *p4;
	triEDGE lefttri, righttri;//, opptri;
	int changed=0;
	
	endpoint1 = org( fixuptri );
	endpoint2 = dest( fixuptri );

	if( !NeedFlip( fixuptri ) ) return ;

	oPrevSpinEdge( fixuptri, fixuptri2 );

	do	{
		changed=0;

		dPrevSpinEdge( fixuptri, lefttri );
		dPrevSpinEdge( fixuptri2, righttri );

		tspivot(lefttri, leftedge);
		tspivot(righttri, rightedge);

		p0 = dest( fixuptri );
		p1 = dest( fixuptri2 );
		p3 = apex( fixuptri );

		if( rightedge.sh != m_dummysh )
			p2 = apex( righttri );
		else p2 = NULL;

		if ( leftedge.sh != m_dummysh  ) 
			p4 = apex(lefttri );
		else p4 = NULL;

		changed += DelaunayFixupForRemoveSegment( fixuptri, p1, true );
		changed += DelaunayFixupForRemoveSegment( fixuptri, p2,  true );

		changed += DelaunayFixupForRemoveSegment( fixuptri2, p3, false );
		changed += DelaunayFixupForRemoveSegment( fixuptri2, p4, false );
	}	while( changed > 0 );

	flip( &fixuptri );	
	PrevEdgeSelf( fixuptri );

	int times = 10000;

	for( int i=0; i < times; i++)
	{
		oNextSpinEdge( fixuptri, lefttri );
		oPrevSpinEdge( fixuptri, righttri );

		if( !IsInverted( lefttri ) && !IsInverted(righttri)  ) {
			if( !NeedFlip( lefttri ) && !NeedFlip( fixuptri ) ) {
				break;
			}
		}

		do	{
			changed=0;

			tspivot(lefttri, leftedge);
			tspivot(fixuptri, fixupedge);

			if ( fixupedge.sh == m_dummysh  ) 
				p1 = dest( righttri );
			else p1 = NULL;

			if( leftedge.sh == m_dummysh )
				p3 = apex( lefttri );
			else p3 = NULL;

			p2 = dest( fixuptri );	// should not be NULL
			p4 = apex( fixuptri );	// should not be NULL

			changed += DelaunayFixupForRemoveSegment( lefttri, p1, true );
			changed += DelaunayFixupForRemoveSegment( lefttri, p2, true );

			changed += DelaunayFixupForRemoveSegment( righttri, p3, false );
			changed += DelaunayFixupForRemoveSegment( righttri, p4, false );
		}	while( changed > 0 );

		if( p1 != NULL && p3 != NULL ) {	
			if( incircle( p1, p2, p4, p3 ) > 0 ) {
				flip( &lefttri );
				PrevEdge( lefttri, fixuptri );
			}
			else	{
				flip( &fixuptri );
				PrevEdgeSelf( fixuptri );
			}
		}
		else if( p3 != NULL ) {	// righttri outside
			flip( &lefttri );
			PrevEdge( lefttri, fixuptri );
		}
		else	{				// leftttri outside
			flip( &fixuptri );
			PrevEdgeSelf( fixuptri );
		}

		m_fixuptri = fixuptri;
	}
}



void CTINClass::RemoveShelle(triEDGE *tri)
{
	triEDGE oppotri;
	EDGE theShelle, adjoinShelle;

	/* Check if there's already a shell EDGE here. */
	if( tri->tri == m_dummytri )
		return ;

	tspivot(*tri, theShelle);
	if( theShelle.sh != m_dummysh ) {
		/* disBond shell EDGE from the two triangles it is sandwiched between. */
		/*   Note that the facing TRIANGLE `oppotri' might be equal to        */
		/*   `m_dummytri' (outer space), but the new shell EDGE is bonded to it */
		/*   all the same.*/

		// can not remove boundary
		if( theShelle.sh->tri[0] ==  m_dummytri || theShelle.sh->tri[1] ==  m_dummytri )
			return ;

		tshDissolve( *tri ); 
		SymmTri(*tri, oppotri);
		tshDissolve( oppotri );

		for( int i =0;  i < 2; i++)
		{
			if( theShelle.sh->adjoin[i] != m_dummysh ) {
				shDecode( theShelle.sh->adjoin[i], adjoinShelle );
				shDissolve( adjoinShelle );
			}
		}
		shelleDealloc( theShelle.sh);

		UnConstrainedEdge( *tri );
	}
}


/*****************************************************************************/
/*  DeScout : 反侦察                                                                         */
/*  DeScoutSegment()   Scout the first TRIANGLE on the path from one endpoint  */
/*                   to another, and check for completion (reaching the      */
/*                   second endpoint), a collinear triPOINT, and the            */
/*                   intersection of two segments.                           */
/*                                                                           */
/*  Returns one if the entire segment is successfully inserted, and zero if  */
/*  the job must be finished by conformingedge() or constrainededge().       */
/*                                                                           */
/*  If the first TRIANGLE on the path has the second endpoint as its         */
/*  destination or apex, a shell EDGE is inserted and the job is done.       */
/*                                                                           */
/*  If the first TRIANGLE on the path has a destination or apex that lies on */
/*  the segment, a shell EDGE is inserted connecting the first endpoint to   */
/*  the collinear triPOINT, and the search is continued from the collinear      */
/*  triPOINT.                                                                   */
/*                                                                           */
/*  If the first TRIANGLE on the path has a shell EDGE opposite its origin,  */
/*  then there is a segment that intersects the segment being inserted.      */
/*  Their intersection triPOINT is inserted, splitting the shell EDGE.          */
/*                                                                           */
/*  Otherwise, return zero.                                                  */
/*                                                                           */
/*****************************************************************************/
bool CTINClass::DeScoutSegment(triEDGE*searchtri, triPOINT *endpoint2)
{
	triEDGE opptri;
	triPOINT *leftpoint, *rightpoint;
	enum finddirectionresult collinear;
	
	collinear = finddirection(searchtri, endpoint2);
	rightpoint = dest(*searchtri);
	leftpoint = apex(*searchtri);

	if( ((leftpoint->x == endpoint2->x) && (leftpoint->y == endpoint2->y) ) ||
		((rightpoint->x == endpoint2->x) && (rightpoint->y == endpoint2->y)) ) {
		/* The segment is already an EDGE in the mesh. */
		if ((leftpoint->x == endpoint2->x) && (leftpoint->y == endpoint2->y)) {
			oNextSpinEdge( *searchtri, opptri);
			RemoveShelle( &opptri );
		}
		else
			RemoveShelle( searchtri );
		return 1;
	} else if (collinear == LEFTCOLLINEAR) {
		/* We've collided with a triPOINT between the segment's endpoints. */
		/* Make the collinear triPOINT be the TRIANGLE's origin. */
		oPrevSpinEdge( *searchtri, opptri);
		RemoveShelle( &opptri );

		PrevEdgeSelf( *searchtri );

		/* Insert the remainder of the segment. */
		return DeScoutSegment(searchtri, endpoint2 );
	} else if (collinear == RIGHTCOLLINEAR) {
		/* We've collided with a triPOINT between the segment's endpoints. */
		RemoveShelle( searchtri );
		/* Make the collinear triPOINT be the TRIANGLE's origin. */
		NextEdgeSelf( *searchtri );
		/* Insert the remainder of the segment. */
		return DeScoutSegment(searchtri, endpoint2 );
	} else {
		printf(" temp to remove a unexisting edge\n");
		return 1;
	}
}



void CTINClass::RemoveSegment(triPOINT *endPoint1, triPOINT *endPoint2)
{
	triEDGE searchtri1;

	if( endPoint1 == NULL || endPoint2 == NULL )
		return ;

	if( PointDup ( endPoint1 ) != NULL ) 
		endPoint1 = PointDup ( endPoint1 );

	if( PointDup ( endPoint2 ) != NULL ) 
		endPoint2 = PointDup ( endPoint2 );

	/* Find a TRIANGLE whose origin is the segment's first endPoint. */
	LocatePoint( endPoint1, searchtri1);

	/* Scout the beginnings of a path from the first endPoint */
	/*   toward the second.                                   */
	/* The segment should be easily removed. */
	DeScoutSegment( &searchtri1, endPoint2);
}

////////////////////////////////////////////////////////////////////

triPOINT *CTINClass::InsertPoint(double x, double y, float attr, long marker)
{
	triPOINT *insertPoint = PointAlloc();
	
	insertPoint->x = x;
	insertPoint->y = y;
	insertPoint->attr = attr;
	SetPointDup( insertPoint, NULL );

	insertPoint->marker= marker;

	triEDGE	searchtri;

	searchtri.tri =  NULL;

	insertsiteresult ret = InsertSite(insertPoint, &searchtri, (EDGE *)NULL, 0, 0);
	
	switch ( ret ) {
	case DUPLICATEPOINT:
		{
		triPOINT *dupPoint = org( searchtri );
		SetPointDup( insertPoint , dupPoint);
		}
	case SUCCESSFULPOINT:
		return insertPoint;

	case ENCROACHINGPOINT:
	case VIOLATINGPOINT:
		PointDealloc( insertPoint );
		return NULL;
	}

	return NULL;
}


bool CTINClass::RemovePoint(triPOINT *delPoint)
{
	triEDGE countingtri;
	triEDGE firstedge, lastedge;
	triEDGE deltriright;
	triEDGE leftTri, rightTri;
	triEDGE leftcasing, rightcasing;
	EDGE leftshelle, rightshelle, shelle;
	triPOINT *neworg;
	triEDGE deltri;

	int edgecount;
		
	if( delPoint == NULL )
		return false;

	/* Find a TRIANGLE whose origin is the segment's first endpoint. */
	if( PointDup ( delPoint ) ) 
		delPoint = PointDup ( delPoint );
	
	if( DecreasePointDupCount( delPoint ) > 0 )
		return false;

	LocatePoint( delPoint, deltri );

	/* Count the degree of the triPOINT being deleted. */
	tspivot( deltri, shelle);
	if( shelle.sh != m_dummysh ) {
		printf(" Attempt to delete POINT on constrained edge\n");
		return false;
	}

	oNextSpinEdge( deltri, countingtri);
	edgecount = 1;
	while ( !TriEdgeEqual(deltri, countingtri) ) {
		tspivot( countingtri, shelle);
		if( shelle.sh != m_dummysh ) {
			printf(" Attempt to delete POINT on constrained edge\n");
			return false;
		}

		if (countingtri.tri == m_dummytri) {
			printf("  Attempt to delete boundary POINT.\n");
			return false;
		}

		edgecount++;
		oNextSpinEdgeSelf(countingtri);

		if( edgecount > 100 ) {
			printf("  Error counting edge .\n");
			return false;
		}
	}
	
	if (edgecount < 3) {
		printf("Attempt to delete POINT of hole : \n  Point has degree %d.\n",edgecount);
		return false;
	}

	if (edgecount > 3) {
		PointDealloc(delPoint);
		
		/* Triangulate the polygon defined by the union of all m_triangles */
		/*   adjacent to the triPOINT being deleted.  Check the quality of  */
		/*   the resulting m_triangles.                                    */
		oNextSpinEdge(deltri, firstedge);
		oPrevSpinEdge(deltri, lastedge);
		triangulatepolygon(&firstedge, &lastedge, edgecount, 0, !nobisect);
	}
	/* Splice out two m_triangles. */
	PrevEdge(deltri, deltriright);
	dNextSpinEdge(deltri, leftTri);
	SymmTri(leftTri, leftcasing);
	oPrevSpinEdge(deltriright, rightTri);
	SymmTri(rightTri, rightcasing);
	bond(deltri, leftcasing);
	bond(deltriright, rightcasing);
	tspivot(leftTri, leftshelle);
	if (leftshelle.sh != m_dummysh) 
		tshBond(deltri, leftshelle);
	tspivot(rightTri, rightshelle);
	if (rightshelle.sh != m_dummysh) 
		tshBond(deltriright, rightshelle);
	
	/* Set the new origin of `deltri' and check its quality. */
	neworg = org(leftTri );
	SetOrg(deltri, neworg);
	if (!nobisect) 
		testtriangle( &deltri);
	
	/* Delete the two spliced-out m_triangles. */
	triangleDealloc(leftTri.tri);
	triangleDealloc(rightTri.tri);

	return true;
}

/*****************************************************************************/
/*                                                                           */
/*  GetNeighborPointNums()   Get m_points around a triPOINT			             */
/*                                                                           */
/*****************************************************************************/

static triPOINT *NeighborPoints[256];

triPOINT **CTINClass::GetNeighborPoints(triPOINT *thePoint, int *pSum)
{
	triPOINT *p1, *p2, *p3;
	triEDGE begintri, nexttri;
	
	p1 = thePoint;

	*pSum = 0;

	/* Find a TRIANGLE whose origin is the segment's first endpoint. */
	LocatePoint( thePoint, begintri);

	nexttri = begintri;
	
	p2 = dest(nexttri );
	if( p2 ) {
		NeighborPoints[ *pSum ] = p2; 
		(*pSum)++;
	}
	do	{
		p3 = apex(nexttri);
		if( p3 ) {
			NeighborPoints[*pSum] = p3; 
			(*pSum)++;
		}
		oNextSpinEdgeSelf( nexttri );
	} while ( nexttri.tri != m_dummytri && nexttri.tri != begintri.tri );
	
	if( nexttri.tri != m_dummytri )
		(*pSum)--;
	
	return NeighborPoints;
}
