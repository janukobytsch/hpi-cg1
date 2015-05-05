#pragma once

//
// Qt
//
#include <QGraphicsView>

class Exercise5 : public QGraphicsView
{
    Q_OBJECT

	public:
        Exercise5(QWidget *parent = NULL);
        ~Exercise5();

	protected:
		/**
		*  @brief
		*    Called when widget is resized
		*
		*  @param event
		*    Resize event
		*/
        virtual void resizeEvent(QResizeEvent *event);

		/**
		*  @brief
		*    Called when mouse button is released
		*
		*  @param event
		*    Mouse event
		*/
        virtual void mouseReleaseEvent(QMouseEvent *event);

		/**
		*  @brief
		*    Render metaballs
		*/
	    void renderMetaballs();
        QColor interpolateLinear(QColor const &start, QColor const &end, float const p);

	protected:
        QGraphicsScene mScene;       /**< Graphics scene */
        QGraphicsPixmapItem*mImageItem;   /**< Graphics image item */
        QVector<QPoint> mMetaballs;   /**< List of metaballs */
};
