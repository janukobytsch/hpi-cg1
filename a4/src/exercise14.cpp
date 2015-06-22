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
    m_angles0[0] = 0.0f;
    m_angles0[1] = 0.0f;
    m_angles0[2] = 0.0f;

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
    QMatrix4x4 result;
    float angle1, angle2, angle3;

    lerp(angle1, m_angles0[0], m_angles1[0], t);
    lerp(angle2, m_angles0[1], m_angles1[1], t);
    lerp(angle3, m_angles0[2], m_angles1[2], t);

    result.rotate(angle1, 1, 0, 0);
    result.rotate(angle2, 0, 1, 0);
    result.rotate(angle3, 0, 0, 1);

    return result;
}

QMatrix4x4 Exercise14::interpolateQuaternion(const float t)
{
    QMatrix4x4 mat1 = eulerToMatrix(m_angles0);
    QMatrix4x4 mat2 = eulerToMatrix(m_angles1);

    float start[16];
    float end[16];
    mat1.copyDataTo(start);
    mat2.copyDataTo(end);

    float quat1[4], quat2[4];
    quat(quat1, start);
    quat(quat2, end);

    float slerpedQuat[4];
    slerp(slerpedQuat, quat1, quat2, t);

    float angle;
    float axis[3];
    axisAngle(angle, axis, slerpedQuat);

    QMatrix4x4 result;
    result.setToIdentity();
    result.rotate(angle, axis[0], axis[1], axis[2]);

    return result;
}

QMatrix4x4 Exercise14::interpolateMatrix(const float t)
{
    QMatrix4x4 result;
    QMatrix4x4 start = eulerToMatrix(m_angles0);
    QMatrix4x4 end = eulerToMatrix(m_angles1);

    lerpMatrices(result, start, end, t);

    return result;
}

void Exercise14::lerpMatrices(QMatrix4x4 &result, const QMatrix4x4 &mat1, const QMatrix4x4 &mat2, const float &t)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            lerp(result(i, j), mat1(i, j), mat2(i, j), t);
        }
    }
}

void Exercise14::applySlerp(float & result, const float & theta, const float & a, const float & b, const float & u)
{
    float coef1 = (float) (sin((1 - u) * theta) / sin(theta));
    float coef2 = (float) (sin(u * theta) / sin(theta));
    result = coef1 * a + coef2 * b;
}

void Exercise14::slerp(
    float result[4],
    const float a[4],
    const float b[4],
    const float & t)
{
    float theta = (float) acos(a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3]);

    for (int i = 0; i < 4; ++i) {
        if (theta == 0) {
            lerp(result[i], a[i], b[i], t); // fallback
        } else {
            applySlerp(result[i], theta, a[i], b[i], t);
        }
    }
}

void Exercise14::lerp(
    float & result,
    const float & a,
    const float & b,
    const float & t)
{
    result = (1 - t) * a + t * b;
}

QMatrix4x4 Exercise14::eulerToMatrix(float const angles[3])
{
    float phi = angles[0];
    float psi = angles[1];
    float th = angles[2];

    float values[16] = {
            // row major order
            (float) (cos(th)*cos(psi)), (float) (cos(th)*sin(psi)), (float) -sin(th), 0,
            (float) (sin(phi)*sin(th)*cos(psi)-cos(phi)*sin(psi)),
            (float) (sin(phi)*sin(th)*sin(psi)+cos(phi)*cos(psi)), (float) (sin(phi)*cos(th)), 0,
            (float) (cos(phi)*sin(th)*cos(psi)+sin(phi)*sin(psi)),
            (float) (cos(phi)*sin(th)*sin(psi)-sin(phi)*cos(psi)), (float) (cos(phi)*cos(th)), 0,
            0, 0, 0, 1
    };

    return QMatrix4x4(values);
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

    m_angles1[0] = -90.0f;
    m_angles1[1] = 90.0f;
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
