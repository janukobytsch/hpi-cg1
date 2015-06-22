#include "exercise17.h"

#include "util/box.h"
#include "util/camera.h"

#include <iostream>

Exercise17::Exercise17(const float animationFrame)
    : AbstractExercise(animationFrame)
    , m_globalDefMode(None)
{
    m_box = new Box(2.0f, 10, 4.0f, 20, 2.0f, 10);
}

Exercise17::~Exercise17()
{
    delete m_box;
}

const QString Exercise17::hints() const
{
    return QString("Use [q],[w],[e],[r],[t] to change global deformation.");
}

bool Exercise17::initialize()
{
    AbstractExercise::initialize();

    m_program.reset(createBasicShaderProgram("data/box.vert", "data/box.frag"));
    m_program->bind();

    m_box->initialize(*m_program);
    m_boxTransform = m_box->getTransform();

    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	return true;
}

void Exercise17::render()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    m_program->setUniformValue("lightsource", QVector3D(0.0, 10.0, 0.0));
    m_program->setUniformValue("viewprojection", camera()->viewProjection());

    m_program->setUniformValue("transform", m_boxTransform);
    m_program->setUniformValue("normalMatrix", (camera()->view() * m_boxTransform).normalMatrix());
    m_program->setUniformValue("color", QVector4D(1.0, 0.0, 0.0, 1.0));
    m_program->setUniformValue("useLighting", true);

    glm::vec3 overallDim = m_box->getOverallObjectDimensions();
    m_program->setUniformValue("overallObjectDimensions", QVector3D(overallDim[0], overallDim[1], overallDim[2]));
    m_program->setUniformValue("animationFrame", m_animationFrame);
    m_program->setUniformValue("globalDeformationMode", m_globalDefMode);

    glLineWidth(1.0f);
    glPolygonOffset(1.0, 1.0);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_box->draw(*this);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glLineWidth(3.0f);    
    m_program->setUniformValue("useLighting", false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_box->draw(*this);
}

bool Exercise17::onKeyPressed(QKeyEvent *keyEvent)
{
    bool changed = false;

    switch(keyEvent->key())
    {
        case Qt::Key_Q:
            m_globalDefMode = None;
            break;
        case Qt::Key_W:
            m_globalDefMode = Pinch;
            break;
        case Qt::Key_E:
            m_globalDefMode = Mold;
            break;
        case Qt::Key_R:
            m_globalDefMode = Twist;
            break;
        case Qt::Key_T:
            m_globalDefMode = Bend;
            break;
        default:
            break;
    }

    return changed || AbstractExercise::onKeyPressed(keyEvent);
}
