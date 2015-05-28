#pragma once

#include "util/abstractexercise.h"
#include "util/icosahedron.h"
#include "util/unitcube.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class Exercise13 : public AbstractExercise
{
public:
    Exercise13();
    virtual ~Exercise13();

    virtual const QString hints() const override;

protected:
    virtual bool initialize() override;

    virtual void render() override;

    void drawEnvironment();

    virtual QMatrix4x4 applyBallTransformation(const int frame);
protected:
    GLuint m_textureID;

    QScopedPointer<QOpenGLShaderProgram> m_program0;

    Icosahedron m_sphere;
    UnitCube m_cube;
    QMatrix4x4 m_sphereScale;
    QScopedPointer<QOpenGLShaderProgram> m_program1;
};
