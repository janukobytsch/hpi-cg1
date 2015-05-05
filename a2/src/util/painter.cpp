#include "painter.h"

#include <cassert>

#include "fileassociatedshader.h"
#include "camera.h"
#include "canvas.h"

Painter::Painter()
: m_camera(0)
, m_vertices(QOpenGLBuffer::VertexBuffer)
, m_interpolationMode(0)
{
}

Painter::~Painter()
{
    delete m_program;
    qDeleteAll(m_shaders);
}

bool Painter::initialize()
{
    initializeOpenGLFunctions();

    glClearColor(1.f, 1.f, 1.f, 0.f);

    m_transform.scale(2.f, 0.16f, 2.f);
    m_transform.translate(-.5f, 0.f, -.5f);

    // prepare vao and vbo
    m_vao.create();
    m_vao.bind();

    m_vertices.create();
    m_vertices.setUsagePattern(QOpenGLBuffer::StaticDraw);
    glEnableVertexAttribArray(0);

    m_vao.release();    

    // create Texture
    m_height = getOrCreateTexture("data/height.png");

    // create shader program
    m_program = createBasicShaderProgram("data/exercise6.vert", "data/exercise6.frag");

    return true;
}

// ToDo: this should be an extra asset class (similar to file associated shaders..)

GLuint Painter::getOrCreateTexture(const QString &fileName)
{
    QImage image(fileName);
    if (image.isNull())
    {
        qDebug() << "Loading image from" << fileName << "failed.";
        return -1;
    }

    image = image.convertToFormat(QImage::Format_ARGB32);

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glGenerateMipmap(GL_TEXTURE_2D);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height()
        , 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    return texture;
}

QOpenGLShaderProgram* Painter::createBasicShaderProgram(const QString &vertexShaderFileName, const QString &fragmentShaderFileName)
{
    QOpenGLShaderProgram * program = new QOpenGLShaderProgram();

    m_shaders << FileAssociatedShader::getOrCreate(
        QOpenGLShader::Vertex, vertexShaderFileName, *program);
    m_shaders << FileAssociatedShader::getOrCreate(
        QOpenGLShader::Fragment, fragmentShaderFileName, *program);
    program->bindAttributeLocation("position", 0);
    program->link();

    return program;
}

void Painter::resize(int width, int height)
{
    glViewport(0, 0, width, height);

    if (m_program->isLinked())
    {
        m_program->bind();
        m_program->setUniformValue("transform", camera()->viewProjection() * m_transform);
        m_program->release();
    }
}

void Painter::update()
{
    update(QList<QOpenGLShaderProgram *>() << m_program);
}

void Painter::update(const QList<QOpenGLShaderProgram *> & programs)
{
    if(programs.contains(m_program) && m_program->isLinked())
    {
        m_program->bind();

        m_program->setUniformValue("transform"
            , camera()->viewProjection() * m_transform);

        m_program->setUniformValue("interpolationMode", m_interpolationMode);

        m_program->setUniformValue("heightMap", 0);

        m_program->release();
    }
}

bool Painter::onKeyPressed(int keyCode)
{
    if (keyCode == Qt::Key_1) {
        m_interpolationMode = 0;
        return true;
    } else if (keyCode == Qt::Key_2) {
        m_interpolationMode = 1;
        return true;
    } else if (keyCode == Qt::Key_3) {
        m_interpolationMode = 2;
        return true;
    }

    // Do not repaint
    return false;
}

void Painter::paint()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(m_program->isLinked())
    {
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_height);

        if(m_mode == WIREFRAME)
        {
            glLineWidth(2.f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        m_program->bind();
        render();
        m_program->release();
            
        if(m_mode == WIREFRAME)
        {
            glLineWidth(1.f);    
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glDisable(GL_CULL_FACE);

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}
