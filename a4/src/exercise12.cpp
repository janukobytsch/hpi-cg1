// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 4
//                     - Aufgabe 12
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include <QOpenGLShaderProgram>

#include "util/camera.h"

#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>

#include "exercise12.h"

Exercise12::Exercise12()
{
    m_cubeTransform.scale(0.5f);
}

Exercise12::~Exercise12()
{
}

QMatrix4x4 Exercise12::rotateClockwise(int frame)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Aufgabe 12
    // Apply correct transformations (rotate, translate, scale) with respect to the current frame
    /////////////////////////////////////////////////////////////////////////////////////////////////

    frame = frame % 360;

    QMatrix4x4 transform;

    transform.setToIdentity();
    transform.translate(0,1,0);
    QMatrix4x4 rotation;
    rotation.translate(-0.5,0,0);
    QMatrix4x4 rotationHelper = rotation;
    rotation.rotate(360 - frame, QVector3D(0,0,1));
    rotationHelper.inverted();
    rotation = rotation * rotationHelper;
    transform = transform * rotation;

    return transform;
}

const QString Exercise12::hints() const
{
	return QString("Press [SPACE] to pause or resume animation.");
}

bool Exercise12::initialize()
{
    AbstractExercise::initialize();

    m_program.reset(createBasicShaderProgram("data/cube12.vert", "data/cube12.frag"));
    m_program->bind();

    m_cube.initialize(*m_program);

    glClearColor(0.6, 0.6, 0.6, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	return true;
}

void Exercise12::render()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    m_program->setUniformValue("lightsource", QVector3D(0.0, 10.0, 0.0));
    m_program->setUniformValue("viewprojection", camera()->viewProjection());

    m_program->setUniformValue("transform", m_cubeTransform);
    m_program->setUniformValue("normalMatrix", (camera()->view() * m_cubeTransform).normalMatrix());
    m_program->setUniformValue("color", QVector4D(1.0, 0.0, 0.0, 1.0));

    m_cube.draw(*this);

    m_program->setUniformValue("transform", rotateClockwise(m_frame) * m_cubeTransform);
    m_program->setUniformValue("normalMatrix", (camera()->view() * rotateClockwise(m_frame) * m_cubeTransform).normalMatrix());
    m_program->setUniformValue("color", QVector4D(1.0, 1.0, 0.0, 1.0));

    m_cube.draw(*this);
}
