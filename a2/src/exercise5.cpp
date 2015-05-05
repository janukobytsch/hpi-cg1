// ======================================
// 3D Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 2
//                     - Aufgabe 5
//
// Diese Datei bearbeiten.
//
// Bearbeiter
// Matr.-Nr: xxxxx
// Matr.-Nr: xxxxx
//
// ======================================

#include "exercise5.h"

//
// Qt
//
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QMouseEvent>

using namespace Qt;

Exercise5::Exercise5(QWidget *parent) :
    QGraphicsView(parent)
{
    // Draw a deadmau5 with metaballs
    mMetaballs.push_back(QPoint(250, 200));
    mMetaballs.push_back(QPoint(250, 200));
    mMetaballs.push_back(QPoint(250, 200));
    mMetaballs.push_back(QPoint(350, 200));
    mMetaballs.push_back(QPoint(350, 200));
    mMetaballs.push_back(QPoint(350, 200));
    mMetaballs.push_back(QPoint(300, 280));
    mMetaballs.push_back(QPoint(300, 280));
    mMetaballs.push_back(QPoint(300, 280));
    mMetaballs.push_back(QPoint(300, 280));
    mMetaballs.push_back(QPoint(300, 280));

    // Set graphics scene
    setScene(&mScene);

    // Add image item
    QPixmap image;
    mImageItem = mScene.addPixmap(image);

    // Render metaballs
    renderMetaballs();
}

Exercise5::~Exercise5()
{
}

/**
*  @brief
*    Called when widget is resized
*/
void Exercise5::resizeEvent(QResizeEvent *event)
{
    // Resize graphics scene according to widget size
    mScene.setSceneRect(contentsRect());

    // Update metaballs
    renderMetaballs();

    // Call base implementation
    QWidget::resizeEvent(event);
}

/**
*  @brief
*    Called when mouse button is released
*/
void Exercise5::mouseReleaseEvent(QMouseEvent *event)
{
    // Left mouse button
    if (event->button() == Qt::LeftButton) {
        float x = event->x();
        float y = event->y();
        mMetaballs.push_back(QPoint(x, y));
    }

    // Right mouse button
    else if (event->button() == Qt::RightButton) {
        mMetaballs.clear();
    }

    // Update metaballs
    renderMetaballs();
}

QColor Exercise5::interpolateLinear(const QColor &start, const QColor &end, const float p) {
    if (p <= 0) {
        return start;
    }
    if (p >= 1) {
        return end;
    }

    float redF = (1 - p) * start.redF() + p * end.redF();
    float greenF = (1 - p) * start.greenF() + p * end.greenF();
    float blueF = (1 - p) * start.blueF() + p * end.blueF();

    return QColor::fromRgbF(redF, greenF, blueF, 1.0f);
}

/**
*  @brief
*    Render metaballs
*/
void Exercise5::renderMetaballs()
{
    static const float s = 3.2f;
    static const float e = 1.5f; // glow environment
    static const float d = 0.8f; // separate environment for inner glow
    static const QColor outerColor = QColor(0, 0, 0);
    static const QColor glowColor = QColor(200, 50, 50);
    static const QColor innerColor = QColor(255, 255, 255);

    // Get image size
    int w = contentsRect().width();
    int h = contentsRect().height();

    // Create temporary map
    std::vector<float> densityMap = std::vector<float>(w*h, 0.0f);

    // Draw image
    QPixmap image(w, h);
    QPainter painter(&image);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int pixelIndex = y * w + x;

            // Iterate over list of metaballs
            for (int i = 0; i< mMetaballs.size(); i++)
            {
                float mass = 1000.0f;

                QPoint metaball = mMetaballs.at(i);
                float density = mass / (pow((x - metaball.x()), 2) + pow((y - metaball.y()),2));

                densityMap[pixelIndex] += density;
            }

            // Determine interpolation
            float p;
            if (densityMap[pixelIndex] > s) {
                p = (densityMap[pixelIndex] - s) / d;
                painter.setPen(interpolateLinear(glowColor, innerColor, p));
            } else {
                p = (s - densityMap[pixelIndex]) / e;
                painter.setPen(interpolateLinear(glowColor, outerColor, p));
            }

            // Draw pixel
            painter.drawPoint(QPoint(x, y));
        }
    }

    // Update image
    mImageItem->setPixmap(image);
}
