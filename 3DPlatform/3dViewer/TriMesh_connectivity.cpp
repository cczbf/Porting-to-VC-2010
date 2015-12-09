/*
Szymon Rusinkiewicz
Princeton University

TriMesh_connectivity.cc
Manipulate data structures that describe connectivity between faces and verts.
*/


#include <stdio.h>
#include "TriMesh.h"
#include <vector>
#include <algorithm>

using namespace std;
//using std::find;


// Find the direct neighbors of each vertex
void TriMesh::need_neighbors()
{
	if (!neighbors.empty())
		return;
	need_faces();

	dprintf(stderr, "Finding vertex neighbors... "); dflush(stderr);

	vector<int> numneighbors(vertices.size());
	int i;
	for (i = 0; i < faces.size(); i++) {
		numneighbors[faces[i][0]]++;
		numneighbors[faces[i][1]]++;
		numneighbors[faces[i][2]]++;
	}

	neighbors.resize(vertices.size());
	for (i = 0; i < vertices.size(); i++)
		neighbors[i].reserve(numneighbors[i]+2); // Slop for boundaries

	for (i = 0; i < faces.size(); i++) {
		for (int j = 0; j < 3; j++) {
			vector<int> &me = neighbors[faces[i][j]];
			int n1 = faces[i][(j+1)%3];
			int n2 = faces[i][(j+2)%3];

			if (find(me.begin(), me.end(), n1) == me.end())
				me.push_back(n1);
			if (find(me.begin(), me.end(), n2) == me.end())
				me.push_back(n2);
		}
	}

	dprintf(stderr, "Done.\n");
}


// Find the faces touching each vertex
void TriMesh::need_adjacentfaces()
{
	if (!adjacentfaces.empty())
		return;
	need_faces();

	dprintf(stderr, "Finding vertex to triangle maps... "); dflush(stderr);

	vector<int> numadjacentfaces(vertices.size());
	int i;
	for (i = 0; i < faces.size(); i++) {
		numadjacentfaces[faces[i][0]]++;
		numadjacentfaces[faces[i][1]]++;
		numadjacentfaces[faces[i][2]]++;
	}

	adjacentfaces.resize(vertices.size());
	for (i = 0; i < vertices.size(); i++)
		adjacentfaces[i].reserve(numadjacentfaces[i]);

	for (i = 0; i < faces.size(); i++) {
		for (int j = 0; j < 3; j++)
			adjacentfaces[faces[i][j]].push_back(i);
	}

	dprintf(stderr, "Done.\n");
}


// Find the face across each edge from each other face (-1 on boundary)
// If topology is bad, not necessarily what one would expect...
void TriMesh::need_across_edge()
{
	if (!across_edge.empty())
		return;
	need_adjacentfaces();

	dprintf(stderr, "Finding across-edge maps... "); dflush(stderr);

	across_edge.resize(faces.size(), Face(-1,-1,-1));
	for (int i = 0; i < faces.size(); i++) {
		for (int j = 0; j < 3; j++) {
			if (across_edge[i][j] != -1)
				continue;
			int v1 = faces[i][(j+1)%3];
			int v2 = faces[i][(j+2)%3];
			const vector<int> &a1 = adjacentfaces[v1];
			const vector<int> &a2 = adjacentfaces[v2];
			for (int k1 = 0; k1 < a1.size(); k1++) {
				int other = a1[k1];
				if (other == i)
					continue;
				vector<int>::const_iterator it =
					find(a2.begin(), a2.end(), other);
				if (it == a2.end())
					continue;
				int ind = (faces[other].indexof(v1)+1)%3;
				if (faces[other][(ind+1)%3] != v2)
					continue;
				across_edge[i][j] = other;
				across_edge[other][ind] = i;
				break;
			}
		}
	}

	dprintf(stderr, "Done.\n");
}

