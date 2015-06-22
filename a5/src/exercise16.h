#pragma once

#include <array>

#include <QMatrix4x4>

#include "util/abstractexercise.h"

class PolygonalDrawable;

class Exercise16 : public AbstractExercise
{
public:
    Exercise16(const float animationFrame = 0.0f);
    virtual ~Exercise16();

    virtual const QString hints() const;

    void setProjectionMode(int index);

protected:
    void setupViews();
    virtual bool initialize() override;
    virtual void render() override;
    virtual bool onKeyPressed(QKeyEvent * event) override;
    virtual void resize(int width, int height) override;

protected:
    QScopedPointer<QOpenGLShaderProgram> m_program;
    PolygonalDrawable * m_drawable;
    std::array<QMatrix4x4, 6> m_views;
    QMatrix4x4 m_orthoProjection;
    int m_viewIndex;
};
