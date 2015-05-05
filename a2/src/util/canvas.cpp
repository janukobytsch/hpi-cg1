#include "canvas.h"

#include <cassert>

#include <QDebug>
#include <QApplication>
#include <QBasicTimer>
#include <QResizeEvent>

#include "abstractpainter.h"
#include "fileassociatedshader.h"
#include "camera.h"
#include "navigation.h"
#include "navigationmath.h"

Canvas::Canvas(const QSurfaceFormat &format, QScreen *screen)
: QWindow(screen)
, m_context(new QOpenGLContext)
, m_functions(0)
, m_painter(0)
, m_camera(new Camera())
, m_navigation(new Navigation(*m_camera))
, m_swapInterval(VerticalSyncronization)
, m_repaintTimer(new QBasicTimer())
, m_update(false)
, m_continuousRepaint(false)
{
    setSurfaceType(OpenGLSurface); 

    create();

    m_camera->setFovy(40.0);
    m_navigation->reset();

    initializeGL(format);
}

Canvas::~Canvas()
{
}

QSurfaceFormat Canvas::format() const
{
    if (!m_context)
        return QSurfaceFormat();

    return m_context->format();
}

void Canvas::setContinuousRepaint(bool enable, int msec)
{
    if (m_continuousRepaint)
        m_repaintTimer->stop();

	m_continuousRepaint = enable;

    if (m_continuousRepaint)
        m_repaintTimer->start(msec, this);
}

bool Canvas::continuousRepaint() const
{
	return m_continuousRepaint;
}

QString Canvas::querys(GLenum penum)
{
    const QString result = reinterpret_cast<const char*>(m_functions->glGetString(penum));
    //glError();

    return result;
}

GLint Canvas::queryi(GLenum penum)
{
    GLint result;
    m_functions->glGetIntegerv(penum, &result);

    return result;
}

void Canvas::initializeGL(const QSurfaceFormat & format)
{
    m_context->setFormat(format);
    if (!m_context->create())
	{
		qCritical() << "Errors during creation of OpenGL context.";
		return;
	}

    m_context->makeCurrent(this);

    m_functions = dynamic_cast<OpenGLFunctions*>(m_context->versionFunctions<OpenGLFunctions>());

    assert(m_functions != 0);

    if (!m_functions->initializeOpenGLFunctions())
    {
        qCritical() << "Initializing OpenGL failed.";
        return;
    }

    // print some hardware information

    qDebug();
    qDebug().nospace() << "GPU: " 
        << qPrintable(querys(GL_RENDERER)) << " ("
        << qPrintable(querys(GL_VENDOR)) << ", "
        << qPrintable(querys(GL_VERSION)) << ")";
    qDebug().nospace() << "GL Version: "
        << qPrintable(QString::number(queryi(GL_MAJOR_VERSION))) << "."
        << qPrintable(QString::number(queryi(GL_MINOR_VERSION))) << " "
        << (queryi(GL_CONTEXT_CORE_PROFILE_BIT) ? "Core" : "Compatibility");
    qDebug();

    verifyExtensions(); // false if no painter ...

    connect(m_camera.data(), &Camera::changed, this, &Canvas::cameraChanged);

    m_context->doneCurrent();
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    if (!m_painter)
        return;

    m_camera->setViewport(event->size());

    m_context->makeCurrent(this);

    m_painter->resize(event->size().width(), event->size().height());

    m_context->doneCurrent();

    if (isExposed() && Hidden != visibility())
        paintGL();
}

void Canvas::paintGL()
{
    if (!m_painter || !isExposed() || Hidden == visibility())
        return;

    m_context->makeCurrent(this);    
    QList<QOpenGLShaderProgram*> programsWithInvalidatedUniforms(FileAssociatedShader::process()); // recompile file associated shaders if required

    if (m_update)
    {
        m_painter->update();

        m_update = false;
    }
    else
        m_painter->update(programsWithInvalidatedUniforms);

	m_painter->paint();

    m_context->swapBuffers(this);
    m_context->doneCurrent();
}

void Canvas::cameraChanged()
{
    m_update = true;
}

void Canvas::timerEvent(QTimerEvent * event)
{
    assert(m_repaintTimer);

    if(event->timerId() != m_repaintTimer->timerId())
        return;

    paintGL();
}

//QScopedPointer<QOpenGLContext> Canvas::getContext()
//{
//}

