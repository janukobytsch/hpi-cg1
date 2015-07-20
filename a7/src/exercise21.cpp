// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 7
//                     - Aufgabe 21
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

#include "exercise21.h"

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "util/camera.h"

Exercise21::Exercise21()
    : AbstractExercise()
    ,m_toggleLines(false)
    ,m_togglePoints(true)
    ,m_toggleTriangles(false)
    ,m_toggleBezier(false)
{
    initBezierLightingAndMaterial();
    calculateHeightField();
}

Exercise21::~Exercise21()
{
}

void Exercise21::initBezierLightingAndMaterial()
{
    m_bezierMaterial.m_kAmbient[0] = 0.4f;
    m_bezierMaterial.m_kAmbient[1] = 0.0f;
    m_bezierMaterial.m_kAmbient[2] = 0.0f;
    m_bezierMaterial.m_kAmbient[3] = 1.0f;
    m_bezierMaterial.m_kDiffuse[0] = 0.7f;
    m_bezierMaterial.m_kDiffuse[1] = 0.0f;
    m_bezierMaterial.m_kDiffuse[2] = 0.0f;
    m_bezierMaterial.m_kDiffuse[3] = 1.0f;
    m_bezierMaterial.m_kSpecular[0] = 0.7f;
    m_bezierMaterial.m_kSpecular[1] = 0.6f;
    m_bezierMaterial.m_kSpecular[2] = 0.6f;
    m_bezierMaterial.m_kSpecular[3] = 1.0f;
    m_bezierMaterial.m_shininess = 0.25f;

    m_lighting.m_pos[0] = 0.0f;
    m_lighting.m_pos[1] = 0.0f;
    m_lighting.m_pos[2] = 0.2f;
    m_lighting.m_pos[3] = 1.0f;
    m_lighting.m_iAmbient[0] = 0.8f;
    m_lighting.m_iAmbient[1] = 0.8f;
    m_lighting.m_iAmbient[2] = 0.8f;
    m_lighting.m_iAmbient[3] = 1.0f;
    m_lighting.m_iDiffuse[0] = 1.0f;
    m_lighting.m_iDiffuse[1] = 1.0f;
    m_lighting.m_iDiffuse[2] = 1.0f;
    m_lighting.m_iDiffuse[3] = 1.0f;
    m_lighting.m_iSpecular[0] = 0.5f;
    m_lighting.m_iSpecular[1] = 0.5f;
    m_lighting.m_iSpecular[2] = 0.5f;
    m_lighting.m_iSpecular[3] = 1.0f;
}

void Exercise21::calculateHeightField()
{
    int sqrtGridSize = SIZE;
    for(int i = 0; i < sqrtGridSize; i++)
    {
        float y = 2.0f - i*(4.0f / (sqrtGridSize - 1));
        for(int j = 0; j < sqrtGridSize; j++)
        {
            float x = -2.0f + j*(4.0f / (sqrtGridSize - 1));

            m_heightField[i][j][0] = x;
            float height = 0.01f + qrand() % 5 / 5.0f;
            m_heightField[i][j][1] = height;
            m_heightField[i][j][2] = -1 * y;
        }
    }
}

bool Exercise21::initialize()
{
    initializeOpenGLFunctions();

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 21
    // Initialize a two-dimensional opengl evaluator.
    // Make sure that if SIZE changes, the evaluator still works
    /////////////////////////////////////////////////////////////////////////////////////////////////

    glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3, SIZE, 0.0f, 1.0f, SIZE * 3, SIZE, &m_heightField[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(SIZE, 0.0f,1.0f,SIZE,0.0f,1.0f);

    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    return true;
}

void Exercise21::render()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glLoadMatrixf(m_camera->viewProjection().data());

    glPushMatrix();

    drawGroundPlate();

    if(m_toggleBezier)
        drawHeightFieldBezierPatch();
    if(m_toggleTriangles)
        drawTriangulatedHeightField();
    if(m_toggleLines)
        drawHeightFieldLines();
    if(m_togglePoints)
        drawHeightFieldPoints();

    glPopMatrix();
}

void Exercise21::drawGroundPlate()
{
    glPushMatrix();
    glBegin(GL_LINE_LOOP);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-2.5f, 0.0f, -2.5f);
    glVertex3f(2.5f, 0.0f, -2.5f);
    glVertex3f(2.5f, 0.0f, 2.5f);
    glVertex3f(-2.5f, 0.0f, 2.5f);

    glEnd();
    glPopMatrix();
}

