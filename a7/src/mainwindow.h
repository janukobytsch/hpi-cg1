#pragma once

// Qt
#include <QMainWindow>

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

signals:
    void keyPressed(QKeyEvent *event);

protected slots:
    void setCurrentStatusText(int index);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

    std::vector<QString> m_hints;
    QStatusBar *m_statusBar;
};
