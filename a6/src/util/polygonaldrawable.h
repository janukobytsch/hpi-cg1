#pragma once

#include <QMap>
#include <QVector>

#include <QtOpenGL>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>


typedef QVector<unsigned int> t_uints;

typedef QVector<QVector3D> t_vec3s;
typedef QVector<QVector2D> t_vec2s;

class OpenGLFunctions;
class QOpenGLShaderProgram;


class PolygonalDrawable
{
public:
    PolygonalDrawable();
    virtual ~PolygonalDrawable();

    virtual void draw(OpenGLFunctions & gl);

    const GLenum mode() const;
    void setMode(const GLenum mode);

    const t_vec3s & vertices() const;
    t_vec3s & vertices();  
    const t_vec3s & normals() const;
    t_vec3s & normals();
    const t_vec2s & texcs() const;
    t_vec2s & texcs();
    const t_uints & indices() const;
    t_uints & indices();

    void retrieveNormals();

    void createVAO(QOpenGLShaderProgram * program);

    void updateBuffers(QOpenGLShaderProgram * program);

protected:
    t_vec3s m_vertices;
    t_vec3s m_normals;
    t_vec2s m_texcs;
    t_uints m_indices;

    GLenum  m_mode;

    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_normalBuffer;
    QOpenGLBuffer m_texcoordBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLVertexArrayObject m_vao;
};
