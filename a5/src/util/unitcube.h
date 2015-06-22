#pragma once

#include <unordered_map>
#include <array>
#include <vector>

#include <glm/vec3.hpp>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "openglfunctions.h"

class UnitCube
{
public:
    using Face = std::array<GLuint, 3>;

    static std::array<glm::vec3, 8> vertices();
    static std::array<Face, 12> indices(); /// individual triangle indices (no strip, no fan)

public:
    UnitCube();

    void initialize(QOpenGLShaderProgram &program);


    /** draws the unit cube as single triangles (TODO: generate set of triangle strips?)
    */
    void draw(OpenGLFunctions & functions);
    void draw(GLenum mode, OpenGLFunctions & functions);

private:
    QOpenGLVertexArrayObject m_vao;

    QOpenGLBuffer m_vertices;
    QOpenGLBuffer m_normals;
public:
    virtual ~UnitCube();
};
