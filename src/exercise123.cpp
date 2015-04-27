// ======================================
// 3D-Computergrafik
// moodle.hpi3d.de
// ======================================
//
// Sommersemester 2015 - Aufgabenblatt 1
//                     - Aufgabe 1,2,3
//
//
//
// ======================================

#include "exercise123.h"

//
// std
//
#include <math.h>
#include <algorithm>
#include <iostream>

//
// Qt
//
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QtMath>
#include <QDebug>

using namespace Qt;

//[-------------------------------------------------------]
//[ Helper functions                                      ]
//[-------------------------------------------------------]
// Get pixel at position
QColor Exercise123::getPixel(const QImage &image, int x, int y)
{
    return QColor(image.pixel(qBound(0, x, image.width()-1), qBound(0, y, image.height() - 1)));
}

// clamp color components to [0.0, 1.0]
void Exercise123::clampColor(float &r, float &g, float &b)
{
    r = qBound(0.0f, r, 1.0f);
    g = qBound(0.0f, g, 1.0f);
    b = qBound(0.0f, b, 1.0f);
}

// return grayscale
float Exercise123::getGrayColor(const QColor &color)
{
    float gray = 0.0f;

    float wr = 0.299f;
    float wg = 0.587f;
    float wb = 0.114f;

    gray = wr * color.redF()
        + wg * color.greenF()
        + wb * color.blueF();

    return gray;
}

// return grayscale
QColor Exercise123::getInvertColor(const QColor &color)
{
    QColor invert;

    float r = 1.0f - color.redF();
    float g = 1.0f - color.greenF();
    float b = 1.0f - color.blueF();

    invert = QColor::fromRgbF(r, g, b);

    return invert;
}

bool Exercise123::inBounds(const QImage &image, int x, int y) {
    if (x < 0 || y < 0 || x > image.width()-1 || y > image.height()-1) {
        return false;
    }
    return true;
}

float Exercise123::getNearestFloat(const float &ref, const float palette[], const int &paletteSize)
{
    int nearestIndex = 0;
    float minDistance = 1.0f;

    for (int i = 0; i < paletteSize; i++) {
        float distance = ref - palette[i];
        float distanceSquared = distance * distance;
        if (distanceSquared < minDistance) {
            nearestIndex = i;
            minDistance = distanceSquared;
        }
    }

    return palette[nearestIndex];
}

QColor Exercise123::getDiffusedColor(const QColor &color, const float &weight, const float &error)
{
    float r = color.redF() + weight * error;
    float g = color.greenF() + weight * error;
    float b = color.blueF() + weight * error;

    clampColor(r, g, b);
    return QColor::fromRgbF(r, g, b);
}

void Exercise123::diffusePixel(QImage &image, int x, int y, const float &weight, const float &error) {
    if (!inBounds(image, x, y)) {
        return;
    }

    QColor pixel = getPixel(image, x, y);
    QColor diffusedPixel = getDiffusedColor(pixel, weight, error);
    image.setPixel(x, y, diffusedPixel.rgb());
}

//[-------------------------------------------------------]
//[ Filter functions                                      ]
//[-------------------------------------------------------]

QColor Exercise123::convolute(const QImage &image, int x, int y, const int kernel[], int kernelSize = 3, bool normalize = false)
{
    float r = 0;
    float g = 0;
    float b = 0;
    float total = 0;

    int imageWidth = image.width();
    int imageHeight = image.height();

    for (int filterY = -kernelSize/2; filterY <= kernelSize/2; filterY++) {
        for (int filterX = -kernelSize/2; filterX <= kernelSize/2; filterX++) {
            // int currentY = (y + filterY) % imageHeight;
            // int currentX = (x + filterX) % imageWidth;
            int currentY = (y + filterY > imageHeight) ? imageHeight : std::max(0, y + filterY);
            int currentX = (x + filterX > imageWidth) ? imageWidth : std::max(0, x + filterX);

            int kernelOffsetY = (filterY + kernelSize/2) * kernelSize;
            int kernelOffsetX = filterX + kernelSize/2;
            int kernelValue = kernel[kernelOffsetY + kernelOffsetX];

            QColor pixel = getPixel(image, currentX, currentY);
            r += pixel.redF()*kernelValue;
            g += pixel.greenF()*kernelValue;
            b += pixel.blueF()*kernelValue;

            total += abs(kernelValue);
        }
    }

    if (total > 0 && normalize) {
        r = r/total;
        b = b/total;
        g = g/total;
    }

    clampColor(r, g, b);

    return QColor::fromRgbF(r, g, b);
}

