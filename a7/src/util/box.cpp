#include "box.h"

Box::Box(const float width, const int numCellsXAxis, const float height, const int numCellsYAxis, const float depth, const int numCellsZAxis)
    :  m_vertexCount(0)
    , m_overallObjectDimensions(width, height, depth)
{
    m_stepSizes = glm::vec3(
        m_overallObjectDimensions[0] / static_cast<float>(numCellsXAxis),
        m_overallObjectDimensions[1] / static_cast<float>(numCellsYAxis),
        m_overallObjectDimensions[2] / static_cast<float>(numCellsZAxis));
}

void Box::initialize(QOpenGLShaderProgram &program)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    float widthT = m_overallObjectDimensions[0] * 0.5f;
    float heightT = m_overallObjectDimensions[1] * 0.5f;
    float depthT = m_overallObjectDimensions[2] * 0.5f;

    float longestAxis = std::max(heightT, std::max(widthT, depthT));

    m_transform.scale(1.0f / longestAxis);
    //m_transform.setToIdentity();

    for(float y = -heightT; y <= heightT - (m_stepSizes[1] * 0.5f); y += m_stepSizes[1])
    {
        for(float x = -widthT; x <= widthT - (m_stepSizes[0] * 0.5f); x += m_stepSizes[0])
        {
            // front
            normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
            vertices.push_back(glm::vec3(x, y, depthT));

            normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], y, depthT));

            normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
            vertices.push_back(glm::vec3(x, y + m_stepSizes[1], depthT));

            normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], y, depthT));

            normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], y + m_stepSizes[1], depthT));
            
            normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
            vertices.push_back(glm::vec3(x, y + m_stepSizes[1], depthT));

            // back
            normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
            vertices.push_back(glm::vec3(x, y + m_stepSizes[1], -depthT));

            normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], y + m_stepSizes[1], -depthT));

            normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], y, -depthT));

            normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
            vertices.push_back(glm::vec3(x, y + m_stepSizes[1], -depthT));

            normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], y, -depthT));

            normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
            vertices.push_back(glm::vec3(x, y, -depthT));
        }
    }

    for(float x = -widthT; x <= widthT - (m_stepSizes[0] * 0.5f); x += m_stepSizes[0])
    {
        for(float z = -depthT; z <= depthT - (m_stepSizes[2] * 0.5f); z += m_stepSizes[2])
        {
            // top
            normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
            vertices.push_back(glm::vec3(x, heightT, z + m_stepSizes[2]));

            normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], heightT, z + m_stepSizes[2]));

            normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
            vertices.push_back(glm::vec3(x, heightT, z));

            normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], heightT, z + m_stepSizes[2]));

            normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], heightT, z));

            normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
            vertices.push_back(glm::vec3(x, heightT, z));

            // bottom
            normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
            vertices.push_back(glm::vec3(x, -heightT, z));

            normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], -heightT, z));

            normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], -heightT, z + m_stepSizes[2]));

            normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
            vertices.push_back(glm::vec3(x, -heightT, z));

            normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
            vertices.push_back(glm::vec3(x + m_stepSizes[0], -heightT, z + m_stepSizes[2]));

            normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
            vertices.push_back(glm::vec3(x, -heightT, z + m_stepSizes[2]));
        }
    }

    for(float z = -depthT; z <= depthT - (m_stepSizes[2] * 0.5f); z += m_stepSizes[2])
    {
        for(float y = -heightT; y <= heightT - (m_stepSizes[1] * 0.5f); y += m_stepSizes[1])
        {
            // left
            normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(-widthT, y, z));

            normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(-widthT, y, z + m_stepSizes[2]));

            normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(-widthT, y + m_stepSizes[1], z + m_stepSizes[2]));

            normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(-widthT, y, z));

            normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(-widthT, y + m_stepSizes[1], z + m_stepSizes[2]));

            normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(-widthT, y + m_stepSizes[1], z));

            // right
            normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(widthT, y + m_stepSizes[1], z));

            normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(widthT, y + m_stepSizes[1], z + m_stepSizes[2]));

            normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(widthT, y, z + m_stepSizes[2]));

            normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(widthT, y + m_stepSizes[1], z));

            normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(widthT, y, z + m_stepSizes[2]));

            normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
            vertices.push_back(glm::vec3(widthT, y, z));
        }
    }

    m_vertexCount = vertices.size();

    m_vao.create();
    m_vao.bind();

    m_vertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertices.create();
    m_vertices.bind();

    m_vertices.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertices.allocate(vertices.data(), vertices.size() * sizeof(glm::vec3));

    program.enableAttributeArray("in_vertex");
    program.setAttributeBuffer("in_vertex", GL_FLOAT, 0, 3, 3 * sizeof(float));

    m_normals = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_normals.create();
    m_normals.bind();

    m_normals.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_normals.allocate(normals.data(), normals.size() * sizeof(glm::vec3));

    program.enableAttributeArray("in_normal");
    program.setAttributeBuffer("in_normal", GL_FLOAT, 0, 3, 3 * sizeof(float));
}

void Box::draw(OpenGLFunctions &functions)
{
    draw(GL_TRIANGLES, functions);
}

void Box::draw(const GLenum mode, OpenGLFunctions &functions)
{
    m_vao.bind();

    functions.glDrawArrays(mode, 0, m_vertexCount);
}