#pragma once

#include <array>

#include <QMatrix4x4>

#include "util/abstractexercise.h"

class PolygonalDrawable;

class Exercise18 : public AbstractExercise
{
public:
    Exercise18(const float animationFrame = 0.0f);
    virtual ~Exercise18();

    virtual const QString hints() const;

    void setProjectionMode(int index);

protected:
    virtual bool initialize() override;
    virtual void render() override;

protected:
    QScopedPointer<QOpenGLShaderProgram> m_program;
    PolygonalDrawable * m_drawable;
};
