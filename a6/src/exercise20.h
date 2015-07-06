#pragma once

#include "util/abstractexercise.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <glm/vec3.hpp>
#include <vector>

// CGAL
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_traits_with_normals_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

typedef CGAL::Simple_cartesian<double>     Kernel;
typedef CGAL::Polyhedron_traits_with_normals_3<Kernel> Traits;
typedef CGAL::Polyhedron_3<Traits>         Polyhedron;

class Exercise20 : public AbstractExercise, protected OpenGLFunctions
{
public:
    Exercise20(const float animationFrame = 0.0f);
    virtual ~Exercise20();

    virtual const QString hints() const override;

protected:
    enum class SubdivisionMode : unsigned char
    {
        MODE_A = 0,
        MODE_B = 1
    };

    enum class PolyhedronMode : unsigned char
    {
        ICOSAHEDRON = 0,
        TETRAHEDRON = 1
    };

    QScopedPointer<QOpenGLShaderProgram> m_program;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertices;
    QOpenGLBuffer m_normals;
    std::vector<glm::vec3> m_verticesRaw;
    std::vector<glm::vec3> m_normalsRaw;
    QMatrix4x4  m_boxTransform;
    bool m_wireframeMode;
    bool m_update;
    Polyhedron m_mesh;
    SubdivisionMode m_subdivisionMode;
    PolyhedronMode m_polyhedronMode;
    
    virtual bool onKeyPressed(QKeyEvent *keyEvent) override;
    virtual bool initialize() override;
    virtual void render() override;
    virtual void setAnimationFrame(const float animationFrame) override;

    Polyhedron createMesh();
    void prepareMesh(Polyhedron& poly);
    void updateMeshGL(Polyhedron& poly);
    void updateMesh();

    void buildPolyhedron(CGAL::Polyhedron_incremental_builder_3<Polyhedron::HalfedgeDS> builder, int const numVertices, int const numFacets, float vetices[][3], float facets[][3]);
};
