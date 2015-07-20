#include "exercise22.h"

#include "util/camera.h"
#include "util/polygonaldrawable.h"
#include "util/objio.h"

Exercise22::Exercise22()
    : AbstractExercise()
    , m_shadingMode(Phong_Shading)
    , m_materialMode(Gold)
{
    m_drawable = ObjIO::fromObjFile("data/suzanne.obj"); // not centered

    setupLight();
    setupMaterials();
}

Exercise22::~Exercise22()
{
    delete m_drawable;
    delete[] m_materials;
}

void Exercise22::setupLight()
{
    m_lighting.m_pos[0] = 0.0f;
    m_lighting.m_pos[1] = 5.0f;
    m_lighting.m_pos[2] = 5.0f;
    m_lighting.m_pos[3] = 1.0f;
    m_lighting.m_iAmbient[0] = 0.2f;
    m_lighting.m_iAmbient[1] = 0.2f;
    m_lighting.m_iAmbient[2] = 0.2f;
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

void Exercise22::setupMaterials()
{
    m_materials = new MaterialDefinition[NumMaterialModes];
    // Gold
    m_materials[Gold].m_kAmbient[0] = 0.24725f;
    m_materials[Gold].m_kAmbient[1] = 0.1995f;
    m_materials[Gold].m_kAmbient[2] = 0.0745f;
    m_materials[Gold].m_kAmbient[3] = 1.0f;
    m_materials[Gold].m_kDiffuse[0] = 0.75164f;
    m_materials[Gold].m_kDiffuse[1] = 0.60648f;
    m_materials[Gold].m_kDiffuse[2] = 0.22648f;
    m_materials[Gold].m_kDiffuse[3] = 1.0f;
    m_materials[Gold].m_kSpecular[0] = 0.628281f;
    m_materials[Gold].m_kSpecular[1] = 0.555802f;
    m_materials[Gold].m_kSpecular[2] = 0.366065f;
    m_materials[Gold].m_kSpecular[3] = 1.0f;
    m_materials[Gold].m_kEmission[0] = 0.1f;
    m_materials[Gold].m_kEmission[1] = 0.1f;
    m_materials[Gold].m_kEmission[2] = 0.1f;
    m_materials[Gold].m_kEmission[3] = 0.0f;
    m_materials[Gold].m_shininess = 0.4f;

    // Red plastic
    m_materials[Red_Plastic].m_kAmbient[0] = 0.0f;
    m_materials[Red_Plastic].m_kAmbient[1] = 0.0f;
    m_materials[Red_Plastic].m_kAmbient[2] = 0.0f;
    m_materials[Red_Plastic].m_kAmbient[3] = 1.0f;
    m_materials[Red_Plastic].m_kDiffuse[0] = 0.5f;
    m_materials[Red_Plastic].m_kDiffuse[1] = 0.0f;
    m_materials[Red_Plastic].m_kDiffuse[2] = 0.0f;
    m_materials[Red_Plastic].m_kDiffuse[3] = 1.0f;
    m_materials[Red_Plastic].m_kSpecular[0] = 0.7f;
    m_materials[Red_Plastic].m_kSpecular[1] = 0.6f;
    m_materials[Red_Plastic].m_kSpecular[2] = 0.6f;
    m_materials[Red_Plastic].m_kSpecular[3] = 1.0f;
    m_materials[Red_Plastic].m_kEmission[0] = 0.1f;
    m_materials[Red_Plastic].m_kEmission[1] = 0.1f;
    m_materials[Red_Plastic].m_kEmission[2] = 0.1f;
    m_materials[Red_Plastic].m_kEmission[3] = 0.0f;
    m_materials[Red_Plastic].m_shininess = 0.25f;

    // Jade
    m_materials[Jade].m_kAmbient[0] = 0.135f;
    m_materials[Jade].m_kAmbient[1] = 0.2225f;
    m_materials[Jade].m_kAmbient[2] = 0.1575f;
    m_materials[Jade].m_kAmbient[3] = 1.0f;
    m_materials[Jade].m_kDiffuse[0] = 0.54f;
    m_materials[Jade].m_kDiffuse[1] = 0.89f;
    m_materials[Jade].m_kDiffuse[2] = 0.63f;
    m_materials[Jade].m_kDiffuse[3] = 1.0f;
    m_materials[Jade].m_kSpecular[0] = 0.316228f;
    m_materials[Jade].m_kSpecular[1] = 0.316228f;
    m_materials[Jade].m_kSpecular[2] = 0.316228f;
    m_materials[Jade].m_kSpecular[3] = 1.0f;
    m_materials[Jade].m_kEmission[0] = 0.1f;
    m_materials[Jade].m_kEmission[1] = 0.1f;
    m_materials[Jade].m_kEmission[2] = 0.1f;
    m_materials[Jade].m_kEmission[3] = 0.0f;
    m_materials[Jade].m_shininess = 0.1f;

    // Chrome
    m_materials[Chrome].m_kAmbient[0] = 0.25f;
    m_materials[Chrome].m_kAmbient[1] = 0.25f;
    m_materials[Chrome].m_kAmbient[2] = 0.25f;
    m_materials[Chrome].m_kAmbient[3] = 1.0f;
    m_materials[Chrome].m_kDiffuse[0] = 0.4f;
    m_materials[Chrome].m_kDiffuse[1] = 0.4f;
    m_materials[Chrome].m_kDiffuse[2] = 0.4f;
    m_materials[Chrome].m_kDiffuse[3] = 1.0f;
    m_materials[Chrome].m_kSpecular[0] = 0.774597f;
    m_materials[Chrome].m_kSpecular[1] = 0.774597f;
    m_materials[Chrome].m_kSpecular[2] = 0.774597f;
    m_materials[Chrome].m_kSpecular[3] = 1.0f;
    m_materials[Chrome].m_kEmission[0] = 0.1f;
    m_materials[Chrome].m_kEmission[1] = 0.1f;
    m_materials[Chrome].m_kEmission[2] = 0.1f;
    m_materials[Chrome].m_kEmission[3] = 0.0f;
    m_materials[Chrome].m_shininess = 0.6f;
}

void Exercise22::setupLightUniforms(QOpenGLShaderProgram *program)
{
    program->setUniformValue("light_pos", QVector4D(m_lighting.m_pos[0], m_lighting.m_pos[1], m_lighting.m_pos[2], m_lighting.m_pos[3]));
    program->setUniformValue("light_iAmbient", QVector4D(m_lighting.m_iAmbient[0], m_lighting.m_iAmbient[1], m_lighting.m_iAmbient[2], m_lighting.m_iAmbient[3]));
    program->setUniformValue("light_iDiffuse", QVector4D(m_lighting.m_iDiffuse[0], m_lighting.m_iDiffuse[1], m_lighting.m_iDiffuse[2], m_lighting.m_iDiffuse[3]));
    program->setUniformValue("light_iSpecular", QVector4D(m_lighting.m_iSpecular[0], m_lighting.m_iSpecular[1], m_lighting.m_iSpecular[2], m_lighting.m_iSpecular[3]));
}

void Exercise22::setupMaterialUniforms(QOpenGLShaderProgram *program)
{
    program->setUniformValue("material_ambient", QVector4D(m_materials[m_materialMode].m_kAmbient[0], m_materials[m_materialMode].m_kAmbient[1], m_materials[m_materialMode].m_kAmbient[2], m_materials[m_materialMode].m_kAmbient[3]));
    program->setUniformValue("material_diffuse", QVector4D(m_materials[m_materialMode].m_kDiffuse[0], m_materials[m_materialMode].m_kDiffuse[1], m_materials[m_materialMode].m_kDiffuse[2], m_materials[m_materialMode].m_kDiffuse[3]));
    program->setUniformValue("material_specular", QVector4D(m_materials[m_materialMode].m_kSpecular[0], m_materials[m_materialMode].m_kSpecular[1], m_materials[m_materialMode].m_kSpecular[2], m_materials[m_materialMode].m_kSpecular[3]));
    program->setUniformValue("material_emission", QVector4D(m_materials[m_materialMode].m_kEmission[0], m_materials[m_materialMode].m_kEmission[1], m_materials[m_materialMode].m_kEmission[2], m_materials[m_materialMode].m_kEmission[3]));
    program->setUniformValue("material_shininess", m_materials[m_materialMode].m_shininess);
}

bool Exercise22::initialize()
{
    initializeOpenGLFunctions();

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_drawable->setMode(GL_TRIANGLES);
    m_drawable->createVAO();
    
    m_phongProgram.reset(createBasicShaderProgram("data/phong.vert", "data/phong.frag"));
    m_phongProgram->bind();
    m_drawable->prepareProgram(m_phongProgram.data());

    m_toonProgram.reset(createBasicShaderProgram("data/toon.vert", "data/toon.frag"));
    m_toonProgram->bind();
    m_drawable->prepareProgram(m_toonProgram.data());

    return true;
}

void Exercise22::render()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    QOpenGLShaderProgram* currentProg;

    if(m_shadingMode == Phong_Shading)
        currentProg = m_phongProgram.data();
    else
        currentProg = m_toonProgram.data();

    currentProg->bind();

    currentProg->setUniformValue("view", camera()->view());
    currentProg->setUniformValue("viewprojection", camera()->viewProjection());
    currentProg->setUniformValue("normalMatrix", camera()->view().normalMatrix());

    setupLightUniforms(currentProg);
    setupMaterialUniforms(currentProg);

    m_drawable->draw(*this);
}

const QString Exercise22::hints() const
{
    return "[1] : toon shading. [2]-[5] : phong shading with different materials (jade, chrome, red plastic, gold)";
}

const QString Exercise22::description() const
{
    return "Phong & Toon Shading";
}

bool Exercise22::onKeyPressed(QKeyEvent * event)
{
    bool changed = false;

    switch(event->key())
    {
        /*case Qt::Key_R:
        loadToonProgram();
        loadPhongProgram();
        break;*/
    case Qt::Key_1:
        m_shadingMode = Toon_Shading;
        break;
    case Qt::Key_2:
        m_shadingMode = Phong_Shading;
        m_materialMode = Jade;
        break;
    case Qt::Key_3:
        m_shadingMode = Phong_Shading;
        m_materialMode = Chrome;
        break;
    case Qt::Key_4:
        m_shadingMode = Phong_Shading;
        m_materialMode = Red_Plastic;
        break;
    case Qt::Key_5:
        m_shadingMode = Phong_Shading;
        m_materialMode = Gold;
        break;
    default:
        break;
    }

    return changed || AbstractExercise::onKeyPressed(event);
}
