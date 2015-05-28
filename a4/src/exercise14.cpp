// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 4
//                     - Aufgabe 14
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

#include "exercise14.h"

#include <math.h>

#include <QMatrix4x4>
#include <QKeyEvent>
#include <QRectF>
#include <QSettings>

#include <glm/trigonometric.hpp>

#include "util/camera.h"
#include "util/polygonaldrawable.h"
#include "util/objio.h"

namespace
{
    const QString settingsGroup("SlerpRotation");
}

Exercise14::Exercise14()
: m_drawable(nullptr)
{
    m_angles0[0] = -50.0f;
    m_angles0[1] = -80.0f;
    m_angles0[2] = -80.0f;

    m_angles1[0] = 0.0f;
    m_angles1[1] = 0.0f;
    m_angles1[2] = 0.0f;
}

Exercise14::~Exercise14()
{
    QSettings s;
    s.beginGroup(settingsGroup);

    s.setValue("angle_x", m_angles0[0]);
    s.setValue("angle_y", m_angles0[1]);
    s.setValue("angle_z", m_angles0[2]);

    s.endGroup();

    delete m_drawable;
}

QMatrix4x4 Exercise14::interpolateEuler(const float t)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 14
    // - Interpolate rotations by interpolating between the euler angles
    // - hint: use the lerp method (to be defined below)
    // - hint: use QMatrix4x4::rotate calls for applying the rotation(s)
    /////////////////////////////////////////////////////////////////////////////////////////////////

    QMatrix4x4 result;

    return result;
}

QMatrix4x4 Exercise14::interpolateQuaternion(const float t)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 14
    // - Implement a spherical interpolation based on quaternions
    // - hint: use the quat method to convert the matrices to quaternions
    // - hint: use the axisAngle method to get the axis and the angle represented by a quaternion
    // - hint: use the slerp method (to be defined below)
    // - hint: use QMatrix4x4::rotate calls for applying the rotation(s)
    /////////////////////////////////////////////////////////////////////////////////////////////////

    QMatrix4x4 result;

    return result;
}

QMatrix4x4 Exercise14::interpolateMatrix(const float t)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 14
    // - Interpolate between the elements of the matrices
    // - hint: use the lerp method (to be defined below)
    /////////////////////////////////////////////////////////////////////////////////////////////////

    QMatrix4x4 result;

    return result;
}

void Exercise14::slerp(
    float result[4],
    const float a[4],
    const float b[4],
    const float & t)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 14
    // - Implement the slerp function.
    // - Keep in mind, that sin(x) might equal zero. Handle that case appropriately.
    /////////////////////////////////////////////////////////////////////////////////////////////////
}

void Exercise14::lerp(
    float & result,
    const float & a,
    const float & b,
    const float & t)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 14
    // - Implement a linear interpolation between a and b as a function of t.
    /////////////////////////////////////////////////////////////////////////////////////////////////
}

