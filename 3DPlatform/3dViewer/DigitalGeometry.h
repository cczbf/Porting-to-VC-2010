
#ifndef DIGITAL_GEOMETRY_H
#define DIGITAL_GEOMETRY_H

class CMesh ;

class CMPt
{
public:
	CMPt(int a, int b): m_a(a), m_b(b){}
	virtual ~CMPt(){};
	
    bool operator==(const CMPt& rhs) const;
    int  GetA() const{return m_a;}
	int  GetB() const{return m_b;}

private: 
	int m_a;
	int m_b;
};

struct TiangleIndices
{
	int v1;
	int v2;
	int v3;
};

//Subdivision Mesh
CMesh* SubdivisionMesh(const CMesh &mesh);


#endif

