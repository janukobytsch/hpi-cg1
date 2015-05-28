#pragma once

#include "util/abstractexercise.h"

#include <QKeyEvent>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QScopedPointer>

#include "util/unitcube.h"

class Exercise12 : public AbstractExercise
{
public:
    Exercise12();
    virtual ~Exercise12();

    virtual const QString hints() const override;

protected:
    virtual bool initialize() override;
    virtual void render() override;

protected:
    QMatrix4x4 rotateClockwise(int frame);

protected:
    QMatrix4x4  m_cubeTransform;
    QScopedPointer<QOpenGLShaderProgram> m_program;
    UnitCube m_cube;
};
