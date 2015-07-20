
#include <cassert>

#include "halfedgestructure.h"
#include "polygonaldrawable.h"
#include "mathmacros.h"


HalfEdgeStructure::HalfEdgeStructure(PolygonalDrawable & drawable)
{
    setup(drawable);
}

HalfEdgeStructure::~HalfEdgeStructure()
{
}

const HalfEdgeStructure::t_halfEdges & HalfEdgeStructure::halfEdges() const
{
    return m_halfEdges;
}

const HalfEdgeStructure::t_faces & HalfEdgeStructure::faces() const
{
    return m_faces;
}

void HalfEdgeStructure::setup(PolygonalDrawable & drawable)
{
    if(drawable.indices().isEmpty())
		return;

    const int nTriangles(drawable.indices().size() / 3);
    m_faces.resize(nTriangles);

    // TODO: there is strict triangle support only...
    assert(drawable.indices().size() % 3 == 0);
  
	// Map of edges (given by two indices) -> opposite half-edge

    typedef std::map<std::pair<QVector3D const *, QVector3D const *>, HalfEdge*> t_opp_map;
    t_opp_map opp;

    const int he_count = nTriangles * 3;
	m_halfEdges.resize(he_count);

	for(int k = 0; k < nTriangles; k++) 
	{
        m_faces[k].he = &m_halfEdges[k * 3];

		for (int i = 0; i < 3; ++i) 
		{
            const int j(k * 3 + i);

            m_halfEdges[j].prev = &m_halfEdges[(i == 0) ? j + 2 : j - 1];
			m_halfEdges[j].next = &m_halfEdges[(i == 2) ? j - 2 : j + 1];
			m_halfEdges[j].opp = NULL;

            m_halfEdges[j].face = &m_faces[k];

            const int l(drawable.indices()[j]);

            m_halfEdges[j].vertex = &(drawable.vertices()[l]);
            m_halfEdges[j].normal =   drawable.normals()[l];
        }

        // set opposite-pointers

        for (int i = 0; i < 3; ++i) 
		{
            const int j(k * 3 + i);

			QVector3D const * v0 = m_halfEdges[j].vertex;
			QVector3D const * v1 = m_halfEdges[j].next->vertex;

			// Check if opposite half-edge is already stored
			t_opp_map::iterator p = opp.find(t_opp_map::key_type(v0, v1));
			if(p == opp.end()) 
			{   // no: Add half-edge in opposite direction
				opp[t_opp_map::key_type(v1, v0)] = &m_halfEdges[j];
			} 
			else 
			{
				// yes: Set opposite-pointers of both half-edges
				p->second->opp = &m_halfEdges[j];
                m_halfEdges[j].opp = p->second;

				opp.erase(p);
            }
        }
    }

    calculatePerFaceNormals();
    calculatePerVertexNormals(0.f);
}

void HalfEdgeStructure::calculatePerFaceNormals()
{
    HalfEdgeStructure::t_faces::iterator i = m_faces.begin();
    const HalfEdgeStructure::t_faces::const_iterator iEnd = m_faces.end();

    for(; i != iEnd; ++i)
	{
        // calc face normal
        const QVector3D v0 = *(i->he->prev->vertex) - *(i->he->vertex);
        const QVector3D v1 = *(i->he->next->vertex) - *(i->he->vertex);

        i->normal = QVector3D::crossProduct(v1, v0).normalized();
	}
}

void HalfEdgeStructure::calculatePerVertexNormals(const float threshold)
{
    const int size(static_cast<int>(m_halfEdges.size()));

	for(int i = 0; i < size; ++i) 
	{
		HalfEdge * edge = &m_halfEdges[i];

        std::vector<QVector3D> normals;

        // start with given edge
		HalfEdge * e = edge;
		do 
		{
            normals.push_back(e->face->normal);

			e = e->prev;
			if (!e) 
                break;

            e = e->opp;
			if (!e) 
                break;
		} 
        while(e && e != edge);

        if(e != edge)
            e = edge;

        while(e != edge)
        {
			e = e->opp;
			if (!e) 
                break;

            e = e->next;
			if (!e) 
                break;

            normals.push_back(e->face->normal);
        } 

		// initialize vertex normal with face normal
        edge->normal = normals[0];
        const int n = static_cast<int>(normals.size());

		// go around each neighbour face

        int count = 1;
        int last  = 0;

		for(int j = 1; j < n; ++j) 
		{
			// get normals of current and previous face
			const QVector3D n0 = normals[j - 1];
			const QVector3D n1 = normals[j    ];

			// calculate angle between faces
            const double angle = _deg(acos(QVector3D::dotProduct(n0, n1)));

			if(angle < threshold) 
			{
				// smooth edge, take normal vector into account
                edge->normal += normals[j];
				++count;
				last = j;
			} 
            else
				break;
		}

		// go around backwards
		for(int j = n - 1; j > last; --j) 
		{
			// get normals of current and previous face
			const QVector3D n0 = (j + 1 >= n) ? normals[0] : normals[j + 1];
			const QVector3D n1 = normals[j];

			// calculate angle between faces
			const double angle = _deg(acos(QVector3D::dotProduct(n0, n1)));

            if(angle < threshold)
            { 
                // smooth edge, take normal vector into account
                edge->normal += normals[j];
				++count;
			} 
            else 
				break;
		}

        edge->normal /= static_cast<float>(count);
    }
}
