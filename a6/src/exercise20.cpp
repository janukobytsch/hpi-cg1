// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 6
//                     - Aufgabe 20
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

#include "exercise20.h"

#include "util/camera.h"

#include <iostream>
#include <glm/geometric.hpp>

// cgal
#include <CGAL/Subdivision_method_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

typedef Polyhedron::Halfedge_around_facet_circulator HF_circulator;
typedef Kernel::Point_3 Point_3;

// taken from cgal
// http://doc.cgal.org/latest/Polyhedron/Polyhedron_2polyhedron_prog_normals_8cpp-example.html
struct Normal_vector {
    template <class Facet>
    typename Facet::Plane_3 operator()(Facet& f) {
        typename Facet::Halfedge_handle h = f.halfedge();
        // Facet::Plane_3 is the normal vector type. We assume the
        // CGAL Kernel here and use its global functions.
        return CGAL::cross_product(
            h->next()->vertex()->point() - h->vertex()->point(),
            h->next()->next()->vertex()->point() - h->next()->vertex()->point());
    }
};

Exercise20::Exercise20(const float animationFrame)
    : AbstractExercise(animationFrame)
    , m_wireframeMode(true)
    , m_update(true)
    , m_subdivisionMode(SubdivisionMode::MODE_A)
    , m_polyhedronMode(PolyhedronMode::ICOSAHEDRON)
{
    //
}

Exercise20::~Exercise20()
{
    //
}

const QString Exercise20::hints() const
{
    return QString("Use [w] to toggle wireframe mode, [s] to toggle subdivision mode, [p] to toggle polyhedron mode.");
}

