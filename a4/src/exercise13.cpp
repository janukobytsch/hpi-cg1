// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 4
//                     - Aufgabe 13
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

#include <QDebug>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "util/camera.h"

#include <math.h>

#include "exercise13.h"

namespace 
{
    const QString TEXTURE_FILENAME = "data/luxo.png";
}

Exercise13::Exercise13()
: m_textureID(-1)
, m_sphere(3)
{
    m_sphereScale.scale(0.3);
}

Exercise13::~Exercise13()
{
	if (m_textureID != -1)
        glDeleteTextures(1, &m_textureID);
}

float calculateVelocity(const float &t) {
    const float gravitation = 9.8f;
    return (float) (0.5 * gravitation * t);
}

QMatrix4x4 Exercise13::applyBallTransformation(const int frame)
{
    // environment
    static const std::vector<float> left = {-0.9f, 0.8f, 0.0f};
    static const std::vector<float> right = {0.9f, 0.4f, 0.0f};
    static const std::vector<float> bottom = {0.0f, -1.05f, 0.0f};

    // ball
    static const float r = 0.3f;
    static const float d = 0.3f * r;
    static const std::vector<float> startPos = {-2.0f, left[1], 0.0f};
    static const std::vector<float> endPos = {2.0f, right[1], 0.0f};
    static const float ballSpeedX = 0.02f;
    static const float angularSpeed = (float const) (-ballSpeedX/(r*2*M_PI)*360);
    static bool isBounced = false;
    static float x, y, z;
    float scaleY = 1, scaleX = 1, scaleZ = 1;

    // animation
    static const int numFramesPerAnimation = static_cast<int>(4.0f / ballSpeedX);
    const int periodicity = frame % numFramesPerAnimation;

    // restart animation
    if (x >= endPos[0]) {
        x = startPos[0];
        y = startPos[1];
        z = startPos[2];
        isBounced = false;
    }

    if (x <= left[0]) {
        // clamp to cliff height
        y = left[1] + r;
    } else if (x > left[0] && x < right[0]) {
        float tolerance = r/3;
        if (y - r + tolerance <= bottom[1]) {
            isBounced = true;
        }
        // scale ball
        if (y - r <= bottom[1]) {
            float delta = (bottom[1] - y - r) / tolerance;
            float lerpScale = (1 - delta) * d + delta * d;
            scaleX += lerpScale;
            scaleY = lerpScale;
            scaleZ += lerpScale;
        }
        // simulate gravitation and bounce
        float delta = (x - left[0]) / (right[0] - left[0]);
        if (!isBounced) {
            int slope = 1500;
            float velocityY = calculateVelocity(delta / (slope * ballSpeedX));
            y -= velocityY;
        } else {
            int slope = 1200;
            float velocityY = calculateVelocity((1-delta) / (slope * ballSpeedX));
            y += velocityY;
        }
    } else if (x >= right[0]) {
        // finish bounce and clamp to cliff height
        int slope = 800;
        float delta = (x - right[0]) / (endPos[0] - right[0]);
        float velocityY = calculateVelocity(delta / (slope * ballSpeedX));
        y -= velocityY;
        y = (y <= right[1] + r) ? right[1] + r : y;
    }

    // roll the ball
    x += ballSpeedX;

    // apply transformations
    QMatrix4x4 transform;
    transform.setToIdentity();
    transform.translate(x, y, z);
    transform.scale(scaleX, scaleY, scaleZ);
    transform.rotate(angularSpeed * periodicity, 0, 0, 1); // around z-axis

    return transform;
}

void Exercise13::drawEnvironment()
{
    m_program0->bind();
    m_program0->setUniformValue("lightsource", QVector3D(0.0, 10.0, 0.0));
    m_program0->setUniformValue("viewprojection", camera()->viewProjection());

    // Draw big cubes

    m_program0->setUniformValue("color", QVector4D(0.8, 0.8, 0.8, 1.0));

    QMatrix4x4 transform;
    transform.translate(-4.9f, -1.201f, 0.0f);
    transform.scale(4.f, 2.0f,  4.0f);

    m_program0->setUniformValue("transform", transform);
    m_program0->setUniformValue("normalMatrix", (camera()->view() * transform).normalMatrix());

    m_cube.draw(*this);


    transform.setToIdentity();
    transform.translate(4.9f, -1.401f, 0.0f);
    transform.scale(4.f, 1.8f, 4.0f);

    m_program0->setUniformValue("transform", transform);
    m_program0->setUniformValue("normalMatrix", (camera()->view() * transform).normalMatrix());

    m_cube.draw(*this);


    transform.setToIdentity();
    transform.translate(0.0f, -3.0501f, 0.0f);
    transform.scale(1.0f, 2.0f,  4.0f);

    m_program0->setUniformValue("transform", transform);
    m_program0->setUniformValue("normalMatrix", (camera()->view() * transform).normalMatrix());

    m_cube.draw(*this);


    // Draw trail

    m_program0->setUniformValue("color", QVector4D(0.9, 0.9, 0.9, 1.0));

    transform.setToIdentity();
    transform.translate(-4.9f, 0.8f, 0.0f);
    transform.scale(4.f, 0.0f, 0.2f);

    m_program0->setUniformValue("transform", transform);
    m_program0->setUniformValue("normalMatrix", (camera()->view() * transform).normalMatrix());

    m_cube.draw(*this);


    transform.setToIdentity();
    transform.translate(4.9f, 0.4f, 0.0f);
    transform.scale(4.f, 0.0f, 0.2f);

    m_program0->setUniformValue("transform", transform);
    m_program0->setUniformValue("normalMatrix", (camera()->view() * transform).normalMatrix());

    m_cube.draw(*this);

    transform.setToIdentity();
    transform.translate(0.0f, -1.05f, 0.0f);
    transform.scale(1.0f, 0.0f,  0.2f);

    m_program0->setUniformValue("transform", transform);
    m_program0->setUniformValue("normalMatrix", (camera()->view() * transform).normalMatrix());

    m_cube.draw(*this);
}

void Exercise13::render()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    drawEnvironment();

	glBindTexture(GL_TEXTURE_2D, m_textureID);
    glActiveTexture(GL_TEXTURE0);

    QMatrix4x4 transform = applyBallTransformation(m_frame) * m_sphereScale;

    m_program1->bind();
    m_program1->setUniformValue("lightsource", QVector3D(0.0, 10.0, 0.0));
    m_program1->setUniformValue("viewprojection", camera()->viewProjection());

    m_program1->setUniformValue("transform", transform);
    m_program1->setUniformValue("normalMatrix", (camera()->view() * transform).normalMatrix());
    m_program1->setUniformValue("diffuse", 0);

    m_sphere.draw(*this);
}

bool Exercise13::initialize()
{
    AbstractExercise::initialize();

    m_textureID = getOrCreateTexture(TEXTURE_FILENAME);


    m_program0.reset(createBasicShaderProgram("data/cube13.vert", "data/cube13.frag"));

    m_cube.initialize(*m_program0);

    m_program1.reset(createBasicShaderProgram("data/sphere.vert", "data/sphere.frag"));

    m_sphere.initialize(*m_program1);

    glClearColor(0.6, 0.6, 0.6, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}

const QString Exercise13::hints() const
{
    return QString("Press [SPACE] to pause or resume animation.");
}
