#pragma once

#include "util/imageview.h"

class QPainter;
class QTimer;

class Exercise4b : public ImageView
{
    Q_OBJECT

	public:
        Exercise4b(QWidget *parent = NULL);
        ~Exercise4b();

	protected:
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

    protected slots:
        /**
        *  @brief
        *    Called when the timer has fired
        */
        void onTimer();

	protected:
        QTimer              *mTimer;        /**< Timer */
        int                  mCurrentLevel;
        int                  mCurrentDir;

};