bool Exercise20::initialize()
{
    initializeOpenGLFunctions();

    m_program.reset(createBasicShaderProgram("data/mesh.vert", "data/mesh.frag"));
    m_program->bind();

    m_vao.create();
    m_vao.bind();

    m_vertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertices.create();
    m_vertices.bind();
    m_vertices.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_program->enableAttributeArray("in_vertex");
    m_program->setAttributeBuffer("in_vertex", GL_FLOAT, 0, 3, 3 * sizeof(float));

    m_normals = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_normals.create();
    m_normals.bind();
    m_normals.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_program->enableAttributeArray("in_normal");
    m_program->setAttributeBuffer("in_normal", GL_FLOAT, 0, 3, 3 * sizeof(float));

    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

void Exercise20::updateMeshGL(Polyhedron& poly)
{
    m_vao.bind();
    m_vertices.bind();
    m_vertices.allocate(m_verticesRaw.data(), m_verticesRaw.size() * sizeof(glm::vec3));

    m_normals.bind();
    m_normals.allocate(m_normalsRaw.data(), m_normalsRaw.size() * sizeof(glm::vec3));
}

void Exercise20::buildPolyhedron(CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS> builder, int const numVertices, int const numFacets, float vertices[][3],
                                 float facets[][3])
{
    const int numHalfedges = 2 * (numVertices + numFacets - 2);

    builder.begin_surface(numVertices, numFacets, numHalfedges);
    for (int i = 0; i < numVertices; ++i)
    {
        builder.add_vertex(Point_3(vertices[i][0], vertices[i][1], vertices[i][2]));
    }

    for(int i = 0; i < numFacets; ++i)
    {
        builder.begin_facet();
        for(int j = 0; j < 3; ++j)
        {
            builder.add_vertex_to_facet(facets[i][j]);
        }
        builder.end_facet();
    }
    builder.end_surface();
}

Polyhedron Exercise20::createMesh()
{
    Polyhedron poly;
    CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS> builder(poly.hds(), true);

    if (m_polyhedronMode == PolyhedronMode::ICOSAHEDRON)
    {
        const int numVertices = 12;
        const int numFacets = 20;
        const float r = static_cast<float>(0.5f * (1 + sqrt(5)));

        float vertices[numVertices][3] = {
                0.0, 1.0, r,
                0.0, 1.0, -r,
                0.0, -1.0, r,
                0.0, -1.0, -r,
                1.0, r, 0.0,
                1.0, -r, 0.0,
                -1.0, r, 0.0,
                -1.0, -r, 0.0,
                r, 0.0, 1.0,
                r, 0.0, -1.0,
                -r, 0.0, 1.0,
                -r, 0.0, -1.0
        };
        float facets[numFacets][3] = {
                2, 10, 0,
                2, 0, 8,
                6, 0, 10,
                0, 6, 4,
                8, 0, 4,
                4, 9, 8,
                4, 6, 1,
                4, 1, 9,
                10, 11, 6,
                11, 1, 6,
                1, 3, 9,
                1, 11, 3,
                3, 5, 9,
                9, 5, 8,
                8, 5, 2,
                11, 7, 3,
                7, 11, 10,
                7, 10, 2,
                7, 2, 5,
                7, 5, 3
        };

        buildPolyhedron(builder, numVertices, numFacets, vertices, facets);
    }
    else
    {
        const int numVertices = 4;
        const int numFacets = 4;

        float vertices[numVertices][3] = {
                1.0, 1.0, 1.0,
                1.0, -1.0, -1.0,
                -1.0, 1.0, -1.0,
                -1.0, -1.0, 1.0
        };
        float facets[numFacets][3] = {
                0, 1, 2,
                0, 3, 1,
                0, 2, 3,
                1, 3, 2
        };

        buildPolyhedron(builder, numVertices, numFacets, vertices, facets);
    }

    return poly;
}

void Exercise20::prepareMesh(Polyhedron& poly)
{
    Point_3 p3Vertex;
    glm::vec3 normal, vertex;
    const int numIterations = m_animationFrame * 8;

    if (m_subdivisionMode == SubdivisionMode::MODE_A)
    {
        CGAL::Subdivision_method_3::Loop_subdivision(poly, numIterations);
    }
    else
    {
        CGAL::Subdivision_method_3::Sqrt3_subdivision(poly, numIterations);
    }

    std::transform(poly.facets_begin(), poly.facets_end(), poly.planes_begin(), Normal_vector());

    m_verticesRaw.clear();
    m_normalsRaw.clear();

    // Iterate over the half-edge-structure of the given polyhedron to fill the vertex array
    for (auto currentFacet = poly.facets_begin(); currentFacet != poly.facets_end(); ++currentFacet)
    {
        auto p3Normal = currentFacet->plane();
        normal = glm::vec3(p3Normal.x(), p3Normal.y(), p3Normal.z());

        // Iterate over vertices contained in current facet
        for (auto currentVertex = currentFacet->facet_begin(); ;)
        {
            p3Vertex = currentVertex->vertex()->point();

            // The vertices should be normalized to a length of 2
            vertex = glm::normalize(glm::vec3(p3Vertex.x(), p3Vertex.y(), p3Vertex.z()));
            vertex *= 2;

            // The normals are defined per face. Thus, each vertex uses the face normal of its corresponding face.
            m_normalsRaw.push_back(normal);
            m_verticesRaw.push_back(vertex);

            ++currentVertex;
            if (currentVertex== currentFacet->facet_begin())
            {
                break;
            }
        }
    }
}

void Exercise20::setAnimationFrame(const float animationFrame)
{
    AbstractPainter::setAnimationFrame(animationFrame);

    m_update = true;
}

void Exercise20::updateMesh()
{
    m_mesh = createMesh();
    prepareMesh(m_mesh);
    updateMeshGL(m_mesh);
}

void Exercise20::render()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    if(m_update)
    {
        updateMesh();
        m_update = false;
    }

    if(m_verticesRaw.empty())
        return;

    m_program->setUniformValue("lightsource", QVector3D(0.0, 10.0, 0.0));
    m_program->setUniformValue("viewprojection", camera()->viewProjection());

    m_program->setUniformValue("normalMatrix", (camera()->view()).normalMatrix());
    m_program->setUniformValue("color", QVector4D(1.0, 0.0, 0.0, 1.0));

    m_vao.bind();

    if(m_wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    this->glDrawArrays(GL_TRIANGLES, 0, m_verticesRaw.size());

    if(m_wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool Exercise20::onKeyPressed(QKeyEvent *keyEvent)
{
    bool changed = false;

    switch(keyEvent->key())
    {
        case Qt::Key_W:
            m_wireframeMode = !m_wireframeMode;
            break;
        case Qt::Key_S:
            m_subdivisionMode = (m_subdivisionMode == SubdivisionMode::MODE_A) ? SubdivisionMode::MODE_B : SubdivisionMode::MODE_A;
            m_update = true;
            break;
        case Qt::Key_P:
            m_polyhedronMode = (m_polyhedronMode == PolyhedronMode::ICOSAHEDRON) ? PolyhedronMode::TETRAHEDRON : PolyhedronMode::ICOSAHEDRON;
            m_update = true;
            break;
        default:
            break;
    }

    return changed || AbstractExercise::onKeyPressed(keyEvent);
}
