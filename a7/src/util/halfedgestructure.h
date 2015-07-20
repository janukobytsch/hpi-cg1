#pragma once    

#include <string>
#include <vector>

#include <QVector3D>


class PolygonalDrawable;

class HalfEdgeStructure
{
public:

    struct Face;

    struct HalfEdge 
    {
        HalfEdge * next;
        HalfEdge * prev;
        HalfEdge * opp;

        Face * face;

        QVector3D * vertex;
	    QVector3D normal;
    };

    struct Face
    {
        QVector3D normal; // Per-face normal
        HalfEdge * he;    // Pointer to a half edge that belongs to the face
    };

    typedef std::vector<HalfEdge> t_halfEdges;
    typedef std::vector<Face> t_faces;

    const t_halfEdges & halfEdges() const;
    const t_faces & faces() const;

    void calculatePerVertexNormals(const float threshold);

public:
    HalfEdgeStructure(PolygonalDrawable & drawable);
    virtual ~HalfEdgeStructure();

protected:
    void setup(PolygonalDrawable & drawable);
    
    void calculatePerFaceNormals();

protected:
    t_halfEdges m_halfEdges;
    t_faces m_faces;
};

//
//
//struct Vertex;
//struct HalfEdge;
//struct Face;
//struct Vec3;
//
//class Model
//{
//public:
//	Model();
//	virtual ~Model();
//		
//	void load(const std::string & filename);
//
//	void clear();
//
//	bool setupHalfEdges();
//
//	void calculateFaceNormals();
//	void calculateVertexNormals();
//
//	void render(bool useVertexNormals);
//
//	bool valid() const;
//
//protected:
//    void removeLineEnding(char * line);
//
//protected:
//	void parseFace();
//	void parseVertex();
//	void parseNormal();
//
//	std::vector<Vertex>   m_vertices;
//	std::vector<Face>     m_faces;
//	std::vector<Vec3>     m_loadedNormals;
//	std::vector<HalfEdge> m_halfEdges;
//
//	bool m_bValid; // true if the model could be loaded without errors
//};