/*
Szymon Rusinkiewicz
Princeton University

TriMesh_io.cc
Input and output of triangle meshes
Can read: PLY (triangle mesh and range grid), OFF, OBJ, RAY, SM, 3DS
Can write: PLY (triangle mesh only), OFF, OBJ, RAY, SM, C++
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "assert.h"

#include "TriMesh.h"

#ifdef WIN32
# ifndef strncasecmp
#  define strncasecmp strnicmp
# endif
#endif

#define GET_LINE() if (!fgets(buf, 1024, f)) goto readerror
#define COND_READ(cond, where, len) if ((cond) && !fread((void *)&(where), (len), 1, f)) return false
#define LINE_IS(text) !strncasecmp(buf, text, strlen(text))

#define BIGNUM 1.0e10


// Forward declarations
static TriMesh *read_ply(FILE *f);
static TriMesh *read_3ds(FILE *f);
static TriMesh *read_ray(FILE *f);
static TriMesh *read_obj(FILE *f);
static TriMesh *read_off(FILE *f);
static TriMesh *read_sm(FILE *f);
static bool read_verts_bin(FILE *f, TriMesh *newmesh, bool &need_swap,
	int nverts, int vert_len, int vert_pos, int vert_norm,
	int vert_color, int vert_conf);
static bool slurp_verts_bin(FILE *f, TriMesh *newmesh, bool need_swap,
	int nverts);
static bool read_verts_asc(FILE *f, TriMesh *newmesh,
	int nverts, int vert_len, int vert_pos, int vert_norm,
	int vert_color, int vert_conf);
static bool read_faces_bin(FILE *f, TriMesh *newmesh, bool need_swap,
	int nfaces, int face_len, int face_idx);
static bool read_faces_asc(FILE *f, TriMesh *newmesh, int nfaces,
	int face_len, int face_idx);
static bool read_strips_bin(FILE *f, TriMesh *newmesh, bool need_swap);
static bool read_strips_asc(FILE *f, TriMesh *newmesh);
static bool read_grid_bin(FILE *f, TriMesh *newmesh, bool need_swap,
	int range_width, int range_height);
static bool read_grid_asc(FILE *f, TriMesh *newmesh,
	int range_width, int range_height);
static void triangulate_grid(TriMesh *mesh, int range_width, int range_height,
	vector<int> &grid);
static bool ply_property(const char *buf, int &len, bool binary);
static bool we_are_little_endian();
static void check_need_swap(const point &p, bool &need_swap);
static void skip_comments(FILE *f);
static void write_ply_ascii(TriMesh *mesh, FILE *f, bool write_norm);
static void write_ply_binary(TriMesh *mesh, FILE *f,
	bool need_swap, bool write_norm);
static void write_ray(TriMesh *mesh, FILE *f);
static void write_obj(TriMesh *mesh, FILE *f);
static void write_off(TriMesh *mesh, FILE *f);
static void write_sm(TriMesh *mesh, FILE *f);
static void write_cc(TriMesh *mesh, FILE *f, const char *filename,
	bool write_norm);
static void write_verts_asc(TriMesh *mesh, FILE *f,
			    const char *before_vert,
			    const char *before_norm,
			    const char *before_color,
			    const char *before_conf,
			    const char *after_line);
static void write_verts_bin(TriMesh *mesh, FILE *f, bool need_swap,
			    bool write_norm, bool write_color, bool write_conf);
static void write_faces_asc(TriMesh *mesh, FILE *f,
			    const char *before_face, const char *after_line);
static void write_faces_bin(TriMesh *mesh, FILE *f, bool need_swap,
			    int before_face_len, const char *before_face,
			    int after_face_len, const char *after_face);
static void write_strips_asc(TriMesh *mesh, FILE *f);
static void write_strips_bin(TriMesh *mesh, FILE *f, bool need_swap);
static void tess(const vector<point> &verts, const vector<int> &thisface,
		 vector<TriMesh::Face> &tris);


// Byte swap uints, ints, and floats
static inline void swap_unsigned(unsigned &x)
{
	x = (x << 24) |
	    ((x << 8) & 0x00ff0000u) |
	    ((x >> 8) & 0x0000ff00u) |
	    (x >> 24);
}

static inline void swap_int(int &x)
{
	swap_unsigned(* (unsigned *)(&x));
}

static inline void swap_float(float &x)
{
	swap_unsigned(* (unsigned *)(&x));
}

static inline void swap_ushort(unsigned short &x)
{
	x = (x << 8) | (x >> 8);
}

static inline void swap_short(signed short &x)
{
	swap_ushort(* (unsigned short *)(&x));
}



// unget a whole string of characters
void pushback(const char *buf, FILE *f)
{
	const char *c = buf;
	while (*c)
		c++;
	while ((--c) >= buf)
		ungetc(*c, f);
}


// Read a TriMesh from a file.  Tries to figure out type of file from first
// few bytes.  Filename can be "-" for stdin
TriMesh *TriMesh::read(const char *filename)
{
	if (!filename || *filename == '\0')
		return NULL;

	FILE *f = NULL;
	TriMesh *newmesh = NULL;
	int c;

	if (strcmp(filename, "-") == 0) 
	{
		f = stdin;
		filename = "standard input";
	}
	else 
	{
		f = fopen(filename, "rb");
		if (!f) {
			perror("fopen");
			fclose(f);				
			return NULL;
			//goto out;
		}
	}
	
	dprintf(stderr, "Reading %s... ", filename); dflush(stderr);

	c = fgetc(f);
	if (c == EOF) 
	{
		fprintf(stderr, "Can't read header\n");
		fclose(f);
		return NULL;
		//goto out;
	}

	if (c == 'p') 
	{
		// See if it's a ply file
		char buf[4];
		if (!fgets(buf, 4, f)) 
		{
			fprintf(stderr, "Can't read header\n");
			fclose(f);
			return NULL;
			//goto out;
		}
	
		if (strncmp(buf, "ly", 2) == 0)
			newmesh = read_ply(f);
	} 
	else if (c == 0x4d) 
	{
		int c2 = fgetc(f);
		ungetc(c2, f);
		ungetc(c, f);
		
		if (c2 == 0x4d)
			newmesh = read_3ds(f);
	}
	else if (c == '#') 
	{
		char buf[1024];
		fscanf(f, "%1024s", buf);
		
		if (LINE_IS("material") || LINE_IS("vertex") ||
		    LINE_IS("shape_")) 
		{
			// Assume a ray file
			pushback(buf, f);
			ungetc(c, f);
			newmesh = read_ray(f);
		}
		else
		{
			// Assume an obj file
			newmesh = read_obj(f);
		}
	}
	else if (c == 'v' || c == 'u' || c == 'f' || c == 'g' || c == 's' || c == 'o') 
	{
		// Assume an obj file
		ungetc(c, f);
		newmesh = read_obj(f);
	} 
	else if (c == 'O') 
	{
		// Assume an OFF file
		char buf[3];
		if (!fgets(buf, 3, f)) 
		{
			fprintf(stderr, "Can't read header\n");
			fclose(f);
			return NULL;
			//goto out;
		}
		if (strncmp(buf, "FF", 2) == 0)
			newmesh = read_off(f);
	}
	else if (isdigit(c)) 
	{
		// Assume an old-style sm file
		ungetc(c, f);
		newmesh = read_sm(f);
	}
	else
		fprintf(stderr, "Unknown file type\n");

//out:
//	if (f)
//		fclose(f);

	if (!newmesh || (newmesh->vertices.empty() && newmesh->faces.empty())) 
	{
		if (newmesh)
			delete newmesh;
		fprintf(stderr, "\nError reading file [%s]\n", filename);
		return NULL;
	}
	
	dprintf(stderr, "Done.\n");
	return newmesh;
}


// Read a ply file
static TriMesh *read_ply(FILE *f)
{
	char buf[1024];	
	bool binary, need_swap;
	TriMesh *newmesh = NULL;
	int range_width = -1, range_height = -1;
	int result, nverts = 0, nfaces = 0, nstrips = 0, ngrid = 0;
	int vert_len = 0, vert_pos = -1, vert_norm = -1;
	int vert_color = -1, vert_conf = -1;
	int face_len = 0, face_idx = -1;

	// Read file format
	GET_LINE();
	while (buf[0] && isspace(buf[0]))
		GET_LINE();
	if (LINE_IS("format binary_big_endian 1.0")) {
		binary = true;
		need_swap = we_are_little_endian();
	} else if (LINE_IS("format binary_little_endian 1.0")) {
		binary = true;
		need_swap = !we_are_little_endian();
	} else if (LINE_IS("format ascii 1.0")) {
		binary = false;
	} else {
		fprintf(stderr, "Unknown ply format or version\n");
		return NULL;
	}

	// Skip comments and unknown obj_info lines
	GET_LINE();
	while (LINE_IS("obj_info") || LINE_IS("comment")) {
		if (LINE_IS("obj_info num_cols"))
			sscanf(buf, "obj_info num_cols %d", &range_width);
		if (LINE_IS("obj_info num_rows"))
			sscanf(buf, "obj_info num_rows %d", &range_height);
		GET_LINE();
	}

	// Find number of vertices
	result = sscanf(buf, "element vertex %d\n", &nverts);
	if (result != 1) 
	{
		fprintf(stderr, "Expected \"element vertex\"\n");
		goto readerror;
	}

	// Parse vertex properties
	GET_LINE();
	while (LINE_IS("property")) 
	{
		if (LINE_IS("property float x") ||
		    LINE_IS("property float32 x"))
		    	vert_pos = vert_len;
		if (LINE_IS("property float nx") ||
		    LINE_IS("property float32 nx"))
		    	vert_norm = vert_len;
		if (LINE_IS("property uchar diffuse_red") ||
		    LINE_IS("property uint8 diffuse_red"))
		    	vert_color = vert_len;
		if (LINE_IS("property float confidence") ||
		    LINE_IS("property float32 confidence"))
		    	vert_conf = vert_len;

		if (!ply_property(buf, vert_len, binary))
			goto readerror;

		GET_LINE();
	}

	// Look for faces, tristrips, or range grid
	if (LINE_IS("element face")) 
	{
		if (sscanf(buf, "element face %d\n", &nfaces) != 1)
			goto readerror;
		GET_LINE();
		while (LINE_IS("property")) 
		{
			if (LINE_IS("property list uchar int vertex_indices") ||
			    LINE_IS("property list uint8 int32 vertex_indices") ||
			    LINE_IS("property list char int vertex_indices") ||
			    LINE_IS("property list int8 int32 vertex_indices")) {
				face_idx = face_len + 1;
				face_len += (binary ? 13 : 4);
			} 
			else if
			   (LINE_IS("property list uint int vertex_indices") ||
			    LINE_IS("property list uint32 int32 vertex_indices") ||
			    LINE_IS("property list int int vertex_indices") ||
			    LINE_IS("property list int32 int32 vertex_indices")) 
			{
				face_idx = face_len + (binary ? 4 : 1);
				face_len += (binary ? 16 : 4);
			}
			else if (!ply_property(buf, face_len, binary))
				goto readerror;
			GET_LINE();
		}
	}
	else if (LINE_IS("element tristrips")) 
	{
		nstrips = 1;
		GET_LINE();
		if (!LINE_IS("property list int int vertex_indices") &&
		    !LINE_IS("property list int32 int32 vertex_indices"))
			goto readerror;
		GET_LINE();
	}
	else if (LINE_IS("element range_grid")) 
	{
		if (sscanf(buf, "element range_grid %d\n", &ngrid) != 1)
			goto readerror;
		if (ngrid != range_width*range_height) 
		{
			fprintf(stderr, "Range grid size does not equal num_rows*num_cols\n");
			goto readerror;
		}
		GET_LINE();
		if (!LINE_IS("property list uchar int vertex_indices") &&
		    !LINE_IS("property list uint8 int32 vertex_indices") &&
		    !LINE_IS("property list char int vertex_indices") &&
		    !LINE_IS("property list int8 int32 vertex_indices"))
			goto readerror;
		GET_LINE();
	}

	while (LINE_IS("property")) 
	{
		if (!ply_property(buf, face_len, binary))
			goto readerror;
		GET_LINE();
	}

	// Skip to the end of the header
	while (!LINE_IS("end_header"))
		GET_LINE();
	if (binary && buf[10] == '\r') {
		fprintf(stderr, "Warning!  Possibly corrupt file\n");
		fprintf(stderr, "     If things don't work, make sure this file was transferred in BINARY, not ASCII mode\n");
	}


	// Create the mesh and actually read everything in
	newmesh = new TriMesh();

	if (binary) {
		if (!read_verts_bin(f, newmesh, need_swap, nverts, vert_len,
				    vert_pos, vert_norm, vert_color, vert_conf))
			goto readerror;
	} else {
		if (!read_verts_asc(f, newmesh, nverts, vert_len,
				    vert_pos, vert_norm, vert_color, vert_conf))
			goto readerror;
	}

	if (ngrid) {
		if (binary) {
			if (!read_grid_bin(f, newmesh, need_swap,
					   range_width, range_height))
				goto readerror;
		} else {
			if (!read_grid_asc(f, newmesh,
					   range_width, range_height))
				goto readerror;
		}
	} else if (nstrips) {
		if (binary) {
			if (!read_strips_bin(f, newmesh, need_swap))
				goto readerror;
		} else {
			if (!read_strips_asc(f, newmesh))
				goto readerror;
		}
		newmesh->convert_strips(TriMesh::TSTRIP_LENGTH);
	} else if (nfaces) {
		if (binary) {
			if (!read_faces_bin(f, newmesh, need_swap, nfaces,
					    face_len, face_idx))
				goto readerror;
		} else {
			if (!read_faces_asc(f, newmesh, nfaces,
					    face_len, face_idx))
				goto readerror;
		}
	}

	return newmesh;

readerror:
	if (newmesh)
		delete newmesh;
	return NULL;
}


#define CHUNK_3DS_MAIN  0x4d4d
#define CHUNK_3DS_MODEL 0x3d3d
#define CHUNK_3DS_OBJ   0x4000
#define CHUNK_3DS_MESH  0x4100
#define CHUNK_3DS_VERT  0x4110
#define CHUNK_3DS_FACE  0x4120

// Read a 3DS file.
static TriMesh *read_3ds(FILE *f)
{
	bool need_swap = !we_are_little_endian();
	TriMesh *newmesh = new TriMesh();
	int mstart = 0;

	while (!feof(f)) {
		short chunkid;
		int chunklen;
		if (!fread(&chunkid, 2, 1, f) ||
		    !fread(&chunklen, 4, 1, f))
			return newmesh;
		if (need_swap) {
			swap_short(chunkid);
			swap_int(chunklen);
		}
		//printf("Found chunk %x of length %d\n", chunkid, chunklen);
		switch (chunkid) {
			case CHUNK_3DS_MAIN:
			case CHUNK_3DS_MODEL:
				// Just recurse into this chunk
				break;

			case CHUNK_3DS_OBJ:
				// Skip name, then recurse
				while (!feof(f) && fgetc(f))
					;
				break;

			case CHUNK_3DS_MESH:
				mstart = newmesh->vertices.size();
				break;

			case CHUNK_3DS_VERT: {
				unsigned short nverts;
				if (!fread(&nverts, 2, 1, f))
					return newmesh;
				if (need_swap)
					swap_ushort(nverts);
				read_verts_bin(f, newmesh, need_swap,
					       nverts, 12, 0, -1, -1, -1);
				break;
			}
			case CHUNK_3DS_FACE: {
				unsigned short nfaces;
				if (!fread(&nfaces, 2, 1, f))
					return newmesh;
				if (need_swap)
					swap_ushort(nfaces);
				dprintf(stderr, "\n  Reading %d faces... ", nfaces); dflush(stderr);
				int old_nfaces = newmesh->faces.size();
				int new_nfaces = old_nfaces + nfaces;
				newmesh->faces.resize(new_nfaces);
				for (int i = old_nfaces; i < new_nfaces; i++) {
					unsigned short buf[4];
					COND_READ(true, buf[0], 8);
					if (need_swap) {
						swap_ushort(buf[0]);
						swap_ushort(buf[1]);
						swap_ushort(buf[2]);
					}
					newmesh->faces[i][0] = mstart + buf[0];
					newmesh->faces[i][1] = mstart + buf[1];
					newmesh->faces[i][2] = mstart + buf[2];
				}
				break;
			}
			default: {
				// Skip over this chunk
				char *buf = new  char[chunklen-6];
				fread(buf, chunklen-6, 1, f);
			}
		}
	}
	return newmesh;
}


// Read a ray file
static TriMesh *read_ray(FILE *f)
{
	TriMesh *newmesh = new TriMesh();
	while (1) {
		char buf[1024];
		buf[0] = '\0';
		if (feof(f) || fscanf(f, " %1024s", buf) == 0)
			return newmesh;
		if (LINE_IS("#vertex")) {
			float x, y, z;
			if (fscanf(f, "%f %f %f", &x, &y, &z) != 3) {
				delete newmesh;
				return NULL;
			}
			newmesh->vertices.push_back(point(x,y,z));
		} else if (LINE_IS("#shape_triangle")) {
			int f1, f2, f3, m;
			if (fscanf(f, "%d %d %d %d", &m, &f1, &f2, &f3) != 4) {
				delete newmesh;
				return NULL;
			}
			newmesh->faces.push_back(TriMesh::Face(f1,f2,f3));
		}
	}
}


// Read an obj file
static TriMesh *read_obj(FILE *f)
{
	TriMesh *newmesh = new TriMesh();
	while (1) {
		char buf[1024];
		GET_LINE();
		if (LINE_IS("v ") || LINE_IS("v\t")) {
			float x, y, z;
			if (sscanf(buf+1, "%f %f %f", &x, &y, &z) != 3) {
				delete newmesh;
				return NULL;
			}
			newmesh->vertices.push_back(point(x,y,z));
		} else if (LINE_IS("f ")) {
			vector<int> thisface;
			char *c = buf;
			while (1) {
				while (*c && *c != '\n' && !isspace(*c))
					c++;
				while (*c && isspace(*c))
					c++;
				int thisf;
				if (sscanf(c, " %d", &thisf) != 1)
					break;
				if (thisf < 0)
					thisf += newmesh->vertices.size();
				else
					thisf--;
				thisface.push_back(thisf);
			}
			tess(newmesh->vertices, thisface, newmesh->faces);
		}
	}

readerror:
	return newmesh;
}


// Read an off file
static TriMesh *read_off(FILE *f)
{
	int nverts, nfaces, unused;
	if (fscanf(f, " %d %d %d", &nverts, &nfaces, &unused) != 3)
		return NULL;

	TriMesh *newmesh = new TriMesh();
	if (!read_verts_asc(f, newmesh, nverts, 3, 0, -1, -1, -1))
		goto readerror;
	if (!read_faces_asc(f, newmesh, nfaces, 4, 1))
		goto readerror;

	return newmesh;

readerror:
	if (newmesh)
		delete newmesh;
	return NULL;
}


// Read an sm file
static TriMesh *read_sm(FILE *f)
{
	int nverts, nfaces;

	if (fscanf(f, "%d", &nverts) != 1)
		return NULL;

	TriMesh *newmesh = new TriMesh();
	if (!read_verts_asc(f, newmesh, nverts, 3, 0, -1, -1, -1))
		goto readerror;

	skip_comments(f);
	if (fscanf(f, "%d", &nfaces) != 1)
		goto readerror;
	if (!read_faces_asc(f, newmesh, nfaces, 3, 0))
		goto readerror;

	return newmesh;

readerror:
	if (newmesh)
		delete newmesh;
	return NULL;
}


// Read a bunch of vertices from a binary file
static bool read_verts_bin(FILE *f, TriMesh *newmesh, bool &need_swap,
	int nverts, int vert_len, int vert_pos, int vert_norm,
	int vert_color, int vert_conf)
{
	if (nverts <= 0 || vert_len < 12 || vert_pos < 0)
		return false;

	int old_nverts = newmesh->vertices.size();
	int new_nverts = old_nverts + nverts;
	newmesh->vertices.resize(new_nverts);

	int last = vert_pos + 12;
	int norm_skip = vert_norm - last;
	bool have_norm = (norm_skip >= 0);
	if (have_norm) {
		newmesh->normals.resize(new_nverts);
		last = vert_norm + 12;
	}
	int color_skip = vert_color - last;
	bool have_colors = (color_skip >= 0);
	if (have_colors) {
		newmesh->colors.resize(new_nverts);
		last = vert_color + 3;
	}
	int conf_skip = vert_conf - last;
	bool have_conf = (conf_skip >= 0);
	if (have_conf) {
		newmesh->confidences.resize(new_nverts);
		last = vert_conf + 4;
	}

	int skip = vert_len - last + vert_pos;

	int i = old_nverts;
	char buf[1024];
	COND_READ(vert_pos > 0, buf, vert_pos);
	COND_READ(true, newmesh->vertices[i][0], 12);
	COND_READ(norm_skip > 0, buf, norm_skip);
	COND_READ(have_norm, newmesh->normals[i][0], 12);
	COND_READ(color_skip > 0, buf, color_skip);
	COND_READ(have_colors, newmesh->colors[i][0], 3);
	COND_READ(conf_skip > 0, buf, conf_skip);
	COND_READ(have_conf, newmesh->confidences[i], 4);

	check_need_swap(newmesh->vertices[i], need_swap);
	if (need_swap) {
		swap_float(newmesh->vertices[i][0]);
		swap_float(newmesh->vertices[i][1]);
		swap_float(newmesh->vertices[i][2]);
		if (have_norm) {
			swap_float(newmesh->normals[i][0]);
			swap_float(newmesh->normals[i][1]);
			swap_float(newmesh->normals[i][2]);
		}
		if (have_conf)
			swap_float(newmesh->confidences[i]);
	}

	dprintf(stderr, "\n  Reading %d vertices... ", nverts); dflush(stderr);
	if (vert_len == 12 && sizeof(point) == 12 && nverts > 1)
		return slurp_verts_bin(f, newmesh, need_swap, nverts);
	while (++i < new_nverts) {
		COND_READ(skip > 0, buf, skip);
		COND_READ(true, newmesh->vertices[i][0], 12);
		COND_READ(norm_skip > 0, buf, norm_skip);
		COND_READ(have_norm, newmesh->normals[i][0], 12);
		COND_READ(color_skip > 0, buf, color_skip);
		COND_READ(have_colors, newmesh->colors[i][0], 3);
		COND_READ(conf_skip > 0, buf, conf_skip);
		COND_READ(have_conf, newmesh->confidences[i], 4);
		if (need_swap) {
			swap_float(newmesh->vertices[i][0]);
			swap_float(newmesh->vertices[i][1]);
			swap_float(newmesh->vertices[i][2]);
			if (have_norm) {
				swap_float(newmesh->normals[i][0]);
				swap_float(newmesh->normals[i][1]);
				swap_float(newmesh->normals[i][2]);
			}
			if (have_conf)
				swap_float(newmesh->confidences[i]);
		}
	}
	int final_skip = skip - vert_pos;
	COND_READ(final_skip > 0, buf, final_skip);

	return true;
}


// Optimized reader for the simple case of just vertices w/o other properties
static bool slurp_verts_bin(FILE *f, TriMesh *newmesh, bool need_swap, int nverts)
{
	int first = newmesh->vertices.size() - nverts + 1;
	COND_READ(true, newmesh->vertices[first][0], (nverts-1)*12);
	if (need_swap) {
		for (int i = first; i < newmesh->vertices.size(); i++) {
			swap_float(newmesh->vertices[i][0]);
			swap_float(newmesh->vertices[i][1]);
			swap_float(newmesh->vertices[i][2]);
		}
	}
	return true;
}


// Read a bunch of vertices from an ASCII file
static bool read_verts_asc(FILE *f, TriMesh *newmesh,
	int nverts, int vert_len, int vert_pos, int vert_norm,
	int vert_color, int vert_conf)
{
	if (nverts <= 0 || vert_len < 3 || vert_pos < 0)
		return false;

	int old_nverts = newmesh->vertices.size();
	int new_nverts = old_nverts + nverts;
	newmesh->vertices.resize(new_nverts);
	if (vert_norm > 0)
		newmesh->normals.resize(new_nverts);
	if (vert_color > 0)
		newmesh->colors.resize(new_nverts);
	if (vert_conf > 0)
		newmesh->confidences.resize(new_nverts);

	char buf[1024];
	skip_comments(f);
	dprintf(stderr, "\n  Reading %d vertices... ", nverts); dflush(stderr);
	for (int i = old_nverts; i < new_nverts; i++) {
		for (int j = 0; j < vert_len; j++) {
			if (j == vert_pos) {
				if (fscanf(f, "%f %f %f",
					      &newmesh->vertices[i][0],
					      &newmesh->vertices[i][1],
					      &newmesh->vertices[i][2]) != 3)
					return false;
				j += 2;
			} else if (j == vert_norm) {
				if (fscanf(f, "%f %f %f",
					      &newmesh->normals[i][0],
					      &newmesh->normals[i][1],
					      &newmesh->normals[i][2]) != 3)
					return false;
				j += 2;
			} else if (j == vert_color) {
				int r, g, b;
				if (fscanf(f, "%d %d %d", &r, &g, &b) != 3)
					return false;
				newmesh->colors[i][0] = min(max(r, 0), 255);
				newmesh->colors[i][1] = min(max(g, 0), 255);
				newmesh->colors[i][2] = min(max(b, 0), 255);
				j += 2;
			} else if (j == vert_conf) {
				if (fscanf(f, "%f", &newmesh->confidences[i]) != 1)
					return false;
			} else {
				fscanf(f, " %1024s", buf);
			}
		}
	}

	return true;
}


// Read a bunch of faces from a binary file
static bool read_faces_bin(FILE *f, TriMesh *newmesh, bool need_swap,
	int nfaces, int face_len, int face_idx)
{
	if (nfaces <= 0 || face_len < 12 || face_idx < 0)
		return false;

	int old_nfaces = newmesh->faces.size();
	int new_nfaces = old_nfaces + nfaces;
	newmesh->faces.resize(new_nfaces);

	int skip = face_len - 12;

	int i = old_nfaces;
	char buf[1024];
	COND_READ(face_idx > 0, buf, face_idx);
	COND_READ(true, newmesh->faces[i][0], 12);
	if (need_swap) {
		swap_int(newmesh->faces[i][0]);
		swap_int(newmesh->faces[i][1]);
		swap_int(newmesh->faces[i][2]);
	}

	dprintf(stderr, "\n  Reading %d faces... ", nfaces); dflush(stderr);
	while (++i < new_nfaces) {
		COND_READ(skip > 0, buf, skip);
		COND_READ(true, newmesh->faces[i][0], 12);
		if (need_swap) {
			swap_int(newmesh->faces[i][0]);
			swap_int(newmesh->faces[i][1]);
			swap_int(newmesh->faces[i][2]);
		}
	}
	int final_skip = skip - face_idx;
	COND_READ(final_skip > 0, buf, final_skip);

	return true;
}


// Read a bunch of faces from an ASCII file
static bool read_faces_asc(FILE *f, TriMesh *newmesh, int nfaces,
	int face_len, int face_idx)
{
	if (nfaces <= 0 || face_len < 3 || face_idx < 0)
		return false;

	int old_nfaces = newmesh->faces.size();
	int new_nfaces = old_nfaces + nfaces;
	newmesh->faces.resize(new_nfaces);

	char buf[1024];
	skip_comments(f);
	dprintf(stderr, "\n  Reading %d faces... ", nfaces); dflush(stderr);
	for (int i = old_nfaces; i < new_nfaces; i++) {
		for (int j = 0; j < face_len; j++) {
			if (j == face_idx) {
				if (fscanf(f, "%d %d %d",
					      &newmesh->faces[i][0],
					      &newmesh->faces[i][1],
					      &newmesh->faces[i][2]) != 3)
					return false;
				j += 2;
			} else {
				fscanf(f, " %s", buf);
			}
		}
	}

	return true;
}


// Read triangle strips from a binary file
static bool read_strips_bin(FILE *f, TriMesh *newmesh, bool need_swap)
{
	int striplen;
	COND_READ(true, striplen, 4);
	if (need_swap)
		swap_int(striplen);

	int old_striplen = newmesh->tstrips.size();
	int new_striplen = old_striplen + striplen;
	newmesh->tstrips.resize(new_striplen);

	dprintf(stderr, "\n  Reading triangle strips... "); dflush(stderr);
	COND_READ(true, newmesh->tstrips[old_striplen], 4*striplen);
	if (need_swap) {
		for (int i = old_striplen; i < new_striplen; i++)
			swap_int(newmesh->tstrips[i]);
	}

	return true;
}


// Read triangle strips from an ASCII file
static bool read_strips_asc(FILE *f, TriMesh *newmesh)
{
	skip_comments(f);
	int striplen;
	if (fscanf(f, "%d", &striplen) != 1)
		return false;
	int old_striplen = newmesh->tstrips.size();
	int new_striplen = old_striplen + striplen;
	newmesh->tstrips.resize(new_striplen);

	dprintf(stderr, "\n  Reading triangle strips... "); dflush(stderr);
	skip_comments(f);
	for (int i = old_striplen; i < new_striplen; i++)
		if (fscanf(f, "%d", &newmesh->tstrips[i]) != 1)
			return false;

	return true;
}


// Read range grid data from a binary file
static bool read_grid_bin(FILE *f, TriMesh *newmesh, bool need_swap,
	int range_width, int range_height)
{
	dprintf(stderr, "\n  Reading range grid... "); dflush(stderr);
	int ngrid = range_width*range_height;
	vector<int> grid(ngrid, -1);
	for (int i = 0; i < ngrid; i++) {
		int n = fgetc(f);
		if (n == EOF)
			return false;
		while (n--) {
			if (!fread((void *)&(grid[i]), 4, 1, f))
				return false;
			if (need_swap)
				swap_int(grid[i]);
		}
	}

	triangulate_grid(newmesh, range_width, range_height, grid);
	return true;
}


// Read range grid data from an ASCII file
static bool read_grid_asc(FILE *f, TriMesh *newmesh,
	int range_width, int range_height)
{
	dprintf(stderr, "\n  Reading range grid... "); dflush(stderr);
	int ngrid = range_width*range_height;
	vector<int> grid(ngrid, -1);
	for (int i = 0; i < ngrid; i++) {
		int n;
		if (fscanf(f, "%d", &n) != 1)
			return false;
		while (n--) {
			if (fscanf(f, "%d", &(grid[i])) != 1)
				return false;
		}
	}

	triangulate_grid(newmesh, range_width, range_height, grid);
	return true;
}


// Helper function - make a face with the 3 given vertices from the grid
static void mkface(TriMesh *mesh, vector<int> &grid, int v1, int v2, int v3)
{
	mesh->faces.push_back(TriMesh::Face(grid[v1], grid[v2], grid[v3]));
}


// Triangulate a range grid
static void triangulate_grid(TriMesh *mesh, int range_width, int range_height,
			     vector<int> &grid)
{
	int i, j;
	dprintf(stderr, "Triangulating... "); dflush(stderr);
	int ntris = 0;
	for (j = 0; j < range_height - 1; j++) {
		for (i = 0; i < range_width - 1; i++) {
			int ll = i + j * range_width;
			int lr = ll + 1;
			int ul = ll + range_width;
			int ur = ul + 1;
			int nvalid = (grid[ll] >= 0) + (grid[lr] >= 0) +
				     (grid[ul] >= 0) + (grid[ur] >= 0);
			if (nvalid == 4)
				ntris += 2;
			else if (nvalid == 3)
				ntris += 1;
		}
	}

	int old_nfaces = mesh->faces.size();
	int new_nfaces = old_nfaces + ntris;
	mesh->faces.reserve(new_nfaces);

	for (j = 0; j < range_height - 1; j++) {
		for (i = 0; i < range_width - 1; i++) {
			int ll = i + j * range_width;
			int lr = ll + 1;
			int ul = ll + range_width;
			int ur = ul + 1;
			int nvalid = (grid[ll] >= 0) + (grid[lr] >= 0) +
				     (grid[ul] >= 0) + (grid[ur] >= 0);
			if (nvalid < 3)
				continue;
			if (nvalid == 4) {
				// Triangulate in the direction that
				// gives the shorter diagonal
				float ll_ur = dist2(mesh->vertices[grid[ll]],
						    mesh->vertices[grid[ur]]);
				float lr_ul = dist2(mesh->vertices[grid[lr]],
						    mesh->vertices[grid[ul]]);
				if (ll_ur < lr_ul) {
					mkface(mesh, grid, ll, lr, ur);
					mkface(mesh, grid, ll, ur, ul);
				} else {
					mkface(mesh, grid, ll, lr, ul);
					mkface(mesh, grid, lr, ur, ul);
				}
				continue;
			}
			// nvalid == 3
			if (grid[ll] < 0)
				mkface(mesh, grid, lr, ur, ul);
			else if (grid[lr] < 0)
				mkface(mesh, grid, ll, ur, ul);
			else if (grid[ul] < 0)
				mkface(mesh, grid, ll, lr, ur);
			else
				mkface(mesh, grid, ll, lr, ul);
		}
	}

	// Remove long, skinny triangles
	float thresh = sqr(4.0f * mesh->feature_size());
	int offset = 0;
	for (i = old_nfaces; i < new_nfaces; i++) {
		if (dist2(mesh->vertices[mesh->faces[i][0]],
			  mesh->vertices[mesh->faces[i][1]]) > thresh ||
		    dist2(mesh->vertices[mesh->faces[i][1]],
			  mesh->vertices[mesh->faces[i][2]]) > thresh ||
		    dist2(mesh->vertices[mesh->faces[i][2]],
			  mesh->vertices[mesh->faces[i][0]]) > thresh) {
			offset++;
			continue;
		}
		if (!offset)
			continue;
		mesh->faces[i-offset] = mesh->faces[i];
	}
	while (offset--)
		mesh->faces.pop_back();
	dprintf(stderr, "%d faces... ", mesh->faces.size()); dflush(stderr);
}


// Parse a PLY property line, and figure how many bytes it represents
// Increments "len" by the number of bytes, or by 1 if !binary
static bool ply_property(const char *buf, int &len, bool binary)
{
	if (LINE_IS("property char") ||
	    LINE_IS("property uchar") ||
	    LINE_IS("property int8") ||
	    LINE_IS("property uint8")) {
		len += 1;
	} else if (LINE_IS("property short") ||
		   LINE_IS("property ushort") ||
		   LINE_IS("property int16") ||
		   LINE_IS("property uint16")) {
		len += (binary ? 2 : 1);
	} else if (LINE_IS("property int") ||
		   LINE_IS("property uint") ||
		   LINE_IS("property float") ||
		   LINE_IS("property int32") ||
		   LINE_IS("property uint32") ||
		   LINE_IS("property float32")) {
		len += (binary ? 4 : 1);
	} else if (LINE_IS("property double") ||
		   LINE_IS("property float64")) {
		len += (binary ? 8 : 1);
	} else {
		fprintf(stderr, "Unsupported vertex property: %s\n", buf);
		return false;
	}
	return true;
}


// Figure out whether this machine is little- or big-endian
static bool we_are_little_endian()
{
	char buf[4];
	*(int *)(&buf[0]) = 1;
	return (buf[0] == 1);
}


// Figure out whether the need_swap setting makes sense, or whether this
// file incorrectly declares its endianness
static void check_need_swap(const point &p, bool &need_swap)
{
	float p0 = p[0], p1 = p[1], p2 = p[2];
	if (need_swap) {
		swap_float(p0);
		swap_float(p1);
		swap_float(p2);
	}
	bool makes_sense = (p0 > -BIGNUM && p0 < BIGNUM &&
			    p1 > -BIGNUM && p1 < BIGNUM &&
			    p2 > -BIGNUM && p2 < BIGNUM);
	if (makes_sense)
		return;

	swap_float(p0);
	swap_float(p1);
	swap_float(p2);

	bool makes_sense_swapped = (p0 > -BIGNUM && p0 < BIGNUM &&
				    p1 > -BIGNUM && p1 < BIGNUM &&
				    p2 > -BIGNUM && p2 < BIGNUM);
	if (makes_sense_swapped) {
		fprintf(stderr, "Compensating for bogus endianness.\n");
		need_swap = !need_swap;
	}
}


// Skip comments in an ASCII file (lines beginning with #)
static void skip_comments(FILE *f)
{
	int c;
	bool in_comment = false;
	while (1) {
		c = fgetc(f);
		if (c == EOF)
			return;
		if (in_comment) {
			if (c == '\n')
				in_comment = false;
		} else if (c == '#') {
			in_comment = true;
		} else if (!isspace(c)) {
			break;
		}
	}
	ungetc(c, f);
}



// Write mesh to a file
void TriMesh::write(const char *filename)
{
	if (!filename || *filename == '\0')
		return;

	if (vertices.empty()) {
		fprintf(stderr, "Empty mesh - nothing to write!\n");
		return;
	}

	enum { PLY_ASCII, PLY_BINARY_BE, PLY_BINARY_LE,
	       RAY, OBJ, OFF, SM, CC } filetype;
	// Set default file type to be native-endian binary ply
	filetype = we_are_little_endian() ? PLY_BINARY_LE : PLY_BINARY_BE;
	
	//filetype = PLY_ASCII;

	bool write_norm = false;

	// Infer file type from file extension
	const char *c = strrchr(filename, '.');
	if (c) {
		if (!strncasecmp(c, ".ply", 4))
			filetype = we_are_little_endian() ?
					PLY_BINARY_LE :
					PLY_BINARY_BE;
		else if (!strncasecmp(c, ".ray", 4))
			filetype = RAY;
		else if (!strncasecmp(c, ".obj", 4))
			filetype = OBJ;
		else if (!strncasecmp(c, ".off", 4))
			filetype = OFF;
		else if (!strncasecmp(c, ".sm", 3))
			filetype = SM;
		else if (!strncasecmp(c, ".cc", 3))
			filetype = CC;
		else if (!strncasecmp(c, ".c++", 4))
			filetype = CC;
		else if (!strncasecmp(c, ".cpp", 4))
			filetype = CC;
		else if (!strncasecmp(c, ".C", 2))
			filetype = CC;
	}

	// Handle filetype:filename.foo constructs
	if (!strncasecmp(filename, "norm:", 5)) {
		filename += 5;
		write_norm = true;
	}
	if (!strncasecmp(filename, "ply:", 4)) {
		filename += 4;
		filetype = we_are_little_endian() ?
				PLY_BINARY_LE :
				PLY_BINARY_BE;
	} else if (!strncasecmp(filename, "ply_binary:", 11)) {
		filename += 11;
		filetype = we_are_little_endian() ?
				PLY_BINARY_LE :
				PLY_BINARY_BE;
	} else if (!strncasecmp(filename, "ply_binary_be:", 14)) {
		filename += 14;
		filetype = PLY_BINARY_BE;
	} else if (!strncasecmp(filename, "ply_binary_le:", 14)) {
		filename += 14;
		filetype = PLY_BINARY_LE;
	} else if (!strncasecmp(filename, "ply_ascii:", 10)) {
		filename += 10;
		filetype = PLY_ASCII;
	} else if (!strncasecmp(filename, "ply_asc:", 8)) {
		filename += 8;
		filetype = PLY_ASCII;
	} else if (!strncasecmp(filename, "ascii:", 6)) {
		filename += 6;
		filetype = PLY_ASCII;
	} else if (!strncasecmp(filename, "asc:", 4)) {
		filename += 4;
		filetype = PLY_ASCII;
	} else if (!strncasecmp(filename, "be:", 3)) {
		filename += 3;
		filetype = PLY_BINARY_BE;
	} else if (!strncasecmp(filename, "le:", 3)) {
		filename += 3;
		filetype = PLY_BINARY_LE;
	} else if (!strncasecmp(filename, "ray:", 4)) {
		filename += 4;
		filetype = RAY;
	} else if (!strncasecmp(filename, "obj:", 4)) {
		filename += 4;
		filetype = OBJ;
	} else if (!strncasecmp(filename, "off:", 4)) {
		filename += 4;
		filetype = OFF;
	} else if (!strncasecmp(filename, "sm:", 3)) {
		filename += 3;
		filetype = SM;
	}


	FILE *f = NULL;

	if (strcmp(filename, "-") == 0) {
		f = stdout;
		filename = "standard output";
	} else {
		f = fopen(filename, "wb");
		if (!f) {
			perror("fopen");
			fprintf(stderr, "Error opening %s for writing.\n", filename);
			return;
		}
	}


	dprintf(stderr, "Writing %s... ", filename); dflush(stderr);

	switch (filetype) {
		case PLY_ASCII:
			write_ply_ascii(this, f, write_norm);
			break;
		case PLY_BINARY_BE:
			write_ply_binary(this, f,
				we_are_little_endian(), write_norm);
			break;
		case PLY_BINARY_LE:
			write_ply_binary(this, f,
				!we_are_little_endian(), write_norm);
			break;
		case RAY:
			write_ray(this, f);
			break;
		case OBJ:
			write_obj(this, f);
			break;
		case OFF:
			write_off(this, f);
			break;
		case SM:
			write_sm(this, f);
			break;
		case CC:
			write_cc(this, f, filename, write_norm);
			break;
	}
	fclose(f);
	dprintf(stderr, "Done.\n");
}


// Write a ply header
static void write_ply_header(TriMesh *mesh, FILE *f, const char *format,
			     bool write_norm)
{
	fprintf(f, "ply\nformat %s 1.0\n", format);
	fprintf(f, "element vertex %d\n", mesh->vertices.size());
	fprintf(f, "property float x\n");
	fprintf(f, "property float y\n");
	fprintf(f, "property float z\n");
	if (write_norm && !mesh->normals.empty()) {
		fprintf(f, "property float nx\n");
		fprintf(f, "property float ny\n");
		fprintf(f, "property float nz\n");
	}
	if (!mesh->colors.empty()) {
		fprintf(f, "property uchar diffuse_red\n");
		fprintf(f, "property uchar diffuse_green\n");
		fprintf(f, "property uchar diffuse_blue\n");
	}
	if (!mesh->confidences.empty()) {
		fprintf(f, "property float confidence\n");
	}
	if (!mesh->tstrips.empty()) {
		fprintf(f, "element tristrips 1\n");
		fprintf(f, "property list int int vertex_indices\n");
	} else {
		fprintf(f, "element face %d\n", mesh->faces.size());
		fprintf(f, "property list uchar int vertex_indices\n");
	}
	fprintf(f, "end_header\n");
}


// Write an ASCII ply file
static void write_ply_ascii(TriMesh *mesh, FILE *f, bool write_norm)
{
	write_ply_header(mesh, f, "ascii", write_norm);
	write_verts_asc(mesh, f, "", write_norm ? " " : 0, " ", " ", "");
	
	if (!mesh->tstrips.empty())
	{
		fprintf(f, "%d ", mesh->tstrips.size());
		mesh->convert_strips(TriMesh::TSTRIP_TERM);
		write_strips_asc(mesh, f);
		mesh->convert_strips(TriMesh::TSTRIP_LENGTH);
	}
	else 
	{
		write_faces_asc(mesh, f, "3 ", "");
	}
}


// Write a binary ply file
static void write_ply_binary(TriMesh *mesh, FILE *f,
	bool need_swap, bool write_norm)
{
	if (write_norm)
		mesh->need_normals();
	const char *format = (need_swap ^ we_are_little_endian()) ?
		"binary_little_endian" : "binary_big_endian";
	write_ply_header(mesh, f, format, write_norm);
	write_verts_bin(mesh, f, need_swap, write_norm, true, true);
	if (!mesh->tstrips.empty()) {
		int s = mesh->tstrips.size();
		if (need_swap)
			swap_int(s);
		fwrite(&s, 4, 1, f);
		mesh->convert_strips(TriMesh::TSTRIP_TERM);
		write_strips_bin(mesh, f, need_swap);
		mesh->convert_strips(TriMesh::TSTRIP_LENGTH);
	} else {
		char buf[1] = { 3 };
		write_faces_bin(mesh, f, need_swap, 1, buf, 0, 0);
	}
}


// Write a ray file
static void write_ray(TriMesh *mesh, FILE *f)
{
	fprintf(f, "#material 0 0 0  1 1 1  0 0 0  0 0 0  -1 -1\n");
	fprintf(f, "%d\n", mesh->vertices.size());
	write_verts_asc(mesh, f, "#vertex  ", 0, 0, 0, "  0 0 0  0 0");
	mesh->need_faces();
	fprintf(f, "%d\n", mesh->faces.size());
	write_faces_asc(mesh, f, "#shape_triangle 0 ", "");
}


// Write a obj file
static void write_obj(TriMesh *mesh, FILE *f)
{
	fprintf(f, "# OBJ\n");
	write_verts_asc(mesh, f, "v ", 0, 0, 0, "");
	mesh->need_faces();
	int i;
	for (i = 0; i < mesh->faces.size(); i++) {
		mesh->faces[i][0]++;
		mesh->faces[i][1]++;
		mesh->faces[i][2]++;
	}
	write_faces_asc(mesh, f, "f ", "");
	for (i = 0; i < mesh->faces.size(); i++) {
		mesh->faces[i][0]--;
		mesh->faces[i][1]--;
		mesh->faces[i][2]--;
	}
}


// Write a off file
static void write_off(TriMesh *mesh, FILE *f)
{
	fprintf(f, "OFF\n");
	mesh->need_faces();
	fprintf(f, "%d %d\n", mesh->vertices.size(), mesh->faces.size());
	write_verts_asc(mesh, f, "", 0, 0, 0, "");
	write_faces_asc(mesh, f, "3 ", "");
}


// Write an SM file
static void write_sm(TriMesh *mesh, FILE *f)
{
	fprintf(f, "%d\n", mesh->vertices.size());
	write_verts_asc(mesh, f, "", 0, 0, 0, "");
	mesh->need_faces();
	fprintf(f, "%d\n", mesh->faces.size());
	write_faces_asc(mesh, f, "", "");
	fprintf(f, "0 0\n");
}


// Write C++ code
static void write_cc(TriMesh *mesh, FILE *f, const char *filename,
	bool write_norm)
{
	mesh->need_faces();

	char *meshname = new char[strlen(filename)+1];
	strcpy(meshname, filename);
	char *c = strrchr(meshname, '.');
	if (c)
		*c = '\0';
	fprintf(f, "#include <string.h>\n");
	fprintf(f, "#include \"TriMesh.h\"\n\n");
	fprintf(f, "TriMesh *make_%s()\n{", meshname);
	delete [] meshname;

	fprintf(f, "\tstatic const float vertdata[][3] = {\n");
	int nv = mesh->vertices.size(), nf = mesh->faces.size(), i;
	for (i = 0; i < nv; i++) {
		fprintf(f, "\t\t{ %.7g, %.7g, %.7g },\n",
				mesh->vertices[i][0],
				mesh->vertices[i][1],
				mesh->vertices[i][2]);
	}
	fprintf(f, "\t};\n");
	if (write_norm && !mesh->normals.empty()) {
		fprintf(f, "\tstatic const float normdata[][3] = {\n");
		for (i = 0; i < nv; i++) {
			fprintf(f, "\t\t{ %.7g, %.7g, %.7g },\n",
					mesh->normals[i][0],
					mesh->normals[i][1],
					mesh->normals[i][2]);
		}
		fprintf(f, "\t};\n");
	}
	if (!mesh->colors.empty()) {
		fprintf(f, "\tstatic const unsigned char colordata[][3] = {\n");
		for (i = 0; i < nv; i++) {
			fprintf(f, "\t\t{ %d, %d, %d },\n",
					mesh->colors[i][0],
					mesh->colors[i][1],
					mesh->colors[i][2]);
		}
		fprintf(f, "\t};\n");
	}
	fprintf(f, "\tstatic const int facedata[][3] = {\n");
	for (i = 0; i < nf; i++) {
		fprintf(f, "\t\t{ %d, %d, %d },\n",
				mesh->faces[i][0],
				mesh->faces[i][1],
				mesh->faces[i][2]);
	}
	fprintf(f, "\t};\n");
	fprintf(f, "\n\tTriMesh *m = new TriMesh;\n");
	fprintf(f, "\tm->vertices.resize(%d);\n", nv);
	fprintf(f, "\tmemcpy(&m->vertices[0][0], vertdata, sizeof(vertdata);\n");
	if (!mesh->colors.empty()) {
		fprintf(f, "\tm->colors.resize(%d);\n", nv);
		fprintf(f, "\tmemcpy(&m->colors[0][0], colordata, sizeof(colordata);\n");
	}
	if (write_norm && !mesh->normals.empty()) {
		fprintf(f, "\tm->normals.resize(%d);\n", nv);
		fprintf(f, "\tmemcpy(&m->normals[0][0], normdata, sizeof(normdata);\n");
	}
	fprintf(f, "\tm->faces.resize(%d);\n", nf);
	fprintf(f, "\tmemcpy(&m->faces[0][0], facedata, sizeof(facedata);\n");
	fprintf(f, "\n\treturn m;\n");
	fprintf(f, "}\n");
}


// Write a bunch of vertices to an ASCII file
static void write_verts_asc(TriMesh *mesh, FILE *f,
			    const char *before_vert,
			    const char *before_norm,
			    const char *before_color,
			    const char *before_conf,
			    const char *after_line)
{
	for (int i = 0; i < mesh->vertices.size(); i++) {
		fprintf(f, "%s%.7g %.7g %.7g", before_vert,
					 mesh->vertices[i][0],
					 mesh->vertices[i][1],
					 mesh->vertices[i][2]);
		
		if (!mesh->normals.empty() && before_norm)
			fprintf(f, "%s%.7g %.7g %.7g", before_norm,
						 mesh->normals[i][0],
						 mesh->normals[i][1],
						 mesh->normals[i][2]);

		if (!mesh->colors.empty() && before_color)
			fprintf(f, "%s%d %d %d", before_color,
						  mesh->colors[i][0],
						  mesh->colors[i][1],
						  mesh->colors[i][2]);
		if (!mesh->confidences.empty() && before_conf)
			fprintf(f, "%s%.7g", before_conf, mesh->confidences[i]);
		fprintf(f, "%s\n", after_line);
	}
}


// Write a bunch of vertices to a binary file
static void write_verts_bin(TriMesh *mesh, FILE *f, bool need_swap,
			    bool write_norm, bool write_color, bool write_conf)
{
	if (need_swap) {
		for (int i = 0; i < mesh->vertices.size(); i++) {
			swap_float(mesh->vertices[i][0]);
			swap_float(mesh->vertices[i][1]);
			swap_float(mesh->vertices[i][2]);
		}
		if (!mesh->normals.empty()) {
			for (int i = 0; i < mesh->normals.size(); i++) {
				swap_float(mesh->normals[i][0]);
				swap_float(mesh->normals[i][1]);
				swap_float(mesh->normals[i][2]);
			}
		}
		if (!mesh->confidences.empty()) {
			for (int i = 0; i < mesh->confidences.size(); i++)
				swap_float(mesh->confidences[i]);
		}
	}
	if ((mesh->normals.empty() || !write_norm) &&
	    (mesh->colors.empty() || !write_color) &&
	    (mesh->confidences.empty() || !write_conf)) {
		// Optimized vertex-only code
		fwrite(&(mesh->vertices[0][0]), 12*mesh->vertices.size(), 1, f);
	} else {
		// Generic code
		for (int i = 0; i < mesh->vertices.size(); i++) {
			fwrite(&(mesh->vertices[i][0]), 12, 1, f);
			if (!mesh->normals.empty() && write_norm)
				fwrite(&(mesh->normals[i][0]), 12, 1, f);
			if (!mesh->colors.empty() && write_color)
				fwrite(&(mesh->colors[i][0]), 3, 1, f);
			if (!mesh->confidences.empty() && write_conf)
				fwrite(&(mesh->confidences[i]), 4, 1, f);
		}
	}
	if (need_swap) {
		for (int i = 0; i < mesh->vertices.size(); i++) {
			swap_float(mesh->vertices[i][0]);
			swap_float(mesh->vertices[i][1]);
			swap_float(mesh->vertices[i][2]);
		}
		if (!mesh->normals.empty()) {
			for (int i = 0; i < mesh->normals.size(); i++) {
				swap_float(mesh->normals[i][0]);
				swap_float(mesh->normals[i][1]);
				swap_float(mesh->normals[i][2]);
			}
		}
		if (!mesh->confidences.empty()) {
			for (int i = 0; i < mesh->confidences.size(); i++)
				swap_float(mesh->confidences[i]);
		}
	}
}


// Write a bunch of faces to an ASCII file
static void write_faces_asc(TriMesh *mesh, FILE *f,
			    const char *before_face, const char *after_line)
{
	mesh->need_faces();
	for (int i = 0; i < mesh->faces.size(); i++) {
		fprintf(f, "%s%d %d %d%s\n", before_face, mesh->faces[i][0],
			mesh->faces[i][1], mesh->faces[i][2], after_line);
	}
}


// Write a bunch of faces to a binary file
static void write_faces_bin(TriMesh *mesh, FILE *f, bool need_swap,
			    int before_face_len, const char *before_face,
			    int after_face_len, const char *after_face)
{
	mesh->need_faces();
	if (need_swap) {
		for (int i = 0; i < mesh->faces.size(); i++) {
			swap_int(mesh->faces[i][0]);
			swap_int(mesh->faces[i][1]);
			swap_int(mesh->faces[i][2]);
		}
	}
	for (int i = 0; i < mesh->faces.size(); i++) {
		if (before_face_len)
			fwrite(before_face, before_face_len, 1, f);
		fwrite(&(mesh->faces[i][0]), 12, 1, f);
		if (after_face_len)
			fwrite(after_face, after_face_len, 1, f);
	}
	if (need_swap) {
		for (int i = 0; i < mesh->faces.size(); i++) {
			swap_int(mesh->faces[i][0]);
			swap_int(mesh->faces[i][1]);
			swap_int(mesh->faces[i][2]);
		}
	}
}


// Write tstrips to an ASCII file
static void write_strips_asc(TriMesh *mesh, FILE *f)
{
	for (int i = 0; i < mesh->tstrips.size(); i++) {
		fprintf(f, "%d ", mesh->tstrips[i]);
	}
	fprintf(f, "\n");
}


// Write tstrips to a binary file
static void write_strips_bin(TriMesh *mesh, FILE *f, bool need_swap)
{
	if (need_swap) {
		for (int i = 0; i < mesh->tstrips.size(); i++)
			swap_int(mesh->tstrips[i]);
	}
	fwrite(&(mesh->tstrips[0]), 4*mesh->tstrips.size(), 1, f);
	if (need_swap) {
		for (int i = 0; i < mesh->tstrips.size(); i++)
			swap_int(mesh->tstrips[i]);
	}
}


// Tesselate an arbitrary n-gon.  Appends triangles to "tris".
static void tess(const vector<point> &verts, const vector<int> &thisface,
		 vector<TriMesh::Face> &tris)
{
	if (thisface.size() < 3)
		return;
	if (thisface.size() == 3) {
		tris.push_back(TriMesh::Face(thisface[0],
					     thisface[1],
					     thisface[2]));
		return;
	}
	if (thisface.size() == 4) {
		// Try to generate good triangulation
		const point &p0 = verts[thisface[0]], &p1 = verts[thisface[1]];
		const point &p2 = verts[thisface[2]], &p3 = verts[thisface[3]];
		vec e[4] = { p1-p0, p2-p1, p3-p2, p0-p3 };
		normalize(e[0]); normalize(e[1]);
		normalize(e[2]); normalize(e[3]);
		float maxcos = e[3] DOT e[0];
		int maxind = 0;
		for (int j = 1; j < 3; j++) {
			float jcos = e[j-1] DOT e[j];
			if (jcos > maxcos) {
				maxcos = jcos;
				maxind = j;
			}
		}
		tris.push_back(TriMesh::Face(thisface[maxind],
					     thisface[(maxind+1)%4],
					     thisface[(maxind+2)%4]));
		tris.push_back(TriMesh::Face(thisface[maxind],
					     thisface[(maxind+2)%4],
					     thisface[(maxind+3)%4]));
		return;
	}

	// 5-gon or higher - just tesselate arbitrarily...
	for (int i = 2; i < thisface.size(); i++)
		tris.push_back(TriMesh::Face(thisface[0],
					     thisface[i-1],
					     thisface[i]));
}

void TriMesh::WriteAsciiPly(const char *filename)
{
	FILE *f = fopen(filename, "w");

	bool write_norm = false;

	write_ply_header(this, f, "ascii", write_norm);
	write_verts_asc(this, f, "", write_norm ? " " : 0, " ", " ", "");
	
	if (!this->tstrips.empty())
	{
		fprintf(f, "%d ", this->tstrips.size());
		this->convert_strips(TriMesh::TSTRIP_TERM);
		write_strips_asc(this, f);
		this->convert_strips(TriMesh::TSTRIP_LENGTH);
	}
	else 
	{
		write_faces_asc(this, f, "3 ", "");
	}

	fclose(f);
}


void TriMesh::GetNormal(float &nx, float &ny, float &nz, int index)
{
	assert(index < normals.size());
	
	nx = normals[index][0];
	ny = normals[index][1];
	nz = normals[index][2];
}