void Canvas::setPainter(AbstractPainter * painter)
{
    if (m_painter == painter)
        return;

    m_painter = painter;
    if (!m_painter)
        return;

    m_context->makeCurrent(this);

    m_painter->initialize();
    m_painter->setCamera(m_camera.data());

    verifyExtensions();

    m_context->doneCurrent();

    m_navigation->setCoordinateProvider(m_painter);
}

bool Canvas::verifyExtensions() const
{
    if (!m_painter)
        return false;

    if (!m_context->isValid())
    {
        qWarning("Extensions cannot be verified due to invalid context.");
        return false;
    }

    QStringList unsupported;

    const QStringList & extensions(m_painter->extensions());
    foreach(const QString & extension, extensions)
    if (!m_context->hasExtension(qPrintable(extension)))
        unsupported << extension;

    if (unsupported.isEmpty())
        return true;

    if (unsupported.size() > 1)
        qWarning("The following mandatory OpenGL extensions are not supported:");
    else
        qWarning("The following mandatory OpenGL extension is not supported:");

    foreach(const QString & extension, unsupported)
        qWarning() << extension;

    qWarning() << "";

    return false;
}

void Canvas::setSwapInterval(SwapInterval swapInterval)
{
    m_context->makeCurrent(this);

    bool result(false);
    m_swapInterval = swapInterval;

#ifdef WIN32

    // ToDo: C++11 - type aliases
    typedef bool(WINAPI * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;

    if (!wglSwapIntervalEXT)
        wglSwapIntervalEXT = reinterpret_cast<SWAPINTERVALEXTPROC>(m_context->getProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        result = wglSwapIntervalEXT(swapInterval);

#elif __APPLE__

    qWarning("ToDo: Setting swap interval is currently not implemented for __APPLE__");

#else
    // ToDo: C++11 - type aliases
    typedef int(APIENTRY * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC glXSwapIntervalSGI = 0;

    if (!glXSwapIntervalSGI)
        glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(m_context->getProcAddress("glXSwapIntervalSGI"));
    if (glXSwapIntervalSGI)
        result = glXSwapIntervalSGI(swapInterval);

#endif

    if (!result)
        qWarning("Setting swap interval to %s failed."
            , qPrintable(swapIntervalToString(swapInterval)));
    else
        qDebug("Setting swap interval to %s."
            , qPrintable(swapIntervalToString(swapInterval)));

    m_context->doneCurrent();
}

void Canvas::toggleSwapInterval()
{
    switch (m_swapInterval)
    {
    case NoVerticalSyncronization:
        setSwapInterval(VerticalSyncronization);
        break;
    case VerticalSyncronization:
        setSwapInterval(AdaptiveVerticalSyncronization);
        break;
    case AdaptiveVerticalSyncronization:
        setSwapInterval(NoVerticalSyncronization);
        break;
    }
}

const QString Canvas::swapIntervalToString(SwapInterval swapInterval)
{
    switch (swapInterval)
    {
    case NoVerticalSyncronization:
        return QString("NoVerticalSyncronization");
    case VerticalSyncronization:
        return QString("VerticalSyncronization");
    case AdaptiveVerticalSyncronization:
        return QString("AdaptiveVerticalSyncronization");
    default:
        return QString();
    }
}

void Canvas::keyPressEvent(QKeyEvent * event)
{
    if (!m_navigation)
        return;

    m_navigation->keyPressEvent(event);
}

void Canvas::keyReleaseEvent(QKeyEvent * event)
{
    if (m_painter && Qt::Key_W == event->key())
    {
        if(m_painter->mode() == AbstractPainter::NORMAL)
            m_painter->setMode(AbstractPainter::WIREFRAME);
        else
            m_painter->setMode(AbstractPainter::NORMAL);
    }

    if (m_painter) {
        if (m_painter->onKeyPressed(event->key())) {
            m_update = true;
        }
    }

    if (m_navigation)
    {
        m_navigation->keyReleaseEvent(event);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->mouseMoveEvent(event);
    m_context->doneCurrent();        
}

void Canvas::mousePressEvent(QMouseEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->mousePressEvent(event);
    m_context->doneCurrent();
}

void Canvas::mouseReleaseEvent(QMouseEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->mouseReleaseEvent(event);
    m_context->doneCurrent();
}

void Canvas::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->mouseDoubleClickEvent(event);
    m_context->doneCurrent();
}

void Canvas::wheelEvent(QWheelEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->wheelEvent(event);
    m_context->doneCurrent();
}
