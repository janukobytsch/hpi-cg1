// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
// 
// Sommersemester 2015 - Aufgabenblatt 1
//                     - Aufgabe 4a
//
//
//
// ======================================

#include "exercise4a.h"
#include "stable.h"

//
// Qt
//
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

using namespace Qt;

//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
const float maxAbsSquare = 4.0f;
const int maxIterations  = 100;

//[-------------------------------------------------------]
//[ Helper functions                                      ]
//[-------------------------------------------------------]
int computeIterations(float cx, float cy)
{
    float absSquare = 0;
	int iterationCount = 0;

	float x = 0, y = 0;

    //////////////////////////////////////////////////////////////////////////
    // TODO: Compute number of iterations, stored in the variable iterationsCount
    //////////////////////////////////////////////////////////////////////////

    // ...

	return iterationCount;
}

// You can use it or customize it
QColor chooseColor(int value, int maxValue)
{
	float p = (float)value/maxValue;
	int i = (int)(299 * std::min(std::max(p, 0.0f), 1.0f));
	int k = i / 60;
	float m = 1.0f * (i % 60) / 59.0f;
	switch (k)
	{
		case 0: return QColor(255, m*255, 0);
		case 1: return QColor((1-m)*255, 255, 0);
		case 2: return QColor(0, 255, m);
		case 3: return QColor(m*255, (1-m)*255, 255);
		case 4: return QColor((1-m)*255, 0, 255);
	}

	return QColor(0, 0, 0);
}

//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]

Exercise4a::Exercise4a(QWidget *parent) :
    ImageView(parent)
{
    // Render mandelbrot set
    renderMandelbrot();
}

Exercise4a::~Exercise4a()
{
    //
}

//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Render mandelbrot
*/
void Exercise4a::renderMandelbrot()
{
    int width = 800, height = 600;
    QPixmap mandelbrot(width, height);
    QPainter painter(&mandelbrot);

    float cx, cy;
    QPoint origin(600, 250);
    float scaleX = 3.0f, scaleY = 3.0f;
    int iterationCount;

    for (int x=0; x<width; x++)
    {
        cx = ((float)(x - origin.x()) / width)  * scaleX;

        for (int y=0; y<height; y++)
		{
            cy = ((float)(y - origin.y()) / height) * scaleY;

            // Compute number of iterations
            iterationCount = computeIterations(cx, cy);

            // Get color
            QColor color = chooseColor(iterationCount, maxIterations);

            // Draw pixel
            painter.setPen(color);
            painter.drawPoint(x, y);
        }
    }

    this->setImage(mandelbrot.toImage());
}
