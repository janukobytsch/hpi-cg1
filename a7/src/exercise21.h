#pragma once

#include "util/abstractexercise.h"
#include "util/lightingdefinition.h"
#include "util/materialdefinition.h"

#define SIZE 8

class Exercise21 : public AbstractExercise, protected OpenGLFunctions21
{
public:
    Exercise21();
    virtual ~Exercise21();

    // from abstractglexercise.h
    virtual void render() override;

    // from abstractexercise.h
    virtual bool initialize() override;

    virtual const QString hints() const;
    virtual const QString description() const;

    virtual bool onKeyPressed(QKeyEvent * event);

protected:
    void initBezierLightingAndMaterial();
    void calculateHeightField();

    void drawHeightFieldPoints();
    void drawHeightFieldLines();
    void drawTriangulatedHeightField();
    void drawHeightFieldBezierPatch();
    void drawGroundPlate();

    bool m_togglePoints;
    bool m_toggleLines;
    bool m_toggleTriangles;
    bool m_toggleBezier;

    //int m_gridSize;
    MaterialDefinition  m_bezierMaterial;
    LightingDefinition  m_lighting;
    GLfloat m_heightField[SIZE][SIZE][3];
};