void Exercise21::drawHeightFieldPoints()
{
    glPushMatrix();

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 21
    // Choose appropriate point size.
    /////////////////////////////////////////////////////////////////////////////////////////////////

    glPointSize(3.0f);

    glBegin(GL_POINTS);

    glColor3f(1.f, 1.f, 1.f);

    for(int i = 0; i < SIZE; i++)
    {
        for(int j = 0; j < SIZE; j++)
        {
            /////////////////////////////////////////////////////////////////////////////////////////////////
            // TODO: Aufgabe 21
            // Visualize the height field using GL_POINTS.
            /////////////////////////////////////////////////////////////////////////////////////////////////
            glVertex3f(m_heightField[i][j][0], m_heightField[i][j][1], m_heightField[i][j][2]);

        }
    }
    glEnd();
    glPopMatrix();
}

void Exercise21::drawHeightFieldLines()
{
    glPushMatrix();
    glBegin(GL_LINES);

    glColor3f(1.f, 1.f, 0.f);

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 21
    // Visualize the height field using GL_LINES.
    // Make sure that if SIZE changes, the rendering results are still correct.
    /////////////////////////////////////////////////////////////////////////////////////////////////

    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            glVertex3f(m_heightField[i][j][0], 0.0f, m_heightField[i][j][2]);
            glVertex3f(m_heightField[i][j][0], m_heightField[i][j][1], m_heightField[i][j][2]);
        }}
    glEnd();
    glPopMatrix();
}

void Exercise21::drawTriangulatedHeightField()
{
    glDisable(GL_CULL_FACE);
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glColor3f(1.f, 1.f, 1.f);

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 21
    // Triangulate the given height field. Use GL_TRIANGLE_STRIPS.
    // Make sure that if SIZE changes, the triangulation still works.
    /////////////////////////////////////////////////////////////////////////////////////////////////

    for(int i = 0; i < (SIZE - 1); i++){
        glBegin(GL_TRIANGLE_STRIP);
        for(int j = 0; j < (SIZE); j++){
            glVertex3f(m_heightField[i][j][0], m_heightField[i][j][1], m_heightField[i][j][2]);
            glVertex3f(m_heightField[i + 1][j][0], m_heightField[i + 1][j][1], m_heightField[i + 1][j][2]);
            //glVertex3f(m_heightField[i][j + 1][0], m_heightField[i][j + 1][1], m_heightField[i][j + 1][2]);
        }
        glEnd();

    }
    glPopMatrix();
    glEnable(GL_CULL_FACE);
}

void Exercise21::drawHeightFieldBezierPatch()
{
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_lighting.m_iAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lighting.m_iDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_lighting.m_iSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, m_lighting.m_pos);

    glMaterialfv(GL_FRONT, GL_AMBIENT, m_bezierMaterial.m_kAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_bezierMaterial.m_kDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_bezierMaterial.m_kSpecular);
    GLfloat sh[] = { m_bezierMaterial.m_shininess };
    glMaterialfv(GL_FRONT, GL_SHININESS, sh);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(1.f, 1.f, 1.f);

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 21
    // Draw a *filled* bezier patch using the opengl evaluator
    /////////////////////////////////////////////////////////////////////////////////////////////////
    glEvalMesh2(GL_FILL,0,SIZE,0,SIZE);

    glPopMatrix();
    glDisable(GL_LIGHTING);
}

const QString Exercise21::hints() const
{
    return "[P] : toggle points. [L] : toggle lines. [T] : toggle triangulation. [B] : toggle bezier patch.";
}

const QString Exercise21::description() const
{
    return "Triangulation and Bezier Surface";
}

bool Exercise21::onKeyPressed(QKeyEvent * event)
{
    bool changed = false;

    switch(event->key())
    {
    case Qt::Key_P:
        m_togglePoints = !m_togglePoints;
        break;
    case Qt::Key_L:
        m_toggleLines = !m_toggleLines;
        break;
    case Qt::Key_T:
        m_toggleTriangles = !m_toggleTriangles;
        break;
    case Qt::Key_B:
        m_toggleBezier = !m_toggleBezier;
        break;
    default:
        break;
    }

    return changed || AbstractExercise::onKeyPressed(event);
}
