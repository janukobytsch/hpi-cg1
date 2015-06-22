// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 2
//                     - Aufgabe 6
//
// Musterlösung
//
// ======================================

#include "glviewer.h"

#include <iostream>

#include <QKeyEvent>
#include <QMatrix4x4>
#include <QTimer>
#include <QBoxLayout>

#include "openglfunctions.h"
#include "canvas.h"
#include "abstractpainter.h"

GLViewer::GLViewer(AbstractPainter *painter, QWidget *parent)
    : QWidget(parent)
    , m_painter(painter)
    , m_canvas(0)
{
    setup();
}

GLViewer::~GLViewer()
{
    m_canvas->setPainter(0);
    delete m_painter;
    delete m_canvas;
}

void GLViewer::setup()
{
    QSurfaceFormat format;

    format.setVersion(3, 2);
#ifdef __APPLE__
    format.setProfile(QSurfaceFormat::CoreProfile);
#else
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
#endif

    format.setDepthBufferSize(16);

    m_canvas = new Canvas(format);
    m_canvas->setContinuousRepaint(true, 0);
    m_canvas->setSwapInterval(Canvas::VerticalSyncronization);

    m_canvas->setPainter(m_painter);

    QWidget *widget = QWidget::createWindowContainer(m_canvas);
    widget->setMinimumSize(1, 1);
    widget->setAutoFillBackground(false); // Important for overdraw, not occluding the scene.
    widget->setFocusPolicy(Qt::StrongFocus);
    widget->setParent(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(widget);
    setLayout(layout);

    show();
}

void GLViewer::animationFrameChanged(const float animationFrame)
{
    m_canvas->animationFrameChanged(animationFrame);
}

void GLViewer::keyPressed(QKeyEvent *event)
{
    m_canvas->keyPressed(event);
}

Canvas * GLViewer::canvas()
{
    return m_canvas;
}
