// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 2
//                     - Aufgabe 6
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

#include "exercise6.h"

#include <Qt>
#include <QTimer>
#include <iostream>

Exercise6::Exercise6()
    : Painter()
    , m_tessellationDepth(0)
    , m_update(false)
    , m_timer(0)
{
    // Create timer
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout(void)), this, SLOT(onTimer(void)));

    // Hinweis: Zum Debuggen kann setSingleShot(true) gesetzt werden, um nur 1x tessellate() aufzurufen
    m_timer->setSingleShot(true);
    m_timer->start(1000);
}

Exercise6::~Exercise6()
{
    // Destroy timer
    delete m_timer;
}

bool Exercise6::initialize()
{    
    if(!Painter::initialize())
        return false;

    m_triangles.push_back(Triangle(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f));
    m_triangles.push_back(Triangle(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));

    m_currentBuffer = &m_triangles;

    m_vao.bind();
    transferGeometryToGPU();
    m_vao.release();


    return true;
}

void Exercise6::transferGeometryToGPU()
{    
    m_vertices.bind();
    m_vertices.allocate(m_currentBuffer->data(), Triangle::memorySize() * m_currentBuffer->size());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
}

void Exercise6::tessellate(const Triangle &triangle, std::vector<Triangle> &triangleBuffer)
{
    //////////////////////////////////////////////////
    // TODO: Aufgabe 6a) - tessellieren
    //////////////////////////////////////////////////

    // Tessellieren Sie das gegebene Dreieck triangle:
    // Vertex 0 => triangle.x0, triangle.y0
    // Vertex 1 => triangle.x1, triangle.y1
    // Vertex 2 => triangle.x2, triangle.y2
    // Nutzen Sie triangleBuffer.push_back() um dem Ausgabeparameter triangleBuffer
    // die neuen Dreiecke hinzuzufuegen.
    // Beachten Sie, dass f?r die korrekte Darstellung der (neuen) Dreiecke die Vertices
    // im Uhrzeigersinn orientiert sein muessen.
    //
    // Hinweis: Mit der Taste <w> kann zwischen Wireframe und normalem Dreiecksrendering umgeschaltet werden.
    //          Damit kann das Ergebnis der Tessellation einfacher Ueberprueft werden.

    float array[4] = {triangle.x0 - triangle.x2,triangle.x1 - triangle.x2, triangle.y0 - triangle.y2, triangle.y1 - triangle.y2};
    std::cout << "11:" << array[0] << " 12:" << array[1] << " 21:" << array[2] << " 22:" << array[3] << " ";

    QMatrix2x2 t = QMatrix2x2(array);

    float halfpoint1[2] = {0.5,0.5};
    float halfpoint2[2] = {0.5,0};
    float halfpoint3[2] = {0, 0.5};
    float r3[2] = {triangle.x2, triangle.y2};
    QGenericMatrix<1,2,float> x0x1 = (t * QGenericMatrix<1,2,float>(halfpoint1)) + QGenericMatrix<1,2,float>(r3);
    QGenericMatrix<1,2,float> x0x2 = (t * QGenericMatrix<1,2,float>(halfpoint2))+ QGenericMatrix<1,2,float>(r3);
    QGenericMatrix<1,2,float> x1x2 = (t * QGenericMatrix<1,2,float>(halfpoint3)) + QGenericMatrix<1,2,float>(r3);

    std::cout << x0x1(0,0) << " " << x0x1(1,0) << std::endl;

    Triangle outputTriangle1 = Triangle(triangle.x0,triangle.y0, x0x1(0,0),x0x1(1,0),x0x2(0,0), x0x2(1,0));
    Triangle outputTriangle2 = Triangle(x0x1(0,0),x0x1(1,0), triangle.x1, triangle.y1, x1x2(0,0),x1x2(1,0));
    Triangle outputTriangle3 = Triangle(x1x2(0,0), x1x2(1,0), x0x2(0,0),x0x2(1,0), triangle.x2,triangle.y2);
    Triangle outputTriangle4 = Triangle(x0x2(0,0), x0x2(1,0), x0x1(0,0),x0x1(1,0), x1x2(0,0),x1x2(1,0));

    triangleBuffer.push_back(outputTriangle1);
    triangleBuffer.push_back(outputTriangle2);
    triangleBuffer.push_back(outputTriangle3);
    triangleBuffer.push_back(outputTriangle4);

}

void Exercise6::render()
{
    //////////////////////////////////////////////////
    // TODO: Aufgabe 6a) - drawTriangles
    //
    // Hinweis: Hier kann neben der L?sung der Aufgabe auch mit den beiden Parametern von drawTriangles experimentiert werden, um das verhalten von OpenGL zu erkunden.
    //////////////////////////////////////////////////
    
    // Nutzen Sie m_currentBuffer und drawTriangles().
    drawTriangles(0, m_currentBuffer->size());
}

void Exercise6::drawTriangles(int start, int count)
{
    m_vao.bind();

    if(m_update)
    {
        transferGeometryToGPU();
        m_update = false;
    }

    glDrawArrays(GL_TRIANGLES, start * 3, count * 3);

    m_vao.release();
}

void Exercise6::onTimer()
{
    //std::cout << "onTimer()" << std::endl;

    if(m_tessellationDepth == 0)
    {
        m_currentBuffer = &m_triangles;
        m_helperBuffer = &m_trianglesRefined;
    }

    for(int i = 0; i < m_currentBuffer->size(); i++)
        tessellate((*m_currentBuffer)[i], (*m_helperBuffer));

    std::vector<Triangle>* tmp = m_currentBuffer;
    m_currentBuffer = m_helperBuffer;
    m_helperBuffer = tmp;
    m_helperBuffer->clear();

    m_update = true;

    // Count tesselation depth from 0 to 8 and restart
    ++m_tessellationDepth;

    if(m_tessellationDepth >= 8 )
    {
        m_tessellationDepth = 0;

        m_triangles.clear();
        m_trianglesRefined.clear();
        m_triangles.push_back(Triangle(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f));
        m_triangles.push_back(Triangle(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));

        m_update = true;
    }
}
