#pragma once

#include <QWidget>

class QSurfaceFormat;

class Canvas;
class AbstractPainter;

class GLViewer : public QWidget
{
    Q_OBJECT

public:
    GLViewer(AbstractPainter *painter, const QSurfaceFormat & defaultFormat, QWidget *parent = NULL);
    virtual ~GLViewer();

    Canvas * canvas();

public slots:
    void keyPressed(QKeyEvent *event);

protected:
    Canvas *m_canvas;
    AbstractPainter *m_painter;

    void setup(const QSurfaceFormat & defaultFormat);
};
