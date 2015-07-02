#include "abstractexercise.h"

AbstractExercise::AbstractExercise(const float animationFrame)
    : AbstractPainter(animationFrame)
    , m_frame(0)
    , m_active(true)
{
}

AbstractExercise::~AbstractExercise()
{
}

void AbstractExercise::paint()
{
    glViewport(0, 0, m_viewport[0], m_viewport[1]);

    if (isActive())
    {
        ++m_frame;
    }

    render();
}

void AbstractExercise::resize(int width, int height)
{
    m_viewport[0] = width;
    m_viewport[1] = height;
}

void AbstractExercise::update()
{

}

void AbstractExercise::update(const QList<QOpenGLShaderProgram *> &programs)
{

}

void AbstractExercise::setActive(const bool active)
{
    m_active = active;
}

const bool AbstractExercise::isActive() const
{
    return m_active;
}

void AbstractExercise::enable()
{
    m_active = true;
}

void AbstractExercise::disable()
{
    m_active = false;
}

bool AbstractExercise::onKeyPressed(QKeyEvent * keyEvent)
{
    bool changed = false;
    if (keyEvent->key() & Qt::Key_Space)
    {
        m_active ? disable() : enable();
        keyEvent->accept();
        changed = true;
    }

    return changed || AbstractPainter::onKeyPressed(keyEvent);
}
