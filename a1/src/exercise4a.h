#pragma once

#include "util/imageview.h"

//
// Qt
//
#include <QGraphicsView>
#include <QPixmap>

class Exercise4a : public ImageView
{
    Q_OBJECT

	public:
        Exercise4a(QWidget *parent = NULL);
        ~Exercise4a();

	protected:
        /**
        *  @brief
        *    Render mandelbrot
        */
        void renderMandelbrot();

		/**
		*  @brief
		*    Draw function recursively by using a quadtree-method
		*
		*  @param x
		*    X coordinate (upper-left corner)
		*  @param y
		*    Y coordinate (upper-left corner)
		*  @param w
		*    Width
		*  @param h
		*    Height
		*  @param level
		*    Recursion level (0: terminate recursion)
		*/
        void drawRecursive(QPainter &painter, int x, int y, int w, int h, int level);
};
