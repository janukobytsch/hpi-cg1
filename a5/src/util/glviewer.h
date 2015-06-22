#pragma once

#include <QWidget>

class Canvas;
class AbstractPainter;

class GLViewer : public QWidget
{
    Q_OBJECT

public:
    GLViewer(AbstractPainter *painter, QWidget *parent = NULL);
    virtual ~GLViewer();

    Canvas * canvas();

public slots:
    void animationFrameChanged(const float animationFrame);
    void keyPressed(QKeyEvent *event);

protected:
    Canvas *m_canvas;
    AbstractPainter *m_painter;

    void setup();
};
