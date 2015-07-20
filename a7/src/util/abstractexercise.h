#pragma once

#include "abstractpainter.h"

#include <QKeyEvent>
#include <QMatrix4x4>

class AbstractExercise : public AbstractPainter
{
public:
    AbstractExercise();
    virtual ~AbstractExercise();

    virtual const QString hints() const = 0;

protected:
    virtual void render() = 0;

    virtual void paint() override;

    virtual void resize(int width, int height) override;

    virtual void update() override;
    virtual void update(const QList<QOpenGLShaderProgram *> &programs) override;

    virtual bool onKeyPressed(QKeyEvent * event);

    void setActive(const bool active);
    const bool isActive() const;

    virtual void enable();
    virtual void disable();

protected:
    int m_viewport[2];
    int m_frame;
    bool m_active;
};
