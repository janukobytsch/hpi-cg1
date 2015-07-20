#pragma once

#include <glm/vec3.hpp>
#include <cinttypes>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "openglfunctions.h"

class Box
{
public:
    Box(const float width, const int numCellsXAxis, const float height, const int numCellsYAxis, const float depth, const int numCellsZAxis);

    void initialize(QOpenGLShaderProgram &program);    

    void draw(OpenGLFunctions &functions);

    QMatrix4x4 getTransform() { return m_transform; }
    glm::vec3 getOverallObjectDimensions() { return m_overallObjectDimensions; }

protected:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertices;
    QOpenGLBuffer m_normals;

    QMatrix4x4              m_transform;
    glm::vec3               m_overallObjectDimensions;
    glm::vec3               m_stepSizes;
    uint64_t                m_vertexCount;

    void draw(GLenum mode, OpenGLFunctions &functions);
};