QColor Exercise123::getSharpenColor(const QImage &image, int x, int y)
{
    int kernel[] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
    int kernelSize = 3;
    int normalize = false;
    QColor convolution = convolute(image, x, y, kernel, kernelSize, normalize);

    return convolution;
}



QColor Exercise123::getGaussColor(const QImage &image, int x, int y)
{
    int kernel[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
    int kernelSize = 3;
    int normalize = true;
    QColor convolution = convolute(image, x, y, kernel, kernelSize, normalize);

    return convolution;
}

QColor Exercise123::getSobelColor(const QImage &image, int x, int y)
{
    int kernelX[] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
    int kernelY[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    int kernelSize = 3;
    int normalize = false;
    QColor convolution = convolute(image, x, y, kernelX, kernelSize, normalize);

    // TODO apply y-kernel

    float c = getGrayColor(convolution);
    c = qBound(0.0f, c, 1.0f);

    return QColor::fromRgbF(c, c, c);
}

QColor Exercise123::getMeanColorDynamicSize(const QImage &image, int x, int y, int kernelSize)
{
    int kernelValue = 1;

    // initialize dynamic kernel with values
    int length = kernelSize*kernelSize;
    int *kernel = new int[length];
    std::fill(kernel, kernel + length, kernelValue);

    int normalize = true;
    QColor convolution = convolute(image, x, y, kernel, kernelSize, normalize);

    return convolution;
}

//getDitheringColor can work directly on image - use it
QColor Exercise123::getDitheringColor(QImage &image, int x, int y)
{
    QColor oldPixel = getPixel(image, x, y);
    float oldGray = getGrayColor(oldPixel);

    float palette[] = {0.0f, 0.33f, 0.66f, 1.0f};
    int paletteSize = 4;

    float quantizedGray = getNearestFloat(oldGray, palette, paletteSize);
    float error = oldGray - quantizedGray;

    diffusePixel(image, x+1, y, 7.0f/16, error);
    diffusePixel(image, x-1, y+1, 3.0f/16, error);
    diffusePixel(image, x, y+1, 5.0f/16, error);
    diffusePixel(image, x+1, y+1, 1.0f/16, error);

    return QColor::fromRgbF(quantizedGray, quantizedGray, quantizedGray);
}

Exercise123::Exercise123(Filter_Type type, QWidget *parent) :
    ImageView(parent),
    m_type(type)
{
    // load image
    const QImage image("image1.png");
    QImage filterImage(image);

    if(type == ORIGINAL)
    {
        this->setImage(image);
    }
    else
    {
        // filter image
        for (int y=0; y<image.height(); y++)
        {
            for (int x=0; x<image.width(); x++)
            {
                QColor filteredColor;

                // choose filter
                if(type == LAPLACE)
                {
                    filteredColor = getSharpenColor(image, x, y);
                }
                else if(type == GAUSS)
                {
                    filteredColor = getGaussColor(image, x, y);
                }
                else if(type == SOBEL)
                {
                    filteredColor = getSobelColor(image, x, y);
                }
                else if(type == MEANFILTER)
                {
                    filteredColor = getMeanColorDynamicSize(image, x, y, 5);
                }
                else if(type == GRAYSCALE)
                {
                    float luminance = getGrayColor(getPixel(image, x, y));
                    filteredColor = QColor::fromRgbF(luminance, luminance, luminance);
                }
                else if(type == INVERT)
                {
                    filteredColor = getInvertColor(getPixel(image, x, y));
                }
                else if(type == DITHERED)
                {
                    filteredColor = getDitheringColor(filterImage, x, y);
                }
                else
                {
                    // ORIGINAL -> do nothing
                }

                // draw pixel
                filterImage.setPixel(x,y,filteredColor.rgb());
            }
        }

        this->setImage(filterImage);
        this->zoomOut();
    }
}

Exercise123::~Exercise123()
{
}
