#include "abstractpainter.h"

#include <cassert>
#include <QStringList>

#include "camera.h"

AbstractPainter::AbstractPainter()
    : m_camera(0)
    , m_mode(WIREFRAME)
{
}
 
AbstractPainter::~AbstractPainter()
{
}

const QStringList& AbstractPainter::extensions() const
{
    static QStringList extensions;
    
    return extensions;
}

void AbstractPainter::setCamera(Camera *camera)
{
    if (m_camera == camera)
        return;

    m_camera = camera;
    update();
}

Camera *AbstractPainter::camera()
{
    return m_camera;
}

float AbstractPainter::depthAt(const QPointF &windowCoordinates)
{
    const GLint x(static_cast<GLint>(windowCoordinates.x()));
    const GLint y(static_cast<GLint>(windowCoordinates.y()));

    assert(m_camera);

    const GLint w(static_cast<GLint>(m_camera->viewport().width()));
    const GLint h(static_cast<GLint>(m_camera->viewport().height()));

    if (x >= w || y >= h)
        return 1.f;

    GLfloat z;
    glReadPixels(x, h - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, reinterpret_cast<void*>(&z));

    return z;
}

const QVector3D AbstractPainter::objAt(const QPointF &windowCoordinates)
{
    return objAt(windowCoordinates, depthAt(windowCoordinates), m_camera->viewProjectionInverted());
}

const QVector3D AbstractPainter::objAt(const QPointF &windowCoordinates, const float depth)
{
    return objAt(windowCoordinates, depth, m_camera->viewProjectionInverted());
}

const QVector3D AbstractPainter::objAt(const QPointF & windowCoordinates, const float depth, const QMatrix4x4 &viewProjectionInverted)
{
    const GLint x(static_cast<GLint>(windowCoordinates.x()));
    const GLint y(static_cast<GLint>(windowCoordinates.y()));

    // transform viewport to [-1;+1] (including z!)

    const float w = 2.0f / static_cast<float>(m_camera->viewport().width());
    const float h = 2.0f / static_cast<float>(m_camera->viewport().height());

    const QVector4D p = QVector4D(x * w - 1.f, 1.f - y * h, depth * 2.f - 1.f, 1.f);

    // unproject this point back to object space
    return (viewProjectionInverted * p).toVector3DAffine();
}

bool AbstractPainter::onKeyPressed(int keyCode)
{
    // Empty by default. Override in derived classes.

    // Do not repaint
    return false;
}

void AbstractPainter::setMode(PaintMode mode)
{
    m_mode = mode;
}

AbstractPainter::PaintMode AbstractPainter::mode() const
{
    return m_mode;
}