bool Exercise14::initialize()
{
    AbstractExercise::initialize();

    if (m_drawable)
        return true;

    QSettings s;
    s.beginGroup(settingsGroup);

    m_angles0[0] = s.value("angle_x", 0.0f).toFloat();
    m_angles0[1] = s.value("angle_y", 0.0f).toFloat();
    m_angles0[2] = s.value("angle_z", 0.0f).toFloat();

    m_angles1[0] = 0.0f;
    m_angles1[1] = 0.0f;
    m_angles1[2] = 0.0f;

    s.endGroup();

    m_program.reset(createBasicShaderProgram("data/model.vert", "data/model.frag"));
    m_program->bind();

    const std::string fileName = "data/suzanne.obj";
    m_drawable = ObjIO::fromObjFile(fileName);

    m_drawable->createVAO(m_program.data());

    m_cubeProgram.reset(createBasicShaderProgram("data/cube14.vert", "data/cube14.frag"));
    m_cubeProgram->bind();

    m_cube.initialize(*m_cubeProgram);

    glClearColor(0.6, 0.6, 0.6, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

void Exercise14::drawEnvironment(const float x0,
    const float y0,
    const float x1,
    const float y1,
    const QMatrix4x4 & globalTransform)
{
    const float dy = (y1 - y0) * 0.5;

    QMatrix4x4 horizontalTranslation;
    horizontalTranslation.translate(x0,  y0, 0.f);
    
    QMatrix4x4 verticalTranslation;
    for(int i = 0; i < 3; ++i)
    {
        QMatrix4x4 rotation;

        rotation.rotate(m_angles0[0], 1.0f, 0.0f, 0.0f);
        rotation.rotate(m_angles0[1], 0.0f, 1.0f, 0.0f);
        rotation.rotate(m_angles0[2], 0.0f, 0.0f, 1.0f);

        drawUnitBase(globalTransform * verticalTranslation * horizontalTranslation * rotation);

        verticalTranslation.translate(0.0f, dy, 0.0f);
    }

    verticalTranslation.setToIdentity();
    horizontalTranslation.setToIdentity();

    horizontalTranslation.translate(x1,  y0, 0.0f);
    
    for(int i = 0; i < 3; ++i)
    {
        QMatrix4x4 rotation;

        rotation.rotate(m_angles1[0], 1.0f, 0.0f, 0.0f);
        rotation.rotate(m_angles1[1], 0.0f, 1.0f, 0.0f);
        rotation.rotate(m_angles1[2], 0.0f, 0.0f, 1.0f);

        drawUnitBase(globalTransform * verticalTranslation * horizontalTranslation * rotation);

        verticalTranslation.translate(0.0f, dy, 0.0f);
    }
}

void Exercise14::drawUnitBase(const QMatrix4x4 & globalTransform)
{
    static const GLfloat s(0.01f);
    static const GLfloat l(0.5f);

    m_cubeProgram->bind();
    m_cubeProgram->setUniformValue("lightsource", QVector3D(0.0, 10.0, 4.0));
    m_cubeProgram->setUniformValue("viewprojection", camera()->viewProjection());

    QMatrix4x4 transform;

    // x
    transform.setToIdentity();
    transform.scale(l, s, s);
    transform.translate(1.0, 0.0, 0.0);
    m_cubeProgram->setUniformValue("transform", globalTransform * transform);
    m_cubeProgram->setUniformValue("normalMatrix", (camera()->view() * globalTransform * transform).normalMatrix());
    m_cubeProgram->setUniformValue("color", QVector4D(1.0, 0.0, 0.0, 1.0));
    m_cube.draw(*this);

    // y
    transform.setToIdentity();
    transform.scale(s, l, s);
    transform.translate(0.0, 1.0, 0.0);
    m_cubeProgram->setUniformValue("transform", globalTransform * transform);
    m_cubeProgram->setUniformValue("normalMatrix", (camera()->view() * globalTransform * transform).normalMatrix());
    m_cubeProgram->setUniformValue("color", QVector4D(0.0, 1.0, 0.0, 1.0));
    m_cube.draw(*this);

    // z
    transform.setToIdentity();
    transform.scale(s, s, l);
    transform.translate(0.0, 0.0, 1.0);
    m_cubeProgram->setUniformValue("transform", globalTransform * transform);
    m_cubeProgram->setUniformValue("normalMatrix", (camera()->view() * globalTransform * transform).normalMatrix());
    m_cubeProgram->setUniformValue("color", QVector4D(0.0, 0.0, 1.0, 1.0));
    m_cube.draw(*this);
}

void Exercise14::render()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    static QMatrix4x4 globalScale;
    if(globalScale.isIdentity())
        globalScale.scale(0.6f);

    m_program->bind();
    m_program->setUniformValue("lightsource", QVector3D(0.0, 10.0, 4.0));
    m_program->setUniformValue("viewprojection", camera()->viewProjection());

    static const float tx[2] = { -4.0f, +4.0f };
    static const float ty[2] = { +2.0f, -2.0f };
    static const float dy    = (ty[1] - ty[0]) * 0.5f;

    drawEnvironment(tx[0], ty[0], tx[1], ty[1], globalScale);

    float t = static_cast<float>(m_frame % 360) / 360.f;
    float x = (tx[1] - tx[0]) * t + tx[0];

    QMatrix4x4 translation;
    for(int i = 0; i < 3; ++i)
    {
        translation.setToIdentity();

        float y = ty[0] + dy * i;
        translation.translate(x, y, 0.0f);

        QMatrix4x4 rotation;
        switch(i)
        {
            case 0: 
                rotation = interpolateMatrix(t);
                break;
            case 1: 
                rotation = interpolateEuler(t);
                break;
            case 2: 
                rotation = interpolateQuaternion(t);
                break;
            default: 
                break;
        }

        QMatrix4x4 transform = globalScale * translation * rotation;

        QMatrix4x4 modelScale;
        modelScale.scale(0.7);

        m_program->bind();
        m_program->setUniformValue("transform", transform * modelScale);
        m_program->setUniformValue("normalMatrix", (camera()->view() * transform * modelScale).normalMatrix());
        m_program->setUniformValue("color", QVector4D(1.0, 1.0, 1.0, 1.0));

        m_drawable->draw(*this);

        glEnable(GL_DEPTH_TEST);
        drawUnitBase(transform);
    }
}

void Exercise14::quat(
    float q[4]
,   const float m[16])
{   
	double tr = m[0 + 4 * 0] + m[1 + 4 * 1] + m[2 + 4 * 2];
	if (tr > 0.0) 
    {
		q[3] = 0.5 * sqrt(tr + 1.0);
		float s = 0.25 / q[3];
		q[0] = (m[2 + 4 * 1] - m[1 + 4 * 2]) * s;
		q[1] = (m[0 + 4 * 2] - m[2 + 4 * 0]) * s;
		q[2] = (m[1 + 4 * 0] - m[0 + 4 * 1]) * s;
	}
	else 
    {
        int i(0), j, k;

        if (m[1 + 4 * 1] > m[0 + 4 * 0])
            i = 1;
		if (m[2 + 4 * 2] > m[i + 4 * i]) 
            i = 2;
		
        static const int nxt[3] = { 1, 2, 0 };

        j = nxt[i];
		k = nxt[j];
		
        q[i] = 0.5 * sqrt(m[i + 4 * i] - m[j + 4 * j] - m[k + 4 * k] + 1.0);

		float s = 0.25f / q[i];

		q[3] = (m[k + 4 * j] - m[j + 4 * k]) * s;
		q[j] = (m[j + 4 * i] + m[i + 4 * j]) * s;
		q[k] = (m[k + 4 * i] + m[i + 4 * k]) * s;
	}
}

void Exercise14::axisAngle(
    float & angle
,   float axis[3]
,   const float q[4])
{
	const double d = q[0] * q[0] + q[1] * q[1] + q[2] * q[2];
	double s = 1.0 / sqrt(d + q[3] * q[3]);

    angle = glm::degrees(2.f * atan2(sqrt(d), static_cast<double>(q[3])));

    axis[0] = q[0] * s;
	axis[1] = q[1] * s;
	axis[2] = q[2] * s;
}

bool Exercise14::onKeyPressed(QKeyEvent* keyEvent)
{
    int i = 0;

    bool changed = false;

    switch(keyEvent->key())
    {
        case Qt::Key_Z:
            ++i;
        case Qt::Key_Y:
            ++i;
        case Qt::Key_X:
            m_angles0[i] += keyEvent->modifiers() & Qt::SHIFT ? 1.f : -1.f;
            update();

            changed = true;

            break;
        case Qt::Key_R:
            m_angles0[0] = 0.0f;
            m_angles0[1] = 0.0f;
            m_angles0[2] = 0.0f;
            update();

            changed = true;

            break;
        default:
            break;
    }

    return changed || AbstractExercise::onKeyPressed(keyEvent);
}

const QString Exercise14::hints() const
{
	return "Press [SPACE] to pause/resume animation. Use X, Y, and Z keys to modify orientation at start.";
}
