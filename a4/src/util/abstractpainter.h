#pragma once

#include <QList>
#include <QSize>
#include <QKeyEvent>

#include "openglfunctions.h"
#include "abstractcoordinateprovider.h"

class QStringList;
class QOpenGLShaderProgram;
class QOpenGLShader;

class Camera;
class AbstractSceneObject;

class AbstractPainter : public QObject, public AbstractCoordinateProvider
    , public OpenGLFunctions
{
    Q_OBJECT
public:
    enum PaintMode
    {
        NORMAL
    ,   WIREFRAME
    };

public:
    AbstractPainter();
    virtual ~AbstractPainter();

    void setCamera(Camera *camera);
    Camera *camera();

    /** return list of mandatory extensions (verify results in fatal if one is
        not supported).
    */
    virtual const QStringList& extensions() const;

    /** this should setup the opengl state and allocate all required static 
        resources (context is 3.2 core for this one).
    */
    virtual bool initialize();

    /** returns wether m_context is 0 or not
    */
    bool isValid() const;

    virtual void paint() = 0;
    virtual void resize(int width, int height) = 0;

    virtual void update() = 0;
    virtual void update(const QList<QOpenGLShaderProgram *> &programs) = 0;

    virtual bool onKeyPressed(QKeyEvent * event);

    // AbstractCoordinateProvider interface

    virtual float depthAt(const QPointF &windowCoordinates);

    virtual const QVector3D objAt(const QPointF &windowCoordinates);
    const QVector3D objAt(const QPointF &windowCoordinates, float depth);
    const QVector3D objAt(const QPointF &windowCoordinates, float depth, const QMatrix4x4 &viewProjectionInverted);

protected:
    Camera *m_camera;

    QList<QOpenGLShader*> m_shaders;

    GLuint getOrCreateTexture(const QString & fileName);
    QOpenGLShaderProgram* createBasicShaderProgram(const QString &vertexShaderFileName, const QString &fragmentShaderFileName);
};
