#pragma once

#include <QWindow>

#include <QList>
#include <QScopedPointer>

#include "openglfunctions.h"

class QOpenGLContext;
class QSurfaceFormat;
class QBasicTimer;
class QTimerEvent;
class QKeyEvent;

class AbstractPainter;
class Camera;
class Navigation;

class Canvas : public QWindow
{
    Q_OBJECT

public:
    enum SwapInterval
    {
        NoVerticalSyncronization        =  0
    ,   VerticalSyncronization          =  1 ///< WGL_EXT_swap_control, GLX_EXT_swap_control, GLX_SGI_video_sync
    ,   AdaptiveVerticalSyncronization  = -1 ///< requires EXT_swap_control_tear
    };

public:
    Canvas(const QSurfaceFormat &format, QScreen *screen = 0);
    virtual ~Canvas();

    // from QWindow
    virtual QSurfaceFormat format() const;

    //QScopedPointer<QOpenGLContext> getContext();

    void setContinuousRepaint(bool enable, int msec = 1000 / 60);
	bool continuousRepaint() const;

    void setPainter(AbstractPainter * painter);
    AbstractPainter * painter();

    void setSwapInterval(SwapInterval swapInterval);
    static const QString swapIntervalToString(SwapInterval swapInterval);

    void animationFrameChanged(const float animationFrame);
    void keyPressed(QKeyEvent *event);

public slots:
    void toggleSwapInterval();
    
protected slots:
    void cameraChanged();

protected:
    QString querys(GLenum penum);
    GLint queryi(GLenum penum);

    virtual void initializeGL(const QSurfaceFormat & format);
	virtual void paintGL();

	virtual void resizeEvent(QResizeEvent * event);

    // navigation event forwarding

    virtual void keyPressEvent        (QKeyEvent   * event);
    virtual void keyReleaseEvent      (QKeyEvent   * event);

    virtual void mouseMoveEvent       (QMouseEvent * event);
    virtual void mousePressEvent      (QMouseEvent * event);
    virtual void mouseReleaseEvent    (QMouseEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);

    virtual void wheelEvent           (QWheelEvent * event);

    /** uses the context to verify existence/support of each mandatory extension
    */
    bool verifyExtensions() const;

	void timerEvent(QTimerEvent * event);

signals:

protected:
    QScopedPointer<QOpenGLContext> m_context;
    OpenGLFunctions *m_functions;

    AbstractPainter * m_painter;
    QScopedPointer<Camera> m_camera;
    QScopedPointer<Navigation> m_navigation;

    SwapInterval m_swapInterval;    ///< required for toggle

    QScopedPointer<QBasicTimer> m_repaintTimer;

    bool m_update; // checked in paintGL, if true, update of program gets triggered

    bool m_continuousRepaint;
};
