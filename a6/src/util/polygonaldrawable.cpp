
#include "polygonaldrawable.h"

#include <cassert>

#include <QOpenGLShaderProgram>

#include "openglfunctions.h"


PolygonalDrawable::PolygonalDrawable()
: m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
}

PolygonalDrawable::~PolygonalDrawable()
{
}

void PolygonalDrawable::draw(OpenGLFunctions & gl)
{
    m_vao.bind();

    gl.glEnable(GL_DEPTH_TEST);
    gl.glEnable(GL_CULL_FACE);

    if (m_indices.size())
    {
        gl.glDrawElements(m_mode, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
    }
    else
    {
        gl.glDrawArrays(m_mode, 0, m_vertices.size());
    }

    gl.glDisable(GL_DEPTH_TEST);
    gl.glDisable(GL_CULL_FACE);
}

const GLenum PolygonalDrawable::mode() const
{
    return m_mode;
}

void PolygonalDrawable::setMode(const GLenum mode)
{
    m_mode = mode;
}

const t_vec3s & PolygonalDrawable::vertices() const
{
    return m_vertices;
}

t_vec3s & PolygonalDrawable::vertices()
{
    return m_vertices;
}

const t_vec3s & PolygonalDrawable::normals() const
{
    return m_normals;
}

t_vec3s & PolygonalDrawable::normals()
{
    return m_normals;
}

const t_vec2s & PolygonalDrawable::texcs() const
{
    return m_texcs;
}

t_vec2s & PolygonalDrawable::texcs()
{
    return m_texcs;
}

const t_uints & PolygonalDrawable::indices() const
{
    return m_indices;
}

t_uints & PolygonalDrawable::indices()
{
    return m_indices;
}

void PolygonalDrawable::retrieveNormals()
{
    assert(m_indices.size() % 3 == 0);

    unsigned int i0, i1, i2;
    QVector3D v[3], a, b, n;

    unsigned int m(0);
    for(int i = 0; i < m_indices.size(); ++i)
        if(m_indices[i] > m)
            m = m_indices[i];

    if(m_normals.size() < static_cast<int>(m))
        m_normals.resize(m + 1);

    for(int i = 0; i < m_indices.size(); i += 3)
    {
        i0 = m_indices[i + 0];
        i1 = m_indices[i + 1];
        i2 = m_indices[i + 2];

        a = (m_vertices[i2] - m_vertices[i0]).normalized();
        b = (m_vertices[i1] - m_vertices[i0]).normalized();

        n = a * b;

        m_normals[i0] = n;
        m_normals[i1] = n;
        m_normals[i2] = n;
    }
}

void PolygonalDrawable::createVAO(QOpenGLShaderProgram * program)
{
    m_vao.create();
    m_vao.bind();

    m_vertexBuffer = QOpenGLBuffer();
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_vertexBuffer.allocate(m_vertices.constData(), m_vertices.size() * sizeof(QVector3D));

    program->setAttributeBuffer("in_vertex", GL_FLOAT, 0, 3, 3 * sizeof(float));
    if (m_vertices.size())
    {
        program->enableAttributeArray("in_vertex");
    }

    m_normalBuffer = QOpenGLBuffer();
    m_normalBuffer.create();
    m_normalBuffer.bind();
    m_normalBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_normalBuffer.allocate(m_normals.constData(), m_normals.size() * sizeof(QVector3D));


    program->setAttributeBuffer("in_normal", GL_FLOAT, 0, 3, 3 * sizeof(float));
    if (m_normals.size())
    {
        program->enableAttributeArray("in_normal");
    }

    m_texcoordBuffer.create();
    m_texcoordBuffer.bind();
    m_texcoordBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_texcoordBuffer.allocate(m_texcs.constData(), m_texcs.size() * sizeof(QVector2D));

    program->setAttributeBuffer("in_texcoord", GL_FLOAT, 0, 2, 2 * sizeof(float));
    if (m_texcs.size())
    {
        program->enableAttributeArray("in_texcoord");
    }

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_indexBuffer.allocate(m_indices.constData(), m_indices.size() * sizeof(unsigned int));
}

void PolygonalDrawable::updateBuffers(QOpenGLShaderProgram * program)
{
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertices.constData(), m_vertices.size() * sizeof(QVector3D));

    if (m_vertices.size())
    {
        program->enableAttributeArray("in_vertex");
    }
    else
    {
        program->disableAttributeArray("in_vertex");
    }

    m_normalBuffer.bind();
    m_normalBuffer.allocate(m_normals.constData(), m_normals.size() * sizeof(QVector3D));

    if (m_normals.size())
    {
        program->enableAttributeArray("in_normal");
    }
    else
    {
        program->disableAttributeArray("in_normal");
    }

    m_texcoordBuffer.bind();
    m_texcoordBuffer.allocate(m_texcs.constData(), m_texcs.size() * sizeof(QVector2D));

    if (m_texcs.size())
    {
        program->enableAttributeArray("in_texcoord");
    }
    else
    {
        program->disableAttributeArray("in_texcoord");
    }

    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indices.constData(), m_indices.size() * sizeof(unsigned int));
}
