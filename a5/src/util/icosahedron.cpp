#include "icosahedron.h"

#include <iterator>
#include <algorithm>

#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>

std::array<glm::vec3, 12> Icosahedron::vertices()
{
    static const float t = (1.f + glm::sqrt(5.f)) * 0.5f; // 2.118
    static const float i = glm::inversesqrt(t * t + 1.f);  // 0.427
    static const float a = t * i;                     // 0.904

    // icosahedron vertices (normalized) form three orthogonal golden rectangles
    // http://en.wikipedia.org/wiki/Icosahedron#Cartesian_coordinates

    return std::array<glm::vec3, 12>{{
            glm::vec3(-i, a, 0)
                , glm::vec3(i, a, 0)
                , glm::vec3(-i, -a, 0)
                , glm::vec3(i, -a, 0)
                , glm::vec3(0, -i, a)
                , glm::vec3(0, i, a)
                , glm::vec3(0, -i, -a)
                , glm::vec3(0, i, -a)
                , glm::vec3(a, 0, -i)
                , glm::vec3(a, 0, i)
                , glm::vec3(-a, 0, -i)
                , glm::vec3(-a, 0, i)
    }};
}

std::array<Icosahedron::Face, 20> Icosahedron::indices()
{
    return std::array<Face, 20>{{
        Face{{  0, 11,  5 }}
    ,   Face{{  0,  5,  1 }}
    ,   Face{{  0,  1,  7 }}
    ,   Face{{  0,  7, 10 }}
    ,   Face{{  0, 10, 11 }}

    ,   Face{{  1,  5,  9 }}
    ,   Face{{  5, 11,  4 }}
    ,   Face{{ 11, 10,  2 }}
    ,   Face{{ 10,  7,  6 }}
    ,   Face{{  7,  1,  8 }}

    ,   Face{{  3,  9,  4 }}
    ,   Face{{  3,  4,  2 }}
    ,   Face{{  3,  2,  6 }}
    ,   Face{{  3,  6,  8 }}
    ,   Face{{  3,  8,  9 }}

    ,   Face{{  4,  9,  5 }}
    ,   Face{{  2,  4, 11 }}
    ,   Face{{  6,  2, 10 }}
    ,   Face{{  8,  6,  7 }}
    ,   Face{{  9,  8,  1 }}
    }};
}

Icosahedron::Icosahedron(const GLsizei iterations, const GLint positionLocation, const GLint normalLocation)
: m_indices(QOpenGLBuffer::IndexBuffer)
, m_iterations(iterations)
, m_size(0)
, m_positionLocation(positionLocation)
, m_normalLocation(normalLocation)
{
}

Icosahedron::~Icosahedron()
{
}

void Icosahedron::initialize(QOpenGLShaderProgram & program)
{
    auto v(vertices());
    auto i(indices());

    std::vector<glm::vec3> vertices(v.begin(), v.end());
    std::vector<Face> indices(i.begin(), i.end());

    refine(vertices, indices, static_cast<char>(glm::clamp(m_iterations, 0, 8)));

    m_vao.create();
    m_vao.bind();

    m_vertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertices.create();
    m_vertices.bind();

    m_vertices.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertices.allocate(vertices.data(), vertices.size() * sizeof(std::vector<glm::vec3>::value_type));

    program.enableAttributeArray("in_vertex");
    program.setAttributeBuffer("in_vertex", GL_FLOAT, 0, 3, 3 * sizeof(float));

    m_indices = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_indices.create();
    m_indices.bind();

    m_indices.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indices.allocate(indices.data(), indices.size() * std::tuple_size<Face>::value * sizeof(Face::value_type));

    m_size = static_cast<GLsizei>(indices.size() * std::tuple_size<Face>::value);
}

void Icosahedron::draw(OpenGLFunctions & functions)
{
    draw(GL_TRIANGLES, functions);
}

void Icosahedron::draw(const GLenum mode, OpenGLFunctions & functions)
{
    m_vao.bind();

    functions.glDrawElements(mode, m_size, GL_UNSIGNED_INT, nullptr);
}

void Icosahedron::refine(
    std::vector<glm::vec3> & vertices
,   std::vector<Face> & indices
,   const unsigned char levels)
{
    std::unordered_map<uint, GLushort> cache;

    for(int i = 0; i < levels; ++i)
    {
        const int size(static_cast<int>(indices.size()));

        for(int f = 0; f < size; ++f)
        {
            Face & face = indices[f];

            const GLushort a(face[0]);
            const GLushort b(face[1]);
            const GLushort c(face[2]);

            const GLushort ab(split(a, b, vertices, cache));
            const GLushort bc(split(b, c, vertices, cache));
            const GLushort ca(split(c, a, vertices, cache));

            face = { ab, bc, ca };

            indices.emplace_back(Face{ a, ab, ca });
            indices.emplace_back(Face{ b, bc, ab });
            indices.emplace_back(Face{ c, ca, bc });
        }
    }
}

GLushort Icosahedron::split(
    const GLushort a
,   const GLushort b
,   std::vector<glm::vec3> & points
,   std::unordered_map<uint, GLushort> & cache)
{
    const bool aSmaller(a < b);

    const uint smaller(aSmaller ? a : b);
    const uint greater(aSmaller ? b : a);
    const uint hash((smaller << 16) + greater);

    auto h(cache.find(hash));
    if(cache.end() != h)
        return h->second;

    points.push_back(normalize((points[a] + points[b]) * .5f));

    const GLushort i = static_cast<GLushort>(points.size() - 1);

    cache[hash] = i;

    return i;
}
