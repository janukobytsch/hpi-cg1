#pragma once

// Qt
#include <QMainWindow>

class QSlider; 
class AbstractExercise;

class MainWindow : public QMainWindow
{
    Q_OBJECT

//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Constructor
	*
	*  @param parent
	*    Parent widget (can be NULL)
	*  @param flags
	*    QT flags
	*/
    MainWindow(QWidget *parent = NULL, QFlag flags = 0);

	/**
	*  @brief
	*    Destructor
	*/
    ~MainWindow();
  
public slots:
    void handleSliderInput(const int value);

signals:
    void animationFrameChanged(const float animationFrame);
    void keyPressed(QKeyEvent *event);

protected:
    QSlider *m_inputSlider;

    virtual void keyPressEvent(QKeyEvent *event);
};
