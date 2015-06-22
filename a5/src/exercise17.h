#pragma once

#include "util/abstractexercise.h"

enum GlobalDeformationMode
{
    None,
    Pinch,
    Mold,
    Twist,
    Bend
};

class Box;

class Exercise17 : public AbstractExercise
{
public:
    Exercise17(const float animationFrame = 0.0f);
    virtual ~Exercise17();

    virtual const QString hints() const override;

protected:
    QScopedPointer<QOpenGLShaderProgram> m_program;
    QMatrix4x4  m_boxTransform;
    Box         *m_box;
    GlobalDeformationMode   m_globalDefMode;
    
    virtual bool onKeyPressed(QKeyEvent *keyEvent) override;
    virtual bool initialize() override;
    virtual void render() override;
};
