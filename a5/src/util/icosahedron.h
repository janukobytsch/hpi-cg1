#pragma once

#include <unordered_map>
#include <array>
#include <vector>

#include <glm/vec3.hpp>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "openglfunctions.h"

class Icosahedron
{
public:
    using Face = std::array<GLuint, 3>;

    static std::array<glm::vec3, 12> vertices();
    static std::array<Face, 20> indices(); /// individual triangle indices (no strip, no fan)

   /**  Iterative triangle refinement: split each triangle into 4 new ones and 
        create points and indices appropriately.
    */
    static void refine(
        std::vector<glm::vec3> & vertices
    ,   std::vector<Face> & indices
    ,   unsigned char levels);

public:
    Icosahedron(
        GLsizei iterations = 0
    ,   const GLint positionLocation = 0
    ,   const GLint normalLocation = 1);

    void initialize(QOpenGLShaderProgram &program);


    /** draws the icosahedron as single triangles (TODO: generate set of triangle strips?)
    */
    void draw(OpenGLFunctions & functions);
    void draw(GLenum mode, OpenGLFunctions & functions);

private:
    /** Splits a triangle edge by adding an appropriate new point (normalized
        on sphere) to the points (if not already cached) and returns the index
        to this point.
    */
    static GLushort split(
        GLushort a
    ,   GLushort b
    ,   std::vector<glm::vec3> & points
    ,   std::unordered_map<glm::uint, GLushort> & cache);

private:
    QOpenGLVertexArrayObject m_vao;

    QOpenGLBuffer m_vertices;
    QOpenGLBuffer m_indices;

    int m_iterations;
    GLsizei m_size;
    GLint m_positionLocation;
    GLint m_normalLocation;

public:
    virtual ~Icosahedron();
};
