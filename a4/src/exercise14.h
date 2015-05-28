#pragma once

#include "util/abstractexercise.h"
#include "util/unitcube.h"

#include <QScopedPointer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

class PolygonalDrawable;

class Exercise14 : public AbstractExercise
{
public:
    Exercise14();
    virtual ~Exercise14();

    virtual void render() override;

    virtual const QString hints() const override;

protected:
    virtual bool onKeyPressed(QKeyEvent* keyEvent) override;

    virtual bool initialize() override;

    QMatrix4x4 interpolateEuler     (const float t);
    QMatrix4x4 interpolateQuaternion(const float t);
    QMatrix4x4 interpolateMatrix    (const float t);

    static QMatrix4x4 eulerToMatrix(const float angles[3]);
    static void lerpMatrices(QMatrix4x4 & result, const QMatrix4x4 & mat1, const QMatrix4x4 & mat2, const float & t);

    static void slerp(float c[4] /*out*/, const float a[4], const float b[4], const float& t);
	static void lerp(
        float & result,
        const float & a,
        const float & b,
        const float & t);

    static void quat(
        float q[4],
        const float m[16]);

    void axisAngle(
        float & angle,
        float axis[3],
        const float q[4]);

protected:
    void drawEnvironment(
        const float x0,
        const float y0,
        const float x1,
        const float y1,
        const QMatrix4x4 & globalTransform);

    void drawUnitBase(const QMatrix4x4 & globalTransform);

protected:
    float m_angles0[3];
    float m_angles1[3];

    PolygonalDrawable * m_drawable;
    UnitCube m_cube;

    QScopedPointer<QOpenGLShaderProgram> m_program;
    QScopedPointer<QOpenGLShaderProgram> m_cubeProgram;
};
