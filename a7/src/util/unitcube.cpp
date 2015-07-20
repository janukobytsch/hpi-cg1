#include "unitcube.h"

#include <iterator>
#include <algorithm>

#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>

std::array<glm::vec3, 8> UnitCube::vertices()
{
    return std::array<glm::vec3, 8>{{
        glm::vec3(-1, -1,  1)
    ,   glm::vec3( 1, -1,  1)
    ,   glm::vec3( 1,  1,  1)
    ,   glm::vec3(-1,  1,  1)
    ,   glm::vec3(-1, -1, -1)
    ,   glm::vec3( 1, -1, -1)
    ,   glm::vec3( 1,  1, -1)
    ,   glm::vec3(-1,  1, -1)
    }};
}

std::array<UnitCube::Face, 12> UnitCube::indices()
{
    return std::array<Face, 12>{{
        Face{{ 0, 1, 2 }}
    ,   Face{{ 2, 3, 0 }}
    ,   Face{{ 3, 2, 6 }}
    ,   Face{{ 6, 7, 3 }}
    ,   Face{{ 7, 6, 5 }}
    ,   Face{{ 5, 4, 7 }}
    ,   Face{{ 4, 5, 1 }}
    ,   Face{{ 1, 0, 4 }}
    ,   Face{{ 4, 0, 3 }}
    ,   Face{{ 3, 7, 4 }}
    ,   Face{{ 1, 5, 6 }}
    ,   Face{{ 6, 2, 1 }}
    }};
}

UnitCube::UnitCube()
{
}

UnitCube::~UnitCube()
{
}

void UnitCube::initialize(QOpenGLShaderProgram & program)
{
    static auto v(vertices());

    std::vector<glm::vec3> vertices(36);
    std::vector<glm::vec3> normals(36);

    int i = 0;
    for (const Face & face : indices())
    {
        vertices[i] = v[face[0]];
        vertices[i+1] = v[face[1]];
        vertices[i+2] = v[face[2]];

        glm::vec3 v0 = vertices[i+1] - vertices[i];
        glm::vec3 v1 = vertices[i+2] - vertices[i];

        glm::vec3 normal = glm::normalize(glm::cross(v0, v1));

        normals[i] = normal;
        normals[i+1] = normal;
        normals[i+2] = normal;

        i += 3;
    }

    m_vao.create();
    m_vao.bind();

    m_vertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertices.create();
    m_vertices.bind();

    m_vertices.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertices.allocate(vertices.data(), vertices.size() * sizeof(glm::vec3));

    program.enableAttributeArray("in_vertex");
    program.setAttributeBuffer("in_vertex", GL_FLOAT, 0, 3, 3 * sizeof(float));

    m_normals = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_normals.create();
    m_normals.bind();

    m_normals.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_normals.allocate(normals.data(), normals.size() * sizeof(glm::vec3));

    program.enableAttributeArray("in_normal");
    program.setAttributeBuffer("in_normal", GL_FLOAT, 0, 3, 3 * sizeof(float));
}

void UnitCube::draw(OpenGLFunctions & functions)
{
    draw(GL_TRIANGLES, functions);
}

void UnitCube::draw(const GLenum mode, OpenGLFunctions & functions)
{
    m_vao.bind();

    functions.glDrawArrays(mode, 0, 36);
}
