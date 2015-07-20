#pragma once

#include "util/abstractexercise.h"
#include "util/lightingdefinition.h"
#include "util/materialdefinition.h"

class PolygonalDrawable;

enum ShadingMode
{
    Toon_Shading = 0,
    Phong_Shading,
    NumShadingModes
};

enum MaterialMode
{
    Gold = 0,
    Red_Plastic,
    Jade,
    Chrome,
    NumMaterialModes
};

class Exercise22 : public AbstractExercise, protected OpenGLFunctions
{
public:
    Exercise22();
    virtual ~Exercise22();

    // from abstractglexercise.h
    virtual void render() override;

    // from abstractexercise.h
    virtual bool initialize() override;

    virtual const QString hints() const;
    virtual const QString description() const;

    virtual bool onKeyPressed(QKeyEvent * event);

protected:
    void setupLight();
    void setupMaterials();
    void setupLightUniforms(QOpenGLShaderProgram *program);
    void setupMaterialUniforms(QOpenGLShaderProgram *program);

    QScopedPointer<QOpenGLShaderProgram> m_phongProgram;
    QScopedPointer<QOpenGLShaderProgram> m_toonProgram;
    PolygonalDrawable *m_drawable;

    MaterialDefinition *m_materials;
    LightingDefinition m_lighting;

    ShadingMode m_shadingMode;
    MaterialMode m_materialMode;
};
