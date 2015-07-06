// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 6
//                     - Aufgabe 19
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

#include "exercise19.h"

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <glm/common.hpp>

#include "util/camera.h"
#include "util/polygonaldrawable.h"


#include <iostream>
#include <QtGlobal>
#include <QMouseEvent>
#include <QWindow>
#include <QMatrix4x4>


namespace
{
    // global state FTW

    std::vector<Vertex> g_combinedVertices; // new vertices from tesselation (see combineCallback)
    std::vector<Vertex> g_vertices; // vertices for the current primitive

    GLenum g_primitive; // Current primitive type
}

void draw_shape(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.1f, 0.75f, 0.1f);
    glBegin(g_primitive);
    for (int i = 0; i < g_vertices.size(); ++i)
    {
        glVertex3d(g_vertices[i].x, g_vertices[i].y, g_vertices[i].z);
    }
    glEnd();
}

void draw_lines(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(g_primitive);
    for (int i = 0; i < g_vertices.size(); ++i)
    {
        glVertex3d(g_vertices[i].x, g_vertices[i].y, g_vertices[i].z);
    }
    glEnd();
}

void errorCallback(GLenum errorCode)
{
    qWarning() << "Tessellation Error: " << gluErrorString(errorCode);
}

void beginCallback(GLenum prim)
{
    g_vertices.clear();
    g_primitive = prim;
    //qDebug() << "beginCallback";
}

void vertexCallback(void * vdata)
{
    const GLdouble *ptr;
    ptr = (GLdouble *) vdata;
    Vertex vertex(ptr[0], ptr[1], ptr[2]);
    g_vertices.push_back(vertex);
    //qDebug() << "vertexCallback";
}

void combineCallback(double coords[3], double vertex_data[4], float weight[4], double **dataOut)
{
    Vertex vertex(coords[0], coords[1], coords[2]);
    g_combinedVertices.push_back(vertex);
    Vertex &v = g_combinedVertices.back();
    *dataOut = &v.x;
    //qDebug() << "combineCallback";
}

void endCallback(void)
{
    draw_shape();
    glClear(GL_DEPTH_BUFFER_BIT); // allow overdraw
    draw_lines();
    //qDebug() << "endCallback";
}

Exercise19::Exercise19()
:   AbstractExercise()
{
}

Exercise19::~Exercise19()
{
    gluDeleteTess(m_tesselator);
}

void Exercise19::render()
{
    glViewport(0, 0, m_viewport[0], m_viewport[1]);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glPushMatrix();

    tessellatePolygons();
    drawContours();

    glPopMatrix();
}

bool Exercise19::initialize()
{
    initializeOpenGLFunctions();

    m_contours.push_back(Contour());

    glClearColor(0.5, 0.5, 0.5, 1.0);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    glPointSize(2.f);

    m_tesselator = gluNewTess();

    return true;
}

const QString Exercise19::hints() const
{
    return "Left click to add point to current contour. Right click to close current contour.";
}

const QString Exercise19::description() const
{
    return "Tessellation";
}

void Exercise19::drawContours()
{
    static const GLenum mode[] = { GL_POINTS, GL_LINES, GL_POLYGON };

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (unsigned int i = 0; i < m_contours.size(); ++i)
    {
        const Contour contour = m_contours[i];

        const QColor color((Qt::GlobalColor)(i % 13 + 6));
        glColor3f(color.redF(), color.greenF(), color.blueF());

        if (contour.size() > 0)
        {
            int modeIndex = glm::min(static_cast<unsigned int>(contour.size()-1), 2u);
            glBegin(mode[modeIndex]);

            for (Contour::const_iterator it = contour.begin(); it != contour.end(); ++it)
            {
                //qDebug() << it->x << ", " << it->y;
                glVertex3f(it->x, it->y, it->z);
            }
            glEnd();
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Exercise19::tessellatePolygons()
{
    gluTessCallback(m_tesselator, GLU_TESS_BEGIN, (GLvoid (*) ()) &beginCallback);
    gluTessCallback(m_tesselator, GLU_TESS_END, (GLvoid (*) ()) &endCallback);
    gluTessCallback(m_tesselator, GLU_TESS_VERTEX, (GLvoid (*) ()) &vertexCallback);
    gluTessCallback(m_tesselator, GLU_TESS_COMBINE, (GLvoid(*)()) &combineCallback);
    gluTessCallback(m_tesselator, GLU_TESS_ERROR, (GLvoid (*) ()) &errorCallback);

    //gluTessProperty(m_tesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

    gluTessBeginPolygon(m_tesselator, NULL);
    for (ContourList::iterator clit = m_contours.begin(); clit != m_contours.end(); ++clit)
    {
        Contour &currentContour = *clit;
        gluTessBeginContour(m_tesselator);
        for (Contour::iterator cit = currentContour.begin(); cit != currentContour.end(); ++cit)
        {
            double *vertex = new double[3];
            vertex[0] = cit->x;
            vertex[1] = cit->y;
            vertex[2] = cit->z;
            gluTessVertex(m_tesselator, vertex, vertex);
        }
        gluTessEndContour(m_tesselator);
    }
    gluTessEndPolygon(m_tesselator);
}

bool Exercise19::onMouseReleased(QMouseEvent * mouseEvent)
{
    bool changed = false;

    if (mouseEvent->button() == Qt::LeftButton)
    {
        // Add vertex to contour
        float ratio = 1.0;// TODO use pixel device ratio;
        int x = mouseEvent->pos().x()*ratio;
        int y = mouseEvent->pos().y()*ratio;

        Vertex v(static_cast<float>(x), static_cast<float>(y), 0.f);
        m_contours[m_contours.size() - 1].push_back(v);

        changed = true;
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        // Finish current contour and go to the next one
        Contour & current = m_contours[m_contours.size() - 1];

        if (current.size() >= 3)
        {
            // The last contour has 3 or more vertices, so it's valid and we create a new one
            Contour contour;
            m_contours.push_back(contour);
        }
        else
        {
            // The last contour did not have 3 or more vertices, so we clear it and start again
            current.clear();
        }

        changed = true;
    }

    return changed || AbstractExercise::onMouseReleased(mouseEvent);
}

bool Exercise19::onKeyPressed(QKeyEvent * event)
{
    bool changed = false;

    switch(event->key())
    {
    case Qt::Key_R:

        m_contours.clear();

        m_contours.push_back(Contour());

        changed = true;

        break;
    }

    return changed || AbstractExercise::onKeyPressed(event);
}

void Exercise19::resize(int width, int height)
{
    AbstractExercise::resize(width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
