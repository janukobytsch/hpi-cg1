#pragma once

#include "util/imageview.h"

class Exercise123 : public ImageView
{
    Q_OBJECT

	public:
        enum Filter_Type
        {
            ORIGINAL,
            GRAYSCALE,
            INVERT,
            LAPLACE,
            GAUSS,
            SOBEL,
            MEANFILTER,
            DITHERED
        };

        Exercise123(Filter_Type type = ORIGINAL, QWidget *parent = NULL);
        ~Exercise123();

        // filter functions
        QColor convolute(const QImage &image, int x, int y, const int kernel[], int kernelSize, bool normalize);
        QColor getSharpenColor(const QImage &image, int x, int y);
        QColor getGaussColor(const QImage &image, int x, int y);
        QColor getMeanColorDynamicSize(const QImage &image, int x, int y, int kernelSize);
        QColor getSobelColor(const QImage &image, int x, int y);
        QColor getDitheringColor(QImage &image, int x, int y);

        // helper functions
        QColor getPixel(const QImage &image, int x, int y);
        void clampColor(float &r, float &g, float &b);
        float getGrayColor(const QColor &color);
        QColor getInvertColor(const QColor &color);
        bool inBounds(const QImage &image, int x, int y);
        float getNearestFloat(const float &ref, const float palette[], const int &paletteSize);
        QColor getDiffusedColor(const QColor &color, const float &weight, const float &error);
        void diffusePixel(QImage &image, int x, int y, const float &weight, const float &error);

    private:
        Filter_Type m_type;
};
