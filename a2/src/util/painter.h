#pragma once

#include <QMatrix4x4>
#include <QMap>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "abstractpainter.h"

class QOpenGLShader;
class QOpenGLShaderProgram;

class Camera;

class Painter : public AbstractPainter
{
public:
    Painter();
    virtual ~Painter();

    virtual bool initialize();
    
    virtual void paint();
    virtual void resize(int width, int height);

    virtual void update();
    virtual void update(const QList<QOpenGLShaderProgram *> &programs);

    virtual bool onKeyPressed(int keyCode);

protected:
    Camera *m_camera;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertices;

    QMatrix4x4 m_transform;

    QOpenGLShaderProgram* m_program;
    QList<QOpenGLShader*> m_shaders;

    GLuint m_height;
    GLuint m_interpolationMode;

    virtual void render() = 0;
    GLuint getOrCreateTexture(const QString & fileName);
    QOpenGLShaderProgram* createBasicShaderProgram(const QString &vertexShaderFileName, const QString &fragmentShaderFileName);

};
