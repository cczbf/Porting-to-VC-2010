#ifndef TRIMESH_H
#define TRIMESH_H
/*
Szymon Rusinkiewicz
Princeton University

TriMesh.h
Class for triangle meshes.
*/

#include "Vec.h"
#include "Color.h"
#include <vector>

using namespace std;
//using std::vector;

class TriMesh {
public:
	void GetVertex(float &x , float &y, float &z, int index);
	void GetNormal(float &nx, float &ny, float &nz, int index);
	void GetFaceIndex(int &a, int &b, int &c, int index);
	
	// Types
	struct Face {
		int v[3];

		Face() {}
		Face(const int &v0, const int &v1, const int &v2)
			{ v[0] = v0; v[1] = v1; v[2] = v2; }
		Face(const int *v_)
			{ v[0] = v_[0]; v[1] = v_[1]; v[2] = v_[2]; }
		int &operator[] (int i) { return v[i]; }
		const int &operator[] (int i) const { return v[i]; }
		operator const int * () const { return &(v[0]); }
		operator const int * () { return &(v[0]); }
		operator int * () { return &(v[0]); }
		int indexof(int v_) const
		{
			return (v[0] == v_) ? 0 :
			       (v[1] == v_) ? 1 :
			       (v[2] == v_) ? 2 : -1;
		}
	};

	struct BBox {
		point min, max;
		point center() const { return 0.5f * (min+max); }
		vec size() const { return max - min; }
	};

	struct BSphere {
		point center;
		float r;
	};

	// Enums
	enum tstrip_rep { TSTRIP_LENGTH, TSTRIP_TERM };

	// The basics: vertices and faces
	vector<point> vertices;
	vector<Face> faces;

	// Triangle strips
	vector<int> tstrips;

	// Other per-vertex properties
	vector<Color> colors;
	vector<float> confidences;
	vector<unsigned> flags;
	unsigned flag_curr;

	// Computed per-vertex properties
	vector<vec> normals;
	vector<vec> pdir1, pdir2;
	vector<float> curv1, curv2;
	//vector< Vec<4,float> > dcurv;
	vector<vec> cornerareas;
	vector<float> pointareas;

	// Bounding structures
	BBox bbox;
	BSphere bsphere;

	// Connectivity structures:
	//  For each vertex, all neighboring vertices
	vector< vector<int> > neighbors;
	//  For each vertex, all neighboring faces
	vector< vector<int> > adjacentfaces;
	//  For each face, the three faces attached to its edges
	//  (for example, across_edge[3][2] is the number of the face
	//   that's touching the edge opposite vertex 2 of face 3)
	vector<Face> across_edge;

	// Compute all this stuff...
	void need_tstrips();
	void convert_strips(tstrip_rep rep);
	void need_faces();
	void need_normals();
	void need_pointareas();
	void need_curvatures();
	void need_dcurv();
	void need_bbox();
	void need_bsphere();
	void need_neighbors();
	void need_adjacentfaces();
	void need_across_edge();

	// Input and output
	static TriMesh *read(const char *filename);
	void write(const char *filename);
	void WriteAsciiPly(const char *filename);
	
	// Statistics
	float feature_size();
};


#if !defined(NDEBUG)
#  define dprintf fprintf
#  define dflush fflush
#else
   static inline void dprintf(...) {}
   static inline void dflush(...) {}
#endif


#endif
